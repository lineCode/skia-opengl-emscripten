// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/button/menu_button_controller.h"

#if !defined(UI_VIEWS_NO_AX)
#include "ui/accessibility/ax_node_data.h"
#endif // UI_VIEWS_NO_AX
#if !defined(UI_VIEWS_PORT)
#include "ui/base/dragdrop/drag_drop_types.h"
#endif // UI_VIEWS_PORT
#include "ui/events/event_constants.h"
#include "ui/views/animation/ink_drop.h"
#include "ui/views/controls/button/button_controller_delegate.h"
#include "ui/views/controls/button/menu_button.h"
#include "ui/views/controls/button/menu_button_listener.h"
#include "ui/views/mouse_constants.h"
#include "ui/views/widget/root_view.h"
#include "ui/views/widget/widget.h"

using base::TimeDelta;
using base::TimeTicks;

namespace views {

////////////////////////////////////////////////////////////////////////////////
//
// MenuButtonController::PressedLock
//
////////////////////////////////////////////////////////////////////////////////

MenuButtonController::PressedLock::PressedLock(
    MenuButtonController* menu_button_controller)
    : PressedLock(menu_button_controller, false, nullptr) {}

MenuButtonController::PressedLock::PressedLock(
    MenuButtonController* menu_button_controller,
    bool is_sibling_menu_show,
    const ui::LocatedEvent* event)
    : menu_button_controller_(
          menu_button_controller->weak_factory_.GetWeakPtr()) {
  menu_button_controller_->IncrementPressedLocked(is_sibling_menu_show, event);
}

std::unique_ptr<MenuButtonController::PressedLock>
MenuButtonController::TakeLock() {
  return TakeLock(false, nullptr);
}

std::unique_ptr<MenuButtonController::PressedLock>
MenuButtonController::TakeLock(bool is_sibling_menu_show,
                               const ui::LocatedEvent* event) {
  return std::make_unique<MenuButtonController::PressedLock>(
      this, is_sibling_menu_show, event);
}

MenuButtonController::PressedLock::~PressedLock() {
  if (menu_button_controller_)
    menu_button_controller_->DecrementPressedLocked();
}

////////////////////////////////////////////////////////////////////////////////
//
// MenuButtonController
//
////////////////////////////////////////////////////////////////////////////////

MenuButtonController::MenuButtonController(
    Button* button,
    MenuButtonListener* listener,
    std::unique_ptr<ButtonControllerDelegate> delegate)
    : ButtonController(button, std::move(delegate)), listener_(listener) {}

MenuButtonController::~MenuButtonController() = default;

bool MenuButtonController::OnMousePressed(const ui::MouseEvent& event) {
  if (button()->request_focus_on_press())
    button()->RequestFocus();
  if (button()->state() != Button::STATE_DISABLED &&
      button()->HitTestPoint(event.location()) && IsTriggerableEvent(event)) {
    return Activate(&event);
  }
  return true;
}

void MenuButtonController::OnMouseReleased(const ui::MouseEvent& event) {
  if (button()->state() != Button::STATE_DISABLED &&
      delegate()->IsTriggerableEvent(event) &&
      button()->HitTestPoint(event.location()) && !delegate()->InDrag()) {
    Activate(&event);
  } else {
    button()->AnimateInkDrop(InkDropState::HIDDEN, &event);
    ButtonController::OnMouseReleased(event);
  }
}

void MenuButtonController::OnMouseMoved(const ui::MouseEvent& event) {
  if (pressed_lock_count_ == 0)  // Ignore mouse movement if state is locked.
    ButtonController::OnMouseMoved(event);
}

void MenuButtonController::OnMouseEntered(const ui::MouseEvent& event) {
  if (pressed_lock_count_ == 0)  // Ignore mouse movement if state is locked.
    ButtonController::OnMouseEntered(event);
}

void MenuButtonController::OnMouseExited(const ui::MouseEvent& event) {
  if (pressed_lock_count_ == 0)  // Ignore mouse movement if state is locked.
    ButtonController::OnMouseExited(event);
}

bool MenuButtonController::OnKeyPressed(const ui::KeyEvent& event) {
  switch (event.key_code()) {
    case ui::VKEY_SPACE:
      // Alt-space on windows should show the window menu.
      if (event.IsAltDown())
        break;
      FALLTHROUGH;
    case ui::VKEY_RETURN:
    case ui::VKEY_UP:
    case ui::VKEY_DOWN: {
      // WARNING: we may have been deleted by the time Activate returns.
      Activate(&event);
      // This is to prevent the keyboard event from being dispatched twice.  If
      // the keyboard event is not handled, we pass it to the default handler
      // which dispatches the event back to us causing the menu to get displayed
      // again. Return true to prevent this.
      return true;
    }
    default:
      break;
  }
  return false;
}

bool MenuButtonController::OnKeyReleased(const ui::KeyEvent& event) {
  // A MenuButton always activates the menu on key press.
  return false;
}

#if !defined(UI_VIEWS_NO_AX)
void MenuButtonController::UpdateAccessibleNodeData(ui::AXNodeData* node_data) {
  node_data->role = ax::mojom::Role::kPopUpButton;
  node_data->SetHasPopup(ax::mojom::HasPopup::kMenu);
  if (button()->enabled())
    node_data->SetDefaultActionVerb(ax::mojom::DefaultActionVerb::kOpen);
}
#endif // UI_VIEWS_NO_AX

void MenuButtonController::OnStateChanged(LabelButton::ButtonState old_state) {
  // State change occurs in IncrementPressedLocked() and
  // DecrementPressedLocked().
  if (pressed_lock_count_ != 0) {
    // The button's state was changed while it was supposed to be locked in a
    // pressed state. This shouldn't happen, but conceivably could if a caller
    // tries to switch from enabled to disabled or vice versa while the button
    // is pressed.
    if (button()->state() == Button::STATE_NORMAL)
      should_disable_after_press_ = false;
    else if (button()->state() == Button::STATE_DISABLED)
      should_disable_after_press_ = true;
  }
}

bool MenuButtonController::IsTriggerableEvent(const ui::Event& event) {
  return ButtonController::IsTriggerableEvent(event) &&
         IsTriggerableEventType(event) && IsIntentionalMenuTrigger();
}

void MenuButtonController::OnGestureEvent(ui::GestureEvent* event) {
  if (button()->state() != Button::STATE_DISABLED) {
    auto ref = weak_factory_.GetWeakPtr();
    if (delegate()->IsTriggerableEvent(*event) && !Activate(event)) {
      // When |Activate()| returns |false|, it means the click was handled by
      // a button listener and has handled the gesture event. So, there is no
      // need to further process the gesture event here. However, if the
      // listener didn't run menu code, we should make sure to reset our state.
      if (ref && button()->state() == Button::STATE_HOVERED)
        button()->SetState(Button::STATE_NORMAL);

      return;
    }
    if (event->type() == ui::ET_GESTURE_TAP_DOWN) {
      event->SetHandled();
      if (pressed_lock_count_ == 0)
        button()->SetState(Button::STATE_HOVERED);
    } else if (button()->state() == Button::STATE_HOVERED &&
               (event->type() == ui::ET_GESTURE_TAP_CANCEL ||
                event->type() == ui::ET_GESTURE_END) &&
               pressed_lock_count_ == 0) {
      button()->SetState(Button::STATE_NORMAL);
    }
  }
  ButtonController::OnGestureEvent(event);
}

bool MenuButtonController::Activate(const ui::Event* event) {
  if (listener_) {
    gfx::Rect lb = button()->GetLocalBounds();

    // Offset of the associated menu position.
    constexpr gfx::Vector2d kMenuOffset{-2, -4};

    // The position of the menu depends on whether or not the locale is
    // right-to-left.
    gfx::Point menu_position(lb.right(), lb.bottom());
    if (base::i18n::IsRTL())
      menu_position.set_x(lb.x());

    View::ConvertPointToScreen(button(), &menu_position);
    if (base::i18n::IsRTL())
      menu_position.Offset(-kMenuOffset.x(), kMenuOffset.y());
    else
      menu_position += kMenuOffset;

    int max_x_coordinate = GetMaximumScreenXCoordinate();
    if (max_x_coordinate && max_x_coordinate <= menu_position.x())
      menu_position.set_x(max_x_coordinate - 1);

    // We're about to show the menu from a mouse press. By showing from the
    // mouse press event we block RootView in mouse dispatching. This also
    // appears to cause RootView to get a mouse pressed BEFORE the mouse
    // release is seen, which means RootView sends us another mouse press no
    // matter where the user pressed. To force RootView to recalculate the
    // mouse target during the mouse press we explicitly set the mouse handler
    // to NULL.
    static_cast<internal::RootView*>(button()->GetWidget()->GetRootView())
        ->SetMouseHandler(nullptr);

    DCHECK(increment_pressed_lock_called_ == nullptr);
    // Observe if IncrementPressedLocked() was called so we can trigger the
    // correct ink drop animations.
    bool increment_pressed_lock_called = false;
    increment_pressed_lock_called_ = &increment_pressed_lock_called;

    // Allow for OnMenuButtonClicked() to delete this.
    auto ref = weak_factory_.GetWeakPtr();

    // We don't set our state here. It's handled in the MenuController code or
    // by our click listener.
    listener_->OnMenuButtonClicked(button(), menu_position, event);

    if (!ref) {
      // The menu was deleted while showing. Don't attempt any processing.
      return false;
    }

    increment_pressed_lock_called_ = nullptr;

    if (!increment_pressed_lock_called && pressed_lock_count_ == 0) {
      button()->AnimateInkDrop(InkDropState::ACTION_TRIGGERED,
                               ui::LocatedEvent::FromIfValid(event));
    }

    // We must return false here so that the RootView does not get stuck
    // sending all mouse pressed events to us instead of the appropriate
    // target.
    return false;
  }

  button()->AnimateInkDrop(InkDropState::HIDDEN,
                           ui::LocatedEvent::FromIfValid(event));
  return true;
}

bool MenuButtonController::IsTriggerableEventType(const ui::Event& event) {
  if (event.IsMouseEvent()) {
    const auto* mouse_event = event.AsMouseEvent();
    // Check that the event has the correct flags the button specified can
    // trigger button actions. For example, menus should only active on left
    // mouse button, to prevent a menu from being activated when a right-click
    // would also activate a context menu.
    if (!(mouse_event->button_flags() & button()->triggerable_event_flags()))
      return false;
    // If dragging is supported activate on release, otherwise activate on
    // pressed.
#if !defined(UI_VIEWS_PORT)
    ui::EventType active_on =
        delegate()->GetDragOperations(mouse_event->location()) ==
                ui::DragDropTypes::DRAG_NONE
            ? ui::ET_MOUSE_PRESSED
            : ui::ET_MOUSE_RELEASED;
    return event.type() == active_on;
#endif // UI_VIEWS_PORT
  }
  return event.type() == ui::ET_GESTURE_TAP;
}

bool MenuButtonController::IsIntentionalMenuTrigger() const {
  return (TimeTicks::Now() - menu_closed_time_).InMilliseconds() >=
         kMinimumMsBetweenButtonClicks;
}

void MenuButtonController::IncrementPressedLocked(
    bool snap_ink_drop_to_activated,
    const ui::LocatedEvent* event) {
  ++pressed_lock_count_;
  if (increment_pressed_lock_called_)
    *(increment_pressed_lock_called_) = true;
  should_disable_after_press_ = button()->state() == Button::STATE_DISABLED;
  if (button()->state() != Button::STATE_PRESSED) {
    if (snap_ink_drop_to_activated)
      delegate()->GetInkDrop()->SnapToActivated();
    else
      button()->AnimateInkDrop(InkDropState::ACTIVATED, event);
  }
  button()->SetState(Button::STATE_PRESSED);
  delegate()->GetInkDrop()->SetHovered(false);
}

void MenuButtonController::DecrementPressedLocked() {
  --pressed_lock_count_;
  DCHECK_GE(pressed_lock_count_, 0);

  // If this was the last lock, manually reset state to the desired state.
  if (pressed_lock_count_ == 0) {
    menu_closed_time_ = TimeTicks::Now();
    LabelButton::ButtonState desired_state = Button::STATE_NORMAL;
    if (should_disable_after_press_) {
      desired_state = Button::STATE_DISABLED;
      should_disable_after_press_ = false;
    } else if (button()->GetWidget() &&
               !button()->GetWidget()->dragged_view() &&
               delegate()->ShouldEnterHoveredState()) {
      desired_state = Button::STATE_HOVERED;
      delegate()->GetInkDrop()->SetHovered(true);
    }
    button()->SetState(desired_state);
    // The widget may be null during shutdown. If so, it doesn't make sense to
    // try to add an ink drop effect.
    if (button()->GetWidget() && button()->state() != Button::STATE_PRESSED)
      button()->AnimateInkDrop(InkDropState::DEACTIVATED, nullptr /* event */);
  }
}

int MenuButtonController::GetMaximumScreenXCoordinate() {
  if (!button()->GetWidget()) {
    NOTREACHED();
    return 0;
  }

  gfx::Rect monitor_bounds = button()->GetWidget()->GetWorkAreaBoundsInScreen();
  return monitor_bounds.right() - 1;
}

}  // namespace views
