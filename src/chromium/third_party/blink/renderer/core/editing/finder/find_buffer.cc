// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/editing/finder/find_buffer.h"

#include "third_party/blink/renderer/core/css/style_change_reason.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/range.h"
#include "third_party/blink/renderer/core/dom/text.h"
#include "third_party/blink/renderer/core/editing/editing_utilities.h"
#include "third_party/blink/renderer/core/editing/ephemeral_range.h"
#include "third_party/blink/renderer/core/editing/iterators/text_searcher_icu.h"
#include "third_party/blink/renderer/core/html/forms/html_form_control_element.h"
#include "third_party/blink/renderer/core/html/forms/html_select_element.h"
#include "third_party/blink/renderer/core/invisible_dom/invisible_dom.h"
#include "third_party/blink/renderer/core/layout/layout_block_flow.h"
#include "third_party/blink/renderer/core/layout/layout_object.h"
#include "third_party/blink/renderer/core/layout/ng/inline/ng_inline_node.h"
#include "third_party/blink/renderer/core/layout/ng/inline/ng_offset_mapping.h"
#include "third_party/blink/renderer/core/style/computed_style.h"
#include "third_party/blink/renderer/platform/text/unicode_utilities.h"
#include "third_party/blink/renderer/platform/wtf/text/character_names.h"
#include "third_party/blink/renderer/platform/wtf/text/unicode.h"

namespace blink {

FindBuffer::FindBuffer(const EphemeralRangeInFlatTree& range) {
  DCHECK(range.IsNotNull() && !range.IsCollapsed()) << range;
  CollectTextUntilBlockBoundary(range);
}

FindBuffer::Results::Results() {
  empty_result_ = true;
}

FindBuffer::Results::Results(const Vector<UChar>& buffer,
                             String search_text,
                             const blink::FindOptions options) {
  // We need to own the |search_text| because |text_searcher_| only has a
  // StringView (doesn't own the search text).
  search_text_ = search_text;
  text_searcher_.SetPattern(search_text_, options);
  text_searcher_.SetText(buffer.data(), buffer.size());
  text_searcher_.SetOffset(0);
}

FindBuffer::Results::Iterator::Iterator(TextSearcherICU* text_searcher,
                                        String search_text)
    : text_searcher_(text_searcher), has_match_(true) {
  operator++();
}

const FindBuffer::BufferMatchResult FindBuffer::Results::Iterator::operator*()
    const {
  DCHECK(has_match_);
  return FindBuffer::BufferMatchResult({match_.start, match_.length});
}

void FindBuffer::Results::Iterator::operator++() {
  DCHECK(has_match_);
  has_match_ = text_searcher_->NextMatchResult(match_);
}

FindBuffer::Results::Iterator FindBuffer::Results::begin() {
  if (empty_result_)
    return end();
  text_searcher_.SetOffset(0);
  return Iterator(&text_searcher_, search_text_);
}

FindBuffer::Results::Iterator FindBuffer::Results::end() const {
  return Iterator();
}

bool FindBuffer::Results::IsEmpty() {
  return begin() == end();
}

FindBuffer::BufferMatchResult FindBuffer::Results::front() {
  return *begin();
}

FindBuffer::BufferMatchResult FindBuffer::Results::back() {
  Iterator last_result;
  for (Iterator it = begin(); it != end(); ++it) {
    last_result = it;
  }
  return *last_result;
}

unsigned FindBuffer::Results::CountForTesting() {
  unsigned result = 0;
  for (Iterator it = begin(); it != end(); ++it) {
    ++result;
  }
  return result;
}

void FindBuffer::InvisibleLayoutScope::EnsureRecalc(Node& block_root) {
  if (!RuntimeEnabledFeatures::InvisibleDOMEnabled())
    return;
  if (did_recalc_)
    return;
  did_recalc_ = true;
  DCHECK(block_root.GetDocument().Lifecycle().GetState() >=
         DocumentLifecycle::kStyleClean);
  // If we're in an invisible subtree, we should recalc style from the invisible
  // root/the highest ancestor of |block_root| with the invisible attribute,
  // otherwise we should recalc from |block_root|.
  // InvisibleRoot is always non-null when IsInsideInvisibleSubtree is true.
  if (InvisibleDOM::IsInsideInvisibleSubtree(block_root))
    invisible_root_ = InvisibleDOM::InvisibleRoot(block_root);
  else
    invisible_root_ = &ToElement(block_root);

  DCHECK(invisible_root_);
  invisible_root_->GetDocument().SetFindInPageRoot(invisible_root_);
  invisible_root_->SetNeedsStyleRecalc(
      kSubtreeStyleChange,
      StyleChangeReasonForTracing::Create(style_change_reason::kFindInvisible));
  // TODO(rakina): This currently does layout too and might be expensive. In the
  // future, we might to figure out a way to make NGOffsetMapping work with only
  // style & layout tree so that we don't have to do layout here.
  invisible_root_->GetDocument().UpdateStyleAndLayout();
}

FindBuffer::InvisibleLayoutScope::~InvisibleLayoutScope() {
  if (!RuntimeEnabledFeatures::InvisibleDOMEnabled())
    return;
  if (!did_recalc_)
    return;
  invisible_root_->GetDocument().SetFindInPageRoot(nullptr);
  invisible_root_->SetNeedsStyleRecalc(
      kSubtreeStyleChange,
      StyleChangeReasonForTracing::Create(style_change_reason::kFindInvisible));
  invisible_root_->GetDocument().UpdateStyleAndLayout();
}

bool ShouldIgnoreContents(const Node& node) {
  const auto* element = DynamicTo<HTMLElement>(node);
  if (!element)
    return false;
  return (!element->ShouldSerializeEndTag() && !IsHTMLInputElement(*element)) ||
         IsHTMLIFrameElement(*element) || IsHTMLImageElement(*element) ||
         IsHTMLLegendElement(*element) || IsHTMLMeterElement(*element) ||
         IsHTMLObjectElement(*element) || IsHTMLProgressElement(*element) ||
         (IsHTMLSelectElement(*element) &&
          ToHTMLSelectElement(*element).UsesMenuList()) ||
         IsHTMLStyleElement(*element) || IsHTMLScriptElement(*element) ||
         IsHTMLVideoElement(*element) || IsHTMLAudioElement(*element) ||
         (element->GetDisplayLockContext() &&
          !element->GetDisplayLockContext()->IsActivatable());
}

Node* GetNonSearchableAncestor(const Node& node) {
  for (Node& ancestor : FlatTreeTraversal::InclusiveAncestorsOf(node)) {
    const ComputedStyle* style = ancestor.EnsureComputedStyle();
    if ((style && style->Display() == EDisplay::kNone) ||
        ShouldIgnoreContents(ancestor))
      return &ancestor;
    if (ancestor.IsDocumentNode())
      return nullptr;
  }
  return nullptr;
}

bool IsBlock(EDisplay display) {
  return display == EDisplay::kBlock || display == EDisplay::kTable ||
         display == EDisplay::kFlowRoot || display == EDisplay::kGrid ||
         display == EDisplay::kFlex || display == EDisplay::kListItem;
}

Node* GetVisibleTextNode(Node& start_node) {
  Node* node = &start_node;
  // Move to outside display none subtree if we're inside one.
  while (Node* ancestor = GetNonSearchableAncestor(*node)) {
    if (ancestor->IsDocumentNode())
      return nullptr;
    node = FlatTreeTraversal::NextSkippingChildren(*ancestor);
    if (!node)
      return nullptr;
  }
  // Move to first text node that's visible.
  while (node) {
    const ComputedStyle* style = node->EnsureComputedStyle();
    if (ShouldIgnoreContents(*node) ||
        (style && style->Display() == EDisplay::kNone)) {
      // This element and its descendants are not visible, skip it.
      node = FlatTreeTraversal::NextSkippingChildren(*node);
      continue;
    }
    if (style && style->Visibility() == EVisibility::kVisible &&
        node->IsTextNode()) {
      return node;
    }
    // This element is hidden, but node might be visible,
    // or this is not a text node, so we move on.
    node = FlatTreeTraversal::Next(*node);
  }
  return nullptr;
}

Node& GetLowestDisplayBlockInclusiveAncestor(const Node& start_node) {
  // Gets lowest inclusive ancestor that has block display value.
  // <div id=outer>a<div id=inner>b</div>c</div>
  // If we run this on "a" or "c" text node in we will get the outer div.
  // If we run it on the "b" text node we will get the inner div.
  for (Node& ancestor : FlatTreeTraversal::InclusiveAncestorsOf(start_node)) {
    const ComputedStyle* style = ancestor.EnsureComputedStyle();
    if (style && !ancestor.IsTextNode() && IsBlock(style->Display()))
      return ancestor;
  }
  return *start_node.GetDocument().documentElement();
}

EphemeralRangeInFlatTree FindBuffer::FindMatchInRange(
    const EphemeralRangeInFlatTree& range,
    String search_text,
    FindOptions options) {
  if (!range.StartPosition().IsConnected())
    return EphemeralRangeInFlatTree();

  EphemeralRangeInFlatTree last_match_range;
  Node* first_node = range.StartPosition().NodeAsRangeFirstNode();
  Node* past_last_node = range.EndPosition().NodeAsRangePastLastNode();
  Node* node = first_node;
  while (node && node != past_last_node) {
    if (GetNonSearchableAncestor(*node)) {
      node = FlatTreeTraversal::NextSkippingChildren(*node);
      continue;
    }
    if (!node->IsTextNode()) {
      node = FlatTreeTraversal::Next(*node);
      continue;
    }
    // If we're in the same node as the start position, start from the start
    // position instead of the start of this node.
    PositionInFlatTree start_position =
        node == first_node ? range.StartPosition()
                           : PositionInFlatTree::FirstPositionInNode(*node);
    if (start_position >= range.EndPosition())
      break;

    FindBuffer buffer(
        EphemeralRangeInFlatTree(start_position, range.EndPosition()));
    std::unique_ptr<Results> match_results =
        buffer.FindMatches(search_text, options);
    if (!match_results->IsEmpty()) {
      if (!(options & kBackwards)) {
        BufferMatchResult match = match_results->front();
        return buffer.RangeFromBufferIndex(match.start,
                                           match.start + match.length);
      }
      BufferMatchResult match = match_results->back();
      last_match_range =
          buffer.RangeFromBufferIndex(match.start, match.start + match.length);
    }
    node = buffer.PositionAfterBlock().ComputeContainerNode();
  }
  return last_match_range;
}

std::unique_ptr<FindBuffer::Results> FindBuffer::FindMatches(
    const WebString& search_text,
    const blink::FindOptions options) const {
  if (buffer_.IsEmpty() || search_text.length() > buffer_.size())
    return std::make_unique<Results>();
  String search_text_16_bit = search_text;
  search_text_16_bit.Ensure16Bit();
  FoldQuoteMarksAndSoftHyphens(search_text_16_bit);
  return std::make_unique<Results>(buffer_, search_text_16_bit, options);
}

bool FindBuffer::PushScopedForcedUpdateIfNeeded(const Element& element) {
  if (auto* context = element.GetDisplayLockContext()) {
    DCHECK(context->IsActivatable());
    scoped_forced_update_list_.push_back(context->GetScopedForcedUpdate());
    return true;
  }
  return false;
}

void FindBuffer::CollectScopedForcedUpdates(Node& start_node,
                                            const Node* search_range_end_node,
                                            const Node* node_after_block) {
  if (!RuntimeEnabledFeatures::DisplayLockingEnabled())
    return;
  if (start_node.GetDocument().LockedDisplayLockCount() ==
      start_node.GetDocument().ActivationBlockingDisplayLockCount())
    return;

  Node* node = &start_node;
  // We assume |start_node| is always visible/activatable if locked, so we don't
  // need to check activatability of ancestors here.
  for (Node& ancestor : FlatTreeTraversal::InclusiveAncestorsOf(*node)) {
    if (!ancestor.IsElementNode())
      continue;
    PushScopedForcedUpdateIfNeeded(ToElement(ancestor));
  }

  while (node && node != node_after_block && node != search_range_end_node) {
    if (ShouldIgnoreContents(*node)) {
      // Will skip display:none/non-activatable locked subtrees/etc.
      node = FlatTreeTraversal::NextSkippingChildren(*node);
      continue;
    }
    if (node->IsElementNode())
      PushScopedForcedUpdateIfNeeded(ToElement(*node));
    node = FlatTreeTraversal::Next(*node);
  }
}

// Collects text until block boundary located at or after |start_node|
// to |buffer_|. Saves the next starting node after the block to
// |node_after_block_|.
void FindBuffer::CollectTextUntilBlockBoundary(
    const EphemeralRangeInFlatTree& range) {
  DCHECK(range.IsNotNull() && !range.IsCollapsed()) << range;

  node_after_block_ = nullptr;
  const Node* const first_node = range.StartPosition().NodeAsRangeFirstNode();
  if (!first_node)
    return;
  // Get first visible text node from |start_position|.
  Node* node =
      GetVisibleTextNode(*range.StartPosition().NodeAsRangeFirstNode());
  if (!node || !node->isConnected())
    return;

  Node& block_ancestor = GetLowestDisplayBlockInclusiveAncestor(*node);
  const Node* just_after_block = FlatTreeTraversal::Next(
      FlatTreeTraversal::LastWithinOrSelf(block_ancestor));
  const LayoutBlockFlow* last_block_flow = nullptr;

  // Calculate layout tree and style for invisible nodes inside the whole
  // subtree of |block_ancestor|.
  if (RuntimeEnabledFeatures::InvisibleDOMEnabled() && node &&
      InvisibleDOM::IsInsideInvisibleSubtree(*node))
    invisible_layout_scope_.EnsureRecalc(block_ancestor);

  // Collect all text under |block_ancestor| to |buffer_|,
  // unless we meet another block on the way. If so, we should split.
  // Example: <div id="outer">a<span>b</span>c<div>d</div></div>
  // Will try to collect all text in outer div but will actually
  // stop when it encounters the inner div. So buffer will be "abc".
  Node* const first_traversed_node = node;
  // We will also stop if we encountered/passed |end_node|.
  Node* end_node = range.EndPosition().NodeAsRangeLastNode();

  if (node) {
    CollectScopedForcedUpdates(*node, end_node, just_after_block);
    if (!scoped_forced_update_list_.IsEmpty())
      node->GetDocument().UpdateStyleAndLayout();
  }

  while (node && node != just_after_block) {
    if (ShouldIgnoreContents(*node)) {
      if (end_node && (end_node == node ||
                       FlatTreeTraversal::IsDescendantOf(*end_node, *node))) {
        // For setting |node_after_block| later.
        node = FlatTreeTraversal::NextSkippingChildren(*node);
        break;
      }
      // Move the node so we wouldn't encounter this node or its descendants
      // later.
      if (!IsHTMLWBRElement(To<HTMLElement>(*node)))
        buffer_.push_back(kObjectReplacementCharacter);
      node = FlatTreeTraversal::NextSkippingChildren(*node);
      continue;
    }
    if (RuntimeEnabledFeatures::InvisibleDOMEnabled() &&
        node->IsElementNode() && ToElement(node)->HasInvisibleAttribute() &&
        !invisible_layout_scope_.DidRecalc()) {
      // We found and invisible node. Calculate the layout & style for the whole
      // block at once, and we need to recalculate the NGOffsetMapping and start
      // from the beginning again because the layout tree had changed.
      mapping_needs_recalc_ = true;
      node = first_traversed_node;
      last_block_flow = nullptr;
      buffer_.clear();
      invisible_layout_scope_.EnsureRecalc(block_ancestor);
      continue;
    }
    const ComputedStyle* style = node->EnsureComputedStyle();
    if (style->Display() == EDisplay::kNone) {
      // This element and its descendants are not visible, skip it.
      // We can safely just check the computed style of this node since
      // we guarantee |block_ancestor| is visible.
      if (end_node && (end_node == node ||
                       FlatTreeTraversal::IsDescendantOf(*end_node, *node))) {
        // For setting |node_after_block| later.
        node = FlatTreeTraversal::NextSkippingChildren(*node);
        break;
      }
      node = FlatTreeTraversal::NextSkippingChildren(*node);
      if (node && !FlatTreeTraversal::IsDescendantOf(*node, block_ancestor))
        break;
      continue;
    }
    // This node is in its own sub-block separate from our starting position.
    const auto* text_node = DynamicTo<Text>(node);
    if (first_traversed_node != node && !text_node &&
        IsBlock(style->Display())) {
      break;
    }

    if (style->Visibility() == EVisibility::kVisible && text_node &&
        node->GetLayoutObject()) {
      LayoutBlockFlow& block_flow =
          *NGOffsetMapping::GetInlineFormattingContextOf(
              *text_node->GetLayoutObject());
      if (last_block_flow && last_block_flow != block_flow) {
        // We enter another block flow.
        break;
      }
      if (!last_block_flow) {
        last_block_flow = &block_flow;
      }
      AddTextToBuffer(*text_node, block_flow, range);
    }
    if (node == end_node) {
      node = FlatTreeTraversal::Next(*node);
      break;
    }
    node = FlatTreeTraversal::Next(*node);
  }
  node_after_block_ = node;
  FoldQuoteMarksAndSoftHyphens(buffer_.data(), buffer_.size());
}

EphemeralRangeInFlatTree FindBuffer::RangeFromBufferIndex(
    unsigned start_index,
    unsigned end_index) const {
  DCHECK_LE(start_index, end_index);
  PositionInFlatTree start_position =
      PositionAtStartOfCharacterAtIndex(start_index);
  PositionInFlatTree end_position =
      PositionAtEndOfCharacterAtIndex(end_index - 1);
  return EphemeralRangeInFlatTree(start_position, end_position);
}

FindBuffer::BufferNodeMapping FindBuffer::MappingForIndex(
    unsigned index) const {
  // Get the first entry that starts at a position higher than offset, and
  // move back one entry.
  auto* it = std::upper_bound(
      buffer_node_mappings_.begin(), buffer_node_mappings_.end(), index,
      [](const unsigned offset, const BufferNodeMapping& entry) {
        return offset < entry.offset_in_buffer;
      });
  DCHECK_NE(it, buffer_node_mappings_.begin());
  auto* const entry = std::prev(it);
  return *entry;
}

PositionInFlatTree FindBuffer::PositionAtStartOfCharacterAtIndex(
    unsigned index) const {
  DCHECK_LT(index, buffer_.size());
  BufferNodeMapping entry = MappingForIndex(index);
  return ToPositionInFlatTree(offset_mapping_->GetLastPosition(
      index - entry.offset_in_buffer + entry.offset_in_mapping));
}

PositionInFlatTree FindBuffer::PositionAtEndOfCharacterAtIndex(
    unsigned index) const {
  DCHECK_LT(index, buffer_.size());
  BufferNodeMapping entry = MappingForIndex(index);
  return ToPositionInFlatTree(offset_mapping_->GetFirstPosition(
      index - entry.offset_in_buffer + entry.offset_in_mapping + 1));
}

void FindBuffer::AddTextToBuffer(const Text& text_node,
                                 LayoutBlockFlow& block_flow,
                                 const EphemeralRangeInFlatTree& range) {
  if (!offset_mapping_ || mapping_needs_recalc_) {
    offset_mapping_ = NGInlineNode::GetOffsetMapping(&block_flow);
    mapping_needs_recalc_ = false;
  }

  Position node_start =
      (&text_node == range.StartPosition().ComputeContainerNode())
          ? ToPositionInDOMTree(range.StartPosition().ToOffsetInAnchor())
          : Position::FirstPositionInNode(text_node);
  Position node_end =
      (&text_node == range.EndPosition().ComputeContainerNode())
          ? ToPositionInDOMTree(range.EndPosition().ToOffsetInAnchor())
          : Position::LastPositionInNode(text_node);
  unsigned last_unit_end = 0;
  bool first_unit = true;
  const String mapped_text = offset_mapping_->GetText();
  for (const NGOffsetMappingUnit& unit :
       offset_mapping_->GetMappingUnitsForDOMRange(
           EphemeralRange(node_start, node_end))) {
    if (first_unit || last_unit_end != unit.TextContentStart()) {
      // This is the first unit, or the units are not consecutive, so we need to
      // insert a new BufferNodeMapping.
      buffer_node_mappings_.push_back(
          BufferNodeMapping({buffer_.size(), unit.TextContentStart()}));
      first_unit = false;
    }
    String text_for_unit =
        mapped_text.Substring(unit.TextContentStart(),
                              unit.TextContentEnd() - unit.TextContentStart());
    text_for_unit.Ensure16Bit();
    text_for_unit.Replace('\n', kObjectReplacementCharacter);
    buffer_.Append(text_for_unit.Characters16(), text_for_unit.length());
    last_unit_end = unit.TextContentEnd();
  }
}

}  // namespace blink
