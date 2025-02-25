// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/resize_area.h"

#include "base/logging.h"
#if !defined(UI_VIEWS_NO_AX)
#include "ui/accessibility/ax_node_data.h"
#endif // UI_VIEWS_NO_AX
#include "ui/base/cursor/cursor.h"
#include "ui/views/controls/resize_area_delegate.h"
#include "ui/views/native_cursor.h"

namespace views {

const char ResizeArea::kViewClassName[] = "ResizeArea";

ResizeArea::ResizeArea(ResizeAreaDelegate* delegate)
    : delegate_(delegate),
      initial_position_(0) {
}

ResizeArea::~ResizeArea() = default;

const char* ResizeArea::GetClassName() const {
  return kViewClassName;
}

gfx::NativeCursor ResizeArea::GetCursor(const ui::MouseEvent& event) {
#if !defined(UI_VIEWS_PORT)
  return enabled() ? GetNativeEastWestResizeCursor()
                   : gfx::kNullCursor;
#else
  return gfx::NativeCursor();
#endif // UI_VIEWS_PORT
}

void ResizeArea::OnGestureEvent(ui::GestureEvent* event) {
  if (event->type() == ui::ET_GESTURE_TAP_DOWN) {
    SetInitialPosition(event->x());
    event->SetHandled();
  } else if (event->type() == ui::ET_GESTURE_SCROLL_BEGIN ||
             event->type() == ui::ET_GESTURE_SCROLL_UPDATE) {
    ReportResizeAmount(event->x(), false);
    event->SetHandled();
  } else if (event->type() == ui::ET_GESTURE_END) {
    ReportResizeAmount(event->x(), true);
    event->SetHandled();
  }
}

bool ResizeArea::OnMousePressed(const ui::MouseEvent& event) {
  if (!event.IsOnlyLeftMouseButton())
    return false;

  SetInitialPosition(event.x());
  return true;
}

bool ResizeArea::OnMouseDragged(const ui::MouseEvent& event) {
  if (!event.IsLeftMouseButton())
    return false;

  ReportResizeAmount(event.x(), false);
  return true;
}

void ResizeArea::OnMouseReleased(const ui::MouseEvent& event) {
  ReportResizeAmount(event.x(), true);
}

void ResizeArea::OnMouseCaptureLost() {
  ReportResizeAmount(initial_position_, true);
}

#if !defined(UI_VIEWS_NO_AX)
void ResizeArea::GetAccessibleNodeData(ui::AXNodeData* node_data) {
  node_data->role = ax::mojom::Role::kSplitter;
}
#endif // UI_VIEWS_NO_AX

void ResizeArea::ReportResizeAmount(int resize_amount, bool last_update) {
  gfx::Point point(resize_amount, 0);
  View::ConvertPointToScreen(this, &point);
  resize_amount = point.x() - initial_position_;
  delegate_->OnResize(base::i18n::IsRTL() ? -resize_amount : resize_amount,
                      last_update);
}

void ResizeArea::SetInitialPosition(int event_x) {
  gfx::Point point(event_x, 0);
  View::ConvertPointToScreen(this, &point);
  initial_position_ = point.x();
}

}  // namespace views
