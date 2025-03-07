/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Dirk Mueller (mueller@kde.org)
 *           (C) 2004 Allan Sandfeld Jensen (kde@carewolf.com)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2011 Apple Inc.
 *               All rights reserved.
 * Copyright (C) 2009 Google Inc. All rights reserved.
 * Copyright (C) 2009 Torch Mobile Inc. All rights reserved.
 *               (http://www.torchmobile.com/)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#include "third_party/blink/renderer/core/layout/layout_object.h"

#include <algorithm>
#include <memory>
#include <utility>

#include "third_party/blink/public/platform/web_scroll_into_view_params.h"
#include "third_party/blink/renderer/core/accessibility/ax_object_cache.h"
#include "third_party/blink/renderer/core/animation/element_animations.h"
#include "third_party/blink/renderer/core/css/resolver/style_resolver.h"
#include "third_party/blink/renderer/core/css/style_change_reason.h"
#include "third_party/blink/renderer/core/css/style_engine.h"
#include "third_party/blink/renderer/core/dom/element_traversal.h"
#include "third_party/blink/renderer/core/dom/first_letter_pseudo_element.h"
#include "third_party/blink/renderer/core/dom/shadow_root.h"
#include "third_party/blink/renderer/core/editing/editing_utilities.h"
#include "third_party/blink/renderer/core/editing/ephemeral_range.h"
#include "third_party/blink/renderer/core/editing/frame_selection.h"
#include "third_party/blink/renderer/core/editing/layout_selection.h"
#include "third_party/blink/renderer/core/editing/position_with_affinity.h"
#include "third_party/blink/renderer/core/editing/text_affinity.h"
#include "third_party/blink/renderer/core/editing/visible_units.h"
#include "third_party/blink/renderer/core/frame/deprecated_schedule_style_recalc_during_layout.h"
#include "third_party/blink/renderer/core/frame/event_handler_registry.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_view.h"
#include "third_party/blink/renderer/core/frame/settings.h"
#include "third_party/blink/renderer/core/frame/use_counter.h"
#include "third_party/blink/renderer/core/html/html_element.h"
#include "third_party/blink/renderer/core/html/html_html_element.h"
#include "third_party/blink/renderer/core/html/html_table_cell_element.h"
#include "third_party/blink/renderer/core/html/html_table_element.h"
#include "third_party/blink/renderer/core/input/event_handler.h"
#include "third_party/blink/renderer/core/layout/custom/layout_custom.h"
#include "third_party/blink/renderer/core/layout/geometry/transform_state.h"
#include "third_party/blink/renderer/core/layout/hit_test_result.h"
#include "third_party/blink/renderer/core/layout/layout_counter.h"
#include "third_party/blink/renderer/core/layout/layout_deprecated_flexible_box.h"
#include "third_party/blink/renderer/core/layout/layout_embedded_content.h"
#include "third_party/blink/renderer/core/layout/layout_fieldset.h"
#include "third_party/blink/renderer/core/layout/layout_flexible_box.h"
#include "third_party/blink/renderer/core/layout/layout_flow_thread.h"
#include "third_party/blink/renderer/core/layout/layout_grid.h"
#include "third_party/blink/renderer/core/layout/layout_image.h"
#include "third_party/blink/renderer/core/layout/layout_image_resource_style_image.h"
#include "third_party/blink/renderer/core/layout/layout_inline.h"
#include "third_party/blink/renderer/core/layout/layout_list_item.h"
#include "third_party/blink/renderer/core/layout/layout_multi_column_spanner_placeholder.h"
#include "third_party/blink/renderer/core/layout/layout_object_factory.h"
#include "third_party/blink/renderer/core/layout/layout_scrollbar_part.h"
#include "third_party/blink/renderer/core/layout/layout_table_caption.h"
#include "third_party/blink/renderer/core/layout/layout_table_cell.h"
#include "third_party/blink/renderer/core/layout/layout_table_col.h"
#include "third_party/blink/renderer/core/layout/layout_table_row.h"
#include "third_party/blink/renderer/core/layout/layout_text_fragment.h"
#include "third_party/blink/renderer/core/layout/layout_theme.h"
#include "third_party/blink/renderer/core/layout/layout_view.h"
#include "third_party/blink/renderer/core/layout/ng/layout_ng_block_flow.h"
#include "third_party/blink/renderer/core/layout/ng/ng_block_node.h"
#include "third_party/blink/renderer/core/layout/ng/ng_layout_result.h"
#include "third_party/blink/renderer/core/layout/ng/ng_unpositioned_float.h"
#include "third_party/blink/renderer/core/layout/svg/layout_svg_resource_clipper.h"
#include "third_party/blink/renderer/core/layout/svg/svg_resources.h"
#include "third_party/blink/renderer/core/layout/svg/svg_resources_cache.h"
#include "third_party/blink/renderer/core/page/autoscroll_controller.h"
#include "third_party/blink/renderer/core/page/page.h"
#include "third_party/blink/renderer/core/paint/image_element_timing.h"
#include "third_party/blink/renderer/core/paint/ng/ng_paint_fragment.h"
#include "third_party/blink/renderer/core/paint/object_paint_invalidator.h"
#include "third_party/blink/renderer/core/paint/paint_layer.h"
#include "third_party/blink/renderer/core/paint/paint_layer_scrollable_area.h"
#include "third_party/blink/renderer/core/paint/paint_timing_detector.h"
#include "third_party/blink/renderer/core/scroll/smooth_scroll_sequencer.h"
#include "third_party/blink/renderer/core/style/content_data.h"
#include "third_party/blink/renderer/core/style/cursor_data.h"
#include "third_party/blink/renderer/platform/graphics/graphics_layer.h"
#include "third_party/blink/renderer/platform/graphics/paint/geometry_mapper.h"
#include "third_party/blink/renderer/platform/graphics/paint/property_tree_state.h"
#include "third_party/blink/renderer/platform/graphics/touch_action.h"
#include "third_party/blink/renderer/platform/instance_counters.h"
#include "third_party/blink/renderer/platform/instrumentation/tracing/traced_value.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"
#include "third_party/blink/renderer/platform/wtf/allocator/partitions.h"
#include "third_party/blink/renderer/platform/wtf/text/string_builder.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"
#ifndef NDEBUG
#include <stdio.h>
#endif

namespace blink {

namespace {

// In order for an image to be rendered from the content property, there can be
// at most one piece of image content data, followed by some optional
// alternative text.
bool ShouldUseContentData(const ContentData* content_data) {
  if (!content_data)
    return false;
  if (!content_data->IsImage())
    return false;
  if (content_data->Next() && !content_data->Next()->IsAltText())
    return false;

  return true;
}

template <typename Predicate>
LayoutObject* FindAncestorByPredicate(const LayoutObject* descendant,
                                      LayoutObject::AncestorSkipInfo* skip_info,
                                      Predicate predicate) {
  for (auto* object = descendant->Parent(); object; object = object->Parent()) {
    if (predicate(object))
      return object;
    if (skip_info)
      skip_info->Update(*object);
  }
  return nullptr;
}

LayoutBlock* FindContainingBlock(LayoutObject* container,
                                 LayoutObject::AncestorSkipInfo* skip_info) {
  // For inlines, we return the nearest non-anonymous enclosing
  // block. We don't try to return the inline itself. This allows us to avoid
  // having a positioned objects list in all LayoutInlines and lets us return a
  // strongly-typed LayoutBlock* result from this method. The
  // LayoutObject::Container() method can actually be used to obtain the inline
  // directly.
  if (container && container->IsInline() && !container->IsAtomicInlineLevel()) {
    DCHECK(container->StyleRef().HasInFlowPosition() ||
           container->StyleRef().HasFilter());
    container = container->ContainingBlock(skip_info);
  }

  if (container && !container->IsLayoutBlock())
    container = container->ContainingBlock(skip_info);

  while (container && container->IsAnonymousBlock())
    container = container->ContainingBlock(skip_info);

  return DynamicTo<LayoutBlock>(container);
}

}  // namespace

#if DCHECK_IS_ON()

LayoutObject::SetLayoutNeededForbiddenScope::SetLayoutNeededForbiddenScope(
    LayoutObject& layout_object)
    : layout_object_(layout_object),
      preexisting_forbidden_(layout_object_.IsSetNeedsLayoutForbidden()) {
  layout_object_.SetNeedsLayoutIsForbidden(true);
}

LayoutObject::SetLayoutNeededForbiddenScope::~SetLayoutNeededForbiddenScope() {
  layout_object_.SetNeedsLayoutIsForbidden(preexisting_forbidden_);
}
#endif

struct SameSizeAsLayoutObject : DisplayItemClient {
  // Normally this field uses the gap between DisplayItemClient and
  // LayoutObject's other fields.
  uint8_t paint_invalidation_reason_;
  void* pointers[5];
  Member<void*> members[1];
#if DCHECK_IS_ON()
  unsigned debug_bitfields_;
#endif
  unsigned bitfields_;
  unsigned bitfields2_;
  unsigned bitfields3_;
  // The following fields are in FragmentData.
  LayoutRect visual_rect_;
  LayoutPoint paint_offset_;
  std::unique_ptr<int> rare_data_;
  std::unique_ptr<FragmentData> next_fragment_;
};

static_assert(sizeof(LayoutObject) == sizeof(SameSizeAsLayoutObject),
              "LayoutObject should stay small");

bool LayoutObject::affects_parent_block_ = false;

void* LayoutObject::operator new(size_t sz) {
  DCHECK(IsMainThread());
  return WTF::Partitions::LayoutPartition()->Alloc(
      sz, WTF_HEAP_PROFILER_TYPE_NAME(LayoutObject));
}

void LayoutObject::operator delete(void* ptr) {
  DCHECK(IsMainThread());
  base::PartitionFree(ptr);
}

LayoutObject* LayoutObject::CreateObject(Element* element,
                                         const ComputedStyle& style,
                                         LegacyLayout legacy) {
  DCHECK(IsAllowedToModifyLayoutTreeStructure(element->GetDocument()));

  // Minimal support for content properties replacing an entire element.
  // Works only if we have exactly one piece of content and it's a URL, with
  // some optional alternative text. Otherwise acts as if we didn't support this
  // feature.
  const ContentData* content_data = style.GetContentData();
  if (!element->IsPseudoElement() && ShouldUseContentData(content_data)) {
    LayoutImage* image = new LayoutImage(element);
    // LayoutImageResourceStyleImage requires a style being present on the
    // image but we don't want to trigger a style change now as the node is
    // not fully attached. Moving this code to style change doesn't make sense
    // as it should be run once at layoutObject creation.
    image->SetStyleInternal(const_cast<ComputedStyle*>(&style));
    if (const StyleImage* style_image =
            To<ImageContentData>(content_data)->GetImage()) {
      image->SetImageResource(
          MakeGarbageCollected<LayoutImageResourceStyleImage>(
              const_cast<StyleImage*>(style_image)));
      image->SetIsGeneratedContent();
    } else {
      image->SetImageResource(MakeGarbageCollected<LayoutImageResource>());
    }
    image->SetStyleInternal(nullptr);
    return image;
  }

  switch (style.Display()) {
    case EDisplay::kNone:
    case EDisplay::kContents:
      return nullptr;
    case EDisplay::kInline:
      return new LayoutInline(element);
    case EDisplay::kBlock:
    case EDisplay::kFlowRoot:
    case EDisplay::kInlineBlock:
      return LayoutObjectFactory::CreateBlockFlow(*element, style, legacy);
    case EDisplay::kListItem:
      return LayoutObjectFactory::CreateListItem(*element, style, legacy);
    case EDisplay::kTable:
    case EDisplay::kInlineTable:
      return new LayoutTable(element);
    case EDisplay::kTableRowGroup:
    case EDisplay::kTableHeaderGroup:
    case EDisplay::kTableFooterGroup:
      return new LayoutTableSection(element);
    case EDisplay::kTableRow:
      return new LayoutTableRow(element);
    case EDisplay::kTableColumnGroup:
    case EDisplay::kTableColumn:
      return new LayoutTableCol(element);
    case EDisplay::kTableCell:
      return LayoutObjectFactory::CreateTableCell(*element, style, legacy);
    case EDisplay::kTableCaption:
      return LayoutObjectFactory::CreateTableCaption(*element, style, legacy);
    case EDisplay::kWebkitBox:
    case EDisplay::kWebkitInlineBox:
      return new LayoutDeprecatedFlexibleBox(*element);
    case EDisplay::kFlex:
    case EDisplay::kInlineFlex:
      UseCounter::Count(element->GetDocument(), WebFeature::kCSSFlexibleBox);
      return LayoutObjectFactory::CreateFlexibleBox(*element, style, legacy);
    case EDisplay::kGrid:
    case EDisplay::kInlineGrid:
      UseCounter::Count(element->GetDocument(), WebFeature::kCSSGridLayout);
      return new LayoutGrid(element);
    case EDisplay::kLayoutCustom:
    case EDisplay::kInlineLayoutCustom:
      return new LayoutCustom(element);
  }

  NOTREACHED();
  return nullptr;
}

LayoutObject::LayoutObject(Node* node)
    : full_paint_invalidation_reason_(PaintInvalidationReason::kNone),
      style_(nullptr),
      node_(node),
      parent_(nullptr),
      previous_(nullptr),
      next_(nullptr),
#if DCHECK_IS_ON()
      has_ax_object_(false),
      set_needs_layout_forbidden_(false),
      as_image_observer_count_(0),
#endif
      bitfields_(node) {
  InstanceCounters::IncrementCounter(InstanceCounters::kLayoutObjectCounter);
  if (node_)
    GetFrameView()->IncrementLayoutObjectCount();

  if (const Node* node = GetNode()) {
    if (const Element* element = ToElementOrNull(node)) {
      if (element->ShouldForceLegacyLayout())
        SetForceLegacyLayout();
    }
  }
}

LayoutObject::~LayoutObject() {
#if DCHECK_IS_ON()
  DCHECK(!has_ax_object_);
#endif
  InstanceCounters::DecrementCounter(InstanceCounters::kLayoutObjectCounter);
}

bool LayoutObject::IsDescendantOf(const LayoutObject* obj) const {
  for (const LayoutObject* r = this; r; r = r->parent_) {
    if (r == obj)
      return true;
  }
  return false;
}

bool LayoutObject::IsHR() const {
  return IsHTMLHRElement(GetNode());
}

void LayoutObject::SetIsInsideFlowThreadIncludingDescendants(
    bool inside_flow_thread) {
  LayoutObject* next;
  for (LayoutObject* object = this; object; object = next) {
    // If object is a fragmentation context it already updated the descendants
    // flag accordingly.
    if (object->IsLayoutFlowThread()) {
      next = object->NextInPreOrderAfterChildren(this);
      continue;
    }
    next = object->NextInPreOrder(this);
    DCHECK_NE(inside_flow_thread, object->IsInsideFlowThread());
    object->SetIsInsideFlowThread(inside_flow_thread);
  }
}

bool LayoutObject::RequiresAnonymousTableWrappers(
    const LayoutObject* new_child) const {
  // Check should agree with:
  // CSS 2.1 Tables: 17.2.1 Anonymous table objects
  // http://www.w3.org/TR/CSS21/tables.html#anonymous-boxes
  if (new_child->IsLayoutTableCol()) {
    const LayoutTableCol* new_table_column = ToLayoutTableCol(new_child);
    bool is_column_in_column_group =
        new_table_column->IsTableColumn() && IsLayoutTableCol();
    return !IsTable() && !is_column_in_column_group;
  }
  if (new_child->IsTableCaption())
    return !IsTable();
  if (new_child->IsTableSection())
    return !IsTable();
  if (new_child->IsTableRow())
    return !IsTableSection();
  if (new_child->IsTableCell())
    return !IsTableRow();
  return false;
}

DISABLE_CFI_PERF
void LayoutObject::AddChild(LayoutObject* new_child,
                            LayoutObject* before_child) {
  DCHECK(IsAllowedToModifyLayoutTreeStructure(GetDocument()));

  LayoutObjectChildList* children = VirtualChildren();
  DCHECK(children);
  if (!children)
    return;

  if (RequiresAnonymousTableWrappers(new_child)) {
    // Generate an anonymous table or reuse existing one from previous child
    // Per: 17.2.1 Anonymous table objects 3. Generate missing parents
    // http://www.w3.org/TR/CSS21/tables.html#anonymous-boxes
    LayoutTable* table;
    LayoutObject* after_child =
        before_child ? before_child->PreviousSibling() : children->LastChild();
    if (after_child && after_child->IsAnonymous() && after_child->IsTable() &&
        !after_child->IsBeforeContent()) {
      table = ToLayoutTable(after_child);
    } else {
      table = LayoutTable::CreateAnonymousWithParent(this);
      children->InsertChildNode(this, table, before_child);
    }
    table->AddChild(new_child);
  } else {
    children->InsertChildNode(this, new_child, before_child);
  }

  if (new_child->IsText() &&
      new_child->StyleRef().TextTransform() == ETextTransform::kCapitalize)
    ToLayoutText(new_child)->TransformText();
}

void LayoutObject::RemoveChild(LayoutObject* old_child) {
  DCHECK(IsAllowedToModifyLayoutTreeStructure(GetDocument()));

  LayoutObjectChildList* children = VirtualChildren();
  DCHECK(children);
  if (!children)
    return;

  children->RemoveChildNode(this, old_child);
}

void LayoutObject::SetDangerousOneWayParent(LayoutObject* parent) {
  DCHECK(!PreviousSibling());
  DCHECK(!NextSibling());
  DCHECK(!parent || !parent_);
  SetParent(parent);
}

void LayoutObject::RegisterSubtreeChangeListenerOnDescendants(bool value) {
  // If we're set to the same value then we're done as that means it's
  // set down the tree that way already.
  if (bitfields_.SubtreeChangeListenerRegistered() == value)
    return;

  bitfields_.SetSubtreeChangeListenerRegistered(value);

  for (LayoutObject* curr = SlowFirstChild(); curr; curr = curr->NextSibling())
    curr->RegisterSubtreeChangeListenerOnDescendants(value);
}

void LayoutObject::NotifyAncestorsOfSubtreeChange() {
  if (bitfields_.NotifiedOfSubtreeChange())
    return;

  bitfields_.SetNotifiedOfSubtreeChange(true);
  if (Parent())
    Parent()->NotifyAncestorsOfSubtreeChange();
}

void LayoutObject::NotifyOfSubtreeChange() {
  if (!bitfields_.SubtreeChangeListenerRegistered())
    return;
  if (bitfields_.NotifiedOfSubtreeChange())
    return;

  NotifyAncestorsOfSubtreeChange();

  // We can modify the layout tree during layout which means that we may
  // try to schedule this during performLayout. This should no longer
  // happen when crbug.com/370457 is fixed.
  DeprecatedScheduleStyleRecalcDuringLayout marker(GetDocument().Lifecycle());
  GetDocument().ScheduleLayoutTreeUpdateIfNeeded();
}

void LayoutObject::HandleSubtreeModifications() {
  DCHECK(WasNotifiedOfSubtreeChange());
  DCHECK(GetDocument().Lifecycle().StateAllowsLayoutTreeNotifications());

  if (ConsumesSubtreeChangeNotification())
    SubtreeDidChange();

  bitfields_.SetNotifiedOfSubtreeChange(false);

  for (LayoutObject* object = SlowFirstChild(); object;
       object = object->NextSibling()) {
    if (!object->WasNotifiedOfSubtreeChange())
      continue;
    object->HandleSubtreeModifications();
  }
}

LayoutObject* LayoutObject::NextInPreOrder() const {
  if (LayoutObject* o = SlowFirstChild())
    return o;

  return NextInPreOrderAfterChildren();
}

bool LayoutObject::HasClipRelatedProperty() const {
  // TODO(trchen): Refactor / remove this function.
  // This function detects a bunch of properties that can potentially affect
  // clip inheritance chain. However such generalization is practially useless
  // because these properties change clip inheritance in different way that
  // needs to be handled explicitly.
  // CSS clip applies clip to the current element and all descendants.
  // CSS overflow clip applies only to containg-block descendants.
  // CSS contain:paint applies to all descendants by making itself a containing
  // block for all descendants.
  // CSS clip-path/mask/filter induces a stacking context and applies inherited
  // clip to that stacking context, while resetting clip for descendants. This
  // special behavior is already handled elsewhere.
  if (HasClip() || HasOverflowClip() || ShouldApplyPaintContainment())
    return true;
  if (IsBox() && ToLayoutBox(this)->HasControlClip())
    return true;
  return false;
}

bool LayoutObject::IsRenderedLegendInternal() const {
  DCHECK(IsBox());
  DCHECK(IsHTMLLegendElement());

  if (IsFloatingOrOutOfFlowPositioned())
    return false;
  const auto* parent = Parent();
  if (RuntimeEnabledFeatures::LayoutNGFieldsetEnabled()) {
    // If there is a rendered legend, it will be found inside the anonymous
    // fieldset wrapper.
    if (parent->IsAnonymous() && parent->Parent()->IsLayoutNGFieldset())
      parent = parent->Parent();
  }
  const auto* parent_layout_block = DynamicTo<LayoutBlock>(parent);
  return parent_layout_block && IsHTMLFieldSetElement(parent->GetNode()) &&
         LayoutFieldset::FindInFlowLegend(*parent_layout_block) == this;
}

LayoutObject* LayoutObject::NextInPreOrderAfterChildren() const {
  LayoutObject* o = NextSibling();
  if (!o) {
    o = Parent();
    while (o && !o->NextSibling())
      o = o->Parent();
    if (o)
      o = o->NextSibling();
  }

  return o;
}

LayoutObject* LayoutObject::NextInPreOrder(
    const LayoutObject* stay_within) const {
  if (LayoutObject* o = SlowFirstChild())
    return o;

  return NextInPreOrderAfterChildren(stay_within);
}

LayoutObject* LayoutObject::PreviousInPostOrder(
    const LayoutObject* stay_within) const {
  if (LayoutObject* o = SlowLastChild())
    return o;

  return PreviousInPostOrderBeforeChildren(stay_within);
}

LayoutObject* LayoutObject::NextInPreOrderAfterChildren(
    const LayoutObject* stay_within) const {
  if (this == stay_within)
    return nullptr;

  const LayoutObject* current = this;
  LayoutObject* next = current->NextSibling();
  for (; !next; next = current->NextSibling()) {
    current = current->Parent();
    if (!current || current == stay_within)
      return nullptr;
  }
  return next;
}

LayoutObject* LayoutObject::PreviousInPostOrderBeforeChildren(
    const LayoutObject* stay_within) const {
  if (this == stay_within)
    return nullptr;

  const LayoutObject* current = this;
  LayoutObject* previous = current->PreviousSibling();
  for (; !previous; previous = current->PreviousSibling()) {
    current = current->Parent();
    if (!current || current == stay_within)
      return nullptr;
  }
  return previous;
}

LayoutObject* LayoutObject::PreviousInPreOrder() const {
  if (LayoutObject* o = PreviousSibling()) {
    while (LayoutObject* last_child = o->SlowLastChild())
      o = last_child;
    return o;
  }

  return Parent();
}

LayoutObject* LayoutObject::PreviousInPreOrder(
    const LayoutObject* stay_within) const {
  if (this == stay_within)
    return nullptr;

  return PreviousInPreOrder();
}

LayoutObject* LayoutObject::LastLeafChild() const {
  LayoutObject* r = SlowLastChild();
  while (r) {
    LayoutObject* n = nullptr;
    n = r->SlowLastChild();
    if (!n)
      break;
    r = n;
  }
  return r;
}

static void AddLayers(LayoutObject* obj,
                      PaintLayer* parent_layer,
                      LayoutObject*& new_object,
                      PaintLayer*& before_child) {
  if (obj->HasLayer()) {
    if (!before_child && new_object) {
      // We need to figure out the layer that follows newObject. We only do
      // this the first time we find a child layer, and then we update the
      // pointer values for newObject and beforeChild used by everyone else.
      before_child =
          new_object->Parent()->FindNextLayer(parent_layer, new_object);
      new_object = nullptr;
    }
    parent_layer->AddChild(ToLayoutBoxModelObject(obj)->Layer(), before_child);
    return;
  }

  for (LayoutObject* curr = obj->SlowFirstChild(); curr;
       curr = curr->NextSibling())
    AddLayers(curr, parent_layer, new_object, before_child);
}

void LayoutObject::AddLayers(PaintLayer* parent_layer) {
  if (!parent_layer)
    return;

  LayoutObject* object = this;
  PaintLayer* before_child = nullptr;
  blink::AddLayers(this, parent_layer, object, before_child);
}

void LayoutObject::RemoveLayers(PaintLayer* parent_layer) {
  if (!parent_layer)
    return;

  if (HasLayer()) {
    parent_layer->RemoveChild(ToLayoutBoxModelObject(this)->Layer());
    return;
  }

  for (LayoutObject* curr = SlowFirstChild(); curr; curr = curr->NextSibling())
    curr->RemoveLayers(parent_layer);
}

void LayoutObject::MoveLayers(PaintLayer* old_parent, PaintLayer* new_parent) {
  if (!new_parent)
    return;

  if (HasLayer()) {
    PaintLayer* layer = ToLayoutBoxModelObject(this)->Layer();
    DCHECK_EQ(old_parent, layer->Parent());
    if (old_parent)
      old_parent->RemoveChild(layer);
    new_parent->AddChild(layer);
    return;
  }

  for (LayoutObject* curr = SlowFirstChild(); curr; curr = curr->NextSibling())
    curr->MoveLayers(old_parent, new_parent);
}

PaintLayer* LayoutObject::FindNextLayer(PaintLayer* parent_layer,
                                        LayoutObject* start_point,
                                        bool check_parent) {
  // Error check the parent layer passed in. If it's null, we can't find
  // anything.
  if (!parent_layer)
    return nullptr;

  // Step 1: If our layer is a child of the desired parent, then return our
  // layer.
  PaintLayer* our_layer =
      HasLayer() ? ToLayoutBoxModelObject(this)->Layer() : nullptr;
  if (our_layer && our_layer->Parent() == parent_layer)
    return our_layer;

  // Step 2: If we don't have a layer, or our layer is the desired parent, then
  // descend into our siblings trying to find the next layer whose parent is the
  // desired parent.
  if (!our_layer || our_layer == parent_layer) {
    for (LayoutObject* curr = start_point ? start_point->NextSibling()
                                          : SlowFirstChild();
         curr; curr = curr->NextSibling()) {
      PaintLayer* next_layer =
          curr->FindNextLayer(parent_layer, nullptr, false);
      if (next_layer)
        return next_layer;
    }
  }

  // Step 3: If our layer is the desired parent layer, then we're finished. We
  // didn't find anything.
  if (parent_layer == our_layer)
    return nullptr;

  // Step 4: If |checkParent| is set, climb up to our parent and check its
  // siblings that follow us to see if we can locate a layer.
  if (check_parent && Parent())
    return Parent()->FindNextLayer(parent_layer, this, true);

  return nullptr;
}

PaintLayer* LayoutObject::EnclosingLayer() const {
  for (const LayoutObject* current = this; current;
       current = current->Parent()) {
    if (current->HasLayer())
      return ToLayoutBoxModelObject(current)->Layer();
  }
  // TODO(crbug.com/365897): we should get rid of detached layout subtrees, at
  // which point this code should not be reached.
  return nullptr;
}

PaintLayer* LayoutObject::PaintingLayer() const {
  for (const LayoutObject* current = this; current;
       // Use containingBlock instead of parentCrossingFrames for floating
       // objects to omit any self-painting layers of inline objects that don't
       // paint the floating object.
       // This is only needed for inline-level floats not managed by LayoutNG.
       // LayoutNG floats are painted by the correct painting layer.
       current = (current->IsFloating() &&
                  !current->IsInLayoutNGInlineFormattingContext())
                     ? current->ContainingBlock()
                     : current->ParentCrossingFrames()) {
    if (current->HasLayer() &&
        ToLayoutBoxModelObject(current)->Layer()->IsSelfPaintingLayer()) {
      return ToLayoutBoxModelObject(current)->Layer();
    } else if (current->IsColumnSpanAll()) {
      // Column spanners paint through their multicolumn containers which can
      // be accessed through the associated out-of-flow placeholder's parent.
      current = current->SpannerPlaceholder();
    }
  }
  // TODO(crbug.com/365897): we should get rid of detached layout subtrees, at
  // which point this code should not be reached.
  return nullptr;
}

bool LayoutObject::IsFixedPositionObjectInPagedMedia() const {
  if (StyleRef().GetPosition() != EPosition::kFixed)
    return false;
  LayoutView* view = View();
  return Container() == view && view->PageLogicalHeight() &&
         // TODO(crbug.com/619094): Figure out the correct behaviour for fixed
         // position objects in paged media with vertical writing modes.
         view->IsHorizontalWritingMode();
}

LayoutRect LayoutObject::ScrollRectToVisible(
    const LayoutRect& rect,
    const WebScrollIntoViewParams& params) {
  LayoutBox* enclosing_box = EnclosingBox();
  if (!enclosing_box)
    return rect;

  GetDocument().GetFrame()->GetSmoothScrollSequencer().AbortAnimations();
  GetDocument().GetFrame()->GetSmoothScrollSequencer().SetScrollType(
      params.GetScrollType());
  WebScrollIntoViewParams new_params(params);
  new_params.is_for_scroll_sequence |=
      params.GetScrollType() == kProgrammaticScroll;
  LayoutRect new_location =
      enclosing_box->ScrollRectToVisibleRecursive(rect, new_params);
  GetDocument().GetFrame()->GetSmoothScrollSequencer().RunQueuedAnimations();

  return new_location;
}

LayoutBox* LayoutObject::EnclosingBox() const {
  LayoutObject* curr = const_cast<LayoutObject*>(this);
  while (curr) {
    if (curr->IsBox())
      return ToLayoutBox(curr);
    curr = curr->Parent();
  }

  NOTREACHED();
  return nullptr;
}

LayoutBlockFlow* LayoutObject::ContainingNGBlockFlow() const {
  DCHECK(IsInline());
  if (!RuntimeEnabledFeatures::LayoutNGEnabled())
    return nullptr;
  if (LayoutObject* parent = Parent()) {
    LayoutBox* box = parent->EnclosingBox();
    DCHECK(box);
    if (NGBlockNode::CanUseNewLayout(*box)) {
      DCHECK(box->IsLayoutBlockFlow());
      return To<LayoutBlockFlow>(box);
    }
  }
  return nullptr;
}

const NGPhysicalBoxFragment* LayoutObject::ContainingBlockFlowFragment() const {
  DCHECK(IsInline() || IsText());
  LayoutBlockFlow* const block_flow = ContainingNGBlockFlow();
  if (!block_flow || !block_flow->ChildrenInline())
    return nullptr;
  // TODO(kojii): CurrentFragment isn't always available after layout clean.
  // Investigate why.
  return block_flow->CurrentFragment();
}

LayoutBox* LayoutObject::EnclosingScrollableBox() const {
  for (LayoutObject* ancestor = Parent(); ancestor;
       ancestor = ancestor->Parent()) {
    if (!ancestor->IsBox())
      continue;

    LayoutBox* ancestor_box = ToLayoutBox(ancestor);
    if (ancestor_box->CanBeScrolledAndHasScrollableArea())
      return ancestor_box;
  }

  return nullptr;
}

LayoutFlowThread* LayoutObject::LocateFlowThreadContainingBlock() const {
  DCHECK(IsInsideFlowThread());

  // See if we have the thread cached because we're in the middle of layout.
  if (LayoutView* view = View()) {
    if (LayoutState* layout_state = view->GetLayoutState()) {
      // TODO(mstensho): We should really just return whatever
      // layoutState->flowThread() returns here, also if the value is nullptr.
      if (LayoutFlowThread* flow_thread = layout_state->FlowThread())
        return flow_thread;
    }
  }

  // Not in the middle of layout so have to find the thread the slow way.
  return LayoutFlowThread::LocateFlowThreadContainingBlockOf(
      *this, LayoutFlowThread::kAnyAncestor);
}

static inline bool ObjectIsRelayoutBoundary(const LayoutObject* object) {
  // FIXME: In future it may be possible to broaden these conditions in order to
  // improve performance.
  if (object->IsTextControl())
    return true;

  if (object->IsSVGRoot())
    return true;

  // LayoutInline can't be relayout roots since LayoutBlockFlow is responsible
  // for layouting them.
  if (object->IsLayoutInline())
    return false;

  // Table parts can't be relayout roots since the table is responsible for
  // layouting all the parts.
  if (object->IsTablePart())
    return false;

  const ComputedStyle* style = object->Style();
  if (object->ShouldApplyLayoutContainment() &&
      object->ShouldApplySizeContainment())
    return true;

  if (!object->HasOverflowClip())
    return false;

  // If either dimension is percent-based, intrinsic, or anything but fixed,
  // this object cannot form a re-layout boundary. A non-fixed computed logical
  // height will allow the object to grow and shrink based on the content
  // inside. The same goes for for logical width, if this objects is inside a
  // shrink-to-fit container, for instance.
  if (!style->Width().IsFixed() || !style->Height().IsFixed())
    return false;

  // Scrollbar parts can be removed during layout. Avoid the complexity of
  // having to deal with that.
  if (object->IsLayoutScrollbarPart())
    return false;

  if (const LayoutBox* layout_box = ToLayoutBoxOrNull(object)) {
    // In general we can't relayout a flex item independently of its container;
    // not only is the result incorrect due to the override size that's set, it
    // also messes with the cached main size on the flexbox.
    if (layout_box->IsFlexItemIncludingNG())
      return false;

    // In LayoutNG, if box has any OOF descendants, they are propagated to
    // parent. Therefore, we must mark parent chain for layout.
    if (layout_box->GetCachedLayoutResult() &&
        layout_box->GetCachedLayoutResult()
                ->PhysicalFragment()
                .OutOfFlowPositionedDescendants()
                .size() > 0)
      return false;
  }

  // Inside multicol it's generally problematic to allow relayout roots. The
  // multicol container itself may be scheduled for relayout as well (due to
  // other changes that may have happened since the previous layout pass),
  // which might affect the column heights, which may affect how this object
  // breaks across columns). Spanners may also have been added or removed since
  // the previous layout pass, which is just another way of affecting the column
  // heights (and the number of rows). Instead of identifying cases where it's
  // safe to allow relayout roots, just disallow them inside multicol.
  if (object->IsInsideFlowThread())
    return false;

  return true;
}

// Mark this object needing to re-run |CollectInlines()|.
//
// The flag is propagated to its container so that NGInlineNode that contains
// |this| is marked too. When |this| is a container, the propagation stops at
// |this|. When invalidating on inline blocks, floats, or OOF, caller need to
// pay attention whether it should mark its inner context or outer.
void LayoutObject::SetNeedsCollectInlines() {
  if (!RuntimeEnabledFeatures::LayoutNGEnabled())
    return;

  if (UNLIKELY(IsSVGChild()))
    return;

  LayoutObject* object = this;
  do {
    // Should not stop at |LayoutFlowThread| as |CollectInlines()| skips them.
    if (UNLIKELY(object->IsLayoutFlowThread())) {
      object = object->Parent();
      continue;
    }
    if (object->NeedsCollectInlines())
      break;
    object->SetNeedsCollectInlines(true);
    if (object->IsLayoutBlockFlow())
      break;
    object = object->Parent();
  } while (object);
}

void LayoutObject::MarkContainerChainForLayout(bool schedule_relayout,
                                               SubtreeLayoutScope* layouter) {
#if DCHECK_IS_ON()
  DCHECK(!IsSetNeedsLayoutForbidden());
#endif
  DCHECK(!layouter || this != layouter->Root());
  // When we're in layout, we're marking a descendant as needing layout with
  // the intention of visiting it during this layout. We shouldn't be
  // scheduling it to be laid out later. Also, scheduleRelayout() must not be
  // called while iterating LocalFrameView::layout_subtree_root_list_.
  schedule_relayout &= !GetFrameView()->IsInPerformLayout();

  LayoutObject* object = Container();
  LayoutObject* last = this;

  bool simplified_normal_flow_layout = NeedsSimplifiedNormalFlowLayout() &&
                                       !SelfNeedsLayout() &&
                                       !NormalChildNeedsLayout();

  while (object) {
    if (object->SelfNeedsLayout())
      return;

    // Note that if the last element we processed was blocked by a display lock,
    // and the reason we're propagating a change is that a subtree needed layout
    // (ie self doesn't need layout), then we can return and stop the dirty bit
    // propagation. Note that it's not enough to check |object|, since the
    // element that is actually locked needs its child bits set properly, we
    // need to go one more iteration after that.
    if (!last->SelfNeedsLayout() &&
        last->LayoutBlockedByDisplayLock(DisplayLockContext::kChildren)) {
      return;
    }

    // Don't mark the outermost object of an unrooted subtree. That object will
    // be marked when the subtree is added to the document.
    LayoutObject* container = object->Container();
    if (!container && !object->IsLayoutView())
      return;
    if (!last->IsTextOrSVGChild() && last->StyleRef().HasOutOfFlowPosition()) {
      object = last->ContainingBlock();
      if (object->PosChildNeedsLayout())
        return;
      container = object->Container();
      object->SetPosChildNeedsLayout(true);
      simplified_normal_flow_layout = true;
    } else if (simplified_normal_flow_layout) {
      if (object->NeedsSimplifiedNormalFlowLayout())
        return;
      object->SetNeedsSimplifiedNormalFlowLayout(true);
    } else {
      if (object->NormalChildNeedsLayout())
        return;
      object->SetNormalChildNeedsLayout(true);
    }
#if DCHECK_IS_ON()
    DCHECK(!object->IsSetNeedsLayoutForbidden());
#endif

    if (object->HasLayer() &&
        ToLayoutBoxModelObject(object)->Layer()->IsSelfPaintingLayer())
      ToLayoutBoxModelObject(object)->Layer()->SetNeedsVisualOverflowRecalc();

    if (layouter) {
      layouter->RecordObjectMarkedForLayout(object);

      if (object == layouter->Root()) {
        if (auto* painting_layer = PaintingLayer())
          painting_layer->SetNeedsVisualOverflowRecalc();

        return;
      }
    }

    last = object;
    if (schedule_relayout && ObjectIsRelayoutBoundary(last))
      break;
    object = container;
  }

  if (schedule_relayout)
    last->ScheduleRelayout();
}

// LayoutNG has different OOF-positioned handling compared to the existing
// layout system. To correctly determine the static-position of the object,
// LayoutNG "bubbles" up the static-position inside the NGLayoutResult.
// See: |NGLayoutResult::OutOfFlowPositionedDescendants()|.
//
// Whenever an OOF-positioned object is added/removed we need to invalidate
// layout for all the layout objects which may have stored a NGLayoutResult
// with this object contained in that list.
//
// In the future it may be possible to optimize this, e.g.
//  - For the removal case, add a pass which modifies the layout result to
//    remove the OOF-positioned descendant.
//  - For the adding case, if the OOF-positioned doesn't require a
//    static-position, simply insert the object up the NGLayoutResult chain with
//    an invalid static-position.
void LayoutObject::MarkParentForOutOfFlowPositionedChange() {
#if DCHECK_IS_ON()
  DCHECK(!IsSetNeedsLayoutForbidden());
#endif

  LayoutObject* object = Parent();
  if (!object)
    return;

  // As OOF-positioned objects are represented as an object replacement
  // character in the inline items list. We need to ensure we collect the
  // inline items again to either collect or drop the OOF-positioned object.
  object->SetNeedsCollectInlines();

  const LayoutBlock* containing_block = ContainingBlock();
  while (object != containing_block) {
    object->SetChildNeedsLayout(kMarkOnlyThis);
    object = object->Parent();
  }
  // Finally mark the parent block for layout. This will mark everything which
  // has an OOF-positioned object in a NGLayoutResult as needing layout.
  if (object)
    object->SetChildNeedsLayout();
}

#if DCHECK_IS_ON()
void LayoutObject::CheckBlockPositionedObjectsNeedLayout() {
  if (LayoutBlockedByDisplayLock(DisplayLockContext::kChildren))
    return;
  DCHECK(!NeedsLayout());

  auto* layout_block = DynamicTo<LayoutBlock>(this);
  if (layout_block)
    layout_block->CheckPositionedObjectsNeedLayout();
}
#endif

void LayoutObject::SetPreferredLogicalWidthsDirty(
    MarkingBehavior mark_parents) {
  bitfields_.SetPreferredLogicalWidthsDirty(true);
  if (mark_parents == kMarkContainerChain &&
      (IsText() || !StyleRef().HasOutOfFlowPosition()))
    InvalidateContainerPreferredLogicalWidths();
}

void LayoutObject::ClearPreferredLogicalWidthsDirty() {
  bitfields_.SetPreferredLogicalWidthsDirty(false);
}

static inline bool NGKeepInvalidatingBeyond(LayoutObject* o) {
  // Because LayoutNG does not work on individual inline objects, we can't
  // use a dirty width on an inline as a signal that it is safe to stop --
  // inlines never get marked as clean. Instead, we need to keep going to the
  // next block container.
  // Atomic inlines do not have this problem as they are treated like blocks
  // in this context.
  if (!RuntimeEnabledFeatures::LayoutNGEnabled())
    return false;
  if (o->IsLayoutInline() || o->IsText())
    return true;
  return false;
}

inline void LayoutObject::InvalidateContainerPreferredLogicalWidths() {
  // In order to avoid pathological behavior when inlines are deeply nested, we
  // do include them in the chain that we mark dirty (even though they're kind
  // of irrelevant).
  LayoutObject* o = IsTableCell() ? ContainingBlock() : Container();
  while (o &&
         (!o->PreferredLogicalWidthsDirty() || NGKeepInvalidatingBeyond(o))) {
    // Don't invalidate the outermost object of an unrooted subtree. That object
    // will be invalidated when the subtree is added to the document.
    LayoutObject* container =
        o->IsTableCell() ? o->ContainingBlock() : o->Container();
    if (!container && !o->IsLayoutView())
      break;

    o->bitfields_.SetPreferredLogicalWidthsDirty(true);
    // A positioned object has no effect on the min/max width of its containing
    // block ever. We can optimize this case and not go up any further.
    if (o->StyleRef().HasOutOfFlowPosition())
      break;
    o = container;
  }
}

LayoutObject* LayoutObject::ContainerForAbsolutePosition(
    AncestorSkipInfo* skip_info) const {
  return FindAncestorByPredicate(this, skip_info, [](LayoutObject* candidate) {
    if (!candidate->StyleRef().CanContainAbsolutePositionObjects() &&
        candidate->ShouldApplyLayoutContainment()) {
      UseCounter::Count(candidate->GetDocument(),
                        WebFeature::kCSSContainLayoutPositionedDescendants);
    }
    return candidate->CanContainAbsolutePositionObjects();
  });
}

LayoutObject* LayoutObject::ContainerForFixedPosition(
    AncestorSkipInfo* skip_info) const {
  DCHECK(!IsText());
  return FindAncestorByPredicate(this, skip_info, [](LayoutObject* candidate) {
    if (!candidate->StyleRef().CanContainFixedPositionObjects(
            candidate->IsDocumentElement()) &&
        candidate->ShouldApplyLayoutContainment()) {
      UseCounter::Count(candidate->GetDocument(),
                        WebFeature::kCSSContainLayoutPositionedDescendants);
    }
    return candidate->CanContainFixedPositionObjects();
  });
}

LayoutBlock* LayoutObject::ContainingBlockForAbsolutePosition(
    AncestorSkipInfo* skip_info) const {
  auto* container = ContainerForAbsolutePosition(skip_info);
  return FindContainingBlock(container, skip_info);
}

LayoutBlock* LayoutObject::ContainingBlockForFixedPosition(
    AncestorSkipInfo* skip_info) const {
  auto* container = ContainerForFixedPosition(skip_info);
  return FindContainingBlock(container, skip_info);
}

const LayoutBlock* LayoutObject::InclusiveContainingBlock() const {
  auto* layout_block = DynamicTo<LayoutBlock>(this);
  return layout_block ? layout_block : ContainingBlock();
}

LayoutBlock* LayoutObject::ContainingBlock(AncestorSkipInfo* skip_info) const {
  if (!IsTextOrSVGChild()) {
    if (style_->GetPosition() == EPosition::kFixed)
      return ContainingBlockForFixedPosition(skip_info);
    if (style_->GetPosition() == EPosition::kAbsolute)
      return ContainingBlockForAbsolutePosition(skip_info);
  }
  LayoutObject* object;
  if (IsColumnSpanAll()) {
    object = SpannerPlaceholder()->ContainingBlock();
  } else {
    object = Parent();
    if (!object && IsLayoutScrollbarPart())
      object = ToLayoutScrollbarPart(this)->GetScrollableArea()->GetLayoutBox();
    while (object && ((object->IsInline() && !object->IsAtomicInlineLevel()) ||
                      !object->IsLayoutBlock())) {
      if (skip_info)
        skip_info->Update(*object);
      object = object->Parent();
    }
  }

  return DynamicTo<LayoutBlock>(object);
}

bool LayoutObject::ComputeIsFixedContainer(const ComputedStyle* style) const {
  if (!style)
    return false;
  // https://www.w3.org/TR/filter-effects-1/#FilterProperty
  if (style->HasFilter() && !this->IsDocumentElement())
    return true;
  // The LayoutView is always a container of fixed positioned descendants. In
  // addition, SVG foreignObjects become such containers, so that descendants
  // of a foreignObject cannot escape it. Similarly, text controls let authors
  // select elements inside that are created by user agent shadow DOM, and we
  // have (C++) code that assumes that the elements are indeed contained by the
  // text control. So just make sure this is the case.
  if (this->IsLayoutView() || this->IsSVGForeignObject() ||
      this->IsTextControl())
    return true;
  // https://www.w3.org/TR/css-transforms-1/#containing-block-for-all-descendants
  if (style->HasTransformRelatedProperty()) {
    if (!this->IsInline() || this->IsAtomicInlineLevel())
      return true;
  }
  // https://www.w3.org/TR/css-contain-1/#containment-layout
  if (this->ShouldApplyPaintContainment(*style) ||
      this->ShouldApplyLayoutContainment(*style))
    return true;
  return false;
}

bool LayoutObject::ComputeIsAbsoluteContainer(
    const ComputedStyle* style) const {
  if (!style)
    return false;
  return style->CanContainAbsolutePositionObjects() ||
         ComputeIsFixedContainer(style);
}

FloatRect LayoutObject::AbsoluteBoundingBoxFloatRect(
    MapCoordinatesFlags flags) const {
  Vector<FloatQuad> quads;
  AbsoluteQuads(quads, flags);

  wtf_size_t n = quads.size();
  if (n == 0)
    return FloatRect();

  FloatRect result = quads[0].BoundingBox();
  for (wtf_size_t i = 1; i < n; ++i)
    result.Unite(quads[i].BoundingBox());
  return result;
}

IntRect LayoutObject::AbsoluteBoundingBoxRect(MapCoordinatesFlags flags) const {
  Vector<FloatQuad> quads;
  AbsoluteQuads(quads, flags);

  wtf_size_t n = quads.size();
  if (!n)
    return IntRect();

  IntRect result = quads[0].EnclosingBoundingBox();
  for (wtf_size_t i = 1; i < n; ++i)
    result.Unite(quads[i].EnclosingBoundingBox());
  return result;
}

IntRect LayoutObject::AbsoluteBoundingBoxRectIgnoringTransforms() const {
  FloatPoint abs_pos = LocalToAbsolute();
  Vector<IntRect> rects;
  AbsoluteRects(rects, FlooredLayoutPoint(abs_pos));

  wtf_size_t n = rects.size();
  if (!n)
    return IntRect();

  IntRect result = rects[0];
  for (wtf_size_t i = 1; i < n; ++i)
    result.Unite(rects[i]);
  return result;
}

LayoutRect LayoutObject::AbsoluteBoundingBoxRectHandlingEmptyInline() const {
  return EnclosingLayoutRect(AbsoluteBoundingBoxFloatRect());
}

LayoutRect LayoutObject::AbsoluteBoundingBoxRectForScrollIntoView() const {
  LayoutRect rect = AbsoluteBoundingBoxRectHandlingEmptyInline();
  const auto& style = StyleRef();
  rect.ExpandEdges(LayoutUnit(style.ScrollMarginTop()),
                   LayoutUnit(style.ScrollMarginRight()),
                   LayoutUnit(style.ScrollMarginBottom()),
                   LayoutUnit(style.ScrollMarginLeft()));
  return rect;
}

FloatRect LayoutObject::AbsoluteBoundingBoxRectForRange(
    const EphemeralRange& range) {
  if (range.IsNull() || !range.StartPosition().ComputeContainerNode())
    return FloatRect();

  range.GetDocument().UpdateStyleAndLayout();

  return ComputeTextFloatRect(range);
}

void LayoutObject::AddAbsoluteRectForLayer(IntRect& result) {
  if (HasLayer())
    result.Unite(AbsoluteBoundingBoxRect());
  for (LayoutObject* current = SlowFirstChild(); current;
       current = current->NextSibling())
    current->AddAbsoluteRectForLayer(result);
}

IntRect LayoutObject::AbsoluteBoundingBoxRectIncludingDescendants() const {
  IntRect result = AbsoluteBoundingBoxRect();
  for (LayoutObject* current = SlowFirstChild(); current;
       current = current->NextSibling())
    current->AddAbsoluteRectForLayer(result);
  return result;
}

void LayoutObject::Paint(const PaintInfo&) const {}

const LayoutBoxModelObject& LayoutObject::ContainerForPaintInvalidation()
    const {
  CHECK(IsRooted());

  if (const LayoutBoxModelObject* paint_invalidation_container =
          EnclosingCompositedContainer())
    return *paint_invalidation_container;

  // If the current frame is not composited, we send just return the main
  // frame's LayoutView so that we generate invalidations on the window.
  const LayoutView* layout_view = View();
  while (const LayoutObject* owner_object =
             layout_view->GetFrame()->OwnerLayoutObject())
    layout_view = owner_object->View();

  DCHECK(layout_view);
  return *layout_view;
}

bool LayoutObject::RecalcLayoutOverflow() {
  if (!ChildNeedsLayoutOverflowRecalc())
    return false;
  ClearChildNeedsLayoutOverflowRecalc();
  bool children_layout_overflow_changed = false;
  for (LayoutObject* current = SlowFirstChild(); current;
       current = current->NextSibling()) {
    if (current->RecalcLayoutOverflow())
      children_layout_overflow_changed = true;
  }
  return children_layout_overflow_changed;
}

void LayoutObject::RecalcVisualOverflow() {
  for (LayoutObject* current = SlowFirstChild(); current;
       current = current->NextSibling()) {
    if (current->HasLayer() &&
        ToLayoutBoxModelObject(current)->HasSelfPaintingLayer())
      continue;
    current->RecalcVisualOverflow();
  }
}

const LayoutBoxModelObject* LayoutObject::EnclosingCompositedContainer() const {
  LayoutBoxModelObject* container = nullptr;
  // FIXME: CompositingState is not necessarily up to date for many callers of
  // this function.
  DisableCompositingQueryAsserts disabler;

  if (PaintLayer* painting_layer = PaintingLayer()) {
    if (PaintLayer* compositing_layer =
            painting_layer
                ->EnclosingLayerForPaintInvalidationCrossingFrameBoundaries())
      container = &compositing_layer->GetLayoutObject();
  }
  return container;
}

bool LayoutObject::HasDistortingVisualEffects() const {
  // TODO(szager): Check occlusion information propagated from out-of-process
  // parent frame.
  PropertyTreeState paint_properties = EnclosingLayer()
                                           ->GetLayoutObject()
                                           .FirstFragment()
                                           .LocalBorderBoxProperties();

  // No filters, no blends, no opacity < 100%.
  for (const auto* effect = &paint_properties.Effect().Unalias(); effect;
       effect = SafeUnalias(effect->Parent())) {
    if (!effect->Filter().IsEmpty() || !effect->BackdropFilter().IsEmpty() ||
        effect->GetColorFilter() != kColorFilterNone ||
        effect->BlendMode() != SkBlendMode::kSrcOver ||
        effect->Opacity() != 1.0) {
      return true;
    }
  }

  PropertyTreeState root_properties = GetDocument()
                                          .GetFrame()
                                          ->LocalFrameRoot()
                                          .ContentLayoutObject()
                                          ->FirstFragment()
                                          .LocalBorderBoxProperties();

  // The only allowed transforms are 2D translation and proportional up-scaling.
  const auto& translation_2d_or_matrix =
      GeometryMapper::SourceToDestinationProjection(
          paint_properties.Transform(), root_properties.Transform());
  if (!translation_2d_or_matrix.IsIdentityOr2DTranslation() &&
      !translation_2d_or_matrix.Matrix()
           .Is2DProportionalUpscaleAndOr2DTranslation())
    return true;

  return false;
}

bool LayoutObject::HasNonZeroEffectiveOpacity() const {
  const FragmentData& fragment =
      EnclosingLayer()->GetLayoutObject().FirstFragment();

  // This can happen for an iframe element which is outside the viewport and has
  // therefore never been painted. In that case, we do the safe thing -- report
  // it as having non-zero opacity -- since this method is used by
  // IntersectionObserver to detect occlusion.
  if (!fragment.HasLocalBorderBoxProperties())
    return true;

  PropertyTreeState paint_properties = fragment.LocalBorderBoxProperties();

  for (const auto* effect = &paint_properties.Effect().Unalias(); effect;
       effect = SafeUnalias(effect->Parent())) {
    if (effect->Opacity() == 0.0)
      return false;
  }
  return true;
}

String LayoutObject::DecoratedName() const {
  StringBuilder name;
  name.Append(GetName());

  if (IsAnonymous())
    name.Append(" (anonymous)");
  // FIXME: Remove the special case for LayoutView here (requires rebaseline of
  // all tests).
  if (IsOutOfFlowPositioned() && !IsLayoutView())
    name.Append(" (positioned)");
  if (IsRelPositioned())
    name.Append(" (relative positioned)");
  if (IsStickyPositioned())
    name.Append(" (sticky positioned)");
  if (IsFloating())
    name.Append(" (floating)");
  if (SpannerPlaceholder())
    name.Append(" (column spanner)");

  return name.ToString();
}

String LayoutObject::DebugName() const {
  StringBuilder name;
  name.Append(DecoratedName());

  if (const Node* node = GetNode()) {
    name.Append(' ');
    name.Append(node->DebugName());
  }
  return name.ToString();
}

DOMNodeId LayoutObject::OwnerNodeId() const {
  return GetNode() ? DOMNodeIds::IdForNode(GetNode()) : kInvalidDOMNodeId;
}

IntRect LayoutObject::FragmentsVisualRectBoundingBox() const {
  if (!fragment_.NextFragment())
    return fragment_.VisualRect();
  IntRect visual_rect;
  for (auto* fragment = &fragment_; fragment;
       fragment = fragment->NextFragment())
    visual_rect.Unite(fragment->VisualRect());
  return visual_rect;
}

IntRect LayoutObject::VisualRect() const {
  return FragmentsVisualRectBoundingBox();
}

bool LayoutObject::IsPaintInvalidationContainer() const {
  return HasLayer() &&
         ToLayoutBoxModelObject(this)->Layer()->IsPaintInvalidationContainer();
}

void LayoutObject::InvalidateDisplayItemClients(
    PaintInvalidationReason reason) const {
  // This default implementation invalidates only the object itself as a
  // DisplayItemClient.
  ObjectPaintInvalidator(*this).InvalidateDisplayItemClient(*this, reason);
}

bool LayoutObject::CompositedScrollsWithRespectTo(
    const LayoutBoxModelObject& paint_invalidation_container) const {
  return paint_invalidation_container.UsesCompositedScrolling() &&
         this != &paint_invalidation_container;
}

void LayoutObject::InvalidatePaintRectangle(const LayoutRect& dirty_rect) {
  DCHECK_NE(GetDocument().Lifecycle().GetState(), DocumentLifecycle::kInPaint);

  if (dirty_rect.IsEmpty())
    return;

  fragment_.SetPartialInvalidationLocalRect(
      UnionRect(dirty_rect, fragment_.PartialInvalidationLocalRect()));

  // Not using the WithoutGeometryChange version because we need to map the
  // partial invalidated rect to visual rect in backing or the containing
  // transform node.
  SetShouldCheckForPaintInvalidation();
}

LayoutRect LayoutObject::AbsoluteSelectionRect() const {
  LayoutRect selection_rect = LocalSelectionVisualRect();
  if (!selection_rect.IsEmpty())
    MapToVisualRectInAncestorSpace(View(), selection_rect);

  if (LocalFrameView* frame_view = GetFrameView())
    selection_rect = frame_view->DocumentToFrame(selection_rect);

  return selection_rect;
}

DISABLE_CFI_PERF
void LayoutObject::InvalidatePaint(
    const PaintInvalidatorContext& context) const {
  ObjectPaintInvalidatorWithContext(*this, context).InvalidatePaint();
}

void LayoutObject::AdjustVisualRectForCompositedScrolling(
    IntRect& rect,
    const LayoutBoxModelObject& paint_invalidation_container) const {
  if (CompositedScrollsWithRespectTo(paint_invalidation_container)) {
    rect.Move(
        -ToLayoutBox(paint_invalidation_container).ScrolledContentOffset());
  }
}

IntRect LayoutObject::VisualRectIncludingCompositedScrolling(
    const LayoutBoxModelObject& paint_invalidation_container) const {
  IntRect rect = VisualRect();
  AdjustVisualRectForCompositedScrolling(rect, paint_invalidation_container);
  return rect;
}

void LayoutObject::ClearPreviousVisualRects() {
  DCHECK(!RuntimeEnabledFeatures::CompositeAfterPaintEnabled());

  for (auto* fragment = &fragment_; fragment;
       fragment = fragment->NextFragment()) {
    fragment->SetVisualRect(IntRect());
    fragment->SetSelectionVisualRect(IntRect());
  }

  // After clearing ("invalidating") the visual rects, mark this object as
  // needing to re-compute them.
  SetShouldDoFullPaintInvalidation();
}

LayoutRect LayoutObject::VisualRectInDocument(VisualRectFlags flags) const {
  LayoutRect rect = LocalVisualRect();
  MapToVisualRectInAncestorSpace(View(), rect, flags);
  return rect;
}

LayoutRect LayoutObject::LocalVisualRectIgnoringVisibility() const {
  NOTREACHED();
  return LayoutRect();
}

bool LayoutObject::MapToVisualRectInAncestorSpaceInternalFastPath(
    const LayoutBoxModelObject* ancestor,
    LayoutRect& rect,
    VisualRectFlags visual_rect_flags,
    bool& intersects) const {
  intersects = true;
  if (!(visual_rect_flags & kUseGeometryMapper) || !ancestor ||
      !ancestor->FirstFragment().HasLocalBorderBoxProperties())
    return false;

  if (ancestor == this)
    return true;

  const auto* property_container = this;
  AncestorSkipInfo skip_info(ancestor);
  while (!property_container->FirstFragment().HasLocalBorderBoxProperties()) {
    property_container = property_container->Container(&skip_info);
    if (!property_container || skip_info.AncestorSkipped())
      return false;
  }

  // This works because it's not possible to have any intervening clips,
  // effects, transforms between |this| and |property_container|, and therefore
  // FirstFragment().PaintOffset() is relative to the transform space defined by
  // FirstFragment().LocalBorderBoxProperties() (if this == property_container)
  // or property_container->FirstFragment().ContentsProperties().
  rect.MoveBy(FirstFragment().PaintOffset());
  if (property_container != ancestor) {
    FloatClipRect clip_rect((FloatRect(rect)));
    const auto& local_state =
        property_container == this
            ? FirstFragment().LocalBorderBoxProperties()
            : property_container->FirstFragment().ContentsProperties();
    intersects = GeometryMapper::LocalToAncestorVisualRect(
        local_state, ancestor->FirstFragment().ContentsProperties(), clip_rect,
        kIgnorePlatformOverlayScrollbarSize,
        (visual_rect_flags & kEdgeInclusive) ? kInclusiveIntersect
                                             : kNonInclusiveIntersect);
    rect = LayoutRect(clip_rect.Rect());
  }
  rect.MoveBy(-ancestor->FirstFragment().PaintOffset());

  return true;
}

bool LayoutObject::MapToVisualRectInAncestorSpace(
    const LayoutBoxModelObject* ancestor,
    LayoutRect& rect,
    VisualRectFlags visual_rect_flags) const {
  bool intersects = true;
  if (MapToVisualRectInAncestorSpaceInternalFastPath(
          ancestor, rect, visual_rect_flags, intersects))
    return intersects;

  TransformState transform_state(TransformState::kApplyTransformDirection,
                                 FloatQuad(FloatRect(rect)));
  intersects = MapToVisualRectInAncestorSpaceInternal(ancestor, transform_state,
                                                      visual_rect_flags);
  transform_state.Flatten();
  rect = LayoutRect(transform_state.LastPlanarQuad().BoundingBox());
  return intersects;
}

bool LayoutObject::MapToVisualRectInAncestorSpaceInternal(
    const LayoutBoxModelObject* ancestor,
    TransformState& transform_state,
    VisualRectFlags visual_rect_flags) const {
  // For any layout object that doesn't override this method (the main example
  // is LayoutText), the rect is assumed to be in the parent's coordinate space,
  // except for container flip.

  if (ancestor == this)
    return true;

  if (LayoutObject* parent = Parent()) {
    if (parent->IsBox()) {
      bool preserve3d = parent->StyleRef().Preserves3D() && !parent->IsText();
      TransformState::TransformAccumulation accumulation =
          preserve3d ? TransformState::kAccumulateTransform
                     : TransformState::kFlattenTransform;

      if (parent != ancestor &&
          !ToLayoutBox(parent)->MapContentsRectToBoxSpace(
              transform_state, accumulation, *this, visual_rect_flags))
        return false;
    }
    return parent->MapToVisualRectInAncestorSpaceInternal(
        ancestor, transform_state, visual_rect_flags);
  }
  return true;
}

HitTestResult LayoutObject::HitTestForOcclusion(
    const LayoutRect& hit_rect) const {
  LocalFrame* frame = GetDocument().GetFrame();
  DCHECK(!frame->View()->NeedsLayout());
  HitTestRequest::HitTestRequestType hit_type =
      HitTestRequest::kIgnorePointerEventsNone | HitTestRequest::kReadOnly |
      HitTestRequest::kIgnoreClipping |
      HitTestRequest::kIgnoreZeroOpacityObjects |
      HitTestRequest::kHitTestVisualOverflow;
  HitTestLocation location(hit_rect);
  return frame->GetEventHandler().HitTestResultAtLocation(location, hit_type,
                                                          this, true);
}

void LayoutObject::DirtyLinesFromChangedChild(LayoutObject*, MarkingBehavior) {}

std::ostream& operator<<(std::ostream& out, const LayoutObject& object) {
  StringBuilder string_builder;
  object.DumpLayoutObject(string_builder, false, 0);
  return out << static_cast<const void*>(&object) << ":"
             << string_builder.ToString().Utf8().data();
}

std::ostream& operator<<(std::ostream& out, const LayoutObject* object) {
  if (!object)
    return out << "<null>";
  return out << *object;
}

#ifndef NDEBUG

void LayoutObject::ShowTreeForThis() const {
  if (GetNode())
    ::showTree(GetNode());
}

void LayoutObject::ShowLayoutTreeForThis() const {
  showLayoutTree(this, nullptr);
}

void LayoutObject::ShowLineTreeForThis() const {
  if (const LayoutBlock* cb = InclusiveContainingBlock()) {
    auto* child_block_flow = DynamicTo<LayoutBlockFlow>(cb);
    if (child_block_flow) {
      child_block_flow->ShowLineTreeAndMark(nullptr, nullptr, nullptr, nullptr,
                                            this);
    }
  }
}

void LayoutObject::ShowLayoutObject() const {
  StringBuilder string_builder;
  DumpLayoutObject(string_builder, true, kShowTreeCharacterOffset);
  DLOG(INFO) << "\n" << string_builder.ToString().Utf8().data();
}

#endif  // NDEBUG

void LayoutObject::DumpLayoutObject(StringBuilder& string_builder,
                                    bool dump_address,
                                    unsigned show_tree_character_offset) const {
  string_builder.Append(DecoratedName());

  if (dump_address)
    string_builder.AppendFormat(" %p", this);

  if (IsText() && ToLayoutText(this)->IsTextFragment()) {
    string_builder.AppendFormat(" \"%s\" ",
                                ToLayoutText(this)->GetText().Ascii().data());
  }

  if (VirtualContinuation())
    string_builder.AppendFormat(" continuation=%p", VirtualContinuation());

  if (GetNode()) {
    while (string_builder.length() < show_tree_character_offset)
      string_builder.Append(' ');
    string_builder.Append('\t');
    string_builder.Append(GetNode()->ToString());
  }
}

#ifndef NDEBUG

void LayoutObject::DumpLayoutTreeAndMark(StringBuilder& string_builder,
                                         const LayoutObject* marked_object1,
                                         const char* marked_label1,
                                         const LayoutObject* marked_object2,
                                         const char* marked_label2,
                                         unsigned depth) const {
  StringBuilder object_info;
  if (marked_object1 == this && marked_label1)
    object_info.Append(marked_label1);
  if (marked_object2 == this && marked_label2)
    object_info.Append(marked_label2);
  while (object_info.length() < depth * 2)
    object_info.Append(' ');

  DumpLayoutObject(object_info, true, kShowTreeCharacterOffset);
  string_builder.Append(object_info);

  for (const LayoutObject* child = SlowFirstChild(); child;
       child = child->NextSibling()) {
    string_builder.Append('\n');
    child->DumpLayoutTreeAndMark(string_builder, marked_object1, marked_label1,
                                 marked_object2, marked_label2, depth + 1);
  }
}

#endif  // NDEBUG

bool LayoutObject::IsSelected() const {
  // Keep this fast and small, used in very hot functions to skip computing
  // selection when this is not selected. This function may be inlined in
  // link-optimized builds, but keeping fast and small helps running perf
  // tests.
  return GetSelectionState() != SelectionState::kNone ||
         // TODO(kojii): Can't we set SelectionState() properly to
         // LayoutTextFragment too?
         (IsText() && ToLayoutText(*this).IsTextFragment() &&
          LayoutSelection::IsSelected(*this));
}

bool LayoutObject::IsSelectable() const {
  return !IsInert() && !(StyleRef().UserSelect() == EUserSelect::kNone &&
                         StyleRef().UserModify() == EUserModify::kReadOnly);
}

// Called when an object that was floating or positioned becomes a normal flow
// object again. We have to make sure the layout tree updates as needed to
// accommodate the new normal flow object.
static inline void HandleDynamicFloatPositionChange(LayoutObject* object) {
  // We have gone from not affecting the inline status of the parent flow to
  // suddenly having an impact.  See if there is a mismatch between the parent
  // flow's childrenInline() state and our state.
  object->SetInline(object->StyleRef().IsDisplayInlineType());
  if (object->IsInline() != object->Parent()->ChildrenInline()) {
    if (!object->IsInline()) {
      ToLayoutBoxModelObject(object->Parent())->ChildBecameNonInline(object);
    } else {
      // An anonymous block must be made to wrap this inline.
      LayoutBlock* block =
          To<LayoutBlock>(object->Parent())->CreateAnonymousBlock();
      LayoutObjectChildList* childlist = object->Parent()->VirtualChildren();
      childlist->InsertChildNode(object->Parent(), block, object);
      block->Children()->AppendChildNode(
          block, childlist->RemoveChildNode(object->Parent(), object));
    }
  }
}

StyleDifference LayoutObject::AdjustStyleDifference(
    StyleDifference diff) const {
  if (diff.TransformChanged() && IsSVG()) {
    // Skip a full layout for transforms at the html/svg boundary which do not
    // affect sizes inside SVG.
    if (!IsSVGRoot())
      diff.SetNeedsFullLayout();
  }

  // TODO(wangxianzhu): We may avoid subtree paint invalidation on CSS clip
  // change for CAP.
  if (diff.CssClipChanged())
    diff.SetNeedsPaintInvalidationSubtree();

  // Optimization: for decoration/color property changes, invalidation is only
  // needed if we have style or text affected by these properties.
  if (diff.TextDecorationOrColorChanged() &&
      !diff.NeedsFullPaintInvalidation()) {
    if (StyleRef().HasBorderColorReferencingCurrentColor() ||
        StyleRef().HasOutlineWithCurrentColor() ||
        StyleRef().HasBackgroundRelatedColorReferencingCurrentColor() ||
        // Skip any text nodes that do not contain text boxes. Whitespace cannot
        // be skipped or we will miss invalidating decorations (e.g.,
        // underlines).
        (IsText() && !IsBR() && ToLayoutText(this)->HasTextBoxes()) ||
        (IsSVG() && StyleRef().SvgStyle().IsFillColorCurrentColor()) ||
        (IsSVG() && StyleRef().SvgStyle().IsStrokeColorCurrentColor()) ||
        IsListMarker() || IsDetailsMarker())
      diff.SetNeedsPaintInvalidationObject();
  }

  // The answer to layerTypeRequired() for plugins, iframes, and canvas can
  // change without the actual style changing, since it depends on whether we
  // decide to composite these elements. When the/ layer status of one of these
  // elements changes, we need to force a layout.
  if (!diff.NeedsFullLayout() && Style() && IsBoxModelObject()) {
    bool requires_layer =
        ToLayoutBoxModelObject(this)->LayerTypeRequired() != kNoPaintLayer;
    if (HasLayer() != requires_layer)
      diff.SetNeedsFullLayout();
  }

  return diff;
}

void LayoutObject::SetPseudoStyle(scoped_refptr<ComputedStyle> pseudo_style) {
  DCHECK(pseudo_style->StyleType() == kPseudoIdBefore ||
         pseudo_style->StyleType() == kPseudoIdAfter ||
         pseudo_style->StyleType() == kPseudoIdFirstLetter);

  // FIXME: We should consider just making all pseudo items use an inherited
  // style.

  // Images are special and must inherit the pseudoStyle so the width and height
  // of the pseudo element doesn't change the size of the image. In all other
  // cases we can just share the style.
  //
  // Quotes are also LayoutInline, so we need to create an inherited style to
  // avoid getting an inline with positioning or an invalid display.
  //
  if (IsImage() || IsQuote()) {
    scoped_refptr<ComputedStyle> style = ComputedStyle::Create();
    style->InheritFrom(*pseudo_style);
    SetStyle(std::move(style));
    return;
  }

  SetStyle(std::move(pseudo_style));
}

void LayoutObject::MarkContainerChainForOverflowRecalcIfNeeded() {
  LayoutObject* object = this;
  do {
    // Cell and row need to propagate the flag to their containing section and
    // row as their containing block is the table wrapper.
    // This enables us to only recompute overflow the modified sections / rows.
    object = object->IsTableCell() || object->IsTableRow()
                 ? object->Parent()
                 : object->Container();
    if (object) {
      object->SetChildNeedsLayoutOverflowRecalc();

      if (object->HasLayer()) {
        auto* box_model_object = ToLayoutBoxModelObject(object);
        if (box_model_object->HasSelfPaintingLayer())
          box_model_object->Layer()->SetNeedsVisualOverflowRecalc();
      }
    }

  } while (object);
}

void LayoutObject::SetNeedsOverflowRecalc() {
  bool needed_recalc = SelfNeedsLayoutOverflowRecalc();
  SetSelfNeedsLayoutOverflowRecalc();
  SetShouldCheckForPaintInvalidation();

  if (HasLayer()) {
    auto* box_model_object = ToLayoutBoxModelObject(this);
    if (box_model_object->HasSelfPaintingLayer())
      box_model_object->Layer()->SetNeedsVisualOverflowRecalc();
  }
  if (!needed_recalc)
    MarkContainerChainForOverflowRecalcIfNeeded();
}

DISABLE_CFI_PERF
void LayoutObject::SetStyle(scoped_refptr<ComputedStyle> style,
                            ApplyStyleChanges apply_changes) {
  if (style_ == style)
    return;

  if (apply_changes == ApplyStyleChanges::kNo) {
    SetStyleInternal(std::move(style));
    return;
  }

  DCHECK(style);

  StyleDifference diff;
  if (style_)
    diff = style_->VisualInvalidationDiff(GetDocument(), *style);

  diff = AdjustStyleDifference(diff);

  StyleWillChange(diff, *style);

  scoped_refptr<ComputedStyle> old_style = std::move(style_);
  SetStyleInternal(std::move(style));

  if (!IsText())
    UpdateImageObservers(old_style.get(), style_.get());

  CheckCounterChanges(old_style.get(), style_.get());

  bool does_not_need_layout_or_paint_invalidation = !parent_;

  StyleDidChange(diff, old_style.get());

  // FIXME: |this| might be destroyed here. This can currently happen for a
  // LayoutTextFragment when its first-letter block gets an update in
  // LayoutTextFragment::styleDidChange. For LayoutTextFragment(s),
  // we will safely bail out with the doesNotNeedLayoutOrPaintInvalidation flag.
  // We might want to broaden this condition in the future as we move
  // layoutObject changes out of layout and into style changes.
  if (does_not_need_layout_or_paint_invalidation)
    return;

  // Now that the layer (if any) has been updated, we need to adjust the diff
  // again, check whether we should layout now, and decide if we need to
  // invalidate paints.
  StyleDifference updated_diff = AdjustStyleDifference(diff);

  if (!diff.NeedsFullLayout()) {
    if (updated_diff.NeedsFullLayout()) {
      SetNeedsLayoutAndPrefWidthsRecalc(
          layout_invalidation_reason::kStyleChange);
    } else if (updated_diff.NeedsPositionedMovementLayout()) {
      SetNeedsPositionedMovementLayout();
    }
  }

  if (diff.TransformChanged() && !NeedsLayout()) {
    if (LayoutBlock* container = ContainingBlock())
      container->SetNeedsOverflowRecalc();
  }

  if (diff.NeedsRecomputeOverflow() && !NeedsLayout()) {
    // TODO(rhogan): Make inlines capable of recomputing overflow too.
    if (IsLayoutBlock()) {
      SetNeedsOverflowRecalc();
    } else {
      SetNeedsLayoutAndPrefWidthsRecalc(
          layout_invalidation_reason::kStyleChange);
    }
  }

  if (diff.NeedsPaintInvalidationSubtree() ||
      updated_diff.NeedsPaintInvalidationSubtree()) {
    SetSubtreeShouldDoFullPaintInvalidation();
  } else if (diff.NeedsPaintInvalidationObject() ||
             updated_diff.NeedsPaintInvalidationObject()) {
    // TODO(wangxianzhu): For now LayoutSVGRoot::localVisualRect() depends on
    // several styles. Refactor to avoid this special case.
    if (IsSVGRoot())
      SetShouldDoFullPaintInvalidation();
    else
      SetShouldDoFullPaintInvalidationWithoutGeometryChange();
  }

  if ((diff.NeedsPaintInvalidationObject() ||
       diff.NeedsPaintInvalidationSubtree()) &&
      old_style && !old_style->ClipPathDataEquivalent(*style_))
    InvalidateClipPathCache();

  if (diff.NeedsVisualRectUpdate())
    SetShouldCheckForPaintInvalidation();

  // Text nodes share style with their parents but the paint properties don't
  // apply to them, hence the !isText() check. If property nodes are added or
  // removed as a result of these style changes, PaintPropertyTreeBuilder will
  // call SetNeedsRepaint to cause re-generation of PaintChunks.
  if (!IsText() && !HasLayer() &&
      (diff.TransformChanged() || diff.OpacityChanged() ||
       diff.ZIndexChanged() || diff.FilterChanged() ||
       diff.BackdropFilterChanged() || diff.CssClipChanged() ||
       diff.BlendModeChanged() || diff.MaskChanged())) {
    SetNeedsPaintPropertyUpdate();
  }
}

void LayoutObject::UpdateImageObservers(const ComputedStyle* old_style,
                                        const ComputedStyle* new_style) {
  DCHECK(old_style || new_style);
  DCHECK(!IsText());

  UpdateFillImages(old_style ? &old_style->BackgroundLayers() : nullptr,
                   new_style ? &new_style->BackgroundLayers() : nullptr);
  UpdateFillImages(old_style ? &old_style->MaskLayers() : nullptr,
                   new_style ? &new_style->MaskLayers() : nullptr);

  UpdateImage(old_style ? old_style->BorderImage().GetImage() : nullptr,
              new_style ? new_style->BorderImage().GetImage() : nullptr);
  UpdateImage(old_style ? old_style->MaskBoxImage().GetImage() : nullptr,
              new_style ? new_style->MaskBoxImage().GetImage() : nullptr);

  StyleImage* old_content_image =
      old_style && old_style->GetContentData() &&
              old_style->GetContentData()->IsImage()
          ? To<ImageContentData>(old_style->GetContentData())->GetImage()
          : nullptr;
  StyleImage* new_content_image =
      new_style && new_style->GetContentData() &&
              new_style->GetContentData()->IsImage()
          ? To<ImageContentData>(new_style->GetContentData())->GetImage()
          : nullptr;
  UpdateImage(old_content_image, new_content_image);

  StyleImage* old_box_reflect_mask_image =
      old_style && old_style->BoxReflect()
          ? old_style->BoxReflect()->Mask().GetImage()
          : nullptr;
  StyleImage* new_box_reflect_mask_image =
      new_style && new_style->BoxReflect()
          ? new_style->BoxReflect()->Mask().GetImage()
          : nullptr;
  UpdateImage(old_box_reflect_mask_image, new_box_reflect_mask_image);

  UpdateShapeImage(old_style ? old_style->ShapeOutside() : nullptr,
                   new_style ? new_style->ShapeOutside() : nullptr);
  UpdateCursorImages(old_style ? old_style->Cursors() : nullptr,
                     new_style ? new_style->Cursors() : nullptr);

  UpdateFirstLineImageObservers(old_style, new_style);
}

void LayoutObject::UpdateFirstLineImageObservers(
    const ComputedStyle* old_style,
    const ComputedStyle* new_style) {
  bool has_old_first_line_style =
      old_style && old_style->HasPseudoStyle(kPseudoIdFirstLine);
  bool has_new_first_line_style =
      new_style && new_style->HasPseudoStyle(kPseudoIdFirstLine);
  if (!has_old_first_line_style && !has_new_first_line_style)
    return;

  // Don't call CacheFirstLineStyle() which will update the cache, because this
  // function can be called when the object has not been inserted into the tree
  // and we can't update the pseudo style cache which may depend on ancestors.
  const auto* cached_old_first_line_style =
      has_old_first_line_style
          ? old_style->GetCachedPseudoStyle(kPseudoIdFirstLine)
          : nullptr;
  const auto* cached_new_first_line_style =
      has_new_first_line_style
          ? new_style->GetCachedPseudoStyle(kPseudoIdFirstLine)
          : nullptr;

  if (has_new_first_line_style) {
    // If cached_new_first_line_style is null, it means that the new first line
    // style has not been cached yet. Will check again when the object's first
    // line style is actually used and cached.
    bitfields_.SetPendingUpdateFirstLineImageObservers(
        !cached_new_first_line_style);
  }

  if (cached_old_first_line_style || cached_new_first_line_style) {
    UpdateFillImages(cached_old_first_line_style
                         ? &cached_old_first_line_style->BackgroundLayers()
                         : nullptr,
                     cached_new_first_line_style
                         ? &cached_new_first_line_style->BackgroundLayers()
                         : nullptr);
  }
}

void LayoutObject::StyleWillChange(StyleDifference diff,
                                   const ComputedStyle& new_style) {
  if (style_) {
    bool visibility_changed = style_->Visibility() != new_style.Visibility();
    // If our z-index changes value or our visibility changes,
    // we need to dirty our stacking context's z-order list.
    if (visibility_changed || style_->ZIndex() != new_style.ZIndex() ||
        style_->IsStackingContext() != new_style.IsStackingContext()) {
      GetDocument().SetAnnotatedRegionsDirty(true);
      if (AXObjectCache* cache = GetDocument().ExistingAXObjectCache()) {
        if (GetNode())
          cache->ChildrenChanged(GetNode()->parentNode());
        else
          cache->ChildrenChanged(Parent());
      }
    }

    if (diff.TextDecorationOrColorChanged() ||
        style_->InsideLink() != new_style.InsideLink()) {
      if (AXObjectCache* cache = GetDocument().ExistingAXObjectCache())
        cache->TextChanged(this);
    }

    if (diff.TransformChanged()) {
      if (AXObjectCache* cache = GetDocument().ExistingAXObjectCache())
        cache->LocationChanged(this);
    }

    // Keep layer hierarchy visibility bits up to date if visibility changes.
    if (visibility_changed) {
      // We might not have an enclosing layer yet because we might not be in the
      // tree.
      if (PaintLayer* layer = EnclosingLayer())
        layer->DirtyVisibleContentStatus();
    }

    if (IsFloating() && (style_->Floating() != new_style.Floating())) {
      // For changes in float styles, we need to conceivably remove ourselves
      // from the floating objects list.
      ToLayoutBox(this)->RemoveFloatingOrPositionedChildFromBlockLists();
    } else if (IsOutOfFlowPositioned() &&
               (style_->GetPosition() != new_style.GetPosition())) {
      // For changes in positioning styles, we need to conceivably remove
      // ourselves from the positioned objects list.
      ToLayoutBox(this)->RemoveFloatingOrPositionedChildFromBlockLists();
    }

    affects_parent_block_ =
        IsFloatingOrOutOfFlowPositioned() &&
        (!new_style.IsFloating() && !new_style.HasOutOfFlowPosition()) &&
        Parent() &&
        (Parent()->IsLayoutBlockFlow() || Parent()->IsLayoutInline());

    // Clearing these bits is required to avoid leaving stale layoutObjects.
    // FIXME: We shouldn't need that hack if our logic was totally correct.
    if (diff.NeedsLayout()) {
      SetFloating(false);
      ClearPositionedState();
    }
  } else {
    affects_parent_block_ = false;
  }

  // Elements with non-auto touch-action will send a SetTouchAction message
  // on touchstart in EventHandler::handleTouchEvent, and so effectively have
  // a touchstart handler that must be reported.
  //
  // Since a CSS property cannot be applied directly to a text node, a
  // handler will have already been added for its parent so ignore it.
  //
  // Elements may inherit touch action from parent frame, so we need to report
  // touchstart handler if the root layout object has non-auto effective touch
  // action.
  TouchAction old_touch_action = TouchAction::kTouchActionAuto;
  bool is_document_element = GetNode() && IsDocumentElement();
  if (style_) {
    old_touch_action = is_document_element ? style_->GetEffectiveTouchAction()
                                           : style_->GetTouchAction();
  }
  TouchAction new_touch_action = is_document_element
                                     ? new_style.GetEffectiveTouchAction()
                                     : new_style.GetTouchAction();
  if (GetNode() && !GetNode()->IsTextNode() &&
      (old_touch_action == TouchAction::kTouchActionAuto) !=
          (new_touch_action == TouchAction::kTouchActionAuto)) {
    EventHandlerRegistry& registry =
        GetDocument().GetFrame()->GetEventHandlerRegistry();
    if (new_touch_action != TouchAction::kTouchActionAuto) {
      registry.DidAddEventHandler(*GetNode(),
                                  EventHandlerRegistry::kTouchAction);
    } else {
      registry.DidRemoveEventHandler(*GetNode(),
                                     EventHandlerRegistry::kTouchAction);
    }
    MarkEffectiveAllowedTouchActionChanged();
  }
}

void LayoutObject::ClearBaseComputedStyle() {
  if (!GetNode())
    return;
  if (!GetNode()->IsElementNode())
    return;
  if (ElementAnimations* animations =
          ToElement(GetNode())->GetElementAnimations())
    animations->ClearBaseComputedStyle();
}

static bool AreNonIdenticalCursorListsEqual(const ComputedStyle* a,
                                            const ComputedStyle* b) {
  DCHECK_NE(a->Cursors(), b->Cursors());
  return a->Cursors() && b->Cursors() && *a->Cursors() == *b->Cursors();
}

static inline bool AreCursorsEqual(const ComputedStyle* a,
                                   const ComputedStyle* b) {
  return a->Cursor() == b->Cursor() && (a->Cursors() == b->Cursors() ||
                                        AreNonIdenticalCursorListsEqual(a, b));
}

void LayoutObject::SetScrollAnchorDisablingStyleChangedOnAncestor() {
  // Walk up the parent chain and find the first scrolling block to disable
  // scroll anchoring on.
  LayoutObject* object = Parent();
  Element* viewport_defining_element = GetDocument().ViewportDefiningElement();
  while (object) {
    auto* block = DynamicTo<LayoutBlock>(object);
    if (block && (block->HasOverflowClip() ||
                  block->GetNode() == viewport_defining_element)) {
      block->SetScrollAnchorDisablingStyleChanged(true);
      return;
    }
    object = object->Parent();
  }
}

void LayoutObject::StyleDidChange(StyleDifference diff,
                                  const ComputedStyle* old_style) {
  // First assume the outline will be affected. It may be updated when we know
  // it's not affected.
  bool has_outline = style_->HasOutline();
  SetOutlineMayBeAffectedByDescendants(has_outline);
  if (!has_outline)
    SetPreviousOutlineMayBeAffectedByDescendants(false);

  if (affects_parent_block_)
    HandleDynamicFloatPositionChange(this);

  if (diff.NeedsFullLayout()) {
    // If the in-flow state of an element is changed, disable scroll
    // anchoring on the containing scroller.
    if (old_style->HasOutOfFlowPosition() != style_->HasOutOfFlowPosition()) {
      SetScrollAnchorDisablingStyleChangedOnAncestor();
      if (RuntimeEnabledFeatures::LayoutNGEnabled())
        MarkParentForOutOfFlowPositionedChange();
    }

    // If the object already needs layout, then setNeedsLayout won't do
    // any work. But if the containing block has changed, then we may need
    // to mark the new containing blocks for layout. The change that can
    // directly affect the containing block of this object is a change to
    // the position style.
    if (NeedsLayout() && old_style->GetPosition() != style_->GetPosition()) {
      MarkContainerChainForLayout();
    }

    SetNeedsLayoutAndPrefWidthsRecalc(layout_invalidation_reason::kStyleChange);
  } else if (diff.NeedsPositionedMovementLayout()) {
    SetNeedsPositionedMovementLayout();
  }

  if (diff.ScrollAnchorDisablingPropertyChanged())
    SetScrollAnchorDisablingStyleChanged(true);

  // Don't check for paint invalidation here; we need to wait until the layer
  // has been updated by subclasses before we know if we have to invalidate
  // paints (in setStyle()).

  if (old_style && !AreCursorsEqual(old_style, Style())) {
    if (LocalFrame* frame = GetFrame()) {
      // Cursor update scheduling is done by the local root, which is the main
      // frame if there are no RemoteFrame ancestors in the frame tree. Use of
      // localFrameRoot() is discouraged but will change when cursor update
      // scheduling is moved from EventHandler to PageEventHandler.
      frame->LocalFrameRoot().GetEventHandler().ScheduleCursorUpdate();
    }
  }

  if (diff.NeedsFullPaintInvalidation() && old_style) {
    if (ResolveColor(*old_style, GetCSSPropertyBackgroundColor()) !=
            ResolveColor(GetCSSPropertyBackgroundColor()) ||
        old_style->BackgroundLayers() != StyleRef().BackgroundLayers())
      SetBackgroundNeedsFullPaintInvalidation();
  }

  ApplyPseudoStyleChanges(old_style);

  if (old_style &&
      old_style->UsedTransformStyle3D() != StyleRef().UsedTransformStyle3D()) {
    // Change of transform-style may affect descendant transform property nodes.
    AddSubtreePaintPropertyUpdateReason(
        SubtreePaintPropertyUpdateReason::kTransformStyleChanged);
  }
}

void LayoutObject::ApplyPseudoStyleChanges(const ComputedStyle* old_style) {
  ApplyFirstLineChanges(old_style);

  if ((old_style && old_style->HasPseudoStyle(kPseudoIdSelection)) ||
      StyleRef().HasPseudoStyle(kPseudoIdSelection))
    InvalidateSelectedChildrenOnStyleChange();
}

void LayoutObject::ApplyFirstLineChanges(const ComputedStyle* old_style) {
  bool has_old_first_line_style =
      old_style && old_style->HasPseudoStyle(kPseudoIdFirstLine);
  bool has_new_first_line_style = StyleRef().HasPseudoStyle(kPseudoIdFirstLine);
  if (!has_old_first_line_style && !has_new_first_line_style)
    return;

  StyleDifference diff;
  bool has_diff = false;
  if (Parent() && has_old_first_line_style && has_new_first_line_style) {
    if (const auto* old_first_line_style =
            old_style->GetCachedPseudoStyle(kPseudoIdFirstLine)) {
      if (auto new_first_line_style = UncachedFirstLineStyle()) {
        diff = old_first_line_style->VisualInvalidationDiff(
            GetDocument(), *new_first_line_style);
        has_diff = true;
      }
    }
  }
  if (!has_diff) {
    diff.SetNeedsPaintInvalidationObject();
    diff.SetNeedsFullLayout();
  }

  if (BehavesLikeBlockContainer() && (diff.NeedsFullPaintInvalidation() ||
                                      diff.TextDecorationOrColorChanged())) {
    if (auto* first_line_container =
            To<LayoutBlock>(this)->NearestInnerBlockWithFirstLine())
      first_line_container->SetShouldDoFullPaintInvalidationForFirstLine();
  }

  if (diff.NeedsLayout()) {
    if (diff.NeedsFullLayout())
      SetNeedsCollectInlines();
    SetNeedsLayoutAndPrefWidthsRecalc(layout_invalidation_reason::kStyleChange);
  }
}

void LayoutObject::PropagateStyleToAnonymousChildren() {
  // FIXME: We could save this call when the change only affected non-inherited
  // properties.
  for (LayoutObject* child = SlowFirstChild(); child;
       child = child->NextSibling()) {
    if (!child->IsAnonymous() || child->StyleRef().StyleType() != kPseudoIdNone)
      continue;
    if (child->AnonymousHasStylePropagationOverride())
      continue;

    scoped_refptr<ComputedStyle> new_style =
        ComputedStyle::CreateAnonymousStyleWithDisplay(
            StyleRef(), child->StyleRef().Display());

    // Preserve the position style of anonymous block continuations as they can
    // have relative position when they contain block descendants of relative
    // positioned inlines.
    auto* child_block_flow = DynamicTo<LayoutBlockFlow>(child);
    if (child->IsInFlowPositioned() && child_block_flow &&
        child_block_flow->IsAnonymousBlockContinuation())
      new_style->SetPosition(child->StyleRef().GetPosition());

    if (child->IsLayoutNGListMarker())
      new_style->SetWhiteSpace(child->StyleRef().WhiteSpace());

    UpdateAnonymousChildStyle(child, *new_style);

    child->SetStyle(std::move(new_style));
  }

  if (StyleRef().StyleType() == kPseudoIdNone)
    return;

  // Propagate style from pseudo elements to generated content. We skip children
  // with pseudo element StyleType() in the for-loop above and skip over
  // descendants which are not generated content in this subtree traversal.
  //
  // TODO(futhark): It's possible we could propagate anonymous style from pseudo
  // elements through anonymous table layout objects in the recursive
  // implementation above, but it would require propagating the StyleType()
  // somehow because there is code relying on generated content having a certain
  // StyleType().
  LayoutObject* child = NextInPreOrder(this);
  while (child) {
    if (!child->IsAnonymous()) {
      // Don't propagate into non-anonymous descendants of pseudo elements. This
      // can typically happen for ::first-letter inside ::before. The
      // ::first-letter will propagate to its anonymous children separately.
      child = child->NextInPreOrderAfterChildren(this);
      continue;
    }
    if (child->IsText() || child->IsQuote() || child->IsImage())
      child->SetPseudoStyle(MutableStyle());
    child = child->NextInPreOrder(this);
  }
}

void LayoutObject::SetStyleWithWritingModeOf(scoped_refptr<ComputedStyle> style,
                                             LayoutObject* parent) {
  if (parent) {
    style->SetWritingMode(parent->StyleRef().GetWritingMode());
    style->UpdateFontOrientation();
  }
  SetStyle(std::move(style));
}

void LayoutObject::SetStyleWithWritingModeOfParent(
    scoped_refptr<ComputedStyle> style) {
  SetStyleWithWritingModeOf(std::move(style), Parent());
}

void LayoutObject::AddAsImageObserver(StyleImage* image) {
  if (!image)
    return;
#if DCHECK_IS_ON()
  ++as_image_observer_count_;
#endif
  image->AddClient(this);
}

void LayoutObject::RemoveAsImageObserver(StyleImage* image) {
  if (!image)
    return;
#if DCHECK_IS_ON()
  SECURITY_DCHECK(as_image_observer_count_ > 0u);
  --as_image_observer_count_;
#endif
  image->RemoveClient(this);
}

void LayoutObject::UpdateFillImages(const FillLayer* old_layers,
                                    const FillLayer* new_layers) {
  // Optimize the common case
  if (FillLayer::ImagesIdentical(old_layers, new_layers))
    return;

  // Go through the new layers and AddAsImageObserver() first, to avoid removing
  // all clients of an image.
  for (const FillLayer* curr_new = new_layers; curr_new;
       curr_new = curr_new->Next())
    AddAsImageObserver(curr_new->GetImage());

  for (const FillLayer* curr_old = old_layers; curr_old;
       curr_old = curr_old->Next())
    RemoveAsImageObserver(curr_old->GetImage());
}

void LayoutObject::UpdateCursorImages(const CursorList* old_cursors,
                                      const CursorList* new_cursors) {
  if (old_cursors && new_cursors && *old_cursors == *new_cursors)
    return;

  if (new_cursors) {
    for (const auto& cursor : *new_cursors)
      AddAsImageObserver(cursor.GetImage());
  }
  if (old_cursors) {
    for (const auto& cursor : *old_cursors)
      RemoveAsImageObserver(cursor.GetImage());
  }
}

void LayoutObject::UpdateImage(StyleImage* old_image, StyleImage* new_image) {
  if (old_image != new_image) {
    // AddAsImageObserver first, to avoid removing all clients of an image.
    AddAsImageObserver(new_image);
    RemoveAsImageObserver(old_image);
  }
}

void LayoutObject::UpdateShapeImage(const ShapeValue* old_shape_value,
                                    const ShapeValue* new_shape_value) {
  if (old_shape_value || new_shape_value) {
    UpdateImage(old_shape_value ? old_shape_value->GetImage() : nullptr,
                new_shape_value ? new_shape_value->GetImage() : nullptr);
  }
}

void LayoutObject::CheckCounterChanges(const ComputedStyle* old_style,
                                       const ComputedStyle* new_style) {
  DCHECK(new_style);
  if (old_style) {
    if (old_style->CounterDirectivesEqual(*new_style))
      return;
  } else {
    if (!new_style->GetCounterDirectives())
      return;
  }
  LayoutCounter::LayoutObjectStyleChanged(*this, old_style, *new_style);
  View()->SetNeedsCounterUpdate();
}

LayoutRect LayoutObject::ViewRect() const {
  return View()->ViewRect();
}

FloatPoint LayoutObject::LocalToAbsolute(const FloatPoint& local_point,
                                         MapCoordinatesFlags mode) const {
  TransformState transform_state(TransformState::kApplyTransformDirection,
                                 local_point);
  MapLocalToAncestor(nullptr, transform_state, mode | kApplyContainerFlip);
  transform_state.Flatten();

  return transform_state.LastPlanarPoint();
}

FloatPoint LayoutObject::AncestorToLocal(LayoutBoxModelObject* ancestor,
                                         const FloatPoint& container_point,
                                         MapCoordinatesFlags mode) const {
  TransformState transform_state(
      TransformState::kUnapplyInverseTransformDirection, container_point);
  MapAncestorToLocal(ancestor, transform_state, mode);
  transform_state.Flatten();

  return transform_state.LastPlanarPoint();
}

FloatQuad LayoutObject::AncestorToLocalQuad(LayoutBoxModelObject* ancestor,
                                            const FloatQuad& quad,
                                            MapCoordinatesFlags mode) const {
  TransformState transform_state(
      TransformState::kUnapplyInverseTransformDirection,
      quad.BoundingBox().Center(), quad);
  MapAncestorToLocal(ancestor, transform_state, mode);
  transform_state.Flatten();
  return transform_state.LastPlanarQuad();
}

void LayoutObject::MapLocalToAncestor(const LayoutBoxModelObject* ancestor,
                                      TransformState& transform_state,
                                      MapCoordinatesFlags mode) const {
  if (ancestor == this)
    return;

  AncestorSkipInfo skip_info(ancestor);
  const LayoutObject* container = Container(&skip_info);
  if (!container)
    return;

  if (mode & kApplyContainerFlip) {
    if (IsBox()) {
      mode &= ~kApplyContainerFlip;
    } else if (container->IsBox()) {
      if (container->StyleRef().IsFlippedBlocksWritingMode()) {
        IntPoint center_point = RoundedIntPoint(transform_state.MappedPoint());
        transform_state.Move(ToLayoutBox(container)->FlipForWritingMode(
                                 LayoutPoint(center_point)) -
                             center_point);
      }
      mode &= ~kApplyContainerFlip;
    }
  }

  LayoutSize container_offset =
      OffsetFromContainer(container, mode & kIgnoreScrollOffset);
  // TODO(smcgruer): This is inefficient. Instead we should avoid including
  // offsetForInFlowPosition in offsetFromContainer when ignoring sticky.
  if (mode & kIgnoreStickyOffset && IsStickyPositioned()) {
    container_offset -= ToLayoutBoxModelObject(this)->OffsetForInFlowPosition();
  }

  if (IsLayoutFlowThread()) {
    // So far the point has been in flow thread coordinates (i.e. as if
    // everything in the fragmentation context lived in one tall single column).
    // Convert it to a visual point now, since we're about to escape the flow
    // thread.
    container_offset +=
        ColumnOffset(LayoutPoint(transform_state.MappedPoint()));
  }

  // Text objects just copy their parent's computed style, so we need to ignore
  // them.
  bool preserve3d =
      mode & kUseTransforms &&
      ((container->StyleRef().Preserves3D() && !container->IsText()) ||
       (StyleRef().Preserves3D() && !IsText()));
  if (mode & kUseTransforms && ShouldUseTransformFromContainer(container)) {
    TransformationMatrix t;
    GetTransformFromContainer(container, container_offset, t);
    transform_state.ApplyTransform(t, preserve3d
                                          ? TransformState::kAccumulateTransform
                                          : TransformState::kFlattenTransform);
  } else {
    transform_state.Move(container_offset.Width(), container_offset.Height(),
                         preserve3d ? TransformState::kAccumulateTransform
                                    : TransformState::kFlattenTransform);
  }

  if (skip_info.AncestorSkipped()) {
    // There can't be a transform between |ancestor| and |o|, because transforms
    // create containers, so it should be safe to just subtract the delta
    // between the ancestor and |o|.
    LayoutSize container_offset =
        ancestor->OffsetFromAncestor(container);
    transform_state.Move(-container_offset.Width(), -container_offset.Height(),
                         preserve3d ? TransformState::kAccumulateTransform
                                    : TransformState::kFlattenTransform);
    // If the ancestor is fixed, then the rect is already in its coordinates so
    // doesn't need viewport-adjusting.
    if (ancestor->StyleRef().GetPosition() != EPosition::kFixed &&
        container->IsLayoutView() &&
        StyleRef().GetPosition() == EPosition::kFixed) {
      LayoutSize adjustment = ToLayoutView(container)->OffsetForFixedPosition();
      transform_state.Move(adjustment.Width(), adjustment.Height());
    }
    return;
  }

  container->MapLocalToAncestor(ancestor, transform_state, mode);
}

const LayoutObject* LayoutObject::PushMappingToContainer(
    const LayoutBoxModelObject* ancestor_to_stop_at,
    LayoutGeometryMap& geometry_map) const {
  NOTREACHED();
  return nullptr;
}

void LayoutObject::MapAncestorToLocal(const LayoutBoxModelObject* ancestor,
                                      TransformState& transform_state,
                                      MapCoordinatesFlags mode) const {
  if (this == ancestor)
    return;

  AncestorSkipInfo skip_info(ancestor);
  LayoutObject* container = Container(&skip_info);
  if (!container)
    return;

  bool apply_container_flip = false;
  if (mode & kApplyContainerFlip) {
    if (IsBox()) {
      mode &= ~kApplyContainerFlip;
    } else if (container->IsBox()) {
      apply_container_flip = container->StyleRef().IsFlippedBlocksWritingMode();
      mode &= ~kApplyContainerFlip;
    }
  }

  if (!skip_info.AncestorSkipped())
    container->MapAncestorToLocal(ancestor, transform_state, mode);

  LayoutSize container_offset = OffsetFromContainer(container);
  bool preserve3d =
      mode & kUseTransforms &&
      (container->StyleRef().Preserves3D() || StyleRef().Preserves3D());
  if (mode & kUseTransforms && ShouldUseTransformFromContainer(container)) {
    TransformationMatrix t;
    GetTransformFromContainer(container, container_offset, t);
    transform_state.ApplyTransform(t, preserve3d
                                          ? TransformState::kAccumulateTransform
                                          : TransformState::kFlattenTransform);
  } else {
    transform_state.Move(container_offset.Width(), container_offset.Height(),
                         preserve3d ? TransformState::kAccumulateTransform
                                    : TransformState::kFlattenTransform);
  }

  if (IsLayoutFlowThread()) {
    // Descending into a flow thread. Convert to the local coordinate space,
    // i.e. flow thread coordinates.
    LayoutPoint visual_point = LayoutPoint(transform_state.MappedPoint());
    transform_state.Move(
        visual_point -
        ToLayoutFlowThread(this)->VisualPointToFlowThreadPoint(visual_point));
  }

  if (apply_container_flip) {
    IntPoint center_point = RoundedIntPoint(transform_state.MappedPoint());
    transform_state.Move(
        center_point -
        ToLayoutBox(container)->FlipForWritingMode(LayoutPoint(center_point)));
  }

  if (skip_info.AncestorSkipped()) {
    container_offset = ancestor->OffsetFromAncestor(container);
    transform_state.Move(-container_offset.Width(), -container_offset.Height());
    // If the ancestor is fixed, then the rect is already in its coordinates so
    // doesn't need viewport-adjusting.
    if (ancestor->StyleRef().GetPosition() != EPosition::kFixed &&
        container->IsLayoutView() &&
        StyleRef().GetPosition() == EPosition::kFixed) {
      LayoutSize adjustment = ToLayoutView(container)->OffsetForFixedPosition();
      transform_state.Move(adjustment.Width(), adjustment.Height());
    }
  }
}

bool LayoutObject::ShouldUseTransformFromContainer(
    const LayoutObject* container_object) const {
  // hasTransform() indicates whether the object has transform, transform-style
  // or perspective. We just care about transform, so check the layer's
  // transform directly.
  return (HasLayer() && ToLayoutBoxModelObject(this)->Layer()->Transform()) ||
         (container_object && container_object->StyleRef().HasPerspective());
}

void LayoutObject::GetTransformFromContainer(
    const LayoutObject* container_object,
    const LayoutSize& offset_in_container,
    TransformationMatrix& transform) const {
  transform.MakeIdentity();
  PaintLayer* layer =
      HasLayer() ? ToLayoutBoxModelObject(this)->Layer() : nullptr;
  if (layer && layer->Transform())
    transform.Multiply(layer->CurrentTransform());

  transform.PostTranslate(offset_in_container.Width().ToFloat(),
                          offset_in_container.Height().ToFloat());

  if (container_object && container_object->HasLayer() &&
      container_object->StyleRef().HasPerspective()) {
    // Perspective on the container affects us, so we have to factor it in here.
    DCHECK(container_object->HasLayer());
    FloatPoint perspective_origin =
        ToLayoutBoxModelObject(container_object)->Layer()->PerspectiveOrigin();

    TransformationMatrix perspective_matrix;
    perspective_matrix.ApplyPerspective(
        container_object->StyleRef().Perspective());
    perspective_matrix.ApplyTransformOrigin(perspective_origin.X(),
                                            perspective_origin.Y(), 0);

    transform = perspective_matrix * transform;
  }
}

FloatQuad LayoutObject::LocalToAncestorQuad(
    const FloatQuad& local_quad,
    const LayoutBoxModelObject* ancestor,
    MapCoordinatesFlags mode) const {
  return LocalToAncestorQuadInternal(local_quad, ancestor,
                                     mode | kUseTransforms);
}

FloatQuad LayoutObject::LocalToAncestorQuadWithoutTransforms(
    const FloatQuad& local_quad,
    const LayoutBoxModelObject* ancestor,
    MapCoordinatesFlags mode) const {
  DCHECK(!(mode & kUseTransforms));
  return LocalToAncestorQuadInternal(local_quad, ancestor, mode);
}

FloatQuad LayoutObject::LocalToAncestorQuadInternal(
    const FloatQuad& local_quad,
    const LayoutBoxModelObject* ancestor,
    MapCoordinatesFlags mode) const {
  // Track the point at the center of the quad's bounding box. As
  // mapLocalToAncestor() calls offsetFromContainer(), it will use that point
  // as the reference point to decide which column's transform to apply in
  // multiple-column blocks.
  // TODO(chrishtr): the second argument to this constructor is unnecessary,
  // since we never call lastPlanarPoint().
  TransformState transform_state(TransformState::kApplyTransformDirection,
                                 local_quad.BoundingBox().Center(), local_quad);
  MapLocalToAncestor(ancestor, transform_state, mode | kApplyContainerFlip);
  transform_state.Flatten();

  return transform_state.LastPlanarQuad();
}

FloatPoint LayoutObject::LocalToAncestorPoint(
    const FloatPoint& local_point,
    const LayoutBoxModelObject* ancestor,
    MapCoordinatesFlags mode) const {
  TransformState transform_state(TransformState::kApplyTransformDirection,
                                 local_point);
  MapLocalToAncestor(ancestor, transform_state,
                     mode | kApplyContainerFlip | kUseTransforms);
  transform_state.Flatten();

  return transform_state.LastPlanarPoint();
}

void LayoutObject::LocalToAncestorRects(Vector<LayoutRect>& rects,
                                        const LayoutBoxModelObject* ancestor,
                                        const LayoutPoint& pre_offset,
                                        const LayoutPoint& post_offset) const {
  for (wtf_size_t i = 0; i < rects.size(); ++i) {
    LayoutRect& rect = rects[i];
    rect.MoveBy(pre_offset);
    FloatQuad container_quad =
        LocalToAncestorQuad(FloatQuad(FloatRect(rect)), ancestor);
    LayoutRect container_rect = LayoutRect(container_quad.BoundingBox());
    if (container_rect.IsEmpty()) {
      rects.EraseAt(i--);
      continue;
    }
    container_rect.MoveBy(post_offset);
    rects[i] = container_rect;
  }
}

TransformationMatrix LayoutObject::LocalToAncestorTransform(
    const LayoutBoxModelObject* ancestor,
    MapCoordinatesFlags mode) const {
  TransformState transform_state(TransformState::kApplyTransformDirection);
  MapLocalToAncestor(ancestor, transform_state,
                     mode | kApplyContainerFlip | kUseTransforms);
  return transform_state.AccumulatedTransform();
}

LayoutSize LayoutObject::OffsetFromContainer(const LayoutObject* o,
                                             bool ignore_scroll_offset) const {
  return OffsetFromContainerInternal(o, ignore_scroll_offset);
}

LayoutSize LayoutObject::OffsetFromContainerInternal(
    const LayoutObject* o,
    bool ignore_scroll_offset) const {
  DCHECK_EQ(o, Container());
  return o->HasOverflowClip()
             ? OffsetFromScrollableContainer(o, ignore_scroll_offset)
             : LayoutSize();
}

LayoutSize LayoutObject::OffsetFromScrollableContainer(
    const LayoutObject* container,
    bool ignore_scroll_offset) const {
  DCHECK(container->HasOverflowClip());
  const LayoutBox* box = ToLayoutBox(container);
  if (!ignore_scroll_offset)
    return -LayoutSize(box->ScrolledContentOffset());

  // ScrollOrigin accounts for other writing modes whose content's origin is not
  // at the top-left.
  return LayoutSize(ToIntSize(box->GetScrollableArea()->ScrollOrigin()));
}

LayoutSize LayoutObject::OffsetFromAncestor(
    const LayoutObject* ancestor_container) const {
  if (ancestor_container == this)
    return LayoutSize();

  LayoutSize offset;
  LayoutPoint reference_point;
  const LayoutObject* curr_container = this;
  AncestorSkipInfo skip_info(ancestor_container);
  do {
    const LayoutObject* next_container = curr_container->Container(&skip_info);

    // This means we reached the top without finding container.
    CHECK(next_container);
    if (!next_container)
      break;
    DCHECK(!curr_container->HasTransformRelatedProperty());
    LayoutSize current_offset =
        curr_container->OffsetFromContainer(next_container);
    offset += current_offset;
    reference_point.Move(current_offset);
    curr_container = next_container;
  } while (curr_container != ancestor_container &&
           !skip_info.AncestorSkipped());
  if (skip_info.AncestorSkipped()) {
    DCHECK(curr_container);
    offset -= ancestor_container->OffsetFromAncestor(curr_container);
  }

  return offset;
}

LayoutRect LayoutObject::LocalCaretRect(
    const InlineBox*,
    int,
    LayoutUnit* extra_width_to_end_of_line) const {
  if (extra_width_to_end_of_line)
    *extra_width_to_end_of_line = LayoutUnit();

  return LayoutRect();
}

bool LayoutObject::IsRooted() const {
  const LayoutObject* object = this;
  while (object->Parent() && !object->HasLayer())
    object = object->Parent();
  if (object->HasLayer())
    return ToLayoutBoxModelObject(object)->Layer()->Root()->IsRootLayer();
  return false;
}

RespectImageOrientationEnum LayoutObject::ShouldRespectImageOrientation(
    const LayoutObject* layout_object) {
  if (!layout_object)
    return kDoNotRespectImageOrientation;

  // Respect the image's orientation if it's being used as a full-page image or
  // it's an <img> and the setting to respect it everywhere is set or the <img>
  // has image-orientation: from-image style. FIXME: crbug.com/498233
  if (layout_object->GetDocument().IsImageDocument())
    return kRespectImageOrientation;

  if (!IsHTMLImageElement(layout_object->GetNode()))
    return kDoNotRespectImageOrientation;

  if (layout_object->GetDocument().GetSettings() &&
      layout_object->GetDocument()
          .GetSettings()
          ->GetShouldRespectImageOrientation())
    return kRespectImageOrientation;

  if (layout_object->Style() &&
      layout_object->StyleRef().RespectImageOrientation() ==
          kRespectImageOrientation)
    return kRespectImageOrientation;

  return kDoNotRespectImageOrientation;
}

LayoutObject* LayoutObject::Container(AncestorSkipInfo* skip_info) const {
  // TODO(mstensho): Get rid of this. Nobody should call this method with those
  // flags already set.
  if (skip_info)
    skip_info->ResetOutput();

  if (IsTextOrSVGChild())
    return Parent();

  EPosition pos = style_->GetPosition();
  if (pos == EPosition::kFixed)
    return ContainerForFixedPosition(skip_info);

  if (pos == EPosition::kAbsolute) {
    return ContainerForAbsolutePosition(skip_info);
  }

  if (IsColumnSpanAll()) {
    LayoutObject* multicol_container = SpannerPlaceholder()->Container();
    if (skip_info) {
      // We jumped directly from the spanner to the multicol container. Need to
      // check if we skipped |ancestor| or filter/reflection on the way.
      for (LayoutObject* walker = Parent();
           walker && walker != multicol_container; walker = walker->Parent())
        skip_info->Update(*walker);
    }
    return multicol_container;
  }

  if (IsFloating() && !IsInLayoutNGInlineFormattingContext())
    return ContainingBlock(skip_info);

  return Parent();
}

inline LayoutObject* LayoutObject::ParentCrossingFrames() const {
  if (IsLayoutView())
    return GetFrame()->OwnerLayoutObject();
  return Parent();
}

inline void LayoutObject::ClearLayoutRootIfNeeded() const {
  if (LocalFrameView* view = GetFrameView()) {
    if (!DocumentBeingDestroyed())
      view->ClearLayoutSubtreeRoot(*this);
  }
}

void LayoutObject::WillBeDestroyed() {
  // Destroy any leftover anonymous children.
  LayoutObjectChildList* children = VirtualChildren();
  if (children)
    children->DestroyLeftoverChildren();

  if (LocalFrame* frame = GetFrame()) {
    // If this layoutObject is being autoscrolled, stop the autoscrolling.
    if (frame->GetPage())
      frame->GetPage()->GetAutoscrollController().StopAutoscrollIfNeeded(this);
  }

  // For accessibility management, notify the parent of the imminent change to
  // its child set.
  // We do it now, before remove(), while the parent pointer is still available.
  if (AXObjectCache* cache = GetDocument().ExistingAXObjectCache())
    cache->ChildrenChanged(Parent());

  Remove();

  // The remove() call above may invoke axObjectCache()->childrenChanged() on
  // the parent, which may require the AX layout object for this layoutObject.
  // So we remove the AX layout object now, after the layoutObject is removed.
  if (AXObjectCache* cache = GetDocument().ExistingAXObjectCache())
    cache->Remove(this);

  // If this layoutObject had a parent, remove should have destroyed any
  // counters attached to this layoutObject and marked the affected other
  // counters for reevaluation. This apparently redundant check is here for the
  // case when this layoutObject had no parent at the time remove() was called.

  if (HasCounterNodeMap())
    LayoutCounter::DestroyCounterNodes(*this);

  // Remove the handler if node had touch-action set. Handlers are not added
  // for text nodes so don't try removing for one too. Need to check if
  // m_style is null in cases of partial construction. Any handler we added
  // previously may have already been removed by the Document independently.
  if (GetNode() && !GetNode()->IsTextNode() && style_ &&
      style_->GetTouchAction() != TouchAction::kTouchActionAuto) {
    EventHandlerRegistry& registry =
        GetDocument().GetFrame()->GetEventHandlerRegistry();
    if (registry.EventHandlerTargets(EventHandlerRegistry::kTouchAction)
            ->Contains(GetNode())) {
      registry.DidRemoveEventHandler(*GetNode(),
                                     EventHandlerRegistry::kTouchAction);
    }
  }

  SetAncestorLineBoxDirty(false);

  ClearLayoutRootIfNeeded();

  // Remove this object as ImageResourceObserver.
  if (style_ && !IsText())
    UpdateImageObservers(style_.get(), nullptr);

#if DCHECK_IS_ON()
  // We must have removed all image observers.
  SECURITY_DCHECK(as_image_observer_count_ == 0u);
#endif

  if (GetFrameView())
    SetIsBackgroundAttachmentFixedObject(false);
}

DISABLE_CFI_PERF
void LayoutObject::InsertedIntoTree() {
  // FIXME: We should DCHECK(isRooted()) here but generated content makes some
  // out-of-order insertion.

  // Keep our layer hierarchy updated. Optimize for the common case where we
  // don't have any children and don't have a layer attached to ourselves.
  PaintLayer* layer = nullptr;
  if (SlowFirstChild() || HasLayer()) {
    layer = Parent()->EnclosingLayer();
    AddLayers(layer);
  }

  // If |this| is visible but this object was not, tell the layer it has some
  // visible content that needs to be drawn and layer visibility optimization
  // can't be used
  if (Parent()->StyleRef().Visibility() != EVisibility::kVisible &&
      StyleRef().Visibility() == EVisibility::kVisible && !HasLayer()) {
    if (!layer)
      layer = Parent()->EnclosingLayer();
    if (layer)
      layer->DirtyVisibleContentStatus();
  }

  if (IsInLayoutNGInlineFormattingContext()) {
    // In case of |this| layout object is moved, to avoid paint fragments in old
    // tree live longer than |this|, we reset associated paint fragment list.
    SetFirstInlineFragment(nullptr);
  }

  if (Parent()->ChildrenInline())
    Parent()->DirtyLinesFromChangedChild(this);

  if (LayoutFlowThread* flow_thread = FlowThreadContainingBlock())
    flow_thread->FlowThreadDescendantWasInserted(this);
}

enum FindReferencingScrollAnchorsBehavior { kDontClear, kClear };

static bool FindReferencingScrollAnchors(
    LayoutObject* layout_object,
    FindReferencingScrollAnchorsBehavior behavior) {
  PaintLayer* layer = nullptr;
  if (LayoutObject* parent = layout_object->Parent())
    layer = parent->EnclosingLayer();
  bool found = false;

  // Walk up the layer tree to clear any scroll anchors that reference us.
  while (layer) {
    if (PaintLayerScrollableArea* scrollable_area =
            layer->GetScrollableArea()) {
      ScrollAnchor* anchor = scrollable_area->GetScrollAnchor();
      DCHECK(anchor);
      if (anchor->RefersTo(layout_object)) {
        found = true;
        if (behavior == kClear)
          anchor->NotifyRemoved(layout_object);
        else
          return true;
      }
    }
    layer = layer->Parent();
  }
  return found;
}

void LayoutObject::WillBeRemovedFromTree() {
  // FIXME: We should DCHECK(isRooted()) but we have some out-of-order removals
  // which would need to be fixed first.

  // If we remove a visible child from an invisible parent, we don't know the
  // layer visibility any more.
  PaintLayer* layer = nullptr;
  if (Parent()->StyleRef().Visibility() != EVisibility::kVisible &&
      StyleRef().Visibility() == EVisibility::kVisible && !HasLayer()) {
    layer = Parent()->EnclosingLayer();
    if (layer)
      layer->DirtyVisibleContentStatus();
  }

  // Keep our layer hierarchy updated.
  if (SlowFirstChild() || HasLayer()) {
    if (!layer)
      layer = Parent()->EnclosingLayer();
    RemoveLayers(layer);
  }

  if (IsOutOfFlowPositioned() && Parent()->ChildrenInline())
    Parent()->DirtyLinesFromChangedChild(this);

  RemoveFromLayoutFlowThread();

  // Update cached boundaries in SVG layoutObjects if a child is removed.
  if (Parent()->IsSVG())
    Parent()->SetNeedsBoundariesUpdate();

  if (bitfields_.IsScrollAnchorObject()) {
    // Clear the bit first so that anchor.clear() doesn't recurse into
    // findReferencingScrollAnchors.
    bitfields_.SetIsScrollAnchorObject(false);
    FindReferencingScrollAnchors(this, kClear);
  }

  if (RuntimeEnabledFeatures::FirstContentfulPaintPlusPlusEnabled()) {
    if (LocalFrameView* frame_view = GetFrameView()) {
      frame_view->GetPaintTimingDetector().NotifyNodeRemoved(*this);
    }
  }
}

void LayoutObject::SetNeedsPaintPropertyUpdate() {
  if (bitfields_.NeedsPaintPropertyUpdate())
    return;

  // Anytime a layout object needs a paint property update, we should also do
  // intersection observation.
  // TODO(vmpstr): Figure out if there's a cleaner way to do this outside of
  // this function, since this is potentially called many times for a single
  // frame view subtree.
  GetFrameView()->SetIntersectionObservationState(LocalFrameView::kDesired);

  bitfields_.SetNeedsPaintPropertyUpdate(true);
  for (auto* ancestor = ParentCrossingFrames();
       ancestor && !ancestor->DescendantNeedsPaintPropertyUpdate();
       ancestor = ancestor->ParentCrossingFrames()) {
    ancestor->bitfields_.SetDescendantNeedsPaintPropertyUpdate(true);
  }
}

void LayoutObject::SetAncestorsNeedPaintPropertyUpdateForMainThreadScrolling() {
  LayoutObject* ancestor = ParentCrossingFrames();
  while (ancestor) {
    ancestor->SetNeedsPaintPropertyUpdate();
    ancestor = ancestor->ParentCrossingFrames();
  }
}

void LayoutObject::MaybeClearIsScrollAnchorObject() {
  if (!bitfields_.IsScrollAnchorObject())
    return;
  bitfields_.SetIsScrollAnchorObject(
      FindReferencingScrollAnchors(this, kDontClear));
}

void LayoutObject::RemoveFromLayoutFlowThread() {
  if (!IsInsideFlowThread())
    return;

  // Sometimes we remove the element from the flow, but it's not destroyed at
  // that time.
  // It's only until later when we actually destroy it and remove all the
  // children from it.
  // Currently, that happens for firstLetter elements and list markers.
  // Pass in the flow thread so that we don't have to look it up for all the
  // children.
  // If we're a column spanner, we need to use our parent to find the flow
  // thread, since a spanner doesn't have the flow thread in its containing
  // block chain. We still need to notify the flow thread when the layoutObject
  // removed happens to be a spanner, so that we get rid of the spanner
  // placeholder, and column sets around the placeholder get merged.
  LayoutFlowThread* flow_thread = IsColumnSpanAll()
                                      ? Parent()->FlowThreadContainingBlock()
                                      : FlowThreadContainingBlock();
  RemoveFromLayoutFlowThreadRecursive(flow_thread);
}

void LayoutObject::RemoveFromLayoutFlowThreadRecursive(
    LayoutFlowThread* layout_flow_thread) {
  if (const LayoutObjectChildList* children = VirtualChildren()) {
    for (LayoutObject* child = children->FirstChild(); child;
         child = child->NextSibling()) {
      if (child->IsLayoutFlowThread())
        continue;  // Don't descend into inner fragmentation contexts.
      child->RemoveFromLayoutFlowThreadRecursive(child->IsLayoutFlowThread()
                                                     ? ToLayoutFlowThread(child)
                                                     : layout_flow_thread);
    }
  }

  if (layout_flow_thread && layout_flow_thread != this)
    layout_flow_thread->FlowThreadDescendantWillBeRemoved(this);
  SetIsInsideFlowThread(false);
  CHECK(!SpannerPlaceholder());
}

void LayoutObject::DestroyAndCleanupAnonymousWrappers() {
  // If the tree is destroyed, there is no need for a clean-up phase.
  if (DocumentBeingDestroyed()) {
    Destroy();
    return;
  }

  LayoutObject* destroy_root = this;
  for (LayoutObject *destroy_root_parent = destroy_root->Parent();
       destroy_root_parent && destroy_root_parent->IsAnonymous();
       destroy_root = destroy_root_parent,
                    destroy_root_parent = destroy_root_parent->Parent()) {
    // Anonymous block continuations are tracked and destroyed elsewhere (see
    // the bottom of LayoutBlockFlow::RemoveChild)
    auto* destroy_root_parent_block =
        DynamicTo<LayoutBlockFlow>(destroy_root_parent);
    if (destroy_root_parent_block &&
        destroy_root_parent_block->IsAnonymousBlockContinuation())
      break;
    // A flow thread is tracked by its containing block. Whether its children
    // are removed or not is irrelevant.
    if (destroy_root_parent->IsLayoutFlowThread())
      break;

    if (destroy_root->PreviousSibling() || destroy_root->NextSibling())
      break;  // Need to keep the anonymous parent, since it won't become empty
              // by the removal of this LayoutObject.
  }

  destroy_root->Destroy();

  // WARNING: |this| is deleted here.
}

void LayoutObject::Destroy() {
  WillBeDestroyed();
  delete this;
}

PositionWithAffinity LayoutObject::PositionForPoint(const LayoutPoint&) const {
  return CreatePositionWithAffinity(CaretMinOffset());
}

CompositingState LayoutObject::GetCompositingState() const {
  return HasLayer()
             ? ToLayoutBoxModelObject(this)->Layer()->GetCompositingState()
             : kNotComposited;
}

CompositingReasons LayoutObject::AdditionalCompositingReasons() const {
  return CompositingReason::kNone;
}

bool LayoutObject::HitTestAllPhases(
    HitTestResult& result,
    const HitTestLocation& location_in_container,
    const LayoutPoint& accumulated_offset,
    HitTestFilter hit_test_filter) {
  bool inside = false;
  if (hit_test_filter != kHitTestSelf) {
    // First test the foreground layer (lines and inlines).
    inside = NodeAtPoint(result, location_in_container, accumulated_offset,
                         kHitTestForeground);

    // Test floats next.
    if (!inside)
      inside = NodeAtPoint(result, location_in_container, accumulated_offset,
                           kHitTestFloat);

    // Finally test to see if the mouse is in the background (within a child
    // block's background).
    if (!inside)
      inside = NodeAtPoint(result, location_in_container, accumulated_offset,
                           kHitTestChildBlockBackgrounds);
  }

  // See if the mouse is inside us but not any of our descendants
  if (hit_test_filter != kHitTestDescendants && !inside)
    inside = NodeAtPoint(result, location_in_container, accumulated_offset,
                         kHitTestBlockBackground);

  return inside;
}

Node* LayoutObject::NodeForHitTest() const {
  Node* node = GetNode();

  // If we hit the anonymous layoutObjects inside generated content we should
  // actually hit the generated content so walk up to the PseudoElement.
  if (!node && Parent() && Parent()->IsBeforeOrAfterContent()) {
    for (LayoutObject* layout_object = Parent(); layout_object && !node;
         layout_object = layout_object->Parent())
      node = layout_object->GetNode();
  }

  return node;
}

void LayoutObject::UpdateHitTestResult(HitTestResult& result,
                                       const LayoutPoint& point) const {
  if (result.InnerNode())
    return;

  if (Node* n = NodeForHitTest())
    result.SetNodeAndPosition(n, point);
}

bool LayoutObject::NodeAtPoint(HitTestResult&,
                               const HitTestLocation& /*locationInContainer*/,
                               const LayoutPoint& /*accumulatedOffset*/,
                               HitTestAction) {
  return false;
}

void LayoutObject::ScheduleRelayout() {
  if (IsLayoutView()) {
    LocalFrameView* view = ToLayoutView(this)->GetFrameView();
    if (view)
      view->ScheduleRelayout();
  } else {
    if (IsRooted()) {
      if (LayoutView* layout_view = View()) {
        if (LocalFrameView* frame_view = layout_view->GetFrameView())
          frame_view->ScheduleRelayoutOfSubtree(this);
      }
    }
  }
}

void LayoutObject::ForceLayout() {
  SetSelfNeedsLayoutForAvailableSpace(true);
  UpdateLayout();
}

enum StyleCacheState { kCached, kUncached };

static scoped_refptr<const ComputedStyle> FirstLineStyleForCachedUncachedType(
    StyleCacheState type,
    const LayoutObject* layout_object,
    const ComputedStyle* style) {
  DCHECK(layout_object);
  const LayoutObject* layout_object_for_first_line_style = layout_object;
  if (layout_object->IsBeforeOrAfterContent())
    layout_object_for_first_line_style = layout_object->Parent();

  if (layout_object_for_first_line_style->BehavesLikeBlockContainer()) {
    if (const LayoutBlock* first_line_block =
            To<LayoutBlock>(layout_object_for_first_line_style)
                ->EnclosingFirstLineStyleBlock()) {
      if (type == kCached)
        return first_line_block->GetCachedPseudoStyle(kPseudoIdFirstLine,
                                                      style);
      return first_line_block->GetUncachedPseudoStyle(
          PseudoStyleRequest(kPseudoIdFirstLine), style);
    }
  } else if (layout_object_for_first_line_style->IsLayoutInline()) {
    if (layout_object_for_first_line_style->IsAnonymous()) {
      // Anonymous inline box for ::first-line should inherit background.
      if (ToLayoutInline(layout_object_for_first_line_style)
              ->IsFirstLineAnonymous()) {
        // TODO(kojii): This function must return a style that is referred by
        // someone else, and that we can't create an inherited style here.
        // Returning parent's style seems to work for now.
        return FirstLineStyleForCachedUncachedType(
            type, layout_object->Parent(), style);
      }
      // TODO(kojii): This does not look correct, but creating a first-line
      // style for anonymous inline box does not seem easy for now.
      return nullptr;
    }
    if (layout_object_for_first_line_style->GetNode()
            ->IsFirstLetterPseudoElement()) {
      return nullptr;
    }
    const ComputedStyle* parent_style =
        layout_object_for_first_line_style->Parent()->FirstLineStyle();
    if (parent_style != layout_object_for_first_line_style->Parent()->Style()) {
      if (type == kCached) {
        // A first-line style is in effect. Cache a first-line style for
        // ourselves.
        return layout_object_for_first_line_style->GetCachedPseudoStyle(
            kPseudoIdFirstLineInherited, parent_style);
      }
      return layout_object_for_first_line_style->GetUncachedPseudoStyle(
          PseudoStyleRequest(kPseudoIdFirstLineInherited), parent_style);
    }
  }
  return nullptr;
}

scoped_refptr<const ComputedStyle> LayoutObject::UncachedFirstLineStyle()
    const {
  if (!GetDocument().GetStyleEngine().UsesFirstLineRules())
    return nullptr;

  DCHECK(!IsText());

  return FirstLineStyleForCachedUncachedType(kUncached, this, style_.get());
}

const ComputedStyle* LayoutObject::CachedFirstLineStyle() const {
  DCHECK(GetDocument().GetStyleEngine().UsesFirstLineRules());

  if (scoped_refptr<const ComputedStyle> style =
          FirstLineStyleForCachedUncachedType(
              kCached, IsText() ? Parent() : this, style_.get()))
    return style.get();

  return style_.get();
}

const ComputedStyle* LayoutObject::GetCachedPseudoStyle(
    PseudoId pseudo,
    const ComputedStyle* parent_style) const {
  DCHECK_NE(pseudo, kPseudoIdBefore);
  DCHECK_NE(pseudo, kPseudoIdAfter);
  if (!GetNode())
    return nullptr;

  Element* element = Traversal<Element>::FirstAncestorOrSelf(*GetNode());
  if (!element)
    return nullptr;

  const auto* cached_pseudo_style = element->CachedStyleForPseudoElement(
      PseudoStyleRequest(pseudo), parent_style);
  if (cached_pseudo_style && pseudo == kPseudoIdFirstLine &&
      bitfields_.PendingUpdateFirstLineImageObservers()) {
    // Update image observers now after we have updated the first line
    // style cache.
    const_cast<LayoutObject*>(this)->UpdateFirstLineImageObservers(nullptr,
                                                                   Style());
  }
  return cached_pseudo_style;
}

scoped_refptr<ComputedStyle> LayoutObject::GetUncachedPseudoStyle(
    const PseudoStyleRequest& request,
    const ComputedStyle* parent_style) const {
  DCHECK_NE(request.pseudo_id, kPseudoIdBefore);
  DCHECK_NE(request.pseudo_id, kPseudoIdAfter);
  if (!GetNode())
    return nullptr;

  Element* element = Traversal<Element>::FirstAncestorOrSelf(*GetNode());
  if (!element)
    return nullptr;
  if (element->IsPseudoElement())
    return nullptr;

  return element->StyleForPseudoElement(request, parent_style);
}

void LayoutObject::AddAnnotatedRegions(Vector<AnnotatedRegionValue>& regions) {
  // Convert the style regions to absolute coordinates.
  if (StyleRef().Visibility() != EVisibility::kVisible || !IsBox())
    return;

  if (StyleRef().DraggableRegionMode() == EDraggableRegionMode::kNone)
    return;

  LayoutBox* box = ToLayoutBox(this);
  FloatRect local_bounds(FloatPoint(), FloatSize(box->Size()));
  FloatRect abs_bounds = LocalToAbsoluteQuad(local_bounds).BoundingBox();

  AnnotatedRegionValue region;
  region.draggable =
      StyleRef().DraggableRegionMode() == EDraggableRegionMode::kDrag;
  region.bounds = LayoutRect(abs_bounds);
  regions.push_back(region);
}

bool LayoutObject::WillRenderImage() {
  // Without visibility we won't render (and therefore don't care about
  // animation).
  if (StyleRef().Visibility() != EVisibility::kVisible)
    return false;

  // We will not render a new image when ExecutionContext is paused
  if (GetDocument().IsContextPaused())
    return false;

  // Suspend animations when the page is not visible.
  if (GetDocument().hidden())
    return false;

  // If we're not in a window (i.e., we're dormant from being in a background
  // tab) then we don't want to render either.
  return GetDocument().View()->IsVisible();
}

bool LayoutObject::GetImageAnimationPolicy(ImageAnimationPolicy& policy) {
  if (!GetDocument().GetSettings())
    return false;
  policy = GetDocument().GetSettings()->GetImageAnimationPolicy();
  return true;
}

int LayoutObject::CaretMinOffset() const {
  return 0;
}

int LayoutObject::CaretMaxOffset() const {
  if (IsAtomicInlineLevel())
    return GetNode() ? std::max(1U, GetNode()->CountChildren()) : 1;
  if (IsHR())
    return 1;
  return 0;
}

bool LayoutObject::IsInert() const {
  const LayoutObject* layout_object = this;
  while (!layout_object->GetNode())
    layout_object = layout_object->Parent();
  return layout_object->GetNode()->IsInert();
}

void LayoutObject::ImageChanged(ImageResourceContent* image,
                                CanDeferInvalidation defer) {
  DCHECK(node_);

  // Image change notifications should not be received during paint because
  // the resulting invalidations will be cleared following paint. This can also
  // lead to modifying the tree out from under paint(), see: crbug.com/616700.
  DCHECK_NE(GetDocument().Lifecycle().GetState(),
            DocumentLifecycle::LifecycleState::kInPaint);

  ImageChanged(static_cast<WrappedImagePtr>(image), defer);
}

void LayoutObject::ImageNotifyFinished(ImageResourceContent*) {
  if (AXObjectCache* cache = GetDocument().ExistingAXObjectCache())
    cache->ImageLoaded(this);
}

Element* LayoutObject::OffsetParent(const Element* base) const {
  if (IsDocumentElement() || IsBody())
    return nullptr;

  if (IsFixedPositioned())
    return nullptr;

  float effective_zoom = StyleRef().EffectiveZoom();
  Node* node = nullptr;
  for (LayoutObject* ancestor = Parent(); ancestor;
       ancestor = ancestor->Parent()) {
    // Spec: http://www.w3.org/TR/cssom-view/#offset-attributes

    node = ancestor->GetNode();

    if (!node)
      continue;

    // TODO(kochi): If |base| or |node| is nested deep in shadow roots, this
    // loop may get expensive, as isUnclosedNodeOf() can take up to O(N+M) time
    // (N and M are depths).
    if (base && (node->IsClosedShadowHiddenFrom(*base) ||
                 (node->IsInShadowTree() &&
                  node->ContainingShadowRoot()->IsUserAgent()))) {
      // If 'position: fixed' node is found while traversing up, terminate the
      // loop and return null.
      if (ancestor->IsFixedPositioned())
        return nullptr;
      continue;
    }

    if (ancestor->CanContainAbsolutePositionObjects())
      break;

    if (IsHTMLBodyElement(*node))
      break;

    if (!IsPositioned() &&
        (IsHTMLTableElement(*node) || IsHTMLTableCellElement(*node)))
      break;

    // Webkit specific extension where offsetParent stops at zoom level changes.
    if (effective_zoom != ancestor->StyleRef().EffectiveZoom())
      break;
  }

  return node && node->IsElementNode() ? ToElement(node) : nullptr;
}

void LayoutObject::NotifyImageFullyRemoved(ImageResourceContent* image) {
  if (RuntimeEnabledFeatures::ElementTimingEnabled(&GetDocument())) {
    LocalDOMWindow* window = GetDocument().domWindow();
    if (window) {
      ImageElementTiming::From(*window).NotifyBackgroundImageRemoved(this,
                                                                     image);
    }
  }
  if (RuntimeEnabledFeatures::FirstContentfulPaintPlusPlusEnabled()) {
    if (LocalFrameView* frame_view = GetFrameView()) {
      frame_view->GetPaintTimingDetector().NotifyBackgroundImageRemoved(*this,
                                                                        image);
    }
  }
}

PositionWithAffinity LayoutObject::CreatePositionWithAffinity(
    int offset,
    TextAffinity affinity) const {
  // If this is a non-anonymous layoutObject in an editable area, then it's
  // simple.
  if (Node* node = NonPseudoNode()) {
    if (!HasEditableStyle(*node)) {
      // If it can be found, we prefer a visually equivalent position that is
      // editable.
      // TODO(layout-dev): Once we fix callers of |CreatePositionWithAffinity()|
      // we should use |Position| constructor. See http://crbug.com/827923
      const Position position =
          Position::CreateWithoutValidationDeprecated(*node, offset);
      Position candidate =
          MostForwardCaretPosition(position, kCanCrossEditingBoundary);
      if (HasEditableStyle(*candidate.AnchorNode()))
        return PositionWithAffinity(candidate, affinity);
      candidate = MostBackwardCaretPosition(position, kCanCrossEditingBoundary);
      if (HasEditableStyle(*candidate.AnchorNode()))
        return PositionWithAffinity(candidate, affinity);
    }
    // FIXME: Eliminate legacy editing positions
    return PositionWithAffinity(Position::EditingPositionOf(node, offset),
                                affinity);
  }

  // We don't want to cross the boundary between editable and non-editable
  // regions of the document, but that is either impossible or at least
  // extremely unlikely in any normal case because we stop as soon as we
  // find a single non-anonymous layoutObject.

  // Find a nearby non-anonymous layoutObject.
  const LayoutObject* child = this;
  while (const LayoutObject* parent = child->Parent()) {
    // Find non-anonymous content after.
    for (const LayoutObject* layout_object = child->NextInPreOrder(parent);
         layout_object; layout_object = layout_object->NextInPreOrder(parent)) {
      if (const Node* node = layout_object->NonPseudoNode()) {
        return PositionWithAffinity(FirstPositionInOrBeforeNode(*node));
      }
    }

    // Find non-anonymous content before.
    for (const LayoutObject* layout_object = child->PreviousInPreOrder();
         layout_object; layout_object = layout_object->PreviousInPreOrder()) {
      if (layout_object == parent)
        break;
      if (const Node* node = layout_object->NonPseudoNode())
        return PositionWithAffinity(LastPositionInOrAfterNode(*node));
    }

    // Use the parent itself unless it too is anonymous.
    if (const Node* node = parent->NonPseudoNode())
      return PositionWithAffinity(FirstPositionInOrBeforeNode(*node));

    // Repeat at the next level up.
    child = parent;
  }

  // Everything was anonymous. Give up.
  return PositionWithAffinity();
}

PositionWithAffinity LayoutObject::CreatePositionWithAffinity(
    int offset) const {
  return CreatePositionWithAffinity(offset, TextAffinity::kDownstream);
}

PositionWithAffinity LayoutObject::CreatePositionWithAffinity(
    const Position& position) const {
  if (position.IsNotNull())
    return PositionWithAffinity(position);

  DCHECK(!GetNode());
  return CreatePositionWithAffinity(0);
}

CursorDirective LayoutObject::GetCursor(const LayoutPoint&, Cursor&) const {
  return kSetCursorBasedOnStyle;
}

bool LayoutObject::CanUpdateSelectionOnRootLineBoxes() const {
  if (NeedsLayout())
    return false;

  const LayoutBlock* containing_block = ContainingBlock();
  return containing_block ? !containing_block->NeedsLayout() : false;
}

void LayoutObject::SetNeedsBoundariesUpdate() {
  if (IsSVGChild()) {
    // The boundaries affect mask clip.
    auto* resources = SVGResourcesCache::CachedResourcesForLayoutObject(*this);
    if (resources && resources->Masker())
      SetNeedsPaintPropertyUpdate();
    if (resources && resources->Clipper())
      InvalidateClipPathCache();
  }
  if (LayoutObject* layout_object = Parent())
    layout_object->SetNeedsBoundariesUpdate();
}

FloatRect LayoutObject::ObjectBoundingBox() const {
  NOTREACHED();
  return FloatRect();
}

FloatRect LayoutObject::StrokeBoundingBox() const {
  NOTREACHED();
  return FloatRect();
}

FloatRect LayoutObject::VisualRectInLocalSVGCoordinates() const {
  NOTREACHED();
  return FloatRect();
}

AffineTransform LayoutObject::LocalSVGTransform() const {
  return AffineTransform();
}

bool LayoutObject::IsRelayoutBoundaryForInspector() const {
  return ObjectIsRelayoutBoundary(this);
}

inline void LayoutObject::SetNeedsPaintOffsetAndVisualRectUpdate() {
  DCHECK(ShouldCheckForPaintInvalidation());
  bitfields_.SetNeedsPaintOffsetAndVisualRectUpdate(true);
  for (auto* ancestor = ParentCrossingFrames();
       ancestor && !ancestor->DescendantNeedsPaintOffsetAndVisualRectUpdate();
       ancestor = ancestor->ParentCrossingFrames()) {
    ancestor->bitfields_.SetDescendantNeedsPaintOffsetAndVisualRectUpdate(true);
  }
}

void LayoutObject::SetShouldInvalidateSelection() {
  if (!CanUpdateSelectionOnRootLineBoxes())
    return;
  bitfields_.SetShouldInvalidateSelection(true);
  SetShouldCheckForPaintInvalidation();
}

void LayoutObject::SetShouldDoFullPaintInvalidation(
    PaintInvalidationReason reason) {
  SetShouldDoFullPaintInvalidationWithoutGeometryChange(reason);
  SetNeedsPaintOffsetAndVisualRectUpdate();
}

static PaintInvalidationReason DocumentLifecycleBasedPaintInvalidationReason(
    const DocumentLifecycle& document_lifecycle) {
  switch (document_lifecycle.GetState()) {
    case DocumentLifecycle::kInStyleRecalc:
      return PaintInvalidationReason::kStyle;
    case DocumentLifecycle::kInPreLayout:
    case DocumentLifecycle::kInPerformLayout:
    case DocumentLifecycle::kAfterPerformLayout:
      return PaintInvalidationReason::kGeometry;
    case DocumentLifecycle::kInCompositingUpdate:
      return PaintInvalidationReason::kCompositing;
    default:
      return PaintInvalidationReason::kFull;
  }
}

void LayoutObject::SetShouldDoFullPaintInvalidationWithoutGeometryChange(
    PaintInvalidationReason reason) {
  // Only full invalidation reasons are allowed.
  DCHECK(IsFullPaintInvalidationReason(reason));
  // This is before the early return to ensure visual update is always scheduled
  // in case that this is called not during a document lifecycle update.
  GetFrameView()->ScheduleVisualUpdateForPaintInvalidationIfNeeded();

  if (ShouldDoFullPaintInvalidation())
    return;
  SetShouldCheckForPaintInvalidationWithoutGeometryChange();
  if (reason == PaintInvalidationReason::kFull) {
    reason = DocumentLifecycleBasedPaintInvalidationReason(
        GetDocument().Lifecycle());
  }
  full_paint_invalidation_reason_ = reason;
  bitfields_.SetShouldDelayFullPaintInvalidation(false);
}

void LayoutObject::SetShouldCheckForPaintInvalidation() {
  SetShouldCheckForPaintInvalidationWithoutGeometryChange();
  SetNeedsPaintOffsetAndVisualRectUpdate();
}

void LayoutObject::SetShouldCheckForPaintInvalidationWithoutGeometryChange() {
  if (ShouldCheckForPaintInvalidation())
    return;
  GetFrameView()->ScheduleVisualUpdateForPaintInvalidationIfNeeded();

  bitfields_.SetShouldCheckForPaintInvalidation(true);
  for (LayoutObject* parent = ParentCrossingFrames();
       parent && !parent->ShouldCheckForPaintInvalidation();
       parent = parent->ParentCrossingFrames()) {
    parent->bitfields_.SetShouldCheckForPaintInvalidation(true);
  }
}

void LayoutObject::SetSubtreeShouldCheckForPaintInvalidation() {
  if (SubtreeShouldCheckForPaintInvalidation()) {
    DCHECK(ShouldCheckForPaintInvalidation());
    return;
  }
  SetShouldCheckForPaintInvalidation();
  bitfields_.SetSubtreeShouldCheckForPaintInvalidation(true);
}

void LayoutObject::SetMayNeedPaintInvalidationAnimatedBackgroundImage() {
  if (MayNeedPaintInvalidationAnimatedBackgroundImage())
    return;
  bitfields_.SetMayNeedPaintInvalidationAnimatedBackgroundImage(true);
  SetShouldCheckForPaintInvalidationWithoutGeometryChange();
}

void LayoutObject::SetShouldDelayFullPaintInvalidation() {
  // Should have already set a full paint invalidation reason.
  DCHECK(IsFullPaintInvalidationReason(full_paint_invalidation_reason_));

  bitfields_.SetShouldDelayFullPaintInvalidation(true);
  if (!ShouldCheckForPaintInvalidation()) {
    // This will also schedule a visual update.
    SetShouldCheckForPaintInvalidationWithoutGeometryChange();
  } else {
    // Schedule visual update for the next document cycle in which we will
    // check if the delayed invalidation should be promoted to a real
    // invalidation.
    GetFrameView()->ScheduleVisualUpdateForPaintInvalidationIfNeeded();
  }
}

void LayoutObject::ClearPaintInvalidationFlags() {
// PaintInvalidationStateIsDirty should be kept in sync with the
// booleans that are cleared below.
#if DCHECK_IS_ON()
  DCHECK(!ShouldCheckForPaintInvalidation() || PaintInvalidationStateIsDirty());
#endif
  fragment_.SetPartialInvalidationLocalRect(LayoutRect());
  if (!ShouldDelayFullPaintInvalidation()) {
    full_paint_invalidation_reason_ = PaintInvalidationReason::kNone;
    bitfields_.SetBackgroundNeedsFullPaintInvalidation(false);
  }
  bitfields_.SetShouldCheckForPaintInvalidation(false);
  bitfields_.SetSubtreeShouldCheckForPaintInvalidation(false);
  bitfields_.SetSubtreeShouldDoFullPaintInvalidation(false);
  bitfields_.SetMayNeedPaintInvalidationAnimatedBackgroundImage(false);
  bitfields_.SetNeedsPaintOffsetAndVisualRectUpdate(false);
  bitfields_.SetDescendantNeedsPaintOffsetAndVisualRectUpdate(false);
  bitfields_.SetShouldInvalidateSelection(false);
}

#if DCHECK_IS_ON()
bool LayoutObject::PaintInvalidationStateIsDirty() const {
  return BackgroundNeedsFullPaintInvalidation() ||
         ShouldCheckForPaintInvalidation() || ShouldInvalidateSelection() ||
         NeedsPaintOffsetAndVisualRectUpdate() ||
         DescendantNeedsPaintOffsetAndVisualRectUpdate() ||
         ShouldDoFullPaintInvalidation() ||
         SubtreeShouldDoFullPaintInvalidation() ||
         MayNeedPaintInvalidationAnimatedBackgroundImage() ||
         !fragment_.PartialInvalidationLocalRect().IsEmpty();
}
#endif

bool LayoutObject::IsAllowedToModifyLayoutTreeStructure(Document& document) {
  return document.Lifecycle().StateAllowsLayoutTreeMutations();
}

void LayoutObject::SetSubtreeShouldDoFullPaintInvalidation(
    PaintInvalidationReason reason) {
  SetShouldDoFullPaintInvalidation(reason);
  bitfields_.SetSubtreeShouldDoFullPaintInvalidation(true);
}

void LayoutObject::SetIsBackgroundAttachmentFixedObject(
    bool is_background_attachment_fixed_object) {
  DCHECK(GetFrameView());
  if (bitfields_.IsBackgroundAttachmentFixedObject() ==
      is_background_attachment_fixed_object)
    return;
  bitfields_.SetIsBackgroundAttachmentFixedObject(
      is_background_attachment_fixed_object);
  if (is_background_attachment_fixed_object)
    GetFrameView()->AddBackgroundAttachmentFixedObject(this);
  else
    GetFrameView()->RemoveBackgroundAttachmentFixedObject(this);
}

LayoutRect LayoutObject::DebugRect() const {
  return LayoutRect();
}

void LayoutObject::InvalidateSelectedChildrenOnStyleChange() {
  // LayoutSelection::Commit() propagates the state up the containing node
  // chain to
  // tell if a block contains selected nodes or not. If this layout object is
  // not a block, we need to get the selection state from the containing block
  // to tell if we have any selected node children.
  LayoutBlock* block =
      IsLayoutBlock() ? To<LayoutBlock>(this) : ContainingBlock();
  if (!block)
    return;
  if (!block->IsSelected())
    return;

  // ::selection style only applies to direct selection leaf children of the
  // element on which the ::selection style is set. Thus, we only walk the
  // direct children here.
  for (LayoutObject* child = SlowFirstChild(); child;
       child = child->NextSibling()) {
    if (!child->CanBeSelectionLeaf())
      continue;
    if (!child->IsSelected())
      continue;
    child->SetShouldInvalidateSelection();
  }
}

void LayoutObject::MarkEffectiveAllowedTouchActionChanged() {
  bitfields_.SetEffectiveAllowedTouchActionChanged(true);
  // If we're locked, mark our descendants as needing this change. This is used
  // a signal to ensure we mark the element as needing effective allowed
  // touch action recalculation when the element becomes unlocked.
  if (PrePaintBlockedByDisplayLock()) {
    bitfields_.SetDescendantEffectiveAllowedTouchActionChanged(true);
    return;
  }

  LayoutObject* obj = ParentCrossingFrames();
  while (obj && !obj->DescendantEffectiveAllowedTouchActionChanged()) {
    obj->bitfields_.SetDescendantEffectiveAllowedTouchActionChanged(true);
    if (obj->PrePaintBlockedByDisplayLock())
      break;

    obj = obj->ParentCrossingFrames();
  }
}

void LayoutObject::InvalidateIfControlStateChanged(ControlState control_state) {
  if (LayoutTheme::GetTheme().ControlStateChanged(GetNode(), StyleRef(),
                                                  control_state)) {
    SetSubtreeShouldDoFullPaintInvalidation();
  }
}

// Note about ::first-letter pseudo-element:
//   When an element has ::first-letter pseudo-element, first letter characters
//   are taken from |Text| node and first letter characters are considered
//   as content of <pseudo:first-letter>.
//   For following HTML,
//      <style>div::first-letter {color: red}</style>
//      <div>abc</div>
//   we have following layout tree:
//      LayoutBlockFlow {DIV} at (0,0) size 784x55
//        LayoutInline {<pseudo:first-letter>} at (0,0) size 22x53
//          LayoutTextFragment (anonymous) at (0,1) size 22x53
//            text run at (0,1) width 22: "a"
//        LayoutTextFragment {#text} at (21,30) size 16x17
//          text run at (21,30) width 16: "bc"
//  In this case, |Text::layoutObject()| for "abc" returns |LayoutTextFragment|
//  containing "bc", and it is called remaining part.
//
//  Even if |Text| node contains only first-letter characters, e.g. just "a",
//  remaining part of |LayoutTextFragment|, with |fragmentLength()| == 0, is
//  appeared in layout tree.
//
//  When |Text| node contains only first-letter characters and whitespaces, e.g.
//  "B\n", associated |LayoutTextFragment| is first-letter part instead of
//  remaining part.
//
//  Punctuation characters are considered as first-letter. For "(1)ab",
//  "(1)" are first-letter part and "ab" are remaining part.
const LayoutObject* AssociatedLayoutObjectOf(const Node& node,
                                             int offset_in_node,
                                             LayoutObjectSide object_side) {
  DCHECK_GE(offset_in_node, 0);
  LayoutObject* layout_object = node.GetLayoutObject();
  if (!node.IsTextNode() || !layout_object ||
      !ToLayoutText(layout_object)->IsTextFragment())
    return layout_object;
  LayoutTextFragment* layout_text_fragment =
      ToLayoutTextFragment(layout_object);
  if (!layout_text_fragment->IsRemainingTextLayoutObject()) {
    DCHECK_LE(
        static_cast<unsigned>(offset_in_node),
        layout_text_fragment->Start() + layout_text_fragment->FragmentLength());
    return layout_text_fragment;
  }
  if (layout_text_fragment->FragmentLength()) {
    const unsigned threshold =
        object_side == LayoutObjectSide::kRemainingTextIfOnBoundary
            ? layout_text_fragment->Start()
            : layout_text_fragment->Start() + 1;
    if (static_cast<unsigned>(offset_in_node) >= threshold)
      return layout_object;
  }
  return layout_text_fragment->GetFirstLetterPart();
}

bool LayoutObject::CanBeSelectionLeaf() const {
  if (SlowFirstChild() || StyleRef().Visibility() != EVisibility::kVisible)
    return false;
  return CanBeSelectionLeafInternal();
}

void LayoutObject::InvalidateClipPathCache() {
  SetNeedsPaintPropertyUpdate();
  for (auto* fragment = &fragment_; fragment;
       fragment = fragment->NextFragment())
    fragment->InvalidateClipPathCache();
}

IntRect LayoutObject::AdjustVisualRectForInlineBox(
    const IntRect& visual_rect) const {
  // For simplicity, we use the layout object's visual rect as the visual rect
  // of contained inline boxes, mapped to the correct transform space of the
  // inline boxes.
  if (const auto* properties = FirstFragment().PaintProperties()) {
    if (const auto* scroll_translation = properties->ScrollTranslation()) {
      // This mapping happens for inline box whose LayoutObject is a LayoutBlock
      // whose VisualRect is not in the same transform space as the inline box.
      // For now this happens for EllipsisBox only.
      auto float_visual_rect = FloatRect(visual_rect);
      float_visual_rect.Move(-scroll_translation->Translation2D());
      return EnclosingIntRect(float_visual_rect);
    }
  }
  return visual_rect;
}

Vector<LayoutRect> LayoutObject::PhysicalOutlineRects(
    const LayoutPoint& additional_offset,
    NGOutlineType outline_type) const {
  Vector<LayoutRect> outline_rects;
  AddOutlineRects(outline_rects, additional_offset, outline_type);
  if (IsSVGChild() || !HasFlippedBlocksWritingMode())
    return outline_rects;

  const auto* writing_mode_container =
      IsBox() ? ToLayoutBox(this) : ContainingBlock();
  for (auto& r : outline_rects) {
    r.MoveBy(-additional_offset);
    writing_mode_container->FlipForWritingMode(r);
    r.MoveBy(additional_offset);
  }
  return outline_rects;
}

void LayoutObject::SetModifiedStyleOutsideStyleRecalc(
    scoped_refptr<ComputedStyle> style,
    ApplyStyleChanges apply_changes) {
  SetStyle(style, apply_changes);
  if (IsAnonymous() || !GetNode() || !GetNode()->IsElementNode())
    return;
  GetNode()->SetComputedStyle(std::move(style));
}

}  // namespace blink

#ifndef NDEBUG

void showTree(const blink::LayoutObject* object) {
  if (object)
    object->ShowTreeForThis();
  else
    DLOG(INFO) << "Cannot showTree. Root is (nil)";
}

void showLineTree(const blink::LayoutObject* object) {
  if (object)
    object->ShowLineTreeForThis();
  else
    DLOG(INFO) << "Cannot showLineTree. Root is (nil)";
}

void showLayoutTree(const blink::LayoutObject* object1) {
  showLayoutTree(object1, nullptr);
}

void showLayoutTree(const blink::LayoutObject* object1,
                    const blink::LayoutObject* object2) {
  if (object1) {
    const blink::LayoutObject* root = object1;
    while (root->Parent())
      root = root->Parent();
    if (object1) {
      StringBuilder string_builder;
      root->DumpLayoutTreeAndMark(string_builder, object1, "*", object2, "-",
                                  0);
      DLOG(INFO) << "\n" << string_builder.ToString().Utf8().data();
    }
  } else {
    DLOG(INFO) << "Cannot showLayoutTree. Root is (nil)";
  }
}

#endif
