// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/keyboard/keyboard_event_handler.h"

#include "ui/events/event.h"
#include "ui/gfx/geometry/vector2d.h"
#include "ui/keyboard/keyboard_controller.h"

namespace keyboard {

void KeyboardEventHandler::OnGestureEvent(ui::GestureEvent* event) {
  switch (event->type()) {
    case ui::ET_GESTURE_PINCH_BEGIN:
    case ui::ET_GESTURE_PINCH_END:
    case ui::ET_GESTURE_PINCH_UPDATE:
      event->StopPropagation();
      break;
    default:
      break;
  }
}

void KeyboardEventHandler::OnMouseEvent(ui::MouseEvent* event) {
  ProcessPointerEvent(event);
}

void KeyboardEventHandler::OnTouchEvent(ui::TouchEvent* event) {
  ProcessPointerEvent(event);
}

void KeyboardEventHandler::ProcessPointerEvent(ui::LocatedEvent* event) {
  auto* controller = KeyboardController::Get();
  if (controller->IsEnabled() && controller->HandlePointerEvent(*event))
    event->SetHandled();
}

}  // namespace keyboard
