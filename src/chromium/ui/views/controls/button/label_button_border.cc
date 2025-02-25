// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/button/label_button_border.h"

#include "base/logging.h"
#include "cc/paint/paint_flags.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/animation/animation.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/skia_util.h"
#include "ui/gfx/sys_color_change_listener.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/border.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/layout/layout_provider.h"
#include "ui/views/native_theme_delegate.h"
#if !defined(UI_VIEWS_PORT)
#include "ui/views/resources/grit/views_resources.h"
#endif // UI_VIEWS_PORT

namespace views {

namespace {

// Insets for the unified button images. This assumes that the images
// are of a 9 grid, of 5x5 size each.
constexpr int kButtonInsets = 5;

// The text-button hot and pushed image IDs; normal is unadorned by default.
#if !defined(UI_VIEWS_PORT)
constexpr int kTextHoveredImages[] = IMAGE_GRID(IDR_TEXTBUTTON_HOVER);
constexpr int kTextPressedImages[] = IMAGE_GRID(IDR_TEXTBUTTON_PRESSED);
#endif // UI_VIEWS_PORT

// A helper function to paint the appropriate broder images.
void PaintHelper(LabelButtonAssetBorder* border,
                 gfx::Canvas* canvas,
                 ui::NativeTheme::State state,
                 const gfx::Rect& rect,
                 const ui::NativeTheme::ExtraParams& extra) {
  Painter* painter =
      border->GetPainter(extra.button.is_focused,
                         Button::GetButtonStateFrom(state));
  // Paint any corresponding unfocused painter if there is no focused painter.
  if (!painter && extra.button.is_focused)
    painter = border->GetPainter(false, Button::GetButtonStateFrom(state));
  if (painter)
    Painter::PaintPainterAt(canvas, painter, rect);
}

}  // namespace

LabelButtonBorder::LabelButtonBorder() = default;
LabelButtonBorder::~LabelButtonBorder() = default;

bool LabelButtonBorder::PaintsButtonState(bool focused,
                                          Button::ButtonState state) {
  return false;
}

void LabelButtonBorder::Paint(const View& view, gfx::Canvas* canvas) {}

gfx::Insets LabelButtonBorder::GetInsets() const {
  return insets_;
}

gfx::Size LabelButtonBorder::GetMinimumSize() const {
  return gfx::Size();
}

LabelButtonAssetBorder::LabelButtonAssetBorder(Button::ButtonStyle style) {
  set_insets(GetDefaultInsetsForStyle(style));

  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  const gfx::Insets insets(kButtonInsets);
  if (style == Button::STYLE_BUTTON) {
#if !defined(UI_VIEWS_PORT)
    SetPainter(false, Button::STATE_NORMAL,
               Painter::CreateImagePainter(
                   *rb.GetImageSkiaNamed(IDR_BUTTON_NORMAL), insets));
    SetPainter(false, Button::STATE_HOVERED,
               Painter::CreateImagePainter(
                   *rb.GetImageSkiaNamed(IDR_BUTTON_HOVER), insets));
    SetPainter(false, Button::STATE_PRESSED,
               Painter::CreateImagePainter(
                   *rb.GetImageSkiaNamed(IDR_BUTTON_PRESSED), insets));
    SetPainter(false, Button::STATE_DISABLED,
               Painter::CreateImagePainter(
                   *rb.GetImageSkiaNamed(IDR_BUTTON_DISABLED), insets));
    SetPainter(true, Button::STATE_NORMAL,
               Painter::CreateImagePainter(
                   *rb.GetImageSkiaNamed(IDR_BUTTON_FOCUSED_NORMAL), insets));
    SetPainter(true, Button::STATE_HOVERED,
               Painter::CreateImagePainter(
                   *rb.GetImageSkiaNamed(IDR_BUTTON_FOCUSED_HOVER), insets));
    SetPainter(true, Button::STATE_PRESSED,
               Painter::CreateImagePainter(
                   *rb.GetImageSkiaNamed(IDR_BUTTON_FOCUSED_PRESSED), insets));
    SetPainter(true, Button::STATE_DISABLED,
               Painter::CreateImagePainter(
                   *rb.GetImageSkiaNamed(IDR_BUTTON_DISABLED), insets));
#endif // UI_VIEWS_PORT
  } else if (style == Button::STYLE_TEXTBUTTON) {
#if !defined(UI_VIEWS_PORT)
    SetPainter(false, Button::STATE_HOVERED,
               Painter::CreateImageGridPainter(kTextHoveredImages));
    SetPainter(false, Button::STATE_PRESSED,
               Painter::CreateImageGridPainter(kTextPressedImages));
#endif // UI_VIEWS_PORT
  }
}

LabelButtonAssetBorder::~LabelButtonAssetBorder() = default;

// static
gfx::Insets LabelButtonAssetBorder::GetDefaultInsetsForStyle(
    Button::ButtonStyle style) {
  gfx::Insets insets;
  if (style == Button::STYLE_BUTTON) {
    insets = gfx::Insets(8, 13);
  } else if (style == Button::STYLE_TEXTBUTTON) {
    insets = LayoutProvider::Get()->GetInsetsMetric(
        InsetsMetric::INSETS_LABEL_BUTTON);
  } else {
    NOTREACHED();
  }
  return insets;
}

bool LabelButtonAssetBorder::PaintsButtonState(bool focused,
                                               Button::ButtonState state) {
  // PaintHelper() above will paint the unfocused painter for a given state if
  // the button is focused, but there is no focused painter.
  return GetPainter(focused, state) || (focused && GetPainter(false, state));
}

void LabelButtonAssetBorder::Paint(const View& view, gfx::Canvas* canvas) {
  const NativeThemeDelegate* native_theme_delegate =
      static_cast<const LabelButton*>(&view);
  gfx::Rect rect(native_theme_delegate->GetThemePaintRect());
  ui::NativeTheme::ExtraParams extra;
  const gfx::Animation* animation = native_theme_delegate->GetThemeAnimation();
  ui::NativeTheme::State state = native_theme_delegate->GetThemeState(&extra);

  if (animation && animation->is_animating()) {
    // Linearly interpolate background and foreground painters during animation.
    uint8_t fg_alpha =
        static_cast<uint8_t>(animation->CurrentValueBetween(0, 255));

    const SkRect sk_rect = gfx::RectToSkRect(rect);
    cc::PaintCanvasAutoRestore auto_restore(canvas->sk_canvas(), false);
    canvas->sk_canvas()->saveLayer(&sk_rect, nullptr);

    {
      // First, modulate the background by 1 - alpha.
      cc::PaintCanvasAutoRestore auto_restore(canvas->sk_canvas(), false);
      canvas->sk_canvas()->saveLayerAlpha(&sk_rect, 255 - fg_alpha);
      state = native_theme_delegate->GetBackgroundThemeState(&extra);
      PaintHelper(this, canvas, state, rect, extra);
    }

    // Then modulate the foreground by alpha, and blend using kPlus_Mode.
    cc::PaintFlags flags;
    flags.setAlpha(fg_alpha);
    flags.setBlendMode(SkBlendMode::kPlus);
    canvas->sk_canvas()->saveLayer(&sk_rect, &flags);
    state = native_theme_delegate->GetForegroundThemeState(&extra);
    PaintHelper(this, canvas, state, rect, extra);
  } else {
    PaintHelper(this, canvas, state, rect, extra);
  }
}

gfx::Size LabelButtonAssetBorder::GetMinimumSize() const {
  gfx::Size minimum_size;
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < Button::STATE_COUNT; ++j) {
      if (painters_[i][j])
        minimum_size.SetToMax(painters_[i][j]->GetMinimumSize());
    }
  }
  return minimum_size;
}

Painter* LabelButtonAssetBorder::GetPainter(bool focused,
                                            Button::ButtonState state) {
  return painters_[focused ? 1 : 0][state].get();
}

void LabelButtonAssetBorder::SetPainter(bool focused,
                                        Button::ButtonState state,
                                        std::unique_ptr<Painter> painter) {
  painters_[focused ? 1 : 0][state] = std::move(painter);
}

}  // namespace views
