// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/canvas.h"

#include <cmath>
#include <limits>

#include "base/i18n/rtl.h"
#include "base/logging.h"
#include "cc/paint/paint_flags.h"
#include "cc/paint/paint_shader.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkPath.h"
#include "third_party/skia/include/core/SkRefCnt.h"
#include "third_party/skia/include/effects/SkDashPathEffect.h"
#include "third_party/skia/include/effects/SkGradientShader.h"
#include "ui/gfx/font_list.h"
#include "ui/gfx/geometry/insets_f.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/rect_conversions.h"
#include "ui/gfx/geometry/rect_f.h"
#include "ui/gfx/geometry/safe_integer_conversions.h"
#include "ui/gfx/geometry/size_conversions.h"
#include "ui/gfx/scoped_canvas.h"
#include "ui/gfx/skia_paint_util.h"
#include "ui/gfx/skia_util.h"
#include "ui/gfx/switches.h"
#include "ui/gfx/transform.h"

namespace gfx {

Canvas::Canvas(const Size& size, float image_scale, bool is_opaque)
    : image_scale_(image_scale) {
  Size pixel_size = ScaleToCeiledSize(size, image_scale);
  canvas_ = CreateOwnedCanvas(pixel_size, is_opaque);

  SkScalar scale_scalar = SkFloatToScalar(image_scale);
  canvas_->scale(scale_scalar, scale_scalar);
}

Canvas::Canvas()
    : image_scale_(1.f), canvas_(CreateOwnedCanvas({0, 0}, false)) {}

Canvas::Canvas(cc::PaintCanvas* canvas, float image_scale)
    : image_scale_(image_scale), canvas_(canvas) {
  DCHECK(canvas_);
}

Canvas::~Canvas() {
}

void Canvas::RecreateBackingCanvas(const Size& size,
                                   float image_scale,
                                   bool is_opaque) {
  image_scale_ = image_scale;
  Size pixel_size = ScaleToFlooredSize(size, image_scale);
  canvas_ = CreateOwnedCanvas(pixel_size, is_opaque);

  SkScalar scale_scalar = SkFloatToScalar(image_scale);
  canvas_->scale(scale_scalar, scale_scalar);
}

// static
void Canvas::SizeStringInt(const base::string16& text,
                           const FontList& font_list,
                           int* width,
                           int* height,
                           int line_height,
                           int flags,
                           Typesetter typesetter) {
  float fractional_width = static_cast<float>(*width);
  float factional_height = static_cast<float>(*height);
  SizeStringFloat(text, font_list, &fractional_width, &factional_height,
                  line_height, flags, typesetter);
  *width = ToCeiledInt(fractional_width);
  *height = ToCeiledInt(factional_height);
}

// static
int Canvas::GetStringWidth(const base::string16& text,
                           const FontList& font_list,
                           Typesetter typesetter) {
  int width = 0, height = 0;
  SizeStringInt(text, font_list, &width, &height, 0, NO_ELLIPSIS, typesetter);
  return width;
}

// static
float Canvas::GetStringWidthF(const base::string16& text,
                              const FontList& font_list,
                              Typesetter typesetter) {
  float width = 0, height = 0;
  SizeStringFloat(text, font_list, &width, &height, 0, NO_ELLIPSIS, typesetter);
  return width;
}

// static
int Canvas::DefaultCanvasTextAlignment() {
  return base::i18n::IsRTL() ? TEXT_ALIGN_RIGHT : TEXT_ALIGN_LEFT;
}

void Canvas::DrawDashedRect(const RectF& inrect, SkColor color) {
  if (inrect.IsEmpty())
    return;
  RectF rect = inrect;

  cc::PaintFlags flags;
  flags.setColor(color);
  SkScalar intervals[] = {1.f, 1.f};
  flags.setStrokeWidth(1.f);
  flags.setStyle(cc::PaintFlags::kStroke_Style);
  rect.Inset(gfx::InsetsF(0.5f));

  flags.setPathEffect(SkDashPathEffect::Make(intervals, 2, 0));

  // Top-left to top-right.
  canvas_->drawLine(rect.x() - 0.5f, rect.y(), rect.right() + 0.5f, rect.y(),
                    flags);
  // Top-left to bottom-left.
  canvas_->drawLine(rect.right() + 0.5f, rect.bottom(), rect.x() - 0.5f,
                    rect.bottom(), flags);
  // Bottom-right to bottom-left.
  canvas_->drawLine(rect.x(), rect.y() - 0.5f, rect.x(), rect.bottom() + 0.5f,
                    flags);
  // Bottom-right to top-right.
  canvas_->drawLine(rect.right(), rect.bottom() + 0.5f, rect.right(),
                    rect.y() - 0.5f, flags);
}

float Canvas::UndoDeviceScaleFactor() {
  SkScalar scale_factor = 1.0f / image_scale_;
  canvas_->scale(scale_factor, scale_factor);
  return image_scale_;
}

void Canvas::Save() {
  canvas_->save();
}

void Canvas::SaveLayerAlpha(uint8_t alpha) {
  canvas_->saveLayerAlpha(NULL, alpha);
}

void Canvas::SaveLayerAlpha(uint8_t alpha, const Rect& layer_bounds) {
  SkRect bounds(RectToSkRect(layer_bounds));
  canvas_->saveLayerAlpha(&bounds, alpha);
}

void Canvas::SaveLayerWithFlags(const cc::PaintFlags& flags) {
  canvas_->saveLayer(nullptr /* bounds */, &flags);
}

void Canvas::Restore() {
  canvas_->restore();
}

void Canvas::ClipRect(const Rect& rect, SkClipOp op) {
  canvas_->clipRect(RectToSkRect(rect), op);
}

void Canvas::ClipRect(const RectF& rect, SkClipOp op) {
  canvas_->clipRect(RectFToSkRect(rect), op);
}

void Canvas::ClipPath(const SkPath& path, bool do_anti_alias) {
  canvas_->clipPath(path, SkClipOp::kIntersect, do_anti_alias);
}

bool Canvas::IsClipEmpty() const {
  return canvas_->isClipEmpty();
}

bool Canvas::GetClipBounds(Rect* bounds) {
  SkRect out;
  if (canvas_->getLocalClipBounds(&out)) {
    *bounds = ToEnclosingRect(SkRectToRectF(out));
    return true;
  }
  *bounds = gfx::Rect();
  return false;
}

void Canvas::Translate(const Vector2d& offset) {
  canvas_->translate(SkIntToScalar(offset.x()), SkIntToScalar(offset.y()));
}

void Canvas::Scale(float x_scale, float y_scale) {
  canvas_->scale(SkFloatToScalar(x_scale), SkFloatToScalar(y_scale));
}

void Canvas::DrawColor(SkColor color) {
  DrawColor(color, SkBlendMode::kSrcOver);
}

void Canvas::DrawColor(SkColor color, SkBlendMode mode) {
  canvas_->drawColor(color, mode);
}

void Canvas::FillRect(const Rect& rect, SkColor color) {
  FillRect(rect, color, SkBlendMode::kSrcOver);
}

void Canvas::FillRect(const Rect& rect, SkColor color, SkBlendMode mode) {
  cc::PaintFlags flags;
  flags.setColor(color);
  flags.setStyle(cc::PaintFlags::kFill_Style);
  flags.setBlendMode(mode);
  DrawRect(rect, flags);
}

void Canvas::DrawRect(const RectF& rect, SkColor color) {
  DrawRect(rect, color, SkBlendMode::kSrcOver);
}

void Canvas::DrawRect(const RectF& rect, SkColor color, SkBlendMode mode) {
  cc::PaintFlags flags;
  flags.setColor(color);
  flags.setStyle(cc::PaintFlags::kStroke_Style);
  // Set a stroke width of 0, which will put us down the stroke rect path.  If
  // we set a stroke width of 1, for example, this will internally create a
  // path and fill it, which causes problems near the edge of the canvas.
  flags.setStrokeWidth(SkIntToScalar(0));
  flags.setBlendMode(mode);

  DrawRect(rect, flags);
}

void Canvas::DrawRect(const Rect& rect, const cc::PaintFlags& flags) {
  DrawRect(RectF(rect), flags);
}

void Canvas::DrawRect(const RectF& rect, const cc::PaintFlags& flags) {
  canvas_->drawRect(RectFToSkRect(rect), flags);
}

void Canvas::DrawLine(const Point& p1, const Point& p2, SkColor color) {
  DrawLine(PointF(p1), PointF(p2), color);
}

void Canvas::DrawLine(const PointF& p1, const PointF& p2, SkColor color) {
  cc::PaintFlags flags;
  flags.setColor(color);
  flags.setStrokeWidth(SkIntToScalar(1));
  DrawLine(p1, p2, flags);
}

void Canvas::DrawLine(const Point& p1,
                      const Point& p2,
                      const cc::PaintFlags& flags) {
  DrawLine(PointF(p1), PointF(p2), flags);
}

void Canvas::DrawLine(const PointF& p1,
                      const PointF& p2,
                      const cc::PaintFlags& flags) {
  canvas_->drawLine(SkFloatToScalar(p1.x()), SkFloatToScalar(p1.y()),
                    SkFloatToScalar(p2.x()), SkFloatToScalar(p2.y()), flags);
}

void Canvas::DrawSharpLine(PointF p1, PointF p2, SkColor color) {
  ScopedCanvas scoped(this);
  float dsf = UndoDeviceScaleFactor();
  p1.Scale(dsf);
  p2.Scale(dsf);

  cc::PaintFlags flags;
  flags.setColor(color);
  flags.setStrokeWidth(SkFloatToScalar(std::floor(dsf)));

  DrawLine(p1, p2, flags);
}

void Canvas::Draw1pxLine(PointF p1, PointF p2, SkColor color) {
  ScopedCanvas scoped(this);
  float dsf = UndoDeviceScaleFactor();
  p1.Scale(dsf);
  p2.Scale(dsf);

  DrawLine(p1, p2, color);
}

void Canvas::DrawCircle(const Point& center_point,
                        int radius,
                        const cc::PaintFlags& flags) {
  canvas_->drawOval(
      SkRect::MakeLTRB(center_point.x() - radius, center_point.y() - radius,
                       center_point.x() + radius, center_point.y() + radius),
      flags);
}

void Canvas::DrawCircle(const PointF& center_point,
                        float radius,
                        const cc::PaintFlags& flags) {
  canvas_->drawOval(
      SkRect::MakeLTRB(center_point.x() - radius, center_point.y() - radius,
                       center_point.x() + radius, center_point.y() + radius),
      flags);
}

void Canvas::DrawRoundRect(const Rect& rect,
                           int radius,
                           const cc::PaintFlags& flags) {
  DrawRoundRect(RectF(rect), radius, flags);
}

void Canvas::DrawRoundRect(const RectF& rect,
                           float radius,
                           const cc::PaintFlags& flags) {
  canvas_->drawRoundRect(RectFToSkRect(rect), SkFloatToScalar(radius),
                         SkFloatToScalar(radius), flags);
}

void Canvas::DrawPath(const SkPath& path, const cc::PaintFlags& flags) {
  canvas_->drawPath(path, flags);
}

void Canvas::DrawFocusRect(const Rect& rect) {
  DrawFocusRect(RectF(rect));
}

void Canvas::DrawFocusRect(const RectF& rect) {
  DrawDashedRect(rect, SK_ColorGRAY);
}

void Canvas::DrawSolidFocusRect(RectF rect, SkColor color, int thickness) {
  cc::PaintFlags flags;
  flags.setColor(color);
  const float adjusted_thickness =
      std::floor(thickness * image_scale_) / image_scale_;
  flags.setStrokeWidth(SkFloatToScalar(adjusted_thickness));
  flags.setStyle(cc::PaintFlags::kStroke_Style);
  rect.Inset(gfx::InsetsF(adjusted_thickness / 2));
  DrawRect(rect, flags);
}

void Canvas::DrawImageInt(const ImageSkia& image, int x, int y) {
  cc::PaintFlags flags;
  DrawImageInt(image, x, y, flags);
}

void Canvas::DrawImageInt(const ImageSkia& image, int x, int y, uint8_t a) {
  cc::PaintFlags flags;
  flags.setAlpha(a);
  DrawImageInt(image, x, y, flags);
}

void Canvas::DrawImageInt(const ImageSkia& image,
                          int x,
                          int y,
                          const cc::PaintFlags& flags) {
  const ImageSkiaRep& image_rep = image.GetRepresentation(image_scale_);
  if (image_rep.is_null())
    return;
  float bitmap_scale = image_rep.scale();

  ScopedCanvas scoper(this);
  canvas_->scale(SkFloatToScalar(1.0f / bitmap_scale),
                 SkFloatToScalar(1.0f / bitmap_scale));
  canvas_->translate(std::round(x * bitmap_scale),
                     std::round(y * bitmap_scale));
  canvas_->saveLayer(nullptr, &flags);
  canvas_->drawPicture(image_rep.GetPaintRecord());
  canvas_->restore();
}

void Canvas::DrawImageInt(const ImageSkia& image,
                          int src_x,
                          int src_y,
                          int src_w,
                          int src_h,
                          int dest_x,
                          int dest_y,
                          int dest_w,
                          int dest_h,
                          bool filter) {
  cc::PaintFlags flags;
  DrawImageInt(image, src_x, src_y, src_w, src_h, dest_x, dest_y, dest_w,
               dest_h, filter, flags);
}

void Canvas::DrawImageInt(const ImageSkia& image,
                          int src_x,
                          int src_y,
                          int src_w,
                          int src_h,
                          int dest_x,
                          int dest_y,
                          int dest_w,
                          int dest_h,
                          bool filter,
                          const cc::PaintFlags& flags) {
  const ImageSkiaRep& image_rep = image.GetRepresentation(image_scale_);
  if (image_rep.is_null())
    return;
  bool remove_image_scale = true;
  DrawImageIntHelper(image_rep, src_x, src_y, src_w, src_h, dest_x, dest_y,
                     dest_w, dest_h, filter, flags, remove_image_scale);
}

void Canvas::DrawImageIntInPixel(const ImageSkiaRep& image_rep,
                                 int dest_x,
                                 int dest_y,
                                 int dest_w,
                                 int dest_h,
                                 bool filter,
                                 const cc::PaintFlags& flags) {
  int src_x = 0;
  int src_y = 0;
  int src_w = image_rep.pixel_width();
  int src_h = image_rep.pixel_height();
  // Don't remove image scale here, this function is used to draw the
  // (already scaled) |image_rep| at a 1:1 scale with the canvas.
  bool remove_image_scale = false;
  DrawImageIntHelper(image_rep, src_x, src_y, src_w, src_h, dest_x, dest_y,
                     dest_w, dest_h, filter, flags, remove_image_scale);
}

void Canvas::DrawImageInPath(const ImageSkia& image,
                             int x,
                             int y,
                             const SkPath& path,
                             const cc::PaintFlags& original_flags) {
  const ImageSkiaRep& image_rep = image.GetRepresentation(image_scale_);
  if (image_rep.is_null())
    return;

  SkMatrix matrix;
  matrix.setTranslate(SkIntToScalar(x), SkIntToScalar(y));
  cc::PaintFlags flags(original_flags);
  flags.setShader(CreateImageRepShader(image_rep, SkTileMode::kRepeat,
                                       SkTileMode::kRepeat, matrix));
  canvas_->drawPath(path, flags);
}

#if defined(ENABLE_SKOTTIE)
void Canvas::DrawSkottie(scoped_refptr<cc::SkottieWrapper> skottie,
                         const Rect& dst,
                         float t) {
  canvas_->drawSkottie(std::move(skottie), RectToSkRect(dst), t);
}
#endif

void Canvas::DrawStringRect(const base::string16& text,
                            const FontList& font_list,
                            SkColor color,
                            const Rect& display_rect) {
  DrawStringRectWithFlags(text, font_list, color, display_rect,
                          DefaultCanvasTextAlignment());
}

void Canvas::TileImageInt(const ImageSkia& image,
                          int x,
                          int y,
                          int w,
                          int h) {
  TileImageInt(image, 0, 0, x, y, w, h);
}

void Canvas::TileImageInt(const ImageSkia& image,
                          int src_x,
                          int src_y,
                          int dest_x,
                          int dest_y,
                          int w,
                          int h,
                          float tile_scale,
                          SkTileMode tile_mode_x,
                          SkTileMode tile_mode_y,
                          cc::PaintFlags* flags) {
  SkRect dest_rect = { SkIntToScalar(dest_x),
                       SkIntToScalar(dest_y),
                       SkIntToScalar(dest_x + w),
                       SkIntToScalar(dest_y + h) };
  if (!IntersectsClipRect(dest_rect))
    return;

  cc::PaintFlags paint_flags;
  if (!flags)
    flags = &paint_flags;

  if (InitPaintFlagsForTiling(image, src_x, src_y, tile_scale, tile_scale,
                              dest_x, dest_y, tile_mode_x, tile_mode_y, flags))
    canvas_->drawRect(dest_rect, *flags);
}

bool Canvas::InitPaintFlagsForTiling(const ImageSkia& image,
                                     int src_x,
                                     int src_y,
                                     float tile_scale_x,
                                     float tile_scale_y,
                                     int dest_x,
                                     int dest_y,
                                     SkTileMode tile_mode_x,
                                     SkTileMode tile_mode_y,
                                     cc::PaintFlags* flags) {
  const ImageSkiaRep& image_rep = image.GetRepresentation(image_scale_);
  if (image_rep.is_null())
    return false;

  SkMatrix shader_scale;
  shader_scale.setScale(SkFloatToScalar(tile_scale_x),
                        SkFloatToScalar(tile_scale_y));
  shader_scale.preTranslate(SkIntToScalar(-src_x), SkIntToScalar(-src_y));
  shader_scale.postTranslate(SkIntToScalar(dest_x), SkIntToScalar(dest_y));

  flags->setShader(CreateImageRepShader(image_rep, tile_mode_x, tile_mode_y,
                                        shader_scale));
  return true;
}

void Canvas::Transform(const gfx::Transform& transform) {
  canvas_->concat(transform.matrix());
}

SkBitmap Canvas::GetBitmap() const {
  DCHECK(bitmap_);
  return bitmap_.value();
}

bool Canvas::IntersectsClipRect(const SkRect& rect) {
  SkRect clip;
  return canvas_->getLocalClipBounds(&clip) && clip.intersects(rect);
}

void Canvas::DrawImageIntHelper(const ImageSkiaRep& image_rep,
                                int src_x,
                                int src_y,
                                int src_w,
                                int src_h,
                                int dest_x,
                                int dest_y,
                                int dest_w,
                                int dest_h,
                                bool filter,
                                const cc::PaintFlags& original_flags,
                                bool remove_image_scale) {
  DLOG_ASSERT(src_x + src_w < std::numeric_limits<int16_t>::max() &&
              src_y + src_h < std::numeric_limits<int16_t>::max());
  if (src_w <= 0 || src_h <= 0) {
    NOTREACHED() << "Attempting to draw bitmap from an empty rect!";
    return;
  }

  SkRect dest_rect = { SkIntToScalar(dest_x),
                       SkIntToScalar(dest_y),
                       SkIntToScalar(dest_x + dest_w),
                       SkIntToScalar(dest_y + dest_h) };
  if (!IntersectsClipRect(dest_rect))
    return;

  float user_scale_x = static_cast<float>(dest_w) / src_w;
  float user_scale_y = static_cast<float>(dest_h) / src_h;

  // Make a bitmap shader that contains the bitmap we want to draw. This is
  // basically what SkCanvas.drawBitmap does internally, but it gives us
  // more control over quality and will use the mipmap in the source image if
  // it has one, whereas drawBitmap won't.
  SkMatrix shader_scale;
  shader_scale.setScale(SkFloatToScalar(user_scale_x),
                        SkFloatToScalar(user_scale_y));
  shader_scale.preTranslate(SkIntToScalar(-src_x), SkIntToScalar(-src_y));
  shader_scale.postTranslate(SkIntToScalar(dest_x), SkIntToScalar(dest_y));

  cc::PaintFlags flags(original_flags);
  flags.setFilterQuality(filter ? kLow_SkFilterQuality : kNone_SkFilterQuality);
  flags.setShader(CreateImageRepShaderForScale(
      image_rep, SkTileMode::kRepeat, SkTileMode::kRepeat, shader_scale,
      remove_image_scale ? image_rep.scale() : 1.f));

  // The rect will be filled by the bitmap.
  canvas_->drawRect(dest_rect, flags);
}

cc::PaintCanvas* Canvas::CreateOwnedCanvas(const Size& size, bool is_opaque) {
  // SkBitmap cannot be zero-sized, but clients of Canvas sometimes request
  // that (and then later resize).
  int width = std::max(size.width(), 1);
  int height = std::max(size.height(), 1);
  SkAlphaType alpha = is_opaque ? kOpaque_SkAlphaType : kPremul_SkAlphaType;
  SkImageInfo info = SkImageInfo::MakeN32(width, height, alpha);

  bitmap_.emplace();
  bitmap_->allocPixels(info);
  // Ensure that the bitmap is zeroed, since the code expects that.
  memset(bitmap_->getPixels(), 0, bitmap_->computeByteSize());

  owned_canvas_.emplace(bitmap_.value());
  return &owned_canvas_.value();
}

}  // namespace gfx
