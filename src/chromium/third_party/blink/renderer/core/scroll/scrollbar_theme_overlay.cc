/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/core/scroll/scrollbar_theme_overlay.h"

#include "third_party/blink/public/platform/platform.h"
#include "third_party/blink/public/platform/web_rect.h"
#include "third_party/blink/public/platform/web_theme_engine.h"
#include "third_party/blink/renderer/core/scroll/scrollbar.h"
#include "third_party/blink/renderer/platform/graphics/graphics_context.h"
#include "third_party/blink/renderer/platform/graphics/paint/drawing_recorder.h"
#include "third_party/blink/renderer/platform/transforms/transformation_matrix.h"
#include "third_party/blink/renderer/platform/wtf/math_extras.h"

#include <algorithm>

namespace blink {

ScrollbarThemeOverlay::ScrollbarThemeOverlay(int thumb_thickness,
                                             int scrollbar_margin,
                                             HitTestBehavior allow_hit_test,
                                             Color color)
    : ScrollbarTheme(),
      thumb_thickness_(thumb_thickness),
      scrollbar_margin_(scrollbar_margin),
      allow_hit_test_(allow_hit_test),
      color_(color),
      use_solid_color_(true) {}

ScrollbarThemeOverlay::ScrollbarThemeOverlay(int thumb_thickness,
                                             int scrollbar_margin,
                                             HitTestBehavior allow_hit_test)
    : ScrollbarTheme(),
      thumb_thickness_(thumb_thickness),
      scrollbar_margin_(scrollbar_margin),
      allow_hit_test_(allow_hit_test),
      use_solid_color_(false) {}

bool ScrollbarThemeOverlay::ShouldRepaintAllPartsOnInvalidation() const {
  return false;
}

ScrollbarPart ScrollbarThemeOverlay::InvalidateOnThumbPositionChange(
    const Scrollbar&,
    float old_position,
    float new_position) const {
  return kNoPart;
}

int ScrollbarThemeOverlay::ScrollbarThickness(
    ScrollbarControlSize control_size) {
  return thumb_thickness_ + scrollbar_margin_;
}

int ScrollbarThemeOverlay::ScrollbarMargin() const {
  return scrollbar_margin_;
}

bool ScrollbarThemeOverlay::UsesOverlayScrollbars() const {
  return true;
}

TimeDelta ScrollbarThemeOverlay::OverlayScrollbarFadeOutDelay() const {
  // TODO(bokan): Unit tests run without a theme engine. This is normally fine
  // because they expect to use ScrollbarThemeMock which doesn't use a theme
  // engine.  If overlays are turned on though, this class is used even if mock
  // scrollbars are on. We should either provide mock out a web theme engine for
  // unit tests or provide a mock version of this class.
  if (!Platform::Current()->ThemeEngine())
    return TimeDelta();
  WebThemeEngine::ScrollbarStyle style;
  Platform::Current()->ThemeEngine()->GetOverlayScrollbarStyle(&style);
  return style.fade_out_delay;
}

TimeDelta ScrollbarThemeOverlay::OverlayScrollbarFadeOutDuration() const {
  if (!Platform::Current()->ThemeEngine())
    return TimeDelta();
  WebThemeEngine::ScrollbarStyle style;
  Platform::Current()->ThemeEngine()->GetOverlayScrollbarStyle(&style);
  return style.fade_out_duration;
}

int ScrollbarThemeOverlay::ThumbLength(const Scrollbar& scrollbar) {
  int track_len = TrackLength(scrollbar);

  if (!scrollbar.TotalSize())
    return track_len;

  float proportion =
      static_cast<float>(scrollbar.VisibleSize()) / scrollbar.TotalSize();
  int length = round(proportion * track_len);
  int min_len = std::min(MinimumThumbLength(scrollbar), track_len);
  length = clampTo(length, min_len, track_len);
  return length;
}

bool ScrollbarThemeOverlay::HasThumb(const Scrollbar& scrollbar) {
  return true;
}

IntRect ScrollbarThemeOverlay::BackButtonRect(const Scrollbar&,
                                              ScrollbarPart,
                                              bool) {
  return IntRect();
}

IntRect ScrollbarThemeOverlay::ForwardButtonRect(const Scrollbar&,
                                                 ScrollbarPart,
                                                 bool) {
  return IntRect();
}

IntRect ScrollbarThemeOverlay::TrackRect(const Scrollbar& scrollbar, bool) {
  IntRect rect = scrollbar.FrameRect();
  if (scrollbar.Orientation() == kHorizontalScrollbar)
    rect.InflateX(-scrollbar_margin_);
  else
    rect.InflateY(-scrollbar_margin_);
  return rect;
}

int ScrollbarThemeOverlay::ThumbThickness(const Scrollbar&) {
  return thumb_thickness_;
}

void ScrollbarThemeOverlay::PaintThumb(GraphicsContext& context,
                                       const Scrollbar& scrollbar,
                                       const IntRect& rect) {
  if (DrawingRecorder::UseCachedDrawingIfPossible(context, scrollbar,
                                                  DisplayItem::kScrollbarThumb))
    return;

  DrawingRecorder recorder(context, scrollbar, DisplayItem::kScrollbarThumb);

  IntRect thumb_rect = rect;
  if (scrollbar.Orientation() == kHorizontalScrollbar) {
    thumb_rect.SetHeight(thumb_rect.Height() - scrollbar_margin_);
  } else {
    thumb_rect.SetWidth(thumb_rect.Width() - scrollbar_margin_);
    if (scrollbar.IsLeftSideVerticalScrollbar())
      thumb_rect.SetX(thumb_rect.X() + scrollbar_margin_);
  }

  if (use_solid_color_ || !Platform::Current()->ThemeEngine()) {
    context.FillRect(thumb_rect, color_);
    return;
  }

  WebThemeEngine::State state = WebThemeEngine::kStateNormal;

  if (!scrollbar.Enabled())
    state = WebThemeEngine::kStateDisabled;
  else if (scrollbar.PressedPart() == kThumbPart)
    state = WebThemeEngine::kStatePressed;
  else if (scrollbar.HoveredPart() == kThumbPart)
    state = WebThemeEngine::kStateHover;

  cc::PaintCanvas* canvas = context.Canvas();

  WebThemeEngine::Part part = WebThemeEngine::kPartScrollbarHorizontalThumb;
  if (scrollbar.Orientation() == kVerticalScrollbar)
    part = WebThemeEngine::kPartScrollbarVerticalThumb;

  blink::WebThemeEngine::ExtraParams params;
  params.scrollbar_thumb.scrollbar_theme =
      static_cast<WebScrollbarOverlayColorTheme>(
          scrollbar.GetScrollbarOverlayColorTheme());

  // Horizontally flip the canvas if it is left vertical scrollbar.
  if (scrollbar.IsLeftSideVerticalScrollbar()) {
    canvas->save();
    canvas->translate(rect.Width(), 0);
    canvas->scale(-1, 1);
  }

  Platform::Current()->ThemeEngine()->Paint(canvas, part, state, WebRect(rect),
                                            &params);

  if (scrollbar.IsLeftSideVerticalScrollbar())
    canvas->restore();
}

ScrollbarPart ScrollbarThemeOverlay::HitTest(const Scrollbar& scrollbar,
                                             const IntPoint& position) {
  if (allow_hit_test_ == kDisallowHitTest)
    return kNoPart;

  ScrollbarPart part = ScrollbarTheme::HitTest(scrollbar, position);
  if (part != kThumbPart)
    return kNoPart;

  return kThumbPart;
}

// static
ScrollbarThemeOverlay& ScrollbarThemeOverlay::MobileTheme() {
  static ScrollbarThemeOverlay* theme;
  if (!theme) {
    WebThemeEngine::ScrollbarStyle style = {3, 3, 0x80808080};  // default style
    if (Platform::Current()->ThemeEngine()) {
      Platform::Current()->ThemeEngine()->GetOverlayScrollbarStyle(&style);
    }
    theme = new ScrollbarThemeOverlay(
        style.thumb_thickness, style.scrollbar_margin,
        ScrollbarThemeOverlay::kDisallowHitTest, Color(style.color));
    theme->is_mobile_theme_ = true;
  }
  return *theme;
}

bool ScrollbarThemeOverlay::IsMobileTheme() const {
  return is_mobile_theme_;
}

bool ScrollbarThemeOverlay::UsesNinePatchThumbResource() const {
  WebThemeEngine* engine = Platform::Current()->ThemeEngine();
  if (!engine)
    return false;

  // Thumb orientation doesn't matter here.
  return engine->SupportsNinePatch(WebThemeEngine::kPartScrollbarVerticalThumb);
}

IntSize ScrollbarThemeOverlay::NinePatchThumbCanvasSize(
    const Scrollbar& scrollbar) const {
  DCHECK(UsesNinePatchThumbResource());

  WebThemeEngine::Part part =
      scrollbar.Orientation() == kVerticalScrollbar
          ? WebThemeEngine::kPartScrollbarVerticalThumb
          : WebThemeEngine::kPartScrollbarHorizontalThumb;

  DCHECK(Platform::Current()->ThemeEngine());
  return Platform::Current()->ThemeEngine()->NinePatchCanvasSize(part);
}

IntRect ScrollbarThemeOverlay::NinePatchThumbAperture(
    const Scrollbar& scrollbar) const {
  DCHECK(UsesNinePatchThumbResource());

  WebThemeEngine::Part part = WebThemeEngine::kPartScrollbarHorizontalThumb;
  if (scrollbar.Orientation() == kVerticalScrollbar)
    part = WebThemeEngine::kPartScrollbarVerticalThumb;

  DCHECK(Platform::Current()->ThemeEngine());
  return Platform::Current()->ThemeEngine()->NinePatchAperture(part);
}

int ScrollbarThemeOverlay::MinimumThumbLength(const Scrollbar& scrollbar) {
  if (scrollbar.Orientation() == kVerticalScrollbar) {
    return Platform::Current()
        ->ThemeEngine()
        ->GetSize(WebThemeEngine::kPartScrollbarVerticalThumb)
        .height;
  }

  return Platform::Current()
      ->ThemeEngine()
      ->GetSize(WebThemeEngine::kPartScrollbarHorizontalThumb)
      .width;
}

bool ScrollbarThemeOverlay::AllowsHitTest() const {
  return allow_hit_test_ != kDisallowHitTest;
}


}  // namespace blink
