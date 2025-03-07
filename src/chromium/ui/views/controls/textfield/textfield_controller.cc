// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/textfield/textfield_controller.h"

#if !defined(UI_VIEWS_PORT)
#include "ui/base/dragdrop/drag_drop_types.h"
#endif // UI_VIEWS_PORT
#include "ui/events/event.h"

namespace views {

bool TextfieldController::HandleKeyEvent(Textfield* sender,
                                         const ui::KeyEvent& key_event) {
  return false;
}

bool TextfieldController::HandleMouseEvent(Textfield* sender,
                                           const ui::MouseEvent& mouse_event) {
  return false;
}

bool TextfieldController::HandleGestureEvent(
    Textfield* sender,
    const ui::GestureEvent& gesture_event) {
  return false;
}

#if !defined(UI_VIEWS_PORT)
int TextfieldController::OnDrop(const ui::OSExchangeData& data) {
  return ui::DragDropTypes::DRAG_NONE;
}
#endif // UI_VIEWS_PORT

}  // namespace views
