// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/image_view_base.h"

#include <utility>

#include "base/logging.h"
#if !defined(UI_VIEWS_NO_AX)
#include "ui/accessibility/ax_node_data.h"
#endif // UI_VIEWS_NO_AX
#include "ui/gfx/geometry/insets.h"
#include "ui/views/metadata/metadata_impl_macros.h"

namespace views {

ImageViewBase::ImageViewBase() = default;

ImageViewBase::~ImageViewBase() = default;

void ImageViewBase::SetImageSize(const gfx::Size& image_size) {
  image_size_ = image_size;
  PreferredSizeChanged();
}

gfx::Rect ImageViewBase::GetImageBounds() const {
  return gfx::Rect(image_origin_, GetImageSize());
}

void ImageViewBase::ResetImageSize() {
  image_size_.reset();
  PreferredSizeChanged();
}

#if !defined(UI_VIEWS_NO_AX)
void ImageViewBase::GetAccessibleNodeData(ui::AXNodeData* node_data) {
  node_data->role = ax::mojom::Role::kImage;
  node_data->SetName(GetAccessibleName());
}
#endif // UI_VIEWS_NO_AX

void ImageViewBase::SetHorizontalAlignment(Alignment alignment) {
  if (alignment != horizontal_alignment_) {
    horizontal_alignment_ = alignment;
    UpdateImageOrigin();
    OnPropertyChanged(&horizontal_alignment_, kPropertyEffectsPaint);
  }
}

ImageViewBase::Alignment ImageViewBase::GetHorizontalAlignment() const {
  return horizontal_alignment_;
}

void ImageViewBase::SetVerticalAlignment(Alignment alignment) {
  if (alignment != vertical_alignment_) {
    vertical_alignment_ = alignment;
    UpdateImageOrigin();
    OnPropertyChanged(&horizontal_alignment_, kPropertyEffectsPaint);
  }
}

ImageViewBase::Alignment ImageViewBase::GetVerticalAlignment() const {
  return vertical_alignment_;
}

void ImageViewBase::SetAccessibleName(const base::string16& accessible_name) {
  accessible_name_ = accessible_name;
}

const base::string16& ImageViewBase::GetAccessibleName() const {
  return accessible_name_.empty() ? tooltip_text_ : accessible_name_;
}

base::string16 ImageViewBase::GetTooltipText(const gfx::Point& p) const {
  return tooltip_text_;
}

gfx::Size ImageViewBase::CalculatePreferredSize() const {
  gfx::Size size = GetImageSize();
  size.Enlarge(GetInsets().width(), GetInsets().height());
  return size;
}

views::PaintInfo::ScaleType ImageViewBase::GetPaintScaleType() const {
  // ImageViewBase contains an image which is rastered at the device scale
  // factor. By default, the paint commands are recorded at a scale factor
  // slightly different from the device scale factor. Re-rastering the image at
  // this paint recording scale will result in a distorted image. Paint
  // recording scale might also not be uniform along the x & y axis, thus
  // resulting in further distortion in the aspect ratio of the final image.
  // |kUniformScaling| ensures that the paint recording scale is uniform along
  // the x & y axis and keeps the scale equal to the device scale factor.
  // See http://crbug.com/754010 for more details.
  return views::PaintInfo::ScaleType::kUniformScaling;
}

void ImageViewBase::OnBoundsChanged(const gfx::Rect& previous_bounds) {
  UpdateImageOrigin();
}

void ImageViewBase::UpdateImageOrigin() {
  gfx::Size image_size = GetImageSize();
  gfx::Insets insets = GetInsets();

  int x = 0;
  // In order to properly handle alignment of images in RTL locales, we need
  // to flip the meaning of trailing and leading. For example, if the
  // horizontal alignment is set to trailing, then we'll use left alignment for
  // the image instead of right alignment if the UI layout is RTL.
  Alignment actual_horizontal_alignment = horizontal_alignment_;
  if (base::i18n::IsRTL() && (horizontal_alignment_ != Alignment::kCenter)) {
    actual_horizontal_alignment = (horizontal_alignment_ == Alignment::kLeading)
                                      ? Alignment::kTrailing
                                      : Alignment::kLeading;
  }
  switch (actual_horizontal_alignment) {
    case Alignment::kLeading:
      x = insets.left();
      break;
    case Alignment::kTrailing:
      x = width() - insets.right() - image_size.width();
      break;
    case Alignment::kCenter:
      x = (width() - insets.width() - image_size.width()) / 2 + insets.left();
      break;
  }

  int y = 0;
  switch (vertical_alignment_) {
    case Alignment::kLeading:
      y = insets.top();
      break;
    case Alignment::kTrailing:
      y = height() - insets.bottom() - image_size.height();
      break;
    case Alignment::kCenter:
      y = (height() - insets.height() - image_size.height()) / 2 + insets.top();
      break;
  }

  image_origin_ = gfx::Point(x, y);
}

void ImageViewBase::PreferredSizeChanged() {
  View::PreferredSizeChanged();
  UpdateImageOrigin();
}

DEFINE_ENUM_CONVERTERS(
    ImageViewBase::Alignment,
    {ImageViewBase::Alignment::kLeading, base::ASCIIToUTF16("kLeading")},
    {ImageViewBase::Alignment::kCenter, base::ASCIIToUTF16("kCenter")},
    {ImageViewBase::Alignment::kTrailing, base::ASCIIToUTF16("kTrailing")})

BEGIN_METADATA(ImageViewBase)
METADATA_PARENT_CLASS(View)
ADD_PROPERTY_METADATA(ImageViewBase, Alignment, HorizontalAlignment)
ADD_PROPERTY_METADATA(ImageViewBase, Alignment, VerticalAlignment)
END_METADATA()

}  // namespace views
