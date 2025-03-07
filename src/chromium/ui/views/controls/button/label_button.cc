// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/button/label_button.h"

#include <stddef.h>

#include <utility>

#include "base/lazy_instance.h"
#include "base/logging.h"
#include "build/build_config.h"
#if !defined(UI_VIEWS_NO_AX)
#include "ui/accessibility/ax_node_data.h"
#endif // UI_VIEWS_NO_AX
#include "ui/gfx/animation/throb_animation.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/font_list.h"
#include "ui/gfx/geometry/vector2d.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/animation/ink_drop.h"
#include "ui/views/background.h"
#include "ui/views/controls/button/label_button_border.h"
#include "ui/views/metadata/metadata_impl_macros.h"
#include "ui/views/painter.h"
#include "ui/views/style/platform_style.h"
#include "ui/views/view_class_properties.h"
#include "ui/views/window/dialog_delegate.h"

namespace views {

// static
const char LabelButton::kViewClassName[] = "LabelButton";

LabelButton::LabelButton(ButtonListener* listener,
                         const base::string16& text,
                         int button_context)
    : Button(listener),
      cached_normal_font_list_(
          style::GetFont(button_context, style::STYLE_PRIMARY)),
      cached_default_button_font_list_(
          style::GetFont(button_context, style::STYLE_DIALOG_BUTTON_DEFAULT)) {
  ink_drop_container_ = AddChildView(std::make_unique<InkDropContainerView>());
  ink_drop_container_->SetVisible(false);

  image_ = AddChildView(std::make_unique<ImageView>());
  image_->set_can_process_events_within_subtree(false);

  label_ =
      AddChildView(std::make_unique<LabelButtonLabel>(text, button_context));
  label_->SetAutoColorReadabilityEnabled(false);
  label_->SetHorizontalAlignment(gfx::ALIGN_TO_HEAD);

  constexpr int kHoverAnimationDurationMs = 170;
  SetAnimationDuration(kHoverAnimationDurationMs);
  SetTextInternal(text);
}

LabelButton::~LabelButton() = default;

gfx::ImageSkia LabelButton::GetImage(ButtonState for_state) const {
  if (for_state != STATE_NORMAL && button_state_images_[for_state].isNull())
    return button_state_images_[STATE_NORMAL];
  return button_state_images_[for_state];
}

void LabelButton::SetImage(ButtonState for_state, const gfx::ImageSkia& image) {
  button_state_images_[for_state] = image;
  UpdateImage();
}

base::string16 LabelButton::GetText() const {
  return label_->text();
}

void LabelButton::SetText(const base::string16& text) {
  SetTextInternal(text);
}

void LabelButton::SetTextColor(ButtonState for_state, SkColor color) {
  button_state_colors_[for_state] = color;
  if (for_state == STATE_DISABLED)
    label_->SetDisabledColor(color);
  else if (for_state == state())
    label_->SetEnabledColor(color);
  explicitly_set_colors_[for_state] = true;
}

void LabelButton::SetEnabledTextColors(SkColor color) {
  ButtonState states[] = {STATE_NORMAL, STATE_HOVERED, STATE_PRESSED};
  for (auto state : states)
    SetTextColor(state, color);
}

void LabelButton::SetTextShadows(const gfx::ShadowValues& shadows) {
  label_->SetShadows(shadows);
}

void LabelButton::SetTextSubpixelRenderingEnabled(bool enabled) {
  label_->SetSubpixelRenderingEnabled(enabled);
}

void LabelButton::SetElideBehavior(gfx::ElideBehavior elide_behavior) {
  label_->SetElideBehavior(elide_behavior);
}

void LabelButton::SetHorizontalAlignment(gfx::HorizontalAlignment alignment) {
  DCHECK_NE(gfx::ALIGN_TO_HEAD, alignment);
  horizontal_alignment_ = alignment;
  InvalidateLayout();
}

gfx::Size LabelButton::GetMinSize() const {
  return min_size_;
}

void LabelButton::SetMinSize(const gfx::Size& min_size) {
  if (GetMinSize() == min_size)
    return;
  min_size_ = min_size;
  ResetCachedPreferredSize();
  OnPropertyChanged(&min_size_, kPropertyEffectsNone);
}

gfx::Size LabelButton::GetMaxSize() const {
  return max_size_;
}

void LabelButton::SetMaxSize(const gfx::Size& max_size) {
  if (GetMaxSize() == max_size)
    return;
  max_size_ = max_size;
  ResetCachedPreferredSize();
  OnPropertyChanged(&max_size_, kPropertyEffectsNone);
}

bool LabelButton::GetIsDefault() const {
  return is_default_;
}

void LabelButton::SetIsDefault(bool is_default) {
  // TODO(estade): move this to MdTextButton once |style_| is removed.
  if (GetIsDefault() == is_default)
    return;
  is_default_ = is_default;
  ui::Accelerator accel(ui::VKEY_RETURN, ui::EF_NONE);
  if (is_default)
    AddAccelerator(accel);
  else
    RemoveAccelerator(accel);
  OnPropertyChanged(&is_default_, UpdateStyleToIndicateDefaultStatus());
}

void LabelButton::SetStyleDeprecated(ButtonStyle style) {
  // All callers currently pass STYLE_BUTTON, and should only call this once, to
  // change from the default style.
  DCHECK_EQ(style, STYLE_BUTTON);
  DCHECK_EQ(style_, STYLE_TEXTBUTTON);
  DCHECK(!GetWidget()) << "Can't change button style after adding to a Widget.";

  style_ = style;

  SetFocusPainter(nullptr);
  SetHorizontalAlignment(gfx::ALIGN_CENTER);
  SetFocusForPlatform();
  set_request_focus_on_press(true);
  SetMinSize(gfx::Size(PlatformStyle::kMinLabelButtonWidth,
                       PlatformStyle::kMinLabelButtonHeight));

  // Themed borders will be set once the button is added to a Widget, since that
  // provides the value of GetNativeTheme().
}

int LabelButton::GetImageLabelSpacing() const {
  return image_label_spacing_;
}

void LabelButton::SetImageLabelSpacing(int spacing) {
  if (GetImageLabelSpacing() == spacing)
    return;
  image_label_spacing_ = spacing;
  ResetCachedPreferredSize();
  OnPropertyChanged(&image_label_spacing_, kPropertyEffectsLayout);
}

std::unique_ptr<LabelButtonBorder> LabelButton::CreateDefaultBorder() const {
  if (style_ != Button::STYLE_TEXTBUTTON)
    return std::make_unique<LabelButtonAssetBorder>(style_);
  auto border = std::make_unique<LabelButtonBorder>();
  border->set_insets(
      views::LabelButtonAssetBorder::GetDefaultInsetsForStyle(style_));
  return border;
}

void LabelButton::SetBorder(std::unique_ptr<Border> border) {
  border_is_themed_border_ = false;
  View::SetBorder(std::move(border));
  ResetCachedPreferredSize();
}

gfx::Size LabelButton::CalculatePreferredSize() const {
  // Cache the computed size, as recomputing it is an expensive operation.
  if (!cached_preferred_size_) {
    // Use a temporary label copy for sizing to avoid calculation side-effects.
    Label label(GetText(), {label_->font_list()});
    label.SetLineHeight(label_->line_height());
    label.SetShadows(label_->shadows());

    if (style_ == STYLE_BUTTON) {
      // Some text appears wider when rendered normally than when rendered bold.
      // Accommodate the widest, as buttons may show bold and shouldn't resize.
      const int current_width = label.GetPreferredSize().width();
      label.SetFontList(cached_default_button_font_list_);
      if (label.GetPreferredSize().width() < current_width)
        label.SetFontList(label_->font_list());
    }

    // Calculate the required size.
    const gfx::Size preferred_label_size = label.GetPreferredSize();
    gfx::Size size = GetUnclampedSizeWithoutLabel();
    size.Enlarge(preferred_label_size.width(), 0);

    // Increase the height of the label (with insets) if larger.
    size.set_height(std::max(
        preferred_label_size.height() + GetInsets().height(), size.height()));

    size.SetToMax(GetMinSize());

    // Clamp size to max size (if valid).
    const gfx::Size max_size = GetMaxSize();
    if (max_size.width() > 0)
      size.set_width(std::min(max_size.width(), size.width()));
    if (max_size.height() > 0)
      size.set_height(std::min(max_size.height(), size.height()));

    cached_preferred_size_ = size;
  }

  return cached_preferred_size_.value();
}

int LabelButton::GetHeightForWidth(int width) const {
  const gfx::Size size_without_label = GetUnclampedSizeWithoutLabel();
  // Get label height for the remaining width.
  const int label_height_with_insets =
      label_->GetHeightForWidth(width - size_without_label.width()) +
      GetInsets().height();

  // Height is the larger of size without label and label height with insets.
  int height = std::max(size_without_label.height(), label_height_with_insets);

  height = std::max(height, GetMinSize().height());

  // Clamp height to the maximum height (if valid).
  const gfx::Size max_size = GetMaxSize();
  if (max_size.height() > 0)
    return std::min(max_size.height(), height);

  return height;
}

void LabelButton::Layout() {
  ink_drop_container_->SetBoundsRect(GetLocalBounds());

  // By default, GetChildAreaBounds() ignores the top and bottom border, but we
  // want the image to respect it.
  gfx::Rect child_area(GetChildAreaBounds());
  // The space that the label can use. Its actual bounds may be smaller if the
  // label is short.
  gfx::Rect label_area(child_area);

  gfx::Insets insets = GetInsets();
  child_area.Inset(insets);
  // Labels can paint over the vertical component of the border insets.
  label_area.Inset(insets.left(), 0, insets.right(), 0);

  gfx::Size image_size(image_->GetPreferredSize());
  image_size.SetToMin(child_area.size());

  if (!image_size.IsEmpty()) {
    int image_space = image_size.width() + GetImageLabelSpacing();
    if (horizontal_alignment_ == gfx::ALIGN_RIGHT)
      label_area.Inset(0, 0, image_space, 0);
    else
      label_area.Inset(image_space, 0, 0, 0);
  }

  gfx::Size label_size(
      std::min(label_area.width(), label_->GetPreferredSize().width()),
      label_area.height());

  gfx::Point image_origin(child_area.origin());
  if (label_->multi_line()) {
    // Right now this code currently only works for CheckBox and RadioButton
    // descendants that have multi-line enabled for their label.
    image_origin.Offset(
        0, std::max(
               0, (label_->font_list().GetHeight() - image_size.height()) / 2));
  } else {
    image_origin.Offset(0, (child_area.height() - image_size.height()) / 2);
  }
  if (horizontal_alignment_ == gfx::ALIGN_CENTER) {
    const int spacing = (image_size.width() > 0 && label_size.width() > 0)
                            ? GetImageLabelSpacing()
                            : 0;
    const int total_width = image_size.width() + label_size.width() +
        spacing;
    image_origin.Offset((child_area.width() - total_width) / 2, 0);
  } else if (horizontal_alignment_ == gfx::ALIGN_RIGHT) {
    image_origin.Offset(child_area.width() - image_size.width(), 0);
  }
  image_->SetBoundsRect(gfx::Rect(image_origin, image_size));

  gfx::Rect label_bounds = label_area;
  if (label_area.width() == label_size.width()) {
    // Label takes up the whole area.
  } else if (horizontal_alignment_ == gfx::ALIGN_CENTER) {
    label_bounds.ClampToCenteredSize(label_size);
  } else {
    label_bounds.set_size(label_size);
    if (horizontal_alignment_ == gfx::ALIGN_RIGHT)
      label_bounds.Offset(label_area.width() - label_size.width(), 0);
  }

  label_->SetBoundsRect(label_bounds);
  Button::Layout();
}

const char* LabelButton::GetClassName() const {
  return kViewClassName;
}

void LabelButton::EnableCanvasFlippingForRTLUI(bool flip) {
  Button::EnableCanvasFlippingForRTLUI(flip);
  image_->EnableCanvasFlippingForRTLUI(flip);
}

#if !defined(UI_VIEWS_NO_AX)
void LabelButton::GetAccessibleNodeData(ui::AXNodeData* node_data) {
  if (GetIsDefault())
    node_data->AddState(ax::mojom::State::kDefault);
  Button::GetAccessibleNodeData(node_data);
}
#endif // UI_VIEWS_NO_AX

ui::NativeTheme::Part LabelButton::GetThemePart() const {
  return ui::NativeTheme::kPushButton;
}

gfx::Rect LabelButton::GetThemePaintRect() const {
  return GetLocalBounds();
}

ui::NativeTheme::State LabelButton::GetThemeState(
    ui::NativeTheme::ExtraParams* params) const {
  GetExtraParams(params);
  switch (state()) {
    case STATE_NORMAL:
      return ui::NativeTheme::kNormal;
    case STATE_HOVERED:
      return ui::NativeTheme::kHovered;
    case STATE_PRESSED:
      return ui::NativeTheme::kPressed;
    case STATE_DISABLED:
      return ui::NativeTheme::kDisabled;
    case STATE_COUNT:
      NOTREACHED() << "Unknown state: " << state();
  }
  return ui::NativeTheme::kNormal;
}

const gfx::Animation* LabelButton::GetThemeAnimation() const {
  return &hover_animation();
}

ui::NativeTheme::State LabelButton::GetBackgroundThemeState(
    ui::NativeTheme::ExtraParams* params) const {
  GetExtraParams(params);
  return ui::NativeTheme::kNormal;
}

ui::NativeTheme::State LabelButton::GetForegroundThemeState(
    ui::NativeTheme::ExtraParams* params) const {
  GetExtraParams(params);
  return ui::NativeTheme::kHovered;
}

void LabelButton::UpdateImage() {
  image_->SetImage(GetImage(GetVisualState()));
}

void LabelButton::UpdateThemedBorder() {
  // Don't override borders set by others.
  if (!border_is_themed_border_)
    return;

  SetBorder(PlatformStyle::CreateThemedLabelButtonBorder(this));
  border_is_themed_border_ = true;
}

void LabelButton::AddInkDropLayer(ui::Layer* ink_drop_layer) {
  image()->SetPaintToLayer();
  image()->layer()->SetFillsBoundsOpaquely(false);
  ink_drop_container_->AddInkDropLayer(ink_drop_layer);
  InstallInkDropMask(ink_drop_layer);
}

void LabelButton::RemoveInkDropLayer(ui::Layer* ink_drop_layer) {
  image()->DestroyLayer();
  ResetInkDropMask();
  ink_drop_container_->RemoveInkDropLayer(ink_drop_layer);
}

gfx::Rect LabelButton::GetChildAreaBounds() {
  return GetLocalBounds();
}

void LabelButton::GetExtraParams(ui::NativeTheme::ExtraParams* params) const {
  params->button.checked = false;
  params->button.indeterminate = false;
  params->button.is_default = GetIsDefault();
  params->button.is_focused = HasFocus() && IsAccessibilityFocusable();
  params->button.has_border = false;
  params->button.classic_state = 0;
  params->button.background_color = label_->background_color();
}

void LabelButton::ResetColorsFromNativeTheme() {
  const ui::NativeTheme* theme = GetNativeTheme();
  bool button_style = style() == STYLE_BUTTON;
  // Button colors are used only for STYLE_BUTTON, otherwise we use label
  // colors. As it turns out, these are almost always the same color anyway in
  // pre-MD, although in the MD world labels and buttons get different colors.
  // TODO(estade): simplify this by removing STYLE_BUTTON.
  SkColor colors[STATE_COUNT] = {
      theme->GetSystemColor(button_style
                                ? ui::NativeTheme::kColorId_ButtonEnabledColor
                                : ui::NativeTheme::kColorId_LabelEnabledColor),
      theme->GetSystemColor(button_style
                                ? ui::NativeTheme::kColorId_ButtonHoverColor
                                : ui::NativeTheme::kColorId_LabelEnabledColor),
      theme->GetSystemColor(button_style
                                ? ui::NativeTheme::kColorId_ButtonHoverColor
                                : ui::NativeTheme::kColorId_LabelEnabledColor),
      theme->GetSystemColor(button_style
                                ? ui::NativeTheme::kColorId_ButtonDisabledColor
                                : ui::NativeTheme::kColorId_LabelDisabledColor),
  };

  // Use hardcoded colors for inverted color scheme support and STYLE_BUTTON.
  if (color_utils::IsInvertedColorScheme()) {
    colors[STATE_NORMAL] = colors[STATE_HOVERED] = colors[STATE_PRESSED] =
        SK_ColorWHITE;
    label_->SetBackgroundColor(SK_ColorBLACK);
    label_->SetBackground(CreateSolidBackground(SK_ColorBLACK));
    label_->SetAutoColorReadabilityEnabled(true);
    label_->SetShadows(gfx::ShadowValues());
  } else {
    if (style() == STYLE_BUTTON)
      PlatformStyle::ApplyLabelButtonTextStyle(label_, &colors);
    label_->SetBackground(nullptr);
    label_->SetAutoColorReadabilityEnabled(false);
  }

  for (size_t state = STATE_NORMAL; state < STATE_COUNT; ++state) {
    if (!explicitly_set_colors_[state]) {
      SetTextColor(static_cast<ButtonState>(state), colors[state]);
      explicitly_set_colors_[state] = false;
    }
  }
}

PropertyEffects LabelButton::UpdateStyleToIndicateDefaultStatus() {
  // Check that a subclass hasn't replaced the Label font. These buttons may
  // never be given default status.
  DCHECK_EQ(cached_normal_font_list_.GetFontSize(),
            label()->font_list().GetFontSize());
  // TODO(tapted): This should use style::GetFont(), but this part can just be
  // deleted when default buttons no longer go bold. Colors will need updating
  // still.
  label_->SetFontList(GetIsDefault() ? cached_default_button_font_list_
                                     : cached_normal_font_list_);
  ResetLabelEnabledColor();
  return kPropertyEffectsLayout;
}

void LabelButton::ChildPreferredSizeChanged(View* child) {
  PreferredSizeChanged();
}

void LabelButton::PreferredSizeChanged() {
  ResetCachedPreferredSize();
  Button::PreferredSizeChanged();
}

void LabelButton::OnFocus() {
  Button::OnFocus();
  // Typically the border renders differently when focused.
  SchedulePaint();
}

void LabelButton::OnBlur() {
  Button::OnBlur();
  // Typically the border renders differently when focused.
  SchedulePaint();
}

void LabelButton::OnThemeChanged() {
  ResetColorsFromNativeTheme();
  UpdateThemedBorder();
  ResetLabelEnabledColor();
  // Invalidate the layout to pickup the new insets from the border.
  InvalidateLayout();
  // The entire button has to be repainted here, since the native theme can
  // define the tint for the entire background/border/focus ring.
  SchedulePaint();
}

void LabelButton::StateChanged(ButtonState old_state) {
  const gfx::Size previous_image_size(image_->GetPreferredSize());
  UpdateImage();
  ResetLabelEnabledColor();
  label_->SetEnabled(state() != STATE_DISABLED);
  if (image_->GetPreferredSize() != previous_image_size)
    InvalidateLayout();
  Button::StateChanged(old_state);
}

void LabelButton::SetTextInternal(const base::string16& text) {
  SetAccessibleName(text);
  label_->SetText(text);
  // TODO(pkasting): Remove this and forward callback subscriptions to the
  // underlying label property when Label is converted to properties.
  OnPropertyChanged(label_, kPropertyEffectsNone);
}

void LabelButton::ResetCachedPreferredSize() {
  cached_preferred_size_ = base::nullopt;
}

gfx::Size LabelButton::GetUnclampedSizeWithoutLabel() const {
  const gfx::Size image_size = image_->GetPreferredSize();
  gfx::Size size = image_size;
  const gfx::Insets insets(GetInsets());
  size.Enlarge(insets.width(), insets.height());

  // Accommodate for spacing between image and text if both are present.
  if (!GetText().empty() && image_size.width() > 0)
    size.Enlarge(GetImageLabelSpacing(), 0);

  // Make the size at least as large as the minimum size needed by the border.
  if (border())
    size.SetToMax(border()->GetMinimumSize());

  return size;
}

void LabelButton::ResetLabelEnabledColor() {
  const SkColor color = button_state_colors_[state()];
  if (state() != STATE_DISABLED && label_->enabled_color() != color)
    label_->SetEnabledColor(color);
}

BEGIN_METADATA(LabelButton)
METADATA_PARENT_CLASS(Button)
ADD_PROPERTY_METADATA(LabelButton, base::string16, Text)
ADD_PROPERTY_METADATA(LabelButton, gfx::Size, MinSize)
ADD_PROPERTY_METADATA(LabelButton, gfx::Size, MaxSize)
ADD_PROPERTY_METADATA(LabelButton, bool, IsDefault)
ADD_PROPERTY_METADATA(LabelButton, int, ImageLabelSpacing)
END_METADATA()

}  // namespace views
