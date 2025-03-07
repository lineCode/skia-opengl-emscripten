/*
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2009 Antonio Gomes <tonikitoo@webkit.org>
 *
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/core/page/spatial_navigation.h"

#include "third_party/blink/renderer/core/dom/node_traversal.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_view.h"
#include "third_party/blink/renderer/core/frame/settings.h"
#include "third_party/blink/renderer/core/frame/visual_viewport.h"
#include "third_party/blink/renderer/core/html/html_area_element.h"
#include "third_party/blink/renderer/core/html/html_frame_owner_element.h"
#include "third_party/blink/renderer/core/html/html_image_element.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/core/layout/layout_box.h"
#include "third_party/blink/renderer/core/layout/layout_inline.h"
#include "third_party/blink/renderer/core/layout/layout_view.h"
#include "third_party/blink/renderer/core/page/frame_tree.h"
#include "third_party/blink/renderer/core/page/page.h"
#include "third_party/blink/renderer/core/paint/paint_layer_scrollable_area.h"
#include "third_party/blink/renderer/platform/geometry/int_rect.h"

namespace blink {

static void DeflateIfOverlapped(LayoutRect&, LayoutRect&);

FocusCandidate::FocusCandidate(Node* node, SpatialNavigationDirection direction)
    : visible_node(nullptr), focusable_node(nullptr), is_offscreen(true) {
  DCHECK(node);
  DCHECK(node->IsElementNode());

  if (auto* area = ToHTMLAreaElementOrNull(*node)) {
    HTMLImageElement* image = area->ImageElement();
    if (!image || !image->GetLayoutObject())
      return;

    visible_node = image;
    rect_in_root_frame = StartEdgeForAreaElement(*area, direction);
  } else {
    if (!node->GetLayoutObject())
      return;

    visible_node = node;
    rect_in_root_frame = NodeRectInRootFrame(node);
  }

  focusable_node = node;
  is_offscreen = IsOffscreen(visible_node);
}

bool IsSpatialNavigationEnabled(const LocalFrame* frame) {
  return (frame && frame->GetSettings() &&
          frame->GetSettings()->GetSpatialNavigationEnabled());
}

static bool RectsIntersectOnOrthogonalAxis(SpatialNavigationDirection direction,
                                           const LayoutRect& a,
                                           const LayoutRect& b) {
  switch (direction) {
    case SpatialNavigationDirection::kLeft:
    case SpatialNavigationDirection::kRight:
      return a.MaxY() > b.Y() && a.Y() < b.MaxY();
    case SpatialNavigationDirection::kUp:
    case SpatialNavigationDirection::kDown:
      return a.MaxX() > b.X() && a.X() < b.MaxX();
    default:
      NOTREACHED();
      return false;
  }
}

// Return true if rect |a| is below |b|. False otherwise.
// For overlapping rects, |a| is considered to be below |b|
// if both edges of |a| are below the respective ones of |b|.
static inline bool Below(const LayoutRect& a, const LayoutRect& b) {
  return a.Y() >= b.MaxY() || (a.Y() >= b.Y() && a.MaxY() > b.MaxY() &&
                               a.X() < b.MaxX() && a.MaxX() > b.X());
}

// Return true if rect |a| is on the right of |b|. False otherwise.
// For overlapping rects, |a| is considered to be on the right of |b|
// if both edges of |a| are on the right of the respective ones of |b|.
static inline bool RightOf(const LayoutRect& a, const LayoutRect& b) {
  return a.X() >= b.MaxX() || (a.X() >= b.X() && a.MaxX() > b.MaxX() &&
                               a.Y() < b.MaxY() && a.MaxY() > b.Y());
}

static bool IsRectInDirection(SpatialNavigationDirection direction,
                              const LayoutRect& cur_rect,
                              const LayoutRect& target_rect) {
  switch (direction) {
    case SpatialNavigationDirection::kLeft:
      return RightOf(cur_rect, target_rect);
    case SpatialNavigationDirection::kRight:
      return RightOf(target_rect, cur_rect);
    case SpatialNavigationDirection::kUp:
      return Below(cur_rect, target_rect);
    case SpatialNavigationDirection::kDown:
      return Below(target_rect, cur_rect);
    default:
      NOTREACHED();
      return false;
  }
}

FloatRect RectInViewport(const Node& node) {
  LocalFrameView* frame_view = node.GetDocument().View();
  if (!frame_view)
    return FloatRect();

  DCHECK(!frame_view->NeedsLayout());

  LayoutObject* object = node.GetLayoutObject();
  if (!object)
    return FloatRect();

  LayoutRect rect_in_root_frame = NodeRectInRootFrame(&node);

  // Convert to the visual viewport which will account for pinch zoom.
  VisualViewport& visual_viewport =
      object->GetDocument().GetPage()->GetVisualViewport();
  FloatRect rect_in_viewport =
      visual_viewport.RootFrameToViewport(FloatRect(rect_in_root_frame));

  // RootFrameToViewport doesn't clip so manually apply the viewport clip here.
  FloatRect viewport_rect =
      FloatRect(FloatPoint(), FloatSize(visual_viewport.Size()));
  rect_in_viewport.Intersect(viewport_rect);

  return rect_in_viewport;
}

// Answers true if |node| is completely outside the user's (visual) viewport.
// This logic is used by spatnav to rule out offscreen focus candidates and an
// offscreen activeElement. When activeElement is offscreen, spatnav doesn't use
// it as the search origin; the search will start at an edge of the visual
// viewport instead.
// TODO(crbug.com/889840): Fix VisibleBoundsInVisualViewport().
// If VisibleBoundsInVisualViewport() would have taken "element-clips" into
// account, spatnav could have called it directly; no need to check the
// LayoutObject's VisibleContentRect.
bool IsOffscreen(const Node* node) {
  DCHECK(node);
  return RectInViewport(*node).IsEmpty();
}

ScrollableArea* ScrollableAreaFor(const Node* node) {
  if (node->IsDocumentNode()) {
    LocalFrameView* view = node->GetDocument().View();
    if (!view)
      return nullptr;

    return view->GetScrollableArea();
  }

  LayoutObject* object = node->GetLayoutObject();
  if (!object || !object->IsBox())
    return nullptr;

  return ToLayoutBox(object)->GetScrollableArea();
}

bool HasRemoteFrame(const Node* node) {
  auto* frame_owner_element = DynamicTo<HTMLFrameOwnerElement>(node);
  if (!frame_owner_element)
    return false;

  return frame_owner_element->ContentFrame() &&
         frame_owner_element->ContentFrame()->IsRemoteFrame();
}

bool ScrollInDirection(Node* container, SpatialNavigationDirection direction) {
  DCHECK(container);

  if (!CanScrollInDirection(container, direction))
    return false;

  int dx = 0;
  int dy = 0;
  int pixels_per_line_step = ScrollableArea::PixelsPerLineStep(
      container->GetDocument().GetFrame()->View()->GetChromeClient());
  switch (direction) {
    case SpatialNavigationDirection::kLeft:
      dx = -pixels_per_line_step;
      break;
    case SpatialNavigationDirection::kRight:
      // TODO(bokan, https://crbug.com/952326): Fix this DCHECK.
      //  DCHECK_GT(container->GetLayoutBox()->ScrollWidth(),
      //            container->GetLayoutBox()->ScrollLeft() +
      //                container->GetLayoutBox()->ClientWidth());
      dx = pixels_per_line_step;
      break;
    case SpatialNavigationDirection::kUp:
      dy = -pixels_per_line_step;
      break;
    case SpatialNavigationDirection::kDown:
      // TODO(bokan, https://crbug.com/952326): Fix this DCHECK.
      //  DCHECK_GT(container->GetLayoutBox()->ScrollHeight(),
      //            container->GetLayoutBox()->ScrollTop() +
      //                container->GetLayoutBox()->ClientHeight());
      dy = pixels_per_line_step;
      break;
    default:
      NOTREACHED();
      return false;
  }

  // TODO(crbug.com/914775): Use UserScroll() instead. UserScroll() does a
  // smooth, animated scroll which might make it easier for users to understand
  // spatnav's moves. Another advantage of using ScrollableArea::UserScroll() is
  // that it returns a ScrollResult so we don't need to call
  // CanScrollInDirection(). Regular arrow-key scrolling (without
  // --enable-spatial-navigation) already uses smooth scrolling by default.
  ScrollableArea* scroller = ScrollableAreaFor(container);
  DCHECK(scroller);
  scroller->ScrollBy(ScrollOffset(dx, dy), kUserScroll);
  return true;
}

static void DeflateIfOverlapped(LayoutRect& a, LayoutRect& b) {
  if (!a.Intersects(b) || a.Contains(b) || b.Contains(a))
    return;

  LayoutUnit deflate_factor = LayoutUnit(-FudgeFactor());

  // Avoid negative width or height values.
  if ((a.Width() + 2 * deflate_factor > 0) &&
      (a.Height() + 2 * deflate_factor > 0))
    a.Inflate(deflate_factor);

  if ((b.Width() + 2 * deflate_factor > 0) &&
      (b.Height() + 2 * deflate_factor > 0))
    b.Inflate(deflate_factor);
}

bool IsScrollableNode(const Node* node) {
  if (!node)
    return false;

  if (node->IsDocumentNode())
    return true;

  LayoutObject* layout_object = node->GetLayoutObject();
  if (!layout_object || !layout_object->IsBox())
    return false;

  return ToLayoutBox(layout_object)->CanBeScrolledAndHasScrollableArea();
}

Node* ScrollableAreaOrDocumentOf(Node* node) {
  DCHECK(node);
  Node* parent = node;
  do {
    // FIXME: Spatial navigation is broken for OOPI.
    if (auto* document = DynamicTo<Document>(parent))
      parent = document->GetFrame()->DeprecatedLocalOwner();
    else
      parent = parent->ParentOrShadowHostNode();
  } while (parent && !IsScrollableAreaOrDocument(parent));

  return parent;
}

bool IsScrollableAreaOrDocument(const Node* node) {
  if (!node)
    return false;

  auto* frame_owner_element = DynamicTo<HTMLFrameOwnerElement>(node);
  return (frame_owner_element && frame_owner_element->ContentFrame()) ||
         IsScrollableNode(node);
}

bool CanScrollInDirection(const Node* container,
                          SpatialNavigationDirection direction) {
  DCHECK(container);
  if (auto* document = DynamicTo<Document>(container))
    return CanScrollInDirection(document->GetFrame(), direction);

  if (!IsScrollableNode(container))
    return false;

  DCHECK(container->GetLayoutObject());
  switch (direction) {
    case SpatialNavigationDirection::kLeft:
      return (container->GetLayoutObject()->Style()->OverflowX() !=
                  EOverflow::kHidden &&
              container->GetLayoutBox()->ScrollLeft() > 0);
    case SpatialNavigationDirection::kUp:
      return (container->GetLayoutObject()->Style()->OverflowY() !=
                  EOverflow::kHidden &&
              container->GetLayoutBox()->ScrollTop() > 0);
    case SpatialNavigationDirection::kRight:
      return (container->GetLayoutObject()->Style()->OverflowX() !=
                  EOverflow::kHidden &&
              container->GetLayoutBox()->ScrollLeft() +
                      container->GetLayoutBox()->ClientWidth() <
                  container->GetLayoutBox()->ScrollWidth());
    case SpatialNavigationDirection::kDown:
      return (container->GetLayoutObject()->Style()->OverflowY() !=
                  EOverflow::kHidden &&
              container->GetLayoutBox()->ScrollTop() +
                      container->GetLayoutBox()->ClientHeight() <
                  container->GetLayoutBox()->ScrollHeight());
    default:
      NOTREACHED();
      return false;
  }
}

bool CanScrollInDirection(const LocalFrame* frame,
                          SpatialNavigationDirection direction) {
  if (!frame->View())
    return false;
  LayoutView* layoutView = frame->ContentLayoutObject();
  if (!layoutView)
    return false;
  ScrollbarMode vertical_mode;
  ScrollbarMode horizontal_mode;
  layoutView->CalculateScrollbarModes(horizontal_mode, vertical_mode);
  if ((direction == SpatialNavigationDirection::kLeft ||
       direction == SpatialNavigationDirection::kRight) &&
      kScrollbarAlwaysOff == horizontal_mode)
    return false;
  if ((direction == SpatialNavigationDirection::kUp ||
       direction == SpatialNavigationDirection::kDown) &&
      kScrollbarAlwaysOff == vertical_mode)
    return false;
  ScrollableArea* scrollable_area = frame->View()->GetScrollableArea();
  LayoutSize size(scrollable_area->ContentsSize());
  LayoutSize offset(scrollable_area->ScrollOffsetInt());
  LayoutRect rect(scrollable_area->VisibleContentRect(kIncludeScrollbars));

  switch (direction) {
    case SpatialNavigationDirection::kLeft:
      return offset.Width() > 0;
    case SpatialNavigationDirection::kUp:
      return offset.Height() > 0;
    case SpatialNavigationDirection::kRight:
      return rect.Width() + offset.Width() < size.Width();
    case SpatialNavigationDirection::kDown:
      return rect.Height() + offset.Height() < size.Height();
    default:
      NOTREACHED();
      return false;
  }
}

LayoutRect NodeRectInRootFrame(const Node* node) {
  DCHECK(node);
  DCHECK(node->GetLayoutObject());
  DCHECK(!node->GetDocument().View()->NeedsLayout());

  LayoutObject* object = node->GetLayoutObject();

  LayoutRect rect = LayoutRect(object->LocalBoundingBoxRectForAccessibility());

  // Inset the bounding box by the border.
  // TODO(bokan): As far as I can tell, this is to work around empty iframes
  // that have a border. It's unclear if that's still useful.
  rect.Move(node->GetLayoutObject()->Style()->BorderLeftWidth(),
            node->GetLayoutObject()->Style()->BorderTopWidth());
  rect.SetWidth(LayoutUnit(
      rect.Width() - node->GetLayoutObject()->Style()->BorderLeftWidth() -
      node->GetLayoutObject()->Style()->BorderRightWidth()));
  rect.SetHeight(LayoutUnit(
      rect.Height() - node->GetLayoutObject()->Style()->BorderTopWidth() -
      node->GetLayoutObject()->Style()->BorderBottomWidth()));

  object->MapToVisualRectInAncestorSpace(/*ancestor=*/nullptr, rect);
  return rect;
}

// This method calculates the exitPoint from the startingRect and the entryPoint
// into the candidate rect.  The line between those 2 points is the closest
// distance between the 2 rects.  Takes care of overlapping rects, defining
// points so that the distance between them is zero where necessary.
void EntryAndExitPointsForDirection(SpatialNavigationDirection direction,
                                    const LayoutRect& starting_rect,
                                    const LayoutRect& potential_rect,
                                    LayoutPoint& exit_point,
                                    LayoutPoint& entry_point) {
  switch (direction) {
    case SpatialNavigationDirection::kLeft:
      exit_point.SetX(starting_rect.X());
      if (potential_rect.MaxX() < starting_rect.X())
        entry_point.SetX(potential_rect.MaxX());
      else
        entry_point.SetX(starting_rect.X());
      break;
    case SpatialNavigationDirection::kUp:
      exit_point.SetY(starting_rect.Y());
      if (potential_rect.MaxY() < starting_rect.Y())
        entry_point.SetY(potential_rect.MaxY());
      else
        entry_point.SetY(starting_rect.Y());
      break;
    case SpatialNavigationDirection::kRight:
      exit_point.SetX(starting_rect.MaxX());
      if (potential_rect.X() > starting_rect.MaxX())
        entry_point.SetX(potential_rect.X());
      else
        entry_point.SetX(starting_rect.MaxX());
      break;
    case SpatialNavigationDirection::kDown:
      exit_point.SetY(starting_rect.MaxY());
      if (potential_rect.Y() > starting_rect.MaxY())
        entry_point.SetY(potential_rect.Y());
      else
        entry_point.SetY(starting_rect.MaxY());
      break;
    default:
      NOTREACHED();
  }

  switch (direction) {
    case SpatialNavigationDirection::kLeft:
    case SpatialNavigationDirection::kRight:
      if (Below(starting_rect, potential_rect)) {
        exit_point.SetY(starting_rect.Y());
        if (potential_rect.MaxY() < starting_rect.Y())
          entry_point.SetY(potential_rect.MaxY());
        else
          entry_point.SetY(starting_rect.Y());
      } else if (Below(potential_rect, starting_rect)) {
        exit_point.SetY(starting_rect.MaxY());
        if (potential_rect.Y() > starting_rect.MaxY())
          entry_point.SetY(potential_rect.Y());
        else
          entry_point.SetY(starting_rect.MaxY());
      } else {
        exit_point.SetY(max(starting_rect.Y(), potential_rect.Y()));
        entry_point.SetY(exit_point.Y());
      }
      break;
    case SpatialNavigationDirection::kUp:
    case SpatialNavigationDirection::kDown:
      if (RightOf(starting_rect, potential_rect)) {
        exit_point.SetX(starting_rect.X());
        if (potential_rect.MaxX() < starting_rect.X())
          entry_point.SetX(potential_rect.MaxX());
        else
          entry_point.SetX(starting_rect.X());
      } else if (RightOf(potential_rect, starting_rect)) {
        exit_point.SetX(starting_rect.MaxX());
        if (potential_rect.X() > starting_rect.MaxX())
          entry_point.SetX(potential_rect.X());
        else
          entry_point.SetX(starting_rect.MaxX());
      } else {
        exit_point.SetX(max(starting_rect.X(), potential_rect.X()));
        entry_point.SetX(exit_point.X());
      }
      break;
    default:
      NOTREACHED();
  }
}

bool AreElementsOnSameLine(const FocusCandidate& first_candidate,
                           const FocusCandidate& second_candidate) {
  if (first_candidate.IsNull() || second_candidate.IsNull())
    return false;

  if (!first_candidate.visible_node->GetLayoutObject() ||
      !second_candidate.visible_node->GetLayoutObject())
    return false;

  if (!first_candidate.rect_in_root_frame.Intersects(
          second_candidate.rect_in_root_frame))
    return false;

  if (IsHTMLAreaElement(*first_candidate.focusable_node) ||
      IsHTMLAreaElement(*second_candidate.focusable_node))
    return false;

  if (!first_candidate.visible_node->GetLayoutObject()->IsLayoutInline() ||
      !second_candidate.visible_node->GetLayoutObject()->IsLayoutInline())
    return false;

  if (first_candidate.visible_node->GetLayoutObject()->ContainingBlock() !=
      second_candidate.visible_node->GetLayoutObject()->ContainingBlock())
    return false;

  return true;
}

double ComputeDistanceDataForNode(SpatialNavigationDirection direction,
                                  const FocusCandidate& current_interest,
                                  const FocusCandidate& candidate) {
  if (!IsRectInDirection(direction, current_interest.rect_in_root_frame,
                         candidate.rect_in_root_frame))
    return MaxDistance();

  if (AreElementsOnSameLine(current_interest, candidate)) {
    if ((direction == SpatialNavigationDirection::kUp &&
         current_interest.rect_in_root_frame.Y() >
             candidate.rect_in_root_frame.Y()) ||
        (direction == SpatialNavigationDirection::kDown &&
         candidate.rect_in_root_frame.Y() >
             current_interest.rect_in_root_frame.Y())) {
      return 0.0;
    }
  }

  LayoutRect node_rect = candidate.rect_in_root_frame;
  LayoutRect current_rect = current_interest.rect_in_root_frame;
  DeflateIfOverlapped(current_rect, node_rect);

  LayoutPoint exit_point;
  LayoutPoint entry_point;
  EntryAndExitPointsForDirection(direction, current_rect, node_rect, exit_point,
                                 entry_point);

  LayoutUnit x_axis = (exit_point.X() - entry_point.X()).Abs();
  LayoutUnit y_axis = (exit_point.Y() - entry_point.Y()).Abs();

  LayoutUnit navigation_axis_distance;
  LayoutUnit weighted_orthogonal_axis_distance;

  // Bias and weights are put to the orthogonal axis distance calculation
  // so aligned candidates would have advantage over partially-aligned ones
  // and then over not-aligned candidates. The bias is given to not-aligned
  // candidates with respect to size of the current rect. The weight for
  // left/right direction is given a higher value to allow navigation on
  // common horizonally-aligned elements. The hardcoded values are based on
  // tests and experiments.
  const int kOrthogonalWeightForLeftRight = 30;
  const int kOrthogonalWeightForUpDown = 2;
  int orthogonal_bias = 0;

  switch (direction) {
    case SpatialNavigationDirection::kLeft:
    case SpatialNavigationDirection::kRight:
      navigation_axis_distance = x_axis;
      if (!RectsIntersectOnOrthogonalAxis(direction, current_rect, node_rect))
        orthogonal_bias = (current_rect.Height() / 2).ToInt();
      weighted_orthogonal_axis_distance =
          (y_axis + orthogonal_bias) * kOrthogonalWeightForLeftRight;
      break;
    case SpatialNavigationDirection::kUp:
    case SpatialNavigationDirection::kDown:
      navigation_axis_distance = y_axis;
      if (!RectsIntersectOnOrthogonalAxis(direction, current_rect, node_rect))
        orthogonal_bias = (current_rect.Width() / 2).ToInt();
      weighted_orthogonal_axis_distance =
          (x_axis + orthogonal_bias) * kOrthogonalWeightForUpDown;
      break;
    default:
      NOTREACHED();
      return MaxDistance();
  }

  double euclidian_distance_pow2 =
      (x_axis * x_axis + y_axis * y_axis).ToDouble();
  LayoutRect intersection_rect = Intersection(current_rect, node_rect);
  double overlap =
      (intersection_rect.Width() * intersection_rect.Height()).ToDouble();

  // Distance calculation is based on http://www.w3.org/TR/WICD/#focus-handling
  return sqrt(euclidian_distance_pow2) + navigation_axis_distance +
         weighted_orthogonal_axis_distance - sqrt(overlap);
}

// Returns a thin rectangle that represents one of box's sides.
LayoutRect OppositeEdge(SpatialNavigationDirection side,
                        const LayoutRect& box,
                        LayoutUnit thickness) {
  LayoutRect thin_rect = box;
  switch (side) {
    case SpatialNavigationDirection::kLeft:
      thin_rect.SetX(thin_rect.MaxX() - thickness);
      thin_rect.SetWidth(thickness);
      break;
    case SpatialNavigationDirection::kRight:
      thin_rect.SetWidth(thickness);
      break;
    case SpatialNavigationDirection::kDown:
      thin_rect.SetHeight(thickness);
      break;
    case SpatialNavigationDirection::kUp:
      thin_rect.SetY(thin_rect.MaxY() - thickness);
      thin_rect.SetHeight(thickness);
      break;
    default:
      NOTREACHED();
  }

  return thin_rect;
}

LayoutRect StartEdgeForAreaElement(const HTMLAreaElement& area,
                                   SpatialNavigationDirection direction) {
  DCHECK(area.ImageElement());
  // Area elements tend to overlap more than other focusable elements. We
  // flatten the rect of the area elements to minimize the effect of overlapping
  // areas.
  LayoutRect rect = OppositeEdge(
      direction,
      area.GetDocument().GetFrame()->View()->ConvertToRootFrame(
          area.ComputeAbsoluteRect(area.ImageElement()->GetLayoutObject())),
      LayoutUnit(1) /* snav-imagemap-overlapped-areas.html */);
  return rect;
}

HTMLFrameOwnerElement* FrameOwnerElement(const FocusCandidate& candidate) {
  return DynamicTo<HTMLFrameOwnerElement>(candidate.visible_node.Get());
}

// The visual viewport's rect (given in the root frame's coordinate space).
LayoutRect RootViewport(const LocalFrame* current_frame) {
  return LayoutRect(
      current_frame->GetPage()->GetVisualViewport().VisibleRect());
}

// Spatnav uses this rectangle to measure distances to focus candidates.
// The search origin is either activeElement F itself, if it's being at least
// partially visible, or else, its first [partially] visible scroller. If both
// F and its enclosing scroller are completely off-screen, we recurse to the
// scroller’s scroller ... all the way up until the root frame's document.
// The root frame's document is a good base case because it's, per definition,
// a visible scrollable area.
LayoutRect SearchOrigin(const LayoutRect viewport_rect_of_root_frame,
                        Node* focus_node,
                        const SpatialNavigationDirection direction) {
  if (!focus_node) {
    // Search from one of the visual viewport's edges towards the navigated
    // direction. For example, UP makes spatnav search upwards, starting at the
    // visual viewport's bottom.
    return OppositeEdge(direction, viewport_rect_of_root_frame);
  }

  auto* area_element = ToHTMLAreaElementOrNull(focus_node);
  if (area_element)
    focus_node = area_element->ImageElement();

  if (!IsOffscreen(focus_node)) {
    if (area_element)
      return StartEdgeForAreaElement(*area_element, direction);

    LayoutRect box_in_root_frame = NodeRectInRootFrame(focus_node);
    return Intersection(box_in_root_frame, viewport_rect_of_root_frame);
  }

  Node* container = ScrollableAreaOrDocumentOf(focus_node);
  while (container) {
    if (!IsOffscreen(container)) {
      // The first scroller that encloses focus and is [partially] visible.
      LayoutRect box_in_root_frame = NodeRectInRootFrame(container);
      return OppositeEdge(direction, Intersection(box_in_root_frame,
                                                  viewport_rect_of_root_frame));
    }

    container = ScrollableAreaOrDocumentOf(container);
  }

  return OppositeEdge(direction, viewport_rect_of_root_frame);
}

}  // namespace blink
