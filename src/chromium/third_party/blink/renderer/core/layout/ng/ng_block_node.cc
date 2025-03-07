// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/layout/ng/ng_block_node.h"

#include <memory>

#include "third_party/blink/renderer/core/frame/local_frame_view.h"
#include "third_party/blink/renderer/core/html/html_marquee_element.h"
#include "third_party/blink/renderer/core/layout/box_layout_extra_input.h"
#include "third_party/blink/renderer/core/layout/layout_block_flow.h"
#include "third_party/blink/renderer/core/layout/layout_fieldset.h"
#include "third_party/blink/renderer/core/layout/layout_inline.h"
#include "third_party/blink/renderer/core/layout/layout_multi_column_flow_thread.h"
#include "third_party/blink/renderer/core/layout/layout_multi_column_set.h"
#include "third_party/blink/renderer/core/layout/min_max_size.h"
#include "third_party/blink/renderer/core/layout/ng/geometry/ng_fragment_geometry.h"
#include "third_party/blink/renderer/core/layout/ng/inline/ng_inline_node.h"
#include "third_party/blink/renderer/core/layout/ng/legacy_layout_tree_walking.h"
#include "third_party/blink/renderer/core/layout/ng/list/layout_ng_list_item.h"
#include "third_party/blink/renderer/core/layout/ng/ng_block_break_token.h"
#include "third_party/blink/renderer/core/layout/ng/ng_block_layout_algorithm.h"
#include "third_party/blink/renderer/core/layout/ng/ng_box_fragment.h"
#include "third_party/blink/renderer/core/layout/ng/ng_box_fragment_builder.h"
#include "third_party/blink/renderer/core/layout/ng/ng_column_layout_algorithm.h"
#include "third_party/blink/renderer/core/layout/ng/ng_constraint_space.h"
#include "third_party/blink/renderer/core/layout/ng/ng_constraint_space_builder.h"
#include "third_party/blink/renderer/core/layout/ng/ng_fieldset_layout_algorithm.h"
#include "third_party/blink/renderer/core/layout/ng/ng_flex_layout_algorithm.h"
#include "third_party/blink/renderer/core/layout/ng/ng_fragmentation_utils.h"
#include "third_party/blink/renderer/core/layout/ng/ng_layout_input_node.h"
#include "third_party/blink/renderer/core/layout/ng/ng_layout_result.h"
#include "third_party/blink/renderer/core/layout/ng/ng_layout_utils.h"
#include "third_party/blink/renderer/core/layout/ng/ng_length_utils.h"
#include "third_party/blink/renderer/core/layout/ng/ng_page_layout_algorithm.h"
#include "third_party/blink/renderer/core/layout/ng/ng_space_utils.h"
#include "third_party/blink/renderer/core/layout/shapes/shape_outside_info.h"
#include "third_party/blink/renderer/core/paint/paint_layer.h"
#include "third_party/blink/renderer/core/paint/paint_layer_scrollable_area.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"
#include "third_party/blink/renderer/platform/text/writing_mode.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace blink {

namespace {

inline LayoutMultiColumnFlowThread* GetFlowThread(
    const LayoutBlockFlow* block_flow) {
  if (!block_flow)
    return nullptr;
  return block_flow->MultiColumnFlowThread();
}

inline LayoutMultiColumnFlowThread* GetFlowThread(const LayoutBox& box) {
  return GetFlowThread(DynamicTo<LayoutBlockFlow>(box));
}

// The entire purpose of this function is to avoid allocating space on the stack
// for all layout algorithms for each node we lay out. Therefore it must not be
// inline.
template <typename Algorithm, typename Callback>
NOINLINE void CreateAlgorithmAndRun(const NGLayoutAlgorithmParams& params,
                                    const Callback& callback) {
  Algorithm algorithm(params);
  callback(&algorithm);
}

inline void DetermineAlgorithmAndRun(
    const NGLayoutAlgorithmParams& params,
    const std::function<void(NGLayoutAlgorithmOperations*)>& callback) {
  const ComputedStyle& style = params.node.Style();
  const LayoutBox& box = *params.node.GetLayoutBox();
  if (box.IsLayoutNGFlexibleBox()) {
    CreateAlgorithmAndRun<NGFlexLayoutAlgorithm>(params, callback);
  } else if (box.IsLayoutNGFieldset()) {
    CreateAlgorithmAndRun<NGFieldsetLayoutAlgorithm>(params, callback);
    // If there's a legacy layout box, we can only do block fragmentation if
    // we would have done block fragmentation with the legacy engine.
    // Otherwise writing data back into the legacy tree will fail. Look for
    // the flow thread.
  } else if (GetFlowThread(box)) {
    if (style.SpecifiesColumns())
      CreateAlgorithmAndRun<NGColumnLayoutAlgorithm>(params, callback);
    else
      CreateAlgorithmAndRun<NGPageLayoutAlgorithm>(params, callback);
  } else {
    CreateAlgorithmAndRun<NGBlockLayoutAlgorithm>(params, callback);
  }
}

inline scoped_refptr<const NGLayoutResult> LayoutWithAlgorithm(
    const NGLayoutAlgorithmParams& params) {
  scoped_refptr<const NGLayoutResult> result;
  DetermineAlgorithmAndRun(params,
                           [&result](NGLayoutAlgorithmOperations* algorithm) {
                             result = algorithm->Layout();
                           });
  return result;
}

inline base::Optional<MinMaxSize> ComputeMinMaxSizeWithAlgorithm(
    const NGLayoutAlgorithmParams& params,
    const MinMaxSizeInput& input) {
  base::Optional<MinMaxSize> minmax;
  DetermineAlgorithmAndRun(
      params, [&minmax, &input](NGLayoutAlgorithmOperations* algorithm) {
        minmax = algorithm->ComputeMinMaxSize(input);
      });
  return minmax;
}

void UpdateLegacyMultiColumnFlowThread(
    NGBlockNode node,
    LayoutMultiColumnFlowThread* flow_thread,
    const NGConstraintSpace& constraint_space,
    const NGPhysicalBoxFragment& fragment) {
  WritingMode writing_mode = constraint_space.GetWritingMode();
  LayoutUnit flow_end;
  LayoutUnit column_block_size;
  bool has_processed_first_child = false;

  // Stitch the columns together.
  for (const auto& child : fragment.Children()) {
    NGFragment child_fragment(writing_mode, *child);
    flow_end += child_fragment.BlockSize();
    // Non-uniform fragmentainer widths not supported by legacy layout.
    DCHECK(!has_processed_first_child ||
           flow_thread->LogicalWidth() == child_fragment.InlineSize());
    if (!has_processed_first_child) {
      // The offset of the flow thread should be the same as that of the first
      // first column.
      flow_thread->SetLocation(child.Offset().ToLayoutPoint());
      flow_thread->SetLogicalWidth(child_fragment.InlineSize());
      column_block_size = child_fragment.BlockSize();
      has_processed_first_child = true;
    }
  }

  if (LayoutMultiColumnSet* column_set = flow_thread->FirstMultiColumnSet()) {
    NGFragment logical_fragment(writing_mode, fragment);
    auto border_scrollbar_padding =
        ComputeBorders(constraint_space, node) + node.GetScrollbarSizes() +
        ComputePadding(constraint_space, node.Style());

    column_set->SetLogicalLeft(border_scrollbar_padding.inline_start);
    column_set->SetLogicalTop(border_scrollbar_padding.block_start);
    column_set->SetLogicalWidth(logical_fragment.InlineSize() -
                                border_scrollbar_padding.InlineSum());
    column_set->SetLogicalHeight(column_block_size);
    column_set->EndFlow(flow_end);
  }
  // TODO(mstensho): Update all column boxes, not just the first column set
  // (like we do above). This is needed to support column-span:all.

  flow_thread->UpdateFromNG();
  flow_thread->ValidateColumnSets();
  flow_thread->SetLogicalHeight(flow_end);
  flow_thread->UpdateAfterLayout();
  flow_thread->ClearNeedsLayout();
}

NGConstraintSpaceBuilder CreateConstraintSpaceBuilderForMinMax(
    NGBlockNode node) {
  return NGConstraintSpaceBuilder(node.Style().GetWritingMode(),
                                  node.Style().GetWritingMode(),
                                  node.CreatesNewFormattingContext())
      .SetTextDirection(node.Style().Direction())
      .SetIsIntermediateLayout(true);
}

LayoutUnit CalculateAvailableInlineSizeForLegacy(
    const LayoutBox& box,
    const NGConstraintSpace& space) {
  if (box.ShouldComputeSizeAsReplaced())
    return space.ReplacedPercentageResolutionInlineSize();

  return space.PercentageResolutionInlineSize();
}

LayoutUnit CalculateAvailableBlockSizeForLegacy(
    const LayoutBox& box,
    const NGConstraintSpace& space) {
  if (box.ShouldComputeSizeAsReplaced())
    return space.ReplacedPercentageResolutionBlockSize();

  return space.PercentageResolutionBlockSize();
}

}  // namespace

scoped_refptr<const NGLayoutResult> NGBlockNode::Layout(
    const NGConstraintSpace& constraint_space,
    const NGBreakToken* break_token) {
  // Use the old layout code and synthesize a fragment.
  if (!CanUseNewLayout())
    return RunLegacyLayout(constraint_space);

  auto* block_flow = DynamicTo<LayoutBlockFlow>(box_);
  if (RuntimeEnabledFeatures::TrackLayoutPassesPerBlockEnabled() && block_flow)
    block_flow->IncrementLayoutPassCount();

  // The exclusion space internally is a pointer to a shared vector, and
  // equality of exclusion spaces is performed using pointer comparison on this
  // internal shared vector.
  // In order for the caching logic to work correctly we need to set the
  // pointer to the value previous shared vector.
  if (const NGLayoutResult* previous_result = box_->GetCachedLayoutResult()) {
    constraint_space.ExclusionSpace().PreInitialize(
        previous_result->GetConstraintSpaceForCaching().ExclusionSpace());
  }

  base::Optional<NGFragmentGeometry> fragment_geometry;
  scoped_refptr<const NGLayoutResult> layout_result = box_->CachedLayoutResult(
      constraint_space, break_token, &fragment_geometry);
  if (layout_result) {
    // We may have to update the margins on box_; we reuse the layout result
    // even if a percentage margin may have changed.
    if (UNLIKELY(Style().MayHaveMargin() && !IsTableCell()))
      box_->SetMargin(ComputePhysicalMargins(constraint_space, Style()));

    // TODO(layoutng): Figure out why these two call can't be inside the
    // !constraint_space.IsIntermediateLayout() block below.
    UpdateShapeOutsideInfoIfNeeded(
        *layout_result, constraint_space.PercentageResolutionInlineSize());

    return layout_result;
  }

  if (!fragment_geometry) {
    fragment_geometry =
        CalculateInitialFragmentGeometry(constraint_space, *this);
  }

  PrepareForLayout();

  NGLayoutAlgorithmParams params(*this, *fragment_geometry, constraint_space,
                                 To<NGBlockBreakToken>(break_token));
  layout_result = LayoutWithAlgorithm(params);
  FinishLayout(block_flow, constraint_space, break_token, layout_result);

  NGBoxStrut after_layout_scrollbars = GetScrollbarSizes();
  if (fragment_geometry->scrollbar != after_layout_scrollbars) {
    // If our scrollbars have changed, we need to relayout because either:
    // - Our size has changed (if shrinking to fit), or
    // - Space available to our children has changed.
    // This mirrors legacy code in PaintLayerScrollableArea::UpdateAfterLayout.
    // TODO(cbiesinger): It seems that we should also check if
    // PreferredLogicalWidthsDirty() has changed from false to true during
    // layout, so that we correctly size ourselves when shrinking to fit
    // and a child gained a vertical scrollbar. However, no test fails
    // without that check.
    PaintLayerScrollableArea::FreezeScrollbarsScope freeze_scrollbars;

    // Scrollbar changes are hard to detect. Make sure everyone gets the
    // message.
    box_->SetNeedsLayout(layout_invalidation_reason::kScrollbarChanged,
                         kMarkOnlyThis);

    fragment_geometry =
        CalculateInitialFragmentGeometry(constraint_space, *this);
    layout_result = LayoutWithAlgorithm(params);
    FinishLayout(block_flow, constraint_space, break_token, layout_result);
  }

  // We always need to update the ShapeOutsideInfo even if the layout is
  // intermediate (e.g. called during a min/max pass).
  //
  // If a shape-outside float is present in an orthogonal flow, when
  // calculating the min/max-size (by performing an intermediate layout), we
  // might calculate this incorrectly, as the layout won't take into account the
  // shape-outside area.
  //
  // TODO(ikilpatrick): This should be fixed by moving the shape-outside data
  // to the NGLayoutResult, removing this "side" data-structure.
  UpdateShapeOutsideInfoIfNeeded(
      *layout_result, constraint_space.PercentageResolutionInlineSize());

  return layout_result;
}

scoped_refptr<const NGLayoutResult>
NGBlockNode::CachedLayoutResultForOutOfFlowPositioned(
    LogicalSize container_content_size) const {
  DCHECK(IsOutOfFlowPositioned());

  if (box_->NeedsLayout())
    return nullptr;

  const NGLayoutResult* cached_layout_result = box_->GetCachedLayoutResult();
  if (!cached_layout_result)
    return nullptr;

  // TODO(layout-dev): There are potentially more cases where we can reuse this
  // layout result.
  // E.g. when we have a fixed-length top position constraint (top: 5px), we
  // are in the correct writing mode (htb-ltr), and we have a fixed width.
  const NGConstraintSpace& space =
      cached_layout_result->GetConstraintSpaceForCaching();
  if (space.PercentageResolutionSize() != container_content_size)
    return nullptr;

  // We currently don't keep the static-position around to determine if it is
  // the same as the previous layout pass. As such, only reuse the result when
  // we know it doesn't depend on the static-position.
  //
  // TODO(layout-dev): We might be able to determine what the previous
  // static-position was based on |NGLayoutResult::OutOfFlowPositionedOffset|.
  bool depends_on_static_position =
      (Style().Left().IsAuto() && Style().Right().IsAuto()) ||
      (Style().Top().IsAuto() && Style().Bottom().IsAuto());

  if (depends_on_static_position)
    return nullptr;

  return cached_layout_result;
}

void NGBlockNode::PrepareForLayout() {
  auto* block = DynamicTo<LayoutBlock>(box_);
  if (block && block->HasOverflowClip()) {
    DCHECK(block->GetScrollableArea());
    if (block->GetScrollableArea()->ShouldPerformScrollAnchoring())
      block->GetScrollableArea()->GetScrollAnchor()->NotifyBeforeLayout();
  }

  // TODO(layoutng) Can UpdateMarkerTextIfNeeded call be moved
  // somewhere else? List items need up-to-date markers before layout.
  if (IsListItem())
    ToLayoutNGListItem(box_)->UpdateMarkerTextIfNeeded();
}

void NGBlockNode::FinishLayout(
    LayoutBlockFlow* block_flow,
    const NGConstraintSpace& constraint_space,
    const NGBreakToken* break_token,
    scoped_refptr<const NGLayoutResult> layout_result) {
  if (!IsBlockLayoutComplete(constraint_space, *layout_result))
    return;

  box_->SetCachedLayoutResult(*layout_result, break_token);
  if (block_flow) {
    NGLayoutInputNode first_child = FirstChild();
    bool has_inline_children = first_child && first_child.IsInline();
    if (has_inline_children) {
      const auto& physical_fragment =
          To<NGPhysicalBoxFragment>(layout_result->PhysicalFragment());
      CopyFragmentDataToLayoutBoxForInlineChildren(
          physical_fragment, physical_fragment.Size().width,
          Style().IsFlippedBlocksWritingMode());
      block_flow->SetPaintFragment(To<NGBlockBreakToken>(break_token),
                                   &physical_fragment);
    } else if (UNLIKELY(box_->IsLayoutNGFieldset())) {
      // TODO(kojii): NGFieldset should not create PaintFragment.
      block_flow->ClearNGInlineNodeData();
      block_flow->SetPaintFragment(To<NGBlockBreakToken>(break_token),
                                   &layout_result->PhysicalFragment());
    } else {
      // We still need to clear paint fragments in case it had inline children,
      // and thus had NGPaintFragment.
      block_flow->ClearNGInlineNodeData();
      block_flow->SetPaintFragment(To<NGBlockBreakToken>(break_token), nullptr);
    }
  }

  CopyFragmentDataToLayoutBox(constraint_space, *layout_result);
}

MinMaxSize NGBlockNode::ComputeMinMaxSize(
    WritingMode container_writing_mode,
    const MinMaxSizeInput& input,
    const NGConstraintSpace* constraint_space) {
  // TODO(layoutng) Can UpdateMarkerTextIfNeeded call be moved
  // somewhere else? List items need up-to-date markers before layout.
  if (IsListItem())
    ToLayoutNGListItem(box_)->UpdateMarkerTextIfNeeded();

  bool is_orthogonal_flow_root =
      !IsParallelWritingMode(container_writing_mode, Style().GetWritingMode());

  MinMaxSize sizes;
  // If we're orthogonal, we have to run layout to compute the sizes. However,
  // if we're outside of layout, we can't do that. This can happen on Mac.
  if ((!CanUseNewLayout() && !is_orthogonal_flow_root) ||
      (is_orthogonal_flow_root && !box_->GetFrameView()->IsInPerformLayout())) {
    return ComputeMinMaxSizeFromLegacy(input);
  }

  NGConstraintSpace zero_constraint_space =
      CreateConstraintSpaceBuilderForMinMax(*this).ToConstraintSpace();

  if (!constraint_space) {
    // Using the zero-sized constraint space when measuring for an orthogonal
    // flow root isn't going to give the right result.
    DCHECK(!is_orthogonal_flow_root);

    constraint_space = &zero_constraint_space;
  }

  if (is_orthogonal_flow_root || !CanUseNewLayout()) {
    scoped_refptr<const NGLayoutResult> layout_result =
        Layout(*constraint_space);
    DCHECK_EQ(layout_result->Status(), NGLayoutResult::kSuccess);
    NGBoxFragment fragment(
        container_writing_mode,
        TextDirection::kLtr,  // irrelevant here
        To<NGPhysicalBoxFragment>(layout_result->PhysicalFragment()));
    sizes.min_size = sizes.max_size = fragment.Size().inline_size;
    if (input.size_type == NGMinMaxSizeType::kContentBoxSize) {
      sizes -= fragment.Borders().InlineSum() + fragment.Padding().InlineSum() +
               box_->ScrollbarLogicalWidth();
      DCHECK_GE(sizes.min_size, LayoutUnit());
      DCHECK_GE(sizes.max_size, LayoutUnit());
    }
    return sizes;
  }

  NGFragmentGeometry fragment_geometry =
      CalculateInitialMinMaxFragmentGeometry(*constraint_space, *this);
  base::Optional<MinMaxSize> maybe_sizes = ComputeMinMaxSizeWithAlgorithm(
      NGLayoutAlgorithmParams(*this, fragment_geometry, *constraint_space),
      input);

  if (maybe_sizes.has_value()) {
    if (UNLIKELY(IsHTMLMarqueeElement(box_->GetNode()) &&
                 ToHTMLMarqueeElement(box_->GetNode())->IsHorizontal()))
      maybe_sizes->min_size = LayoutUnit();
    return *maybe_sizes;
  }

  if (!box_->GetFrameView()->IsInPerformLayout()) {
    // We can't synthesize these using Layout() if we're not in PerformLayout.
    // This situation can happen on mac. Fall back to legacy instead.
    return ComputeMinMaxSizeFromLegacy(input);
  }

  // Have to synthesize this value.
  scoped_refptr<const NGLayoutResult> layout_result =
      Layout(zero_constraint_space);
  sizes.min_size =
      NGFragment(container_writing_mode, layout_result->PhysicalFragment())
          .InlineSize();

  // Now, redo with infinite space for max_content
  NGConstraintSpace infinite_constraint_space =
      CreateConstraintSpaceBuilderForMinMax(*this)
          .SetAvailableSize({LayoutUnit::Max(), LayoutUnit()})
          .SetPercentageResolutionSize({LayoutUnit(), LayoutUnit()})
          .ToConstraintSpace();

  layout_result = Layout(infinite_constraint_space);
  NGBoxFragment max_fragment(
      container_writing_mode,
      TextDirection::kLtr,  // irrelevant here
      To<NGPhysicalBoxFragment>(layout_result->PhysicalFragment()));
  sizes.max_size = max_fragment.Size().inline_size;

  if (input.size_type == NGMinMaxSizeType::kContentBoxSize) {
    sizes -= max_fragment.Borders().InlineSum() +
             max_fragment.Padding().InlineSum() + box_->ScrollbarLogicalWidth();
    DCHECK_GE(sizes.min_size, LayoutUnit());
    DCHECK_GE(sizes.max_size, LayoutUnit());
  }
  return sizes;
}

MinMaxSize NGBlockNode::ComputeMinMaxSizeFromLegacy(
    const MinMaxSizeInput& input) const {
  bool needs_size_reset = false;
  if (!box_->HasOverrideContainingBlockContentLogicalHeight()) {
    box_->SetOverrideContainingBlockContentLogicalHeight(
        input.percentage_resolution_block_size);
    needs_size_reset = true;
  }

  MinMaxSize sizes;
  // ComputeIntrinsicLogicalWidths returns content-box + scrollbar.
  box_->ComputeIntrinsicLogicalWidths(sizes.min_size, sizes.max_size);
  if (input.size_type == NGMinMaxSizeType::kContentBoxSize) {
    sizes -= LayoutUnit(box_->ScrollbarLogicalWidth());
    DCHECK_GE(sizes.min_size, LayoutUnit());
    DCHECK_GE(sizes.max_size, LayoutUnit());
  } else {
    sizes += box_->BorderAndPaddingLogicalWidth();
  }

  if (needs_size_reset)
    box_->ClearOverrideContainingBlockContentSize();

  return sizes;
}

NGBoxStrut NGBlockNode::GetScrollbarSizes() const {
  NGPhysicalBoxStrut sizes;
  const ComputedStyle& style = box_->StyleRef();
  if (!style.IsOverflowVisible()) {
    LayoutUnit vertical = LayoutUnit(box_->VerticalScrollbarWidth());
    LayoutUnit horizontal = LayoutUnit(box_->HorizontalScrollbarHeight());
    sizes.bottom = horizontal;
    if (box_->ShouldPlaceBlockDirectionScrollbarOnLogicalLeft())
      sizes.left = vertical;
    else
      sizes.right = vertical;
  }
  return sizes.ConvertToLogical(style.GetWritingMode(), style.Direction());
}

NGLayoutInputNode NGBlockNode::NextSibling() const {
  LayoutObject* next_sibling = GetLayoutObjectForNextSiblingNode(box_);
  if (next_sibling) {
    DCHECK(!next_sibling->IsInline());
    return NGBlockNode(ToLayoutBox(next_sibling));
  }
  return nullptr;
}

NGLayoutInputNode NGBlockNode::FirstChild() const {
  auto* block = To<LayoutBlock>(box_);
  auto* child = GetLayoutObjectForFirstChildNode(block);
  if (!child)
    return nullptr;
  if (AreNGBlockFlowChildrenInline(block))
    return NGInlineNode(To<LayoutBlockFlow>(block));
  return NGBlockNode(ToLayoutBox(child));
}

NGBlockNode NGBlockNode::GetRenderedLegend() const {
  if (!IsFieldsetContainer())
    return nullptr;
  return NGBlockNode(LayoutFieldset::FindInFlowLegend(*To<LayoutBlock>(box_)));
}

NGBlockNode NGBlockNode::GetFieldsetContent() const {
  if (!IsFieldsetContainer())
    return nullptr;
  auto* child = GetLayoutObjectForFirstChildNode(To<LayoutBlock>(box_));
  if (!child)
    return nullptr;
  return NGBlockNode(ToLayoutBox(child));
}

bool NGBlockNode::CanUseNewLayout(const LayoutBox& box) {
  DCHECK(RuntimeEnabledFeatures::LayoutNGEnabled());
  if (box.ForceLegacyLayout())
    return false;
  return box.IsLayoutNGMixin() || box.IsLayoutNGFlexibleBox();
}

bool NGBlockNode::CanUseNewLayout() const {
  return CanUseNewLayout(*box_);
}

String NGBlockNode::ToString() const {
  return String::Format("NGBlockNode: '%s'",
                        GetLayoutBox()->DebugName().Ascii().data());
}

void NGBlockNode::CopyFragmentDataToLayoutBox(
    const NGConstraintSpace& constraint_space,
    const NGLayoutResult& layout_result) {
  if (UNLIKELY(constraint_space.IsIntermediateLayout()))
    return;

  const auto& physical_fragment =
      To<NGPhysicalBoxFragment>(layout_result.PhysicalFragment());

  NGBoxFragment fragment(constraint_space.GetWritingMode(),
                         constraint_space.Direction(), physical_fragment);
  LogicalSize fragment_logical_size = fragment.Size();
  // For each fragment we process, we'll accumulate the logical height and
  // logical intrinsic content box height. We reset it at the first fragment,
  // and accumulate at each method call for fragments belonging to the same
  // layout object. Logical width will only be set at the first fragment and is
  // expected to remain the same throughout all subsequent fragments, since
  // legacy layout doesn't support non-uniform fragmentainer widths.
  LayoutUnit intrinsic_content_logical_height;
  if (LIKELY(IsFirstFragment(constraint_space, physical_fragment))) {
    box_->SetSize(LayoutSize(physical_fragment.Size().width,
                             physical_fragment.Size().height));
  } else {
    DCHECK_EQ(box_->LogicalWidth(), fragment_logical_size.inline_size)
        << "Variable fragment inline size not supported";
    box_->SetLogicalHeight(
        PreviouslyUsedBlockSpace(constraint_space, physical_fragment) +
        fragment_logical_size.block_size);
    // TODO(layout-ng): We should store this on the break token instead of
    // relying on previously-stored data. Our relayout in NGBlockNode::Layout
    // will otherwise lead to wrong data.
    intrinsic_content_logical_height = box_->IntrinsicContentLogicalHeight();
  }

  intrinsic_content_logical_height += layout_result.IntrinsicBlockSize();

  NGBoxStrut borders = fragment.Borders();
  NGBoxStrut scrollbars = GetScrollbarSizes();
  NGBoxStrut padding = fragment.Padding();
  NGBoxStrut border_scrollbar_padding = borders + scrollbars + padding;

  if (LIKELY(IsLastFragment(physical_fragment)))
    intrinsic_content_logical_height -= border_scrollbar_padding.BlockSum();
  box_->SetIntrinsicContentLogicalHeight(intrinsic_content_logical_height);

  // TODO(mstensho): This should always be done by the parent algorithm, since
  // we may have auto margins, which only the parent is able to resolve. Remove
  // the following line when all layout modes do this properly.
  if (UNLIKELY(box_->IsTableCell())) {
    // Table-cell margins compute to zero.
    box_->SetMargin(NGPhysicalBoxStrut());
  } else {
    box_->SetMargin(ComputePhysicalMargins(constraint_space, Style()));
  }

  auto* block_flow = DynamicTo<LayoutBlockFlow>(box_);
  LayoutMultiColumnFlowThread* flow_thread = GetFlowThread(block_flow);
  if (UNLIKELY(flow_thread)) {
    PlaceChildrenInFlowThread(constraint_space, physical_fragment);
  } else {
    PhysicalOffset offset_from_start;
    if (UNLIKELY(constraint_space.HasBlockFragmentation())) {
      // Need to include any block space that this container has used in
      // previous fragmentainers. The offset of children will be relative to
      // the container, in flow thread coordinates, i.e. the model where
      // everything is represented as one single strip, rather than being
      // sliced and translated into columns.

      // TODO(mstensho): writing modes
      offset_from_start.top =
          PreviouslyUsedBlockSpace(constraint_space, physical_fragment);
    }
    PlaceChildrenInLayoutBox(constraint_space, physical_fragment,
                             offset_from_start);
  }

  LayoutBlock* block = DynamicTo<LayoutBlock>(box_);
  if (LIKELY(block && IsLastFragment(physical_fragment))) {
    LayoutUnit intrinsic_block_size = layout_result.IntrinsicBlockSize();
    if (UNLIKELY(constraint_space.HasBlockFragmentation())) {
      intrinsic_block_size +=
          PreviouslyUsedBlockSpace(constraint_space, physical_fragment);
    }

#if DCHECK_IS_ON()
    block->CheckPositionedObjectsNeedLayout();
#endif

    if (UNLIKELY(flow_thread)) {
      UpdateLegacyMultiColumnFlowThread(*this, flow_thread, constraint_space,
                                        physical_fragment);
    }

    // |ComputeOverflow()| below calls |AddVisualOverflowFromChildren()|, which
    // computes visual overflow from |RootInlineBox| if |ChildrenInline()|
    block->SetNeedsOverflowRecalc();
    block->ComputeLayoutOverflow(intrinsic_block_size - borders.block_end -
                                 scrollbars.block_end);
  }

  box_->UpdateAfterLayout();
  box_->ClearNeedsLayout();

  // Overflow computation depends on this being set.
  if (LIKELY(block_flow))
    block_flow->UpdateIsSelfCollapsing();
}

void NGBlockNode::PlaceChildrenInLayoutBox(
    const NGConstraintSpace& constraint_space,
    const NGPhysicalBoxFragment& physical_fragment,
    const PhysicalOffset& offset_from_start) {
  LayoutBox* rendered_legend = nullptr;
  for (const auto& child_fragment : physical_fragment.Children()) {
    // Skip any line-boxes we have as children, this is handled within
    // NGInlineNode at the moment.
    if (!child_fragment->IsBox() && !child_fragment->IsRenderedLegend())
      continue;

    const auto& box_fragment = *To<NGPhysicalBoxFragment>(child_fragment.get());
    if (IsFirstFragment(constraint_space, box_fragment)) {
      if (box_fragment.IsRenderedLegend())
        rendered_legend = ToLayoutBox(box_fragment.GetLayoutObject());
      CopyChildFragmentPosition(box_fragment, child_fragment.Offset(),
                                offset_from_start);
    }
  }

  if (rendered_legend) {
    // The rendered legend is a child of the the anonymous fieldset content
    // child wrapper object on the legacy side. LayoutNG, on the other hand,
    // generates a fragment for the rendered legend as a direct child of the
    // fieldset container fragment (as a *sibling* preceding the anonymous
    // fieldset content wrapper). Now that we have positioned the anonymous
    // wrapper, we're ready to compensate for this discrepancy. See
    // LayoutNGFieldset for more details.
    LayoutBlock* content_wrapper = rendered_legend->ContainingBlock();
    DCHECK(content_wrapper->IsAnonymous());
    DCHECK(IsHTMLFieldSetElement(content_wrapper->Parent()->GetNode()));
    LayoutPoint location = rendered_legend->Location();
    location -= content_wrapper->Location();
    rendered_legend->SetLocation(location);
  }
}

void NGBlockNode::PlaceChildrenInFlowThread(
    const NGConstraintSpace& constraint_space,
    const NGPhysicalBoxFragment& physical_fragment) {
  LayoutUnit flowthread_offset;
  for (const auto& child : physical_fragment.Children()) {
    // Each anonymous child of a multicol container constitutes one column.
    DCHECK(child->GetLayoutObject() == box_);

    // TODO(mstensho): writing modes
    PhysicalOffset offset(LayoutUnit(), flowthread_offset);

    // Position each child node in the first column that they occur, relatively
    // to the block-start of the flow thread.
    const auto* column = To<NGPhysicalBoxFragment>(child.get());
    PlaceChildrenInLayoutBox(constraint_space, *column, offset);
    const auto* token = To<NGBlockBreakToken>(column->BreakToken());
    flowthread_offset = token->UsedBlockSize();
  }
}

// Copies data back to the legacy layout tree for a given child fragment.
void NGBlockNode::CopyChildFragmentPosition(
    const NGPhysicalFragment& fragment,
    const PhysicalOffset fragment_offset,
    const PhysicalOffset additional_offset) {
  LayoutBox* layout_box = ToLayoutBox(fragment.GetLayoutObject());
  if (!layout_box)
    return;

  DCHECK(layout_box->Parent()) << "Should be called on children only.";

  // The containing block of |layout_box| on the legacy layout side is normally
  // |box_|, but this is not an invariant. Among other things, it does not apply
  // to list item markers and multicol container children. Multicol containiner
  // children typically have their flow thread (not the multicol container
  // itself) as their containing block, and we need to use the right containing
  // block for inserting floats, flipping for writing modes, etc.
  LayoutBlock* containing_block = layout_box->ContainingBlock();

  // LegacyLayout flips vertical-rl horizontal coordinates before paint.
  // NGLayout flips X location for LegacyLayout compatibility. horizontal_offset
  // will be the offset from the left edge of the container to the left edge of
  // the layout object, except when in vertical-rl: Then it will be the offset
  // from the right edge of the container to the right edge of the layout
  // object.
  LayoutUnit horizontal_offset = fragment_offset.left + additional_offset.left;
  bool has_flipped_x_axis =
      containing_block->StyleRef().IsFlippedBlocksWritingMode();
  if (has_flipped_x_axis) {
    horizontal_offset = containing_block->Size().Width() - horizontal_offset -
                        fragment.Size().width;
  }
  layout_box->SetLocation(LayoutPoint(
      horizontal_offset, fragment_offset.top + additional_offset.top));
}

// For inline children, NG painters handles fragments directly, but there are
// some cases where we need to copy data to the LayoutObject tree. This function
// handles such cases.
void NGBlockNode::CopyFragmentDataToLayoutBoxForInlineChildren(
    const NGPhysicalContainerFragment& container,
    LayoutUnit initial_container_width,
    bool initial_container_is_flipped,
    PhysicalOffset offset) {
  for (const auto& child : container.Children()) {
    if (child->IsContainer()) {
      PhysicalOffset child_offset = offset + child.Offset();

      // Replaced elements and inline blocks need Location() set relative to
      // their block container.
      LayoutObject* layout_object = child->GetLayoutObject();
      if (layout_object && layout_object->IsBox()) {
        LayoutBox& layout_box = ToLayoutBox(*layout_object);
        PhysicalOffset maybe_flipped_offset = child_offset;
        if (initial_container_is_flipped) {
          maybe_flipped_offset.left = initial_container_width -
                                      child->Size().width -
                                      maybe_flipped_offset.left;
        }
        layout_box.SetLocation(maybe_flipped_offset.ToLayoutPoint());
      }

      // Legacy compatibility. This flag is used in paint layer for
      // invalidation.
      if (layout_object && layout_object->IsLayoutInline() &&
          layout_object->StyleRef().HasOutline() &&
          !layout_object->IsElementContinuation() &&
          ToLayoutInline(layout_object)->Continuation()) {
        box_->SetContainsInlineWithOutlineAndContinuation(true);
      }

      // The Location() of inline LayoutObject is relative to the
      // LayoutBlockFlow. If |child| establishes a new block formatting context,
      // it also creates another inline formatting context. Do not copy to its
      // descendants in this case.
      if (!child->IsBlockFormattingContextRoot()) {
        CopyFragmentDataToLayoutBoxForInlineChildren(
            To<NGPhysicalContainerFragment>(*child), initial_container_width,
            initial_container_is_flipped, child_offset);
      }
    }
  }
}

bool NGBlockNode::ChildrenInline() const {
  if (const auto* block = DynamicTo<LayoutBlockFlow>(box_))
    return AreNGBlockFlowChildrenInline(block);
  return false;
}

bool NGBlockNode::IsInlineLevel() const {
  return GetLayoutBox()->IsInline();
}

bool NGBlockNode::IsAtomicInlineLevel() const {
  // LayoutObject::IsAtomicInlineLevel() returns true for e.g., <img
  // style="display: block">. Check IsInline() as well.
  return GetLayoutBox()->IsAtomicInlineLevel() && GetLayoutBox()->IsInline();
}

bool NGBlockNode::UseLogicalBottomMarginEdgeForInlineBlockBaseline() const {
  auto* layout_box = DynamicTo<LayoutBlock>(GetLayoutBox());
  return layout_box &&
         layout_box->UseLogicalBottomMarginEdgeForInlineBlockBaseline();
}

scoped_refptr<const NGLayoutResult> NGBlockNode::LayoutAtomicInline(
    const NGConstraintSpace& parent_constraint_space,
    const ComputedStyle& parent_style,
    FontBaseline baseline_type,
    bool use_first_line_style) {
  NGConstraintSpaceBuilder builder(
      parent_constraint_space, Style().GetWritingMode(), /* is_new_fc */ true);
  SetOrthogonalFallbackInlineSizeIfNeeded(parent_style, *this, &builder);

  builder.SetUseFirstLineStyle(use_first_line_style);

  // Request to compute baseline during the layout, except when we know the box
  // would synthesize box-baseline.
  LayoutBox* layout_box = GetLayoutBox();
  if (NGBaseline::ShouldPropagateBaselines(layout_box)) {
    builder.AddBaselineRequest(
        {NGBaselineAlgorithmType::kAtomicInline, baseline_type});
  }

  NGConstraintSpace constraint_space =
      builder.SetIsShrinkToFit(Style().LogicalWidth().IsAuto())
          .SetAvailableSize(parent_constraint_space.AvailableSize())
          .SetPercentageResolutionSize(
              parent_constraint_space.PercentageResolutionSize())
          .SetReplacedPercentageResolutionSize(
              parent_constraint_space.ReplacedPercentageResolutionSize())
          .SetTextDirection(Style().Direction())
          .ToConstraintSpace();
  scoped_refptr<const NGLayoutResult> result = Layout(constraint_space);
  // TODO(kojii): Investigate why ClearNeedsLayout() isn't called automatically
  // when it's being laid out.
  if (!constraint_space.IsIntermediateLayout())
    layout_box->ClearNeedsLayout();
  return result;
}

scoped_refptr<const NGLayoutResult> NGBlockNode::RunLegacyLayout(
    const NGConstraintSpace& constraint_space) {
  // This is an exit-point from LayoutNG to the legacy engine. This means that
  // we need to be at a formatting context boundary, since NG and legacy don't
  // cooperate on e.g. margin collapsing.
  DCHECK(!box_->IsLayoutBlock() ||
         To<LayoutBlock>(box_)->CreatesNewFormattingContext());

  scoped_refptr<const NGLayoutResult> layout_result =
      box_->GetCachedLayoutResult();

  // We need to force a layout on the child if the constraint space given will
  // change the layout.
  bool needs_force_relayout =
      layout_result &&
      !MaySkipLegacyLayout(*this, *layout_result, constraint_space);

  if (box_->NeedsLayout() || !layout_result || needs_force_relayout) {
    BoxLayoutExtraInput input(*box_);
    input.containing_block_content_inline_size =
        CalculateAvailableInlineSizeForLegacy(*box_, constraint_space);
    input.containing_block_content_block_size =
        CalculateAvailableBlockSizeForLegacy(*box_, constraint_space);

    WritingMode writing_mode = Style().GetWritingMode();
    if (LayoutObject* containing_block = box_->ContainingBlock()) {
      if (!IsParallelWritingMode(containing_block->StyleRef().GetWritingMode(),
                                 writing_mode)) {
        // The sizes should be in the containing block writing mode.
        std::swap(input.containing_block_content_block_size,
                  input.containing_block_content_inline_size);
      }
    }
    input.available_inline_size = constraint_space.AvailableSize().inline_size;

    if (constraint_space.IsFixedSizeInline())
      input.override_inline_size = constraint_space.AvailableSize().inline_size;
    if (constraint_space.IsFixedSizeBlock())
      input.override_block_size = constraint_space.AvailableSize().block_size;
    box_->ComputeAndSetBlockDirectionMargins(box_->ContainingBlock());

    // Using |LayoutObject::LayoutIfNeeded| save us a little bit of overhead,
    // compared to |LayoutObject::ForceLayout|.
    DCHECK(!box_->IsLayoutNGMixin());
    if (box_->NeedsLayout() && !needs_force_relayout)
      box_->LayoutIfNeeded();
    else
      box_->ForceLayout();

    // Synthesize a new layout result.
    NGFragmentGeometry fragment_geometry;
    fragment_geometry.border_box_size = {box_->LogicalWidth(),
                                         box_->LogicalHeight()};
    fragment_geometry.border = {box_->BorderStart(), box_->BorderEnd(),
                                box_->BorderBefore(), box_->BorderAfter()};
    fragment_geometry.scrollbar = GetScrollbarSizes();
    fragment_geometry.padding = {box_->PaddingStart(), box_->PaddingEnd(),
                                 box_->PaddingBefore(), box_->PaddingAfter()};

    // TODO(kojii): Implement use_first_line_style.
    NGBoxFragmentBuilder builder(*this, box_->Style(), &constraint_space,
                                 writing_mode, box_->StyleRef().Direction());
    builder.SetIsNewFormattingContext(
        constraint_space.IsNewFormattingContext());
    builder.SetInitialFragmentGeometry(fragment_geometry);
    builder.SetIsLegacyLayoutRoot();

    CopyBaselinesFromLegacyLayout(constraint_space, &builder);
    layout_result = builder.ToBoxFragment();

    box_->SetCachedLayoutResult(*layout_result, /* break_token */ nullptr);
  }

  UpdateShapeOutsideInfoIfNeeded(
      *layout_result, constraint_space.PercentageResolutionInlineSize());

  return layout_result;
}

void NGBlockNode::CopyBaselinesFromLegacyLayout(
    const NGConstraintSpace& constraint_space,
    NGBoxFragmentBuilder* builder) {
  const NGBaselineRequestList requests = constraint_space.BaselineRequests();
  if (requests.IsEmpty())
    return;

  if (UNLIKELY(constraint_space.GetWritingMode() != Style().GetWritingMode()))
    return;

  for (const auto& request : requests) {
    switch (request.AlgorithmType()) {
      case NGBaselineAlgorithmType::kAtomicInline: {
        LayoutUnit position =
            AtomicInlineBaselineFromLegacyLayout(request, constraint_space);
        if (position != -1)
          builder->AddBaseline(request, position);
        break;
      }
      case NGBaselineAlgorithmType::kFirstLine: {
        LayoutUnit position = box_->FirstLineBoxBaseline();
        if (position != -1)
          builder->AddBaseline(request, position);
        break;
      }
    }
  }
}

LayoutUnit NGBlockNode::AtomicInlineBaselineFromLegacyLayout(
    const NGBaselineRequest& request,
    const NGConstraintSpace& constraint_space) {
  LineDirectionMode line_direction = box_->IsHorizontalWritingMode()
                                         ? LineDirectionMode::kHorizontalLine
                                         : LineDirectionMode::kVerticalLine;

  // If this is an inline box, use |BaselinePosition()|. Some LayoutObject
  // classes override it assuming inline layout calls |BaselinePosition()|.
  if (box_->IsInline()) {
    LayoutUnit position = LayoutUnit(box_->BaselinePosition(
        request.BaselineType(), constraint_space.UseFirstLineStyle(),
        line_direction, kPositionOnContainingLine));

    // BaselinePosition() uses margin edge for atomic inlines. Subtract
    // margin-over so that the position is relative to the border box.
    if (box_->IsAtomicInlineLevel())
      position -= box_->MarginOver();

    return position;
  }

  // If this is a block box, use |InlineBlockBaseline()|. When an inline block
  // has block children, their inline block baselines need to be propagated.
  return box_->InlineBlockBaseline(line_direction);
}

// Floats can optionally have a shape area, specifed by "shape-outside". The
// current shape machinery requires setting the size of the float after layout
// in the parents writing mode.
void NGBlockNode::UpdateShapeOutsideInfoIfNeeded(
    const NGLayoutResult& layout_result,
    LayoutUnit percentage_resolution_inline_size) {
  if (!box_->IsFloating() || !box_->GetShapeOutsideInfo())
    return;

  // The box_ may not have a valid size yet (due to an intermediate layout),
  // use the fragment's size instead.
  LayoutSize box_size = layout_result.PhysicalFragment().Size().ToLayoutSize();

  // TODO(ikilpatrick): Ideally this should be moved to a NGLayoutResult
  // computing the shape area. There may be an issue with the new fragmentation
  // model and computing the correct sizes of shapes.
  ShapeOutsideInfo* shape_outside = box_->GetShapeOutsideInfo();
  LayoutBlock* containing_block = box_->ContainingBlock();
  shape_outside->SetReferenceBoxLogicalSize(
      containing_block->IsHorizontalWritingMode() ? box_size
                                                  : box_size.TransposedSize());
  shape_outside->SetPercentageResolutionInlineSize(
      percentage_resolution_inline_size);
}

void NGBlockNode::UseLegacyOutOfFlowPositioning() const {
  DCHECK(box_->IsOutOfFlowPositioned());
  box_->ContainingBlock()->InsertPositionedObject(box_);
}

// Save static position for legacy AbsPos layout.
void NGBlockNode::SaveStaticOffsetForLegacy(
    const LogicalOffset& offset,
    const LayoutObject* offset_container) {
  DCHECK(box_->IsOutOfFlowPositioned());
  // Only set static position if the current offset container
  // is one that Legacy layout expects static offset from.
  const LayoutObject* parent = box_->Parent();
  if (parent == offset_container ||
      (parent && parent->IsLayoutInline() &&
       parent->ContainingBlock() == offset_container)) {
    DCHECK(box_->Layer());
    box_->Layer()->SetStaticBlockPosition(offset.block_offset);
    box_->Layer()->SetStaticInlinePosition(offset.inline_offset);
  }
}

void NGBlockNode::StoreMargins(const NGConstraintSpace& constraint_space,
                               const NGBoxStrut& margins) {
  if (constraint_space.IsIntermediateLayout())
    return;
  NGPhysicalBoxStrut physical_margins = margins.ConvertToPhysical(
      constraint_space.GetWritingMode(), constraint_space.Direction());
  box_->SetMargin(physical_margins);
}

}  // namespace blink
