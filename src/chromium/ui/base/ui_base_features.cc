// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/ui_base_features.h"

#if defined(OS_WIN)
#include "base/win/windows_version.h"
#endif

namespace features {

#if defined(OS_WIN)
// If enabled, calculate native window occlusion - Windows-only.
const base::Feature kCalculateNativeWinOcclusion{
    "CalculateNativeWinOcclusion", base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // OW_WIN

#if defined(OS_CHROMEOS)
// Integrate input method specific settings to Chrome OS settings page.
// https://crbug.com/895886.
const base::Feature kSettingsShowsPerKeyboardSettings = {
    "InputMethodIntegratedSettings", base::FEATURE_DISABLED_BY_DEFAULT};
#endif  // defined(OS_CHROMEOS)

// Update of the virtual keyboard settings UI as described in
// https://crbug.com/876901.
const base::Feature kInputMethodSettingsUiUpdate = {
    "InputMethodSettingsUiUpdate", base::FEATURE_DISABLED_BY_DEFAULT};

// Allows system caption style for WebVTT Captions.
const base::Feature kSystemCaptionStyle{"SystemCaptionStyle",
                                        base::FEATURE_DISABLED_BY_DEFAULT};

// Allows system keyboard event capture via the keyboard lock API.
const base::Feature kSystemKeyboardLock{"SystemKeyboardLock",
                                        base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature kNotificationIndicator = {
    "EnableNotificationIndicator", base::FEATURE_DISABLED_BY_DEFAULT};

bool IsNotificationIndicatorEnabled() {
  return base::FeatureList::IsEnabled(kNotificationIndicator);
}

// Enables GPU rasterization for all UI drawing (where not blacklisted).
const base::Feature kUiGpuRasterization = {"UiGpuRasterization",
#if defined(OS_MACOSX) || defined(OS_CHROMEOS)
                                           base::FEATURE_ENABLED_BY_DEFAULT
#else
                                           base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

bool IsUiGpuRasterizationEnabled() {
  return base::FeatureList::IsEnabled(kUiGpuRasterization);
}

// Enables scrolling with layers under ui using the ui::Compositor.
const base::Feature kUiCompositorScrollWithLayers = {
    "UiCompositorScrollWithLayers",
// TODO(https://crbug.com/615948): Use composited scrolling on all platforms.
#if defined(OS_MACOSX)
    base::FEATURE_ENABLED_BY_DEFAULT
#else
    base::FEATURE_DISABLED_BY_DEFAULT
    //base::FEATURE_ENABLED_BY_DEFAULT // TODO
#endif
};

// Enables compositor threaded scrollbar scrolling by mapping pointer events to
// gesture events.
const base::Feature kCompositorThreadedScrollbarScrolling = {
    "CompositorThreadedScrollbarScrolling", base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_WIN)
// Enables InputPane API for controlling on screen keyboard.
const base::Feature kInputPaneOnScreenKeyboard = {
    "InputPaneOnScreenKeyboard", base::FEATURE_ENABLED_BY_DEFAULT};

// Enables using WM_POINTER instead of WM_TOUCH for touch events.
const base::Feature kPointerEventsForTouch = {"PointerEventsForTouch",
                                              base::FEATURE_ENABLED_BY_DEFAULT};
// Enables using TSF (over IMM32) for IME.
const base::Feature kTSFImeSupport = {"TSFImeSupport",
                                      base::FEATURE_ENABLED_BY_DEFAULT};

bool IsUsingWMPointerForTouch() {
  return base::win::GetVersion() >= base::win::Version::WIN8 &&
         base::FeatureList::IsEnabled(kPointerEventsForTouch);
}

// Enables DirectManipulation API for processing Precision Touchpad events.
const base::Feature kPrecisionTouchpad{"PrecisionTouchpad",
                                       base::FEATURE_ENABLED_BY_DEFAULT};

// Enables Logging for DirectManipulation.
const base::Feature kPrecisionTouchpadLogging{
    "PrecisionTouchpadLogging", base::FEATURE_DISABLED_BY_DEFAULT};

// Enables Swipe left/right to navigation back/forward API for processing
// Precision Touchpad events.
const base::Feature kPrecisionTouchpadScrollPhase{
    "PrecisionTouchpadScrollPhase", base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // defined(OS_WIN)

#if defined(OS_WIN) || defined(OS_CHROMEOS)
const base::Feature kEnableAutomaticUiAdjustmentsForTouch{
    "EnableAutomaticUiAdjustmentsForTouch", base::FEATURE_ENABLED_BY_DEFAULT};
#endif  // defined(OS_WIN) || defined(OS_CHROMEOS)

#if defined(OS_WIN) || defined(OS_MACOSX) || defined(OS_LINUX)
// Enables stylus appearing as touch when in contact with digitizer.
const base::Feature kDirectManipulationStylus = {
    "DirectManipulationStylus",
#if defined(OS_WIN)
    base::FEATURE_ENABLED_BY_DEFAULT
#else
    base::FEATURE_DISABLED_BY_DEFAULT
#endif
};
#endif  // defined(OS_WIN) || defined(OS_MACOSX) || defined(OS_LINUX)

const base::Feature kMash = {"Mash", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature kSingleProcessMash = {"SingleProcessMash",
                                          base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_CHROMEOS)
// Connecting the client and IME engine via Mojo. https://crbug.com/937167
const base::Feature kMojoIMF = {"MojoIMF", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

const base::Feature kFormControlsRefresh = {"FormControlsRefresh",
                                            base::FEATURE_DISABLED_BY_DEFAULT};

bool IsFormControlsRefreshEnabled() {
  return base::FeatureList::IsEnabled(features::kFormControlsRefresh);
}

bool IsUsingWindowService() {
  return IsSingleProcessMash() || IsMultiProcessMash();
}

bool IsMultiProcessMash() {
  return base::FeatureList::IsEnabled(features::kMash);
}

bool IsSingleProcessMash() {
  return base::FeatureList::IsEnabled(features::kSingleProcessMash) &&
         !base::FeatureList::IsEnabled(features::kMash);
}

bool IsMojoImfEnabled() {
#if defined(OS_CHROMEOS)
  return base::FeatureList::IsEnabled(features::kMojoIMF);
#else
  return false;
#endif
}

bool IsAutomaticUiAdjustmentsForTouchEnabled() {
#if defined(OS_WIN) || defined(OS_CHROMEOS)
  return base::FeatureList::IsEnabled(
      features::kEnableAutomaticUiAdjustmentsForTouch);
#else
  return false;
#endif
}

#if defined(OS_MACOSX)
// When enabled, the NSWindows for apps will be created in the app's process,
// and will forward input to the browser process.
const base::Feature kHostWindowsInAppShimProcess{
    "HostWindowsInAppShimProcess", base::FEATURE_ENABLED_BY_DEFAULT};

bool HostWindowsInAppShimProcess() {
  return base::FeatureList::IsEnabled(kHostWindowsInAppShimProcess);
}
#endif  //  defined(OS_MACOSX)

const base::Feature kEnableOzoneDrmMojo = {"OzoneDrmMojo",
                                           base::FEATURE_DISABLED_BY_DEFAULT};

bool IsOzoneDrmMojo() {
  return base::FeatureList::IsEnabled(kEnableOzoneDrmMojo) ||
         IsMultiProcessMash();
}

#if defined(OS_MACOSX)
const base::Feature kDarkMode = {"DarkMode", base::FEATURE_ENABLED_BY_DEFAULT};
#else
const base::Feature kDarkMode = {"DarkMode", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

#if defined(OS_CHROMEOS)
const base::Feature kHandwritingGesture = {"HandwritingGesture",
                                           base::FEATURE_DISABLED_BY_DEFAULT};
#endif
}  // namespace features
