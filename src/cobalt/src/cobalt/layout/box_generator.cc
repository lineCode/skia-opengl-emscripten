// Copyright 2014 The Cobalt Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cobalt/layout/box_generator.h"

#include <memory>
#include <string>

#include "base/bind.h"
#include "base/trace_event/trace_event.h"
#include "cobalt/cssom/computed_style.h"
#include "cobalt/cssom/css_computed_style_declaration.h"
#include "cobalt/cssom/css_transition_set.h"
#include "cobalt/cssom/keyword_value.h"
#include "cobalt/cssom/property_definitions.h"
#include "cobalt/cssom/property_value_visitor.h"
#include "cobalt/cssom/url_value.h"
#include "cobalt/dom/html_br_element.h"
#include "cobalt/dom/html_element.h"
#include "cobalt/dom/html_video_element.h"

// not in spec
#include "cobalt/dom/html_skottie_element.h"
#include "cobalt/dom/html_custom_element.h"

#include "cobalt/dom/text.h"
#include "cobalt/layout/base_direction.h"
#include "cobalt/layout/block_formatting_block_container_box.h"
#include "cobalt/layout/block_level_replaced_box.h"
#include "cobalt/layout/inline_container_box.h"
#include "cobalt/layout/inline_level_replaced_box.h"
#include "cobalt/layout/layout_boxes.h"
#include "cobalt/layout/layout_stat_tracker.h"
#include "cobalt/layout/text_box.h"
#include "cobalt/layout/used_style.h"
#include "cobalt/layout/white_space_processing.h"
#include "cobalt/media/base/video_frame_provider.h"
#include "cobalt/render_tree/image.h"
#include "cobalt/web_animations/keyframe_effect_read_only.h"
#include "starboard/decode_target.h"

// not in spec
#include "cobalt/layout/inline_level_skottie_box.h"
#include "cobalt/layout/block_level_skottie_box.h"

namespace cobalt {
namespace layout {

using media::VideoFrameProvider;

namespace {

scoped_refptr<render_tree::Image> GetVideoFrame(
    const scoped_refptr<VideoFrameProvider>& frame_provider,
    render_tree::ResourceProvider* resource_provider) {
  TRACE_EVENT0("cobalt::layout", "GetVideoFrame()");
  SbDecodeTarget decode_target = frame_provider->GetCurrentSbDecodeTarget();
  if (SbDecodeTargetIsValid(decode_target)) {
#if SB_HAS(GRAPHICS)
    return resource_provider->CreateImageFromSbDecodeTarget(decode_target);
#else   // SB_HAS(GRAPHICS)
    SB_UNREFERENCED_PARAMETER(resource_provider);
    return NULL;
#endif  // SB_HAS(GRAPHICS)
  } else {
    DCHECK(frame_provider);
    return NULL;
  }
}

}  // namespace

BoxGenerator::BoxGenerator(
    const scoped_refptr<cssom::CSSComputedStyleDeclaration>&
        parent_css_computed_style_declaration,
    const scoped_refptr<const web_animations::AnimationSet>& parent_animations,
    scoped_refptr<Paragraph>* paragraph, const int dom_element_depth,
    const Context* context)
    : parent_css_computed_style_declaration_(
          parent_css_computed_style_declaration),
      parent_animations_(parent_animations),
      paragraph_(paragraph),
      dom_element_depth_(dom_element_depth),
      context_(context) {}

BoxGenerator::~BoxGenerator() {
  // Later code assumes that if layout_boxes() is non-null, then it contains
  // more than one box.  This allows us to avoid some allocations of LayoutBoxes
  // objects.  We don't need to worry about setting layout_boxes() back to
  // null because this should end up being done in html_element.cc when the
  // boxes become invalidated.
  if (generating_html_element_ && !boxes_.empty()) {
    generating_html_element_->set_layout_boxes(
        std::unique_ptr<dom::LayoutBoxes>(new LayoutBoxes(std::move(boxes_))));
  }
}

void BoxGenerator::Visit(dom::Element* element) {
  if (dom_element_depth_ > context_->dom_max_element_depth) {
    LOG(WARNING) << "Elements too deep in the DOM tree are ignored in layout.";
    return;
  }

  scoped_refptr<dom::HTMLElement> html_element = element->AsHTMLElement();
  if (!html_element) {
    return;
  }
  generating_html_element_ = html_element;

  bool partial_layout_is_enabled = true;
#if defined(ENABLE_PARTIAL_LAYOUT_CONTROL)
  partial_layout_is_enabled =
      html_element->node_document()->partial_layout_is_enabled();
#endif  // defined(ENABLE_PARTIAL_LAYOUT_CONTROL)

  // If the html element already has layout boxes, we can reuse them.
  if (partial_layout_is_enabled && html_element->layout_boxes()) {
    LayoutBoxes* layout_boxes =
        base::polymorphic_downcast<LayoutBoxes*>(html_element->layout_boxes());
    DCHECK(boxes_.empty());
    DCHECK(!layout_boxes->boxes().empty());
    if (layout_boxes->boxes().front()->GetLevel() == Box::kBlockLevel) {
      boxes_ = layout_boxes->boxes();
      for (Boxes::const_iterator box_iterator = boxes_.begin();
           box_iterator != boxes_.end(); ++box_iterator) {
        Box* box = (*box_iterator).get();
        do {
          box->InvalidateParent();
          box = box->GetSplitSibling();
        } while (box != NULL);
      }
      return;
    }
  }

  scoped_refptr<dom::HTMLVideoElement> video_element =
      html_element->AsHTMLVideoElement();
  if (video_element) {
    VisitVideoElement(video_element.get());
    return;
  }

  scoped_refptr<dom::HTMLBRElement> br_element =
      html_element->AsHTMLBRElement();
  if (br_element) {
    VisitBrElement(br_element.get());
    return;
  }

  // not in spec
  scoped_refptr<dom::HTMLSkottieElement> skottie_element =
      html_element->AsHTMLSkottieElement();
  if (skottie_element) {
    VisitSkottieElement(skottie_element.get());
    return;
  }

  // not in spec
  scoped_refptr<dom::HTMLCustomElement> custom_element =
      html_element->AsHTMLCustomElement();
  if (custom_element) {
    VisitCustomElement(custom_element.get());
    return;
  }

  VisitNonReplacedElement(html_element.get());
}

namespace {

class SkottieBoxGenerator : public cssom::NotReachedPropertyValueVisitor {
 public:
  SkottieBoxGenerator(const scoped_refptr<cssom::CSSComputedStyleDeclaration>&
                           css_computed_style_declaration,
                       //const SkottieBox::SetBoundsCB& set_bounds_cb,
#if defined(ENABLE_SKOTTIE)
                       const SkottieBox::GetSkottieAnimCB& replace_skottie_animation_cb,
#endif // ENABLE_SKOTTIE
                       const scoped_refptr<Paragraph>& paragraph,
                       int32 text_position,
                       const base::Optional<LayoutUnit>& maybe_intrinsic_width,
                       const base::Optional<LayoutUnit>& maybe_intrinsic_height,
                       const base::Optional<float>& maybe_intrinsic_ratio,
                       const BoxGenerator::Context* context,
                       math::SizeF content_size)
      : css_computed_style_declaration_(css_computed_style_declaration),
       // set_bounds_cb_(set_bounds_cb),
#if defined(ENABLE_SKOTTIE)
        replace_skottie_animation_cb_(replace_skottie_animation_cb),
#endif // ENABLE_SKOTTIE
        paragraph_(paragraph),
        text_position_(text_position),
        maybe_intrinsic_width_(maybe_intrinsic_width),
        maybe_intrinsic_height_(maybe_intrinsic_height),
        maybe_intrinsic_ratio_(maybe_intrinsic_ratio),
        context_(context),
        content_size_(content_size) {}

  void VisitKeyword(cssom::KeywordValue* keyword) override;

  const scoped_refptr<SkottieBox>& skottie_box() { return skottie_box_; }

 private:
  const scoped_refptr<cssom::CSSComputedStyleDeclaration>
      css_computed_style_declaration_;
  //const SkottieBox::SetBoundsCB set_bounds_cb_;
#if defined(ENABLE_SKOTTIE)
  const SkottieBox::GetSkottieAnimCB replace_skottie_animation_cb_;
#endif // ENABLE_SKOTTIE
  const scoped_refptr<Paragraph> paragraph_;
  const int32 text_position_;
  const base::Optional<LayoutUnit> maybe_intrinsic_width_;
  const base::Optional<LayoutUnit> maybe_intrinsic_height_;
  const base::Optional<float> maybe_intrinsic_ratio_;
  const BoxGenerator::Context* context_;
  math::SizeF content_size_;

  scoped_refptr<SkottieBox> skottie_box_;
};

void SkottieBoxGenerator::VisitKeyword(cssom::KeywordValue* keyword) {
  switch (keyword->value()) {
    case cssom::KeywordValue::kBlock:
    case cssom::KeywordValue::kFlex:
      skottie_box_ = WrapRefCounted(new BlockLevelSkottieBox(
          css_computed_style_declaration_
          //, replace_image_cb_, set_bounds_cb_,
#if defined(ENABLE_SKOTTIE)
          , replace_skottie_animation_cb_
#endif // ENABLE_SKOTTIE
          , paragraph_, text_position_, maybe_intrinsic_width_,
          maybe_intrinsic_height_, maybe_intrinsic_ratio_,
          context_->used_style_provider,
          //, is_video_punched_out_
          content_size_,
          context_->layout_stat_tracker));
      break;
    case cssom::KeywordValue::kInline:
    case cssom::KeywordValue::kInlineBlock:
    case cssom::KeywordValue::kInlineFlex:
      skottie_box_ = WrapRefCounted(new InlineLevelSkottieBox(
          css_computed_style_declaration_,
#if defined(ENABLE_SKOTTIE)
          replace_skottie_animation_cb_,
#endif // ENABLE_SKOTTIE
          //set_bounds_cb_,
          paragraph_, text_position_, maybe_intrinsic_width_,
          maybe_intrinsic_height_, maybe_intrinsic_ratio_,
          context_->used_style_provider, content_size_,
          context_->layout_stat_tracker));
      break;
    // The element generates no boxes and has no effect on layout.
    case cssom::KeywordValue::kNone:
      // Leave |skottie_box_| NULL.
      break;
    case cssom::KeywordValue::kAbsolute:
    case cssom::KeywordValue::kAlternate:
    case cssom::KeywordValue::kAlternateReverse:
    case cssom::KeywordValue::kAuto:
    case cssom::KeywordValue::kBackwards:
    case cssom::KeywordValue::kBaseline:
    case cssom::KeywordValue::kBoth:
    case cssom::KeywordValue::kBottom:
    case cssom::KeywordValue::kBreakWord:
    case cssom::KeywordValue::kCenter:
    case cssom::KeywordValue::kClip:
    case cssom::KeywordValue::kCollapse:
    case cssom::KeywordValue::kColumn:
    case cssom::KeywordValue::kColumnReverse:
    case cssom::KeywordValue::kContain:
    case cssom::KeywordValue::kContent:
    case cssom::KeywordValue::kCover:
    case cssom::KeywordValue::kCurrentColor:
    case cssom::KeywordValue::kCursive:
    case cssom::KeywordValue::kEllipsis:
    case cssom::KeywordValue::kEnd:
    case cssom::KeywordValue::kEquirectangular:
    case cssom::KeywordValue::kFantasy:
    case cssom::KeywordValue::kFixed:
    case cssom::KeywordValue::kFlexEnd:
    case cssom::KeywordValue::kFlexStart:
    case cssom::KeywordValue::kForwards:
    case cssom::KeywordValue::kHidden:
    case cssom::KeywordValue::kInfinite:
    case cssom::KeywordValue::kInherit:
    case cssom::KeywordValue::kInitial:
    case cssom::KeywordValue::kLeft:
    case cssom::KeywordValue::kLineThrough:
    case cssom::KeywordValue::kMiddle:
    case cssom::KeywordValue::kMonoscopic:
    case cssom::KeywordValue::kMonospace:
    case cssom::KeywordValue::kNoRepeat:
    case cssom::KeywordValue::kNormal:
    case cssom::KeywordValue::kNowrap:
    case cssom::KeywordValue::kPre:
    case cssom::KeywordValue::kPreLine:
    case cssom::KeywordValue::kPreWrap:
    case cssom::KeywordValue::kRelative:
    case cssom::KeywordValue::kRepeat:
    case cssom::KeywordValue::kReverse:
    case cssom::KeywordValue::kRight:
    case cssom::KeywordValue::kRow:
    case cssom::KeywordValue::kRowReverse:
    case cssom::KeywordValue::kSansSerif:
    case cssom::KeywordValue::kScroll:
    case cssom::KeywordValue::kSerif:
    case cssom::KeywordValue::kSolid:
    case cssom::KeywordValue::kSpaceAround:
    case cssom::KeywordValue::kSpaceBetween:
    case cssom::KeywordValue::kStart:
    case cssom::KeywordValue::kStatic:
    case cssom::KeywordValue::kStereoscopicLeftRight:
    case cssom::KeywordValue::kStereoscopicTopBottom:
    case cssom::KeywordValue::kStretch:
    case cssom::KeywordValue::kTop:
    case cssom::KeywordValue::kUppercase:
    case cssom::KeywordValue::kVisible:
    case cssom::KeywordValue::kWrap:
    case cssom::KeywordValue::kWrapReverse:
      NOTREACHED();
      break;
  }
}

}  // namespace

// not in spec
void BoxGenerator::VisitCustomElement(dom::HTMLCustomElement* custom_element) {
  custom_element->onBoxGeneratorVisit(*this, custom_element);
}

// not in spec
void BoxGenerator::VisitSkottieElement(dom::HTMLSkottieElement* skottie_element) {
  printf("BoxGenerator::VisitSkottieElement\n");
  //   https://www.w3.org/TR/CSS21/visuren.html#inline-boxes
  //   https://www.w3.org/TR/CSS21/visuren.html#propdef-unicode-bidi
  //   https://www.w3.org/TR/css3-text/#line-break-details
  int32 text_position =
      (*paragraph_)
          ->AppendCodePoint(Paragraph::kObjectReplacementCharacterCodePoint);

  render_tree::ResourceProvider* resource_provider =
      *skottie_element->node_document()
           ->html_element_context()
           ->resource_provider();

  SkottieBoxGenerator skottie_box_generator(
      skottie_element->css_computed_style_declaration(),
      //skottie_element->GetSetBoundsCB(),
#if defined(ENABLE_SKOTTIE)
      base::Bind(&dom::HTMLSkottieElement::GetSkottieAnim, base::Unretained(skottie_element)),
#endif // ENABLE_SKOTTIE
      *paragraph_, text_position,
      base::nullopt, base::nullopt, base::nullopt, context_,
      skottie_element->GetSize());
  skottie_element->computed_style()->display()->Accept(&skottie_box_generator);

  printf("VisitSkottieElement with src: %s\n", skottie_element->src().c_str());

  scoped_refptr<SkottieBox> skottie_box =
      skottie_box_generator.skottie_box();
  if (skottie_box.get() == NULL) {
    // The element with "display: none" generates no boxes and has no effect
    // on layout. Descendant elements do not generate any boxes either.
    // This behavior cannot be overridden by setting the "display" property on
    // the descendants.
    //   https://www.w3.org/TR/CSS21/visuren.html#display-prop
    return;
  }

#ifdef COBALT_BOX_DUMP_ENABLED
  skottie_box->SetGeneratingNode(skottie_element);
#endif  // COBALT_BOX_DUMP_ENABLED

  skottie_box->SetUiNavItem(skottie_element->GetUiNavItem());
  boxes_.push_back(skottie_box);
}

namespace {

class ReplacedBoxGenerator : public cssom::NotReachedPropertyValueVisitor {
 public:
  ReplacedBoxGenerator(const scoped_refptr<cssom::CSSComputedStyleDeclaration>&
                           css_computed_style_declaration,
                       const ReplacedBox::ReplaceImageCB& replace_image_cb,
                       const ReplacedBox::SetBoundsCB& set_bounds_cb,
                       const scoped_refptr<Paragraph>& paragraph,
                       int32 text_position,
                       const base::Optional<LayoutUnit>& maybe_intrinsic_width,
                       const base::Optional<LayoutUnit>& maybe_intrinsic_height,
                       const base::Optional<float>& maybe_intrinsic_ratio,
                       const BoxGenerator::Context* context,
                       base::Optional<bool> is_video_punched_out,
                       math::SizeF content_size)
      : css_computed_style_declaration_(css_computed_style_declaration),
        replace_image_cb_(replace_image_cb),
        set_bounds_cb_(set_bounds_cb),
        paragraph_(paragraph),
        text_position_(text_position),
        maybe_intrinsic_width_(maybe_intrinsic_width),
        maybe_intrinsic_height_(maybe_intrinsic_height),
        maybe_intrinsic_ratio_(maybe_intrinsic_ratio),
        context_(context),
        is_video_punched_out_(is_video_punched_out),
        content_size_(content_size) {}

  void VisitKeyword(cssom::KeywordValue* keyword) override;

  const scoped_refptr<ReplacedBox>& replaced_box() { return replaced_box_; }

 private:
  const scoped_refptr<cssom::CSSComputedStyleDeclaration>
      css_computed_style_declaration_;
  const ReplacedBox::ReplaceImageCB replace_image_cb_;
  const ReplacedBox::SetBoundsCB set_bounds_cb_;
  const scoped_refptr<Paragraph> paragraph_;
  const int32 text_position_;
  const base::Optional<LayoutUnit> maybe_intrinsic_width_;
  const base::Optional<LayoutUnit> maybe_intrinsic_height_;
  const base::Optional<float> maybe_intrinsic_ratio_;
  const BoxGenerator::Context* context_;
  base::Optional<bool> is_video_punched_out_;
  math::SizeF content_size_;

  scoped_refptr<ReplacedBox> replaced_box_;
};

void ReplacedBoxGenerator::VisitKeyword(cssom::KeywordValue* keyword) {
  // See https://www.w3.org/TR/CSS21/visuren.html#display-prop.
  switch (keyword->value()) {
    // Generate a block-level replaced box.
    case cssom::KeywordValue::kBlock:
    case cssom::KeywordValue::kFlex:
      replaced_box_ = WrapRefCounted(new BlockLevelReplacedBox(
          css_computed_style_declaration_, replace_image_cb_, set_bounds_cb_,
          paragraph_, text_position_, maybe_intrinsic_width_,
          maybe_intrinsic_height_, maybe_intrinsic_ratio_,
          context_->used_style_provider, is_video_punched_out_, content_size_,
          context_->layout_stat_tracker));
      break;
    // Generate an inline-level replaced box. There is no need to distinguish
    // between inline replaced elements and inline-block replaced elements
    // because their widths, heights, and margins are calculated in the same
    // way.
    case cssom::KeywordValue::kInline:
    case cssom::KeywordValue::kInlineBlock:
    case cssom::KeywordValue::kInlineFlex:
      replaced_box_ = WrapRefCounted(new InlineLevelReplacedBox(
          css_computed_style_declaration_, replace_image_cb_, set_bounds_cb_,
          paragraph_, text_position_, maybe_intrinsic_width_,
          maybe_intrinsic_height_, maybe_intrinsic_ratio_,
          context_->used_style_provider, is_video_punched_out_, content_size_,
          context_->layout_stat_tracker));
      break;
    // The element generates no boxes and has no effect on layout.
    case cssom::KeywordValue::kNone:
      // Leave |replaced_box_| NULL.
      break;
    case cssom::KeywordValue::kAbsolute:
    case cssom::KeywordValue::kAlternate:
    case cssom::KeywordValue::kAlternateReverse:
    case cssom::KeywordValue::kAuto:
    case cssom::KeywordValue::kBackwards:
    case cssom::KeywordValue::kBaseline:
    case cssom::KeywordValue::kBoth:
    case cssom::KeywordValue::kBottom:
    case cssom::KeywordValue::kBreakWord:
    case cssom::KeywordValue::kCenter:
    case cssom::KeywordValue::kClip:
    case cssom::KeywordValue::kCollapse:
    case cssom::KeywordValue::kColumn:
    case cssom::KeywordValue::kColumnReverse:
    case cssom::KeywordValue::kContain:
    case cssom::KeywordValue::kContent:
    case cssom::KeywordValue::kCover:
    case cssom::KeywordValue::kCurrentColor:
    case cssom::KeywordValue::kCursive:
    case cssom::KeywordValue::kEllipsis:
    case cssom::KeywordValue::kEnd:
    case cssom::KeywordValue::kEquirectangular:
    case cssom::KeywordValue::kFantasy:
    case cssom::KeywordValue::kFixed:
    case cssom::KeywordValue::kFlexEnd:
    case cssom::KeywordValue::kFlexStart:
    case cssom::KeywordValue::kForwards:
    case cssom::KeywordValue::kHidden:
    case cssom::KeywordValue::kInfinite:
    case cssom::KeywordValue::kInherit:
    case cssom::KeywordValue::kInitial:
    case cssom::KeywordValue::kLeft:
    case cssom::KeywordValue::kLineThrough:
    case cssom::KeywordValue::kMiddle:
    case cssom::KeywordValue::kMonoscopic:
    case cssom::KeywordValue::kMonospace:
    case cssom::KeywordValue::kNoRepeat:
    case cssom::KeywordValue::kNormal:
    case cssom::KeywordValue::kNowrap:
    case cssom::KeywordValue::kPre:
    case cssom::KeywordValue::kPreLine:
    case cssom::KeywordValue::kPreWrap:
    case cssom::KeywordValue::kRelative:
    case cssom::KeywordValue::kRepeat:
    case cssom::KeywordValue::kReverse:
    case cssom::KeywordValue::kRight:
    case cssom::KeywordValue::kRow:
    case cssom::KeywordValue::kRowReverse:
    case cssom::KeywordValue::kSansSerif:
    case cssom::KeywordValue::kScroll:
    case cssom::KeywordValue::kSerif:
    case cssom::KeywordValue::kSolid:
    case cssom::KeywordValue::kSpaceAround:
    case cssom::KeywordValue::kSpaceBetween:
    case cssom::KeywordValue::kStart:
    case cssom::KeywordValue::kStatic:
    case cssom::KeywordValue::kStereoscopicLeftRight:
    case cssom::KeywordValue::kStereoscopicTopBottom:
    case cssom::KeywordValue::kStretch:
    case cssom::KeywordValue::kTop:
    case cssom::KeywordValue::kUppercase:
    case cssom::KeywordValue::kVisible:
    case cssom::KeywordValue::kWrap:
    case cssom::KeywordValue::kWrapReverse:
      NOTREACHED();
      break;
  }
}

}  // namespace

void BoxGenerator::VisitVideoElement(dom::HTMLVideoElement* video_element) {
  // For video elements, create a replaced box.

  // A replaced box is formatted as an atomic inline element. It is treated
  // directionally as a neutral character and its line breaking behavior is
  // equivalent to that of the Object Replacement Character.
  //   https://www.w3.org/TR/CSS21/visuren.html#inline-boxes
  //   https://www.w3.org/TR/CSS21/visuren.html#propdef-unicode-bidi
  //   https://www.w3.org/TR/css3-text/#line-break-details
  int32 text_position =
      (*paragraph_)
          ->AppendCodePoint(Paragraph::kObjectReplacementCharacterCodePoint);

  render_tree::ResourceProvider* resource_provider =
      *video_element->node_document()
           ->html_element_context()
           ->resource_provider();

  // If the optional is disengaged, then we don't know if punch out is enabled
  // or not.
  base::Optional<bool> is_punch_out;
  if (video_element->GetVideoFrameProvider()) {
    VideoFrameProvider::OutputMode output_mode =
        video_element->GetVideoFrameProvider()->GetOutputMode();
    if (output_mode != VideoFrameProvider::kOutputModeInvalid) {
      is_punch_out = output_mode == VideoFrameProvider::kOutputModePunchOut;
    }
  }

  ReplacedBoxGenerator replaced_box_generator(
      video_element->css_computed_style_declaration(),
      video_element->GetVideoFrameProvider()
          ? base::Bind(GetVideoFrame, video_element->GetVideoFrameProvider(),
                       resource_provider)
          : ReplacedBox::ReplaceImageCB(),
      video_element->GetSetBoundsCB(), *paragraph_, text_position,
      base::nullopt, base::nullopt, base::nullopt, context_, is_punch_out,
      video_element->GetVideoSize());
  video_element->computed_style()->display()->Accept(&replaced_box_generator);

  scoped_refptr<ReplacedBox> replaced_box =
      replaced_box_generator.replaced_box();
  if (replaced_box.get() == NULL) {
    // The element with "display: none" generates no boxes and has no effect
    // on layout. Descendant elements do not generate any boxes either.
    // This behavior cannot be overridden by setting the "display" property on
    // the descendants.
    //   https://www.w3.org/TR/CSS21/visuren.html#display-prop
    return;
  }

#ifdef COBALT_BOX_DUMP_ENABLED
  replaced_box->SetGeneratingNode(video_element);
#endif  // COBALT_BOX_DUMP_ENABLED

  replaced_box->SetUiNavItem(video_element->GetUiNavItem());
  boxes_.push_back(replaced_box);

  // The content of replaced elements is not considered in the CSS rendering
  // model.
  //   https://www.w3.org/TR/CSS21/conform.html#replaced-element
}

void BoxGenerator::VisitBrElement(dom::HTMLBRElement* br_element) {
  // If the br element has "display: none", then it has no effect on the layout.
  if (br_element->computed_style()->display() ==
      cssom::KeywordValue::GetNone()) {
    return;
  }

  scoped_refptr<cssom::CSSComputedStyleDeclaration>
      css_computed_style_declaration = new cssom::CSSComputedStyleDeclaration();
  css_computed_style_declaration->SetData(GetComputedStyleOfAnonymousBox(
      br_element->css_computed_style_declaration()));

  css_computed_style_declaration->set_animations(br_element->animations());

  DCHECK(*paragraph_);
  int32 text_position = (*paragraph_)->GetTextEndPosition();

  const bool kTriggersLineBreakTrue = true;
  const bool kIsProductOfSplitFalse = false;

  scoped_refptr<TextBox> br_text_box =
      new TextBox(css_computed_style_declaration, *paragraph_, text_position,
                  text_position, kTriggersLineBreakTrue, kIsProductOfSplitFalse,
                  context_->used_style_provider, context_->layout_stat_tracker);

  // Add a line feed code point to the paragraph to signify the new line for
  // the line breaking and bidirectional algorithms.
  (*paragraph_)->AppendCodePoint(Paragraph::kLineFeedCodePoint);

#ifdef COBALT_BOX_DUMP_ENABLED
  br_text_box->SetGeneratingNode(br_element);
#endif  // COBALT_BOX_DUMP_ENABLED

  br_text_box->SetUiNavItem(br_element->GetUiNavItem());
  boxes_.push_back(br_text_box);
}

namespace {

class ContainerBoxGenerator : public cssom::NotReachedPropertyValueVisitor {
 public:
  enum CloseParagraph {
    kDoNotCloseParagraph,
    kCloseParagraph,
  };

  ContainerBoxGenerator(dom::Directionality directionality,
                        const scoped_refptr<cssom::CSSComputedStyleDeclaration>&
                            css_computed_style_declaration,
                        scoped_refptr<Paragraph>* paragraph,
                        const BoxGenerator::Context* context)
      : directionality_(directionality),
        css_computed_style_declaration_(css_computed_style_declaration),
        context_(context),
        has_scoped_directional_embedding_(false),
        paragraph_(paragraph),
        paragraph_scoped_(false) {}
  ~ContainerBoxGenerator();

  void VisitKeyword(cssom::KeywordValue* keyword) override;

  const scoped_refptr<ContainerBox>& container_box() { return container_box_; }

 private:
  void CreateScopedParagraph(CloseParagraph close_prior_paragraph);

  const dom::Directionality directionality_;
  const scoped_refptr<cssom::CSSComputedStyleDeclaration>
      css_computed_style_declaration_;
  const BoxGenerator::Context* context_;

  // If a directional embedding was added to the paragraph by this container box
  // and needs to be popped in the destructor:
  // http://unicode.org/reports/tr9/#Explicit_Directional_Embeddings
  bool has_scoped_directional_embedding_;

  scoped_refptr<Paragraph>* paragraph_;
  scoped_refptr<Paragraph> prior_paragraph_;
  bool paragraph_scoped_;

  scoped_refptr<ContainerBox> container_box_;
};

ContainerBoxGenerator::~ContainerBoxGenerator() {
  // If there's a scoped directional embedding, then it needs to popped from
  // the paragraph so that this box does not impact the directionality of later
  // boxes in the paragraph.
  // http://unicode.org/reports/tr9/#Terminating_Explicit_Directional_Embeddings_and_Overrides
  if (has_scoped_directional_embedding_) {
    (*paragraph_)
        ->AppendCodePoint(
            Paragraph::kPopDirectionalFormattingCharacterCodePoint);
  }

  if (paragraph_scoped_) {
    (*paragraph_)->Close();

    // If the prior paragraph was closed, then replace it with a new paragraph
    // that has the same direction as the previous one. Otherwise, restore the
    // prior one.
    if (prior_paragraph_->IsClosed()) {
      *paragraph_ = new Paragraph(
          prior_paragraph_->GetLocale(), prior_paragraph_->GetBaseDirection(),
          prior_paragraph_->GetDirectionalEmbeddingStack(),
          context_->line_break_iterator, context_->character_break_iterator);
    } else {
      *paragraph_ = prior_paragraph_;
    }
  }
}

void ContainerBoxGenerator::VisitKeyword(cssom::KeywordValue* keyword) {
  // See https://www.w3.org/TR/CSS21/visuren.html#display-prop.
  switch (keyword->value()) {
    // Generate a block-level block container box.
    case cssom::KeywordValue::kBlock:
    case cssom::KeywordValue::kFlex:
      // The block ends the current paragraph and begins a new one that ends
      // with the block, so close the current paragraph, and create a new
      // paragraph that will close when the container box generator is
      // destroyed.
      CreateScopedParagraph(kCloseParagraph);

      container_box_ = base::WrapRefCounted(new BlockLevelBlockContainerBox(
          css_computed_style_declaration_, (*paragraph_)->GetBaseDirection(),
          context_->used_style_provider, context_->layout_stat_tracker));
      break;
    // Generate one or more inline boxes. Note that more inline boxes may be
    // generated when the original inline box is split due to participation
    // in the formatting context.
    case cssom::KeywordValue::kInline:
    case cssom::KeywordValue::kInlineFlex:
      // If the creating HTMLElement had an explicit directionality, then append
      // a directional embedding to the paragraph. This will be popped from the
      // paragraph, when the ContainerBoxGenerator goes out of scope.
      // https://dev.w3.org/html5/spec-preview/global-attributes.html#the-directionality
      // http://unicode.org/reports/tr9/#Explicit_Directional_Embeddings
      if (directionality_ == dom::kLeftToRightDirectionality) {
        has_scoped_directional_embedding_ = true;
        (*paragraph_)->AppendCodePoint(Paragraph::kLeftToRightEmbedCodePoint);
      } else if (directionality_ == dom::kRightToLeftDirectionality) {
        has_scoped_directional_embedding_ = true;
        (*paragraph_)->AppendCodePoint(Paragraph::kRightToLeftEmbedCodePoint);
      }

      // If the paragraph has not started yet, then add a no-break space to it,
      // thereby starting the paragraph without providing a wrappable location,
      // as the line should never wrap at the start of text.
      // http://unicode.org/reports/tr14/#BreakingRules
      //
      // Starting the paragraph ensures that subsequent text nodes create text
      // boxes, even when they consist of only collapsible white-space. This is
      // necessary because empty inline container boxes can justify a line's
      // existence if they have a non-zero margin, border or padding, which
      // means that the collapsible white-space is potentially wrappable
      // regardless of whether any intervening text is added to the paragraph.
      // Not creating the collapsible text box in this case would incorrectly
      // eliminate a wrappable location from the line.
      if ((*paragraph_)->GetTextEndPosition() == 0) {
        (*paragraph_)->AppendCodePoint(Paragraph::kNoBreakSpaceCodePoint);
      }

      container_box_ = base::WrapRefCounted(new InlineContainerBox(
          css_computed_style_declaration_, context_->used_style_provider,
          context_->layout_stat_tracker));
      break;
    // Generate an inline-level block container box. The inside of
    // an inline-block is formatted as a block box, and the element itself
    // is formatted as an atomic inline-level box.
    //   https://www.w3.org/TR/CSS21/visuren.html#inline-boxes
    case cssom::KeywordValue::kInlineBlock: {
      // An inline block is is treated directionally as a neutral character and
      // its line breaking behavior is equivalent to that of the Object
      // Replacement Character.
      //   https://www.w3.org/TR/CSS21/visuren.html#propdef-unicode-bidi
      //   https://www.w3.org/TR/css3-text/#line-break-details
      int32 text_position =
          (*paragraph_)
              ->AppendCodePoint(
                  Paragraph::kObjectReplacementCharacterCodePoint);
      scoped_refptr<Paragraph> prior_paragraph = *paragraph_;

      // The inline block creates a new paragraph, which the old paragraph
      // flows around. Create a new paragraph, which will close with the end
      // of the inline block. However, do not close the old paragraph, because
      // it will continue once the scope of the inline block ends.
      CreateScopedParagraph(kDoNotCloseParagraph);

      container_box_ = base::WrapRefCounted(new InlineLevelBlockContainerBox(
          css_computed_style_declaration_, (*paragraph_)->GetBaseDirection(),
          prior_paragraph, text_position, context_->used_style_provider,
          context_->layout_stat_tracker));
    } break;
    // The element generates no boxes and has no effect on layout.
    case cssom::KeywordValue::kNone:
      // Leave |container_box_| NULL.
      break;
    case cssom::KeywordValue::kAbsolute:
    case cssom::KeywordValue::kAlternate:
    case cssom::KeywordValue::kAlternateReverse:
    case cssom::KeywordValue::kAuto:
    case cssom::KeywordValue::kBackwards:
    case cssom::KeywordValue::kBaseline:
    case cssom::KeywordValue::kBoth:
    case cssom::KeywordValue::kBottom:
    case cssom::KeywordValue::kBreakWord:
    case cssom::KeywordValue::kCenter:
    case cssom::KeywordValue::kClip:
    case cssom::KeywordValue::kCollapse:
    case cssom::KeywordValue::kColumn:
    case cssom::KeywordValue::kColumnReverse:
    case cssom::KeywordValue::kContain:
    case cssom::KeywordValue::kContent:
    case cssom::KeywordValue::kCover:
    case cssom::KeywordValue::kCurrentColor:
    case cssom::KeywordValue::kCursive:
    case cssom::KeywordValue::kEllipsis:
    case cssom::KeywordValue::kEnd:
    case cssom::KeywordValue::kEquirectangular:
    case cssom::KeywordValue::kFantasy:
    case cssom::KeywordValue::kFixed:
    case cssom::KeywordValue::kFlexEnd:
    case cssom::KeywordValue::kFlexStart:
    case cssom::KeywordValue::kForwards:
    case cssom::KeywordValue::kHidden:
    case cssom::KeywordValue::kInfinite:
    case cssom::KeywordValue::kInherit:
    case cssom::KeywordValue::kInitial:
    case cssom::KeywordValue::kLeft:
    case cssom::KeywordValue::kLineThrough:
    case cssom::KeywordValue::kMiddle:
    case cssom::KeywordValue::kMonoscopic:
    case cssom::KeywordValue::kMonospace:
    case cssom::KeywordValue::kNoRepeat:
    case cssom::KeywordValue::kNormal:
    case cssom::KeywordValue::kNowrap:
    case cssom::KeywordValue::kPre:
    case cssom::KeywordValue::kPreLine:
    case cssom::KeywordValue::kPreWrap:
    case cssom::KeywordValue::kRelative:
    case cssom::KeywordValue::kRepeat:
    case cssom::KeywordValue::kReverse:
    case cssom::KeywordValue::kRight:
    case cssom::KeywordValue::kRow:
    case cssom::KeywordValue::kRowReverse:
    case cssom::KeywordValue::kSansSerif:
    case cssom::KeywordValue::kScroll:
    case cssom::KeywordValue::kSerif:
    case cssom::KeywordValue::kSolid:
    case cssom::KeywordValue::kSpaceAround:
    case cssom::KeywordValue::kSpaceBetween:
    case cssom::KeywordValue::kStart:
    case cssom::KeywordValue::kStatic:
    case cssom::KeywordValue::kStereoscopicLeftRight:
    case cssom::KeywordValue::kStereoscopicTopBottom:
    case cssom::KeywordValue::kStretch:
    case cssom::KeywordValue::kTop:
    case cssom::KeywordValue::kUppercase:
    case cssom::KeywordValue::kVisible:
    case cssom::KeywordValue::kWrap:
    case cssom::KeywordValue::kWrapReverse:
      NOTREACHED();
      break;
  }
}

void ContainerBoxGenerator::CreateScopedParagraph(
    CloseParagraph close_prior_paragraph) {
  DCHECK(!paragraph_scoped_);

  paragraph_scoped_ = true;
  prior_paragraph_ = *paragraph_;

  // Determine the base direction of the new paragraph based upon the
  // directionality of the creating HTMLElement. If there was no explicit
  // directionality, then it is based upon the prior paragraph, meaning that
  // it is inherited from the parent element.
  // https://dev.w3.org/html5/spec-preview/global-attributes.html#the-directionality
  BaseDirection base_direction;
  if (directionality_ == dom::kLeftToRightDirectionality) {
    base_direction = kLeftToRightBaseDirection;
  } else if (directionality_ == dom::kRightToLeftDirectionality) {
    base_direction = kRightToLeftBaseDirection;
  } else {
    base_direction = prior_paragraph_->GetDirectionalEmbeddingStackDirection();
  }

  if (close_prior_paragraph == kCloseParagraph) {
    prior_paragraph_->Close();
  }

  *paragraph_ = new Paragraph(prior_paragraph_->GetLocale(), base_direction,
                              Paragraph::DirectionalEmbeddingStack(),
                              context_->line_break_iterator,
                              context_->character_break_iterator);
}

}  // namespace

void BoxGenerator::AppendChildBoxToLine(const scoped_refptr<Box>& child_box) {
  // When an inline box contains an in-flow block-level box, the inline box
  // (and its inline ancestors within the same block container box*) are
  // broken around the block-level box, splitting the inline box into two
  // boxes (even if either side is empty), one on each side of
  // the block-level box. The line boxes before the break and after
  // the break are enclosed in anonymous block boxes, and the block-level
  // box becomes a sibling of those anonymous boxes.
  //   https://www.w3.org/TR/CSS21/visuren.html#anonymous-block-level
  //
  // * CSS 2.1 says "the same line box" but line boxes are not real boxes
  //   in Cobalt, see |LineBox| for details.
  ContainerBox* last_container_box =
      base::polymorphic_downcast<ContainerBox*>(boxes_.back().get());

  if (!last_container_box->TryAddChild(child_box)) {
    scoped_refptr<ContainerBox> next_container_box =
        last_container_box->TrySplitAtEnd();
    DCHECK(next_container_box);

    // Attempt to add the box to the next container before adding it to the top
    // level. In the case where a line break was blocking the add in the last
    // container, the child should successfully go into the next container.
    if (!next_container_box->TryAddChild(child_box)) {
      boxes_.push_back(child_box);
    }

    boxes_.push_back(next_container_box);
  }
}

namespace {

class ContentProvider : public cssom::NotReachedPropertyValueVisitor {
 public:
  ContentProvider() : is_element_generated_(false) {}

  const std::string& content_string() const { return content_string_; }
  bool is_element_generated() const { return is_element_generated_; }

  void VisitString(cssom::StringValue* string_value) override {
    content_string_ = string_value->value();
    is_element_generated_ = true;
  }

  void VisitURL(cssom::URLValue* url_value) override {
    // TODO: Implement support for 'content: url(foo)'.
    DLOG(ERROR) << "Unsupported content property value: "
                << url_value->ToString();
  }

  void VisitKeyword(cssom::KeywordValue* keyword) override {
    switch (keyword->value()) {
      case cssom::KeywordValue::kNone:
      case cssom::KeywordValue::kNormal:
        // The pseudo-element is not generated.
        //   https://www.w3.org/TR/CSS21/generate.html#propdef-content
        is_element_generated_ = false;
        break;
      case cssom::KeywordValue::kAbsolute:
      case cssom::KeywordValue::kAlternate:
      case cssom::KeywordValue::kAlternateReverse:
      case cssom::KeywordValue::kAuto:
      case cssom::KeywordValue::kBackwards:
      case cssom::KeywordValue::kBaseline:
      case cssom::KeywordValue::kBlock:
      case cssom::KeywordValue::kBoth:
      case cssom::KeywordValue::kBottom:
      case cssom::KeywordValue::kBreakWord:
      case cssom::KeywordValue::kCenter:
      case cssom::KeywordValue::kClip:
      case cssom::KeywordValue::kCollapse:
      case cssom::KeywordValue::kColumn:
      case cssom::KeywordValue::kColumnReverse:
      case cssom::KeywordValue::kContain:
      case cssom::KeywordValue::kContent:
      case cssom::KeywordValue::kCover:
      case cssom::KeywordValue::kCurrentColor:
      case cssom::KeywordValue::kCursive:
      case cssom::KeywordValue::kEllipsis:
      case cssom::KeywordValue::kEnd:
      case cssom::KeywordValue::kEquirectangular:
      case cssom::KeywordValue::kFantasy:
      case cssom::KeywordValue::kFixed:
      case cssom::KeywordValue::kFlex:
      case cssom::KeywordValue::kFlexEnd:
      case cssom::KeywordValue::kFlexStart:
      case cssom::KeywordValue::kForwards:
      case cssom::KeywordValue::kHidden:
      case cssom::KeywordValue::kInfinite:
      case cssom::KeywordValue::kInherit:
      case cssom::KeywordValue::kInitial:
      case cssom::KeywordValue::kInline:
      case cssom::KeywordValue::kInlineBlock:
      case cssom::KeywordValue::kInlineFlex:
      case cssom::KeywordValue::kLeft:
      case cssom::KeywordValue::kLineThrough:
      case cssom::KeywordValue::kMiddle:
      case cssom::KeywordValue::kMonoscopic:
      case cssom::KeywordValue::kMonospace:
      case cssom::KeywordValue::kNoRepeat:
      case cssom::KeywordValue::kNowrap:
      case cssom::KeywordValue::kPre:
      case cssom::KeywordValue::kPreLine:
      case cssom::KeywordValue::kPreWrap:
      case cssom::KeywordValue::kRelative:
      case cssom::KeywordValue::kRepeat:
      case cssom::KeywordValue::kReverse:
      case cssom::KeywordValue::kRight:
      case cssom::KeywordValue::kRow:
      case cssom::KeywordValue::kRowReverse:
      case cssom::KeywordValue::kSansSerif:
      case cssom::KeywordValue::kScroll:
      case cssom::KeywordValue::kSerif:
      case cssom::KeywordValue::kSolid:
      case cssom::KeywordValue::kSpaceAround:
      case cssom::KeywordValue::kSpaceBetween:
      case cssom::KeywordValue::kStart:
      case cssom::KeywordValue::kStatic:
      case cssom::KeywordValue::kStereoscopicLeftRight:
      case cssom::KeywordValue::kStereoscopicTopBottom:
      case cssom::KeywordValue::kStretch:
      case cssom::KeywordValue::kTop:
      case cssom::KeywordValue::kUppercase:
      case cssom::KeywordValue::kVisible:
      case cssom::KeywordValue::kWrap:
      case cssom::KeywordValue::kWrapReverse:
        NOTREACHED();
    }
  }

 private:
  std::string content_string_;
  bool is_element_generated_;
};

bool HasOnlyColorPropertyAnimations(
    const scoped_refptr<const web_animations::AnimationSet>& animations) {
  const web_animations::AnimationSet::InternalSet& animation_set =
      animations->animations();

  if (animation_set.empty()) {
    return false;
  }

  for (web_animations::AnimationSet::InternalSet::const_iterator iter =
           animation_set.begin();
       iter != animation_set.end(); ++iter) {
    const web_animations::KeyframeEffectReadOnly* keyframe_effect =
        base::polymorphic_downcast<
            const web_animations::KeyframeEffectReadOnly*>(
            (*iter)->effect().get());
    if (!keyframe_effect->data().IsOnlyPropertyAnimated(
            cssom::kColorProperty)) {
      return false;
    }
  }

  return true;
}

}  // namespace

void BoxGenerator::AppendPseudoElementToLine(
    dom::HTMLElement* html_element,
    dom::PseudoElementType pseudo_element_type) {
  // Add boxes with generated content from :before or :after pseudo elements to
  // the line.
  //   https://www.w3.org/TR/CSS21/generate.html#before-after-content
  dom::PseudoElement* pseudo_element =
      html_element->pseudo_element(pseudo_element_type);
  if (!pseudo_element) {
    return;
  }

  // We assume that if our parent element's boxes are being regenerated, then we
  // should regenerate the pseudo element boxes.  There are some cases where
  // the parent element may be regenerating its boxes even if it already had
  // some, such as if its boxes were inline level.  In that case, pseudo
  // elements may also have boxes, so we make it clear that we will not be
  // reusing pseudo element boxes even if they exist by explicitly resetting
  // them now.
  pseudo_element->reset_layout_boxes();

  ContainerBoxGenerator pseudo_element_box_generator(
      dom::kNoExplicitDirectionality,
      pseudo_element->css_computed_style_declaration(), paragraph_, context_);
  pseudo_element->computed_style()->display()->Accept(
      &pseudo_element_box_generator);
  scoped_refptr<ContainerBox> pseudo_element_box =
      pseudo_element_box_generator.container_box();
  // A pseudo element with "display: none" generates no boxes and has no
  // effect on layout.
  if (pseudo_element_box.get() == NULL) {
    return;
  }

  // Generate the box(es) to be added to the associated html element, using
  // the computed style of the pseudo element.

  // The generated content is a text node with the string value of the
  // 'content' property.
  ContentProvider content_provider;
  pseudo_element->computed_style()->content()->Accept(&content_provider);
  if (!content_provider.is_element_generated()) {
    return;
  }

  scoped_refptr<dom::Text> child_node(new dom::Text(
      html_element->node_document(), content_provider.content_string()));

  // In the case where the pseudo element has no color property of its
  // own, but is directly inheriting a color property from its parent html
  // element, we use the parent's animations if the pseudo element has
  // none and the parent has only color property animations. This allows
  // the child text boxes to animate properly and fixes bugs, while
  // keeping the impact of the fix as small as possible to minimize the
  // risk of introducing new bugs.
  // TODO: Remove this logic when support for inheriting
  // animations on inherited properties is added.
  bool use_html_element_animations =
      !pseudo_element->computed_style()->IsDeclared(cssom::kColorProperty) &&
      html_element->computed_style()->IsDeclared(cssom::kColorProperty) &&
      pseudo_element->css_computed_style_declaration()
          ->animations()
          ->IsEmpty() &&
      HasOnlyColorPropertyAnimations(
          html_element->css_computed_style_declaration()->animations());

  BoxGenerator child_box_generator(
      pseudo_element->css_computed_style_declaration(),
      use_html_element_animations ? html_element->animations()
                                  : pseudo_element->animations(),
      paragraph_, dom_element_depth_ + 1, context_);
  child_node->Accept(&child_box_generator);
  for (const auto& child_box : child_box_generator.boxes()) {
    if (!pseudo_element_box->TryAddChild(child_box)) {
      return;
    }
  }

  pseudo_element->set_layout_boxes(
      std::unique_ptr<dom::LayoutBoxes>(new LayoutBoxes({pseudo_element_box})));

  // Add the box(es) from the pseudo element to the associated element.
  AppendChildBoxToLine(pseudo_element_box);
}

namespace {
scoped_refptr<cssom::CSSComputedStyleDeclaration> StripBackground(
    const scoped_refptr<cssom::CSSComputedStyleDeclaration>& style) {
  scoped_refptr<cssom::CSSComputedStyleDeclaration> new_style(
      new cssom::CSSComputedStyleDeclaration());
  new_style->set_animations(style->animations());

  scoped_refptr<cssom::MutableCSSComputedStyleData> new_data(
      new cssom::MutableCSSComputedStyleData());
  new_data->AssignFrom(*style->data());
  new_data->SetPropertyValue(cssom::kBackgroundColorProperty, NULL);
  new_data->SetPropertyValue(cssom::kBackgroundImageProperty, NULL);
  new_style->SetData(new_data);

  return new_style;
}
}  // namespace

void BoxGenerator::VisitNonReplacedElement(dom::HTMLElement* html_element) {
  const scoped_refptr<cssom::CSSComputedStyleDeclaration>& element_style(
      html_element->css_computed_style_declaration());

  ContainerBoxGenerator container_box_generator(
      html_element->directionality(),
      html_element == context_->ignore_background_element
          ? StripBackground(element_style)
          : element_style,
      paragraph_, context_);
  html_element->computed_style()->display()->Accept(&container_box_generator);
  scoped_refptr<ContainerBox> container_box_before_split =
      container_box_generator.container_box();
  if (container_box_before_split.get() == NULL) {
    // The element with "display: none" generates no boxes and has no effect
    // on layout. Descendant elements do not generate any boxes either.
    // This behavior cannot be overridden by setting the "display" property on
    // the descendants.
    //   https://www.w3.org/TR/CSS21/visuren.html#display-prop
    return;
  }

#ifdef COBALT_BOX_DUMP_ENABLED
  container_box_before_split->SetGeneratingNode(html_element);
#endif  // COBALT_BOX_DUMP_ENABLED

  container_box_before_split->SetUiNavItem(html_element->GetUiNavItem());
  boxes_.push_back(container_box_before_split);

  AppendPseudoElementToLine(html_element, dom::kBeforePseudoElementType);

  // Generate child boxes.
  for (dom::Node* child_node = html_element->first_child(); child_node;
       child_node = child_node->next_sibling()) {
    BoxGenerator child_box_generator(
        html_element->css_computed_style_declaration(),
        html_element->css_computed_style_declaration()->animations(),
        paragraph_, dom_element_depth_ + 1, context_);
    child_node->Accept(&child_box_generator);
    const Boxes& child_boxes = child_box_generator.boxes();
    for (Boxes::const_iterator child_box_iterator = child_boxes.begin();
         child_box_iterator != child_boxes.end(); ++child_box_iterator) {
      AppendChildBoxToLine(*child_box_iterator);
    }
  }

  AppendPseudoElementToLine(html_element, dom::kAfterPseudoElementType);
}

void BoxGenerator::Visit(dom::CDATASection* /* cdata_section */) {}

void BoxGenerator::Visit(dom::Comment* /*comment*/) {}

void BoxGenerator::Visit(dom::Document* /*document*/) { NOTREACHED(); }

void BoxGenerator::Visit(dom::DocumentType* /*document_type*/) { NOTREACHED(); }

namespace {
scoped_refptr<web_animations::AnimationSet> GetAnimationsForAnonymousBox(
    const scoped_refptr<const web_animations::AnimationSet>&
        parent_animations) {
  scoped_refptr<web_animations::AnimationSet> animations(
      new web_animations::AnimationSet);
  const web_animations::AnimationSet::InternalSet& animation_set =
      parent_animations->animations();
  const cssom::PropertyKeyVector& properties_set =
    cssom::GetInheritedAnimatableProperties();

  // Go through all the parent animations and only add those pertaining to
  // inheritable properties.
  for (const auto& animation : animation_set) {
    const web_animations::KeyframeEffectReadOnly* keyframe_effect =
        base::polymorphic_downcast<
            const web_animations::KeyframeEffectReadOnly*>(
            animation->effect().get());

    for (const auto& property : properties_set) {
      if (keyframe_effect->data().IsPropertyAnimated(property)) {
        animations->AddAnimation(animation);
        break;
      }
    }
  }

  return animations;
}
}  // namespace

// Append the text from the text node to the text paragraph and create the
// node's initial text box. The text box has indices that map to the paragraph,
// which allows it to retrieve its underlying text. Initially, a single text box
// is created that encompasses the entire node.

// Prior to layout, the paragraph applies the Unicode bidirectional algorithm
// to its text (http://www.unicode.org/reports/tr9/) and causes the text boxes
// referencing it to split at level runs.
//
// During layout, the text boxes are potentially split further, as the paragraph
// determines line breaking locations for its text at soft wrap opportunities
// (https://www.w3.org/TR/css-text-3/#soft-wrap-opportunity) according to the
// Unicode line breaking algorithm (http://www.unicode.org/reports/tr14/).
void BoxGenerator::Visit(dom::Text* text) {
  scoped_refptr<cssom::CSSComputedStyleDeclaration>
      css_computed_style_declaration = new cssom::CSSComputedStyleDeclaration();
  css_computed_style_declaration->SetData(
      GetComputedStyleOfAnonymousBox(parent_css_computed_style_declaration_));

  // Copy inheritable animatable properties from the parent.
  css_computed_style_declaration->set_animations(
      GetAnimationsForAnonymousBox(parent_animations_));

  DCHECK(text);
  DCHECK(css_computed_style_declaration->data());

  const std::string& original_text = text->text();
  if (original_text.empty()) {
    return;
  }

  const scoped_refptr<cssom::PropertyValue>& white_space_property =
      css_computed_style_declaration->data()->white_space();
  bool should_preserve_segment_breaks =
      !DoesCollapseSegmentBreaks(white_space_property);
  bool should_collapse_white_space =
      DoesCollapseWhiteSpace(white_space_property);
  bool should_prevent_text_wrapping =
      !DoesAllowTextWrapping(white_space_property);

  // Loop until the entire text is consumed. If the white-space property does
  // not have a value of "pre" or "pre-wrap" then the entire text will be
  // processed the first time through the loop; otherwise, the text will be
  // split at newline sequences.
  size_t start_index = 0;
  while (start_index < original_text.size()) {
    size_t end_index;
    size_t newline_sequence_length;

    // Phase I: Segment Break Transformation Rules
    // https://www.w3.org/TR/css3-text/#line-break-transform
    bool generates_newline = false;
    if (should_preserve_segment_breaks) {
      generates_newline = FindNextNewlineSequence(
          original_text, start_index, &end_index, &newline_sequence_length);
    } else {
      end_index = original_text.size();
      newline_sequence_length = 0;
    }

    std::string modifiable_text =
        original_text.substr(start_index, end_index - start_index);

    // Phase I: Collapsing and Transformation
    //   https://www.w3.org/TR/css3-text/#white-space-phase-1
    if (should_collapse_white_space) {
      CollapseWhiteSpace(&modifiable_text);

      // If the paragraph hasn't been started yet and the text only consists of
      // a collapsible space, then return without creating the box. The leading
      // spaces in a line box are collapsed, so this box would be collapsed
      // away during the layout.
      if ((*paragraph_)->GetTextEndPosition() == 0 && modifiable_text == " ") {
        return;
      }
    }

    Paragraph::TextTransform transform;
    if (css_computed_style_declaration->data()->text_transform() ==
        cssom::KeywordValue::GetUppercase()) {
      transform = Paragraph::kUppercaseTextTransform;
    } else {
      transform = Paragraph::kNoTextTransform;
    }

    DCHECK(*paragraph_);
    int32 text_start_position =
        (*paragraph_)->AppendUtf8String(modifiable_text, transform);
    int32 text_end_position = (*paragraph_)->GetTextEndPosition();

    const bool kIsProductOfSplitFalse = false;

    boxes_.push_back(new TextBox(
        css_computed_style_declaration, *paragraph_, text_start_position,
        text_end_position, generates_newline, kIsProductOfSplitFalse,
        context_->used_style_provider, context_->layout_stat_tracker));

    // Newline sequences should be transformed into a preserved line feed.
    //   https://www.w3.org/TR/css3-text/#line-break-transform
    if (generates_newline) {
      (*paragraph_)->AppendCodePoint(Paragraph::kLineFeedCodePoint);
    }

    start_index = end_index + newline_sequence_length;
  }

  // If the white-space style prevents text wrapping and the text ends in a
  // space, then add a no-break space to the paragraph, so that the last space
  // will be treated as a no-break space when determining if wrapping can occur
  // before the subsequent box.
  //
  // While CSS3 gives little direction to the user agent as to what should occur
  // in this case, this is guidance given by CSS2, which states that "any
  // sequence of spaces (U+0020) unbroken by an element boundary is treated as a
  // sequence of non-breaking spaces." Furthermore, this matches the behavior of
  // WebKit, Firefox, and IE.
  // https://www.w3.org/TR/css-text-3/#white-space-phase-1
  // https://www.w3.org/TR/CSS2/text.html#white-space-model
  if (should_prevent_text_wrapping &&
      original_text[original_text.size() - 1] == ' ') {
    (*paragraph_)->AppendCodePoint(Paragraph::kNoBreakSpaceCodePoint);
  }
}

}  // namespace layout
}  // namespace cobalt
