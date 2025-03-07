// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/view.h"

#include <stddef.h>

#include <map>
#include <memory>

#include "base/bind.h"
#include "base/command_line.h"
#include "base/i18n/rtl.h"
#include "base/macros.h"
#include "base/rand_util.h"
#include "base/run_loop.h"
#include "base/stl_util.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "base/test/icu_test_util.h"
#include "base/test/scoped_feature_list.h"
#include "base/time/time.h"
#include "build/build_config.h"
#include "cc/paint/display_item_list.h"
#include "components/viz/common/surfaces/parent_local_surface_id_allocator.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/clipboard/clipboard.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/compositor/compositor.h"
#include "ui/compositor/compositor_switches.h"
#include "ui/compositor/layer.h"
#include "ui/compositor/layer_animator.h"
#include "ui/compositor/paint_context.h"
#include "ui/compositor/test/draw_waiter_for_test.h"
#include "ui/events/event.h"
#include "ui/events/event_utils.h"
#include "ui/events/keycodes/keyboard_codes.h"
#include "ui/events/scoped_target_handler.h"
#include "ui/events/test/event_generator.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/transform.h"
#include "ui/native_theme/native_theme.h"
#include "ui/native_theme/test_native_theme.h"
#include "ui/strings/grit/ui_strings.h"
#include "ui/views/background.h"
#include "ui/views/controls/native/native_view_host.h"
#include "ui/views/controls/scroll_view.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/metadata/metadata_types.h"
#include "ui/views/paint_info.h"
#include "ui/views/test/views_test_base.h"
#include "ui/views/view_observer.h"
#include "ui/views/widget/native_widget.h"
#include "ui/views/widget/root_view.h"
#include "ui/views/window/dialog_client_view.h"
#include "ui/views/window/dialog_delegate.h"

using base::ASCIIToUTF16;

namespace {

// Returns true if |ancestor| is an ancestor of |layer|.
bool LayerIsAncestor(const ui::Layer* ancestor, const ui::Layer* layer) {
  while (layer && layer != ancestor)
    layer = layer->parent();
  return layer == ancestor;
}

// Convenience functions for walking a View tree.
const views::View* FirstView(const views::View* view) {
  const views::View* v;
  for (v = view; !v->children().empty(); v = v->children().front()) {
  }
  return v;
}

const views::View* NextView(const views::View* view) {
  const views::View* v = view;
  const views::View* parent = v->parent();
  if (!parent)
    return nullptr;
  const auto next = std::next(parent->FindChild(v));
  return (next == parent->children().cend()) ? parent : FirstView(*next);
}

// Convenience functions for walking a Layer tree.
const ui::Layer* FirstLayer(const ui::Layer* layer) {
  const ui::Layer* l = layer;
  while (l->children().size() > 0)
    l = l->children()[0];
  return l;
}

const ui::Layer* NextLayer(const ui::Layer* layer) {
  const ui::Layer* parent = layer->parent();
  if (!parent)
    return nullptr;
  const std::vector<ui::Layer*> children = parent->children();
  size_t index;
  for (index = 0; index < children.size(); index++) {
    if (children[index] == layer)
      break;
  }
  size_t next = index + 1;
  if (next < children.size())
    return FirstLayer(children[next]);
  return parent;
}

// Given the root nodes of a View tree and a Layer tree, makes sure the two
// trees are in sync.
bool ViewAndLayerTreeAreConsistent(const views::View* view,
                                   const ui::Layer* layer) {
  const views::View* v = FirstView(view);
  const ui::Layer* l = FirstLayer(layer);
  while (v && l) {
    // Find the view with a layer.
    while (v && !v->layer())
      v = NextView(v);
    EXPECT_TRUE(v);
    if (!v)
      return false;

    // Check if the View tree and the Layer tree are in sync.
    EXPECT_EQ(l, v->layer());
    if (v->layer() != l)
      return false;

    // Check if the visibility states of the View and the Layer are in sync.
    EXPECT_EQ(l->IsDrawn(), v->IsDrawn());
    if (v->IsDrawn() != l->IsDrawn()) {
      for (const views::View* vv = v; vv; vv = vv->parent())
        LOG(ERROR) << "V: " << vv << " " << vv->visible() << " "
                   << vv->IsDrawn() << " " << vv->layer();
      for (const ui::Layer* ll = l; ll; ll = ll->parent())
        LOG(ERROR) << "L: " << ll << " " << ll->IsDrawn();
      return false;
    }

    // Check if the size of the View and the Layer are in sync.
    EXPECT_EQ(l->bounds(), v->bounds());
    if (v->bounds() != l->bounds())
      return false;

    if (v == view || l == layer)
      return v == view && l == layer;

    v = NextView(v);
    l = NextLayer(l);
  }

  return false;
}

// Constructs a View tree with the specified depth.
void ConstructTree(views::View* view, int depth) {
  if (depth == 0)
    return;
  int count = base::RandInt(1, 5);
  for (int i = 0; i < count; i++) {
    views::View* v = new views::View;
    view->AddChildView(v);
    if (base::RandDouble() > 0.5)
      v->SetPaintToLayer();
    if (base::RandDouble() < 0.2)
      v->SetVisible(false);

    ConstructTree(v, depth - 1);
  }
}

void ScrambleTree(views::View* view) {
  if (view->children().empty())
    return;

  for (views::View* child : view->children())
    ScrambleTree(child);

  size_t count = view->children().size();
  if (count > 1) {
    const uint64_t max = count - 1;
    size_t a = size_t{base::RandGenerator(max)};
    size_t b = size_t{base::RandGenerator(max)};

    if (a != b) {
      views::View* view_a = view->children()[a];
      views::View* view_b = view->children()[b];
      view->ReorderChildView(view_a, b);
      view->ReorderChildView(view_b, a);
    }
  }

  if (!view->layer() && base::RandDouble() < 0.1)
    view->SetPaintToLayer();

  if (base::RandDouble() < 0.1)
    view->SetVisible(!view->visible());
}

}  // namespace

namespace views {

using ViewTest = ViewsTestBase;

// A derived class for testing purpose.
class TestView : public View {
 public:
  TestView() = default;
  ~TestView() override = default;

  // Reset all test state
  void Reset() {
    did_change_bounds_ = false;
    did_layout_ = false;
    last_mouse_event_type_ = 0;
    location_.SetPoint(0, 0);
    received_mouse_enter_ = false;
    received_mouse_exit_ = false;
    did_paint_ = false;
    accelerator_count_map_.clear();
    can_process_events_within_subtree_ = true;
  }

  // Exposed as public for testing.
  void DoFocus() {
    views::View::Focus();
  }

  void DoBlur() {
    views::View::Blur();
  }

  FocusBehavior focus_behavior() const { return View::focus_behavior(); }

  void set_can_process_events_within_subtree(bool can_process) {
    can_process_events_within_subtree_ = can_process;
  }

  bool CanProcessEventsWithinSubtree() const override {
    return can_process_events_within_subtree_;
  }

  void Layout() override {
    did_layout_ = true;
    View::Layout();
  }

  void OnBoundsChanged(const gfx::Rect& previous_bounds) override;
  bool OnMousePressed(const ui::MouseEvent& event) override;
  bool OnMouseDragged(const ui::MouseEvent& event) override;
  void OnMouseReleased(const ui::MouseEvent& event) override;
  void OnMouseEntered(const ui::MouseEvent& event) override;
  void OnMouseExited(const ui::MouseEvent& event) override;

  void OnPaint(gfx::Canvas* canvas) override;
  void SchedulePaintInRect(const gfx::Rect& rect) override;
  bool AcceleratorPressed(const ui::Accelerator& accelerator) override;

  void OnThemeChanged() override;

  void OnAccessibilityEvent(ax::mojom::Event event_type) override;

  // OnBoundsChanged.
  bool did_change_bounds_;
  gfx::Rect new_bounds_;

  // Layout.
  bool did_layout_ = false;

  // MouseEvent.
  int last_mouse_event_type_;
  gfx::Point location_;
  bool received_mouse_enter_;
  bool received_mouse_exit_;
  bool delete_on_pressed_ = false;

  // Painting.
  std::vector<gfx::Rect> scheduled_paint_rects_;
  bool did_paint_ = false;

  // Accelerators.
  std::map<ui::Accelerator, int> accelerator_count_map_;

  // Native theme.
  const ui::NativeTheme* native_theme_ = nullptr;

  // Value to return from CanProcessEventsWithinSubtree().
  bool can_process_events_within_subtree_ = true;

  // Accessibility events
  ax::mojom::Event last_a11y_event_;
};

////////////////////////////////////////////////////////////////////////////////
// Layout
////////////////////////////////////////////////////////////////////////////////

TEST_F(ViewTest, LayoutCalledInvalidateAndOriginChanges) {
  TestView parent;
  TestView* child = new TestView;
  gfx::Rect parent_rect(0, 0, 100, 100);
  parent.SetBoundsRect(parent_rect);

  parent.Reset();
  // |AddChildView| invalidates parent's layout.
  parent.AddChildView(child);
  // Change rect so that only rect's origin is affected.
  parent.SetBoundsRect(parent_rect + gfx::Vector2d(10, 0));

  EXPECT_TRUE(parent.did_layout_);

  // After child layout is invalidated, parent and child must be laid out
  // during parent->BoundsChanged(...) call.
  parent.Reset();
  child->Reset();

  child->InvalidateLayout();
  parent.SetBoundsRect(parent_rect + gfx::Vector2d(20, 0));
  EXPECT_TRUE(parent.did_layout_);
  EXPECT_TRUE(child->did_layout_);
}

// Tests that SizeToPreferredSize will trigger a Layout if the size has changed
// or if layout is marked invalid.
TEST_F(ViewTest, SizeToPreferredSizeInducesLayout) {
  TestView example_view;
  example_view.SetPreferredSize(gfx::Size(101, 102));
  example_view.SizeToPreferredSize();
  EXPECT_TRUE(example_view.did_layout_);

  example_view.Reset();
  example_view.SizeToPreferredSize();
  EXPECT_FALSE(example_view.did_layout_);

  example_view.InvalidateLayout();
  example_view.SizeToPreferredSize();
  EXPECT_TRUE(example_view.did_layout_);
}

////////////////////////////////////////////////////////////////////////////////
// OnBoundsChanged
////////////////////////////////////////////////////////////////////////////////

void TestView::OnAccessibilityEvent(ax::mojom::Event event_type) {
  last_a11y_event_ = event_type;
}

TEST_F(ViewTest, OnBoundsChangedFiresA11yEvent) {
  TestView v;

  // Should change when scaled or moved.
  gfx::Rect initial(0, 0, 200, 200);
  gfx::Rect scaled(0, 0, 250, 250);
  gfx::Rect moved(100, 100, 250, 250);

  v.last_a11y_event_ = ax::mojom::Event::kNone;
  v.SetBoundsRect(initial);
  EXPECT_EQ(v.last_a11y_event_, ax::mojom::Event::kLocationChanged);

  v.last_a11y_event_ = ax::mojom::Event::kNone;
  v.SetBoundsRect(scaled);
  EXPECT_EQ(v.last_a11y_event_, ax::mojom::Event::kLocationChanged);

  v.last_a11y_event_ = ax::mojom::Event::kNone;
  v.SetBoundsRect(moved);
  EXPECT_EQ(v.last_a11y_event_, ax::mojom::Event::kLocationChanged);
}

void TestView::OnBoundsChanged(const gfx::Rect& previous_bounds) {
  did_change_bounds_ = true;
  new_bounds_ = bounds();
}

TEST_F(ViewTest, OnBoundsChanged) {
  TestView v;

  gfx::Rect prev_rect(0, 0, 200, 200);
  gfx::Rect new_rect(100, 100, 250, 250);

  v.SetBoundsRect(prev_rect);
  v.Reset();
  v.SetBoundsRect(new_rect);

  EXPECT_TRUE(v.did_change_bounds_);
  EXPECT_EQ(v.new_bounds_, new_rect);
  EXPECT_EQ(v.bounds(), new_rect);
}

////////////////////////////////////////////////////////////////////////////////
// MouseEvent
////////////////////////////////////////////////////////////////////////////////

bool TestView::OnMousePressed(const ui::MouseEvent& event) {
  last_mouse_event_type_ = event.type();
  location_.SetPoint(event.x(), event.y());
  if (delete_on_pressed_)
    delete this;
  return true;
}

bool TestView::OnMouseDragged(const ui::MouseEvent& event) {
  last_mouse_event_type_ = event.type();
  location_.SetPoint(event.x(), event.y());
  return true;
}

void TestView::OnMouseReleased(const ui::MouseEvent& event) {
  last_mouse_event_type_ = event.type();
  location_.SetPoint(event.x(), event.y());
}

void TestView::OnMouseEntered(const ui::MouseEvent& event) {
  received_mouse_enter_ = true;
}

void TestView::OnMouseExited(const ui::MouseEvent& event) {
  received_mouse_exit_ = true;
}

TEST_F(ViewTest, MouseEvent) {
  TestView* v1 = new TestView();
  v1->SetBoundsRect(gfx::Rect(0, 0, 300, 300));

  TestView* v2 = new TestView();
  v2->SetBoundsRect(gfx::Rect(100, 100, 100, 100));

  std::unique_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget->Init(params);
  internal::RootView* root =
      static_cast<internal::RootView*>(widget->GetRootView());

  root->AddChildView(v1);
  v1->AddChildView(v2);

  v1->Reset();
  v2->Reset();

  gfx::Point p1(110, 120);
  ui::MouseEvent pressed(ui::ET_MOUSE_PRESSED, p1, p1, ui::EventTimeForNow(),
                         ui::EF_LEFT_MOUSE_BUTTON, ui::EF_LEFT_MOUSE_BUTTON);
  root->OnMousePressed(pressed);
  EXPECT_EQ(v2->last_mouse_event_type_, ui::ET_MOUSE_PRESSED);
  EXPECT_EQ(v2->location_.x(), 10);
  EXPECT_EQ(v2->location_.y(), 20);
  // Make sure v1 did not receive the event
  EXPECT_EQ(v1->last_mouse_event_type_, 0);

  // Drag event out of bounds. Should still go to v2
  v1->Reset();
  v2->Reset();
  gfx::Point p2(50, 40);
  ui::MouseEvent dragged(ui::ET_MOUSE_DRAGGED, p2, p2, ui::EventTimeForNow(),
                         ui::EF_LEFT_MOUSE_BUTTON, 0);
  root->OnMouseDragged(dragged);
  EXPECT_EQ(v2->last_mouse_event_type_, ui::ET_MOUSE_DRAGGED);
  EXPECT_EQ(v2->location_.x(), -50);
  EXPECT_EQ(v2->location_.y(), -60);
  // Make sure v1 did not receive the event
  EXPECT_EQ(v1->last_mouse_event_type_, 0);

  // Releasted event out of bounds. Should still go to v2
  v1->Reset();
  v2->Reset();
  ui::MouseEvent released(ui::ET_MOUSE_RELEASED, gfx::Point(), gfx::Point(),
                          ui::EventTimeForNow(), 0, 0);
  root->OnMouseDragged(released);
  EXPECT_EQ(v2->last_mouse_event_type_, ui::ET_MOUSE_RELEASED);
  EXPECT_EQ(v2->location_.x(), -100);
  EXPECT_EQ(v2->location_.y(), -100);
  // Make sure v1 did not receive the event
  EXPECT_EQ(v1->last_mouse_event_type_, 0);

  widget->CloseNow();
}

// Confirm that a view can be deleted as part of processing a mouse press.
TEST_F(ViewTest, DeleteOnPressed) {
  TestView* v1 = new TestView();
  v1->SetBoundsRect(gfx::Rect(0, 0, 300, 300));

  TestView* v2 = new TestView();
  v2->SetBoundsRect(gfx::Rect(100, 100, 100, 100));

  v1->Reset();
  v2->Reset();

  std::unique_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget->Init(params);
  View* root = widget->GetRootView();

  root->AddChildView(v1);
  v1->AddChildView(v2);

  v2->delete_on_pressed_ = true;
  gfx::Point point(110, 120);
  ui::MouseEvent pressed(ui::ET_MOUSE_PRESSED, point, point,
                         ui::EventTimeForNow(), ui::EF_LEFT_MOUSE_BUTTON,
                         ui::EF_LEFT_MOUSE_BUTTON);
  root->OnMousePressed(pressed);
  EXPECT_TRUE(v1->children().empty());

  widget->CloseNow();
}

////////////////////////////////////////////////////////////////////////////////
// Painting
////////////////////////////////////////////////////////////////////////////////

void TestView::OnPaint(gfx::Canvas* canvas) {
  did_paint_ = true;
}

namespace {

// Helper class to create a Widget with standard parameters that is closed when
// the helper class goes out of scope.
class ScopedTestPaintWidget {
 public:
  explicit ScopedTestPaintWidget(const Widget::InitParams& params)
        : widget_(new Widget) {
    widget_->Init(params);
    widget_->GetRootView()->SetBounds(0, 0, 25, 26);
  }

  ~ScopedTestPaintWidget() {
    widget_->CloseNow();
  }

  Widget* operator->() { return widget_; }

 private:
  Widget* widget_;

  DISALLOW_COPY_AND_ASSIGN(ScopedTestPaintWidget);
};

}  // namespace

TEST_F(ViewTest, PaintEmptyView) {
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();

  // |v1| is empty.
  TestView* v1 = new TestView;
  v1->SetBounds(10, 11, 0, 1);
  root_view->AddChildView(v1);

  // |v11| is a child of an empty |v1|.
  TestView* v11 = new TestView;
  v11->SetBounds(3, 4, 6, 5);
  v1->AddChildView(v11);

  // |v2| is not.
  TestView* v2 = new TestView;
  v2->SetBounds(3, 4, 6, 5);
  root_view->AddChildView(v2);

  // Paint "everything".
  gfx::Rect first_paint(1, 1);
  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, first_paint, false),
      first_paint.size()));

  // The empty view has nothing to paint so it doesn't try build a cache, nor do
  // its children which would be clipped by its (empty) self.
  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v11->did_paint_);
  EXPECT_TRUE(v2->did_paint_);
}

TEST_F(ViewTest, PaintWithMovedViewUsesCache) {
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();
  TestView* v1 = new TestView;
  v1->SetBounds(10, 11, 12, 13);
  root_view->AddChildView(v1);

  // Paint everything once, since it has to build its cache. Then we can test
  // invalidation.
  gfx::Rect pixel_rect = gfx::Rect(1, 1);
  float device_scale_factor = 1.f;
  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->PaintFromPaintRoot(
      ui::PaintContext(list.get(), device_scale_factor, pixel_rect, false));
  EXPECT_TRUE(v1->did_paint_);
  v1->Reset();
  // The visual rects for (clip, drawing, transform) should be in layer space.
  gfx::Rect expected_visual_rect_in_layer_space(10, 11, 12, 13);
  int item_index = 3;
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index++));
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index++));
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index));

  // If invalidation doesn't intersect v1, we paint with the cache.
  list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->PaintFromPaintRoot(
      ui::PaintContext(list.get(), device_scale_factor, pixel_rect, false));
  EXPECT_FALSE(v1->did_paint_);
  v1->Reset();

  // If invalidation does intersect v1, we don't paint with the cache.
  list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->PaintFromPaintRoot(
      ui::PaintContext(list.get(), device_scale_factor, v1->bounds(), false));
  EXPECT_TRUE(v1->did_paint_);
  v1->Reset();

  // Moving the view should still use the cache when the invalidation doesn't
  // intersect v1.
  list = base::MakeRefCounted<cc::DisplayItemList>();
  v1->SetX(9);
  root_view->PaintFromPaintRoot(
      ui::PaintContext(list.get(), device_scale_factor, pixel_rect, false));
  EXPECT_FALSE(v1->did_paint_);
  v1->Reset();
  item_index = 3;
  expected_visual_rect_in_layer_space.SetRect(9, 11, 12, 13);
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index++));
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index++));
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index));

  // Moving the view should not use the cache when painting without
  // invalidation.
  list = base::MakeRefCounted<cc::DisplayItemList>();
  v1->SetX(8);
  root_view->PaintFromPaintRoot(ui::PaintContext(
      ui::PaintContext(list.get(), device_scale_factor, pixel_rect, false),
      ui::PaintContext::CLONE_WITHOUT_INVALIDATION));
  EXPECT_TRUE(v1->did_paint_);
  v1->Reset();
  item_index = 3;
  expected_visual_rect_in_layer_space.SetRect(8, 11, 12, 13);
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index++));
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index++));
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index));
}

TEST_F(ViewTest, PaintWithMovedViewUsesCacheInRTL) {
  base::test::ScopedRestoreICUDefaultLocale scoped_locale_("he");
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();
  TestView* v1 = new TestView;
  v1->SetBounds(10, 11, 12, 13);
  root_view->AddChildView(v1);

  // Paint everything once, since it has to build its cache. Then we can test
  // invalidation.
  gfx::Rect pixel_rect = gfx::Rect(1, 1);
  float device_scale_factor = 1.f;
  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->PaintFromPaintRoot(
      ui::PaintContext(list.get(), device_scale_factor, pixel_rect, false));
  EXPECT_TRUE(v1->did_paint_);
  v1->Reset();
  // The visual rects for (clip, drawing, transform) should be in layer space.
  // x: 25 - 10(x) - 12(width) = 3
  gfx::Rect expected_visual_rect_in_layer_space(3, 11, 12, 13);
  int item_index = 3;
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index++));
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index++));
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index));

  // If invalidation doesn't intersect v1, we paint with the cache.
  list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->PaintFromPaintRoot(
      ui::PaintContext(list.get(), device_scale_factor, pixel_rect, false));
  EXPECT_FALSE(v1->did_paint_);
  v1->Reset();

  // If invalidation does intersect v1, we don't paint with the cache.
  list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->PaintFromPaintRoot(
      ui::PaintContext(list.get(), device_scale_factor, v1->bounds(), false));
  EXPECT_TRUE(v1->did_paint_);
  v1->Reset();

  // Moving the view should still use the cache when the invalidation doesn't
  // intersect v1.
  list = base::MakeRefCounted<cc::DisplayItemList>();
  v1->SetX(9);
  root_view->PaintFromPaintRoot(
      ui::PaintContext(list.get(), device_scale_factor, pixel_rect, false));
  EXPECT_FALSE(v1->did_paint_);
  v1->Reset();
  item_index = 3;
  // x: 25 - 9(x) - 12(width) = 4
  expected_visual_rect_in_layer_space.SetRect(4, 11, 12, 13);
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index++));
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index++));
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index));

  // Moving the view should not use the cache when painting without
  // invalidation.
  list = base::MakeRefCounted<cc::DisplayItemList>();
  v1->SetX(8);
  root_view->PaintFromPaintRoot(ui::PaintContext(
      ui::PaintContext(list.get(), device_scale_factor, pixel_rect, false),
      ui::PaintContext::CLONE_WITHOUT_INVALIDATION));
  EXPECT_TRUE(v1->did_paint_);
  v1->Reset();
  item_index = 3;
  // x: 25 - 8(x) - 12(width) = 5
  expected_visual_rect_in_layer_space.SetRect(5, 11, 12, 13);
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index++));
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index++));
  EXPECT_EQ(expected_visual_rect_in_layer_space,
            list->VisualRectForTesting(item_index));
}

TEST_F(ViewTest, PaintWithUnknownInvalidation) {
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();

  TestView* v1 = new TestView;
  v1->SetBounds(10, 11, 12, 13);
  root_view->AddChildView(v1);

  TestView* v2 = new TestView;
  v2->SetBounds(3, 4, 6, 5);
  v1->AddChildView(v2);

  // Paint everything once, since it has to build its cache. Then we can test
  // invalidation.
  gfx::Rect first_paint(1, 1);
  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->PaintFromPaintRoot(
      ui::PaintContext(list.get(), 1.f, first_paint, false));
  v1->Reset();
  v2->Reset();

  gfx::Rect paint_area(1, 1);
  gfx::Rect root_area(root_view->size());
  list = base::MakeRefCounted<cc::DisplayItemList>();

  // With a known invalidation, v1 and v2 are not painted.
  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
  root_view->PaintFromPaintRoot(
      ui::PaintContext(list.get(), 1.f, paint_area, false));
  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);

  // With unknown invalidation, v1 and v2 are painted.
  root_view->PaintFromPaintRoot(
      ui::PaintContext(ui::PaintContext(list.get(), 1.f, paint_area, false),
                       ui::PaintContext::CLONE_WITHOUT_INVALIDATION));
  EXPECT_TRUE(v1->did_paint_);
  EXPECT_TRUE(v2->did_paint_);
}

TEST_F(ViewTest, PaintContainsChildren) {
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();

  TestView* v1 = new TestView;
  v1->SetBounds(10, 11, 12, 13);
  root_view->AddChildView(v1);

  TestView* v2 = new TestView;
  v2->SetBounds(3, 4, 6, 5);
  v1->AddChildView(v2);

  // Paint everything once, since it has to build its cache. Then we can test
  // invalidation.
  gfx::Rect first_paint(1, 1);
  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, first_paint, false),
      root_view->size()));
  v1->Reset();
  v2->Reset();

  gfx::Rect paint_area(25, 26);
  gfx::Rect root_area(root_view->size());
  list = base::MakeRefCounted<cc::DisplayItemList>();

  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, paint_area, false), root_view->size()));
  EXPECT_TRUE(v1->did_paint_);
  EXPECT_TRUE(v2->did_paint_);
}

TEST_F(ViewTest, PaintContainsChildrenInRTL) {
  base::test::ScopedRestoreICUDefaultLocale scoped_locale_("he");
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();

  TestView* v1 = new TestView;
  v1->SetBounds(10, 11, 12, 13);
  root_view->AddChildView(v1);

  TestView* v2 = new TestView;
  v2->SetBounds(3, 4, 6, 5);
  v1->AddChildView(v2);

  // Verify where the layers actually appear.
  v1->SetPaintToLayer();
  // x: 25 - 10(x) - 12(width) = 3
  EXPECT_EQ(gfx::Rect(3, 11, 12, 13), v1->layer()->bounds());
  v1->DestroyLayer();

  v2->SetPaintToLayer();
  // x: 25 - 10(parent x) - 3(x) - 6(width) = 6
  EXPECT_EQ(gfx::Rect(6, 15, 6, 5), v2->layer()->bounds());
  v2->DestroyLayer();

  // Paint everything once, since it has to build its cache. Then we can test
  // invalidation.
  gfx::Rect first_paint(1, 1);
  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, first_paint, false),
      root_view->size()));
  v1->Reset();
  v2->Reset();

  gfx::Rect paint_area(25, 26);
  gfx::Rect root_area(root_view->size());
  list = base::MakeRefCounted<cc::DisplayItemList>();

  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, paint_area, false), root_view->size()));
  EXPECT_TRUE(v1->did_paint_);
  EXPECT_TRUE(v2->did_paint_);
}

TEST_F(ViewTest, PaintIntersectsChildren) {
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();

  TestView* v1 = new TestView;
  v1->SetBounds(10, 11, 12, 13);
  root_view->AddChildView(v1);

  TestView* v2 = new TestView;
  v2->SetBounds(3, 4, 6, 5);
  v1->AddChildView(v2);

  // Paint everything once, since it has to build its cache. Then we can test
  // invalidation.
  gfx::Rect first_paint(1, 1);
  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, first_paint, false),
      root_view->size()));
  v1->Reset();
  v2->Reset();

  gfx::Rect paint_area(9, 10, 5, 6);
  gfx::Rect root_area(root_view->size());
  list = base::MakeRefCounted<cc::DisplayItemList>();

  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, paint_area, false), root_view->size()));
  EXPECT_TRUE(v1->did_paint_);
  EXPECT_TRUE(v2->did_paint_);
}

TEST_F(ViewTest, PaintIntersectsChildrenInRTL) {
  base::test::ScopedRestoreICUDefaultLocale scoped_locale_("he");
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();

  TestView* v1 = new TestView;
  v1->SetBounds(10, 11, 12, 13);
  root_view->AddChildView(v1);

  TestView* v2 = new TestView;
  v2->SetBounds(3, 4, 6, 5);
  v1->AddChildView(v2);

  // Verify where the layers actually appear.
  v1->SetPaintToLayer();
  // x: 25 - 10(x) - 12(width) = 3
  EXPECT_EQ(gfx::Rect(3, 11, 12, 13), v1->layer()->bounds());
  v1->DestroyLayer();

  v2->SetPaintToLayer();
  // x: 25 - 10(parent x) - 3(x) - 6(width) = 6
  EXPECT_EQ(gfx::Rect(6, 15, 6, 5), v2->layer()->bounds());
  v2->DestroyLayer();

  // Paint everything once, since it has to build its cache. Then we can test
  // invalidation.
  gfx::Rect first_paint(1, 1);
  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, first_paint, false),
      root_view->size()));
  v1->Reset();
  v2->Reset();

  gfx::Rect paint_area(2, 10, 5, 6);
  gfx::Rect root_area(root_view->size());
  list = base::MakeRefCounted<cc::DisplayItemList>();

  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, paint_area, false), root_view->size()));
  EXPECT_TRUE(v1->did_paint_);
  EXPECT_TRUE(v2->did_paint_);
}

TEST_F(ViewTest, PaintIntersectsChildButNotGrandChild) {
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();

  TestView* v1 = new TestView;
  v1->SetBounds(10, 11, 12, 13);
  root_view->AddChildView(v1);

  TestView* v2 = new TestView;
  v2->SetBounds(3, 4, 6, 5);
  v1->AddChildView(v2);

  // Paint everything once, since it has to build its cache. Then we can test
  // invalidation.
  gfx::Rect first_paint(1, 1);
  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, first_paint, false),
      root_view->size()));
  v1->Reset();
  v2->Reset();

  gfx::Rect paint_area(9, 10, 2, 3);
  gfx::Rect root_area(root_view->size());
  list = base::MakeRefCounted<cc::DisplayItemList>();

  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, paint_area, false), root_view->size()));
  EXPECT_TRUE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
}

TEST_F(ViewTest, PaintIntersectsChildButNotGrandChildInRTL) {
  base::test::ScopedRestoreICUDefaultLocale scoped_locale_("he");
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();

  TestView* v1 = new TestView;
  v1->SetBounds(10, 11, 12, 13);
  root_view->AddChildView(v1);

  TestView* v2 = new TestView;
  v2->SetBounds(3, 4, 6, 5);
  v1->AddChildView(v2);

  // Verify where the layers actually appear.
  v1->SetPaintToLayer();
  // x: 25 - 10(x) - 12(width) = 3
  EXPECT_EQ(gfx::Rect(3, 11, 12, 13), v1->layer()->bounds());
  v1->DestroyLayer();

  v2->SetPaintToLayer();
  // x: 25 - 10(parent x) - 3(x) - 6(width) = 6
  EXPECT_EQ(gfx::Rect(6, 15, 6, 5), v2->layer()->bounds());
  v2->DestroyLayer();

  // Paint everything once, since it has to build its cache. Then we can test
  // invalidation.
  gfx::Rect first_paint(1, 1);
  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, first_paint, false),
      root_view->size()));
  v1->Reset();
  v2->Reset();

  gfx::Rect paint_area(2, 10, 2, 3);
  gfx::Rect root_area(root_view->size());
  list = base::MakeRefCounted<cc::DisplayItemList>();

  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, paint_area, false), root_view->size()));
  EXPECT_TRUE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
}

TEST_F(ViewTest, PaintIntersectsNoChildren) {
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();

  TestView* v1 = new TestView;
  v1->SetBounds(10, 11, 12, 13);
  root_view->AddChildView(v1);

  TestView* v2 = new TestView;
  v2->SetBounds(3, 4, 6, 5);
  v1->AddChildView(v2);

  // Paint everything once, since it has to build its cache. Then we can test
  // invalidation.
  gfx::Rect first_paint(1, 1);
  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, first_paint, false),
      root_view->size()));
  v1->Reset();
  v2->Reset();

  gfx::Rect paint_area(9, 10, 2, 1);
  gfx::Rect root_area(root_view->size());
  list = base::MakeRefCounted<cc::DisplayItemList>();

  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, paint_area, false), root_view->size()));
  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
}

TEST_F(ViewTest, PaintIntersectsNoChildrenInRTL) {
  base::test::ScopedRestoreICUDefaultLocale scoped_locale_("he");
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();

  TestView* v1 = new TestView;
  v1->SetBounds(10, 11, 12, 13);
  root_view->AddChildView(v1);

  TestView* v2 = new TestView;
  v2->SetBounds(3, 4, 6, 5);
  v1->AddChildView(v2);

  // Verify where the layers actually appear.
  v1->SetPaintToLayer();
  // x: 25 - 10(x) - 12(width) = 3
  EXPECT_EQ(gfx::Rect(3, 11, 12, 13), v1->layer()->bounds());
  v1->DestroyLayer();

  v2->SetPaintToLayer();
  // x: 25 - 10(parent x) - 3(x) - 6(width) = 6
  EXPECT_EQ(gfx::Rect(6, 15, 6, 5), v2->layer()->bounds());
  v2->DestroyLayer();

  // Paint everything once, since it has to build its cache. Then we can test
  // invalidation.
  gfx::Rect first_paint(1, 1);
  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, first_paint, false),
      root_view->size()));
  v1->Reset();
  v2->Reset();

  gfx::Rect paint_area(2, 10, 2, 1);
  gfx::Rect root_area(root_view->size());
  list = base::MakeRefCounted<cc::DisplayItemList>();

  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, paint_area, false), root_view->size()));
  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
}

TEST_F(ViewTest, PaintIntersectsOneChild) {
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();

  TestView* v1 = new TestView;
  v1->SetBounds(10, 11, 12, 13);
  root_view->AddChildView(v1);

  TestView* v2 = new TestView;
  v2->SetBounds(3, 4, 6, 5);
  root_view->AddChildView(v2);

  // Paint everything once, since it has to build its cache. Then we can test
  // invalidation.
  gfx::Rect first_paint(1, 1);
  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, first_paint, false),
      root_view->size()));
  v1->Reset();
  v2->Reset();

  // Intersects with the second child only.
  gfx::Rect paint_area(3, 3, 1, 2);
  gfx::Rect root_area(root_view->size());
  list = base::MakeRefCounted<cc::DisplayItemList>();

  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, paint_area, false), root_view->size()));
  EXPECT_FALSE(v1->did_paint_);
  EXPECT_TRUE(v2->did_paint_);

  // Intersects with the first child only.
  paint_area = gfx::Rect(20, 10, 1, 2);

  v1->Reset();
  v2->Reset();
  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, paint_area, false), root_view->size()));
  EXPECT_TRUE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
}

TEST_F(ViewTest, PaintIntersectsOneChildInRTL) {
  base::test::ScopedRestoreICUDefaultLocale scoped_locale_("he");
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();

  TestView* v1 = new TestView;
  v1->SetBounds(10, 11, 12, 13);
  root_view->AddChildView(v1);

  TestView* v2 = new TestView;
  v2->SetBounds(3, 4, 6, 5);
  root_view->AddChildView(v2);

  // Verify where the layers actually appear.
  v1->SetPaintToLayer();
  // x: 25 - 10(x) - 12(width) = 3
  EXPECT_EQ(gfx::Rect(3, 11, 12, 13), v1->layer()->bounds());
  v1->DestroyLayer();

  v2->SetPaintToLayer();
  // x: 25 - 3(x) - 6(width) = 16
  EXPECT_EQ(gfx::Rect(16, 4, 6, 5), v2->layer()->bounds());
  v2->DestroyLayer();

  // Paint everything once, since it has to build its cache. Then we can test
  // invalidation.
  gfx::Rect first_paint(1, 1);
  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, first_paint, false),
      root_view->size()));
  v1->Reset();
  v2->Reset();

  // Intersects with the first child only.
  gfx::Rect paint_area(3, 10, 1, 2);
  gfx::Rect root_area(root_view->size());
  list = base::MakeRefCounted<cc::DisplayItemList>();

  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, paint_area, false), root_view->size()));
  EXPECT_TRUE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);

  // Intersects with the second child only.
  paint_area = gfx::Rect(21, 3, 1, 2);

  v1->Reset();
  v2->Reset();
  EXPECT_FALSE(v1->did_paint_);
  EXPECT_FALSE(v2->did_paint_);
  root_view->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, paint_area, false), root_view->size()));
  EXPECT_FALSE(v1->did_paint_);
  EXPECT_TRUE(v2->did_paint_);
}

TEST_F(ViewTest, PaintInPromotedToLayer) {
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();

  TestView* v1 = new TestView;
  v1->SetPaintToLayer();
  v1->SetBounds(10, 11, 12, 13);
  root_view->AddChildView(v1);

  TestView* v2 = new TestView;
  v2->SetBounds(3, 4, 6, 5);
  v1->AddChildView(v2);

  // Paint everything once, since it has to build its cache. Then we can test
  // invalidation.
  gfx::Rect first_paint(1, 1);
  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  v1->Paint(PaintInfo::CreateRootPaintInfo(
      ui::PaintContext(list.get(), 1.f, first_paint, false), v1->size()));
  v1->Reset();
  v2->Reset();

  {
    gfx::Rect paint_area(25, 26);
    gfx::Rect view_area(root_view->size());
    auto list = base::MakeRefCounted<cc::DisplayItemList>();

    // The promoted views are not painted as they are separate paint roots.
    root_view->Paint(PaintInfo::CreateRootPaintInfo(
        ui::PaintContext(list.get(), 1.f, paint_area, false),
        root_view->size()));
    EXPECT_FALSE(v1->did_paint_);
    EXPECT_FALSE(v2->did_paint_);
  }

  {
    gfx::Rect paint_area(1, 1);
    gfx::Rect view_area(v1->size());
    auto list = base::MakeRefCounted<cc::DisplayItemList>();

    // The |v1| view is painted. If it used its offset incorrect, it would think
    // its at (10,11) instead of at (0,0) since it is the paint root.
    v1->Paint(PaintInfo::CreateRootPaintInfo(
        ui::PaintContext(list.get(), 1.f, paint_area, false), v1->size()));
    EXPECT_TRUE(v1->did_paint_);
    EXPECT_FALSE(v2->did_paint_);
  }

  v1->Reset();

  {
    gfx::Rect paint_area(3, 3, 1, 2);
    gfx::Rect view_area(v1->size());
    auto list = base::MakeRefCounted<cc::DisplayItemList>();

    // The |v2| view is painted also. If it used its offset incorrect, it would
    // think its at (13,15) instead of at (3,4) since |v1| is the paint root.
    v1->Paint(PaintInfo::CreateRootPaintInfo(
        ui::PaintContext(list.get(), 1.f, paint_area, false), v1->size()));
    EXPECT_TRUE(v1->did_paint_);
    EXPECT_TRUE(v2->did_paint_);
  }
}

// A derived class for testing paint.
class TestPaintView : public TestView {
 public:
  TestPaintView() : canvas_bounds_(gfx::Rect()) {}
  ~TestPaintView() override = default;

  void OnPaint(gfx::Canvas* canvas) override {
    did_paint_ = true;
    // Get the bounds from the canvas this view paints to.
    EXPECT_TRUE(canvas->GetClipBounds(&canvas_bounds_));
  }

  gfx::Rect canvas_bounds() const { return canvas_bounds_; }

 private:
  gfx::Rect canvas_bounds_;
};

TEST_F(ViewTest, PaintLocalBounds) {
  ScopedTestPaintWidget widget(CreateParams(Widget::InitParams::TYPE_POPUP));
  View* root_view = widget->GetRootView();
  // Make |root_view|'s bounds larger so |v1|'s visible bounds is not clipped by
  // |root_view|.
  root_view->SetBounds(0, 0, 200, 200);

  TestPaintView* v1 = new TestPaintView;
  v1->SetPaintToLayer();

  // Set bounds for |v1| such that it has an offset to its parent and only part
  // of it is visible. The visible bounds does not intersect with |root_view|'s
  // bounds.
  v1->SetBounds(0, -1000, 100, 1100);
  root_view->AddChildView(v1);
  EXPECT_EQ(gfx::Rect(0, 0, 100, 1100), v1->GetLocalBounds());
  EXPECT_EQ(gfx::Rect(0, 1000, 100, 100), v1->GetVisibleBounds());

  auto list = base::MakeRefCounted<cc::DisplayItemList>();
  ui::PaintContext context(list.get(), 1.f, gfx::Rect(), false);

  v1->Paint(PaintInfo::CreateRootPaintInfo(context, gfx::Size()));
  EXPECT_TRUE(v1->did_paint_);

  // Check that the canvas produced by |v1| for paint contains all of |v1|'s
  // visible bounds.
  EXPECT_TRUE(v1->canvas_bounds().Contains(v1->GetVisibleBounds()));
}

void TestView::SchedulePaintInRect(const gfx::Rect& rect) {
  scheduled_paint_rects_.push_back(rect);
  View::SchedulePaintInRect(rect);
}

namespace {

void RotateCounterclockwise(gfx::Transform* transform) {
  transform->matrix().set3x3(0, -1, 0,
                             1,  0, 0,
                             0,  0, 1);
}

void RotateClockwise(gfx::Transform* transform) {
  transform->matrix().set3x3( 0, 1, 0,
                             -1, 0, 0,
                              0, 0, 1);
}

}  // namespace

// Tests the correctness of the rect-based targeting algorithm implemented in
// View::GetEventHandlerForRect(). See http://goo.gl/3Jp2BD for a description
// of rect-based targeting.
TEST_F(ViewTest, GetEventHandlerForRect) {
  Widget* widget = new Widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  widget->Init(params);
  View* root_view = widget->GetRootView();
  root_view->SetBoundsRect(gfx::Rect(0, 0, 500, 500));

  // Have this hierarchy of views (the coordinates here are all in
  // the root view's coordinate space):
  // v1 (0, 0, 100, 100)
  // v2 (150, 0, 250, 100)
  // v3 (0, 200, 150, 100)
  //     v31 (10, 210, 80, 80)
  //     v32 (110, 210, 30, 80)
  // v4 (300, 200, 100, 100)
  //     v41 (310, 210, 80, 80)
  //         v411 (370, 275, 10, 5)
  // v5 (450, 197, 30, 36)
  //     v51 (450, 200, 30, 30)

  // The coordinates used for SetBounds are in parent coordinates.

  TestView* v1 = new TestView;
  v1->SetBounds(0, 0, 100, 100);
  root_view->AddChildView(v1);

  TestView* v2 = new TestView;
  v2->SetBounds(150, 0, 250, 100);
  root_view->AddChildView(v2);

  TestView* v3 = new TestView;
  v3->SetBounds(0, 200, 150, 100);
  root_view->AddChildView(v3);

  TestView* v4 = new TestView;
  v4->SetBounds(300, 200, 100, 100);
  root_view->AddChildView(v4);

  TestView* v31 = new TestView;
  v31->SetBounds(10, 10, 80, 80);
  v3->AddChildView(v31);

  TestView* v32 = new TestView;
  v32->SetBounds(110, 10, 30, 80);
  v3->AddChildView(v32);

  TestView* v41 = new TestView;
  v41->SetBounds(10, 10, 80, 80);
  v4->AddChildView(v41);

  TestView* v411 = new TestView;
  v411->SetBounds(60, 65, 10, 5);
  v41->AddChildView(v411);

  TestView* v5 = new TestView;
  v5->SetBounds(450, 197, 30, 36);
  root_view->AddChildView(v5);

  TestView* v51 = new TestView;
  v51->SetBounds(0, 3, 30, 30);
  v5->AddChildView(v51);

  // |touch_rect| does not intersect any descendant view of |root_view|.
  gfx::Rect touch_rect(105, 105, 30, 45);
  View* result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(root_view, result_view);
  result_view = nullptr;

  // Covers |v1| by at least 60%.
  touch_rect.SetRect(15, 15, 100, 100);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v1, result_view);
  result_view = nullptr;

  // Intersects |v1| but does not cover it by at least 60%. The center
  // of |touch_rect| is within |v1|.
  touch_rect.SetRect(50, 50, 5, 10);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v1, result_view);
  result_view = nullptr;

  // Intersects |v1| but does not cover it by at least 60%. The center
  // of |touch_rect| is not within |v1|.
  touch_rect.SetRect(95, 96, 21, 22);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(root_view, result_view);
  result_view = nullptr;

  // Intersects |v1| and |v2|, but only covers |v2| by at least 60%.
  touch_rect.SetRect(95, 10, 300, 120);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v2, result_view);
  result_view = nullptr;

  // Covers both |v1| and |v2| by at least 60%, but the center point
  // of |touch_rect| is closer to the center point of |v2|.
  touch_rect.SetRect(20, 20, 400, 100);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v2, result_view);
  result_view = nullptr;

  // Covers both |v1| and |v2| by at least 60%, but the center point
  // of |touch_rect| is closer to the center point of |v1|.
  touch_rect.SetRect(-700, -15, 1050, 110);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v1, result_view);
  result_view = nullptr;

  // A mouse click within |v1| will target |v1|.
  touch_rect.SetRect(15, 15, 1, 1);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v1, result_view);
  result_view = nullptr;

  // Intersects |v3| and |v31| by at least 60% and the center point
  // of |touch_rect| is closer to the center point of |v31|.
  touch_rect.SetRect(0, 200, 110, 100);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v31, result_view);
  result_view = nullptr;

  // Intersects |v3| and |v31|, but neither by at least 60%. The
  // center point of |touch_rect| lies within |v31|.
  touch_rect.SetRect(80, 280, 15, 15);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v31, result_view);
  result_view = nullptr;

  // Covers |v3|, |v31|, and |v32| all by at least 60%, and the
  // center point of |touch_rect| is closest to the center point
  // of |v32|.
  touch_rect.SetRect(0, 200, 200, 100);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v32, result_view);
  result_view = nullptr;

  // Intersects all of |v3|, |v31|, and |v32|, but only covers
  // |v31| and |v32| by at least 60%. The center point of
  // |touch_rect| is closest to the center point of |v32|.
  touch_rect.SetRect(30, 225, 180, 115);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v32, result_view);
  result_view = nullptr;

  // A mouse click at the corner of |v3| will target |v3|.
  touch_rect.SetRect(0, 200, 1, 1);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v3, result_view);
  result_view = nullptr;

  // A mouse click within |v32| will target |v32|.
  touch_rect.SetRect(112, 211, 1, 1);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v32, result_view);
  result_view = nullptr;

  // Covers all of |v4|, |v41|, and |v411| by at least 60%.
  // The center point of |touch_rect| is equally close to
  // the center points of |v4| and |v41|.
  touch_rect.SetRect(310, 210, 80, 80);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v41, result_view);
  result_view = nullptr;

  // Intersects all of |v4|, |v41|, and |v411| but only covers
  // |v411| by at least 60%.
  touch_rect.SetRect(370, 275, 7, 5);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v411, result_view);
  result_view = nullptr;

  // Intersects |v4| and |v41| but covers neither by at least 60%.
  // The center point of |touch_rect| is equally close to the center
  // points of |v4| and |v41|.
  touch_rect.SetRect(345, 245, 7, 7);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v41, result_view);
  result_view = nullptr;

  // Intersects all of |v4|, |v41|, and |v411| and covers none of
  // them by at least 60%. The center point of |touch_rect| lies
  // within |v411|.
  touch_rect.SetRect(368, 272, 4, 6);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v411, result_view);
  result_view = nullptr;

  // Intersects all of |v4|, |v41|, and |v411| and covers none of
  // them by at least 60%. The center point of |touch_rect| lies
  // within |v41|.
  touch_rect.SetRect(365, 270, 7, 7);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v41, result_view);
  result_view = nullptr;

  // Intersects all of |v4|, |v41|, and |v411| and covers none of
  // them by at least 60%. The center point of |touch_rect| lies
  // within |v4|.
  touch_rect.SetRect(205, 275, 200, 2);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v4, result_view);
  result_view = nullptr;

  // Intersects all of |v4|, |v41|, and |v411| but only covers
  // |v41| by at least 60%.
  touch_rect.SetRect(310, 210, 61, 66);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v41, result_view);
  result_view = nullptr;

  // A mouse click within |v411| will target |v411|.
  touch_rect.SetRect(372, 275, 1, 1);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v411, result_view);
  result_view = nullptr;

  // A mouse click within |v41| will target |v41|.
  touch_rect.SetRect(350, 215, 1, 1);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v41, result_view);
  result_view = nullptr;

  // Covers |v3|, |v4|, and all of their descendants by at
  // least 60%. The center point of |touch_rect| is closest
  // to the center point of |v32|.
  touch_rect.SetRect(0, 200, 400, 100);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v32, result_view);
  result_view = nullptr;

  // Intersects all of |v2|, |v3|, |v32|, |v4|, |v41|, and |v411|.
  // Covers |v2|, |v32|, |v4|, |v41|, and |v411| by at least 60%.
  // The center point of |touch_rect| is closest to the center
  // point of |root_view|.
  touch_rect.SetRect(110, 15, 375, 450);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(root_view, result_view);
  result_view = nullptr;

  // Covers all views (except |v5| and |v51|) by at least 60%. The
  // center point of |touch_rect| is equally close to the center
  // points of |v2| and |v32|. One is not a descendant of the other,
  // so in this case the view selected is arbitrary (i.e.,
  // it depends only on the ordering of nodes in the views
  // hierarchy).
  touch_rect.SetRect(0, 0, 400, 300);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v32, result_view);
  result_view = nullptr;

  // Covers |v5| and |v51| by at least 60%, and the center point of
  // the touch is located within both views. Since both views share
  // the same center point, the child view should be selected.
  touch_rect.SetRect(440, 190, 40, 40);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v51, result_view);
  result_view = nullptr;

  // Covers |v5| and |v51| by at least 60%, but the center point of
  // the touch is not located within either view. Since both views
  // share the same center point, the child view should be selected.
  touch_rect.SetRect(455, 187, 60, 60);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v51, result_view);
  result_view = nullptr;

  // Covers neither |v5| nor |v51| by at least 60%, but the center
  // of the touch is located within |v51|.
  touch_rect.SetRect(450, 197, 10, 10);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v51, result_view);
  result_view = nullptr;

  // Covers neither |v5| nor |v51| by at least 60% but intersects both.
  // The center point is located outside of both views.
  touch_rect.SetRect(433, 180, 24, 24);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(root_view, result_view);
  result_view = nullptr;

  // Only intersects |v5| but does not cover it by at least 60%. The
  // center point of the touch region is located within |v5|.
  touch_rect.SetRect(449, 196, 3, 3);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v5, result_view);
  result_view = nullptr;

  // A mouse click within |v5| (but not |v51|) should target |v5|.
  touch_rect.SetRect(462, 199, 1, 1);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v5, result_view);
  result_view = nullptr;

  // A mouse click |v5| and |v51| should target the child view.
  touch_rect.SetRect(452, 226, 1, 1);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v51, result_view);
  result_view = nullptr;

  // A mouse click on the center of |v5| and |v51| should target
  // the child view.
  touch_rect.SetRect(465, 215, 1, 1);
  result_view = root_view->GetEventHandlerForRect(touch_rect);
  EXPECT_EQ(v51, result_view);
  result_view = nullptr;

  widget->CloseNow();
}

// Tests that GetEventHandlerForRect() and GetTooltipHandlerForPoint() behave
// as expected when different views in the view hierarchy return false
// when CanProcessEventsWithinSubtree() is called.
TEST_F(ViewTest, CanProcessEventsWithinSubtree) {
  Widget* widget = new Widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  widget->Init(params);
  View* root_view = widget->GetRootView();
  root_view->SetBoundsRect(gfx::Rect(0, 0, 500, 500));

  // Have this hierarchy of views (the coords here are in the coordinate
  // space of the root view):
  // v (0, 0, 100, 100)
  //  - v_child (0, 0, 20, 30)
  //    - v_grandchild (5, 5, 5, 15)

  TestView* v = new TestView;
  v->SetBounds(0, 0, 100, 100);
  root_view->AddChildView(v);
  v->set_notify_enter_exit_on_child(true);

  TestView* v_child = new TestView;
  v_child->SetBounds(0, 0, 20, 30);
  v->AddChildView(v_child);

  TestView* v_grandchild = new TestView;
  v_grandchild->SetBounds(5, 5, 5, 15);
  v_child->AddChildView(v_grandchild);

  v->Reset();
  v_child->Reset();
  v_grandchild->Reset();

  // Define rects and points within the views in the hierarchy.
  gfx::Rect rect_in_v_grandchild(7, 7, 3, 3);
  gfx::Point point_in_v_grandchild(rect_in_v_grandchild.origin());
  gfx::Rect rect_in_v_child(12, 3, 5, 5);
  gfx::Point point_in_v_child(rect_in_v_child.origin());
  gfx::Rect rect_in_v(50, 50, 25, 30);
  gfx::Point point_in_v(rect_in_v.origin());

  // When all three views return true when CanProcessEventsWithinSubtree()
  // is called, targeting should behave as expected.

  View* result_view = root_view->GetEventHandlerForRect(rect_in_v_grandchild);
  EXPECT_EQ(v_grandchild, result_view);
  result_view = nullptr;
  result_view = root_view->GetTooltipHandlerForPoint(point_in_v_grandchild);
  EXPECT_EQ(v_grandchild, result_view);
  result_view = nullptr;

  result_view = root_view->GetEventHandlerForRect(rect_in_v_child);
  EXPECT_EQ(v_child, result_view);
  result_view = nullptr;
  result_view = root_view->GetTooltipHandlerForPoint(point_in_v_child);
  EXPECT_EQ(v_child, result_view);
  result_view = nullptr;

  result_view = root_view->GetEventHandlerForRect(rect_in_v);
  EXPECT_EQ(v, result_view);
  result_view = nullptr;
  result_view = root_view->GetTooltipHandlerForPoint(point_in_v);
  EXPECT_EQ(v, result_view);
  result_view = nullptr;

  // When |v_grandchild| returns false when CanProcessEventsWithinSubtree()
  // is called, then |v_grandchild| cannot be returned as a target.

  v_grandchild->set_can_process_events_within_subtree(false);

  result_view = root_view->GetEventHandlerForRect(rect_in_v_grandchild);
  EXPECT_EQ(v_child, result_view);
  result_view = nullptr;
  result_view = root_view->GetTooltipHandlerForPoint(point_in_v_grandchild);
  EXPECT_EQ(v_child, result_view);
  result_view = nullptr;

  result_view = root_view->GetEventHandlerForRect(rect_in_v_child);
  EXPECT_EQ(v_child, result_view);
  result_view = nullptr;
  result_view = root_view->GetTooltipHandlerForPoint(point_in_v_child);
  EXPECT_EQ(v_child, result_view);
  result_view = nullptr;

  result_view = root_view->GetEventHandlerForRect(rect_in_v);
  EXPECT_EQ(v, result_view);
  result_view = nullptr;
  result_view = root_view->GetTooltipHandlerForPoint(point_in_v);
  EXPECT_EQ(v, result_view);

  // When |v_grandchild| returns false when CanProcessEventsWithinSubtree()
  // is called, then NULL should be returned as a target if we call
  // GetTooltipHandlerForPoint() with |v_grandchild| as the root of the
  // views tree. Note that the location must be in the coordinate space
  // of the root view (|v_grandchild| in this case), so use (1, 1).

  result_view = v_grandchild;
  result_view = v_grandchild->GetTooltipHandlerForPoint(gfx::Point(1, 1));
  EXPECT_EQ(nullptr, result_view);
  result_view = nullptr;

  // When |v_child| returns false when CanProcessEventsWithinSubtree()
  // is called, then neither |v_child| nor |v_grandchild| can be returned
  // as a target (|v| should be returned as the target for each case).

  v_grandchild->Reset();
  v_child->set_can_process_events_within_subtree(false);

  result_view = root_view->GetEventHandlerForRect(rect_in_v_grandchild);
  EXPECT_EQ(v, result_view);
  result_view = nullptr;
  result_view = root_view->GetTooltipHandlerForPoint(point_in_v_grandchild);
  EXPECT_EQ(v, result_view);
  result_view = nullptr;

  result_view = root_view->GetEventHandlerForRect(rect_in_v_child);
  EXPECT_EQ(v, result_view);
  result_view = nullptr;
  result_view = root_view->GetTooltipHandlerForPoint(point_in_v_child);
  EXPECT_EQ(v, result_view);
  result_view = nullptr;

  result_view = root_view->GetEventHandlerForRect(rect_in_v);
  EXPECT_EQ(v, result_view);
  result_view = nullptr;
  result_view = root_view->GetTooltipHandlerForPoint(point_in_v);
  EXPECT_EQ(v, result_view);
  result_view = nullptr;

  // When |v| returns false when CanProcessEventsWithinSubtree()
  // is called, then none of |v|, |v_child|, and |v_grandchild| can be returned
  // as a target (|root_view| should be returned as the target for each case).

  v_child->Reset();
  v->set_can_process_events_within_subtree(false);

  result_view = root_view->GetEventHandlerForRect(rect_in_v_grandchild);
  EXPECT_EQ(root_view, result_view);
  result_view = nullptr;
  result_view = root_view->GetTooltipHandlerForPoint(point_in_v_grandchild);
  EXPECT_EQ(root_view, result_view);
  result_view = nullptr;

  result_view = root_view->GetEventHandlerForRect(rect_in_v_child);
  EXPECT_EQ(root_view, result_view);
  result_view = nullptr;
  result_view = root_view->GetTooltipHandlerForPoint(point_in_v_child);
  EXPECT_EQ(root_view, result_view);
  result_view = nullptr;

  result_view = root_view->GetEventHandlerForRect(rect_in_v);
  EXPECT_EQ(root_view, result_view);
  result_view = nullptr;
  result_view = root_view->GetTooltipHandlerForPoint(point_in_v);
  EXPECT_EQ(root_view, result_view);

  widget->CloseNow();
}

TEST_F(ViewTest, NotifyEnterExitOnChild) {
  Widget* widget = new Widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  widget->Init(params);
  View* root_view = widget->GetRootView();
  root_view->SetBoundsRect(gfx::Rect(0, 0, 500, 500));

  // Have this hierarchy of views (the coords here are in root coord):
  // v1 (0, 0, 100, 100)
  //  - v11 (0, 0, 20, 30)
  //    - v111 (5, 5, 5, 15)
  //  - v12 (50, 10, 30, 90)
  //    - v121 (60, 20, 10, 10)
  // v2 (105, 0, 100, 100)
  //  - v21 (120, 10, 50, 20)

  TestView* v1 = new TestView;
  v1->SetBounds(0, 0, 100, 100);
  root_view->AddChildView(v1);
  v1->set_notify_enter_exit_on_child(true);

  TestView* v11 = new TestView;
  v11->SetBounds(0, 0, 20, 30);
  v1->AddChildView(v11);

  TestView* v111 = new TestView;
  v111->SetBounds(5, 5, 5, 15);
  v11->AddChildView(v111);

  TestView* v12 = new TestView;
  v12->SetBounds(50, 10, 30, 90);
  v1->AddChildView(v12);

  TestView* v121 = new TestView;
  v121->SetBounds(10, 10, 10, 10);
  v12->AddChildView(v121);

  TestView* v2 = new TestView;
  v2->SetBounds(105, 0, 100, 100);
  root_view->AddChildView(v2);

  TestView* v21 = new TestView;
  v21->SetBounds(15, 10, 50, 20);
  v2->AddChildView(v21);

  v1->Reset();
  v11->Reset();
  v111->Reset();
  v12->Reset();
  v121->Reset();
  v2->Reset();
  v21->Reset();

  // Move the mouse in v111.
  gfx::Point p1(6, 6);
  ui::MouseEvent move1(ui::ET_MOUSE_MOVED, p1, p1, ui::EventTimeForNow(), 0, 0);
  root_view->OnMouseMoved(move1);
  EXPECT_TRUE(v111->received_mouse_enter_);
  EXPECT_FALSE(v11->last_mouse_event_type_);
  EXPECT_TRUE(v1->received_mouse_enter_);

  v111->Reset();
  v1->Reset();

  // Now, move into v121.
  gfx::Point p2(65, 21);
  ui::MouseEvent move2(ui::ET_MOUSE_MOVED, p2, p2, ui::EventTimeForNow(), 0, 0);
  root_view->OnMouseMoved(move2);
  EXPECT_TRUE(v111->received_mouse_exit_);
  EXPECT_TRUE(v121->received_mouse_enter_);
  EXPECT_FALSE(v1->last_mouse_event_type_);

  v111->Reset();
  v121->Reset();

  // Now, move into v11.
  gfx::Point p3(1, 1);
  ui::MouseEvent move3(ui::ET_MOUSE_MOVED, p3, p3, ui::EventTimeForNow(), 0, 0);
  root_view->OnMouseMoved(move3);
  EXPECT_TRUE(v121->received_mouse_exit_);
  EXPECT_TRUE(v11->received_mouse_enter_);
  EXPECT_FALSE(v1->last_mouse_event_type_);

  v121->Reset();
  v11->Reset();

  // Move to v21.
  gfx::Point p4(121, 15);
  ui::MouseEvent move4(ui::ET_MOUSE_MOVED, p4, p4, ui::EventTimeForNow(), 0, 0);
  root_view->OnMouseMoved(move4);
  EXPECT_TRUE(v21->received_mouse_enter_);
  EXPECT_FALSE(v2->last_mouse_event_type_);
  EXPECT_TRUE(v11->received_mouse_exit_);
  EXPECT_TRUE(v1->received_mouse_exit_);

  v21->Reset();
  v11->Reset();
  v1->Reset();

  // Move to v1.
  gfx::Point p5(21, 0);
  ui::MouseEvent move5(ui::ET_MOUSE_MOVED, p5, p5, ui::EventTimeForNow(), 0, 0);
  root_view->OnMouseMoved(move5);
  EXPECT_TRUE(v21->received_mouse_exit_);
  EXPECT_TRUE(v1->received_mouse_enter_);

  v21->Reset();
  v1->Reset();

  // Now, move into v11.
  gfx::Point p6(15, 15);
  ui::MouseEvent mouse6(ui::ET_MOUSE_MOVED, p6, p6, ui::EventTimeForNow(), 0,
                        0);
  root_view->OnMouseMoved(mouse6);
  EXPECT_TRUE(v11->received_mouse_enter_);
  EXPECT_FALSE(v1->last_mouse_event_type_);

  v11->Reset();
  v1->Reset();

  // Move back into v1. Although |v1| had already received an ENTER for mouse6,
  // and the mouse remains inside |v1| the whole time, it receives another ENTER
  // when the mouse leaves v11.
  gfx::Point p7(21, 0);
  ui::MouseEvent mouse7(ui::ET_MOUSE_MOVED, p7, p7, ui::EventTimeForNow(), 0,
                        0);
  root_view->OnMouseMoved(mouse7);
  EXPECT_TRUE(v11->received_mouse_exit_);
  EXPECT_FALSE(v1->received_mouse_enter_);

  widget->CloseNow();
}

TEST_F(ViewTest, Textfield) {
  const base::string16 kText = ASCIIToUTF16(
      "Reality is that which, when you stop believing it, doesn't go away.");
  const base::string16 kExtraText = ASCIIToUTF16("Pretty deep, Philip!");

  Widget* widget = new Widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.bounds = gfx::Rect(0, 0, 100, 100);
  widget->Init(params);
  View* root_view = widget->GetRootView();

  Textfield* textfield = new Textfield();
  root_view->AddChildView(textfield);

  // Test setting, appending text.
  textfield->SetText(kText);
  EXPECT_EQ(kText, textfield->text());
  textfield->AppendText(kExtraText);
  EXPECT_EQ(kText + kExtraText, textfield->text());
  textfield->SetText(base::string16());
  EXPECT_TRUE(textfield->text().empty());

  // Test selection related methods.
  textfield->SetText(kText);
  EXPECT_TRUE(textfield->GetSelectedText().empty());
  textfield->SelectAll(false);
  EXPECT_EQ(kText, textfield->text());
  textfield->ClearSelection();
  EXPECT_TRUE(textfield->GetSelectedText().empty());

  widget->CloseNow();
}

// Tests that the Textfield view respond appropiately to cut/copy/paste.
TEST_F(ViewTest, TextfieldCutCopyPaste) {
  const base::string16 kNormalText = ASCIIToUTF16("Normal");
  const base::string16 kReadOnlyText = ASCIIToUTF16("Read only");
  const base::string16 kPasswordText =
      ASCIIToUTF16("Password! ** Secret stuff **");

  ui::Clipboard* clipboard = ui::Clipboard::GetForCurrentThread();

  Widget* widget = new Widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.bounds = gfx::Rect(0, 0, 100, 100);
  widget->Init(params);
  View* root_view = widget->GetRootView();

  Textfield* normal = new Textfield();
  Textfield* read_only = new Textfield();
  read_only->SetReadOnly(true);
  Textfield* password = new Textfield();
  password->SetTextInputType(ui::TEXT_INPUT_TYPE_PASSWORD);

  root_view->AddChildView(normal);
  root_view->AddChildView(read_only);
  root_view->AddChildView(password);

  normal->SetText(kNormalText);
  read_only->SetText(kReadOnlyText);
  password->SetText(kPasswordText);

  //
  // Test cut.
  //

  normal->SelectAll(false);
  normal->ExecuteCommand(IDS_APP_CUT, 0);
  base::string16 result;
  clipboard->ReadText(ui::CLIPBOARD_TYPE_COPY_PASTE, &result);
  EXPECT_EQ(kNormalText, result);
  normal->SetText(kNormalText);  // Let's revert to the original content.

  read_only->SelectAll(false);
  read_only->ExecuteCommand(IDS_APP_CUT, 0);
  result.clear();
  clipboard->ReadText(ui::CLIPBOARD_TYPE_COPY_PASTE, &result);
  // Cut should have failed, so the clipboard content should not have changed.
  EXPECT_EQ(kNormalText, result);

  password->SelectAll(false);
  password->ExecuteCommand(IDS_APP_CUT, 0);
  result.clear();
  clipboard->ReadText(ui::CLIPBOARD_TYPE_COPY_PASTE, &result);
  // Cut should have failed, so the clipboard content should not have changed.
  EXPECT_EQ(kNormalText, result);

  //
  // Test copy.
  //

  // Start with |read_only| to observe a change in clipboard text.
  read_only->SelectAll(false);
  read_only->ExecuteCommand(IDS_APP_COPY, 0);
  result.clear();
  clipboard->ReadText(ui::CLIPBOARD_TYPE_COPY_PASTE, &result);
  EXPECT_EQ(kReadOnlyText, result);

  normal->SelectAll(false);
  normal->ExecuteCommand(IDS_APP_COPY, 0);
  result.clear();
  clipboard->ReadText(ui::CLIPBOARD_TYPE_COPY_PASTE, &result);
  EXPECT_EQ(kNormalText, result);

  password->SelectAll(false);
  password->ExecuteCommand(IDS_APP_COPY, 0);
  result.clear();
  clipboard->ReadText(ui::CLIPBOARD_TYPE_COPY_PASTE, &result);
  // Text cannot be copied from an obscured field; the clipboard won't change.
  EXPECT_EQ(kNormalText, result);

  //
  // Test paste.
  //

  // Attempting to paste kNormalText in a read-only text-field should fail.
  read_only->SelectAll(false);
  read_only->ExecuteCommand(IDS_APP_PASTE, 0);
  EXPECT_EQ(kReadOnlyText, read_only->text());

  password->SelectAll(false);
  password->ExecuteCommand(IDS_APP_PASTE, 0);
  EXPECT_EQ(kNormalText, password->text());

  // Copy from |read_only| to observe a change in the normal textfield text.
  read_only->SelectAll(false);
  read_only->ExecuteCommand(IDS_APP_COPY, 0);
  normal->SelectAll(false);
  normal->ExecuteCommand(IDS_APP_PASTE, 0);
  EXPECT_EQ(kReadOnlyText, normal->text());
  widget->CloseNow();
}

////////////////////////////////////////////////////////////////////////////////
// Accelerators
////////////////////////////////////////////////////////////////////////////////
bool TestView::AcceleratorPressed(const ui::Accelerator& accelerator) {
  accelerator_count_map_[accelerator]++;
  return true;
}

namespace {

// A Widget with a TestView in the view hierarchy. Used for accelerator tests.
class TestViewWidget {
 public:
  TestViewWidget(const Widget::InitParams& create_params,
                 ui::Accelerator* initial_accelerator,
                 bool show_after_init = true)
      : view_(new TestView) {
    view_->Reset();

    // Register a keyboard accelerator before the view is added to a window.
    if (initial_accelerator) {
      view_->AddAccelerator(*initial_accelerator);
      EXPECT_EQ(view_->accelerator_count_map_[*initial_accelerator], 0);
    }

    // Create a window and add the view as its child.
    Widget::InitParams params = create_params;
    params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
    params.bounds = gfx::Rect(0, 0, 100, 100);
    widget_.Init(params);
    View* root = widget_.GetRootView();
    root->AddChildView(view_);
    if (show_after_init)
      widget_.Show();

    EXPECT_TRUE(widget_.GetFocusManager());
  }

  TestView* view() { return view_; }
  Widget* widget() { return &widget_; }

 private:
  TestView* view_;
  Widget widget_;

  DISALLOW_COPY_AND_ASSIGN(TestViewWidget);
};

}  // namespace

// On non-ChromeOS aura there is extra logic to determine whether a view should
// handle accelerators or not (see View::CanHandleAccelerators for details).
// This test targets that extra logic, but should also work on other platforms.
TEST_F(ViewTest, HandleAccelerator) {
  ui::Accelerator return_accelerator(ui::VKEY_RETURN, ui::EF_NONE);
  TestViewWidget test_widget(CreateParams(Widget::InitParams::TYPE_POPUP),
                             &return_accelerator);
  TestView* view = test_widget.view();
  Widget* widget = test_widget.widget();
  FocusManager* focus_manager = widget->GetFocusManager();

#if defined(USE_AURA) && !defined(OS_CHROMEOS)
  // When a non-child view is not active, it shouldn't handle accelerators.
  EXPECT_FALSE(widget->IsActive());
  EXPECT_FALSE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(0, view->accelerator_count_map_[return_accelerator]);
#endif

  // TYPE_POPUP widgets default to non-activatable, so the Show() above wouldn't
  // have activated the Widget. First, allow activation.
  widget->widget_delegate()->SetCanActivate(true);

  // When a non-child view is active, it should handle accelerators.
  view->accelerator_count_map_[return_accelerator] = 0;
  widget->Activate();
  EXPECT_TRUE(widget->IsActive());
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(1, view->accelerator_count_map_[return_accelerator]);

  // Add a child view associated with a child widget.
  TestView* child_view = new TestView();
  child_view->Reset();
  child_view->AddAccelerator(return_accelerator);
  EXPECT_EQ(child_view->accelerator_count_map_[return_accelerator], 0);
  view->AddChildView(child_view);
  Widget* child_widget = new Widget;
  Widget::InitParams child_params =
      CreateParams(Widget::InitParams::TYPE_CONTROL);
  child_params.parent = widget->GetNativeView();
  child_widget->Init(child_params);
  child_widget->SetContentsView(child_view);

  FocusManager* child_focus_manager = child_widget->GetFocusManager();
  ASSERT_TRUE(child_focus_manager);

  // When a child view is in focus, it should handle accelerators.
  child_view->accelerator_count_map_[return_accelerator] = 0;
  view->accelerator_count_map_[return_accelerator] = 0;
  child_focus_manager->SetFocusedView(child_view);
  EXPECT_FALSE(child_view->GetWidget()->IsActive());
  EXPECT_TRUE(child_focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(1, child_view->accelerator_count_map_[return_accelerator]);
  EXPECT_EQ(0, view->accelerator_count_map_[return_accelerator]);

#if defined(USE_AURA) && !defined(OS_CHROMEOS)
  // When a child view is not in focus, its parent should handle accelerators.
  child_view->accelerator_count_map_[return_accelerator] = 0;
  view->accelerator_count_map_[return_accelerator] = 0;
  child_focus_manager->ClearFocus();
  EXPECT_FALSE(child_view->GetWidget()->IsActive());
  EXPECT_TRUE(child_focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(0, child_view->accelerator_count_map_[return_accelerator]);
  EXPECT_EQ(1, view->accelerator_count_map_[return_accelerator]);
#endif
}

// TODO(themblsha): Bring this up on non-Mac platforms. It currently fails
// because TestView::AcceleratorPressed() is not called. See
// http://crbug.com/667757.
#if defined(OS_MACOSX)
// Test that BridgedContentView correctly handles Accelerator key events when
// subject to OS event dispatch.
TEST_F(ViewTest, ActivateAcceleratorOnMac) {
  // Cmd+1 translates to "noop:" command by interpretKeyEvents.
  ui::Accelerator command_accelerator(ui::VKEY_1, ui::EF_COMMAND_DOWN);
  TestViewWidget test_widget(CreateParams(Widget::InitParams::TYPE_POPUP),
                             &command_accelerator);
  TestView* view = test_widget.view();

  ui::test::EventGenerator event_generator(
      test_widget.widget()->GetNativeWindow());
  // Emulate normal event dispatch through -[NSWindow sendEvent:].
  event_generator.set_target(ui::test::EventGenerator::Target::WINDOW);

  event_generator.PressKey(command_accelerator.key_code(),
                           command_accelerator.modifiers());
  event_generator.ReleaseKey(command_accelerator.key_code(),
                             command_accelerator.modifiers());
  EXPECT_EQ(view->accelerator_count_map_[command_accelerator], 1);

  // Without an _wantsKeyDownForEvent: override we'll only get a keyUp: event
  // for this accelerator.
  ui::Accelerator key_up_accelerator(ui::VKEY_TAB,
                                     ui::EF_CONTROL_DOWN | ui::EF_SHIFT_DOWN);
  view->AddAccelerator(key_up_accelerator);
  event_generator.PressKey(key_up_accelerator.key_code(),
                           key_up_accelerator.modifiers());
  event_generator.ReleaseKey(key_up_accelerator.key_code(),
                             key_up_accelerator.modifiers());
  EXPECT_EQ(view->accelerator_count_map_[key_up_accelerator], 1);

  // We should handle this accelerator inside keyDown: as it doesn't translate
  // to any command by default.
  ui::Accelerator key_down_accelerator(
      ui::VKEY_L, ui::EF_CONTROL_DOWN | ui::EF_ALT_DOWN | ui::EF_SHIFT_DOWN);
  view->AddAccelerator(key_down_accelerator);
  event_generator.PressKey(key_down_accelerator.key_code(),
                           key_down_accelerator.modifiers());
  event_generator.ReleaseKey(key_down_accelerator.key_code(),
                             key_down_accelerator.modifiers());
  EXPECT_EQ(view->accelerator_count_map_[key_down_accelerator], 1);
}
#endif  // OS_MACOSX

// TODO: these tests were initially commented out when getting aura to
// run. Figure out if still valuable and either nuke or fix. crbug.com/667757.
#if defined(OS_MACOSX)
TEST_F(ViewTest, ActivateAccelerator) {
  ui::Accelerator return_accelerator(ui::VKEY_RETURN, ui::EF_NONE);
  TestViewWidget test_widget(CreateParams(Widget::InitParams::TYPE_POPUP),
                             &return_accelerator);
  TestView* view = test_widget.view();
  FocusManager* focus_manager = test_widget.widget()->GetFocusManager();

  // Hit the return key and see if it takes effect.
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 1);

  // Hit the escape key. Nothing should happen.
  ui::Accelerator escape_accelerator(ui::VKEY_ESCAPE, ui::EF_NONE);
  EXPECT_FALSE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 1);
  EXPECT_EQ(view->accelerator_count_map_[escape_accelerator], 0);

  // Now register the escape key and hit it again.
  view->AddAccelerator(escape_accelerator);
  EXPECT_TRUE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 1);
  EXPECT_EQ(view->accelerator_count_map_[escape_accelerator], 1);

  // Remove the return key accelerator.
  view->RemoveAccelerator(return_accelerator);
  EXPECT_FALSE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 1);
  EXPECT_EQ(view->accelerator_count_map_[escape_accelerator], 1);

  // Add it again. Hit the return key and the escape key.
  view->AddAccelerator(return_accelerator);
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 2);
  EXPECT_EQ(view->accelerator_count_map_[escape_accelerator], 1);
  EXPECT_TRUE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 2);
  EXPECT_EQ(view->accelerator_count_map_[escape_accelerator], 2);

  // Remove all the accelerators.
  view->ResetAccelerators();
  EXPECT_FALSE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 2);
  EXPECT_EQ(view->accelerator_count_map_[escape_accelerator], 2);
  EXPECT_FALSE(focus_manager->ProcessAccelerator(escape_accelerator));
  EXPECT_EQ(view->accelerator_count_map_[return_accelerator], 2);
  EXPECT_EQ(view->accelerator_count_map_[escape_accelerator], 2);
}

TEST_F(ViewTest, HiddenViewWithAccelerator) {
  ui::Accelerator return_accelerator(ui::VKEY_RETURN, ui::EF_NONE);
  TestViewWidget test_widget(CreateParams(Widget::InitParams::TYPE_POPUP),
                             &return_accelerator);
  TestView* view = test_widget.view();
  FocusManager* focus_manager = test_widget.widget()->GetFocusManager();

  view->SetVisible(false);
  EXPECT_FALSE(focus_manager->ProcessAccelerator(return_accelerator));

  view->SetVisible(true);
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
}

TEST_F(ViewTest, ViewInHiddenWidgetWithAccelerator) {
  ui::Accelerator return_accelerator(ui::VKEY_RETURN, ui::EF_NONE);
  TestViewWidget test_widget(CreateParams(Widget::InitParams::TYPE_POPUP),
                             &return_accelerator, false);
  TestView* view = test_widget.view();
  Widget* widget = test_widget.widget();
  FocusManager* focus_manager = test_widget.widget()->GetFocusManager();

  EXPECT_FALSE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(0, view->accelerator_count_map_[return_accelerator]);

  widget->Show();
  EXPECT_TRUE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(1, view->accelerator_count_map_[return_accelerator]);

  widget->Hide();
  EXPECT_FALSE(focus_manager->ProcessAccelerator(return_accelerator));
  EXPECT_EQ(1, view->accelerator_count_map_[return_accelerator]);
}
#endif  // OS_MACOSX

// TODO: these tests were initially commented out when getting aura to
// run. Figure out if still valuable and either nuke or fix.
#if 0
////////////////////////////////////////////////////////////////////////////////
// Mouse-wheel message rerouting
////////////////////////////////////////////////////////////////////////////////
class ScrollableTestView : public View {
 public:
  ScrollableTestView() { }

  virtual gfx::Size GetPreferredSize() {
    return gfx::Size(100, 10000);
  }

  virtual void Layout() {
    SizeToPreferredSize();
  }
};

class TestViewWithControls : public View {
 public:
  TestViewWithControls() {
    text_field_ = new Textfield();
    AddChildView(text_field_);
  }

  Textfield* text_field_;
};

class SimpleWidgetDelegate : public WidgetDelegate {
 public:
  explicit SimpleWidgetDelegate(View* contents) : contents_(contents) {  }

  virtual void DeleteDelegate() { delete this; }

  virtual View* GetContentsView() { return contents_; }

  virtual Widget* GetWidget() { return contents_->GetWidget(); }
  virtual const Widget* GetWidget() const { return contents_->GetWidget(); }

 private:
  View* contents_;
};

// Tests that the mouse-wheel messages are correctly rerouted to the window
// under the mouse.
// TODO(jcampan): http://crbug.com/10572 Disabled as it fails on the Vista build
//                bot.
// Note that this fails for a variety of reasons:
// - focused view is apparently reset across window activations and never
//   properly restored
// - this test depends on you not having any other window visible open under the
//   area that it opens the test windows. --beng
TEST_F(ViewTest, DISABLED_RerouteMouseWheelTest) {
  TestViewWithControls* view_with_controls = new TestViewWithControls();
  Widget* window1 = Widget::CreateWindowWithBounds(
      new SimpleWidgetDelegate(view_with_controls),
      gfx::Rect(0, 0, 100, 100));
  window1->Show();
  ScrollView* scroll_view = new ScrollView();
  scroll_view->SetContents(new ScrollableTestView());
  Widget* window2 = Widget::CreateWindowWithBounds(
      new SimpleWidgetDelegate(scroll_view),
      gfx::Rect(200, 200, 100, 100));
  window2->Show();
  EXPECT_EQ(0, scroll_view->GetVisibleRect().y());

  // Make the window1 active, as this is what it would be in real-world.
  window1->Activate();

  // Let's send a mouse-wheel message to the different controls and check that
  // it is rerouted to the window under the mouse (effectively scrolling the
  // scroll-view).

  // First to the Window's HWND.
  ::SendMessage(view_with_controls->GetWidget()->GetNativeView(),
                WM_MOUSEWHEEL, MAKEWPARAM(0, -20), MAKELPARAM(250, 250));
  EXPECT_EQ(20, scroll_view->GetVisibleRect().y());

  window1->CloseNow();
  window2->CloseNow();
}
#endif  // 0

////////////////////////////////////////////////////////////////////////////////
// Native view hierachy
////////////////////////////////////////////////////////////////////////////////
class ToplevelWidgetObserverView : public View {
 public:
  ToplevelWidgetObserverView() = default;
  ~ToplevelWidgetObserverView() override = default;

  // View overrides:
  void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) override {
    if (details.is_add) {
      toplevel_ = GetWidget() ? GetWidget()->GetTopLevelWidget() : nullptr;
    } else {
      toplevel_ = nullptr;
    }
  }
  void NativeViewHierarchyChanged() override {
    toplevel_ = GetWidget() ? GetWidget()->GetTopLevelWidget() : nullptr;
  }

  Widget* toplevel() { return toplevel_; }

 private:
  Widget* toplevel_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(ToplevelWidgetObserverView);
};

// Test that a view can track the current top level widget by overriding
// View::ViewHierarchyChanged() and View::NativeViewHierarchyChanged().
TEST_F(ViewTest, NativeViewHierarchyChanged) {
  std::unique_ptr<Widget> toplevel1(new Widget);
  Widget::InitParams toplevel1_params =
      CreateParams(Widget::InitParams::TYPE_POPUP);
  toplevel1_params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  toplevel1->Init(toplevel1_params);

  std::unique_ptr<Widget> toplevel2(new Widget);
  Widget::InitParams toplevel2_params =
      CreateParams(Widget::InitParams::TYPE_POPUP);
  toplevel2_params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  toplevel2->Init(toplevel2_params);

  Widget* child = new Widget;
  Widget::InitParams child_params(Widget::InitParams::TYPE_CONTROL);
  child_params.parent = toplevel1->GetNativeView();
  child->Init(child_params);

  ToplevelWidgetObserverView* observer_view =
      new ToplevelWidgetObserverView();
  EXPECT_EQ(nullptr, observer_view->toplevel());

  child->SetContentsView(observer_view);
  EXPECT_EQ(toplevel1.get(), observer_view->toplevel());

  Widget::ReparentNativeView(child->GetNativeView(),
                             toplevel2->GetNativeView());
  EXPECT_EQ(toplevel2.get(), observer_view->toplevel());

  observer_view->parent()->RemoveChildView(observer_view);
  EXPECT_EQ(nullptr, observer_view->toplevel());

  // Make |observer_view| |child|'s contents view again so that it gets deleted
  // with the widget.
  child->SetContentsView(observer_view);
}

////////////////////////////////////////////////////////////////////////////////
// Transformations
////////////////////////////////////////////////////////////////////////////////

class TransformPaintView : public TestView {
 public:
  TransformPaintView() = default;
  ~TransformPaintView() override = default;

  void ClearScheduledPaintRect() {
    scheduled_paint_rect_ = gfx::Rect();
  }

  gfx::Rect scheduled_paint_rect() const { return scheduled_paint_rect_; }

  // Overridden from View:
  void SchedulePaintInRect(const gfx::Rect& rect) override {
    gfx::Rect xrect = ConvertRectToParent(rect);
    scheduled_paint_rect_.Union(xrect);
  }

 private:
  gfx::Rect scheduled_paint_rect_;

  DISALLOW_COPY_AND_ASSIGN(TransformPaintView);
};

TEST_F(ViewTest, TransformPaint) {
  TransformPaintView* v1 = new TransformPaintView();
  v1->SetBoundsRect(gfx::Rect(0, 0, 500, 300));

  TestView* v2 = new TestView();
  v2->SetBoundsRect(gfx::Rect(100, 100, 200, 100));

  Widget* widget = new Widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget->Init(params);
  widget->Show();
  View* root = widget->GetRootView();

  root->AddChildView(v1);
  v1->AddChildView(v2);

  // At this moment, |v2| occupies (100, 100) to (300, 200) in |root|.
  v1->ClearScheduledPaintRect();
  v2->SchedulePaint();

  EXPECT_EQ(gfx::Rect(100, 100, 200, 100), v1->scheduled_paint_rect());

  // Rotate |v1| counter-clockwise.
  gfx::Transform transform;
  RotateCounterclockwise(&transform);
  transform.matrix().set(1, 3, 500.0);
  v1->SetTransform(transform);

  // |v2| now occupies (100, 200) to (200, 400) in |root|.

  v1->ClearScheduledPaintRect();
  v2->SchedulePaint();

  EXPECT_EQ(gfx::Rect(100, 200, 100, 200), v1->scheduled_paint_rect());

  widget->CloseNow();
}

TEST_F(ViewTest, TransformEvent) {
  TestView* v1 = new TestView();
  v1->SetBoundsRect(gfx::Rect(0, 0, 500, 300));

  TestView* v2 = new TestView();
  v2->SetBoundsRect(gfx::Rect(100, 100, 200, 100));

  Widget* widget = new Widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget->Init(params);
  View* root = widget->GetRootView();

  root->AddChildView(v1);
  v1->AddChildView(v2);

  // At this moment, |v2| occupies (100, 100) to (300, 200) in |root|.

  // Rotate |v1| counter-clockwise.
  gfx::Transform transform(v1->GetTransform());
  RotateCounterclockwise(&transform);
  transform.matrix().set(1, 3, 500.0);
  v1->SetTransform(transform);

  // |v2| now occupies (100, 200) to (200, 400) in |root|.
  v1->Reset();
  v2->Reset();

  gfx::Point p1(110, 210);
  ui::MouseEvent pressed(ui::ET_MOUSE_PRESSED, p1, p1, ui::EventTimeForNow(),
                         ui::EF_LEFT_MOUSE_BUTTON, ui::EF_LEFT_MOUSE_BUTTON);
  root->OnMousePressed(pressed);
  EXPECT_EQ(0, v1->last_mouse_event_type_);
  EXPECT_EQ(ui::ET_MOUSE_PRESSED, v2->last_mouse_event_type_);
  EXPECT_EQ(190, v2->location_.x());
  EXPECT_EQ(10, v2->location_.y());

  ui::MouseEvent released(ui::ET_MOUSE_RELEASED, gfx::Point(), gfx::Point(),
                          ui::EventTimeForNow(), 0, 0);
  root->OnMouseReleased(released);

  // Now rotate |v2| inside |v1| clockwise.
  transform = v2->GetTransform();
  RotateClockwise(&transform);
  transform.matrix().set(0, 3, 100.f);
  v2->SetTransform(transform);

  // Now, |v2| occupies (100, 100) to (200, 300) in |v1|, and (100, 300) to
  // (300, 400) in |root|.

  v1->Reset();
  v2->Reset();

  gfx::Point point2(110, 320);
  ui::MouseEvent p2(ui::ET_MOUSE_PRESSED, point2, point2, ui::EventTimeForNow(),
                    ui::EF_LEFT_MOUSE_BUTTON, ui::EF_LEFT_MOUSE_BUTTON);
  root->OnMousePressed(p2);
  EXPECT_EQ(0, v1->last_mouse_event_type_);
  EXPECT_EQ(ui::ET_MOUSE_PRESSED, v2->last_mouse_event_type_);
  EXPECT_EQ(10, v2->location_.x());
  EXPECT_EQ(20, v2->location_.y());

  root->OnMouseReleased(released);

  v1->SetTransform(gfx::Transform());
  v2->SetTransform(gfx::Transform());

  TestView* v3 = new TestView();
  v3->SetBoundsRect(gfx::Rect(10, 10, 20, 30));
  v2->AddChildView(v3);

  // Rotate |v3| clockwise with respect to |v2|.
  transform = v1->GetTransform();
  RotateClockwise(&transform);
  transform.matrix().set(0, 3, 30.f);
  v3->SetTransform(transform);

  // Scale |v2| with respect to |v1| along both axis.
  transform = v2->GetTransform();
  transform.matrix().set(0, 0, 0.8f);
  transform.matrix().set(1, 1, 0.5f);
  v2->SetTransform(transform);

  // |v3| occupies (108, 105) to (132, 115) in |root|.

  v1->Reset();
  v2->Reset();
  v3->Reset();

  gfx::Point point(112, 110);
  ui::MouseEvent p3(ui::ET_MOUSE_PRESSED, point, point, ui::EventTimeForNow(),
                    ui::EF_LEFT_MOUSE_BUTTON, ui::EF_LEFT_MOUSE_BUTTON);
  root->OnMousePressed(p3);

  EXPECT_EQ(ui::ET_MOUSE_PRESSED, v3->last_mouse_event_type_);
  EXPECT_EQ(10, v3->location_.x());
  EXPECT_EQ(25, v3->location_.y());

  root->OnMouseReleased(released);

  v1->SetTransform(gfx::Transform());
  v2->SetTransform(gfx::Transform());
  v3->SetTransform(gfx::Transform());

  v1->Reset();
  v2->Reset();
  v3->Reset();

  // Rotate |v3| clockwise with respect to |v2|, and scale it along both axis.
  transform = v3->GetTransform();
  RotateClockwise(&transform);
  transform.matrix().set(0, 3, 30.f);
  // Rotation sets some scaling transformation. Using SetScale would overwrite
  // that and pollute the rotation. So combine the scaling with the existing
  // transforamtion.
  gfx::Transform scale;
  scale.Scale(0.8f, 0.5f);
  transform.ConcatTransform(scale);
  v3->SetTransform(transform);

  // Translate |v2| with respect to |v1|.
  transform = v2->GetTransform();
  transform.matrix().set(0, 3, 10.f);
  transform.matrix().set(1, 3, 10.f);
  v2->SetTransform(transform);

  // |v3| now occupies (120, 120) to (144, 130) in |root|.

  gfx::Point point3(124, 125);
  ui::MouseEvent p4(ui::ET_MOUSE_PRESSED, point3, point3, ui::EventTimeForNow(),
                    ui::EF_LEFT_MOUSE_BUTTON, ui::EF_LEFT_MOUSE_BUTTON);
  root->OnMousePressed(p4);

  EXPECT_EQ(ui::ET_MOUSE_PRESSED, v3->last_mouse_event_type_);
  EXPECT_EQ(10, v3->location_.x());
  EXPECT_EQ(25, v3->location_.y());

  root->OnMouseReleased(released);

  widget->CloseNow();
}

TEST_F(ViewTest, TransformVisibleBound) {
  gfx::Rect viewport_bounds(0, 0, 100, 100);

  std::unique_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = viewport_bounds;
  widget->Init(params);
  widget->GetRootView()->SetBoundsRect(viewport_bounds);

  View* viewport = new View;
  widget->SetContentsView(viewport);
  View* contents = new View;
  viewport->AddChildView(contents);
  viewport->SetBoundsRect(viewport_bounds);
  contents->SetBoundsRect(gfx::Rect(0, 0, 100, 200));

  View* child = new View;
  contents->AddChildView(child);
  child->SetBoundsRect(gfx::Rect(10, 90, 50, 50));
  EXPECT_EQ(gfx::Rect(0, 0, 50, 10), child->GetVisibleBounds());

  // Rotate |child| counter-clockwise
  gfx::Transform transform;
  RotateCounterclockwise(&transform);
  transform.matrix().set(1, 3, 50.f);
  child->SetTransform(transform);
  EXPECT_EQ(gfx::Rect(40, 0, 10, 50), child->GetVisibleBounds());

  widget->CloseNow();
}

////////////////////////////////////////////////////////////////////////////////
// OnVisibleBoundsChanged()

class VisibleBoundsView : public View {
 public:
  VisibleBoundsView() = default;
  ~VisibleBoundsView() override = default;

  bool received_notification() const { return received_notification_; }
  void set_received_notification(bool received) {
    received_notification_ = received;
  }

 private:
  // Overridden from View:
  bool GetNeedsNotificationWhenVisibleBoundsChange() const override {
     return true;
  }
  void OnVisibleBoundsChanged() override { received_notification_ = true; }

  bool received_notification_ = false;

  DISALLOW_COPY_AND_ASSIGN(VisibleBoundsView);
};

TEST_F(ViewTest, OnVisibleBoundsChanged) {
  gfx::Rect viewport_bounds(0, 0, 100, 100);

  std::unique_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = viewport_bounds;
  widget->Init(params);
  widget->GetRootView()->SetBoundsRect(viewport_bounds);

  View* viewport = new View;
  widget->SetContentsView(viewport);
  View* contents = new View;
  viewport->AddChildView(contents);
  viewport->SetBoundsRect(viewport_bounds);
  contents->SetBoundsRect(gfx::Rect(0, 0, 100, 200));

  // Create a view that cares about visible bounds notifications, and position
  // it just outside the visible bounds of the viewport.
  VisibleBoundsView* child = new VisibleBoundsView;
  contents->AddChildView(child);
  child->SetBoundsRect(gfx::Rect(10, 110, 50, 50));

  // The child bound should be fully clipped.
  EXPECT_TRUE(child->GetVisibleBounds().IsEmpty());

  // Now scroll the contents, but not enough to make the child visible.
  contents->SetY(contents->y() - 1);

  // We should have received the notification since the visible bounds may have
  // changed (even though they didn't).
  EXPECT_TRUE(child->received_notification());
  EXPECT_TRUE(child->GetVisibleBounds().IsEmpty());
  child->set_received_notification(false);

  // Now scroll the contents, this time by enough to make the child visible by
  // one pixel.
  contents->SetY(contents->y() - 10);
  EXPECT_TRUE(child->received_notification());
  EXPECT_EQ(1, child->GetVisibleBounds().height());
  child->set_received_notification(false);

  widget->CloseNow();
}

TEST_F(ViewTest, SetBoundsPaint) {
  TestView top_view;
  TestView* child_view = new TestView;

  top_view.SetBoundsRect(gfx::Rect(0, 0, 100, 100));
  top_view.scheduled_paint_rects_.clear();
  child_view->SetBoundsRect(gfx::Rect(10, 10, 20, 20));
  top_view.AddChildView(child_view);

  top_view.scheduled_paint_rects_.clear();
  child_view->SetBoundsRect(gfx::Rect(30, 30, 20, 20));
  EXPECT_EQ(2U, top_view.scheduled_paint_rects_.size());

  // There should be 2 rects, spanning from (10, 10) to (50, 50).
  gfx::Rect paint_rect = top_view.scheduled_paint_rects_[0];
  paint_rect.Union(top_view.scheduled_paint_rects_[1]);
  EXPECT_EQ(gfx::Rect(10, 10, 40, 40), paint_rect);
}

// Assertions around painting and focus gain/lost.
TEST_F(ViewTest, FocusBlurPaints) {
  TestView parent_view;
  TestView* child_view1 = new TestView;  // Owned by |parent_view|.

  parent_view.SetBoundsRect(gfx::Rect(0, 0, 100, 100));

  child_view1->SetBoundsRect(gfx::Rect(0, 0, 20, 20));
  parent_view.AddChildView(child_view1);

  parent_view.scheduled_paint_rects_.clear();
  child_view1->scheduled_paint_rects_.clear();

  // Focus change shouldn't trigger paints.
  child_view1->DoFocus();

  EXPECT_TRUE(parent_view.scheduled_paint_rects_.empty());
  EXPECT_TRUE(child_view1->scheduled_paint_rects_.empty());

  child_view1->DoBlur();
  EXPECT_TRUE(parent_view.scheduled_paint_rects_.empty());
  EXPECT_TRUE(child_view1->scheduled_paint_rects_.empty());
}

// Verifies SetBounds(same bounds) doesn't trigger a SchedulePaint().
TEST_F(ViewTest, SetBoundsSameBoundsDoesntSchedulePaint) {
  TestView view;

  view.SetBoundsRect(gfx::Rect(0, 0, 100, 100));
  view.InvalidateLayout();
  view.scheduled_paint_rects_.clear();
  view.SetBoundsRect(gfx::Rect(0, 0, 100, 100));
  EXPECT_TRUE(view.scheduled_paint_rects_.empty());
}

// Verifies AddChildView() and RemoveChildView() schedule appropriate paints.
TEST_F(ViewTest, AddAndRemoveSchedulePaints) {
  gfx::Rect viewport_bounds(0, 0, 100, 100);

  // We have to put the View hierarchy into a Widget or no paints will be
  // scheduled.
  std::unique_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = viewport_bounds;
  widget->Init(params);
  widget->GetRootView()->SetBoundsRect(viewport_bounds);

  TestView* parent_view = new TestView;
  widget->SetContentsView(parent_view);
  parent_view->SetBoundsRect(viewport_bounds);
  parent_view->scheduled_paint_rects_.clear();

  View* child_view = new View;
  child_view->SetBoundsRect(gfx::Rect(0, 0, 20, 20));
  parent_view->AddChildView(child_view);
  ASSERT_EQ(1U, parent_view->scheduled_paint_rects_.size());
  EXPECT_EQ(child_view->bounds(), parent_view->scheduled_paint_rects_.front());

  parent_view->scheduled_paint_rects_.clear();
  parent_view->RemoveChildView(child_view);
  std::unique_ptr<View> child_deleter(child_view);
  ASSERT_EQ(1U, parent_view->scheduled_paint_rects_.size());
  EXPECT_EQ(child_view->bounds(), parent_view->scheduled_paint_rects_.front());

  widget->CloseNow();
}

// Tests conversion methods with a transform.
TEST_F(ViewTest, ConversionsWithTransform) {
  TestView top_view;

  // View hierarchy used to test scale transforms.
  TestView* child = new TestView;
  TestView* child_child = new TestView;

  // View used to test a rotation transform.
  TestView* child_2 = new TestView;

  top_view.AddChildView(child);
  child->AddChildView(child_child);

  top_view.SetBoundsRect(gfx::Rect(0, 0, 1000, 1000));

  child->SetBoundsRect(gfx::Rect(7, 19, 500, 500));
  gfx::Transform transform;
  transform.Scale(3.0, 4.0);
  child->SetTransform(transform);

  child_child->SetBoundsRect(gfx::Rect(17, 13, 100, 100));
  transform.MakeIdentity();
  transform.Scale(5.0, 7.0);
  child_child->SetTransform(transform);

  top_view.AddChildView(child_2);
  child_2->SetBoundsRect(gfx::Rect(700, 725, 100, 100));
  transform.MakeIdentity();
  RotateClockwise(&transform);
  child_2->SetTransform(transform);

  // Sanity check to make sure basic transforms act as expected.
  {
    gfx::Transform transform;
    transform.Translate(110.0, -110.0);
    transform.Scale(100.0, 55.0);
    transform.Translate(1.0, 1.0);

    // convert to a 3x3 matrix.
    const SkMatrix& matrix = transform.matrix();

    EXPECT_EQ(210, matrix.getTranslateX());
    EXPECT_EQ(-55, matrix.getTranslateY());
    EXPECT_EQ(100, matrix.getScaleX());
    EXPECT_EQ(55, matrix.getScaleY());
    EXPECT_EQ(0, matrix.getSkewX());
    EXPECT_EQ(0, matrix.getSkewY());
  }

  {
    gfx::Transform transform;
    transform.Translate(1.0, 1.0);
    gfx::Transform t2;
    t2.Scale(100.0, 55.0);
    gfx::Transform t3;
    t3.Translate(110.0, -110.0);
    transform.ConcatTransform(t2);
    transform.ConcatTransform(t3);

    // convert to a 3x3 matrix
    const SkMatrix& matrix = transform.matrix();

    EXPECT_EQ(210, matrix.getTranslateX());
    EXPECT_EQ(-55, matrix.getTranslateY());
    EXPECT_EQ(100, matrix.getScaleX());
    EXPECT_EQ(55, matrix.getScaleY());
    EXPECT_EQ(0, matrix.getSkewX());
    EXPECT_EQ(0, matrix.getSkewY());
  }

  // Conversions from child->top and top->child.
  {
    gfx::Point point(5, 5);
    View::ConvertPointToTarget(child, &top_view, &point);
    EXPECT_EQ(22, point.x());
    EXPECT_EQ(39, point.y());

    gfx::RectF rect(5.0f, 5.0f, 10.0f, 20.0f);
    View::ConvertRectToTarget(child, &top_view, &rect);
    EXPECT_FLOAT_EQ(22.0f, rect.x());
    EXPECT_FLOAT_EQ(39.0f, rect.y());
    EXPECT_FLOAT_EQ(30.0f, rect.width());
    EXPECT_FLOAT_EQ(80.0f, rect.height());

    point.SetPoint(22, 39);
    View::ConvertPointToTarget(&top_view, child, &point);
    EXPECT_EQ(5, point.x());
    EXPECT_EQ(5, point.y());

    rect.SetRect(22.0f, 39.0f, 30.0f, 80.0f);
    View::ConvertRectToTarget(&top_view, child, &rect);
    EXPECT_FLOAT_EQ(5.0f, rect.x());
    EXPECT_FLOAT_EQ(5.0f, rect.y());
    EXPECT_FLOAT_EQ(10.0f, rect.width());
    EXPECT_FLOAT_EQ(20.0f, rect.height());
  }

  // Conversions from child_child->top and top->child_child.
  {
    gfx::Point point(5, 5);
    View::ConvertPointToTarget(child_child, &top_view, &point);
    EXPECT_EQ(133, point.x());
    EXPECT_EQ(211, point.y());

    gfx::RectF rect(5.0f, 5.0f, 10.0f, 20.0f);
    View::ConvertRectToTarget(child_child, &top_view, &rect);
    EXPECT_FLOAT_EQ(133.0f, rect.x());
    EXPECT_FLOAT_EQ(211.0f, rect.y());
    EXPECT_FLOAT_EQ(150.0f, rect.width());
    EXPECT_FLOAT_EQ(560.0f, rect.height());

    point.SetPoint(133, 211);
    View::ConvertPointToTarget(&top_view, child_child, &point);
    EXPECT_EQ(5, point.x());
    EXPECT_EQ(5, point.y());

    rect.SetRect(133.0f, 211.0f, 150.0f, 560.0f);
    View::ConvertRectToTarget(&top_view, child_child, &rect);
    EXPECT_FLOAT_EQ(5.0f, rect.x());
    EXPECT_FLOAT_EQ(5.0f, rect.y());
    EXPECT_FLOAT_EQ(10.0f, rect.width());
    EXPECT_FLOAT_EQ(20.0f, rect.height());
  }

  // Conversions from child_child->child and child->child_child
  {
    gfx::Point point(5, 5);
    View::ConvertPointToTarget(child_child, child, &point);
    EXPECT_EQ(42, point.x());
    EXPECT_EQ(48, point.y());

    gfx::RectF rect(5.0f, 5.0f, 10.0f, 20.0f);
    View::ConvertRectToTarget(child_child, child, &rect);
    EXPECT_FLOAT_EQ(42.0f, rect.x());
    EXPECT_FLOAT_EQ(48.0f, rect.y());
    EXPECT_FLOAT_EQ(50.0f, rect.width());
    EXPECT_FLOAT_EQ(140.0f, rect.height());

    point.SetPoint(42, 48);
    View::ConvertPointToTarget(child, child_child, &point);
    EXPECT_EQ(5, point.x());
    EXPECT_EQ(5, point.y());

    rect.SetRect(42.0f, 48.0f, 50.0f, 140.0f);
    View::ConvertRectToTarget(child, child_child, &rect);
    EXPECT_FLOAT_EQ(5.0f, rect.x());
    EXPECT_FLOAT_EQ(5.0f, rect.y());
    EXPECT_FLOAT_EQ(10.0f, rect.width());
    EXPECT_FLOAT_EQ(20.0f, rect.height());
  }

  // Conversions from top_view to child with a value that should be negative.
  // This ensures we don't round up with negative numbers.
  {
    gfx::Point point(6, 18);
    View::ConvertPointToTarget(&top_view, child, &point);
    EXPECT_EQ(-1, point.x());
    EXPECT_EQ(-1, point.y());

    float error = 0.01f;
    gfx::RectF rect(6.0f, 18.0f, 10.0f, 39.0f);
    View::ConvertRectToTarget(&top_view, child, &rect);
    EXPECT_NEAR(-0.33f, rect.x(), error);
    EXPECT_NEAR(-0.25f, rect.y(), error);
    EXPECT_NEAR(3.33f, rect.width(), error);
    EXPECT_NEAR(9.75f, rect.height(), error);
  }

  // Rect conversions from top_view->child_2 and child_2->top_view.
  {
    gfx::RectF rect(50.0f, 55.0f, 20.0f, 30.0f);
    View::ConvertRectToTarget(child_2, &top_view, &rect);
    EXPECT_FLOAT_EQ(615.0f, rect.x());
    EXPECT_FLOAT_EQ(775.0f, rect.y());
    EXPECT_FLOAT_EQ(30.0f, rect.width());
    EXPECT_FLOAT_EQ(20.0f, rect.height());

    rect.SetRect(615.0f, 775.0f, 30.0f, 20.0f);
    View::ConvertRectToTarget(&top_view, child_2, &rect);
    EXPECT_FLOAT_EQ(50.0f, rect.x());
    EXPECT_FLOAT_EQ(55.0f, rect.y());
    EXPECT_FLOAT_EQ(20.0f, rect.width());
    EXPECT_FLOAT_EQ(30.0f, rect.height());
  }
}

// Tests conversion methods to and from screen coordinates.
TEST_F(ViewTest, ConversionsToFromScreen) {
  std::unique_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget->Init(params);

  View* child = new View;
  widget->GetRootView()->AddChildView(child);
  child->SetBounds(10, 10, 100, 200);
  gfx::Transform t;
  t.Scale(0.5, 0.5);
  child->SetTransform(t);

  gfx::Size size(10, 10);
  gfx::Point point_in_screen(100, 90);
  gfx::Point point_in_child(80, 60);
  gfx::Rect rect_in_screen(point_in_screen, size);
  gfx::Rect rect_in_child(point_in_child, size);

  gfx::Point point = point_in_screen;
  View::ConvertPointFromScreen(child, &point);
  EXPECT_EQ(point_in_child.ToString(), point.ToString());

  View::ConvertPointToScreen(child, &point);
  EXPECT_EQ(point_in_screen.ToString(), point.ToString());

  View::ConvertRectToScreen(child, &rect_in_child);
  EXPECT_EQ(rect_in_screen.ToString(), rect_in_child.ToString());
}

// Tests conversion methods for rectangles.
TEST_F(ViewTest, ConvertRectWithTransform) {
  std::unique_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget->Init(params);
  View* root = widget->GetRootView();

  TestView* v1 = new TestView;
  TestView* v2 = new TestView;
  root->AddChildView(v1);
  v1->AddChildView(v2);

  v1->SetBoundsRect(gfx::Rect(10, 10, 500, 500));
  v2->SetBoundsRect(gfx::Rect(20, 20, 100, 200));

  // |v2| now occupies (30, 30) to (130, 230) in |widget|
  gfx::Rect rect(5, 5, 15, 40);
  EXPECT_EQ(gfx::Rect(25, 25, 15, 40), v2->ConvertRectToParent(rect));
  EXPECT_EQ(gfx::Rect(35, 35, 15, 40), v2->ConvertRectToWidget(rect));

  // Rotate |v2|
  gfx::Transform t2;
  RotateCounterclockwise(&t2);
  t2.matrix().set(1, 3, 100.f);
  v2->SetTransform(t2);

  // |v2| now occupies (30, 30) to (230, 130) in |widget|
  EXPECT_EQ(gfx::Rect(25, 100, 40, 15), v2->ConvertRectToParent(rect));
  EXPECT_EQ(gfx::Rect(35, 110, 40, 15), v2->ConvertRectToWidget(rect));

  // Scale down |v1|
  gfx::Transform t1;
  t1.Scale(0.5, 0.5);
  v1->SetTransform(t1);

  // The rectangle should remain the same for |v1|.
  EXPECT_EQ(gfx::Rect(25, 100, 40, 15), v2->ConvertRectToParent(rect));

  // |v2| now occupies (20, 20) to (120, 70) in |widget|
  EXPECT_EQ(gfx::Rect(22, 60, 21, 8).ToString(),
            v2->ConvertRectToWidget(rect).ToString());

  widget->CloseNow();
}

class ObserverView : public View {
 public:
  ObserverView();
  ~ObserverView() override;

  void ResetTestState();

  bool has_add_details() const { return has_add_details_; }
  bool has_remove_details() const { return has_remove_details_; }

  const ViewHierarchyChangedDetails& add_details() const {
    return add_details_;
  }

  const ViewHierarchyChangedDetails& remove_details() const {
    return remove_details_;
  }

 private:
  // View:
  void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) override;

  bool has_add_details_ = false;
  bool has_remove_details_ = false;
  ViewHierarchyChangedDetails add_details_;
  ViewHierarchyChangedDetails remove_details_;

  DISALLOW_COPY_AND_ASSIGN(ObserverView);
};

ObserverView::ObserverView() = default;

ObserverView::~ObserverView() = default;

void ObserverView::ResetTestState() {
  has_add_details_ = false;
  has_remove_details_ = false;
  add_details_ = ViewHierarchyChangedDetails();
  remove_details_ = ViewHierarchyChangedDetails();
}

void ObserverView::ViewHierarchyChanged(
    const ViewHierarchyChangedDetails& details) {
  if (details.is_add) {
    has_add_details_ = true;
    add_details_ = details;
  } else {
    has_remove_details_ = true;
    remove_details_ = details;
  }
}

// Verifies that the ViewHierarchyChanged() notification is sent correctly when
// a child view is added or removed to all the views in the hierarchy (up and
// down).
// The tree looks like this:
// v1
// +-- v2
//     +-- v3
//     +-- v4 (starts here, then get reparented to v1)
TEST_F(ViewTest, ViewHierarchyChanged) {
  ObserverView v1;

  ObserverView* v3 = new ObserverView();

  // Add |v3| to |v2|.
  std::unique_ptr<ObserverView> v2(new ObserverView());
  v2->AddChildView(v3);

  // Make sure both |v2| and |v3| receive the ViewHierarchyChanged()
  // notification.
  EXPECT_TRUE(v2->has_add_details());
  EXPECT_FALSE(v2->has_remove_details());
  EXPECT_EQ(v2.get(), v2->add_details().parent);
  EXPECT_EQ(v3, v2->add_details().child);
  EXPECT_EQ(nullptr, v2->add_details().move_view);

  EXPECT_TRUE(v3->has_add_details());
  EXPECT_FALSE(v3->has_remove_details());
  EXPECT_EQ(v2.get(), v3->add_details().parent);
  EXPECT_EQ(v3, v3->add_details().child);
  EXPECT_EQ(nullptr, v3->add_details().move_view);

  // Reset everything to the initial state.
  v2->ResetTestState();
  v3->ResetTestState();

  // Add |v2| to v1.
  v1.AddChildView(v2.get());

  // Verifies that |v2| is the child view *added* and the parent view is |v1|.
  // Make sure all the views (v1, v2, v3) received _that_ information.
  EXPECT_TRUE(v1.has_add_details());
  EXPECT_FALSE(v1.has_remove_details());
  EXPECT_EQ(&v1, v1.add_details().parent);
  EXPECT_EQ(v2.get(), v1.add_details().child);
  EXPECT_EQ(nullptr, v1.add_details().move_view);

  EXPECT_TRUE(v2->has_add_details());
  EXPECT_FALSE(v2->has_remove_details());
  EXPECT_EQ(&v1, v2->add_details().parent);
  EXPECT_EQ(v2.get(), v2->add_details().child);
  EXPECT_EQ(nullptr, v2->add_details().move_view);

  EXPECT_TRUE(v3->has_add_details());
  EXPECT_FALSE(v3->has_remove_details());
  EXPECT_EQ(&v1, v3->add_details().parent);
  EXPECT_EQ(v2.get(), v3->add_details().child);
  EXPECT_EQ(nullptr, v3->add_details().move_view);

  // Reset everything to the initial state.
  v1.ResetTestState();
  v2->ResetTestState();
  v3->ResetTestState();

  // Remove |v2| from |v1|.
  v1.RemoveChildView(v2.get());

  // Verifies that |v2| is the child view *removed* and the parent view is |v1|.
  // Make sure all the views (v1, v2, v3) received _that_ information.
  EXPECT_FALSE(v1.has_add_details());
  EXPECT_TRUE(v1.has_remove_details());
  EXPECT_EQ(&v1, v1.remove_details().parent);
  EXPECT_EQ(v2.get(), v1.remove_details().child);
  EXPECT_EQ(nullptr, v1.remove_details().move_view);

  EXPECT_FALSE(v2->has_add_details());
  EXPECT_TRUE(v2->has_remove_details());
  EXPECT_EQ(&v1, v2->remove_details().parent);
  EXPECT_EQ(v2.get(), v2->remove_details().child);
  EXPECT_EQ(nullptr, v2->remove_details().move_view);

  EXPECT_FALSE(v3->has_add_details());
  EXPECT_TRUE(v3->has_remove_details());
  EXPECT_EQ(&v1, v3->remove_details().parent);
  EXPECT_EQ(v3, v3->remove_details().child);
  EXPECT_EQ(nullptr, v3->remove_details().move_view);

  // Verifies notifications when reparenting a view.
  ObserverView* v4 = new ObserverView();
  // Add |v4| to |v2|.
  v2->AddChildView(v4);

  // Reset everything to the initial state.
  v1.ResetTestState();
  v2->ResetTestState();
  v3->ResetTestState();
  v4->ResetTestState();

  // Reparent |v4| to |v1|.
  v1.AddChildView(v4);

  // Verifies that all views receive the correct information for all the child,
  // parent and move views.

  // |v1| is the new parent, |v4| is the child for add, |v2| is the old parent.
  EXPECT_TRUE(v1.has_add_details());
  EXPECT_FALSE(v1.has_remove_details());
  EXPECT_EQ(&v1, v1.add_details().parent);
  EXPECT_EQ(v4, v1.add_details().child);
  EXPECT_EQ(v2.get(), v1.add_details().move_view);

  // |v2| is the old parent, |v4| is the child for remove, |v1| is the new
  // parent.
  EXPECT_FALSE(v2->has_add_details());
  EXPECT_TRUE(v2->has_remove_details());
  EXPECT_EQ(v2.get(), v2->remove_details().parent);
  EXPECT_EQ(v4, v2->remove_details().child);
  EXPECT_EQ(&v1, v2->remove_details().move_view);

  // |v3| is not impacted by this operation, and hence receives no notification.
  EXPECT_FALSE(v3->has_add_details());
  EXPECT_FALSE(v3->has_remove_details());

  // |v4| is the reparented child, so it receives notifications for the remove
  // and then the add.  |v2| is its old parent, |v1| is its new parent.
  EXPECT_TRUE(v4->has_remove_details());
  EXPECT_TRUE(v4->has_add_details());
  EXPECT_EQ(v2.get(), v4->remove_details().parent);
  EXPECT_EQ(&v1, v4->add_details().parent);
  EXPECT_EQ(v4, v4->add_details().child);
  EXPECT_EQ(v4, v4->remove_details().child);
  EXPECT_EQ(&v1, v4->remove_details().move_view);
  EXPECT_EQ(v2.get(), v4->add_details().move_view);
}

class WidgetObserverView : public View {
 public:
  WidgetObserverView();
  ~WidgetObserverView() override;

  void ResetTestState();

  int added_to_widget_count() { return added_to_widget_count_; }
  int removed_from_widget_count() { return removed_from_widget_count_; }

 private:
  void AddedToWidget() override;
  void RemovedFromWidget() override;

  int added_to_widget_count_ = 0;
  int removed_from_widget_count_ = 0;

  DISALLOW_COPY_AND_ASSIGN(WidgetObserverView);
};

WidgetObserverView::WidgetObserverView() {
  ResetTestState();
}

WidgetObserverView::~WidgetObserverView() = default;

void WidgetObserverView::ResetTestState() {
  added_to_widget_count_ = 0;
  removed_from_widget_count_ = 0;
}

void WidgetObserverView::AddedToWidget() {
  ++added_to_widget_count_;
}

void WidgetObserverView::RemovedFromWidget() {
  ++removed_from_widget_count_;
}

// Verifies that AddedToWidget and RemovedFromWidget are called for a view when
// it is added to hierarchy.
// The tree looks like this:
// widget
// +-- root
//
// then v1 is added to root:
//
//     v1
//     +-- v2
//
// finally v1 is removed from root.
TEST_F(ViewTest, AddedToRemovedFromWidget) {
  Widget widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(50, 50, 650, 650);
  widget.Init(params);

  View* root = widget.GetRootView();

  WidgetObserverView v1;
  WidgetObserverView v2;
  WidgetObserverView v3;
  v1.set_owned_by_client();
  v2.set_owned_by_client();
  v3.set_owned_by_client();

  v1.AddChildView(&v2);
  EXPECT_EQ(0, v2.added_to_widget_count());
  EXPECT_EQ(0, v2.removed_from_widget_count());

  root->AddChildView(&v1);
  EXPECT_EQ(1, v1.added_to_widget_count());
  EXPECT_EQ(0, v1.removed_from_widget_count());
  EXPECT_EQ(1, v2.added_to_widget_count());
  EXPECT_EQ(0, v2.removed_from_widget_count());

  v1.ResetTestState();
  v2.ResetTestState();

  v2.AddChildView(&v3);
  EXPECT_EQ(0, v1.added_to_widget_count());
  EXPECT_EQ(0, v1.removed_from_widget_count());
  EXPECT_EQ(0, v2.added_to_widget_count());
  EXPECT_EQ(0, v2.removed_from_widget_count());

  v1.ResetTestState();
  v2.ResetTestState();

  root->RemoveChildView(&v1);
  EXPECT_EQ(0, v1.added_to_widget_count());
  EXPECT_EQ(1, v1.removed_from_widget_count());
  EXPECT_EQ(0, v2.added_to_widget_count());
  EXPECT_EQ(1, v2.removed_from_widget_count());

  v2.ResetTestState();
  v1.RemoveChildView(&v2);
  EXPECT_EQ(0, v2.removed_from_widget_count());

  // Test move between parents in a single Widget.
  v2.RemoveChildView(&v3);
  v1.ResetTestState();
  v2.ResetTestState();
  v3.ResetTestState();

  v1.AddChildView(&v2);
  root->AddChildView(&v1);
  root->AddChildView(&v3);
  EXPECT_EQ(1, v1.added_to_widget_count());
  EXPECT_EQ(1, v2.added_to_widget_count());
  EXPECT_EQ(1, v3.added_to_widget_count());

  v3.AddChildView(&v1);
  EXPECT_EQ(1, v1.added_to_widget_count());
  EXPECT_EQ(0, v1.removed_from_widget_count());
  EXPECT_EQ(1, v2.added_to_widget_count());
  EXPECT_EQ(0, v2.removed_from_widget_count());
  EXPECT_EQ(1, v3.added_to_widget_count());
  EXPECT_EQ(0, v3.removed_from_widget_count());

  // Test move between widgets.
  Widget second_widget;
  params.bounds = gfx::Rect(150, 150, 650, 650);
  second_widget.Init(params);

  View* second_root = second_widget.GetRootView();

  v1.ResetTestState();
  v2.ResetTestState();
  v3.ResetTestState();

  second_root->AddChildView(&v1);
  EXPECT_EQ(1, v1.removed_from_widget_count());
  EXPECT_EQ(1, v1.added_to_widget_count());
  EXPECT_EQ(1, v2.added_to_widget_count());
  EXPECT_EQ(1, v2.removed_from_widget_count());
  EXPECT_EQ(0, v3.added_to_widget_count());
  EXPECT_EQ(0, v3.removed_from_widget_count());
}

// Verifies if the child views added under the root are all deleted when calling
// RemoveAllChildViews.
// The tree looks like this:
// root
// +-- child1
//     +-- foo
//         +-- bar0
//         +-- bar1
//         +-- bar2
// +-- child2
// +-- child3
TEST_F(ViewTest, RemoveAllChildViews) {
  View root;

  View* child1 = new View;
  root.AddChildView(child1);

  for (int i = 0; i < 2; ++i)
    root.AddChildView(new View);

  View* foo = new View;
  child1->AddChildView(foo);

  // Add some nodes to |foo|.
  for (int i = 0; i < 3; ++i)
    foo->AddChildView(new View);

  EXPECT_EQ(3u, root.children().size());
  EXPECT_EQ(1u, child1->children().size());
  EXPECT_EQ(3u, foo->children().size());

  // Now remove all child views from root.
  root.RemoveAllChildViews(true);

  EXPECT_TRUE(root.children().empty());
}

TEST_F(ViewTest, Contains) {
  View v1;
  View* v2 = new View;
  View* v3 = new View;

  v1.AddChildView(v2);
  v2->AddChildView(v3);

  EXPECT_FALSE(v1.Contains(nullptr));
  EXPECT_TRUE(v1.Contains(&v1));
  EXPECT_TRUE(v1.Contains(v2));
  EXPECT_TRUE(v1.Contains(v3));

  EXPECT_FALSE(v2->Contains(nullptr));
  EXPECT_TRUE(v2->Contains(v2));
  EXPECT_FALSE(v2->Contains(&v1));
  EXPECT_TRUE(v2->Contains(v3));

  EXPECT_FALSE(v3->Contains(nullptr));
  EXPECT_TRUE(v3->Contains(v3));
  EXPECT_FALSE(v3->Contains(&v1));
  EXPECT_FALSE(v3->Contains(v2));
}

// Verifies if GetIndexOf() returns the correct index for the specified child
// view.
// The tree looks like this:
// root
// +-- child1
//     +-- foo1
// +-- child2
TEST_F(ViewTest, GetIndexOf) {
  View root;

  View* child1 = new View;
  root.AddChildView(child1);

  View* child2 = new View;
  root.AddChildView(child2);

  View* foo1 = new View;
  child1->AddChildView(foo1);

  EXPECT_EQ(-1, root.GetIndexOf(nullptr));
  EXPECT_EQ(-1, root.GetIndexOf(&root));
  EXPECT_EQ(0, root.GetIndexOf(child1));
  EXPECT_EQ(1, root.GetIndexOf(child2));
  EXPECT_EQ(-1, root.GetIndexOf(foo1));

  EXPECT_EQ(-1, child1->GetIndexOf(nullptr));
  EXPECT_EQ(-1, child1->GetIndexOf(&root));
  EXPECT_EQ(-1, child1->GetIndexOf(child1));
  EXPECT_EQ(-1, child1->GetIndexOf(child2));
  EXPECT_EQ(0, child1->GetIndexOf(foo1));

  EXPECT_EQ(-1, child2->GetIndexOf(nullptr));
  EXPECT_EQ(-1, child2->GetIndexOf(&root));
  EXPECT_EQ(-1, child2->GetIndexOf(child2));
  EXPECT_EQ(-1, child2->GetIndexOf(child1));
  EXPECT_EQ(-1, child2->GetIndexOf(foo1));
}

// Verifies that the child views can be reordered correctly.
TEST_F(ViewTest, ReorderChildren) {
  View root;

  View* child = root.AddChildView(std::make_unique<View>());

  View* foo1 = child->AddChildView(std::make_unique<View>());
  View* foo2 = child->AddChildView(std::make_unique<View>());
  View* foo3 = child->AddChildView(std::make_unique<View>());
  foo1->SetFocusBehavior(View::FocusBehavior::ALWAYS);
  foo2->SetFocusBehavior(View::FocusBehavior::ALWAYS);
  foo3->SetFocusBehavior(View::FocusBehavior::ALWAYS);

  ASSERT_EQ(0, child->GetIndexOf(foo1));
  ASSERT_EQ(1, child->GetIndexOf(foo2));
  ASSERT_EQ(2, child->GetIndexOf(foo3));
  ASSERT_EQ(foo2, foo1->GetNextFocusableView());
  ASSERT_EQ(foo3, foo2->GetNextFocusableView());
  ASSERT_EQ(nullptr, foo3->GetNextFocusableView());

  // Move |foo2| at the end.
  child->ReorderChildView(foo2, -1);
  ASSERT_EQ(0, child->GetIndexOf(foo1));
  ASSERT_EQ(1, child->GetIndexOf(foo3));
  ASSERT_EQ(2, child->GetIndexOf(foo2));
  ASSERT_EQ(foo3, foo1->GetNextFocusableView());
  ASSERT_EQ(foo2, foo3->GetNextFocusableView());
  ASSERT_EQ(nullptr, foo2->GetNextFocusableView());

  // Move |foo1| at the end.
  child->ReorderChildView(foo1, -1);
  ASSERT_EQ(0, child->GetIndexOf(foo3));
  ASSERT_EQ(1, child->GetIndexOf(foo2));
  ASSERT_EQ(2, child->GetIndexOf(foo1));
  ASSERT_EQ(nullptr, foo1->GetNextFocusableView());
  ASSERT_EQ(foo2, foo1->GetPreviousFocusableView());
  ASSERT_EQ(foo2, foo3->GetNextFocusableView());
  ASSERT_EQ(foo1, foo2->GetNextFocusableView());

  // Move |foo2| to the front.
  child->ReorderChildView(foo2, 0);
  ASSERT_EQ(0, child->GetIndexOf(foo2));
  ASSERT_EQ(1, child->GetIndexOf(foo3));
  ASSERT_EQ(2, child->GetIndexOf(foo1));
  ASSERT_EQ(nullptr, foo1->GetNextFocusableView());
  ASSERT_EQ(foo3, foo1->GetPreviousFocusableView());
  ASSERT_EQ(foo3, foo2->GetNextFocusableView());
  ASSERT_EQ(foo1, foo3->GetNextFocusableView());
}

// Verifies that GetViewByID returns the correctly child view from the specified
// ID.
// The tree looks like this:
// v1
// +-- v2
//     +-- v3
//     +-- v4
TEST_F(ViewTest, GetViewByID) {
  View v1;
  const int kV1ID = 1;
  v1.SetID(kV1ID);

  View v2;
  const int kV2ID = 2;
  v2.SetID(kV2ID);

  View v3;
  const int kV3ID = 3;
  v3.SetID(kV3ID);

  View v4;
  const int kV4ID = 4;
  v4.SetID(kV4ID);

  const int kV5ID = 5;

  v1.AddChildView(&v2);
  v2.AddChildView(&v3);
  v2.AddChildView(&v4);

  EXPECT_EQ(&v1, v1.GetViewByID(kV1ID));
  EXPECT_EQ(&v2, v1.GetViewByID(kV2ID));
  EXPECT_EQ(&v4, v1.GetViewByID(kV4ID));

  EXPECT_EQ(nullptr, v1.GetViewByID(kV5ID));  // No V5 exists.
  EXPECT_EQ(nullptr,
            v2.GetViewByID(kV1ID));  // It can get only from child views.

  const int kGroup = 1;
  v3.SetGroup(kGroup);
  v4.SetGroup(kGroup);

  View::Views views;
  v1.GetViewsInGroup(kGroup, &views);
  EXPECT_EQ(2U, views.size());
  EXPECT_TRUE(base::ContainsValue(views, &v3));
  EXPECT_TRUE(base::ContainsValue(views, &v4));
}

TEST_F(ViewTest, AddExistingChild) {
  View v1, v2, v3;

  v1.AddChildView(&v2);
  v1.AddChildView(&v3);
  EXPECT_EQ(0, v1.GetIndexOf(&v2));
  EXPECT_EQ(1, v1.GetIndexOf(&v3));

  // Check that there's no change in order when adding at same index.
  v1.AddChildViewAt(&v2, 0);
  EXPECT_EQ(0, v1.GetIndexOf(&v2));
  EXPECT_EQ(1, v1.GetIndexOf(&v3));
  v1.AddChildViewAt(&v3, 1);
  EXPECT_EQ(0, v1.GetIndexOf(&v2));
  EXPECT_EQ(1, v1.GetIndexOf(&v3));

  // Add it at a different index and check for change in order.
  v1.AddChildViewAt(&v2, 1);
  EXPECT_EQ(1, v1.GetIndexOf(&v2));
  EXPECT_EQ(0, v1.GetIndexOf(&v3));
  v1.AddChildViewAt(&v2, 0);
  EXPECT_EQ(0, v1.GetIndexOf(&v2));
  EXPECT_EQ(1, v1.GetIndexOf(&v3));

  // Check that calling AddChildView() moves to the end.
  v1.AddChildView(&v2);
  EXPECT_EQ(1, v1.GetIndexOf(&v2));
  EXPECT_EQ(0, v1.GetIndexOf(&v3));
  v1.AddChildView(&v3);
  EXPECT_EQ(0, v1.GetIndexOf(&v2));
  EXPECT_EQ(1, v1.GetIndexOf(&v3));
}

////////////////////////////////////////////////////////////////////////////////
// FocusManager
////////////////////////////////////////////////////////////////////////////////

// A widget that always claims to be active, regardless of its real activation
// status.
class ActiveWidget : public Widget {
 public:
  ActiveWidget() = default;
  ~ActiveWidget() override = default;

  bool IsActive() const override { return true; }

 private:
  DISALLOW_COPY_AND_ASSIGN(ActiveWidget);
};

TEST_F(ViewTest, AdvanceFocusIfNecessaryForUnfocusableView) {
  // Create a widget with two views and give the first one focus.
  ActiveWidget widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  widget.Init(params);

  View* view1 = widget.GetRootView()->AddChildView(std::make_unique<View>());
  view1->SetFocusBehavior(View::FocusBehavior::ALWAYS);

  View* view2 = widget.GetRootView()->AddChildView(std::make_unique<View>());
  view2->SetFocusBehavior(View::FocusBehavior::ALWAYS);

  FocusManager* focus_manager = widget.GetFocusManager();
  ASSERT_TRUE(focus_manager);

  focus_manager->SetFocusedView(view1);
  EXPECT_EQ(view1, focus_manager->GetFocusedView());

  // Disable the focused view and check if the next view gets focused.
  view1->SetEnabled(false);
  EXPECT_EQ(view2, focus_manager->GetFocusedView());

  // Re-enable and re-focus.
  view1->SetEnabled(true);
  focus_manager->SetFocusedView(view1);
  EXPECT_EQ(view1, focus_manager->GetFocusedView());

  // Hide the focused view and check it the next view gets focused.
  view1->SetVisible(false);
  EXPECT_EQ(view2, focus_manager->GetFocusedView());

  // Re-show and re-focus.
  view1->SetVisible(true);
  focus_manager->SetFocusedView(view1);
  EXPECT_EQ(view1, focus_manager->GetFocusedView());

  // Set the focused view as not focusable and check if the next view gets
  // focused.
  view1->SetFocusBehavior(View::FocusBehavior::NEVER);
  EXPECT_EQ(view2, focus_manager->GetFocusedView());
}

////////////////////////////////////////////////////////////////////////////////
// Layers
////////////////////////////////////////////////////////////////////////////////

namespace {

// Test implementation of LayerAnimator.
class TestLayerAnimator : public ui::LayerAnimator {
 public:
  TestLayerAnimator();

  const gfx::Rect& last_bounds() const { return last_bounds_; }

  // LayerAnimator.
  void SetBounds(const gfx::Rect& bounds) override;

 protected:
  ~TestLayerAnimator() override = default;

 private:
  gfx::Rect last_bounds_;

  DISALLOW_COPY_AND_ASSIGN(TestLayerAnimator);
};

TestLayerAnimator::TestLayerAnimator()
    : ui::LayerAnimator(base::TimeDelta::FromMilliseconds(0)) {
}

void TestLayerAnimator::SetBounds(const gfx::Rect& bounds) {
  last_bounds_ = bounds;
}

}  // namespace

class ViewLayerTest : public ViewsTestBase {
 public:
  ViewLayerTest() = default;

  ~ViewLayerTest() override = default;

  // Returns the Layer used by the RootView.
  ui::Layer* GetRootLayer() {
    return widget()->GetLayer();
  }

  void SetUp() override {
    SetUpPixelCanvas();
    ViewTest::SetUp();
    widget_ = new Widget;
    Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
    params.bounds = gfx::Rect(50, 50, 200, 200);
    widget_->Init(params);
    widget_->Show();
    widget_->GetRootView()->SetBounds(0, 0, 200, 200);
  }

  void TearDown() override {
    widget_->CloseNow();
    ViewsTestBase::TearDown();
  }

  Widget* widget() { return widget_; }

  virtual void SetUpPixelCanvas() {
    scoped_feature_list_.InitAndDisableFeature(
        ::features::kEnablePixelCanvasRecording);
  }

 protected:
  // Accessors to View internals.
  void SchedulePaintOnParent(View* view) { view->SchedulePaintOnParent(); }

 private:
  Widget* widget_ = nullptr;
  base::test::ScopedFeatureList scoped_feature_list_;
};

TEST_F(ViewLayerTest, LayerCreationAndDestruction) {
  View view;
  EXPECT_EQ(nullptr, view.layer());

  view.SetPaintToLayer();
  EXPECT_NE(nullptr, view.layer());

  view.DestroyLayer();
  EXPECT_EQ(nullptr, view.layer());
}

TEST_F(ViewLayerTest, SetTransformCreatesAndDestroysLayer) {
  View view;
  EXPECT_EQ(nullptr, view.layer());

  // Set an arbitrary non-identity transform, which should cause a layer to be
  // created.
  gfx::Transform transform;
  transform.Translate(1.0, 1.0);
  view.SetTransform(transform);
  EXPECT_NE(nullptr, view.layer());

  // Set the identity transform, which should destroy the layer.
  view.SetTransform(gfx::Transform());
  EXPECT_EQ(nullptr, view.layer());
}

// Verify that setting an identity transform after SetPaintToLayer() has been
// called doesn't destroy the layer.
TEST_F(ViewLayerTest, IdentityTransformDoesntOverrideSetPaintToLayer) {
  View view;
  EXPECT_EQ(nullptr, view.layer());

  view.SetPaintToLayer();
  EXPECT_NE(nullptr, view.layer());

  gfx::Transform transform;
  transform.Translate(1.0, 1.0);
  view.SetTransform(transform);
  EXPECT_NE(nullptr, view.layer());

  view.SetTransform(transform);
  EXPECT_NE(nullptr, view.layer());
}

// Verify that calling DestroyLayer() while a non-identity transform is present
// doesn't destroy the layer.
TEST_F(ViewLayerTest, DestroyLayerDoesntOverrideTransform) {
  View view;
  EXPECT_EQ(nullptr, view.layer());

  view.SetPaintToLayer();
  EXPECT_NE(nullptr, view.layer());

  gfx::Transform transform;
  transform.Translate(1.0, 1.0);
  view.SetTransform(transform);
  EXPECT_NE(nullptr, view.layer());

  view.DestroyLayer();
  EXPECT_NE(nullptr, view.layer());
}

TEST_F(ViewLayerTest, LayerToggling) {
  // Because we lazily create textures the calls to DrawTree are necessary to
  // ensure we trigger creation of textures.
  ui::Layer* root_layer = widget()->GetLayer();
  View* content_view = new View;
  widget()->SetContentsView(content_view);

  // Create v1, give it a bounds and verify everything is set up correctly.
  View* v1 = new View;
  v1->SetPaintToLayer();
  EXPECT_TRUE(v1->layer() != nullptr);
  v1->SetBoundsRect(gfx::Rect(20, 30, 140, 150));
  content_view->AddChildView(v1);
  ASSERT_TRUE(v1->layer() != nullptr);
  EXPECT_EQ(root_layer, v1->layer()->parent());
  EXPECT_EQ(gfx::Rect(20, 30, 140, 150), v1->layer()->bounds());

  // Create v2 as a child of v1 and do basic assertion testing.
  View* v2 = new View;
  v1->AddChildView(v2);
  EXPECT_TRUE(v2->layer() == nullptr);
  v2->SetBoundsRect(gfx::Rect(10, 20, 30, 40));
  v2->SetPaintToLayer();
  ASSERT_TRUE(v2->layer() != nullptr);
  EXPECT_EQ(v1->layer(), v2->layer()->parent());
  EXPECT_EQ(gfx::Rect(10, 20, 30, 40), v2->layer()->bounds());

  // Turn off v1s layer. v2 should still have a layer but its parent should have
  // changed.
  v1->DestroyLayer();
  EXPECT_TRUE(v1->layer() == nullptr);
  EXPECT_TRUE(v2->layer() != nullptr);
  EXPECT_EQ(root_layer, v2->layer()->parent());
  ASSERT_EQ(1u, root_layer->children().size());
  EXPECT_EQ(root_layer->children()[0], v2->layer());
  // The bounds of the layer should have changed to be relative to the root view
  // now.
  EXPECT_EQ(gfx::Rect(30, 50, 30, 40), v2->layer()->bounds());

  // Make v1 have a layer again and verify v2s layer is wired up correctly.
  gfx::Transform transform;
  transform.Scale(2.0, 2.0);
  v1->SetTransform(transform);
  EXPECT_TRUE(v1->layer() != nullptr);
  EXPECT_TRUE(v2->layer() != nullptr);
  EXPECT_EQ(root_layer, v1->layer()->parent());
  EXPECT_EQ(v1->layer(), v2->layer()->parent());
  ASSERT_EQ(1u, root_layer->children().size());
  EXPECT_EQ(root_layer->children()[0], v1->layer());
  ASSERT_EQ(1u, v1->layer()->children().size());
  EXPECT_EQ(v1->layer()->children()[0], v2->layer());
  EXPECT_EQ(gfx::Rect(10, 20, 30, 40), v2->layer()->bounds());
}

// Verifies turning on a layer wires up children correctly.
TEST_F(ViewLayerTest, NestedLayerToggling) {
  View* content_view = new View;
  widget()->SetContentsView(content_view);

  // Create v1, give it a bounds and verify everything is set up correctly.
  View* v1 = content_view->AddChildView(std::make_unique<View>());
  v1->SetBoundsRect(gfx::Rect(20, 30, 140, 150));

  View* v2 = v1->AddChildView(std::make_unique<View>());

  View* v3 = v2->AddChildView(std::make_unique<View>());
  v3->SetPaintToLayer();
  ASSERT_TRUE(v3->layer() != nullptr);

  // At this point we have v1-v2-v3. v3 has a layer, v1 and v2 don't.

  v1->SetPaintToLayer();
  EXPECT_EQ(v1->layer(), v3->layer()->parent());
}

TEST_F(ViewLayerTest, LayerAnimator) {
  View* content_view = new View;
  widget()->SetContentsView(content_view);

  View* v1 = content_view->AddChildView(std::make_unique<View>());
  v1->SetPaintToLayer();
  EXPECT_TRUE(v1->layer() != nullptr);

  TestLayerAnimator* animator = new TestLayerAnimator();
  v1->layer()->SetAnimator(animator);

  gfx::Rect bounds(1, 2, 3, 4);
  v1->SetBoundsRect(bounds);
  EXPECT_EQ(bounds, animator->last_bounds());
  // TestLayerAnimator doesn't update the layer.
  EXPECT_NE(bounds, v1->layer()->bounds());
}

// Verifies the bounds of a layer are updated if the bounds of ancestor that
// doesn't have a layer change.
TEST_F(ViewLayerTest, BoundsChangeWithLayer) {
  View* content_view = new View;
  widget()->SetContentsView(content_view);

  View* v1 = content_view->AddChildView(std::make_unique<View>());
  v1->SetBoundsRect(gfx::Rect(20, 30, 140, 150));

  View* v2 = v1->AddChildView(std::make_unique<View>());
  v2->SetBoundsRect(gfx::Rect(10, 11, 40, 50));
  v2->SetPaintToLayer();
  ASSERT_TRUE(v2->layer() != nullptr);
  EXPECT_EQ(gfx::Rect(30, 41, 40, 50), v2->layer()->bounds());

  v1->SetPosition(gfx::Point(25, 36));
  EXPECT_EQ(gfx::Rect(35, 47, 40, 50), v2->layer()->bounds());

  v2->SetPosition(gfx::Point(11, 12));
  EXPECT_EQ(gfx::Rect(36, 48, 40, 50), v2->layer()->bounds());

  // Bounds of the layer should change even if the view is not invisible.
  v1->SetVisible(false);
  v1->SetPosition(gfx::Point(20, 30));
  EXPECT_EQ(gfx::Rect(31, 42, 40, 50), v2->layer()->bounds());

  v2->SetVisible(false);
  v2->SetBoundsRect(gfx::Rect(10, 11, 20, 30));
  EXPECT_EQ(gfx::Rect(30, 41, 20, 30), v2->layer()->bounds());
}

// Make sure layers are positioned correctly in RTL.
TEST_F(ViewLayerTest, BoundInRTL) {
  base::test::ScopedRestoreICUDefaultLocale scoped_locale_("he");
  View* view = new View;
  widget()->SetContentsView(view);

  int content_width = view->width();

  // |v1| is initially not attached to anything. So its layer will have the same
  // bounds as the view.
  View* v1 = new View;
  v1->SetPaintToLayer();
  v1->SetBounds(10, 10, 20, 10);
  EXPECT_EQ(gfx::Rect(10, 10, 20, 10),
            v1->layer()->bounds());

  // Once |v1| is attached to the widget, its layer will get RTL-appropriate
  // bounds.
  view->AddChildView(v1);
  EXPECT_EQ(gfx::Rect(content_width - 30, 10, 20, 10),
            v1->layer()->bounds());
  gfx::Rect l1bounds = v1->layer()->bounds();

  // Now attach a View to the widget first, then create a layer for it. Make
  // sure the bounds are correct.
  View* v2 = new View;
  v2->SetBounds(50, 10, 30, 10);
  EXPECT_FALSE(v2->layer());
  view->AddChildView(v2);
  v2->SetPaintToLayer();
  EXPECT_EQ(gfx::Rect(content_width - 80, 10, 30, 10),
            v2->layer()->bounds());
  gfx::Rect l2bounds = v2->layer()->bounds();

  view->SetPaintToLayer();
  EXPECT_EQ(l1bounds, v1->layer()->bounds());
  EXPECT_EQ(l2bounds, v2->layer()->bounds());

  // Move one of the views. Make sure the layer is positioned correctly
  // afterwards.
  v1->SetBounds(v1->x() - 5, v1->y(), v1->width(), v1->height());
  l1bounds.set_x(l1bounds.x() + 5);
  EXPECT_EQ(l1bounds, v1->layer()->bounds());

  view->DestroyLayer();
  EXPECT_EQ(l1bounds, v1->layer()->bounds());
  EXPECT_EQ(l2bounds, v2->layer()->bounds());

  // Move a view again.
  v2->SetBounds(v2->x() + 5, v2->y(), v2->width(), v2->height());
  l2bounds.set_x(l2bounds.x() - 5);
  EXPECT_EQ(l2bounds, v2->layer()->bounds());
}

// Make sure that resizing a parent in RTL correctly repositions its children.
TEST_F(ViewLayerTest, ResizeParentInRTL) {
  base::test::ScopedRestoreICUDefaultLocale scoped_locale_("he");
  View* view = new View;
  widget()->SetContentsView(view);

  int content_width = view->width();

  // Create a paints-to-layer view |v1|.
  View* v1 = view->AddChildView(std::make_unique<View>());
  v1->SetPaintToLayer();
  v1->SetBounds(10, 10, 20, 10);
  EXPECT_EQ(gfx::Rect(content_width - 30, 10, 20, 10),
            v1->layer()->bounds());

  // Attach a paints-to-layer child view to |v1|.
  View* v2 = new View;
  v2->SetPaintToLayer();
  v2->SetBounds(3, 5, 6, 4);
  EXPECT_EQ(gfx::Rect(3, 5, 6, 4),
            v2->layer()->bounds());
  v1->AddChildView(v2);
  // Check that |v2| now has RTL-appropriate bounds.
  EXPECT_EQ(gfx::Rect(11, 5, 6, 4),
            v2->layer()->bounds());

  // Attach a non-layer child view to |v1|, and give it a paints-to-layer child.
  View* v3 = new View;
  v3->SetBounds(1, 1, 18, 8);
  View* v4 = new View;
  v4->SetPaintToLayer();
  v4->SetBounds(2, 4, 6, 4);
  EXPECT_EQ(gfx::Rect(2, 4, 6, 4),
            v4->layer()->bounds());
  v3->AddChildView(v4);
  EXPECT_EQ(gfx::Rect(10, 4, 6, 4),
            v4->layer()->bounds());
  v1->AddChildView(v3);
  // Check that |v4| now has RTL-appropriate bounds.
  EXPECT_EQ(gfx::Rect(11, 5, 6, 4),
            v4->layer()->bounds());

  // Resize |v1|. Make sure that |v2| and |v4|'s layers have been moved
  // correctly to RTL-appropriate bounds.
  v1->SetSize(gfx::Size(30, 10));
  EXPECT_EQ(gfx::Rect(21, 5, 6, 4),
            v2->layer()->bounds());
  EXPECT_EQ(gfx::Rect(21, 5, 6, 4),
            v4->layer()->bounds());

  // Move and resize |v3|. Make sure that |v4|'s layer has been moved correctly
  // to RTL-appropriate bounds.
  v3->SetBounds(2, 1, 12, 8);
  EXPECT_EQ(gfx::Rect(20, 5, 6, 4),
            v4->layer()->bounds());
}

// Makes sure a transform persists after toggling the visibility.
TEST_F(ViewLayerTest, ToggleVisibilityWithTransform) {
  View* view = new View;
  gfx::Transform transform;
  transform.Scale(2.0, 2.0);
  view->SetTransform(transform);
  widget()->SetContentsView(view);
  EXPECT_EQ(2.0f, view->GetTransform().matrix().get(0, 0));

  view->SetVisible(false);
  EXPECT_EQ(2.0f, view->GetTransform().matrix().get(0, 0));

  view->SetVisible(true);
  EXPECT_EQ(2.0f, view->GetTransform().matrix().get(0, 0));
}

// Verifies a transform persists after removing/adding a view with a transform.
TEST_F(ViewLayerTest, ResetTransformOnLayerAfterAdd) {
  View* view = new View;
  gfx::Transform transform;
  transform.Scale(2.0, 2.0);
  view->SetTransform(transform);
  widget()->SetContentsView(view);
  EXPECT_EQ(2.0f, view->GetTransform().matrix().get(0, 0));
  ASSERT_TRUE(view->layer() != nullptr);
  EXPECT_EQ(2.0f, view->layer()->transform().matrix().get(0, 0));

  View* parent = view->parent();
  parent->RemoveChildView(view);
  parent->AddChildView(view);

  EXPECT_EQ(2.0f, view->GetTransform().matrix().get(0, 0));
  ASSERT_TRUE(view->layer() != nullptr);
  EXPECT_EQ(2.0f, view->layer()->transform().matrix().get(0, 0));
}

// Makes sure that layer visibility is correct after toggling View visibility.
TEST_F(ViewLayerTest, ToggleVisibilityWithLayer) {
  View* content_view = new View;
  widget()->SetContentsView(content_view);

  // The view isn't attached to a widget or a parent view yet. But it should
  // still have a layer, but the layer should not be attached to the root
  // layer.
  View* v1 = new View;
  v1->SetPaintToLayer();
  EXPECT_TRUE(v1->layer());
  EXPECT_FALSE(LayerIsAncestor(widget()->GetCompositor()->root_layer(),
                               v1->layer()));

  // Once the view is attached to a widget, its layer should be attached to the
  // root layer and visible.
  content_view->AddChildView(v1);
  EXPECT_TRUE(LayerIsAncestor(widget()->GetCompositor()->root_layer(),
                              v1->layer()));
  EXPECT_TRUE(v1->layer()->IsDrawn());

  v1->SetVisible(false);
  EXPECT_FALSE(v1->layer()->IsDrawn());

  v1->SetVisible(true);
  EXPECT_TRUE(v1->layer()->IsDrawn());

  widget()->Hide();
  EXPECT_FALSE(v1->layer()->IsDrawn());

  widget()->Show();
  EXPECT_TRUE(v1->layer()->IsDrawn());
}

// Tests that the layers in the subtree are orphaned after a View is removed
// from the parent.
TEST_F(ViewLayerTest, OrphanLayerAfterViewRemove) {
  View* content_view = new View;
  widget()->SetContentsView(content_view);

  View* v1 = new View;
  content_view->AddChildView(v1);

  View* v2 = new View;
  v1->AddChildView(v2);
  v2->SetPaintToLayer();
  EXPECT_TRUE(LayerIsAncestor(widget()->GetCompositor()->root_layer(),
                              v2->layer()));
  EXPECT_TRUE(v2->layer()->IsDrawn());

  content_view->RemoveChildView(v1);

  EXPECT_FALSE(LayerIsAncestor(widget()->GetCompositor()->root_layer(),
                               v2->layer()));

  // Reparent |v2|.
  content_view->AddChildView(v2);
  delete v1;
  v1 = nullptr;
  EXPECT_TRUE(LayerIsAncestor(widget()->GetCompositor()->root_layer(),
                              v2->layer()));
  EXPECT_TRUE(v2->layer()->IsDrawn());
}

class PaintTrackingView : public View {
 public:
  PaintTrackingView() = default;

  bool painted() const { return painted_; }
  void set_painted(bool value) { painted_ = value; }

  void OnPaint(gfx::Canvas* canvas) override { painted_ = true; }

 private:
  bool painted_ = false;

  DISALLOW_COPY_AND_ASSIGN(PaintTrackingView);
};

// Makes sure child views with layers aren't painted when paint starts at an
// ancestor.
TEST_F(ViewLayerTest, DontPaintChildrenWithLayers) {
  PaintTrackingView* content_view = new PaintTrackingView;
  widget()->SetContentsView(content_view);
  content_view->SetPaintToLayer();
  GetRootLayer()->GetCompositor()->ScheduleDraw();
  ui::DrawWaiterForTest::WaitForCompositingEnded(
      GetRootLayer()->GetCompositor());
  GetRootLayer()->SchedulePaint(gfx::Rect(0, 0, 10, 10));
  content_view->set_painted(false);
  // content_view no longer has a dirty rect. Paint from the root and make sure
  // PaintTrackingView isn't painted.
  GetRootLayer()->GetCompositor()->ScheduleDraw();
  ui::DrawWaiterForTest::WaitForCompositingEnded(
      GetRootLayer()->GetCompositor());
  EXPECT_FALSE(content_view->painted());

  // Make content_view have a dirty rect, paint the layers and make sure
  // PaintTrackingView is painted.
  content_view->layer()->SchedulePaint(gfx::Rect(0, 0, 10, 10));
  GetRootLayer()->GetCompositor()->ScheduleDraw();
  ui::DrawWaiterForTest::WaitForCompositingEnded(
      GetRootLayer()->GetCompositor());
  EXPECT_TRUE(content_view->painted());
}

TEST_F(ViewLayerTest, NoCrashWhenParentlessViewSchedulesPaintOnParent) {
  TestView v;
  SchedulePaintOnParent(&v);
}

TEST_F(ViewLayerTest, ScheduledRectsInParentAfterSchedulingPaint) {
  TestView parent_view;
  parent_view.SetBounds(10, 10, 100, 100);

  TestView* child_view = parent_view.AddChildView(std::make_unique<TestView>());
  child_view->SetBounds(5, 6, 10, 20);

  parent_view.scheduled_paint_rects_.clear();
  SchedulePaintOnParent(child_view);
  ASSERT_EQ(1U, parent_view.scheduled_paint_rects_.size());
  EXPECT_EQ(gfx::Rect(5, 6, 10, 20),
            parent_view.scheduled_paint_rects_.front());
}

TEST_F(ViewLayerTest, ParentPaintWhenSwitchingPaintToLayerFromFalseToTrue) {
  TestView parent_view;
  parent_view.SetBounds(10, 11, 12, 13);

  TestView* child_view = parent_view.AddChildView(std::make_unique<TestView>());

  parent_view.scheduled_paint_rects_.clear();
  child_view->SetPaintToLayer();
  EXPECT_EQ(1U, parent_view.scheduled_paint_rects_.size());
}

TEST_F(ViewLayerTest, NoParentPaintWhenSwitchingPaintToLayerFromTrueToTrue) {
  TestView parent_view;
  parent_view.SetBounds(10, 11, 12, 13);

  TestView* child_view = parent_view.AddChildView(std::make_unique<TestView>());
  child_view->SetPaintToLayer();

  parent_view.scheduled_paint_rects_.clear();
  EXPECT_EQ(0U, parent_view.scheduled_paint_rects_.size());
}

// Tests that the visibility of child layers are updated correctly when a View's
// visibility changes.
TEST_F(ViewLayerTest, VisibilityChildLayers) {
  View* v1 = new View;
  v1->SetPaintToLayer();
  widget()->SetContentsView(v1);

  View* v2 = v1->AddChildView(std::make_unique<View>());

  View* v3 = v2->AddChildView(std::make_unique<View>());
  v3->SetVisible(false);

  View* v4 = v3->AddChildView(std::make_unique<View>());
  v4->SetPaintToLayer();

  EXPECT_TRUE(v1->layer()->IsDrawn());
  EXPECT_FALSE(v4->layer()->IsDrawn());

  v2->SetVisible(false);
  EXPECT_TRUE(v1->layer()->IsDrawn());
  EXPECT_FALSE(v4->layer()->IsDrawn());

  v2->SetVisible(true);
  EXPECT_TRUE(v1->layer()->IsDrawn());
  EXPECT_FALSE(v4->layer()->IsDrawn());

  v2->SetVisible(false);
  EXPECT_TRUE(v1->layer()->IsDrawn());
  EXPECT_FALSE(v4->layer()->IsDrawn());
  EXPECT_TRUE(ViewAndLayerTreeAreConsistent(v1, v1->layer()));

  v3->SetVisible(true);
  EXPECT_TRUE(v1->layer()->IsDrawn());
  EXPECT_FALSE(v4->layer()->IsDrawn());
  EXPECT_TRUE(ViewAndLayerTreeAreConsistent(v1, v1->layer()));

  // Reparent |v3| to |v1|.
  v1->AddChildView(v3);
  EXPECT_TRUE(v1->layer()->IsDrawn());
  EXPECT_TRUE(v4->layer()->IsDrawn());
  EXPECT_TRUE(ViewAndLayerTreeAreConsistent(v1, v1->layer()));
}

// This test creates a random View tree, and then randomly reorders child views,
// reparents views etc. Unrelated changes can appear to break this test. So
// marking this as FLAKY.
TEST_F(ViewLayerTest, DISABLED_ViewLayerTreesInSync) {
  View* content = new View;
  content->SetPaintToLayer();
  widget()->SetContentsView(content);
  widget()->Show();

  ConstructTree(content, 5);
  EXPECT_TRUE(ViewAndLayerTreeAreConsistent(content, content->layer()));

  ScrambleTree(content);
  EXPECT_TRUE(ViewAndLayerTreeAreConsistent(content, content->layer()));

  ScrambleTree(content);
  EXPECT_TRUE(ViewAndLayerTreeAreConsistent(content, content->layer()));

  ScrambleTree(content);
  EXPECT_TRUE(ViewAndLayerTreeAreConsistent(content, content->layer()));
}

// Verifies when views are reordered the layer is also reordered. The widget is
// providing the parent layer.
TEST_F(ViewLayerTest, ReorderUnderWidget) {
  View* content = new View;
  widget()->SetContentsView(content);
  View* c1 = content->AddChildView(std::make_unique<View>());
  c1->SetPaintToLayer();
  View* c2 = content->AddChildView(std::make_unique<View>());
  c2->SetPaintToLayer();

  ui::Layer* parent_layer = c1->layer()->parent();
  ASSERT_TRUE(parent_layer);
  ASSERT_EQ(2u, parent_layer->children().size());
  EXPECT_EQ(c1->layer(), parent_layer->children()[0]);
  EXPECT_EQ(c2->layer(), parent_layer->children()[1]);

  // Move c1 to the front. The layers should have moved too.
  content->ReorderChildView(c1, -1);
  EXPECT_EQ(c1->layer(), parent_layer->children()[1]);
  EXPECT_EQ(c2->layer(), parent_layer->children()[0]);
}

// Verifies that the layer of a view can be acquired properly.
TEST_F(ViewLayerTest, AcquireLayer) {
  View* content = new View;
  widget()->SetContentsView(content);
  std::unique_ptr<View> c1(new View);
  c1->SetPaintToLayer();
  EXPECT_TRUE(c1->layer());
  content->AddChildView(c1.get());

  std::unique_ptr<ui::Layer> layer(c1->AcquireLayer());
  EXPECT_EQ(layer.get(), c1->layer());

  std::unique_ptr<ui::Layer> layer2(c1->RecreateLayer());
  EXPECT_NE(c1->layer(), layer2.get());

  // Destroy view before destroying layer.
  c1.reset();
}

// Verify the z-order of the layers as a result of calling RecreateLayer().
TEST_F(ViewLayerTest, RecreateLayerZOrder) {
  std::unique_ptr<View> v(new View());
  v->SetPaintToLayer();

  View* v1 = v->AddChildView(std::make_unique<View>());
  v1->SetPaintToLayer();
  View* v2 = v->AddChildView(std::make_unique<View>());
  v2->SetPaintToLayer();

  // Test the initial z-order.
  const std::vector<ui::Layer*>& child_layers_pre = v->layer()->children();
  ASSERT_EQ(2u, child_layers_pre.size());
  EXPECT_EQ(v1->layer(), child_layers_pre[0]);
  EXPECT_EQ(v2->layer(), child_layers_pre[1]);

  std::unique_ptr<ui::Layer> v1_old_layer(v1->RecreateLayer());

  // Test the new layer order. We expect: |v1| |v1_old_layer| |v2|.
  // for |v1| and |v2|.
  const std::vector<ui::Layer*>& child_layers_post = v->layer()->children();
  ASSERT_EQ(3u, child_layers_post.size());
  EXPECT_EQ(v1->layer(), child_layers_post[0]);
  EXPECT_EQ(v1_old_layer.get(), child_layers_post[1]);
  EXPECT_EQ(v2->layer(), child_layers_post[2]);
}

// Verify the z-order of the layers as a result of calling RecreateLayer when
// the widget is the parent with the layer.
TEST_F(ViewLayerTest, RecreateLayerZOrderWidgetParent) {
  View* v = new View();
  widget()->SetContentsView(v);

  View* v1 = v->AddChildView(std::make_unique<View>());
  v1->SetPaintToLayer();
  View* v2 = v->AddChildView(std::make_unique<View>());
  v2->SetPaintToLayer();

  ui::Layer* root_layer = GetRootLayer();

  // Test the initial z-order.
  const std::vector<ui::Layer*>& child_layers_pre = root_layer->children();
  ASSERT_EQ(2u, child_layers_pre.size());
  EXPECT_EQ(v1->layer(), child_layers_pre[0]);
  EXPECT_EQ(v2->layer(), child_layers_pre[1]);

  std::unique_ptr<ui::Layer> v1_old_layer(v1->RecreateLayer());

  // Test the new layer order. We expect: |v1| |v1_old_layer| |v2|.
  const std::vector<ui::Layer*>& child_layers_post = root_layer->children();
  ASSERT_EQ(3u, child_layers_post.size());
  EXPECT_EQ(v1->layer(), child_layers_post[0]);
  EXPECT_EQ(v1_old_layer.get(), child_layers_post[1]);
  EXPECT_EQ(v2->layer(), child_layers_post[2]);
}

// Verifies RecreateLayer() moves all Layers over, even those that don't have
// a View.
TEST_F(ViewLayerTest, RecreateLayerMovesNonViewChildren) {
  View v;
  v.SetPaintToLayer();
  View child;
  child.SetPaintToLayer();
  v.AddChildView(&child);
  ASSERT_TRUE(v.layer() != nullptr);
  ASSERT_EQ(1u, v.layer()->children().size());
  EXPECT_EQ(v.layer()->children()[0], child.layer());

  ui::Layer layer(ui::LAYER_NOT_DRAWN);
  v.layer()->Add(&layer);
  v.layer()->StackAtBottom(&layer);

  std::unique_ptr<ui::Layer> old_layer(v.RecreateLayer());

  // All children should be moved from old layer to new layer.
  ASSERT_TRUE(old_layer.get() != nullptr);
  EXPECT_TRUE(old_layer->children().empty());

  // And new layer should have the two children.
  ASSERT_TRUE(v.layer() != nullptr);
  ASSERT_EQ(2u, v.layer()->children().size());
  EXPECT_EQ(v.layer()->children()[0], &layer);
  EXPECT_EQ(v.layer()->children()[1], child.layer());
}

namespace {

std::string ToString(const gfx::Vector2dF& vector) {
  // Explicitly round it because linux uses banker's rounding
  // while Windows is using "away-from-zero" in printf.
  return base::StringPrintf("%0.2f %0.2f", std::round(vector.x() * 100) / 100.f,
                            std::round(vector.y() * 100) / 100.f);
}

}  // namespace

TEST_F(ViewLayerTest, SnapLayerToPixel) {
  viz::ParentLocalSurfaceIdAllocator allocator;
  allocator.GenerateId();
  View* v1 = new View;

  View* v11 = v1->AddChildView(std::make_unique<View>());

  widget()->SetContentsView(v1);

  const gfx::Size& size = GetRootLayer()->GetCompositor()->size();
  GetRootLayer()->GetCompositor()->SetScaleAndSize(
      1.25f, size, allocator.GetCurrentLocalSurfaceIdAllocation());

  v11->SetBoundsRect(gfx::Rect(1, 1, 10, 10));
  v1->SetBoundsRect(gfx::Rect(1, 1, 10, 10));
  v11->SetPaintToLayer();

  EXPECT_EQ("0.40 0.40", ToString(v11->layer()->subpixel_position_offset()));

  // Creating a layer in parent should update the child view's layer offset.
  v1->SetPaintToLayer();
  EXPECT_EQ("-0.20 -0.20", ToString(v1->layer()->subpixel_position_offset()));
  EXPECT_EQ("-0.20 -0.20", ToString(v11->layer()->subpixel_position_offset()));

  // DSF change should get propagated and update offsets.
  GetRootLayer()->GetCompositor()->SetScaleAndSize(
      1.5f, size, allocator.GetCurrentLocalSurfaceIdAllocation());
  EXPECT_EQ("0.33 0.33", ToString(v1->layer()->subpixel_position_offset()));
  EXPECT_EQ("0.33 0.33", ToString(v11->layer()->subpixel_position_offset()));

  // Deleting parent's layer should update the child view's layer's offset.
  v1->DestroyLayer();
  EXPECT_EQ("0.00 0.00", ToString(v11->layer()->subpixel_position_offset()));

  // Setting parent view should update the child view's layer's offset.
  v1->SetBoundsRect(gfx::Rect(2, 2, 10, 10));
  EXPECT_EQ("0.33 0.33", ToString(v11->layer()->subpixel_position_offset()));

  // Setting integral DSF should reset the offset.
  GetRootLayer()->GetCompositor()->SetScaleAndSize(
      2.0f, size, allocator.GetCurrentLocalSurfaceIdAllocation());
  EXPECT_EQ("0.00 0.00", ToString(v11->layer()->subpixel_position_offset()));
}

namespace {

class PaintLayerView : public View {
 public:
  PaintLayerView() = default;

  void PaintChildren(const PaintInfo& info) override {
    last_paint_info_ = std::make_unique<PaintInfo>(info);
    View::PaintChildren(info);
  }

  std::unique_ptr<PaintInfo> GetLastPaintInfo() {
    return std::move(last_paint_info_);
  }

 private:
  std::unique_ptr<PaintInfo> last_paint_info_;

  DISALLOW_COPY_AND_ASSIGN(PaintLayerView);
};

}  // namespace

class ViewLayerPixelCanvasTest : public ViewLayerTest {
 public:
  ViewLayerPixelCanvasTest() = default;

  ~ViewLayerPixelCanvasTest() override = default;

  void SetUpPixelCanvas() override {
    scoped_feature_list_.InitAndEnableFeature(
        ::features::kEnablePixelCanvasRecording);
  }

  // Test if the recording rects are same with and without layer.
  void PaintRecordingSizeTest(PaintLayerView* v3,
                              const gfx::Size& expected_size) {
    v3->DestroyLayer();
    ui::Compositor* compositor = widget()->GetCompositor();
    auto list = base::MakeRefCounted<cc::DisplayItemList>();
    ui::PaintContext context(list.get(), compositor->device_scale_factor(),
                             gfx::Rect(compositor->size()), true);
    widget()->GetRootView()->PaintFromPaintRoot(context);
    EXPECT_EQ(expected_size, v3->GetLastPaintInfo()->paint_recording_size());
    v3->SetPaintToLayer();
    v3->OnPaintLayer(context);
    EXPECT_EQ(expected_size, v3->GetLastPaintInfo()->paint_recording_size());
  }

 private:
  base::test::ScopedFeatureList scoped_feature_list_;

  DISALLOW_COPY_AND_ASSIGN(ViewLayerPixelCanvasTest);
};

TEST_F(ViewLayerPixelCanvasTest, SnapLayerToPixel) {
  viz::ParentLocalSurfaceIdAllocator allocator;
  allocator.GenerateId();
  View* v1 = new View;
  View* v2 = v1->AddChildView(std::make_unique<View>());
  PaintLayerView* v3 = v2->AddChildView(std::make_unique<PaintLayerView>());

  widget()->SetContentsView(v1);

  const gfx::Size& size = GetRootLayer()->GetCompositor()->size();
  GetRootLayer()->GetCompositor()->SetScaleAndSize(
      1.6f, size, allocator.GetCurrentLocalSurfaceIdAllocation());

  v3->SetBoundsRect(gfx::Rect(14, 13, 13, 5));
  v2->SetBoundsRect(gfx::Rect(7, 7, 50, 50));
  v1->SetBoundsRect(gfx::Rect(9, 9, 100, 100));

  PaintRecordingSizeTest(v3, gfx::Size(21, 8));  // Enclosing Rect = (21, 8)
  EXPECT_EQ("-0.63 -0.25", ToString(v3->layer()->subpixel_position_offset()));

  // Creating a layer in parent should update the child view's layer offset.
  v1->SetPaintToLayer();
  EXPECT_EQ("-0.25 -0.25", ToString(v1->layer()->subpixel_position_offset()));
  EXPECT_EQ("-0.37 -0.00", ToString(v3->layer()->subpixel_position_offset()));

  // DSF change should get propagated and update offsets.
  GetRootLayer()->GetCompositor()->SetScaleAndSize(
      1.5f, size, allocator.GetCurrentLocalSurfaceIdAllocation());

  EXPECT_EQ("0.33 0.33", ToString(v1->layer()->subpixel_position_offset()));
  EXPECT_EQ("0.33 0.67", ToString(v3->layer()->subpixel_position_offset()));

  v1->DestroyLayer();
  PaintRecordingSizeTest(v3, gfx::Size(20, 7));  // Enclosing Rect = (20, 8)
  v1->SetPaintToLayer();

  GetRootLayer()->GetCompositor()->SetScaleAndSize(
      1.33f, size, allocator.GetCurrentLocalSurfaceIdAllocation());

  EXPECT_EQ("0.02 0.02", ToString(v1->layer()->subpixel_position_offset()));
  EXPECT_EQ("0.05 -0.45", ToString(v3->layer()->subpixel_position_offset()));

  v1->DestroyLayer();
  PaintRecordingSizeTest(v3, gfx::Size(17, 7));  // Enclosing Rect = (18, 7)

  // Deleting parent's layer should update the child view's layer's offset.
  EXPECT_EQ("0.08 -0.43", ToString(v3->layer()->subpixel_position_offset()));

  // Setting parent view should update the child view's layer's offset.
  v1->SetBoundsRect(gfx::Rect(3, 3, 10, 10));
  EXPECT_EQ("0.06 -0.44", ToString(v3->layer()->subpixel_position_offset()));

  // Setting integral DSF should reset the offset.
  GetRootLayer()->GetCompositor()->SetScaleAndSize(
      2.0f, size, allocator.GetCurrentLocalSurfaceIdAllocation());
  EXPECT_EQ("0.00 0.00", ToString(v3->layer()->subpixel_position_offset()));
}

TEST_F(ViewTest, FocusableAssertions) {
  // View subclasses may change insets based on whether they are focusable,
  // which effects the preferred size. To avoid preferred size changing around
  // these Views need to key off the last value set to SetFocusBehavior(), not
  // whether the View is focusable right now. For this reason it's important
  // that the return value of focus_behavior() depends on the last value passed
  // to SetFocusBehavior and not whether the View is focusable right now.
  TestView view;
  view.SetFocusBehavior(View::FocusBehavior::ALWAYS);
  EXPECT_EQ(View::FocusBehavior::ALWAYS, view.focus_behavior());
  view.SetEnabled(false);
  EXPECT_EQ(View::FocusBehavior::ALWAYS, view.focus_behavior());
  view.SetFocusBehavior(View::FocusBehavior::NEVER);
  EXPECT_EQ(View::FocusBehavior::NEVER, view.focus_behavior());
  view.SetFocusBehavior(View::FocusBehavior::ACCESSIBLE_ONLY);
  EXPECT_EQ(View::FocusBehavior::ACCESSIBLE_ONLY, view.focus_behavior());
}

////////////////////////////////////////////////////////////////////////////////
// NativeTheme
////////////////////////////////////////////////////////////////////////////////

void TestView::OnThemeChanged() {
  native_theme_ = GetNativeTheme();
}

TEST_F(ViewTest, OnThemeChanged) {
  TestView* test_view = new TestView();
  EXPECT_FALSE(test_view->native_theme_);

  // Child view added before the widget hierarchy exists should get the
  // new native theme notification.
  TestView* test_view_child =
      test_view->AddChildView(std::make_unique<TestView>());
  EXPECT_FALSE(test_view_child->native_theme_);

  std::unique_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_WINDOW);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  widget->Init(params);

  widget->GetRootView()->AddChildView(test_view);
  EXPECT_TRUE(test_view->native_theme_);
  EXPECT_EQ(widget->GetNativeTheme(), test_view->native_theme_);
  EXPECT_TRUE(test_view_child->native_theme_);
  EXPECT_EQ(widget->GetNativeTheme(), test_view_child->native_theme_);

  // Child view added after the widget hierarchy exists should also get the
  // notification.
  TestView* test_view_child_2 =
      test_view->AddChildView(std::make_unique<TestView>());
  EXPECT_TRUE(test_view_child_2->native_theme_);
  EXPECT_EQ(widget->GetNativeTheme(), test_view_child_2->native_theme_);

  widget->CloseNow();
}

class TestEventHandler : public ui::EventHandler {
 public:
  explicit TestEventHandler(TestView* view)
      : view_(view), had_mouse_event_(false) {}
  ~TestEventHandler() override = default;

  void OnMouseEvent(ui::MouseEvent* event) override {
    // The |view_| should have received the event first.
    EXPECT_EQ(ui::ET_MOUSE_PRESSED, view_->last_mouse_event_type_);
    had_mouse_event_ = true;
  }

  TestView* view_;
  bool had_mouse_event_;
};

TEST_F(ViewTest, ScopedTargetHandlerReceivesEvents) {
  std::unique_ptr<Widget> widget(new Widget);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(50, 50, 350, 350);
  widget->Init(params);
  View* root = widget->GetRootView();
  TestView* v = root->AddChildView(std::make_unique<TestView>());
  v->SetBoundsRect(gfx::Rect(0, 0, 300, 300));
  v->Reset();
  {
    TestEventHandler handler(v);
    ui::ScopedTargetHandler scoped_target_handler(v, &handler);
    // View's target EventHandler should be set to the |scoped_target_handler|.
    EXPECT_EQ(&scoped_target_handler,
              v->SetTargetHandler(&scoped_target_handler));

    EXPECT_EQ(ui::ET_UNKNOWN, v->last_mouse_event_type_);
    gfx::Point p(10, 120);
    ui::MouseEvent pressed(ui::ET_MOUSE_PRESSED, p, p, ui::EventTimeForNow(),
                           ui::EF_LEFT_MOUSE_BUTTON, ui::EF_LEFT_MOUSE_BUTTON);
    root->OnMousePressed(pressed);

    // Both the View |v| and the |handler| should have received the event.
    EXPECT_EQ(ui::ET_MOUSE_PRESSED, v->last_mouse_event_type_);
    EXPECT_TRUE(handler.had_mouse_event_);
  }

  // The View should continue receiving events after the |handler| is deleted.
  v->Reset();
  ui::MouseEvent released(ui::ET_MOUSE_RELEASED, gfx::Point(), gfx::Point(),
                          ui::EventTimeForNow(), 0, 0);
  root->OnMouseReleased(released);
  EXPECT_EQ(ui::ET_MOUSE_RELEASED, v->last_mouse_event_type_);
}

// See comment above test for details.
class WidgetWithCustomTheme : public Widget {
 public:
  explicit WidgetWithCustomTheme(ui::NativeTheme* theme) : theme_(theme) {}
  ~WidgetWithCustomTheme() override = default;

  // Widget:
  const ui::NativeTheme* GetNativeTheme() const override { return theme_; }

 private:
  ui::NativeTheme* theme_;

  DISALLOW_COPY_AND_ASSIGN(WidgetWithCustomTheme);
};

// See comment above test for details.
class ViewThatAddsViewInOnThemeChanged : public View {
 public:
  ViewThatAddsViewInOnThemeChanged() { SetPaintToLayer(); }
  ~ViewThatAddsViewInOnThemeChanged() override = default;

  bool on_native_theme_changed_called() const {
    return on_native_theme_changed_called_;
  }

  // View:
  void OnThemeChanged() override {
    on_native_theme_changed_called_ = true;
    GetWidget()->GetRootView()->AddChildView(std::make_unique<View>());
  }

 private:
  bool on_native_theme_changed_called_ = false;

  DISALLOW_COPY_AND_ASSIGN(ViewThatAddsViewInOnThemeChanged);
};

// Creates and adds a new child view to |parent| that has a layer.
void AddViewWithChildLayer(View* parent) {
  View* child = parent->AddChildView(std::make_unique<View>());
  child->SetPaintToLayer();
}

// This test does the following:
// . creates a couple of views with layers added to the root.
// . Add a view that overrides OnThemeChanged(). In OnThemeChanged() another
// view is added. This sequence triggered DCHECKs or crashes previously. This
// tests verifies that doesn't happen. Reason for crash was OnThemeChanged() was
// called before the layer hierarchy was updated. OnThemeChanged() should be
// called after the layer hierarchy matches the view hierarchy.
TEST_F(ViewTest, CrashOnAddFromFromOnThemeChanged) {
  ui::TestNativeTheme theme;
  WidgetWithCustomTheme widget(&theme);
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.bounds = gfx::Rect(50, 50, 350, 350);
  widget.Init(params);

  AddViewWithChildLayer(widget.GetRootView());
  ViewThatAddsViewInOnThemeChanged* v = widget.GetRootView()->AddChildView(
      std::make_unique<ViewThatAddsViewInOnThemeChanged>());
  EXPECT_TRUE(v->on_native_theme_changed_called());
}

// A View that removes its Layer when hidden.
class NoLayerWhenHiddenView : public View {
 public:
  NoLayerWhenHiddenView() {
    SetPaintToLayer();
    set_owned_by_client();
    SetBounds(0, 0, 100, 100);
  }

  bool was_hidden() const { return was_hidden_; }

  // View:
  void VisibilityChanged(View* starting_from, bool is_visible) override {
    if (!is_visible) {
      was_hidden_ = true;
      DestroyLayer();
    }
  }

 private:
  bool was_hidden_ = false;

  DISALLOW_COPY_AND_ASSIGN(NoLayerWhenHiddenView);
};

// Test that Views can safely manipulate Layers during Widget closure.
TEST_F(ViewTest, DestroyLayerInClose) {
  NoLayerWhenHiddenView view;
  Widget* widget = new Widget;
  Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_WINDOW);
  widget->Init(params);
  widget->SetBounds(gfx::Rect(0, 0, 100, 100));
  widget->GetContentsView()->AddChildView(&view);
  widget->Show();

  EXPECT_TRUE(view.layer());
  EXPECT_TRUE(view.GetWidget());
  EXPECT_FALSE(view.was_hidden());

  widget->Close();
  EXPECT_FALSE(view.layer());
  // Ensure the layer went away via VisibilityChanged().
  EXPECT_TRUE(view.was_hidden());

  // Not removed from Widget until Close() completes.
  EXPECT_TRUE(view.GetWidget());
  base::RunLoop().RunUntilIdle();  // Let the Close() complete.
  EXPECT_FALSE(view.GetWidget());
}

// A View that keeps the children with a special ID above other children.
class OrderableView : public View {
 public:
  // ID used by the children that are stacked above other children.
  static constexpr int VIEW_ID_RAISED = 1000;

  OrderableView() = default;
  ~OrderableView() override = default;

  View::Views GetChildrenInZOrder() override {
    View::Views children_in_z_order = children();
    std::stable_partition(
        children_in_z_order.begin(), children_in_z_order.end(),
        [](const View* child) { return child->GetID() != VIEW_ID_RAISED; });
    return children_in_z_order;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(OrderableView);
};

TEST_F(ViewTest, ChildViewZOrderChanged) {
  const size_t kNumChildren = 4;
  auto view = std::make_unique<OrderableView>();
  view->SetPaintToLayer();
  for (size_t i = 0; i < kNumChildren; ++i)
    AddViewWithChildLayer(view.get());
  View::Views children = view->GetChildrenInZOrder();
  const std::vector<ui::Layer*>& layers = view->layer()->children();
  ASSERT_EQ(kNumChildren, children.size());
  ASSERT_EQ(kNumChildren, layers.size());
  for (size_t i = 0; i < kNumChildren; ++i) {
    EXPECT_EQ(view->children()[i], children[i]);
    EXPECT_EQ(view->children()[i]->layer(), layers[i]);
  }

  // Raise one of the children in z-order and add another child to reorder.
  view->children()[2]->SetID(OrderableView::VIEW_ID_RAISED);
  AddViewWithChildLayer(view.get());

  // 2nd child should be now on top, i.e. the last element in the array returned
  // by GetChildrenInZOrder(). Its layer should also be above the others.
  // The rest of the children and layers order should be unchanged.
  const size_t expected_order[] = {0, 1, 3, 4, 2};
  children = view->GetChildrenInZOrder();
  EXPECT_EQ(kNumChildren + 1, children.size());
  EXPECT_EQ(kNumChildren + 1, layers.size());
  for (size_t i = 0; i < kNumChildren + 1; ++i) {
    EXPECT_EQ(view->children()[expected_order[i]], children[i]);
    EXPECT_EQ(view->children()[expected_order[i]]->layer(), layers[i]);
  }
}

TEST_F(ViewTest, AttachChildViewWithComplicatedLayers) {
  std::unique_ptr<View> grand_parent_view(new View());
  grand_parent_view->SetPaintToLayer();

  View* parent_view = new OrderableView();
  parent_view->SetPaintToLayer();

  // child_view1 has layer and has id OrderableView::VIEW_ID_RAISED.
  View* child_view1 = parent_view->AddChildView(std::make_unique<View>());
  child_view1->SetPaintToLayer();
  child_view1->SetID(OrderableView::VIEW_ID_RAISED);

  // child_view2 has no layer.
  View* child_view2 = parent_view->AddChildView(std::make_unique<View>());
  // grand_child_view has layer.
  View* grand_child_view = child_view2->AddChildView(std::make_unique<View>());
  grand_child_view->SetPaintToLayer();
  const std::vector<ui::Layer*>& layers = parent_view->layer()->children();
  EXPECT_EQ(2u, layers.size());
  EXPECT_EQ(layers[0], grand_child_view->layer());
  EXPECT_EQ(layers[1], child_view1->layer());

  // Attach parent_view to grand_parent_view. children layers of parent_view
  // should not change.
  grand_parent_view->AddChildView(parent_view);
  const std::vector<ui::Layer*>& layers_after_attached
      = parent_view->layer()->children();
  EXPECT_EQ(2u, layers_after_attached.size());
  EXPECT_EQ(layers_after_attached[0], grand_child_view->layer());
  EXPECT_EQ(layers_after_attached[1], child_view1->layer());
}

TEST_F(ViewTest, TestEnabledPropertyMetadata) {
  View test_view;
  bool enabled_changed = false;
  auto subscription = test_view.AddEnabledChangedCallback(base::BindRepeating(
      [](bool* enabled_changed) { *enabled_changed = true; },
      &enabled_changed));
  views::metadata::ClassMetaData* view_metadata = View::MetaData();
  ASSERT_TRUE(view_metadata);
  views::metadata::MemberMetaDataBase* enabled_property =
      view_metadata->FindMemberData("Enabled");
  ASSERT_TRUE(enabled_property);
  base::string16 false_value = base::ASCIIToUTF16("false");
  enabled_property->SetValueAsString(&test_view, false_value);
  EXPECT_TRUE(enabled_changed);
  EXPECT_FALSE(test_view.GetEnabled());
  EXPECT_EQ(enabled_property->GetValueAsString(&test_view), false_value);
}

TEST_F(ViewTest, TestEnabledChangedCallback) {
  View test_view;
  bool enabled_changed = false;
  auto subscription = test_view.AddEnabledChangedCallback(base::BindRepeating(
      [](bool* enabled_changed) { *enabled_changed = true; },
      &enabled_changed));
  test_view.SetEnabled(false);
  EXPECT_TRUE(enabled_changed);
  EXPECT_FALSE(test_view.GetEnabled());
}

TEST_F(ViewTest, TestVisibleChangedCallback) {
  View test_view;
  bool visibility_changed = false;
  auto subscription = test_view.AddVisibleChangedCallback(base::BindRepeating(
      [](bool* visibility_changed) { *visibility_changed = true; },
      &visibility_changed));
  test_view.SetVisible(false);
  EXPECT_TRUE(visibility_changed);
  EXPECT_FALSE(test_view.GetVisible());
}

////////////////////////////////////////////////////////////////////////////////
// Observer tests.
////////////////////////////////////////////////////////////////////////////////

class ViewObserverTest : public ViewTest, public ViewObserver {
 public:
  ViewObserverTest() = default;

  ~ViewObserverTest() override = default;

  // ViewObserver:
  void OnChildViewAdded(View* parent, View* child) override {
    child_view_added_times_++;
    child_view_added_ = child;
    child_view_added_parent_ = parent;
  }
  void OnChildViewRemoved(View* parent, View* child) override {
    child_view_removed_times_++;
    child_view_removed_ = child;
    child_view_removed_parent_ = parent;
  }

  void OnViewVisibilityChanged(View* view, View* starting_view) override {
    view_visibility_changed_ = view;
    view_visibility_changed_starting_ = starting_view;
  }

  void OnViewBoundsChanged(View* view) override { view_bounds_changed_ = view; }

  void OnChildViewReordered(View* parent, View* view) override {
    view_reordered_ = view;
  }

  void reset() {
    child_view_added_times_ = 0;
    child_view_removed_times_ = 0;
    child_view_added_ = nullptr;
    child_view_added_parent_ = nullptr;
    child_view_removed_ = nullptr;
    child_view_removed_parent_ = nullptr;
    view_visibility_changed_ = nullptr;
    view_bounds_changed_ = nullptr;
    view_reordered_ = nullptr;
  }

  std::unique_ptr<View> NewView() {
    auto view = std::make_unique<View>();
    view->AddObserver(this);
    return view;
  }

  int child_view_added_times() { return child_view_added_times_; }
  int child_view_removed_times() { return child_view_removed_times_; }
  const View* child_view_added() const { return child_view_added_; }
  const View* child_view_added_parent() const {
    return child_view_added_parent_;
  }
  const View* child_view_removed() const { return child_view_removed_; }
  const View* child_view_removed_parent() const {
    return child_view_removed_parent_;
  }
  const View* view_visibility_changed() const {
    return view_visibility_changed_;
  }
  const View* view_visibility_changed_starting() const {
    return view_visibility_changed_starting_;
  }
  const View* view_bounds_changed() const { return view_bounds_changed_; }
  const View* view_reordered() const { return view_reordered_; }

 private:
  int child_view_added_times_ = 0;
  int child_view_removed_times_ = 0;

  View* child_view_added_parent_ = nullptr;
  View* child_view_added_ = nullptr;
  View* child_view_removed_ = nullptr;
  View* child_view_removed_parent_ = nullptr;
  View* view_visibility_changed_ = nullptr;
  View* view_visibility_changed_starting_ = nullptr;
  View* view_bounds_changed_ = nullptr;
  View* view_reordered_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(ViewObserverTest);
};

TEST_F(ViewObserverTest, ViewParentChanged) {
  std::unique_ptr<View> parent1 = NewView();
  std::unique_ptr<View> parent2 = NewView();
  std::unique_ptr<View> child_view = NewView();

  parent1->AddChildView(child_view.get());
  EXPECT_EQ(0, child_view_removed_times());
  EXPECT_EQ(1, child_view_added_times());
  EXPECT_EQ(child_view.get(), child_view_added());
  EXPECT_EQ(child_view->parent(), child_view_added_parent());
  EXPECT_EQ(child_view->parent(), parent1.get());
  reset();

  // Removed from parent1, added to parent2
  parent2->AddChildView(child_view.get());
  EXPECT_EQ(1, child_view_removed_times());
  EXPECT_EQ(1, child_view_added_times());
  EXPECT_EQ(child_view.get(), child_view_removed());
  EXPECT_EQ(parent1.get(), child_view_removed_parent());
  EXPECT_EQ(child_view.get(), child_view_added());
  EXPECT_EQ(child_view->parent(), parent2.get());

  reset();

  parent2->RemoveChildView(child_view.get());
  EXPECT_EQ(1, child_view_removed_times());
  EXPECT_EQ(0, child_view_added_times());
  EXPECT_EQ(child_view.get(), child_view_removed());
  EXPECT_EQ(parent2.get(), child_view_removed_parent());
}

TEST_F(ViewObserverTest, ViewVisibilityChanged) {
  std::unique_ptr<View> parent(new View);
  View* view = parent->AddChildView(NewView());

  // Ensure setting |view| itself not visible calls the observer.
  view->SetVisible(false);
  EXPECT_EQ(view, view_visibility_changed());
  EXPECT_EQ(view, view_visibility_changed_starting());
  reset();

  // Ditto for setting it visible.
  view->SetVisible(true);
  EXPECT_EQ(view, view_visibility_changed());
  EXPECT_EQ(view, view_visibility_changed_starting());
  reset();

  // Ensure setting |parent| not visible also calls the
  // observer. |view->visible()| should still return true however.
  parent->SetVisible(false);
  EXPECT_EQ(view, view_visibility_changed());
  EXPECT_EQ(parent.get(), view_visibility_changed_starting());
}

TEST_F(ViewObserverTest, ViewBoundsChanged) {
  std::unique_ptr<View> view = NewView();
  gfx::Rect bounds(2, 2, 2, 2);
  view->SetBoundsRect(bounds);
  EXPECT_EQ(view.get(), view_bounds_changed());
  EXPECT_EQ(bounds, view->bounds());

  reset();

  gfx::Rect new_bounds(1, 1, 1, 1);
  view->SetBoundsRect(new_bounds);
  EXPECT_EQ(view.get(), view_bounds_changed());
  EXPECT_EQ(new_bounds, view->bounds());
}

TEST_F(ViewObserverTest, ChildViewReordered) {
  std::unique_ptr<View> view = NewView();
  view->AddChildView(NewView());
  View* child_view2 = view->AddChildView(NewView());
  view->ReorderChildView(child_view2, 0);
  EXPECT_EQ(child_view2, view_reordered());
}

// Provides a simple parent view implementation which tracks layer change
// notifications from child views.
class TestParentView : public View {
 public:
  TestParentView() = default;

  void Reset() {
    received_layer_change_notification_ = false;
    layer_change_count_ = 0;
  }

  bool received_layer_change_notification() const {
    return received_layer_change_notification_;
  }

  int layer_change_count() const { return layer_change_count_; }

  // View overrides.
  void OnChildLayerChanged(View* child) override {
    received_layer_change_notification_ = true;
    layer_change_count_++;
  }

 private:
  // Set to true if we receive the OnChildLayerChanged() notification for a
  // child.
  bool received_layer_change_notification_ = false;

  // Contains the number of OnChildLayerChanged() notifications for a child.
  int layer_change_count_ = 0;

  DISALLOW_COPY_AND_ASSIGN(TestParentView);
};

// Tests the following cases.
// 1. We receive the OnChildLayerChanged() notification when a layer change
//    occurs in a child view.
// 2. We don't receive two layer changes when a child with an existing layer
//    creates a new layer.
TEST_F(ViewObserverTest, ChildViewLayerNotificationTest) {
  std::unique_ptr<TestParentView> parent_view(new TestParentView);
  std::unique_ptr<View> child_view = NewView();
  parent_view->AddChildView(child_view.get());

  EXPECT_FALSE(parent_view->received_layer_change_notification());
  EXPECT_EQ(0, parent_view->layer_change_count());

  child_view->SetPaintToLayer(ui::LAYER_TEXTURED);
  EXPECT_TRUE(parent_view->received_layer_change_notification());
  EXPECT_EQ(1, parent_view->layer_change_count());

  parent_view->Reset();
  child_view->SetPaintToLayer(ui::LAYER_SOLID_COLOR);
  EXPECT_TRUE(parent_view->received_layer_change_notification());
  EXPECT_EQ(1, parent_view->layer_change_count());
}

}  // namespace views
