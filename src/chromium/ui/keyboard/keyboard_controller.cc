// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/keyboard/keyboard_controller.h"

#include <set>

#include "base/bind.h"
#include "base/command_line.h"
#include "base/macros.h"
#include "base/metrics/histogram_functions.h"
#include "base/metrics/histogram_macros.h"
#include "base/stl_util.h"
#include "base/threading/thread_task_runner_handle.h"
#include "base/time/time.h"
#include "ui/aura/client/aura_constants.h"
#include "ui/aura/env.h"
#include "ui/aura/window.h"
#include "ui/aura/window_delegate.h"
#include "ui/aura/window_observer.h"
#include "ui/base/cursor/cursor.h"
#include "ui/base/hit_test.h"
#include "ui/base/ime/input_method.h"
#include "ui/base/ime/text_input_client.h"
#include "ui/base/ime/text_input_flags.h"
#include "ui/compositor/layer_animation_observer.h"
#include "ui/compositor/scoped_layer_animation_settings.h"
#include "ui/display/types/display_constants.h"
#include "ui/events/base_event_utils.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/vector2d.h"
#include "ui/keyboard/container_floating_behavior.h"
#include "ui/keyboard/container_full_width_behavior.h"
#include "ui/keyboard/container_fullscreen_behavior.h"
#include "ui/keyboard/display_util.h"
#include "ui/keyboard/keyboard_controller_observer.h"
#include "ui/keyboard/keyboard_layout_manager.h"
#include "ui/keyboard/keyboard_ui.h"
#include "ui/keyboard/keyboard_ui_factory.h"
#include "ui/keyboard/keyboard_util.h"
#include "ui/keyboard/notification_manager.h"
#include "ui/keyboard/public/keyboard_switches.h"
#include "ui/keyboard/queued_container_type.h"
#include "ui/keyboard/queued_display_change.h"
#include "ui/keyboard/shaped_window_targeter.h"
#include "ui/ozone/public/input_controller.h"
#include "ui/ozone/public/ozone_platform.h"
#include "ui/wm/core/coordinate_conversion.h"
#include "ui/wm/core/window_animations.h"

namespace keyboard {

namespace {

// Owned by ash::Shell.
KeyboardController* g_keyboard_controller = nullptr;

// How long the keyboard stays in WILL_HIDE state before moving to HIDDEN.
constexpr base::TimeDelta kHideKeyboardDelay =
    base::TimeDelta::FromMilliseconds(100);

// Reports an error histogram if the keyboard state is lingering in an
// intermediate state for more than 5 seconds.
constexpr base::TimeDelta kReportLingeringStateDelay =
    base::TimeDelta::FromMilliseconds(5000);

// Delay threshold after the keyboard enters the WILL_HIDE state. If text focus
// is regained during this threshold, the keyboard will show again, even if it
// is an asynchronous event. This is for the benefit of things like login flow
// where the password field may get text focus after an animation that plays
// after the user enters their username.
constexpr base::TimeDelta kTransientBlurThreshold =
    base::TimeDelta::FromMilliseconds(3500);

// State transition diagram (document linked from crbug.com/719905)
bool IsAllowedStateTransition(KeyboardControllerState from,
                              KeyboardControllerState to) {
  static const std::set<
      std::pair<KeyboardControllerState, KeyboardControllerState>>
      kAllowedStateTransition = {
          // The initial ShowKeyboard scenario
          // INITIAL -> LOADING_EXTENSION -> HIDDEN -> SHOWN.
          {KeyboardControllerState::UNKNOWN, KeyboardControllerState::INITIAL},
          {KeyboardControllerState::INITIAL,
           KeyboardControllerState::LOADING_EXTENSION},
          {KeyboardControllerState::LOADING_EXTENSION,
           KeyboardControllerState::HIDDEN},
          {KeyboardControllerState::HIDDEN, KeyboardControllerState::SHOWN},

          // Hide scenario
          // SHOWN -> WILL_HIDE -> HIDDEN.
          {KeyboardControllerState::SHOWN, KeyboardControllerState::WILL_HIDE},
          {KeyboardControllerState::WILL_HIDE, KeyboardControllerState::HIDDEN},

          // Focus transition scenario
          // SHOWN -> WILL_HIDE -> SHOWN.
          {KeyboardControllerState::WILL_HIDE, KeyboardControllerState::SHOWN},

          // HideKeyboard can be called at anytime for example on shutdown.
          {KeyboardControllerState::SHOWN, KeyboardControllerState::HIDDEN},

          // Return to INITIAL when keyboard is disabled.
          {KeyboardControllerState::LOADING_EXTENSION,
           KeyboardControllerState::INITIAL},
          {KeyboardControllerState::HIDDEN, KeyboardControllerState::INITIAL},
      };
  return kAllowedStateTransition.count(std::make_pair(from, to)) == 1;
}

void SetTouchEventLogging(bool enable) {
  // TODO(moshayedi): crbug.com/642863. Revisit when we have mojo interface for
  // InputController for processes that aren't mus-ws.
  if (aura::Env::GetInstance()->mode() == aura::Env::Mode::MUS)
    return;
  ui::InputController* controller =
      ui::OzonePlatform::GetInstance()->GetInputController();
  if (controller)
    controller->SetTouchEventLoggingEnabled(enable);
}

std::string StateToStr(KeyboardControllerState state) {
  switch (state) {
    case KeyboardControllerState::UNKNOWN:
      return "UNKNOWN";
    case KeyboardControllerState::SHOWN:
      return "SHOWN";
    case KeyboardControllerState::LOADING_EXTENSION:
      return "LOADING_EXTENSION";
    case KeyboardControllerState::WILL_HIDE:
      return "WILL_HIDE";
    case KeyboardControllerState::HIDDEN:
      return "HIDDEN";
    case KeyboardControllerState::INITIAL:
      return "INITIAL";
    case KeyboardControllerState::COUNT:
      NOTREACHED();
  }
  NOTREACHED() << "Unknownstate: " << static_cast<int>(state);
  // Needed for windows build.
  return "";
}

// An enumeration of different keyboard control events that should be logged.
enum KeyboardControlEvent {
  KEYBOARD_CONTROL_SHOW = 0,
  KEYBOARD_CONTROL_HIDE_AUTO,
  KEYBOARD_CONTROL_HIDE_USER,
  KEYBOARD_CONTROL_MAX,
};

void LogKeyboardControlEvent(KeyboardControlEvent event) {
  UMA_HISTOGRAM_ENUMERATION("VirtualKeyboard.KeyboardControlEvent", event,
                            KEYBOARD_CONTROL_MAX);
}

class InputMethodKeyboardController : public ui::InputMethodKeyboardController {
 public:
  explicit InputMethodKeyboardController(
      KeyboardController* keyboard_controller)
      : keyboard_controller_(keyboard_controller) {}

  ~InputMethodKeyboardController() override = default;

  // ui::InputMethodKeyboardController
  bool DisplayVirtualKeyboard() override {
    // Calling |ShowKeyboardInternal| may move the keyboard to another display.
    if (keyboard_controller_->IsEnabled() &&
        !keyboard_controller_->keyboard_locked()) {
      keyboard_controller_->ShowKeyboard(false /* locked */);
      return true;
    }
    return false;
  }

  void DismissVirtualKeyboard() override {
    keyboard_controller_->HideKeyboardByUser();
  }

  void AddObserver(
      ui::InputMethodKeyboardControllerObserver* observer) override {
    // TODO: Implement.
  }

  void RemoveObserver(
      ui::InputMethodKeyboardControllerObserver* observer) override {
    // TODO: Implement.
  }

  bool IsKeyboardVisible() override {
    return keyboard_controller_->IsKeyboardVisible();
  }

 private:
  KeyboardController* keyboard_controller_;
};

}  // namespace

// Observer for both keyboard show and hide animations. It should be owned by
// KeyboardController.
class CallbackAnimationObserver : public ui::ImplicitAnimationObserver {
 public:
  CallbackAnimationObserver(base::OnceClosure callback)
      : callback_(std::move(callback)) {}

 private:
  // ui::ImplicitAnimationObserver:
  void OnImplicitAnimationsCompleted() override {
    if (WasAnimationAbortedForProperty(ui::LayerAnimationElement::TRANSFORM) ||
        WasAnimationAbortedForProperty(ui::LayerAnimationElement::OPACITY)) {
      return;
    }
    DCHECK(
        WasAnimationCompletedForProperty(ui::LayerAnimationElement::TRANSFORM));
    DCHECK(
        WasAnimationCompletedForProperty(ui::LayerAnimationElement::OPACITY));
    std::move(callback_).Run();
  }

  base::OnceClosure callback_;

  DISALLOW_COPY_AND_ASSIGN(CallbackAnimationObserver);
};

KeyboardController::KeyboardController()
    : input_method_keyboard_controller_(
          std::make_unique<InputMethodKeyboardController>(this)),
      ime_observer_(this),
      weak_factory_report_lingering_state_(this),
      weak_factory_will_hide_(this) {
  DCHECK_EQ(g_keyboard_controller, nullptr);
  g_keyboard_controller = this;
}

KeyboardController::~KeyboardController() {
  DCHECK(g_keyboard_controller);
  DCHECK(!ui_)
      << "Keyboard UI must be destroyed before KeyboardController is destroyed";
  g_keyboard_controller = nullptr;
}

// static
KeyboardController* KeyboardController::Get() {
  DCHECK(g_keyboard_controller);
  return g_keyboard_controller;
}

// static
bool KeyboardController::HasInstance() {
  return g_keyboard_controller;
}

void KeyboardController::Initialize(
    std::unique_ptr<KeyboardUIFactory> ui_factory,
    KeyboardLayoutDelegate* layout_delegate) {
  DCHECK(ui_factory);
  DCHECK(layout_delegate);

  ui_factory_ = std::move(ui_factory);
  layout_delegate_ = layout_delegate;

  DCHECK(!IsKeyboardEnableRequested());
}

void KeyboardController::Shutdown() {
  keyboard_enable_flags_.clear();
  for (KeyboardControllerObserver& observer : observer_list_)
    observer.OnKeyboardEnableFlagsChanged(keyboard_enable_flags_);

  DCHECK(!IsKeyboardEnableRequested());
  DisableKeyboard();
}

void KeyboardController::EnableKeyboard() {
  if (ui_)
    return;

  ui_ = ui_factory_->CreateKeyboardUI();
  DCHECK(ui_);

  show_on_keyboard_window_load_ = false;
  keyboard_locked_ = false;
  state_ = KeyboardControllerState::UNKNOWN;
  ui_->SetController(this);
  SetContainerBehaviorInternal(mojom::ContainerType::kFullWidth);
  ChangeState(KeyboardControllerState::INITIAL);
  visual_bounds_in_root_ = gfx::Rect();
  time_of_last_blur_ = base::Time::UnixEpoch();
  UpdateInputMethodObserver();

  ActivateKeyboardInContainer(
      layout_delegate_->GetContainerForDefaultDisplay());

  // Start preloading the virtual keyboard UI in the background, so that it
  // shows up faster when needed.
  LoadKeyboardWindowInBackground();

  // Notify observers after the keyboard window has a root window.
  for (KeyboardControllerObserver& observer : observer_list_)
    observer.OnKeyboardEnabledChanged(true);
}

void KeyboardController::DisableKeyboard() {
  if (!ui_)
    return;

  if (parent_container_)
    DeactivateKeyboard();

  aura::Window* keyboard_window = GetKeyboardWindow();
  if (keyboard_window)
    keyboard_window->RemoveObserver(this);

  // Return to the INITIAL state to ensure that transitions entering a state
  // is equal to transitions leaving the state.
  if (state_ != KeyboardControllerState::INITIAL)
    ChangeState(KeyboardControllerState::INITIAL);

  // TODO(https://crbug.com/731537): Move KeyboardController members into a
  // subobject so we can just put this code into the subobject destructor.
  queued_display_change_.reset();
  queued_container_type_.reset();
  container_behavior_.reset();
  animation_observer_.reset();

  ime_observer_.RemoveAll();
  ui_->SetController(nullptr);
  ui_.reset();

  // Notify observers after |ui_| is reset so that IsEnabled() is false.
  for (KeyboardControllerObserver& observer : observer_list_)
    observer.OnKeyboardEnabledChanged(false);
}

void KeyboardController::ActivateKeyboardInContainer(aura::Window* parent) {
  DCHECK(parent);
  DCHECK(!parent_container_);
  parent_container_ = parent;
  // Observe changes to root window bounds.
  parent_container_->GetRootWindow()->AddObserver(this);

  UpdateInputMethodObserver();

  if (GetKeyboardWindow()) {
    DCHECK(!GetKeyboardWindow()->parent());
    parent_container_->AddChild(GetKeyboardWindow());
  }
}

void KeyboardController::DeactivateKeyboard() {
  DCHECK(parent_container_);

  // Ensure the keyboard is not visible before deactivating it.
  HideKeyboardExplicitlyBySystem();

  aura::Window* keyboard_window = GetKeyboardWindow();
  if (keyboard_window) {
    keyboard_window->RemovePreTargetHandler(&event_handler_);
    if (keyboard_window->parent()) {
      DCHECK_EQ(parent_container_, keyboard_window->parent());
      parent_container_->RemoveChild(keyboard_window);
    }
  }
  parent_container_->GetRootWindow()->RemoveObserver(this);
  parent_container_ = nullptr;
}

aura::Window* KeyboardController::GetKeyboardWindow() const {
  return ui_ ? ui_->GetKeyboardWindow() : nullptr;
}

aura::Window* KeyboardController::GetRootWindow() const {
  return parent_container_ ? parent_container_->GetRootWindow() : nullptr;
}

void KeyboardController::MoveToParentContainer(aura::Window* parent) {
  DCHECK(parent);
  if (parent_container_ == parent)
    return;

  TRACE_EVENT0("vk", "MoveKeyboardToDisplayInternal");

  DeactivateKeyboard();
  ActivateKeyboardInContainer(parent);
}

// private
void KeyboardController::NotifyKeyboardBoundsChanging(
    const gfx::Rect& new_bounds_in_root) {
  visual_bounds_in_root_ = new_bounds_in_root;
  gfx::Rect occluded_bounds_in_screen = GetWorkspaceOccludedBoundsInScreen();

  aura::Window* window = GetKeyboardWindow();
  if (window && window->IsVisible()) {
    // TODO(https://crbug.com/943446): Use screen bounds for visual bounds.
    notification_manager_.SendNotifications(
        container_behavior_->OccludedBoundsAffectWorkspaceLayout(),
        new_bounds_in_root, occluded_bounds_in_screen, observer_list_);
  } else {
    visual_bounds_in_root_ = gfx::Rect();
  }

  EnsureCaretInWorkArea(occluded_bounds_in_screen);
}

void KeyboardController::SetKeyboardWindowBounds(
    const gfx::Rect& new_bounds_in_root) {
  ui::LayerAnimator* animator = GetKeyboardWindow()->layer()->GetAnimator();
  // Stops previous animation if a window resize is requested during animation.
  if (animator->is_animating())
    animator->StopAnimating();

  GetKeyboardWindow()->SetBounds(new_bounds_in_root);
}

void KeyboardController::NotifyKeyboardWindowLoaded() {
  const bool should_show = show_on_keyboard_window_load_;
  if (state_ == KeyboardControllerState::LOADING_EXTENSION)
    ChangeState(KeyboardControllerState::HIDDEN);
  if (should_show) {
    // The window height is set to 0 initially or before switch to an IME in a
    // different extension. Virtual keyboard window may wait for this bounds
    // change to correctly animate in.
    if (keyboard_locked_) {
      // Do not move the keyboard to another display after switch to an IME in
      // a different extension.
      ShowKeyboardInDisplay(
          display_util_.GetNearestDisplayToWindow(GetKeyboardWindow()));
    } else {
      ShowKeyboard(false /* lock */);
    }
  }
}

void KeyboardController::Reload() {
  if (!GetKeyboardWindow())
    return;

  ui_->ReloadKeyboardIfNeeded();
}

void KeyboardController::RebuildKeyboardIfEnabled() {
  if (!IsEnabled())
    return;

  DisableKeyboard();
  EnableKeyboard();
}

void KeyboardController::AddObserver(KeyboardControllerObserver* observer) {
  observer_list_.AddObserver(observer);
}

bool KeyboardController::HasObserver(
    KeyboardControllerObserver* observer) const {
  return observer_list_.HasObserver(observer);
}

void KeyboardController::RemoveObserver(KeyboardControllerObserver* observer) {
  observer_list_.RemoveObserver(observer);
}

bool KeyboardController::UpdateKeyboardConfig(
    const mojom::KeyboardConfig& config) {
  if (config.Equals(keyboard_config_))
    return false;
  keyboard_config_ = config;
  if (IsEnabled())
    NotifyKeyboardConfigChanged();
  return true;
}

void KeyboardController::SetEnableFlag(mojom::KeyboardEnableFlag flag) {
  if (!base::ContainsKey(keyboard_enable_flags_, flag))
    keyboard_enable_flags_.insert(flag);

  // If there is a flag that is mutually exclusive with |flag|, clear it.
  using mojom::KeyboardEnableFlag;
  switch (flag) {
    case KeyboardEnableFlag::kPolicyEnabled:
      keyboard_enable_flags_.erase(KeyboardEnableFlag::kPolicyDisabled);
      break;
    case KeyboardEnableFlag::kPolicyDisabled:
      keyboard_enable_flags_.erase(KeyboardEnableFlag::kPolicyEnabled);
      break;
    case KeyboardEnableFlag::kExtensionEnabled:
      keyboard_enable_flags_.erase(KeyboardEnableFlag::kExtensionDisabled);
      break;
    case KeyboardEnableFlag::kExtensionDisabled:
      keyboard_enable_flags_.erase(KeyboardEnableFlag::kExtensionEnabled);
      break;
    default:
      break;
  }
  for (KeyboardControllerObserver& observer : observer_list_)
    observer.OnKeyboardEnableFlagsChanged(keyboard_enable_flags_);

  UpdateKeyboardAsRequestedBy(flag);
}

void KeyboardController::ClearEnableFlag(mojom::KeyboardEnableFlag flag) {
  if (!IsEnableFlagSet(flag))
    return;

  keyboard_enable_flags_.erase(flag);
  for (KeyboardControllerObserver& observer : observer_list_)
    observer.OnKeyboardEnableFlagsChanged(keyboard_enable_flags_);

  UpdateKeyboardAsRequestedBy(flag);
}

bool KeyboardController::IsEnableFlagSet(mojom::KeyboardEnableFlag flag) const {
  return base::ContainsKey(keyboard_enable_flags_, flag);
}

bool KeyboardController::IsKeyboardEnableRequested() const {
  using mojom::KeyboardEnableFlag;
  // Accessibility setting prioritized over policy/arc overrides.
  if (IsEnableFlagSet(KeyboardEnableFlag::kAccessibilityEnabled))
    return true;

  // Keyboard can be enabled temporarily by the shelf.
  if (IsEnableFlagSet(KeyboardEnableFlag::kShelfEnabled))
    return true;

  if (IsEnableFlagSet(KeyboardEnableFlag::kAndroidDisabled) ||
      IsEnableFlagSet(KeyboardEnableFlag::kPolicyDisabled)) {
    return false;
  }
  if (IsEnableFlagSet(KeyboardEnableFlag::kPolicyEnabled))
    return true;

  // Command line overrides extension and touch enabled flags.
  if (IsEnableFlagSet(KeyboardEnableFlag::kCommandLineEnabled))
    return true;

  if (IsEnableFlagSet(KeyboardEnableFlag::kExtensionDisabled))
    return false;

  return IsEnableFlagSet(KeyboardEnableFlag::kExtensionEnabled) ||
         IsEnableFlagSet(KeyboardEnableFlag::kTouchEnabled);
}

void KeyboardController::UpdateKeyboardAsRequestedBy(
    mojom::KeyboardEnableFlag flag) {
  if (IsKeyboardEnableRequested()) {
    // Note that there are two versions of the on-screen keyboard. A full layout
    // is provided for accessibility, which includes sticky modifier keys to
    // enable typing of hotkeys. A compact version is used in tablet mode to
    // provide a layout with larger keys to facilitate touch typing. In the
    // event that the a11y keyboard is being disabled, an on-screen keyboard
    // might still be enabled and a forced reset is required to pick up the
    // layout change.
    if (IsEnabled() && flag == mojom::KeyboardEnableFlag::kAccessibilityEnabled)
      RebuildKeyboardIfEnabled();
    else
      EnableKeyboard();
  } else {
    DisableKeyboard();
  }
}

bool KeyboardController::IsKeyboardOverscrollEnabled() const {
  if (!IsEnabled())
    return false;

  // Users of the sticky accessibility on-screen keyboard are likely to be using
  // mouse input, which may interfere with overscrolling.
  if (IsEnabled() && !IsOverscrollAllowed())
    return false;

  // If overscroll enabled behavior is set, use it instead. Currently
  // login / out-of-box disable keyboard overscroll. http://crbug.com/363635
  if (keyboard_config_.overscroll_behavior !=
      mojom::KeyboardOverscrollBehavior::kDefault) {
    return keyboard_config_.overscroll_behavior ==
           mojom::KeyboardOverscrollBehavior::kEnabled;
  }

  return true;
}

// private
void KeyboardController::HideKeyboard(HideReason reason) {
  TRACE_EVENT0("vk", "HideKeyboard");

  switch (state_) {
    case KeyboardControllerState::UNKNOWN:
    case KeyboardControllerState::INITIAL:
    case KeyboardControllerState::HIDDEN:
      return;
    case KeyboardControllerState::LOADING_EXTENSION:
      show_on_keyboard_window_load_ = false;
      return;

    case KeyboardControllerState::WILL_HIDE:
    case KeyboardControllerState::SHOWN: {
      SetTouchEventLogging(true /* enable */);

      // Log whether this was a user or system (automatic) action.
      switch (reason) {
        case HIDE_REASON_SYSTEM_EXPLICIT:
        case HIDE_REASON_SYSTEM_IMPLICIT:
        case HIDE_REASON_SYSTEM_TEMPORARY:
          LogKeyboardControlEvent(KEYBOARD_CONTROL_HIDE_AUTO);
          break;
        case HIDE_REASON_USER_EXPLICIT:
        case HIDE_REASON_USER_IMPLICIT:
          LogKeyboardControlEvent(KEYBOARD_CONTROL_HIDE_USER);
          break;
      }

      // Decide whether regaining focus in a web-based text field should cause
      // the keyboard to come back.
      switch (reason) {
        case HIDE_REASON_SYSTEM_IMPLICIT:
          time_of_last_blur_ = base::Time::Now();
          break;

        case HIDE_REASON_SYSTEM_TEMPORARY:
        case HIDE_REASON_SYSTEM_EXPLICIT:
        case HIDE_REASON_USER_EXPLICIT:
        case HIDE_REASON_USER_IMPLICIT:
          time_of_last_blur_ = base::Time::UnixEpoch();
          break;
      }

      NotifyKeyboardBoundsChanging(gfx::Rect());

      set_keyboard_locked(false);

      aura::Window* window = GetKeyboardWindow();
      DCHECK(window);

      animation_observer_ = std::make_unique<CallbackAnimationObserver>(
          base::BindOnce(&KeyboardController::HideAnimationFinished,
                         base::Unretained(this)));
      ui::ScopedLayerAnimationSettings layer_animation_settings(
          window->layer()->GetAnimator());
      layer_animation_settings.AddObserver(animation_observer_.get());

      {
        // Scoped settings go into effect when scope ends.
        ::wm::ScopedHidingAnimationSettings hiding_settings(window);
        container_behavior_->DoHidingAnimation(window, &hiding_settings);
      }

      ui_->HideKeyboardWindow();
      ChangeState(KeyboardControllerState::HIDDEN);

      for (KeyboardControllerObserver& observer : observer_list_)
        observer.OnKeyboardHidden(reason == HIDE_REASON_SYSTEM_TEMPORARY);

      break;
    }
    case KeyboardControllerState::COUNT:
      NOTREACHED();
  }
}

void KeyboardController::HideKeyboardByUser() {
  HideKeyboard(HIDE_REASON_USER_EXPLICIT);
}

void KeyboardController::HideKeyboardImplicitlyByUser() {
  if (!keyboard_locked_)
    HideKeyboard(HIDE_REASON_USER_IMPLICIT);
}

void KeyboardController::HideKeyboardTemporarilyForTransition() {
  HideKeyboard(HIDE_REASON_SYSTEM_TEMPORARY);
}

void KeyboardController::HideKeyboardExplicitlyBySystem() {
  HideKeyboard(HIDE_REASON_SYSTEM_EXPLICIT);
}

void KeyboardController::HideKeyboardImplicitlyBySystem() {
  if (state_ != KeyboardControllerState::SHOWN || keyboard_locked_)
    return;

  ChangeState(KeyboardControllerState::WILL_HIDE);

  base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
      FROM_HERE,
      base::BindOnce(&KeyboardController::HideKeyboard,
                     weak_factory_will_hide_.GetWeakPtr(),
                     HIDE_REASON_SYSTEM_IMPLICIT),
      kHideKeyboardDelay);
}

// private
void KeyboardController::HideAnimationFinished() {
  if (state_ == KeyboardControllerState::HIDDEN) {
    if (queued_container_type_) {
      SetContainerBehaviorInternal(queued_container_type_->container_type());
      // The position of the container window will be adjusted shortly in
      // |PopulateKeyboardContent| before showing animation, so we can set the
      // passed bounds directly.
      if (queued_container_type_->target_bounds())
        SetKeyboardWindowBounds(
            queued_container_type_->target_bounds().value());
      ShowKeyboard(false /* lock */);
    }

    if (queued_display_change_) {
      ShowKeyboardInDisplay(queued_display_change_->new_display());
      SetKeyboardWindowBounds(queued_display_change_->new_bounds_in_local());
      queued_display_change_ = nullptr;
    }
  }
}

// private
void KeyboardController::ShowAnimationFinished() {
  MarkKeyboardLoadFinished();

  // Notify observers after animation finished to prevent reveal desktop
  // background during animation.
  NotifyKeyboardBoundsChanging(GetKeyboardWindow()->GetBoundsInRootWindow());
}

// private
void KeyboardController::SetContainerBehaviorInternal(
    mojom::ContainerType type) {
  // Reset the hit test event targeter because the hit test bounds will
  // be wrong when container type changes and may cause the UI to be unusable.
  if (GetKeyboardWindow())
    GetKeyboardWindow()->SetEventTargeter(nullptr);

  switch (type) {
    case mojom::ContainerType::kFullWidth:
      container_behavior_ = std::make_unique<ContainerFullWidthBehavior>(this);
      break;
    case mojom::ContainerType::kFloating:
      container_behavior_ = std::make_unique<ContainerFloatingBehavior>(this);
      break;
    case mojom::ContainerType::kFullscreen:
      container_behavior_ = std::make_unique<ContainerFullscreenBehavior>(this);
      break;
    default:
      NOTREACHED();
  }
}

void KeyboardController::ShowKeyboard(bool lock) {
  DVLOG(1) << "ShowKeyboard";
  set_keyboard_locked(lock);
  ShowKeyboardInternal(layout_delegate_->GetContainerForDefaultDisplay());
}

void KeyboardController::ShowKeyboardInDisplay(
    const display::Display& display) {
  DVLOG(1) << "ShowKeyboardInDisplay: " << display.id();
  set_keyboard_locked(true);
  ShowKeyboardInternal(layout_delegate_->GetContainerForDisplay(display));
}

gfx::Rect KeyboardController::GetVisualBoundsInScreen() const {
  gfx::Rect visual_bounds_in_screen = visual_bounds_in_root_;
  ::wm::ConvertRectToScreen(GetRootWindow(), &visual_bounds_in_screen);
  return visual_bounds_in_screen;
}

void KeyboardController::LoadKeyboardWindowInBackground() {
  DCHECK_EQ(state_, KeyboardControllerState::INITIAL);

  TRACE_EVENT0("vk", "LoadKeyboardWindowInBackground");

  // For now, using Unretained is safe here because the |ui_| is owned by
  // |this| and the callback does not outlive |ui_|.
  // TODO(https://crbug.com/845780): Use a weak ptr here in case this
  // assumption changes.
  DVLOG(1) << "LoadKeyboardWindow";
  aura::Window* keyboard_window = ui_->LoadKeyboardWindow(base::BindOnce(
      &KeyboardController::NotifyKeyboardWindowLoaded, base::Unretained(this)));
  keyboard_window->AddPreTargetHandler(&event_handler_);
  keyboard_window->AddObserver(this);
  parent_container_->AddChild(keyboard_window);

  ChangeState(KeyboardControllerState::LOADING_EXTENSION);
}

ui::InputMethod* KeyboardController::GetInputMethodForTest() {
  return ui_->GetInputMethod();
}

void KeyboardController::EnsureCaretInWorkAreaForTest(
    const gfx::Rect& occluded_bounds_in_screen) {
  EnsureCaretInWorkArea(occluded_bounds_in_screen);
}

// ContainerBehavior::Delegate overrides

bool KeyboardController::IsKeyboardLocked() const {
  return keyboard_locked_;
}

gfx::Rect KeyboardController::GetBoundsInScreen() const {
  return GetKeyboardWindow()->GetBoundsInScreen();
}

void KeyboardController::MoveKeyboardWindow(const gfx::Rect& new_bounds) {
  DCHECK(IsKeyboardVisible());
  SetKeyboardWindowBounds(new_bounds);
}

void KeyboardController::MoveKeyboardWindowToDisplay(
    const display::Display& display,
    const gfx::Rect& new_bounds_in_root) {
  queued_display_change_ =
      std::make_unique<QueuedDisplayChange>(display, new_bounds_in_root);
  HideKeyboardTemporarilyForTransition();
}

// aura::WindowObserver overrides

void KeyboardController::OnWindowAddedToRootWindow(aura::Window* window) {
  container_behavior_->SetCanonicalBounds(GetKeyboardWindow(),
                                          GetRootWindow()->bounds());
}

void KeyboardController::OnWindowBoundsChanged(
    aura::Window* window,
    const gfx::Rect& old_bounds_in_root,
    const gfx::Rect& new_bounds_in_root,
    ui::PropertyChangeReason reason) {
  if (!GetKeyboardWindow())
    return;

  // |window| could be the root window (for detecting screen rotations) or the
  // keyboard window (for detecting keyboard bounds changes).
  if (window == GetRootWindow())
    container_behavior_->SetCanonicalBounds(GetKeyboardWindow(),
                                            new_bounds_in_root);
  else if (window == GetKeyboardWindow())
    NotifyKeyboardBoundsChanging(new_bounds_in_root);
}

// InputMethodObserver overrides

void KeyboardController::OnInputMethodDestroyed(
    const ui::InputMethod* input_method) {
  ime_observer_.RemoveAll();
  OnTextInputStateChanged(nullptr);
}

void KeyboardController::OnTextInputStateChanged(
    const ui::TextInputClient* client) {
  TRACE_EVENT0("vk", "OnTextInputStateChanged");

  bool focused =
      client && (client->GetTextInputType() != ui::TEXT_INPUT_TYPE_NONE &&
                 client->GetTextInputMode() != ui::TEXT_INPUT_MODE_NONE);
  bool should_hide = !focused && container_behavior_->TextBlurHidesKeyboard();
  bool is_web =
      client && client->GetTextInputFlags() != ui::TEXT_INPUT_FLAG_NONE;

  if (should_hide) {
    switch (state_) {
      case KeyboardControllerState::LOADING_EXTENSION:
        show_on_keyboard_window_load_ = false;
        return;
      case KeyboardControllerState::SHOWN:
        HideKeyboardImplicitlyBySystem();
        return;
      default:
        return;
    }
  } else {
    switch (state_) {
      case KeyboardControllerState::WILL_HIDE:
        // Abort a pending keyboard hide.
        ChangeState(KeyboardControllerState::SHOWN);
        return;
      case KeyboardControllerState::HIDDEN:
        if (focused && is_web)
          ShowKeyboardIfWithinTransientBlurThreshold();
        return;
      default:
        break;
    }
    // Do not explicitly show the Virtual keyboard unless it is in the process
    // of hiding or the hide duration was very short (transient blur). Instead,
    // the virtual keyboard is shown in response to a user gesture (mouse or
    // touch) that is received while an element has input focus. Showing the
    // keyboard requires an explicit call to OnShowVirtualKeyboardIfEnabled.
  }
}

void KeyboardController::ShowKeyboardIfWithinTransientBlurThreshold() {
  if (base::Time::Now() - time_of_last_blur_ < kTransientBlurThreshold)
    ShowKeyboard(false);
}

void KeyboardController::OnShowVirtualKeyboardIfEnabled() {
  DVLOG(1) << "OnShowVirtualKeyboardIfEnabled: " << IsEnabled();
  // Calling |ShowKeyboardInternal| may move the keyboard to another display.
  if (IsEnabled() && !keyboard_locked_)
    ShowKeyboardInternal(layout_delegate_->GetContainerForDefaultDisplay());
}

void KeyboardController::ShowKeyboardInternal(aura::Window* target_container) {
  MarkKeyboardLoadStarted();
  PopulateKeyboardContent(target_container);
  UpdateInputMethodObserver();
}

void KeyboardController::PopulateKeyboardContent(
    aura::Window* target_container) {
  DCHECK_NE(state_, KeyboardControllerState::INITIAL);

  DVLOG(1) << "PopulateKeyboardContent: " << StateToStr(state_);
  TRACE_EVENT0("vk", "PopulateKeyboardContent");

  MoveToParentContainer(target_container);

  aura::Window* keyboard_window = GetKeyboardWindow();
  DCHECK(keyboard_window);
  DCHECK_EQ(parent_container_, keyboard_window->parent());

  switch (state_) {
    case KeyboardControllerState::SHOWN:
      return;
    case KeyboardControllerState::LOADING_EXTENSION:
      show_on_keyboard_window_load_ = true;
      return;
    default:
      break;
  }

  ui_->ReloadKeyboardIfNeeded();

  SetTouchEventLogging(false /* enable */);

  switch (state_) {
    case KeyboardControllerState::WILL_HIDE:
      ChangeState(KeyboardControllerState::SHOWN);
      return;
    default:
      break;
  }

  DCHECK_EQ(state_, KeyboardControllerState::HIDDEN);

  // If the container is not animating, makes sure the position and opacity
  // are at begin states for animation.
  container_behavior_->InitializeShowAnimationStartingState(keyboard_window);

  LogKeyboardControlEvent(KEYBOARD_CONTROL_SHOW);
  RecordUkmKeyboardShown();

  ui::LayerAnimator* container_animator =
      keyboard_window->layer()->GetAnimator();
  container_animator->set_preemption_strategy(
      ui::LayerAnimator::IMMEDIATELY_ANIMATE_TO_NEW_TARGET);

  ui_->ShowKeyboardWindow();

  animation_observer_ =
      std::make_unique<CallbackAnimationObserver>(base::BindOnce(
          &KeyboardController::ShowAnimationFinished, base::Unretained(this)));
  ui::ScopedLayerAnimationSettings settings(container_animator);
  settings.AddObserver(animation_observer_.get());

  container_behavior_->DoShowingAnimation(keyboard_window, &settings);

  // the queued container behavior will notify JS to change layout when it
  // gets destroyed.
  queued_container_type_ = nullptr;

  ChangeState(KeyboardControllerState::SHOWN);

  UMA_HISTOGRAM_ENUMERATION("InputMethod.VirtualKeyboard.ContainerBehavior",
                            GetActiveContainerType(),
                            mojom::ContainerType::kMaxValue);
}

bool KeyboardController::WillHideKeyboard() const {
  bool res = weak_factory_will_hide_.HasWeakPtrs();
  DCHECK_EQ(res, state_ == KeyboardControllerState::WILL_HIDE);
  return res;
}

void KeyboardController::NotifyKeyboardConfigChanged() {
  for (KeyboardControllerObserver& observer : observer_list_)
    observer.OnKeyboardConfigChanged();
}

void KeyboardController::CheckStateTransition(KeyboardControllerState prev,
                                              KeyboardControllerState next) {
  std::stringstream error_message;
  const bool valid_transition = IsAllowedStateTransition(prev, next);
  if (!valid_transition)
    error_message << "Unexpected transition";

  // Emit UMA
  const int transition_record =
      (valid_transition ? 1 : -1) *
      (static_cast<int>(prev) * 1000 + static_cast<int>(next));
  base::UmaHistogramSparse("VirtualKeyboard.ControllerStateTransition",
                           transition_record);
  UMA_HISTOGRAM_BOOLEAN("VirtualKeyboard.ControllerStateTransitionIsValid",
                        transition_record > 0);

  DCHECK(error_message.str().empty())
      << "State: " << StateToStr(prev) << " -> " << StateToStr(next) << " "
      << error_message.str();
}

void KeyboardController::ChangeState(KeyboardControllerState state) {
  CheckStateTransition(state_, state);
  if (state_ == state)
    return;

  state_ = state;

  if (state != KeyboardControllerState::WILL_HIDE)
    weak_factory_will_hide_.InvalidateWeakPtrs();
  if (state != KeyboardControllerState::LOADING_EXTENSION)
    show_on_keyboard_window_load_ = false;

  weak_factory_report_lingering_state_.InvalidateWeakPtrs();
  switch (state_) {
    case KeyboardControllerState::LOADING_EXTENSION:
    case KeyboardControllerState::WILL_HIDE:
      base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
          FROM_HERE,
          base::BindOnce(&KeyboardController::ReportLingeringState,
                         weak_factory_report_lingering_state_.GetWeakPtr()),
          kReportLingeringStateDelay);
      break;
    default:
      // Do nothing
      break;
  }
}

void KeyboardController::ReportLingeringState() {
  LOG(ERROR) << "KeyboardController lingering in " << StateToStr(state_);
  UMA_HISTOGRAM_ENUMERATION("VirtualKeyboard.LingeringIntermediateState",
                            state_, KeyboardControllerState::COUNT);
}

gfx::Rect KeyboardController::GetWorkspaceOccludedBoundsInScreen() const {
  if (!ui_)
    return gfx::Rect();

  const gfx::Rect visual_bounds_in_window(visual_bounds_in_root_.size());

  gfx::Rect occluded_bounds_in_screen =
      container_behavior_->GetOccludedBounds(visual_bounds_in_window);
  ::wm::ConvertRectToScreen(GetKeyboardWindow(), &occluded_bounds_in_screen);

  return occluded_bounds_in_screen;
}

gfx::Rect KeyboardController::GetKeyboardLockScreenOffsetBounds() const {
  // Overscroll is generally dependent on lock state, however, its behavior
  // temporarily overridden by a static field in certain lock screen contexts.
  // Furthermore, floating keyboard should never affect layout.
  if (!IsKeyboardOverscrollEnabled() &&
      container_behavior_->GetType() != mojom::ContainerType::kFloating &&
      container_behavior_->GetType() != mojom::ContainerType::kFullscreen) {
    return visual_bounds_in_root_;
  }
  return gfx::Rect();
}

void KeyboardController::SetOccludedBounds(const gfx::Rect& bounds_in_window) {
  container_behavior_->SetOccludedBounds(bounds_in_window);

  // Notify that only the occluded bounds have changed.
  if (IsKeyboardVisible())
    NotifyKeyboardBoundsChanging(visual_bounds_in_root_);
}

void KeyboardController::SetHitTestBounds(
    const std::vector<gfx::Rect>& bounds_in_window) {
  if (!GetKeyboardWindow())
    return;

  GetKeyboardWindow()->SetEventTargeter(
      std::make_unique<ShapedWindowTargeter>(bounds_in_window));
}

gfx::Rect KeyboardController::AdjustSetBoundsRequest(
    const gfx::Rect& display_bounds,
    const gfx::Rect& requested_bounds_in_screen) const {
  return container_behavior_->AdjustSetBoundsRequest(
      display_bounds, requested_bounds_in_screen);
}

bool KeyboardController::IsOverscrollAllowed() const {
  return container_behavior_->IsOverscrollAllowed();
}

bool KeyboardController::HandlePointerEvent(const ui::LocatedEvent& event) {
  const display::Display& current_display =
      display_util_.GetNearestDisplayToWindow(GetRootWindow());
  return container_behavior_->HandlePointerEvent(event, current_display);
}

void KeyboardController::SetContainerType(
    mojom::ContainerType type,
    const base::Optional<gfx::Rect>& target_bounds_in_root,
    base::OnceCallback<void(bool)> callback) {
  if (container_behavior_->GetType() == type) {
    std::move(callback).Run(false);
    return;
  }

  if (state_ == KeyboardControllerState::SHOWN) {
    // Keyboard is already shown. Hiding the keyboard at first then switching
    // container type.
    queued_container_type_ = std::make_unique<QueuedContainerType>(
        this, type, target_bounds_in_root, std::move(callback));
    HideKeyboard(HIDE_REASON_SYSTEM_TEMPORARY);
  } else {
    // Keyboard is hidden. Switching the container type immediately and invoking
    // the passed callback now.
    SetContainerBehaviorInternal(type);
    if (target_bounds_in_root)
      SetKeyboardWindowBounds(*target_bounds_in_root);
    DCHECK_EQ(GetActiveContainerType(), type);
    std::move(callback).Run(true /* change_successful */);
  }
}

void KeyboardController::RecordUkmKeyboardShown() {
  ui::TextInputClient* text_input_client = GetTextInputClient();
  if (!text_input_client)
    return;

  keyboard::RecordUkmKeyboardShown(
      text_input_client->GetClientSourceForMetrics(),
      text_input_client->GetTextInputType());
}

void KeyboardController::SetDraggableArea(const gfx::Rect& rect) {
  container_behavior_->SetDraggableArea(rect);
}

bool KeyboardController::IsKeyboardVisible() {
  if (state_ == KeyboardControllerState::SHOWN) {
    DCHECK(IsEnabled());
    return true;
  }
  return false;
}

void KeyboardController::KeyboardContentsLoaded(
    const gfx::Size& size) {
  if (!IsEnabled())
    return;
  ui_->KeyboardContentsLoaded(size);
}

ui::TextInputClient* KeyboardController::GetTextInputClient() {
  return ui_->GetInputMethod()->GetTextInputClient();
}

void KeyboardController::UpdateInputMethodObserver() {
  ui::InputMethod* ime = ui_->GetInputMethod();

  // IME could be null during initialization. Ignoring the case is okay because
  // UpdateInputMethodObserver() will be called later on.
  if (!ime)
    return;

  if (ime_observer_.IsObserving(ime))
    return;

  // Only observes the current active IME.
  ime_observer_.RemoveAll();
  ime_observer_.Add(ime);

  // Note: We used to call OnTextInputStateChanged(ime->GetTextInputClient())
  // here, but that can trigger HideKeyboardImplicitlyBySystem() from a call to
  // ShowKeyboard() when using mojo APIs in Chrome (SingleProcessMash) if
  // ime->GetTextInputClient() isn't focused.
}

void KeyboardController::EnsureCaretInWorkArea(
    const gfx::Rect& occluded_bounds_in_screen) {
  ui::InputMethod* ime = ui_->GetInputMethod();
  if (!ime)
    return;

  TRACE_EVENT0("vk", "EnsureCaretInWorkArea");

  if (IsOverscrollAllowed()) {
    ime->SetOnScreenKeyboardBounds(occluded_bounds_in_screen);
  } else if (ime->GetTextInputClient()) {
    ime->GetTextInputClient()->EnsureCaretNotInRect(occluded_bounds_in_screen);
  }
}

void KeyboardController::MarkKeyboardLoadStarted() {
  if (!keyboard_load_time_logged_)
    keyboard_load_time_start_ = base::Time::Now();
}

void KeyboardController::MarkKeyboardLoadFinished() {
  // Possible to get a load finished without a start if navigating directly to
  // chrome://keyboard.
  if (keyboard_load_time_start_.is_null())
    return;

  if (keyboard_load_time_logged_)
    return;

  // Log the delta only once.
  UMA_HISTOGRAM_TIMES("VirtualKeyboard.InitLatency.FirstLoad",
                      base::Time::Now() - keyboard_load_time_start_);
  keyboard_load_time_logged_ = true;
}

}  // namespace keyboard
