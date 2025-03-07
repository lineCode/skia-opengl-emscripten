/*
 * Copyright (C) 2010 Google, Inc. All Rights Reserved.
 * Copyright (C) 2011, 2014 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY GOOGLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL GOOGLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/core/html/parser/html_tree_builder.h"

#include <memory>

#include "base/macros.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/document_fragment.h"
#include "third_party/blink/renderer/core/dom/element_traversal.h"
#include "third_party/blink/renderer/core/frame/use_counter.h"
#include "third_party/blink/renderer/core/html/forms/html_form_control_element.h"
#include "third_party/blink/renderer/core/html/forms/html_form_element.h"
#include "third_party/blink/renderer/core/html/html_template_element.h"
#include "third_party/blink/renderer/core/html/parser/atomic_html_token.h"
#include "third_party/blink/renderer/core/html/parser/html_document_parser.h"
#include "third_party/blink/renderer/core/html/parser/html_parser_idioms.h"
#include "third_party/blink/renderer/core/html/parser/html_stack_item.h"
#include "third_party/blink/renderer/core/html/parser/html_token.h"
#include "third_party/blink/renderer/core/html/parser/html_tokenizer.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/core/mathml_names.h"
#include "third_party/blink/renderer/core/svg_names.h"
#include "third_party/blink/renderer/core/xlink_names.h"
#include "third_party/blink/renderer/core/xml_names.h"
#include "third_party/blink/renderer/core/xmlns_names.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include "third_party/blink/renderer/platform/bindings/runtime_call_stats.h"
#include "third_party/blink/renderer/platform/bindings/v8_per_isolate_data.h"
#include "third_party/blink/renderer/platform/text/platform_locale.h"
#include "third_party/blink/renderer/platform/wtf/text/character_names.h"

namespace blink {

using namespace html_names;

namespace {

inline bool IsHTMLSpaceOrReplacementCharacter(UChar character) {
  return IsHTMLSpace<UChar>(character) || character == kReplacementCharacter;
}
}

static TextPosition UninitializedPositionValue1() {
  return TextPosition(OrdinalNumber::FromOneBasedInt(-1),
                      OrdinalNumber::First());
}

static inline bool IsAllWhitespace(const StringView& string_view) {
  return string_view.IsAllSpecialCharacters<IsHTMLSpace<UChar>>();
}

static inline bool IsAllWhitespaceOrReplacementCharacters(
    const StringView& string_view) {
  return string_view
      .IsAllSpecialCharacters<IsHTMLSpaceOrReplacementCharacter>();
}

static bool IsNumberedHeaderTag(const AtomicString& tag_name) {
  return tag_name == kH1Tag || tag_name == kH2Tag || tag_name == kH3Tag ||
         tag_name == kH4Tag || tag_name == kH5Tag || tag_name == kH6Tag;
}

static bool IsCaptionColOrColgroupTag(const AtomicString& tag_name) {
  return tag_name == kCaptionTag || tag_name == kColTag ||
         tag_name == kColgroupTag;
}

static bool IsTableCellContextTag(const AtomicString& tag_name) {
  return tag_name == kThTag || tag_name == kTdTag;
}

static bool IsTableBodyContextTag(const AtomicString& tag_name) {
  return tag_name == kTbodyTag || tag_name == kTfootTag ||
         tag_name == kTheadTag;
}

static bool IsNonAnchorNonNobrFormattingTag(const AtomicString& tag_name) {
  return tag_name == kBTag || tag_name == kBigTag || tag_name == kCodeTag ||
         tag_name == kEmTag || tag_name == kFontTag || tag_name == kITag ||
         tag_name == kSTag || tag_name == kSmallTag || tag_name == kStrikeTag ||
         tag_name == kStrongTag || tag_name == kTtTag || tag_name == kUTag;
}

static bool IsNonAnchorFormattingTag(const AtomicString& tag_name) {
  return tag_name == kNobrTag || IsNonAnchorNonNobrFormattingTag(tag_name);
}

// http://www.whatwg.org/specs/web-apps/current-work/multipage/parsing.html#formatting
static bool IsFormattingTag(const AtomicString& tag_name) {
  return tag_name == kATag || IsNonAnchorFormattingTag(tag_name);
}

class HTMLTreeBuilder::CharacterTokenBuffer {
 public:
  explicit CharacterTokenBuffer(AtomicHTMLToken* token)
      : characters_(token->Characters().Impl()),
        current_(0),
        end_(token->Characters().length()) {
    DCHECK(!IsEmpty());
  }

  explicit CharacterTokenBuffer(const String& characters)
      : characters_(characters.Impl()), current_(0), end_(characters.length()) {
    DCHECK(!IsEmpty());
  }

  ~CharacterTokenBuffer() { DCHECK(IsEmpty()); }

  bool IsEmpty() const { return current_ == end_; }

  void SkipAtMostOneLeadingNewline() {
    DCHECK(!IsEmpty());
    if ((*characters_)[current_] == '\n')
      ++current_;
  }

  void SkipLeadingWhitespace() { SkipLeading<IsHTMLSpace<UChar>>(); }

  StringView TakeLeadingWhitespace() {
    return TakeLeading<IsHTMLSpace<UChar>>();
  }

  void SkipLeadingNonWhitespace() { SkipLeading<IsNotHTMLSpace<UChar>>(); }

  void SkipRemaining() { current_ = end_; }

  StringView TakeRemaining() {
    DCHECK(!IsEmpty());
    unsigned start = current_;
    current_ = end_;
    return StringView(characters_.get(), start, end_ - start);
  }

  void GiveRemainingTo(StringBuilder& recipient) {
    if (characters_->Is8Bit())
      recipient.Append(characters_->Characters8() + current_, end_ - current_);
    else
      recipient.Append(characters_->Characters16() + current_, end_ - current_);
    current_ = end_;
  }

  String TakeRemainingWhitespace() {
    DCHECK(!IsEmpty());
    const unsigned start = current_;
    current_ = end_;  // One way or another, we're taking everything!

    unsigned length = 0;
    for (unsigned i = start; i < end_; ++i) {
      if (IsHTMLSpace<UChar>((*characters_)[i]))
        ++length;
    }
    // Returning the null string when there aren't any whitespace
    // characters is slightly cleaner semantically because we don't want
    // to insert a text node (as opposed to inserting an empty text node).
    if (!length)
      return String();
    if (length == start - end_)  // It's all whitespace.
      return String(characters_->Substring(start, start - end_));

    StringBuilder result;
    result.ReserveCapacity(length);
    for (unsigned i = start; i < end_; ++i) {
      UChar c = (*characters_)[i];
      if (IsHTMLSpace<UChar>(c))
        result.Append(c);
    }

    return result.ToString();
  }

 private:
  template <bool characterPredicate(UChar)>
  void SkipLeading() {
    DCHECK(!IsEmpty());
    while (characterPredicate((*characters_)[current_])) {
      if (++current_ == end_)
        return;
    }
  }

  template <bool characterPredicate(UChar)>
  StringView TakeLeading() {
    DCHECK(!IsEmpty());
    const unsigned start = current_;
    SkipLeading<characterPredicate>();
    return StringView(characters_.get(), start, current_ - start);
  }

  scoped_refptr<StringImpl> characters_;
  unsigned current_;
  unsigned end_;

  DISALLOW_COPY_AND_ASSIGN(CharacterTokenBuffer);
};

HTMLTreeBuilder::HTMLTreeBuilder(HTMLDocumentParser* parser,
                                 Document& document,
                                 ParserContentPolicy parser_content_policy,
                                 const HTMLParserOptions& options)
    : frameset_ok_(true),
      tree_(parser->ReentryPermit(), document, parser_content_policy),
      insertion_mode_(kInitialMode),
      original_insertion_mode_(kInitialMode),
      should_skip_leading_newline_(false),
      parser_(parser),
      script_to_process_start_position_(UninitializedPositionValue1()),
      options_(options) {}

HTMLTreeBuilder::HTMLTreeBuilder(HTMLDocumentParser* parser,
                                 DocumentFragment* fragment,
                                 Element* context_element,
                                 ParserContentPolicy parser_content_policy,
                                 const HTMLParserOptions& options)
    : HTMLTreeBuilder(parser,
                      fragment->GetDocument(),
                      parser_content_policy,
                      options) {
  DCHECK(IsMainThread());
  DCHECK(context_element);
  tree_.InitFragmentParsing(fragment, context_element);
  fragment_context_.Init(fragment, context_element);

  // Steps 4.2-4.6 of the HTML5 Fragment Case parsing algorithm:
  // http://www.whatwg.org/specs/web-apps/current-work/multipage/the-end.html#fragment-case
  // For efficiency, we skip step 4.2 ("Let root be a new html element with no
  // attributes") and instead use the DocumentFragment as a root node.
  tree_.OpenElements()->PushRootNode(HTMLStackItem::Create(
      fragment, HTMLStackItem::kItemForDocumentFragmentNode));

  if (IsHTMLTemplateElement(*context_element))
    template_insertion_modes_.push_back(kTemplateContentsMode);

  ResetInsertionModeAppropriately();
}

HTMLTreeBuilder::~HTMLTreeBuilder() = default;

void HTMLTreeBuilder::FragmentParsingContext::Init(DocumentFragment* fragment,
                                                   Element* context_element) {
  DCHECK(fragment);
  DCHECK(!fragment->HasChildren());
  fragment_ = fragment;
  context_element_stack_item_ = HTMLStackItem::Create(
      context_element, HTMLStackItem::kItemForContextElement);
}

void HTMLTreeBuilder::FragmentParsingContext::Trace(Visitor* visitor) {
  visitor->Trace(fragment_);
  visitor->Trace(context_element_stack_item_);
}

void HTMLTreeBuilder::Trace(Visitor* visitor) {
  visitor->Trace(fragment_context_);
  visitor->Trace(tree_);
  visitor->Trace(parser_);
  visitor->Trace(script_to_process_);
}

void HTMLTreeBuilder::Detach() {
#if DCHECK_IS_ON()
  // This call makes little sense in fragment mode, but for consistency
  // DocumentParser expects Detach() to always be called before it's destroyed.
  is_attached_ = false;
#endif
  // HTMLConstructionSite might be on the callstack when Detach() is called
  // otherwise we'd just call tree_.Clear() here instead.
  tree_.Detach();
}

Element* HTMLTreeBuilder::TakeScriptToProcess(
    TextPosition& script_start_position) {
  DCHECK(script_to_process_);
  DCHECK(!tree_.HasPendingTasks());
  // Unpause ourselves, callers may pause us again when processing the script.
  // The HTML5 spec is written as though scripts are executed inside the tree
  // builder.  We pause the parser to exit the tree builder, and then resume
  // before running scripts.
  script_start_position = script_to_process_start_position_;
  script_to_process_start_position_ = UninitializedPositionValue1();
  return script_to_process_.Release();
}

void HTMLTreeBuilder::ConstructTree(AtomicHTMLToken* token) {
  RUNTIME_CALL_TIMER_SCOPE(V8PerIsolateData::MainThreadIsolate(),
                           RuntimeCallStats::CounterId::kConstructTree);
  if (ShouldProcessTokenInForeignContent(token))
    ProcessTokenInForeignContent(token);
  else
    ProcessToken(token);

  if (parser_->Tokenizer()) {
    bool in_foreign_content = false;
    if (!tree_.IsEmpty()) {
      HTMLStackItem* adjusted_current_node = AdjustedCurrentStackItem();
      in_foreign_content =
          !adjusted_current_node->IsInHTMLNamespace() &&
          !HTMLElementStack::IsHTMLIntegrationPoint(adjusted_current_node) &&
          !HTMLElementStack::IsMathMLTextIntegrationPoint(
              adjusted_current_node);
    }

    parser_->Tokenizer()->SetForceNullCharacterReplacement(
        insertion_mode_ == kTextMode || in_foreign_content);
    parser_->Tokenizer()->SetShouldAllowCDATA(in_foreign_content);
  }

  tree_.ExecuteQueuedTasks();
  // We might be detached now.
}

void HTMLTreeBuilder::ProcessToken(AtomicHTMLToken* token) {
  if (token->GetType() == HTMLToken::kCharacter) {
    ProcessCharacter(token);
    return;
  }

  // Any non-character token needs to cause us to flush any pending text
  // immediately. NOTE: flush() can cause any queued tasks to execute, possibly
  // re-entering the parser.
  tree_.Flush(kFlushAlways);
  should_skip_leading_newline_ = false;

  switch (token->GetType()) {
    case HTMLToken::kUninitialized:
    case HTMLToken::kCharacter:
      NOTREACHED();
      break;
    case HTMLToken::DOCTYPE:
      ProcessDoctypeToken(token);
      break;
    case HTMLToken::kStartTag:
      ProcessStartTag(token);
      break;
    case HTMLToken::kEndTag:
      ProcessEndTag(token);
      break;
    case HTMLToken::kComment:
      ProcessComment(token);
      break;
    case HTMLToken::kEndOfFile:
      ProcessEndOfFile(token);
      break;
  }
}

void HTMLTreeBuilder::ProcessDoctypeToken(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::DOCTYPE);
  if (insertion_mode_ == kInitialMode) {
    tree_.InsertDoctype(token);
    SetInsertionMode(kBeforeHTMLMode);
    return;
  }
  if (insertion_mode_ == kInTableTextMode) {
    DefaultForInTableText();
    ProcessDoctypeToken(token);
    return;
  }
  ParseError(token);
}

void HTMLTreeBuilder::ProcessFakeStartTag(const QualifiedName& tag_name,
                                          const Vector<Attribute>& attributes) {
  // FIXME: We'll need a fancier conversion than just "localName" for SVG/MathML
  // tags.
  AtomicHTMLToken fake_token(HTMLToken::kStartTag, tag_name.LocalName(),
                             attributes);
  ProcessStartTag(&fake_token);
}

void HTMLTreeBuilder::ProcessFakeEndTag(const AtomicString& tag_name) {
  AtomicHTMLToken fake_token(HTMLToken::kEndTag, tag_name);
  ProcessEndTag(&fake_token);
}

void HTMLTreeBuilder::ProcessFakeEndTag(const QualifiedName& tag_name) {
  // FIXME: We'll need a fancier conversion than just "localName" for SVG/MathML
  // tags.
  ProcessFakeEndTag(tag_name.LocalName());
}

void HTMLTreeBuilder::ProcessFakePEndTagIfPInButtonScope() {
  if (!tree_.OpenElements()->InButtonScope(kPTag.LocalName()))
    return;
  AtomicHTMLToken end_p(HTMLToken::kEndTag, kPTag.LocalName());
  ProcessEndTag(&end_p);
}

namespace {

bool IsLi(const HTMLStackItem* item) {
  return item->HasTagName(kLiTag);
}

bool IsDdOrDt(const HTMLStackItem* item) {
  return item->HasTagName(kDdTag) || item->HasTagName(kDtTag);
}

}  // namespace

template <bool shouldClose(const HTMLStackItem*)>
void HTMLTreeBuilder::ProcessCloseWhenNestedTag(AtomicHTMLToken* token) {
  frameset_ok_ = false;
  HTMLElementStack::ElementRecord* node_record =
      tree_.OpenElements()->TopRecord();
  while (1) {
    HTMLStackItem* item = node_record->StackItem();
    if (shouldClose(item)) {
      DCHECK(item->IsElementNode());
      ProcessFakeEndTag(item->LocalName());
      break;
    }
    if (item->IsSpecialNode() && !item->HasTagName(kAddressTag) &&
        !item->HasTagName(kDivTag) && !item->HasTagName(kPTag))
      break;
    node_record = node_record->Next();
  }
  ProcessFakePEndTagIfPInButtonScope();
  tree_.InsertHTMLElement(token);
}

typedef HashMap<AtomicString, QualifiedName> PrefixedNameToQualifiedNameMap;

template <typename TableQualifiedName>
static void MapLoweredLocalNameToName(PrefixedNameToQualifiedNameMap* map,
                                      const TableQualifiedName* const* names,
                                      size_t length) {
  for (size_t i = 0; i < length; ++i) {
    const QualifiedName& name = *names[i];
    const AtomicString& local_name = name.LocalName();
    AtomicString lowered_local_name = local_name.LowerASCII();
    if (lowered_local_name != local_name)
      map->insert(lowered_local_name, name);
  }
}

// "Any other start tag" bullet in
// https://html.spec.whatwg.org/C/#parsing-main-inforeign
static void AdjustSVGTagNameCase(AtomicHTMLToken* token) {
  static PrefixedNameToQualifiedNameMap* case_map = nullptr;
  if (!case_map) {
    case_map = new PrefixedNameToQualifiedNameMap;
    std::unique_ptr<const SVGQualifiedName* []> svg_tags = svg_names::GetTags();
    MapLoweredLocalNameToName(case_map, svg_tags.get(), svg_names::kTagsCount);
  }

  const QualifiedName& cased_name = case_map->at(token->GetName());
  if (cased_name.LocalName().IsNull())
    return;
  token->SetName(cased_name.LocalName());
}

template <std::unique_ptr<const QualifiedName* []> getAttrs(), unsigned length>
static void AdjustAttributes(AtomicHTMLToken* token) {
  static PrefixedNameToQualifiedNameMap* case_map = nullptr;
  if (!case_map) {
    case_map = new PrefixedNameToQualifiedNameMap;
    std::unique_ptr<const QualifiedName* []> attrs = getAttrs();
    MapLoweredLocalNameToName(case_map, attrs.get(), length);
  }

  for (auto& token_attribute : token->Attributes()) {
    const QualifiedName& cased_name = case_map->at(token_attribute.LocalName());
    if (!cased_name.LocalName().IsNull())
      token_attribute.ParserSetName(cased_name);
  }
}

// https://html.spec.whatwg.org/C/#adjust-svg-attributes
static void AdjustSVGAttributes(AtomicHTMLToken* token) {
  AdjustAttributes<svg_names::GetAttrs, svg_names::kAttrsCount>(token);
}

// https://html.spec.whatwg.org/C/#adjust-mathml-attributes
static void AdjustMathMLAttributes(AtomicHTMLToken* token) {
  AdjustAttributes<mathml_names::GetAttrs, mathml_names::kAttrsCount>(token);
}

static void AddNamesWithPrefix(PrefixedNameToQualifiedNameMap* map,
                               const AtomicString& prefix,
                               const QualifiedName* const* names,
                               size_t length) {
  for (size_t i = 0; i < length; ++i) {
    const QualifiedName* name = names[i];
    const AtomicString& local_name = name->LocalName();
    AtomicString prefix_colon_local_name = prefix + ':' + local_name;
    QualifiedName name_with_prefix(prefix, local_name, name->NamespaceURI());
    map->insert(prefix_colon_local_name, name_with_prefix);
  }
}

static void AdjustForeignAttributes(AtomicHTMLToken* token) {
  static PrefixedNameToQualifiedNameMap* map = nullptr;
  if (!map) {
    map = new PrefixedNameToQualifiedNameMap;

    std::unique_ptr<const QualifiedName* []> attrs = xlink_names::GetAttrs();
    AddNamesWithPrefix(map, g_xlink_atom, attrs.get(),
                       xlink_names::kAttrsCount);

    std::unique_ptr<const QualifiedName* []> xml_attrs = xml_names::GetAttrs();
    AddNamesWithPrefix(map, g_xml_atom, xml_attrs.get(),
                       xml_names::kAttrsCount);

    map->insert(WTF::g_xmlns_atom, xmlns_names::kXmlnsAttr);
    map->insert("xmlns:xlink", QualifiedName(g_xmlns_atom, g_xlink_atom,
                                             xmlns_names::kNamespaceURI));
  }

  for (unsigned i = 0; i < token->Attributes().size(); ++i) {
    Attribute& token_attribute = token->Attributes().at(i);
    const QualifiedName& name = map->at(token_attribute.LocalName());
    if (!name.LocalName().IsNull())
      token_attribute.ParserSetName(name);
  }
}

void HTMLTreeBuilder::ProcessStartTagForInBody(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kStartTag);
  if (token->GetName() == kHTMLTag) {
    ProcessHtmlStartTagForInBody(token);
    return;
  }
  if (token->GetName() == kBaseTag || token->GetName() == kBasefontTag ||
      token->GetName() == kBgsoundTag || token->GetName() == kCommandTag ||
      token->GetName() == kLinkTag || token->GetName() == kMetaTag ||
      token->GetName() == kNoframesTag || token->GetName() == kScriptTag ||
      token->GetName() == kStyleTag || token->GetName() == kTitleTag ||
      token->GetName() == kTemplateTag) {
    bool did_process = ProcessStartTagForInHead(token);
    DCHECK(did_process);
    return;
  }
  if (token->GetName() == kBodyTag) {
    ParseError(token);
    if (!tree_.OpenElements()->SecondElementIsHTMLBodyElement() ||
        tree_.OpenElements()->HasOnlyOneElement() ||
        tree_.OpenElements()->HasTemplateInHTMLScope()) {
      DCHECK(IsParsingFragmentOrTemplateContents());
      return;
    }
    frameset_ok_ = false;
    tree_.InsertHTMLBodyStartTagInBody(token);
    return;
  }
  if (token->GetName() == kFramesetTag) {
    ParseError(token);
    if (!tree_.OpenElements()->SecondElementIsHTMLBodyElement() ||
        tree_.OpenElements()->HasOnlyOneElement()) {
      DCHECK(IsParsingFragmentOrTemplateContents());
      return;
    }
    if (!frameset_ok_)
      return;
    tree_.OpenElements()->BodyElement()->remove(ASSERT_NO_EXCEPTION);
    tree_.OpenElements()->PopUntil(tree_.OpenElements()->BodyElement());
    tree_.OpenElements()->PopHTMLBodyElement();

    // Note: in the fragment case the root is a DocumentFragment instead of
    // a proper html element which is a quirk in Blink's implementation.
    DCHECK(!IsParsingTemplateContents());
    DCHECK(!IsParsingFragment() ||
           To<DocumentFragment>(tree_.OpenElements()->TopNode()));
    DCHECK(IsParsingFragment() ||
           tree_.OpenElements()->Top() == tree_.OpenElements()->HtmlElement());
    tree_.InsertHTMLElement(token);
    SetInsertionMode(kInFramesetMode);
    return;
  }
  if (token->GetName() == kAddressTag || token->GetName() == kArticleTag ||
      token->GetName() == kAsideTag || token->GetName() == kBlockquoteTag ||
      token->GetName() == kCenterTag || token->GetName() == kDetailsTag ||
      token->GetName() == kDirTag || token->GetName() == kDivTag ||
      token->GetName() == kDlTag || token->GetName() == kFieldsetTag ||
      token->GetName() == kFigcaptionTag || token->GetName() == kFigureTag ||
      token->GetName() == kFooterTag || token->GetName() == kHeaderTag ||
      token->GetName() == kHgroupTag || token->GetName() == kMainTag ||
      token->GetName() == kMenuTag || token->GetName() == kNavTag ||
      token->GetName() == kOlTag || token->GetName() == kPTag ||
      token->GetName() == kSectionTag || token->GetName() == kSummaryTag ||
      token->GetName() == kUlTag) {
    ProcessFakePEndTagIfPInButtonScope();
    tree_.InsertHTMLElement(token);
    return;
  }
  if (IsNumberedHeaderTag(token->GetName())) {
    ProcessFakePEndTagIfPInButtonScope();
    if (tree_.CurrentStackItem()->IsNumberedHeaderElement()) {
      ParseError(token);
      tree_.OpenElements()->Pop();
    }
    tree_.InsertHTMLElement(token);
    return;
  }
  if (token->GetName() == kPreTag || token->GetName() == kListingTag) {
    ProcessFakePEndTagIfPInButtonScope();
    tree_.InsertHTMLElement(token);
    should_skip_leading_newline_ = true;
    frameset_ok_ = false;
    return;
  }
  if (token->GetName() == kFormTag) {
    if (tree_.IsFormElementPointerNonNull() && !IsParsingTemplateContents()) {
      ParseError(token);
      UseCounter::Count(tree_.CurrentNode()->GetDocument(),
                        WebFeature::kHTMLParseErrorNestedForm);
      return;
    }
    ProcessFakePEndTagIfPInButtonScope();
    tree_.InsertHTMLFormElement(token);
    return;
  }
  if (token->GetName() == kLiTag) {
    ProcessCloseWhenNestedTag<IsLi>(token);
    return;
  }
  if (token->GetName() == kDdTag || token->GetName() == kDtTag) {
    ProcessCloseWhenNestedTag<IsDdOrDt>(token);
    return;
  }
  if (token->GetName() == kPlaintextTag) {
    ProcessFakePEndTagIfPInButtonScope();
    tree_.InsertHTMLElement(token);
    if (parser_->Tokenizer())
      parser_->Tokenizer()->SetState(HTMLTokenizer::kPLAINTEXTState);
    return;
  }
  if (token->GetName() == kButtonTag) {
    if (tree_.OpenElements()->InScope(kButtonTag)) {
      ParseError(token);
      ProcessFakeEndTag(kButtonTag);
      ProcessStartTag(token);  // FIXME: Could we just fall through here?
      return;
    }
    tree_.ReconstructTheActiveFormattingElements();
    tree_.InsertHTMLElement(token);
    frameset_ok_ = false;
    return;
  }
  if (token->GetName() == kATag) {
    Element* active_a_tag =
        tree_.ActiveFormattingElements()->ClosestElementInScopeWithName(
            kATag.LocalName());
    if (active_a_tag) {
      ParseError(token);
      ProcessFakeEndTag(kATag);
      tree_.ActiveFormattingElements()->Remove(active_a_tag);
      if (tree_.OpenElements()->Contains(active_a_tag))
        tree_.OpenElements()->Remove(active_a_tag);
    }
    tree_.ReconstructTheActiveFormattingElements();
    tree_.InsertFormattingElement(token);
    return;
  }
  if (IsNonAnchorNonNobrFormattingTag(token->GetName())) {
    tree_.ReconstructTheActiveFormattingElements();
    tree_.InsertFormattingElement(token);
    return;
  }
  if (token->GetName() == kNobrTag) {
    tree_.ReconstructTheActiveFormattingElements();
    if (tree_.OpenElements()->InScope(kNobrTag)) {
      ParseError(token);
      ProcessFakeEndTag(kNobrTag);
      tree_.ReconstructTheActiveFormattingElements();
    }
    tree_.InsertFormattingElement(token);
    return;
  }
  if (token->GetName() == kAppletTag || token->GetName() == kEmbedTag ||
      token->GetName() == kObjectTag) {
    if (!PluginContentIsAllowed(tree_.GetParserContentPolicy()))
      return;
  }
  if (token->GetName() == kAppletTag || token->GetName() == kMarqueeTag ||
      token->GetName() == kObjectTag) {
    tree_.ReconstructTheActiveFormattingElements();
    tree_.InsertHTMLElement(token);
    tree_.ActiveFormattingElements()->AppendMarker();
    frameset_ok_ = false;
    return;
  }
  if (token->GetName() == kTableTag) {
    if (!tree_.InQuirksMode() && tree_.OpenElements()->InButtonScope(kPTag))
      ProcessFakeEndTag(kPTag);
    tree_.InsertHTMLElement(token);
    frameset_ok_ = false;
    SetInsertionMode(kInTableMode);
    return;
  }
  if (token->GetName() == kImageTag) {
    ParseError(token);
    // Apparently we're not supposed to ask.
    token->SetName(kImgTag.LocalName());
    // Note the fall through to the kImgTag handling below!
  }
  if (token->GetName() == kAreaTag || token->GetName() == kBrTag ||
      token->GetName() == kEmbedTag || token->GetName() == kImgTag ||
      token->GetName() == kKeygenTag || token->GetName() == kWbrTag) {
    tree_.ReconstructTheActiveFormattingElements();
    tree_.InsertSelfClosingHTMLElementDestroyingToken(token);
    frameset_ok_ = false;
    return;
  }
  if (token->GetName() == kInputTag) {
    // Per spec https://html.spec.whatwg.org/C/#parsing-main-inbody,
    // section "A start tag whose tag name is "input""

    Attribute* type_attribute = token->GetAttributeItem(kTypeAttr);
    bool disable_frameset =
        !type_attribute ||
        !DeprecatedEqualIgnoringCase(type_attribute->Value(), "hidden");

    tree_.ReconstructTheActiveFormattingElements();
    tree_.InsertSelfClosingHTMLElementDestroyingToken(token);

    if (disable_frameset)
      frameset_ok_ = false;
    return;
  }
  if (token->GetName() == kParamTag || token->GetName() == kSourceTag ||
      token->GetName() == kTrackTag) {
    tree_.InsertSelfClosingHTMLElementDestroyingToken(token);
    return;
  }
  if (token->GetName() == kHrTag) {
    ProcessFakePEndTagIfPInButtonScope();
    tree_.InsertSelfClosingHTMLElementDestroyingToken(token);
    frameset_ok_ = false;
    return;
  }
  if (token->GetName() == kTextareaTag) {
    tree_.InsertHTMLElement(token);
    should_skip_leading_newline_ = true;
    if (parser_->Tokenizer())
      parser_->Tokenizer()->SetState(HTMLTokenizer::kRCDATAState);
    original_insertion_mode_ = insertion_mode_;
    frameset_ok_ = false;
    SetInsertionMode(kTextMode);
    return;
  }
  if (token->GetName() == kXmpTag) {
    ProcessFakePEndTagIfPInButtonScope();
    tree_.ReconstructTheActiveFormattingElements();
    frameset_ok_ = false;
    ProcessGenericRawTextStartTag(token);
    return;
  }
  if (token->GetName() == kIFrameTag) {
    frameset_ok_ = false;
    ProcessGenericRawTextStartTag(token);
    return;
  }
  if (token->GetName() == kNoembedTag) {
    ProcessGenericRawTextStartTag(token);
    return;
  }
  if (token->GetName() == kNoscriptTag && options_.script_enabled) {
    ProcessGenericRawTextStartTag(token);
    return;
  }
  if (token->GetName() == kSelectTag) {
    tree_.ReconstructTheActiveFormattingElements();
    tree_.InsertHTMLElement(token);
    frameset_ok_ = false;
    if (insertion_mode_ == kInTableMode || insertion_mode_ == kInCaptionMode ||
        insertion_mode_ == kInColumnGroupMode ||
        insertion_mode_ == kInTableBodyMode || insertion_mode_ == kInRowMode ||
        insertion_mode_ == kInCellMode)
      SetInsertionMode(kInSelectInTableMode);
    else
      SetInsertionMode(kInSelectMode);
    return;
  }
  if (token->GetName() == kOptgroupTag || token->GetName() == kOptionTag) {
    if (tree_.CurrentStackItem()->HasTagName(kOptionTag)) {
      AtomicHTMLToken end_option(HTMLToken::kEndTag, kOptionTag.LocalName());
      ProcessEndTag(&end_option);
    }
    tree_.ReconstructTheActiveFormattingElements();
    tree_.InsertHTMLElement(token);
    return;
  }
  if (token->GetName() == kRbTag || token->GetName() == kRTCTag) {
    if (tree_.OpenElements()->InScope(kRubyTag.LocalName())) {
      tree_.GenerateImpliedEndTags();
      if (!tree_.CurrentStackItem()->HasTagName(kRubyTag))
        ParseError(token);
    }
    tree_.InsertHTMLElement(token);
    return;
  }
  if (token->GetName() == kRtTag || token->GetName() == kRpTag) {
    if (tree_.OpenElements()->InScope(kRubyTag.LocalName())) {
      tree_.GenerateImpliedEndTagsWithExclusion(kRTCTag.LocalName());
      if (!tree_.CurrentStackItem()->HasTagName(kRubyTag) &&
          !tree_.CurrentStackItem()->HasTagName(kRTCTag))
        ParseError(token);
    }
    tree_.InsertHTMLElement(token);
    return;
  }
  if (token->GetName() == mathml_names::kMathTag.LocalName()) {
    tree_.ReconstructTheActiveFormattingElements();
    AdjustMathMLAttributes(token);
    AdjustForeignAttributes(token);
    tree_.InsertForeignElement(token, mathml_names::kNamespaceURI);
    return;
  }
  if (token->GetName() == svg_names::kSVGTag.LocalName()) {
    tree_.ReconstructTheActiveFormattingElements();
    AdjustSVGAttributes(token);
    AdjustForeignAttributes(token);
    tree_.InsertForeignElement(token, svg_names::kNamespaceURI);
    return;
  }
  if (IsCaptionColOrColgroupTag(token->GetName()) ||
      token->GetName() == kFrameTag || token->GetName() == kHeadTag ||
      IsTableBodyContextTag(token->GetName()) ||
      IsTableCellContextTag(token->GetName()) || token->GetName() == kTrTag) {
    ParseError(token);
    return;
  }
  tree_.ReconstructTheActiveFormattingElements();
  tree_.InsertHTMLElement(token);
}

void HTMLTreeBuilder::ProcessTemplateStartTag(AtomicHTMLToken* token) {
  tree_.ActiveFormattingElements()->AppendMarker();
  tree_.InsertHTMLElement(token);
  frameset_ok_ = false;
  template_insertion_modes_.push_back(kTemplateContentsMode);
  SetInsertionMode(kTemplateContentsMode);
}

bool HTMLTreeBuilder::ProcessTemplateEndTag(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetName(), kTemplateTag.LocalName());
  if (!tree_.OpenElements()->HasTemplateInHTMLScope()) {
    DCHECK(template_insertion_modes_.IsEmpty() ||
           (template_insertion_modes_.size() == 1 &&
            IsHTMLTemplateElement(fragment_context_.ContextElement())));
    ParseError(token);
    return false;
  }
  tree_.GenerateImpliedEndTags();
  if (!tree_.CurrentStackItem()->HasTagName(kTemplateTag))
    ParseError(token);
  tree_.OpenElements()->PopUntilPopped(kTemplateTag);
  tree_.ActiveFormattingElements()->ClearToLastMarker();
  template_insertion_modes_.pop_back();
  ResetInsertionModeAppropriately();
  return true;
}

bool HTMLTreeBuilder::ProcessEndOfFileForInTemplateContents(
    AtomicHTMLToken* token) {
  AtomicHTMLToken end_template(HTMLToken::kEndTag, kTemplateTag.LocalName());
  if (!ProcessTemplateEndTag(&end_template))
    return false;

  ProcessEndOfFile(token);
  return true;
}

bool HTMLTreeBuilder::ProcessColgroupEndTagForInColumnGroup() {
  if (tree_.CurrentIsRootNode() ||
      IsHTMLTemplateElement(*tree_.CurrentNode())) {
    DCHECK(IsParsingFragmentOrTemplateContents());
    // FIXME: parse error
    return false;
  }
  tree_.OpenElements()->Pop();
  SetInsertionMode(kInTableMode);
  return true;
}

// http://www.whatwg.org/specs/web-apps/current-work/#adjusted-current-node
HTMLStackItem* HTMLTreeBuilder::AdjustedCurrentStackItem() const {
  DCHECK(!tree_.IsEmpty());
  if (IsParsingFragment() && tree_.OpenElements()->HasOnlyOneElement())
    return fragment_context_.ContextElementStackItem();

  return tree_.CurrentStackItem();
}

// http://www.whatwg.org/specs/web-apps/current-work/multipage/tokenization.html#close-the-cell
void HTMLTreeBuilder::CloseTheCell() {
  DCHECK_EQ(GetInsertionMode(), kInCellMode);
  if (tree_.OpenElements()->InTableScope(kTdTag)) {
    DCHECK(!tree_.OpenElements()->InTableScope(kThTag));
    ProcessFakeEndTag(kTdTag);
    return;
  }
  DCHECK(tree_.OpenElements()->InTableScope(kThTag));
  ProcessFakeEndTag(kThTag);
  DCHECK_EQ(GetInsertionMode(), kInRowMode);
}

void HTMLTreeBuilder::ProcessStartTagForInTable(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kStartTag);
  if (token->GetName() == kCaptionTag) {
    tree_.OpenElements()->PopUntilTableScopeMarker();
    tree_.ActiveFormattingElements()->AppendMarker();
    tree_.InsertHTMLElement(token);
    SetInsertionMode(kInCaptionMode);
    return;
  }
  if (token->GetName() == kColgroupTag) {
    tree_.OpenElements()->PopUntilTableScopeMarker();
    tree_.InsertHTMLElement(token);
    SetInsertionMode(kInColumnGroupMode);
    return;
  }
  if (token->GetName() == kColTag) {
    ProcessFakeStartTag(kColgroupTag);
    DCHECK(kInColumnGroupMode);
    ProcessStartTag(token);
    return;
  }
  if (IsTableBodyContextTag(token->GetName())) {
    tree_.OpenElements()->PopUntilTableScopeMarker();
    tree_.InsertHTMLElement(token);
    SetInsertionMode(kInTableBodyMode);
    return;
  }
  if (IsTableCellContextTag(token->GetName()) || token->GetName() == kTrTag) {
    ProcessFakeStartTag(kTbodyTag);
    DCHECK_EQ(GetInsertionMode(), kInTableBodyMode);
    ProcessStartTag(token);
    return;
  }
  if (token->GetName() == kTableTag) {
    ParseError(token);
    if (!ProcessTableEndTagForInTable()) {
      DCHECK(IsParsingFragmentOrTemplateContents());
      return;
    }
    ProcessStartTag(token);
    return;
  }
  if (token->GetName() == kStyleTag || token->GetName() == kScriptTag) {
    ProcessStartTagForInHead(token);
    return;
  }
  if (token->GetName() == kInputTag) {
    Attribute* type_attribute = token->GetAttributeItem(kTypeAttr);
    if (type_attribute &&
        DeprecatedEqualIgnoringCase(type_attribute->Value(), "hidden")) {
      ParseError(token);
      tree_.InsertSelfClosingHTMLElementDestroyingToken(token);
      return;
    }
    // Fall through to "anything else" case.
  }
  if (token->GetName() == kFormTag) {
    ParseError(token);
    if (tree_.IsFormElementPointerNonNull() && !IsParsingTemplateContents())
      return;
    tree_.InsertHTMLFormElement(token, true);
    tree_.OpenElements()->Pop();
    return;
  }
  if (token->GetName() == kTemplateTag) {
    ProcessTemplateStartTag(token);
    return;
  }
  ParseError(token);
  HTMLConstructionSite::RedirectToFosterParentGuard redirecter(tree_);
  ProcessStartTagForInBody(token);
}

void HTMLTreeBuilder::ProcessStartTag(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kStartTag);
  switch (GetInsertionMode()) {
    case kInitialMode:
      DCHECK_EQ(GetInsertionMode(), kInitialMode);
      DefaultForInitial();
      FALLTHROUGH;
    case kBeforeHTMLMode:
      DCHECK_EQ(GetInsertionMode(), kBeforeHTMLMode);
      if (token->GetName() == kHTMLTag) {
        tree_.InsertHTMLHtmlStartTagBeforeHTML(token);
        SetInsertionMode(kBeforeHeadMode);
        return;
      }
      DefaultForBeforeHTML();
      FALLTHROUGH;
    case kBeforeHeadMode:
      DCHECK_EQ(GetInsertionMode(), kBeforeHeadMode);
      if (token->GetName() == kHTMLTag) {
        ProcessHtmlStartTagForInBody(token);
        return;
      }
      if (token->GetName() == kHeadTag) {
        tree_.InsertHTMLHeadElement(token);
        SetInsertionMode(kInHeadMode);
        return;
      }
      DefaultForBeforeHead();
      FALLTHROUGH;
    case kInHeadMode:
      DCHECK_EQ(GetInsertionMode(), kInHeadMode);
      if (ProcessStartTagForInHead(token))
        return;
      DefaultForInHead();
      FALLTHROUGH;
    case kAfterHeadMode:
      DCHECK_EQ(GetInsertionMode(), kAfterHeadMode);
      if (token->GetName() == kHTMLTag) {
        ProcessHtmlStartTagForInBody(token);
        return;
      }
      if (token->GetName() == kBodyTag) {
        frameset_ok_ = false;
        tree_.InsertHTMLBodyElement(token);
        SetInsertionMode(kInBodyMode);
        return;
      }
      if (token->GetName() == kFramesetTag) {
        tree_.InsertHTMLElement(token);
        SetInsertionMode(kInFramesetMode);
        return;
      }
      if (token->GetName() == kBaseTag || token->GetName() == kBasefontTag ||
          token->GetName() == kBgsoundTag || token->GetName() == kLinkTag ||
          token->GetName() == kMetaTag || token->GetName() == kNoframesTag ||
          token->GetName() == kScriptTag || token->GetName() == kStyleTag ||
          token->GetName() == kTemplateTag || token->GetName() == kTitleTag) {
        ParseError(token);
        DCHECK(tree_.Head());
        tree_.OpenElements()->PushHTMLHeadElement(tree_.HeadStackItem());
        ProcessStartTagForInHead(token);
        tree_.OpenElements()->RemoveHTMLHeadElement(tree_.Head());
        return;
      }
      if (token->GetName() == kHeadTag) {
        ParseError(token);
        return;
      }
      DefaultForAfterHead();
      FALLTHROUGH;
    case kInBodyMode:
      DCHECK_EQ(GetInsertionMode(), kInBodyMode);
      ProcessStartTagForInBody(token);
      break;
    case kInTableMode:
      DCHECK_EQ(GetInsertionMode(), kInTableMode);
      ProcessStartTagForInTable(token);
      break;
    case kInCaptionMode:
      DCHECK_EQ(GetInsertionMode(), kInCaptionMode);
      if (IsCaptionColOrColgroupTag(token->GetName()) ||
          IsTableBodyContextTag(token->GetName()) ||
          IsTableCellContextTag(token->GetName()) ||
          token->GetName() == kTrTag) {
        ParseError(token);
        if (!ProcessCaptionEndTagForInCaption()) {
          DCHECK(IsParsingFragment());
          return;
        }
        ProcessStartTag(token);
        return;
      }
      ProcessStartTagForInBody(token);
      break;
    case kInColumnGroupMode:
      DCHECK_EQ(GetInsertionMode(), kInColumnGroupMode);
      if (token->GetName() == kHTMLTag) {
        ProcessHtmlStartTagForInBody(token);
        return;
      }
      if (token->GetName() == kColTag) {
        tree_.InsertSelfClosingHTMLElementDestroyingToken(token);
        return;
      }
      if (token->GetName() == kTemplateTag) {
        ProcessTemplateStartTag(token);
        return;
      }
      if (!ProcessColgroupEndTagForInColumnGroup()) {
        DCHECK(IsParsingFragmentOrTemplateContents());
        return;
      }
      ProcessStartTag(token);
      break;
    case kInTableBodyMode:
      DCHECK_EQ(GetInsertionMode(), kInTableBodyMode);
      if (token->GetName() == kTrTag) {
        // How is there ever anything to pop?
        tree_.OpenElements()->PopUntilTableBodyScopeMarker();
        tree_.InsertHTMLElement(token);
        SetInsertionMode(kInRowMode);
        return;
      }
      if (IsTableCellContextTag(token->GetName())) {
        ParseError(token);
        ProcessFakeStartTag(kTrTag);
        DCHECK_EQ(GetInsertionMode(), kInRowMode);
        ProcessStartTag(token);
        return;
      }
      if (IsCaptionColOrColgroupTag(token->GetName()) ||
          IsTableBodyContextTag(token->GetName())) {
        // FIXME: This is slow.
        if (!tree_.OpenElements()->InTableScope(kTbodyTag) &&
            !tree_.OpenElements()->InTableScope(kTheadTag) &&
            !tree_.OpenElements()->InTableScope(kTfootTag)) {
          DCHECK(IsParsingFragmentOrTemplateContents());
          ParseError(token);
          return;
        }
        tree_.OpenElements()->PopUntilTableBodyScopeMarker();
        DCHECK(IsTableBodyContextTag(tree_.CurrentStackItem()->LocalName()));
        ProcessFakeEndTag(tree_.CurrentStackItem()->LocalName());
        ProcessStartTag(token);
        return;
      }
      ProcessStartTagForInTable(token);
      break;
    case kInRowMode:
      DCHECK_EQ(GetInsertionMode(), kInRowMode);
      if (IsTableCellContextTag(token->GetName())) {
        tree_.OpenElements()->PopUntilTableRowScopeMarker();
        tree_.InsertHTMLElement(token);
        SetInsertionMode(kInCellMode);
        tree_.ActiveFormattingElements()->AppendMarker();
        return;
      }
      if (token->GetName() == kTrTag ||
          IsCaptionColOrColgroupTag(token->GetName()) ||
          IsTableBodyContextTag(token->GetName())) {
        if (!ProcessTrEndTagForInRow()) {
          DCHECK(IsParsingFragmentOrTemplateContents());
          return;
        }
        DCHECK_EQ(GetInsertionMode(), kInTableBodyMode);
        ProcessStartTag(token);
        return;
      }
      ProcessStartTagForInTable(token);
      break;
    case kInCellMode:
      DCHECK_EQ(GetInsertionMode(), kInCellMode);
      if (IsCaptionColOrColgroupTag(token->GetName()) ||
          IsTableCellContextTag(token->GetName()) ||
          token->GetName() == kTrTag ||
          IsTableBodyContextTag(token->GetName())) {
        // FIXME: This could be more efficient.
        if (!tree_.OpenElements()->InTableScope(kTdTag) &&
            !tree_.OpenElements()->InTableScope(kThTag)) {
          DCHECK(IsParsingFragment());
          ParseError(token);
          return;
        }
        CloseTheCell();
        ProcessStartTag(token);
        return;
      }
      ProcessStartTagForInBody(token);
      break;
    case kAfterBodyMode:
    case kAfterAfterBodyMode:
      DCHECK(GetInsertionMode() == kAfterBodyMode ||
             GetInsertionMode() == kAfterAfterBodyMode);
      if (token->GetName() == kHTMLTag) {
        ProcessHtmlStartTagForInBody(token);
        return;
      }
      SetInsertionMode(kInBodyMode);
      ProcessStartTag(token);
      break;
    case kInHeadNoscriptMode:
      DCHECK_EQ(GetInsertionMode(), kInHeadNoscriptMode);
      if (token->GetName() == kHTMLTag) {
        ProcessHtmlStartTagForInBody(token);
        return;
      }
      if (token->GetName() == kBasefontTag || token->GetName() == kBgsoundTag ||
          token->GetName() == kLinkTag || token->GetName() == kMetaTag ||
          token->GetName() == kNoframesTag || token->GetName() == kStyleTag) {
        bool did_process = ProcessStartTagForInHead(token);
        DCHECK(did_process);
        return;
      }
      if (token->GetName() == kHTMLTag || token->GetName() == kNoscriptTag) {
        ParseError(token);
        return;
      }
      DefaultForInHeadNoscript();
      ProcessToken(token);
      break;
    case kInFramesetMode:
      DCHECK_EQ(GetInsertionMode(), kInFramesetMode);
      if (token->GetName() == kHTMLTag) {
        ProcessHtmlStartTagForInBody(token);
        return;
      }
      if (token->GetName() == kFramesetTag) {
        tree_.InsertHTMLElement(token);
        return;
      }
      if (token->GetName() == kFrameTag) {
        tree_.InsertSelfClosingHTMLElementDestroyingToken(token);
        return;
      }
      if (token->GetName() == kNoframesTag) {
        ProcessStartTagForInHead(token);
        return;
      }
      ParseError(token);
      break;
    case kAfterFramesetMode:
    case kAfterAfterFramesetMode:
      DCHECK(GetInsertionMode() == kAfterFramesetMode ||
             GetInsertionMode() == kAfterAfterFramesetMode);
      if (token->GetName() == kHTMLTag) {
        ProcessHtmlStartTagForInBody(token);
        return;
      }
      if (token->GetName() == kNoframesTag) {
        ProcessStartTagForInHead(token);
        return;
      }
      ParseError(token);
      break;
    case kInSelectInTableMode:
      DCHECK_EQ(GetInsertionMode(), kInSelectInTableMode);
      if (token->GetName() == kCaptionTag || token->GetName() == kTableTag ||
          IsTableBodyContextTag(token->GetName()) ||
          token->GetName() == kTrTag ||
          IsTableCellContextTag(token->GetName())) {
        ParseError(token);
        AtomicHTMLToken end_select(HTMLToken::kEndTag, kSelectTag.LocalName());
        ProcessEndTag(&end_select);
        ProcessStartTag(token);
        return;
      }
      FALLTHROUGH;
    case kInSelectMode:
      DCHECK(GetInsertionMode() == kInSelectMode ||
             GetInsertionMode() == kInSelectInTableMode);
      if (token->GetName() == kHTMLTag) {
        ProcessHtmlStartTagForInBody(token);
        return;
      }
      if (token->GetName() == kOptionTag) {
        if (tree_.CurrentStackItem()->HasTagName(kOptionTag)) {
          AtomicHTMLToken end_option(HTMLToken::kEndTag,
                                     kOptionTag.LocalName());
          ProcessEndTag(&end_option);
        }
        tree_.InsertHTMLElement(token);
        return;
      }
      if (token->GetName() == kOptgroupTag) {
        if (tree_.CurrentStackItem()->HasTagName(kOptionTag)) {
          AtomicHTMLToken end_option(HTMLToken::kEndTag,
                                     kOptionTag.LocalName());
          ProcessEndTag(&end_option);
        }
        if (tree_.CurrentStackItem()->HasTagName(kOptgroupTag)) {
          AtomicHTMLToken end_optgroup(HTMLToken::kEndTag,
                                       kOptgroupTag.LocalName());
          ProcessEndTag(&end_optgroup);
        }
        tree_.InsertHTMLElement(token);
        return;
      }
      if (token->GetName() == kSelectTag) {
        ParseError(token);
        AtomicHTMLToken end_select(HTMLToken::kEndTag, kSelectTag.LocalName());
        ProcessEndTag(&end_select);
        return;
      }
      if (token->GetName() == kInputTag || token->GetName() == kKeygenTag ||
          token->GetName() == kTextareaTag) {
        ParseError(token);
        if (!tree_.OpenElements()->InSelectScope(kSelectTag)) {
          DCHECK(IsParsingFragment());
          return;
        }
        AtomicHTMLToken end_select(HTMLToken::kEndTag, kSelectTag.LocalName());
        ProcessEndTag(&end_select);
        ProcessStartTag(token);
        return;
      }
      if (token->GetName() == kScriptTag) {
        bool did_process = ProcessStartTagForInHead(token);
        DCHECK(did_process);
        return;
      }
      if (token->GetName() == kTemplateTag) {
        ProcessTemplateStartTag(token);
        return;
      }
      break;
    case kInTableTextMode:
      DefaultForInTableText();
      ProcessStartTag(token);
      break;
    case kTextMode:
      NOTREACHED();
      break;
    case kTemplateContentsMode:
      if (token->GetName() == kTemplateTag) {
        ProcessTemplateStartTag(token);
        return;
      }

      if (token->GetName() == kLinkTag || token->GetName() == kScriptTag ||
          token->GetName() == kStyleTag || token->GetName() == kMetaTag) {
        ProcessStartTagForInHead(token);
        return;
      }

      InsertionMode insertion_mode = kTemplateContentsMode;
      if (token->GetName() == kColTag)
        insertion_mode = kInColumnGroupMode;
      else if (IsCaptionColOrColgroupTag(token->GetName()) ||
               IsTableBodyContextTag(token->GetName()))
        insertion_mode = kInTableMode;
      else if (token->GetName() == kTrTag)
        insertion_mode = kInTableBodyMode;
      else if (IsTableCellContextTag(token->GetName()))
        insertion_mode = kInRowMode;
      else
        insertion_mode = kInBodyMode;

      DCHECK_NE(insertion_mode, kTemplateContentsMode);
      DCHECK_EQ(template_insertion_modes_.back(), kTemplateContentsMode);
      template_insertion_modes_.back() = insertion_mode;
      SetInsertionMode(insertion_mode);

      ProcessStartTag(token);
      break;
  }
}

void HTMLTreeBuilder::ProcessHtmlStartTagForInBody(AtomicHTMLToken* token) {
  ParseError(token);
  if (tree_.OpenElements()->HasTemplateInHTMLScope()) {
    DCHECK(IsParsingTemplateContents());
    return;
  }
  tree_.InsertHTMLHtmlStartTagInBody(token);
}

bool HTMLTreeBuilder::ProcessBodyEndTagForInBody(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kEndTag);
  DCHECK(token->GetName() == kBodyTag);
  if (!tree_.OpenElements()->InScope(kBodyTag.LocalName())) {
    ParseError(token);
    return false;
  }
  // Emit a more specific parse error based on stack contents.
  DVLOG(1) << "Not implmeneted.";
  SetInsertionMode(kAfterBodyMode);
  return true;
}

void HTMLTreeBuilder::ProcessAnyOtherEndTagForInBody(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kEndTag);
  HTMLElementStack::ElementRecord* record = tree_.OpenElements()->TopRecord();
  while (1) {
    HTMLStackItem* item = record->StackItem();
    if (item->MatchesHTMLTag(token->GetName())) {
      tree_.GenerateImpliedEndTagsWithExclusion(token->GetName());
      if (!tree_.CurrentStackItem()->MatchesHTMLTag(token->GetName()))
        ParseError(token);
      tree_.OpenElements()->PopUntilPopped(item->GetElement());
      return;
    }
    if (item->IsSpecialNode()) {
      ParseError(token);
      return;
    }
    record = record->Next();
  }
}

// http://www.whatwg.org/specs/web-apps/current-work/multipage/tokenization.html#parsing-main-inbody
void HTMLTreeBuilder::CallTheAdoptionAgency(AtomicHTMLToken* token) {
  // The adoption agency algorithm is N^2. We limit the number of iterations
  // to stop from hanging the whole browser. This limit is specified in the
  // adoption agency algorithm:
  // http://www.whatwg.org/specs/web-apps/current-work/multipage/tree-construction.html#parsing-main-inbody
  static const int kOuterIterationLimit = 8;
  static const int kInnerIterationLimit = 3;

  // 1, 2, 3 and 16 are covered by the for() loop.
  for (int i = 0; i < kOuterIterationLimit; ++i) {
    // 4.
    Element* formatting_element =
        tree_.ActiveFormattingElements()->ClosestElementInScopeWithName(
            token->GetName());
    // 4.a
    if (!formatting_element)
      return ProcessAnyOtherEndTagForInBody(token);
    // 4.c
    if ((tree_.OpenElements()->Contains(formatting_element)) &&
        !tree_.OpenElements()->InScope(formatting_element)) {
      ParseError(token);
      // Check the stack of open elements for a more specific parse error.
      DVLOG(1) << "Not implemented.";
      return;
    }
    // 4.b
    HTMLElementStack::ElementRecord* formatting_element_record =
        tree_.OpenElements()->Find(formatting_element);
    if (!formatting_element_record) {
      ParseError(token);
      tree_.ActiveFormattingElements()->Remove(formatting_element);
      return;
    }
    // 4.d
    if (formatting_element != tree_.CurrentElement())
      ParseError(token);
    // 5.
    HTMLElementStack::ElementRecord* furthest_block =
        tree_.OpenElements()->FurthestBlockForFormattingElement(
            formatting_element);
    // 6.
    if (!furthest_block) {
      tree_.OpenElements()->PopUntilPopped(formatting_element);
      tree_.ActiveFormattingElements()->Remove(formatting_element);
      return;
    }
    // 7.
    DCHECK(furthest_block->IsAbove(formatting_element_record));
    HTMLStackItem* common_ancestor =
        formatting_element_record->Next()->StackItem();
    // 8.
    HTMLFormattingElementList::Bookmark bookmark =
        tree_.ActiveFormattingElements()->BookmarkFor(formatting_element);
    // 9.
    HTMLElementStack::ElementRecord* node = furthest_block;
    HTMLElementStack::ElementRecord* next_node = node->Next();
    HTMLElementStack::ElementRecord* last_node = furthest_block;
    // 9.1, 9.2, 9.3 and 9.11 are covered by the for() loop.
    for (int j = 0; j < kInnerIterationLimit; ++j) {
      // 9.4
      node = next_node;
      DCHECK(node);
      // Save node->next() for the next iteration in case node is deleted in
      // 9.5.
      next_node = node->Next();
      // 9.5
      if (!tree_.ActiveFormattingElements()->Contains(node->GetElement())) {
        tree_.OpenElements()->Remove(node->GetElement());
        node = nullptr;
        continue;
      }
      // 9.6
      if (node == formatting_element_record)
        break;
      // 9.7
      HTMLStackItem* new_item =
          tree_.CreateElementFromSavedToken(node->StackItem());

      HTMLFormattingElementList::Entry* node_entry =
          tree_.ActiveFormattingElements()->Find(node->GetElement());
      node_entry->ReplaceElement(new_item);
      node->ReplaceElement(new_item);

      // 9.8
      if (last_node == furthest_block)
        bookmark.MoveToAfter(node_entry);
      // 9.9
      tree_.Reparent(node, last_node);
      // 9.10
      last_node = node;
    }
    // 10.
    tree_.InsertAlreadyParsedChild(common_ancestor, last_node);
    // 11.
    HTMLStackItem* new_item = tree_.CreateElementFromSavedToken(
        formatting_element_record->StackItem());
    // 12.
    tree_.TakeAllChildren(new_item, furthest_block);
    // 13.
    tree_.Reparent(furthest_block, new_item);
    // 14.
    tree_.ActiveFormattingElements()->SwapTo(formatting_element, new_item,
                                             bookmark);
    // 15.
    tree_.OpenElements()->Remove(formatting_element);
    tree_.OpenElements()->InsertAbove(new_item, furthest_block);
  }
}

void HTMLTreeBuilder::ResetInsertionModeAppropriately() {
  // http://www.whatwg.org/specs/web-apps/current-work/multipage/parsing.html#reset-the-insertion-mode-appropriately
  bool last = false;
  HTMLElementStack::ElementRecord* node_record =
      tree_.OpenElements()->TopRecord();
  while (1) {
    HTMLStackItem* item = node_record->StackItem();
    if (item->GetNode() == tree_.OpenElements()->RootNode()) {
      last = true;
      if (IsParsingFragment())
        item = fragment_context_.ContextElementStackItem();
    }
    if (item->HasTagName(kTemplateTag))
      return SetInsertionMode(template_insertion_modes_.back());
    if (item->HasTagName(kSelectTag)) {
      if (!last) {
        while (item->GetNode() != tree_.OpenElements()->RootNode() &&
               !item->HasTagName(kTemplateTag)) {
          node_record = node_record->Next();
          item = node_record->StackItem();
          if (item->HasTagName(kTableTag))
            return SetInsertionMode(kInSelectInTableMode);
        }
      }
      return SetInsertionMode(kInSelectMode);
    }
    if (item->HasTagName(kTdTag) || item->HasTagName(kThTag))
      return SetInsertionMode(kInCellMode);
    if (item->HasTagName(kTrTag))
      return SetInsertionMode(kInRowMode);
    if (item->HasTagName(kTbodyTag) || item->HasTagName(kTheadTag) ||
        item->HasTagName(kTfootTag))
      return SetInsertionMode(kInTableBodyMode);
    if (item->HasTagName(kCaptionTag))
      return SetInsertionMode(kInCaptionMode);
    if (item->HasTagName(kColgroupTag)) {
      return SetInsertionMode(kInColumnGroupMode);
    }
    if (item->HasTagName(kTableTag))
      return SetInsertionMode(kInTableMode);
    if (item->HasTagName(kHeadTag)) {
      if (!fragment_context_.Fragment() ||
          fragment_context_.ContextElement() != item->GetNode())
        return SetInsertionMode(kInHeadMode);
      return SetInsertionMode(kInBodyMode);
    }
    if (item->HasTagName(kBodyTag))
      return SetInsertionMode(kInBodyMode);
    if (item->HasTagName(kFramesetTag)) {
      return SetInsertionMode(kInFramesetMode);
    }
    if (item->HasTagName(kHTMLTag)) {
      if (tree_.HeadStackItem())
        return SetInsertionMode(kAfterHeadMode);

      DCHECK(IsParsingFragment());
      return SetInsertionMode(kBeforeHeadMode);
    }
    if (last) {
      DCHECK(IsParsingFragment());
      return SetInsertionMode(kInBodyMode);
    }
    node_record = node_record->Next();
  }
}

void HTMLTreeBuilder::ProcessEndTagForInTableBody(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kEndTag);
  if (IsTableBodyContextTag(token->GetName())) {
    if (!tree_.OpenElements()->InTableScope(token->GetName())) {
      ParseError(token);
      return;
    }
    tree_.OpenElements()->PopUntilTableBodyScopeMarker();
    tree_.OpenElements()->Pop();
    SetInsertionMode(kInTableMode);
    return;
  }
  if (token->GetName() == kTableTag) {
    // FIXME: This is slow.
    if (!tree_.OpenElements()->InTableScope(kTbodyTag) &&
        !tree_.OpenElements()->InTableScope(kTheadTag) &&
        !tree_.OpenElements()->InTableScope(kTfootTag)) {
      DCHECK(IsParsingFragmentOrTemplateContents());
      ParseError(token);
      return;
    }
    tree_.OpenElements()->PopUntilTableBodyScopeMarker();
    DCHECK(IsTableBodyContextTag(tree_.CurrentStackItem()->LocalName()));
    ProcessFakeEndTag(tree_.CurrentStackItem()->LocalName());
    ProcessEndTag(token);
    return;
  }
  if (token->GetName() == kBodyTag ||
      IsCaptionColOrColgroupTag(token->GetName()) ||
      token->GetName() == kHTMLTag || IsTableCellContextTag(token->GetName()) ||
      token->GetName() == kTrTag) {
    ParseError(token);
    return;
  }
  ProcessEndTagForInTable(token);
}

void HTMLTreeBuilder::ProcessEndTagForInRow(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kEndTag);
  if (token->GetName() == kTrTag) {
    ProcessTrEndTagForInRow();
    return;
  }
  if (token->GetName() == kTableTag) {
    if (!ProcessTrEndTagForInRow()) {
      DCHECK(IsParsingFragmentOrTemplateContents());
      return;
    }
    DCHECK_EQ(GetInsertionMode(), kInTableBodyMode);
    ProcessEndTag(token);
    return;
  }
  if (IsTableBodyContextTag(token->GetName())) {
    if (!tree_.OpenElements()->InTableScope(token->GetName())) {
      ParseError(token);
      return;
    }
    ProcessFakeEndTag(kTrTag);
    DCHECK_EQ(GetInsertionMode(), kInTableBodyMode);
    ProcessEndTag(token);
    return;
  }
  if (token->GetName() == kBodyTag ||
      IsCaptionColOrColgroupTag(token->GetName()) ||
      token->GetName() == kHTMLTag || IsTableCellContextTag(token->GetName())) {
    ParseError(token);
    return;
  }
  ProcessEndTagForInTable(token);
}

void HTMLTreeBuilder::ProcessEndTagForInCell(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kEndTag);
  if (IsTableCellContextTag(token->GetName())) {
    if (!tree_.OpenElements()->InTableScope(token->GetName())) {
      ParseError(token);
      return;
    }
    tree_.GenerateImpliedEndTags();
    if (!tree_.CurrentStackItem()->MatchesHTMLTag(token->GetName()))
      ParseError(token);
    tree_.OpenElements()->PopUntilPopped(token->GetName());
    tree_.ActiveFormattingElements()->ClearToLastMarker();
    SetInsertionMode(kInRowMode);
    return;
  }
  if (token->GetName() == kBodyTag ||
      IsCaptionColOrColgroupTag(token->GetName()) ||
      token->GetName() == kHTMLTag) {
    ParseError(token);
    return;
  }
  if (token->GetName() == kTableTag || token->GetName() == kTrTag ||
      IsTableBodyContextTag(token->GetName())) {
    if (!tree_.OpenElements()->InTableScope(token->GetName())) {
      DCHECK(IsTableBodyContextTag(token->GetName()) ||
             tree_.OpenElements()->InTableScope(kTemplateTag) ||
             IsParsingFragment());
      ParseError(token);
      return;
    }
    CloseTheCell();
    ProcessEndTag(token);
    return;
  }
  ProcessEndTagForInBody(token);
}

void HTMLTreeBuilder::ProcessEndTagForInBody(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kEndTag);
  if (token->GetName() == kBodyTag) {
    ProcessBodyEndTagForInBody(token);
    return;
  }
  if (token->GetName() == kHTMLTag) {
    AtomicHTMLToken end_body(HTMLToken::kEndTag, kBodyTag.LocalName());
    if (ProcessBodyEndTagForInBody(&end_body))
      ProcessEndTag(token);
    return;
  }
  if (token->GetName() == kAddressTag || token->GetName() == kArticleTag ||
      token->GetName() == kAsideTag || token->GetName() == kBlockquoteTag ||
      token->GetName() == kButtonTag || token->GetName() == kCenterTag ||
      token->GetName() == kDetailsTag || token->GetName() == kDirTag ||
      token->GetName() == kDivTag || token->GetName() == kDlTag ||
      token->GetName() == kFieldsetTag || token->GetName() == kFigcaptionTag ||
      token->GetName() == kFigureTag || token->GetName() == kFooterTag ||
      token->GetName() == kHeaderTag || token->GetName() == kHgroupTag ||
      token->GetName() == kListingTag || token->GetName() == kMainTag ||
      token->GetName() == kMenuTag || token->GetName() == kNavTag ||
      token->GetName() == kOlTag || token->GetName() == kPreTag ||
      token->GetName() == kSectionTag || token->GetName() == kSummaryTag ||
      token->GetName() == kUlTag) {
    if (!tree_.OpenElements()->InScope(token->GetName())) {
      ParseError(token);
      return;
    }
    tree_.GenerateImpliedEndTags();
    if (!tree_.CurrentStackItem()->MatchesHTMLTag(token->GetName()))
      ParseError(token);
    tree_.OpenElements()->PopUntilPopped(token->GetName());
    return;
  }
  if (token->GetName() == kFormTag && !IsParsingTemplateContents()) {
    Element* node = tree_.TakeForm();
    if (!node || !tree_.OpenElements()->InScope(node)) {
      ParseError(token);
      return;
    }
    tree_.GenerateImpliedEndTags();
    if (tree_.CurrentElement() != node)
      ParseError(token);
    tree_.OpenElements()->Remove(node);
  }
  if (token->GetName() == kPTag) {
    if (!tree_.OpenElements()->InButtonScope(token->GetName())) {
      ParseError(token);
      ProcessFakeStartTag(kPTag);
      DCHECK(tree_.OpenElements()->InScope(token->GetName()));
      ProcessEndTag(token);
      return;
    }
    tree_.GenerateImpliedEndTagsWithExclusion(token->GetName());
    if (!tree_.CurrentStackItem()->MatchesHTMLTag(token->GetName()))
      ParseError(token);
    tree_.OpenElements()->PopUntilPopped(token->GetName());
    return;
  }
  if (token->GetName() == kLiTag) {
    if (!tree_.OpenElements()->InListItemScope(token->GetName())) {
      ParseError(token);
      return;
    }
    tree_.GenerateImpliedEndTagsWithExclusion(token->GetName());
    if (!tree_.CurrentStackItem()->MatchesHTMLTag(token->GetName()))
      ParseError(token);
    tree_.OpenElements()->PopUntilPopped(token->GetName());
    return;
  }
  if (token->GetName() == kDdTag || token->GetName() == kDtTag) {
    if (!tree_.OpenElements()->InScope(token->GetName())) {
      ParseError(token);
      return;
    }
    tree_.GenerateImpliedEndTagsWithExclusion(token->GetName());
    if (!tree_.CurrentStackItem()->MatchesHTMLTag(token->GetName()))
      ParseError(token);
    tree_.OpenElements()->PopUntilPopped(token->GetName());
    return;
  }
  if (IsNumberedHeaderTag(token->GetName())) {
    if (!tree_.OpenElements()->HasNumberedHeaderElementInScope()) {
      ParseError(token);
      return;
    }
    tree_.GenerateImpliedEndTags();
    if (!tree_.CurrentStackItem()->MatchesHTMLTag(token->GetName()))
      ParseError(token);
    tree_.OpenElements()->PopUntilNumberedHeaderElementPopped();
    return;
  }
  if (IsFormattingTag(token->GetName())) {
    CallTheAdoptionAgency(token);
    return;
  }
  if (token->GetName() == kAppletTag || token->GetName() == kMarqueeTag ||
      token->GetName() == kObjectTag) {
    if (!tree_.OpenElements()->InScope(token->GetName())) {
      ParseError(token);
      return;
    }
    tree_.GenerateImpliedEndTags();
    if (!tree_.CurrentStackItem()->MatchesHTMLTag(token->GetName()))
      ParseError(token);
    tree_.OpenElements()->PopUntilPopped(token->GetName());
    tree_.ActiveFormattingElements()->ClearToLastMarker();
    return;
  }
  if (token->GetName() == kBrTag) {
    ParseError(token);
    ProcessFakeStartTag(kBrTag);
    return;
  }
  if (token->GetName() == kTemplateTag) {
    ProcessTemplateEndTag(token);
    return;
  }
  ProcessAnyOtherEndTagForInBody(token);
}

bool HTMLTreeBuilder::ProcessCaptionEndTagForInCaption() {
  if (!tree_.OpenElements()->InTableScope(kCaptionTag.LocalName())) {
    DCHECK(IsParsingFragment());
    // FIXME: parse error
    return false;
  }
  tree_.GenerateImpliedEndTags();
  // FIXME: parse error if (!tree_.CurrentStackItem()->HasTagName(kCaptionTag))
  tree_.OpenElements()->PopUntilPopped(kCaptionTag.LocalName());
  tree_.ActiveFormattingElements()->ClearToLastMarker();
  SetInsertionMode(kInTableMode);
  return true;
}

bool HTMLTreeBuilder::ProcessTrEndTagForInRow() {
  if (!tree_.OpenElements()->InTableScope(kTrTag)) {
    DCHECK(IsParsingFragmentOrTemplateContents());
    // FIXME: parse error
    return false;
  }
  tree_.OpenElements()->PopUntilTableRowScopeMarker();
  DCHECK(tree_.CurrentStackItem()->HasTagName(kTrTag));
  tree_.OpenElements()->Pop();
  SetInsertionMode(kInTableBodyMode);
  return true;
}

bool HTMLTreeBuilder::ProcessTableEndTagForInTable() {
  if (!tree_.OpenElements()->InTableScope(kTableTag)) {
    DCHECK(IsParsingFragmentOrTemplateContents());
    // FIXME: parse error.
    return false;
  }
  tree_.OpenElements()->PopUntilPopped(kTableTag.LocalName());
  ResetInsertionModeAppropriately();
  return true;
}

void HTMLTreeBuilder::ProcessEndTagForInTable(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kEndTag);
  if (token->GetName() == kTableTag) {
    ProcessTableEndTagForInTable();
    return;
  }
  if (token->GetName() == kBodyTag ||
      IsCaptionColOrColgroupTag(token->GetName()) ||
      token->GetName() == kHTMLTag || IsTableBodyContextTag(token->GetName()) ||
      IsTableCellContextTag(token->GetName()) || token->GetName() == kTrTag) {
    ParseError(token);
    return;
  }
  ParseError(token);
  // Is this redirection necessary here?
  HTMLConstructionSite::RedirectToFosterParentGuard redirecter(tree_);
  ProcessEndTagForInBody(token);
}

void HTMLTreeBuilder::ProcessEndTag(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kEndTag);
  switch (GetInsertionMode()) {
    case kInitialMode:
      DCHECK_EQ(GetInsertionMode(), kInitialMode);
      DefaultForInitial();
      FALLTHROUGH;
    case kBeforeHTMLMode:
      DCHECK_EQ(GetInsertionMode(), kBeforeHTMLMode);
      if (token->GetName() != kHeadTag && token->GetName() != kBodyTag &&
          token->GetName() != kHTMLTag && token->GetName() != kBrTag) {
        ParseError(token);
        return;
      }
      DefaultForBeforeHTML();
      FALLTHROUGH;
    case kBeforeHeadMode:
      DCHECK_EQ(GetInsertionMode(), kBeforeHeadMode);
      if (token->GetName() != kHeadTag && token->GetName() != kBodyTag &&
          token->GetName() != kHTMLTag && token->GetName() != kBrTag) {
        ParseError(token);
        return;
      }
      DefaultForBeforeHead();
      FALLTHROUGH;
    case kInHeadMode:
      DCHECK_EQ(GetInsertionMode(), kInHeadMode);
      // FIXME: This case should be broken out into processEndTagForInHead,
      // because other end tag cases now refer to it ("process the token for
      // using the rules of the "in head" insertion mode"). but because the
      // logic falls through to AfterHeadMode, that gets a little messy.
      if (token->GetName() == kTemplateTag) {
        ProcessTemplateEndTag(token);
        return;
      }
      if (token->GetName() == kHeadTag) {
        tree_.OpenElements()->PopHTMLHeadElement();
        SetInsertionMode(kAfterHeadMode);
        return;
      }
      if (token->GetName() != kBodyTag && token->GetName() != kHTMLTag &&
          token->GetName() != kBrTag) {
        ParseError(token);
        return;
      }
      DefaultForInHead();
      FALLTHROUGH;
    case kAfterHeadMode:
      DCHECK_EQ(GetInsertionMode(), kAfterHeadMode);
      if (token->GetName() != kBodyTag && token->GetName() != kHTMLTag &&
          token->GetName() != kBrTag) {
        ParseError(token);
        return;
      }
      DefaultForAfterHead();
      FALLTHROUGH;
    case kInBodyMode:
      DCHECK_EQ(GetInsertionMode(), kInBodyMode);
      ProcessEndTagForInBody(token);
      break;
    case kInTableMode:
      DCHECK_EQ(GetInsertionMode(), kInTableMode);
      ProcessEndTagForInTable(token);
      break;
    case kInCaptionMode:
      DCHECK_EQ(GetInsertionMode(), kInCaptionMode);
      if (token->GetName() == kCaptionTag) {
        ProcessCaptionEndTagForInCaption();
        return;
      }
      if (token->GetName() == kTableTag) {
        ParseError(token);
        if (!ProcessCaptionEndTagForInCaption()) {
          DCHECK(IsParsingFragment());
          return;
        }
        ProcessEndTag(token);
        return;
      }
      if (token->GetName() == kBodyTag || token->GetName() == kColTag ||
          token->GetName() == kColgroupTag || token->GetName() == kHTMLTag ||
          IsTableBodyContextTag(token->GetName()) ||
          IsTableCellContextTag(token->GetName()) ||
          token->GetName() == kTrTag) {
        ParseError(token);
        return;
      }
      ProcessEndTagForInBody(token);
      break;
    case kInColumnGroupMode:
      DCHECK_EQ(GetInsertionMode(), kInColumnGroupMode);
      if (token->GetName() == kColgroupTag) {
        ProcessColgroupEndTagForInColumnGroup();
        return;
      }
      if (token->GetName() == kColTag) {
        ParseError(token);
        return;
      }
      if (token->GetName() == kTemplateTag) {
        ProcessTemplateEndTag(token);
        return;
      }
      if (!ProcessColgroupEndTagForInColumnGroup()) {
        DCHECK(IsParsingFragmentOrTemplateContents());
        return;
      }
      ProcessEndTag(token);
      break;
    case kInRowMode:
      DCHECK_EQ(GetInsertionMode(), kInRowMode);
      ProcessEndTagForInRow(token);
      break;
    case kInCellMode:
      DCHECK_EQ(GetInsertionMode(), kInCellMode);
      ProcessEndTagForInCell(token);
      break;
    case kInTableBodyMode:
      DCHECK_EQ(GetInsertionMode(), kInTableBodyMode);
      ProcessEndTagForInTableBody(token);
      break;
    case kAfterBodyMode:
      DCHECK_EQ(GetInsertionMode(), kAfterBodyMode);
      if (token->GetName() == kHTMLTag) {
        if (IsParsingFragment()) {
          ParseError(token);
          return;
        }
        SetInsertionMode(kAfterAfterBodyMode);
        return;
      }
      FALLTHROUGH;
    case kAfterAfterBodyMode:
      DCHECK(GetInsertionMode() == kAfterBodyMode ||
             GetInsertionMode() == kAfterAfterBodyMode);
      ParseError(token);
      SetInsertionMode(kInBodyMode);
      ProcessEndTag(token);
      break;
    case kInHeadNoscriptMode:
      DCHECK_EQ(GetInsertionMode(), kInHeadNoscriptMode);
      if (token->GetName() == kNoscriptTag) {
        DCHECK(tree_.CurrentStackItem()->HasTagName(kNoscriptTag));
        tree_.OpenElements()->Pop();
        DCHECK(tree_.CurrentStackItem()->HasTagName(kHeadTag));
        SetInsertionMode(kInHeadMode);
        return;
      }
      if (token->GetName() != kBrTag) {
        ParseError(token);
        return;
      }
      DefaultForInHeadNoscript();
      ProcessToken(token);
      break;
    case kTextMode:
      if (token->GetName() == kScriptTag &&
          tree_.CurrentStackItem()->HasTagName(kScriptTag)) {
        // Pause ourselves so that parsing stops until the script can be
        // processed by the caller.
        if (ScriptingContentIsAllowed(tree_.GetParserContentPolicy()))
          script_to_process_ = tree_.CurrentElement();
        tree_.OpenElements()->Pop();
        SetInsertionMode(original_insertion_mode_);

        if (parser_->Tokenizer()) {
          // We must set the tokenizer's state to DataState explicitly if the
          // tokenizer didn't have a chance to.
          parser_->Tokenizer()->SetState(HTMLTokenizer::kDataState);
        }
        return;
      }
      tree_.OpenElements()->Pop();
      SetInsertionMode(original_insertion_mode_);
      break;
    case kInFramesetMode:
      DCHECK_EQ(GetInsertionMode(), kInFramesetMode);
      if (token->GetName() == kFramesetTag) {
        bool ignore_frameset_for_fragment_parsing = tree_.CurrentIsRootNode();
        ignore_frameset_for_fragment_parsing =
            ignore_frameset_for_fragment_parsing ||
            tree_.OpenElements()->HasTemplateInHTMLScope();
        if (ignore_frameset_for_fragment_parsing) {
          DCHECK(IsParsingFragmentOrTemplateContents());
          ParseError(token);
          return;
        }
        tree_.OpenElements()->Pop();
        if (!IsParsingFragment() &&
            !tree_.CurrentStackItem()->HasTagName(kFramesetTag))
          SetInsertionMode(kAfterFramesetMode);
        return;
      }
      break;
    case kAfterFramesetMode:
      DCHECK_EQ(GetInsertionMode(), kAfterFramesetMode);
      if (token->GetName() == kHTMLTag) {
        SetInsertionMode(kAfterAfterFramesetMode);
        return;
      }
      FALLTHROUGH;
    case kAfterAfterFramesetMode:
      DCHECK(GetInsertionMode() == kAfterFramesetMode ||
             GetInsertionMode() == kAfterAfterFramesetMode);
      ParseError(token);
      break;
    case kInSelectInTableMode:
      DCHECK(GetInsertionMode() == kInSelectInTableMode);
      if (token->GetName() == kCaptionTag || token->GetName() == kTableTag ||
          IsTableBodyContextTag(token->GetName()) ||
          token->GetName() == kTrTag ||
          IsTableCellContextTag(token->GetName())) {
        ParseError(token);
        if (tree_.OpenElements()->InTableScope(token->GetName())) {
          AtomicHTMLToken end_select(HTMLToken::kEndTag,
                                     kSelectTag.LocalName());
          ProcessEndTag(&end_select);
          ProcessEndTag(token);
        }
        return;
      }
      FALLTHROUGH;
    case kInSelectMode:
      DCHECK(GetInsertionMode() == kInSelectMode ||
             GetInsertionMode() == kInSelectInTableMode);
      if (token->GetName() == kOptgroupTag) {
        if (tree_.CurrentStackItem()->HasTagName(kOptionTag) &&
            tree_.OneBelowTop() &&
            tree_.OneBelowTop()->HasTagName(kOptgroupTag))
          ProcessFakeEndTag(kOptionTag);
        if (tree_.CurrentStackItem()->HasTagName(kOptgroupTag)) {
          tree_.OpenElements()->Pop();
          return;
        }
        ParseError(token);
        return;
      }
      if (token->GetName() == kOptionTag) {
        if (tree_.CurrentStackItem()->HasTagName(kOptionTag)) {
          tree_.OpenElements()->Pop();
          return;
        }
        ParseError(token);
        return;
      }
      if (token->GetName() == kSelectTag) {
        if (!tree_.OpenElements()->InSelectScope(token->GetName())) {
          DCHECK(IsParsingFragment());
          ParseError(token);
          return;
        }
        tree_.OpenElements()->PopUntilPopped(kSelectTag.LocalName());
        ResetInsertionModeAppropriately();
        return;
      }
      if (token->GetName() == kTemplateTag) {
        ProcessTemplateEndTag(token);
        return;
      }
      break;
    case kInTableTextMode:
      DefaultForInTableText();
      ProcessEndTag(token);
      break;
    case kTemplateContentsMode:
      if (token->GetName() == kTemplateTag) {
        ProcessTemplateEndTag(token);
        return;
      }
      break;
  }
}

void HTMLTreeBuilder::ProcessComment(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kComment);
  if (insertion_mode_ == kInitialMode || insertion_mode_ == kBeforeHTMLMode ||
      insertion_mode_ == kAfterAfterBodyMode ||
      insertion_mode_ == kAfterAfterFramesetMode) {
    tree_.InsertCommentOnDocument(token);
    return;
  }
  if (insertion_mode_ == kAfterBodyMode) {
    tree_.InsertCommentOnHTMLHtmlElement(token);
    return;
  }
  if (insertion_mode_ == kInTableTextMode) {
    DefaultForInTableText();
    ProcessComment(token);
    return;
  }
  tree_.InsertComment(token);
}

void HTMLTreeBuilder::ProcessCharacter(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kCharacter);
  CharacterTokenBuffer buffer(token);
  ProcessCharacterBuffer(buffer);
}

void HTMLTreeBuilder::ProcessCharacterBuffer(CharacterTokenBuffer& buffer) {
ReprocessBuffer:
  // http://www.whatwg.org/specs/web-apps/current-work/multipage/tokenization.html#parsing-main-inbody
  // Note that this logic is different than the generic \r\n collapsing
  // handled in the input stream preprocessor. This logic is here as an
  // "authoring convenience" so folks can write:
  //
  // <pre>
  // lorem ipsum
  // lorem ipsum
  // </pre>
  //
  // without getting an extra newline at the start of their <pre> element.
  if (should_skip_leading_newline_) {
    should_skip_leading_newline_ = false;
    buffer.SkipAtMostOneLeadingNewline();
    if (buffer.IsEmpty())
      return;
  }

  switch (GetInsertionMode()) {
    case kInitialMode: {
      DCHECK_EQ(GetInsertionMode(), kInitialMode);
      buffer.SkipLeadingWhitespace();
      if (buffer.IsEmpty())
        return;
      DefaultForInitial();
      FALLTHROUGH;
    }
    case kBeforeHTMLMode: {
      DCHECK_EQ(GetInsertionMode(), kBeforeHTMLMode);
      buffer.SkipLeadingWhitespace();
      if (buffer.IsEmpty())
        return;
      DefaultForBeforeHTML();
      if (parser_->IsStopped()) {
        buffer.SkipRemaining();
        return;
      }
      FALLTHROUGH;
    }
    case kBeforeHeadMode: {
      DCHECK_EQ(GetInsertionMode(), kBeforeHeadMode);
      buffer.SkipLeadingWhitespace();
      if (buffer.IsEmpty())
        return;
      DefaultForBeforeHead();
      FALLTHROUGH;
    }
    case kInHeadMode: {
      DCHECK_EQ(GetInsertionMode(), kInHeadMode);
      StringView leading_whitespace = buffer.TakeLeadingWhitespace();
      if (!leading_whitespace.IsEmpty())
        tree_.InsertTextNode(leading_whitespace, kAllWhitespace);
      if (buffer.IsEmpty())
        return;
      DefaultForInHead();
      FALLTHROUGH;
    }
    case kAfterHeadMode: {
      DCHECK_EQ(GetInsertionMode(), kAfterHeadMode);
      StringView leading_whitespace = buffer.TakeLeadingWhitespace();
      if (!leading_whitespace.IsEmpty())
        tree_.InsertTextNode(leading_whitespace, kAllWhitespace);
      if (buffer.IsEmpty())
        return;
      DefaultForAfterHead();
      FALLTHROUGH;
    }
    case kInBodyMode:
    case kInCaptionMode:
    case kTemplateContentsMode:
    case kInCellMode: {
      DCHECK(GetInsertionMode() == kInBodyMode ||
             GetInsertionMode() == kInCaptionMode ||
             GetInsertionMode() == kInCellMode ||
             GetInsertionMode() == kTemplateContentsMode);
      ProcessCharacterBufferForInBody(buffer);
      break;
    }
    case kInTableMode:
    case kInTableBodyMode:
    case kInRowMode: {
      DCHECK(GetInsertionMode() == kInTableMode ||
             GetInsertionMode() == kInTableBodyMode ||
             GetInsertionMode() == kInRowMode);
      DCHECK(pending_table_characters_.IsEmpty());
      if (tree_.CurrentStackItem()->IsElementNode() &&
          (tree_.CurrentStackItem()->HasTagName(kTableTag) ||
           tree_.CurrentStackItem()->HasTagName(kTbodyTag) ||
           tree_.CurrentStackItem()->HasTagName(kTfootTag) ||
           tree_.CurrentStackItem()->HasTagName(kTheadTag) ||
           tree_.CurrentStackItem()->HasTagName(kTrTag))) {
        original_insertion_mode_ = insertion_mode_;
        SetInsertionMode(kInTableTextMode);
        // Note that we fall through to the InTableTextMode case below.
      } else {
        HTMLConstructionSite::RedirectToFosterParentGuard redirecter(tree_);
        ProcessCharacterBufferForInBody(buffer);
        break;
      }
      FALLTHROUGH;
    }
    case kInTableTextMode: {
      buffer.GiveRemainingTo(pending_table_characters_);
      break;
    }
    case kInColumnGroupMode: {
      DCHECK_EQ(GetInsertionMode(), kInColumnGroupMode);
      StringView leading_whitespace = buffer.TakeLeadingWhitespace();
      if (!leading_whitespace.IsEmpty())
        tree_.InsertTextNode(leading_whitespace, kAllWhitespace);
      if (buffer.IsEmpty())
        return;
      if (!ProcessColgroupEndTagForInColumnGroup()) {
        DCHECK(IsParsingFragmentOrTemplateContents());
        // The spec tells us to drop these characters on the floor.
        buffer.SkipLeadingNonWhitespace();
        if (buffer.IsEmpty())
          return;
      }
      goto ReprocessBuffer;
    }
    case kAfterBodyMode:
    case kAfterAfterBodyMode: {
      DCHECK(GetInsertionMode() == kAfterBodyMode ||
             GetInsertionMode() == kAfterAfterBodyMode);
      // FIXME: parse error
      SetInsertionMode(kInBodyMode);
      goto ReprocessBuffer;
    }
    case kTextMode: {
      DCHECK_EQ(GetInsertionMode(), kTextMode);
      tree_.InsertTextNode(buffer.TakeRemaining());
      break;
    }
    case kInHeadNoscriptMode: {
      DCHECK_EQ(GetInsertionMode(), kInHeadNoscriptMode);
      StringView leading_whitespace = buffer.TakeLeadingWhitespace();
      if (!leading_whitespace.IsEmpty())
        tree_.InsertTextNode(leading_whitespace, kAllWhitespace);
      if (buffer.IsEmpty())
        return;
      DefaultForInHeadNoscript();
      goto ReprocessBuffer;
    }
    case kInFramesetMode:
    case kAfterFramesetMode: {
      DCHECK(GetInsertionMode() == kInFramesetMode ||
             GetInsertionMode() == kAfterFramesetMode ||
             GetInsertionMode() == kAfterAfterFramesetMode);
      String leading_whitespace = buffer.TakeRemainingWhitespace();
      if (!leading_whitespace.IsEmpty())
        tree_.InsertTextNode(leading_whitespace, kAllWhitespace);
      // FIXME: We should generate a parse error if we skipped over any
      // non-whitespace characters.
      break;
    }
    case kInSelectInTableMode:
    case kInSelectMode: {
      DCHECK(GetInsertionMode() == kInSelectMode ||
             GetInsertionMode() == kInSelectInTableMode);
      tree_.InsertTextNode(buffer.TakeRemaining());
      break;
    }
    case kAfterAfterFramesetMode: {
      String leading_whitespace = buffer.TakeRemainingWhitespace();
      if (!leading_whitespace.IsEmpty()) {
        tree_.ReconstructTheActiveFormattingElements();
        tree_.InsertTextNode(leading_whitespace, kAllWhitespace);
      }
      // FIXME: We should generate a parse error if we skipped over any
      // non-whitespace characters.
      break;
    }
  }
}

void HTMLTreeBuilder::ProcessCharacterBufferForInBody(
    CharacterTokenBuffer& buffer) {
  tree_.ReconstructTheActiveFormattingElements();
  StringView characters = buffer.TakeRemaining();
  tree_.InsertTextNode(characters);
  if (frameset_ok_ && !IsAllWhitespaceOrReplacementCharacters(characters))
    frameset_ok_ = false;
}

void HTMLTreeBuilder::ProcessEndOfFile(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kEndOfFile);
  switch (GetInsertionMode()) {
    case kInitialMode:
      DCHECK_EQ(GetInsertionMode(), kInitialMode);
      DefaultForInitial();
      FALLTHROUGH;
    case kBeforeHTMLMode:
      DCHECK_EQ(GetInsertionMode(), kBeforeHTMLMode);
      DefaultForBeforeHTML();
      FALLTHROUGH;
    case kBeforeHeadMode:
      DCHECK_EQ(GetInsertionMode(), kBeforeHeadMode);
      DefaultForBeforeHead();
      FALLTHROUGH;
    case kInHeadMode:
      DCHECK_EQ(GetInsertionMode(), kInHeadMode);
      DefaultForInHead();
      FALLTHROUGH;
    case kAfterHeadMode:
      DCHECK_EQ(GetInsertionMode(), kAfterHeadMode);
      DefaultForAfterHead();
      FALLTHROUGH;
    case kInBodyMode:
    case kInCellMode:
    case kInCaptionMode:
    case kInRowMode:
      DCHECK(GetInsertionMode() == kInBodyMode ||
             GetInsertionMode() == kInCellMode ||
             GetInsertionMode() == kInCaptionMode ||
             GetInsertionMode() == kInRowMode ||
             GetInsertionMode() == kTemplateContentsMode);
      // Emit parse error based on what elements are still open.
      DVLOG(1) << "Not implemented.";
      if (!template_insertion_modes_.IsEmpty() &&
          ProcessEndOfFileForInTemplateContents(token))
        return;
      break;
    case kAfterBodyMode:
    case kAfterAfterBodyMode:
      DCHECK(GetInsertionMode() == kAfterBodyMode ||
             GetInsertionMode() == kAfterAfterBodyMode);
      break;
    case kInHeadNoscriptMode:
      DCHECK_EQ(GetInsertionMode(), kInHeadNoscriptMode);
      DefaultForInHeadNoscript();
      ProcessEndOfFile(token);
      return;
    case kAfterFramesetMode:
    case kAfterAfterFramesetMode:
      DCHECK(GetInsertionMode() == kAfterFramesetMode ||
             GetInsertionMode() == kAfterAfterFramesetMode);
      break;
    case kInColumnGroupMode:
      if (tree_.CurrentIsRootNode()) {
        DCHECK(IsParsingFragment());
        return;  // FIXME: Should we break here instead of returning?
      }
      DCHECK(tree_.CurrentNode()->HasTagName(kColgroupTag) ||
             IsHTMLTemplateElement(tree_.CurrentNode()));
      ProcessColgroupEndTagForInColumnGroup();
      FALLTHROUGH;
    case kInFramesetMode:
    case kInTableMode:
    case kInTableBodyMode:
    case kInSelectInTableMode:
    case kInSelectMode:
      DCHECK(GetInsertionMode() == kInSelectMode ||
             GetInsertionMode() == kInSelectInTableMode ||
             GetInsertionMode() == kInTableMode ||
             GetInsertionMode() == kInFramesetMode ||
             GetInsertionMode() == kInTableBodyMode ||
             GetInsertionMode() == kInColumnGroupMode);
      if (tree_.CurrentNode() != tree_.OpenElements()->RootNode())
        ParseError(token);
      if (!template_insertion_modes_.IsEmpty() &&
          ProcessEndOfFileForInTemplateContents(token))
        return;
      break;
    case kInTableTextMode:
      DefaultForInTableText();
      ProcessEndOfFile(token);
      return;
    case kTextMode: {
      ParseError(token);
      if (tree_.CurrentStackItem()->HasTagName(kScriptTag)) {
        // Mark the script element as "already started".
        DVLOG(1) << "Not implemented.";
      }
      Element* el = tree_.OpenElements()->Top();
      if (IsHTMLTextAreaElement(el))
        ToHTMLFormControlElement(el)->SetBlocksFormSubmission(true);
      tree_.OpenElements()->Pop();
      DCHECK_NE(original_insertion_mode_, kTextMode);
      SetInsertionMode(original_insertion_mode_);
      ProcessEndOfFile(token);
      return;
    }
    case kTemplateContentsMode:
      if (ProcessEndOfFileForInTemplateContents(token))
        return;
      break;
  }
  tree_.ProcessEndOfFile();
}

void HTMLTreeBuilder::DefaultForInitial() {
  DVLOG(1) << "Not implemented.";
  tree_.SetDefaultCompatibilityMode();
  // FIXME: parse error
  SetInsertionMode(kBeforeHTMLMode);
}

void HTMLTreeBuilder::DefaultForBeforeHTML() {
  AtomicHTMLToken start_html(HTMLToken::kStartTag, kHTMLTag.LocalName());
  tree_.InsertHTMLHtmlStartTagBeforeHTML(&start_html);
  SetInsertionMode(kBeforeHeadMode);
}

void HTMLTreeBuilder::DefaultForBeforeHead() {
  AtomicHTMLToken start_head(HTMLToken::kStartTag, kHeadTag.LocalName());
  ProcessStartTag(&start_head);
}

void HTMLTreeBuilder::DefaultForInHead() {
  AtomicHTMLToken end_head(HTMLToken::kEndTag, kHeadTag.LocalName());
  ProcessEndTag(&end_head);
}

void HTMLTreeBuilder::DefaultForInHeadNoscript() {
  AtomicHTMLToken end_noscript(HTMLToken::kEndTag, kNoscriptTag.LocalName());
  ProcessEndTag(&end_noscript);
}

void HTMLTreeBuilder::DefaultForAfterHead() {
  AtomicHTMLToken start_body(HTMLToken::kStartTag, kBodyTag.LocalName());
  ProcessStartTag(&start_body);
  frameset_ok_ = true;
}

void HTMLTreeBuilder::DefaultForInTableText() {
  String characters = pending_table_characters_.ToString();
  pending_table_characters_.Clear();
  if (!IsAllWhitespace(characters)) {
    // FIXME: parse error
    HTMLConstructionSite::RedirectToFosterParentGuard redirecter(tree_);
    tree_.ReconstructTheActiveFormattingElements();
    tree_.InsertTextNode(characters, kNotAllWhitespace);
    frameset_ok_ = false;
    SetInsertionMode(original_insertion_mode_);
    return;
  }
  tree_.InsertTextNode(characters);
  SetInsertionMode(original_insertion_mode_);
}

bool HTMLTreeBuilder::ProcessStartTagForInHead(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kStartTag);
  if (token->GetName() == kHTMLTag) {
    ProcessHtmlStartTagForInBody(token);
    return true;
  }
  if (token->GetName() == kBaseTag || token->GetName() == kBasefontTag ||
      token->GetName() == kBgsoundTag || token->GetName() == kCommandTag ||
      token->GetName() == kLinkTag || token->GetName() == kMetaTag) {
    tree_.InsertSelfClosingHTMLElementDestroyingToken(token);
    // Note: The custom processing for the <meta> tag is done in
    // HTMLMetaElement::process().
    return true;
  }
  if (token->GetName() == kTitleTag) {
    ProcessGenericRCDATAStartTag(token);
    return true;
  }
  if (token->GetName() == kNoscriptTag) {
    if (options_.script_enabled) {
      ProcessGenericRawTextStartTag(token);
      return true;
    }
    tree_.InsertHTMLElement(token);
    SetInsertionMode(kInHeadNoscriptMode);
    return true;
  }
  if (token->GetName() == kNoframesTag || token->GetName() == kStyleTag) {
    ProcessGenericRawTextStartTag(token);
    return true;
  }
  if (token->GetName() == kScriptTag) {
    ProcessScriptStartTag(token);
    return true;
  }
  if (token->GetName() == kTemplateTag) {
    ProcessTemplateStartTag(token);
    return true;
  }
  if (token->GetName() == kHeadTag) {
    ParseError(token);
    return true;
  }
  return false;
}

void HTMLTreeBuilder::ProcessGenericRCDATAStartTag(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kStartTag);
  tree_.InsertHTMLElement(token);
  if (parser_->Tokenizer())
    parser_->Tokenizer()->SetState(HTMLTokenizer::kRCDATAState);
  original_insertion_mode_ = insertion_mode_;
  SetInsertionMode(kTextMode);
}

void HTMLTreeBuilder::ProcessGenericRawTextStartTag(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kStartTag);
  tree_.InsertHTMLElement(token);
  if (parser_->Tokenizer())
    parser_->Tokenizer()->SetState(HTMLTokenizer::kRAWTEXTState);
  original_insertion_mode_ = insertion_mode_;
  SetInsertionMode(kTextMode);
}

void HTMLTreeBuilder::ProcessScriptStartTag(AtomicHTMLToken* token) {
  DCHECK_EQ(token->GetType(), HTMLToken::kStartTag);
  tree_.InsertScriptElement(token);
  if (parser_->Tokenizer())
    parser_->Tokenizer()->SetState(HTMLTokenizer::kScriptDataState);
  original_insertion_mode_ = insertion_mode_;

  TextPosition position = parser_->GetTextPosition();

  script_to_process_start_position_ = position;

  SetInsertionMode(kTextMode);
}

// http://www.whatwg.org/specs/web-apps/current-work/multipage/tree-construction.html#tree-construction
bool HTMLTreeBuilder::ShouldProcessTokenInForeignContent(
    AtomicHTMLToken* token) {
  if (tree_.IsEmpty())
    return false;
  HTMLStackItem* adjusted_current_node = AdjustedCurrentStackItem();

  if (adjusted_current_node->IsInHTMLNamespace())
    return false;
  if (HTMLElementStack::IsMathMLTextIntegrationPoint(adjusted_current_node)) {
    if (token->GetType() == HTMLToken::kStartTag &&
        token->GetName() != mathml_names::kMglyphTag &&
        token->GetName() != mathml_names::kMalignmarkTag)
      return false;
    if (token->GetType() == HTMLToken::kCharacter)
      return false;
  }
  if (adjusted_current_node->HasTagName(mathml_names::kAnnotationXmlTag) &&
      token->GetType() == HTMLToken::kStartTag &&
      token->GetName() == svg_names::kSVGTag)
    return false;
  if (HTMLElementStack::IsHTMLIntegrationPoint(adjusted_current_node)) {
    if (token->GetType() == HTMLToken::kStartTag)
      return false;
    if (token->GetType() == HTMLToken::kCharacter)
      return false;
  }
  if (token->GetType() == HTMLToken::kEndOfFile)
    return false;
  return true;
}

void HTMLTreeBuilder::ProcessTokenInForeignContent(AtomicHTMLToken* token) {
  if (token->GetType() == HTMLToken::kCharacter) {
    const String& characters = token->Characters();
    tree_.InsertTextNode(characters);
    if (frameset_ok_ && !IsAllWhitespaceOrReplacementCharacters(characters))
      frameset_ok_ = false;
    return;
  }

  tree_.Flush(kFlushAlways);
  HTMLStackItem* adjusted_current_node = AdjustedCurrentStackItem();

  switch (token->GetType()) {
    case HTMLToken::kUninitialized:
      NOTREACHED();
      break;
    case HTMLToken::DOCTYPE:
      ParseError(token);
      break;
    case HTMLToken::kStartTag: {
      if (token->GetName() == kBTag || token->GetName() == kBigTag ||
          token->GetName() == kBlockquoteTag || token->GetName() == kBodyTag ||
          token->GetName() == kBrTag || token->GetName() == kCenterTag ||
          token->GetName() == kCodeTag || token->GetName() == kDdTag ||
          token->GetName() == kDivTag || token->GetName() == kDlTag ||
          token->GetName() == kDtTag || token->GetName() == kEmTag ||
          token->GetName() == kEmbedTag ||
          IsNumberedHeaderTag(token->GetName()) ||
          token->GetName() == kHeadTag || token->GetName() == kHrTag ||
          token->GetName() == kITag || token->GetName() == kImgTag ||
          token->GetName() == kLiTag || token->GetName() == kListingTag ||
          token->GetName() == kMenuTag || token->GetName() == kMetaTag ||
          token->GetName() == kNobrTag || token->GetName() == kOlTag ||
          token->GetName() == kPTag || token->GetName() == kPreTag ||
          token->GetName() == kRubyTag || token->GetName() == kSTag ||
          token->GetName() == kSmallTag || token->GetName() == kSpanTag ||
          token->GetName() == kStrongTag || token->GetName() == kStrikeTag ||
          token->GetName() == kSubTag || token->GetName() == kSupTag ||
          token->GetName() == kTableTag || token->GetName() == kTtTag ||
          token->GetName() == kUTag || token->GetName() == kUlTag ||
          token->GetName() == kVarTag ||
          (token->GetName() == kFontTag &&
           (token->GetAttributeItem(kColorAttr) ||
            token->GetAttributeItem(kFaceAttr) ||
            token->GetAttributeItem(kSizeAttr)))) {
        ParseError(token);
        tree_.OpenElements()->PopUntilForeignContentScopeMarker();
        ProcessStartTag(token);
        return;
      }
      if (token->GetName() == kScriptTag)
        script_to_process_start_position_ = parser_->GetTextPosition();
      const AtomicString& current_namespace =
          adjusted_current_node->NamespaceURI();
      if (current_namespace == mathml_names::kNamespaceURI)
        AdjustMathMLAttributes(token);
      if (current_namespace == svg_names::kNamespaceURI) {
        AdjustSVGTagNameCase(token);
        AdjustSVGAttributes(token);
      }
      AdjustForeignAttributes(token);
      tree_.InsertForeignElement(token, current_namespace);
      break;
    }
    case HTMLToken::kEndTag: {
      if (adjusted_current_node->NamespaceURI() == svg_names::kNamespaceURI)
        AdjustSVGTagNameCase(token);

      if (token->GetName() == svg_names::kScriptTag &&
          tree_.CurrentStackItem()->HasTagName(svg_names::kScriptTag)) {
        if (ScriptingContentIsAllowed(tree_.GetParserContentPolicy()))
          script_to_process_ = tree_.CurrentElement();
        tree_.OpenElements()->Pop();
        return;
      }
      if (!tree_.CurrentStackItem()->IsInHTMLNamespace()) {
        // FIXME: This code just wants an Element* iterator, instead of an
        // ElementRecord*
        HTMLElementStack::ElementRecord* node_record =
            tree_.OpenElements()->TopRecord();
        if (!node_record->StackItem()->HasLocalName(token->GetName()))
          ParseError(token);
        while (1) {
          if (node_record->StackItem()->HasLocalName(token->GetName())) {
            tree_.OpenElements()->PopUntilPopped(node_record->GetElement());
            return;
          }
          node_record = node_record->Next();

          if (node_record->StackItem()->IsInHTMLNamespace())
            break;
        }
      }
      // Otherwise, process the token according to the rules given in the
      // section corresponding to the current insertion mode in HTML content.
      ProcessEndTag(token);
      break;
    }
    case HTMLToken::kComment:
      tree_.InsertComment(token);
      break;
    case HTMLToken::kCharacter:
    case HTMLToken::kEndOfFile:
      NOTREACHED();
      break;
  }
}

void HTMLTreeBuilder::Finished() {
  if (IsParsingFragment())
    return;

  DCHECK(template_insertion_modes_.IsEmpty());
#if DCHECK_IS_ON()
  DCHECK(is_attached_);
#endif
  // Warning, this may detach the parser. Do not do anything else after this.
  tree_.FinishedParsing();
}

void HTMLTreeBuilder::ParseError(AtomicHTMLToken*) {}

#ifndef NDEBUG
const char* HTMLTreeBuilder::ToString(HTMLTreeBuilder::InsertionMode mode) {
  switch (mode) {
#define DEFINE_STRINGIFY(mode) \
  case mode:                   \
    return #mode;
    DEFINE_STRINGIFY(kInitialMode)
    DEFINE_STRINGIFY(kBeforeHTMLMode)
    DEFINE_STRINGIFY(kBeforeHeadMode)
    DEFINE_STRINGIFY(kInHeadMode)
    DEFINE_STRINGIFY(kInHeadNoscriptMode)
    DEFINE_STRINGIFY(kAfterHeadMode)
    DEFINE_STRINGIFY(kTemplateContentsMode)
    DEFINE_STRINGIFY(kInBodyMode)
    DEFINE_STRINGIFY(kTextMode)
    DEFINE_STRINGIFY(kInTableMode)
    DEFINE_STRINGIFY(kInTableTextMode)
    DEFINE_STRINGIFY(kInCaptionMode)
    DEFINE_STRINGIFY(kInColumnGroupMode)
    DEFINE_STRINGIFY(kInTableBodyMode)
    DEFINE_STRINGIFY(kInRowMode)
    DEFINE_STRINGIFY(kInCellMode)
    DEFINE_STRINGIFY(kInSelectMode)
    DEFINE_STRINGIFY(kInSelectInTableMode)
    DEFINE_STRINGIFY(kAfterBodyMode)
    DEFINE_STRINGIFY(kInFramesetMode)
    DEFINE_STRINGIFY(kAfterFramesetMode)
    DEFINE_STRINGIFY(kAfterAfterBodyMode)
    DEFINE_STRINGIFY(kAfterAfterFramesetMode)
#undef DEFINE_STRINGIFY
  }
  return "<unknown>";
}
#endif

}  // namespace blink
