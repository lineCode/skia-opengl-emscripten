// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_BUTTON_BUTTON_CONTROLLER_H_
#define UI_VIEWS_CONTROLS_BUTTON_BUTTON_CONTROLLER_H_

#include "base/macros.h"
#include "ui/events/event.h"
#include "ui/views/controls/button/button.h"

namespace views {

// Handles logic not related to the visual aspects of a Button such as event
// handling and state changes.
class VIEWS_EXPORT ButtonController {
 public:
  ButtonController(Button* button,
                   std::unique_ptr<ButtonControllerDelegate> delegate);
  virtual ~ButtonController();

  Button* button() { return button_; }

  // Methods that parallel View::On<Event> handlers:
  virtual bool OnMousePressed(const ui::MouseEvent& event);
  virtual void OnMouseReleased(const ui::MouseEvent& event);
  virtual void OnMouseMoved(const ui::MouseEvent& event);
  virtual void OnMouseEntered(const ui::MouseEvent& event);
  virtual void OnMouseExited(const ui::MouseEvent& event);
  virtual bool OnKeyPressed(const ui::KeyEvent& event);
  virtual bool OnKeyReleased(const ui::KeyEvent& event);
  virtual void OnGestureEvent(ui::GestureEvent* event);

#if !defined(UI_VIEWS_NO_AX)
  // Updates |node_data| for a button based on the functionality.
  virtual void UpdateAccessibleNodeData(ui::AXNodeData* node_data);
#endif // UI_VIEWS_NO_AX

  // Methods that parallel respective methods in Button:
  virtual void OnStateChanged(Button::ButtonState old_state);
  virtual bool IsTriggerableEvent(const ui::Event& event);

 protected:
  ButtonControllerDelegate* delegate() {
    return button_controller_delegate_.get();
  }

 private:
  Button* const button_;

  // TODO(cyan): Remove |button_| and access everything via the delegate.
  std::unique_ptr<ButtonControllerDelegate> button_controller_delegate_;

  DISALLOW_COPY_AND_ASSIGN(ButtonController);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_BUTTON_BUTTON_CONTROLLER_H_
