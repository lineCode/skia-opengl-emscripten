// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stddef.h>

#include "build/build_config.h"
#include "cc/layers/picture_layer.h"
#include "cc/layers/solid_color_layer.h"
#include "cc/test/fake_content_layer_client.h"
#include "cc/test/layer_tree_pixel_test.h"
#include "cc/test/pixel_comparator.h"
#include "cc/test/solid_color_content_layer_client.h"

#if !defined(OS_ANDROID)

namespace cc {
namespace {

class LayerTreeHostFiltersPixelTest
    : public LayerTreePixelTest,
      public ::testing::WithParamInterface<LayerTreePixelTest::PixelTestType> {
 protected:
  void InitializeSettings(LayerTreeSettings* settings) override {
    LayerTreePixelTest::InitializeSettings(settings);
    // Required so that device scale is inherited by content scale. True for
    // most tests, but can be overwritten before RunPixelTest() is called.
    settings->layer_transforms_should_scale_layer_contents =
        layer_transforms_should_scale_layer_contents_;
  }

  LayerTreePixelTest::PixelTestType GetPixelTestType() { return GetParam(); }

  // Text string for graphics backend of the RendererType. Suitable for
  // generating separate base line file paths.
  const char* GetRendererSuffix() {
    switch (GetPixelTestType()) {
      case LayerTreePixelTest::PIXEL_TEST_GL:
        return "gl";
      case LayerTreePixelTest::PIXEL_TEST_SKIA_GL:
        return "skia";
      case LayerTreePixelTest::PIXEL_TEST_SOFTWARE:
        return "sw";
    }
  }

  scoped_refptr<SolidColorLayer> BuildFilterWithGiantCropRect(
      bool masks_to_bounds) {
    scoped_refptr<SolidColorLayer> background =
        CreateSolidColorLayer(gfx::Rect(200, 200), SK_ColorWHITE);
    scoped_refptr<SolidColorLayer> filter_layer =
        CreateSolidColorLayer(gfx::Rect(50, 50, 100, 100), SK_ColorRED);

    // This matrix swaps the red and green channels, and has a slight
    // translation in the alpha component, so that it affects transparent
    // pixels.
    float matrix[20] = {
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 20 / 255.0f,
    };

    FilterOperations filters;
    SkImageFilter::CropRect cropRect(
        SkRect::MakeXYWH(-40000, -40000, 80000, 80000));
    filters.Append(FilterOperation::CreateReferenceFilter(
        sk_make_sp<ColorFilterPaintFilter>(SkColorFilters::Matrix(matrix),
                                           nullptr, &cropRect)));
    filter_layer->SetFilters(filters);
    background->SetMasksToBounds(masks_to_bounds);
    background->AddChild(filter_layer);

    return background;
  }

  bool layer_transforms_should_scale_layer_contents_ = true;
};

INSTANTIATE_TEST_SUITE_P(
    ,
    LayerTreeHostFiltersPixelTest,
    ::testing::Values(LayerTreePixelTest::PIXEL_TEST_GL,
                      LayerTreePixelTest::PIXEL_TEST_SKIA_GL,
                      LayerTreePixelTest::PIXEL_TEST_SOFTWARE));

using LayerTreeHostFiltersPixelTestNonSkia = LayerTreeHostFiltersPixelTest;

// TODO(crbug.com/948128): Enable these tests for Skia.
INSTANTIATE_TEST_SUITE_P(
    ,
    LayerTreeHostFiltersPixelTestNonSkia,
    ::testing::Values(LayerTreePixelTest::PIXEL_TEST_GL,
                      LayerTreePixelTest::PIXEL_TEST_SOFTWARE));

using LayerTreeHostFiltersPixelTestGL = LayerTreeHostFiltersPixelTest;

// TODO(crbug.com/948128): Enable these tests for Skia.
INSTANTIATE_TEST_SUITE_P(,
                         LayerTreeHostFiltersPixelTestGL,
                         ::testing::Values(LayerTreePixelTest::PIXEL_TEST_GL));

using LayerTreeHostFiltersPixelTestGPU = LayerTreeHostFiltersPixelTest;

INSTANTIATE_TEST_SUITE_P(
    ,
    LayerTreeHostFiltersPixelTestGPU,
    ::testing::Values(LayerTreePixelTest::PIXEL_TEST_GL,
                      LayerTreePixelTest::PIXEL_TEST_SKIA_GL));

TEST_P(LayerTreeHostFiltersPixelTestGPU, BackdropFilterBlurRect) {
  scoped_refptr<SolidColorLayer> background = CreateSolidColorLayer(
      gfx::Rect(200, 200), SK_ColorWHITE);

  // The green box is entirely behind a layer with backdrop blur, so it
  // should appear blurred on its edges.
  scoped_refptr<SolidColorLayer> green = CreateSolidColorLayer(
      gfx::Rect(50, 50, 100, 100), kCSSGreen);
  scoped_refptr<SolidColorLayer> blur = CreateSolidColorLayer(
      gfx::Rect(30, 30, 140, 140), SK_ColorTRANSPARENT);
  background->AddChild(green);
  background->AddChild(blur);

  FilterOperations filters;
  filters.Append(FilterOperation::CreateBlurFilter(
      2.f, SkBlurImageFilter::kClamp_TileMode));
  blur->SetBackdropFilters(filters);
  gfx::RRectF backdrop_filter_bounds(gfx::RectF(gfx::SizeF(blur->bounds())), 0);
  blur->SetBackdropFilterBounds(backdrop_filter_bounds);

#if defined(OS_WIN) || defined(ARCH_CPU_ARM64)
  // Windows and ARM64 have 436 pixels off by 1: crbug.com/259915
  float percentage_pixels_large_error = 1.09f;  // 436px / (200*200)
  float percentage_pixels_small_error = 0.0f;
  float average_error_allowed_in_bad_pixels = 1.f;
  int large_error_allowed = 1;
  int small_error_allowed = 0;
  pixel_comparator_.reset(new FuzzyPixelComparator(
      true,  // discard_alpha
      percentage_pixels_large_error,
      percentage_pixels_small_error,
      average_error_allowed_in_bad_pixels,
      large_error_allowed,
      small_error_allowed));
#endif

  RunPixelTest(GetPixelTestType(), background,
               base::FilePath(FILE_PATH_LITERAL("backdrop_filter_blur.png")));
}

TEST_P(LayerTreeHostFiltersPixelTestGPU, BackdropFilterBlurRounded) {
  scoped_refptr<SolidColorLayer> background =
      CreateSolidColorLayer(gfx::Rect(200, 200), SK_ColorWHITE);

  // The green box is entirely behind a layer with backdrop blur, so it
  // should appear blurred on its edges.
  scoped_refptr<SolidColorLayer> green =
      CreateSolidColorLayer(gfx::Rect(50, 50, 100, 100), kCSSGreen);
  scoped_refptr<SolidColorLayer> blur =
      CreateSolidColorLayer(gfx::Rect(46, 46, 108, 108), SK_ColorTRANSPARENT);
  background->AddChild(green);
  background->AddChild(blur);

  FilterOperations filters;
  filters.Append(FilterOperation::CreateBlurFilter(
      2.f, SkBlurImageFilter::kClamp_TileMode));
  blur->SetBackdropFilters(filters);
  gfx::RRectF backdrop_filter_bounds(gfx::RectF(gfx::SizeF(blur->bounds())), 14,
                                     16, 18, 20, 22, 30, 40, 50);
  blur->SetBackdropFilterBounds(backdrop_filter_bounds);

#if defined(OS_WIN) || defined(ARCH_CPU_ARM64)
  // Windows and ARM64 have 436 pixels off by 1: crbug.com/259915
  float percentage_pixels_large_error = 1.09f;  // 436px / (200*200)
  float percentage_pixels_small_error = 0.0f;
  float average_error_allowed_in_bad_pixels = 1.f;
  int large_error_allowed = 1;
  int small_error_allowed = 0;
  pixel_comparator_.reset(new FuzzyPixelComparator(
      true,  // discard_alpha
      percentage_pixels_large_error, percentage_pixels_small_error,
      average_error_allowed_in_bad_pixels, large_error_allowed,
      small_error_allowed));
#else
  pixel_comparator_ = std::make_unique<FuzzyPixelOffByOneComparator>(false);
#endif

  RunPixelTest(
      GetPixelTestType(), background,
      base::FilePath(FILE_PATH_LITERAL("backdrop_filter_blur_rounded.png")));
}

TEST_P(LayerTreeHostFiltersPixelTestGPU, BackdropFilterBlurOutsets) {
  scoped_refptr<SolidColorLayer> background = CreateSolidColorLayer(
      gfx::Rect(200, 200), SK_ColorWHITE);

  // The green border is outside the layer with backdrop blur, but the
  // backdrop blur should use pixels from outside its layer borders, up to the
  // radius of the blur effect. So the border should be blurred underneath the
  // top layer causing the green to bleed under the transparent layer, but not
  // in the 1px region between the transparent layer and the green border.
  scoped_refptr<SolidColorLayer> green_border = CreateSolidColorLayerWithBorder(
      gfx::Rect(1, 1, 198, 198), SK_ColorWHITE, 10, kCSSGreen);
  scoped_refptr<SolidColorLayer> blur = CreateSolidColorLayer(
      gfx::Rect(12, 12, 176, 176), SK_ColorTRANSPARENT);
  background->AddChild(green_border);
  background->AddChild(blur);

  FilterOperations filters;
  filters.Append(FilterOperation::CreateBlurFilter(
      5.f, SkBlurImageFilter::kClamp_TileMode));
  blur->SetBackdropFilters(filters);
  gfx::RRectF backdrop_filter_bounds(gfx::RectF(gfx::SizeF(blur->bounds())), 0);
  blur->SetBackdropFilterBounds(backdrop_filter_bounds);

#if defined(OS_WIN) || defined(_MIPS_ARCH_LOONGSON) || defined(ARCH_CPU_ARM64)
#if defined(OS_WIN) || defined(ARCH_CPU_ARM64)
  // Windows has 5.9325% pixels by at most 2: crbug.com/259922
  float percentage_pixels_large_error = 6.0f;
#else
  // Loongson has 8.685% pixels by at most 2: crbug.com/819110
  float percentage_pixels_large_error = 8.7f;
#endif
  float percentage_pixels_small_error = 0.0f;
  float average_error_allowed_in_bad_pixels = 2.f;
  int large_error_allowed = 2;
  int small_error_allowed = 0;
  pixel_comparator_.reset(new FuzzyPixelComparator(
      true,  // discard_alpha
      percentage_pixels_large_error,
      percentage_pixels_small_error,
      average_error_allowed_in_bad_pixels,
      large_error_allowed,
      small_error_allowed));
#endif

  RunPixelTest(
      GetPixelTestType(), background,
      base::FilePath(FILE_PATH_LITERAL("backdrop_filter_blur_outsets.png")));
}

TEST_P(LayerTreeHostFiltersPixelTestGL, BackdropFilterBlurOffAxis) {
  scoped_refptr<SolidColorLayer> background =
      CreateSolidColorLayer(gfx::Rect(200, 200), SK_ColorTRANSPARENT);

  // This verifies that the perspective of the clear layer (with black border)
  // does not influence the blending of the green box behind it. Also verifies
  // that the blur is correctly clipped inside the transformed clear layer.
  scoped_refptr<SolidColorLayer> green = CreateSolidColorLayer(
      gfx::Rect(50, 50, 100, 100), kCSSGreen);
  scoped_refptr<SolidColorLayer> blur = CreateSolidColorLayerWithBorder(
      gfx::Rect(30, 30, 120, 120), SK_ColorTRANSPARENT, 1, SK_ColorBLACK);
  background->AddChild(green);
  background->AddChild(blur);

  background->SetShouldFlattenTransform(false);
  background->Set3dSortingContextId(1);
  green->SetShouldFlattenTransform(false);
  green->Set3dSortingContextId(1);
  gfx::Transform background_transform;
  background_transform.ApplyPerspectiveDepth(200.0);
  background->SetTransform(background_transform);

  blur->SetShouldFlattenTransform(false);
  blur->Set3dSortingContextId(1);
  for (size_t i = 0; i < blur->children().size(); ++i)
    blur->children()[i]->Set3dSortingContextId(1);

  gfx::Transform blur_transform;
  blur_transform.Translate(55.0, 65.0);
  blur_transform.RotateAboutXAxis(85.0);
  blur_transform.RotateAboutYAxis(180.0);
  blur_transform.RotateAboutZAxis(20.0);
  blur_transform.Translate(-60.0, -60.0);
  blur->SetTransform(blur_transform);

  FilterOperations filters;
  filters.Append(FilterOperation::CreateBlurFilter(
      2.f, SkBlurImageFilter::kClamp_TileMode));
  blur->SetBackdropFilters(filters);
  // TODO(916311): Fix clipping for 3D transformed elements.
  blur->SetBackdropFilterBounds(gfx::RRectF());

#if defined(OS_WIN) || defined(ARCH_CPU_ARM64)
#if defined(OS_WIN)
  // Windows has 116 pixels off by at most 2: crbug.com/225027
  float percentage_pixels_large_error = 0.3f;  // 116px / (200*200), rounded up
  int large_error_allowed = 2;
#else
  float percentage_pixels_large_error = 0.25f;  // 96px / (200*200), rounded up
  int large_error_allowed = 1;
#endif
  float percentage_pixels_small_error = 0.0f;
  float average_error_allowed_in_bad_pixels = 1.f;
  int small_error_allowed = 0;
  pixel_comparator_.reset(new FuzzyPixelComparator(
      true,  // discard_alpha
      percentage_pixels_large_error,
      percentage_pixels_small_error,
      average_error_allowed_in_bad_pixels,
      large_error_allowed,
      small_error_allowed));
#endif

  RunPixelTest(
      GetPixelTestType(), background,
      base::FilePath(FILE_PATH_LITERAL("backdrop_filter_blur_off_axis.png")));
}

class LayerTreeHostFiltersScaledPixelTest
    : public LayerTreeHostFiltersPixelTest {
  void SetupTree() override {
    SetInitialDeviceScaleFactor(device_scale_factor_);
    LayerTreePixelTest::SetupTree();
  }

 protected:
  void RunPixelTestType(int content_size, float device_scale_factor) {
    int half_content = content_size / 2;

    scoped_refptr<SolidColorLayer> background = CreateSolidColorLayer(
        gfx::Rect(0, 0, content_size, content_size), SK_ColorGREEN);

    // Add a blue layer that completely covers the green layer.
    scoped_refptr<SolidColorLayer> foreground = CreateSolidColorLayer(
        gfx::Rect(0, 0, content_size, content_size), SK_ColorBLUE);
    background->AddChild(foreground);

    // Add an alpha threshold filter to the blue layer which will filter out
    // everything except the lower right corner.
    FilterOperations filters;
    FilterOperation::ShapeRects alpha_shape;
    alpha_shape.emplace_back(half_content, half_content, content_size,
                             content_size);
    filters.Append(
        FilterOperation::CreateAlphaThresholdFilter(alpha_shape, 1.f, 0.f));
    foreground->SetFilters(filters);

    device_scale_factor_ = device_scale_factor;
    RunPixelTest(
        GetPixelTestType(), background,
        base::FilePath(FILE_PATH_LITERAL("green_small_with_blue_corner.png")));
  }

  float device_scale_factor_;
};

// TODO(crbug.com/948128): Enable these tests for Skia.
INSTANTIATE_TEST_SUITE_P(
    ,
    LayerTreeHostFiltersScaledPixelTest,
    ::testing::Values(LayerTreePixelTest::PIXEL_TEST_GL,
                      LayerTreePixelTest::PIXEL_TEST_SOFTWARE));

TEST_P(LayerTreeHostFiltersScaledPixelTest, StandardDpi) {
  RunPixelTestType(100, 1.f);
}

TEST_P(LayerTreeHostFiltersScaledPixelTest, HiDpi) {
  RunPixelTestType(50, 2.f);
}

TEST_P(LayerTreeHostFiltersPixelTest, NullFilter) {
  layer_transforms_should_scale_layer_contents_ = false;

  scoped_refptr<SolidColorLayer> foreground =
      CreateSolidColorLayer(gfx::Rect(0, 0, 200, 200), SK_ColorGREEN);

  FilterOperations filters;
  filters.Append(FilterOperation::CreateReferenceFilter(nullptr));
  foreground->SetFilters(filters);

  RunPixelTest(GetPixelTestType(), foreground,
               base::FilePath(FILE_PATH_LITERAL("green.png")));
}

TEST_P(LayerTreeHostFiltersPixelTest, CroppedFilter) {
  layer_transforms_should_scale_layer_contents_ = false;

  scoped_refptr<SolidColorLayer> foreground =
      CreateSolidColorLayer(gfx::Rect(0, 0, 200, 200), SK_ColorGREEN);

  // Check that a filter with a zero-height crop rect crops out its
  // result completely.
  FilterOperations filters;
  SkImageFilter::CropRect cropRect(SkRect::MakeXYWH(0, 0, 100, 0));
  sk_sp<PaintFilter> offset(
      sk_make_sp<OffsetPaintFilter>(0, 0, nullptr, &cropRect));
  filters.Append(FilterOperation::CreateReferenceFilter(offset));
  foreground->SetFilters(filters);

  RunPixelTest(GetPixelTestType(), foreground,
               base::FilePath(FILE_PATH_LITERAL("white.png")));
}

TEST_P(LayerTreeHostFiltersPixelTest, ImageFilterClipped) {
  scoped_refptr<SolidColorLayer> background =
      CreateSolidColorLayer(gfx::Rect(200, 200), SK_ColorYELLOW);

  scoped_refptr<SolidColorLayer> foreground =
      CreateSolidColorLayer(gfx::Rect(200, 200), SK_ColorRED);
  background->AddChild(foreground);

  float matrix[20];
  memset(matrix, 0, 20 * sizeof(matrix[0]));
  // This filter does a red-blue swap, so the foreground becomes blue.
  matrix[2] = matrix[6] = matrix[10] = matrix[18] = 1.0f;
  // We filter only the bottom 200x100 pixels of the foreground.
  SkImageFilter::CropRect crop_rect(SkRect::MakeXYWH(0, 100, 200, 100));
  FilterOperations filters;
  filters.Append(
      FilterOperation::CreateReferenceFilter(sk_make_sp<ColorFilterPaintFilter>(
          SkColorFilters::Matrix(matrix), nullptr, &crop_rect)));

  // Make the foreground layer's render surface be clipped by the background
  // layer.
  background->SetMasksToBounds(true);
  foreground->SetFilters(filters);

  // Then we translate the foreground up by 100 pixels in Y, so the cropped
  // region is moved to to the top. This ensures that the crop rect is being
  // correctly transformed in skia by the amount of clipping that the
  // compositor performs.
  gfx::Transform transform;
  transform.Translate(0.0, -100.0);
  foreground->SetTransform(transform);

  RunPixelTest(GetPixelTestType(), background,
               base::FilePath(FILE_PATH_LITERAL("blue_yellow.png")));
}

TEST_P(LayerTreeHostFiltersPixelTest, ImageFilterNonZeroOrigin) {
  scoped_refptr<SolidColorLayer> background =
      CreateSolidColorLayer(gfx::Rect(200, 200), SK_ColorYELLOW);

  scoped_refptr<SolidColorLayer> foreground =
      CreateSolidColorLayer(gfx::Rect(200, 200), SK_ColorRED);
  background->AddChild(foreground);

  float matrix[20];
  memset(matrix, 0, 20 * sizeof(matrix[0]));
  // This filter does a red-blue swap, so the foreground becomes blue.
  matrix[2] = matrix[6] = matrix[10] = matrix[18] = 1.0f;
  // Set up a crop rec to filter the bottom 200x100 pixels of the foreground.
  SkImageFilter::CropRect crop_rect(SkRect::MakeXYWH(0, 100, 200, 100));
  FilterOperations filters;
  filters.Append(
      FilterOperation::CreateReferenceFilter(sk_make_sp<ColorFilterPaintFilter>(
          SkColorFilters::Matrix(matrix), nullptr, &crop_rect)));

  // Make the foreground layer's render surface be clipped by the background
  // layer.
  background->SetMasksToBounds(true);
  foreground->SetFilters(filters);

  // Now move the filters origin up by 100 pixels, so the crop rect is
  // applied only to the top 100 pixels, not the bottom.
  foreground->SetFiltersOrigin(gfx::PointF(0.0f, -100.0f));

  RunPixelTest(GetPixelTestType(), background,
               base::FilePath(FILE_PATH_LITERAL("blue_yellow.png")));
}

TEST_P(LayerTreeHostFiltersPixelTestNonSkia, ImageFilterScaled) {
  scoped_refptr<SolidColorLayer> background =
      CreateSolidColorLayer(gfx::Rect(200, 200), SK_ColorWHITE);

  gfx::Rect rect(50, 50, 100, 100);

  const int kInset = 3;
  for (int i = 0; !rect.IsEmpty(); ++i) {
    scoped_refptr<SolidColorLayer> layer =
        CreateSolidColorLayer(rect, (i & 1) ? SK_ColorWHITE : SK_ColorRED);

    gfx::Transform transform;
    transform.Translate(rect.width() / 2.0, rect.height() / 2.0);
    transform.RotateAboutZAxis(30.0);
    transform.Translate(-rect.width() / 2.0, -rect.height() / 2.0);
    layer->SetTransform(transform);

    background->AddChild(layer);

    rect.Inset(kInset, kInset);
  }

  scoped_refptr<SolidColorLayer> filter =
      CreateSolidColorLayer(gfx::Rect(100, 0, 100, 200), SK_ColorTRANSPARENT);

  background->AddChild(filter);

  // Apply a scale to |background| so that we can see any scaling artifacts
  // that may appear.
  gfx::Transform background_transform;
  static float scale = 1.1f;
  background_transform.Scale(scale, scale);
  background->SetTransform(background_transform);

  FilterOperations filters;
  filters.Append(FilterOperation::CreateGrayscaleFilter(1.0f));
  filter->SetBackdropFilters(filters);
  filter->SetBackdropFilterBounds(gfx::RRectF());

#if defined(OS_WIN) || defined(_MIPS_ARCH_LOONGSON) || defined(ARCH_CPU_ARM64)
#if defined(OS_WIN)
  // Windows has 153 pixels off by at most 2: crbug.com/225027
  float percentage_pixels_large_error = 0.3825f;  // 153px / (200*200)
  int large_error_allowed = 2;
#elif defined(_MIPS_ARCH_LOONGSON)
  // Loongson has 2 pixels off by at most 2: crbug.com/819075
  float percentage_pixels_large_error = 0.005f;  // 2px / (200*200)
  int large_error_allowed = 2;
#else
  float percentage_pixels_large_error = 0.0325f;  // 13px / (200*200)
  int large_error_allowed = 1;
#endif
  float percentage_pixels_small_error = 0.0f;
  float average_error_allowed_in_bad_pixels = 1.f;
  int small_error_allowed = 0;
  pixel_comparator_.reset(new FuzzyPixelComparator(
      true,  // discard_alpha
      percentage_pixels_large_error, percentage_pixels_small_error,
      average_error_allowed_in_bad_pixels, large_error_allowed,
      small_error_allowed));
#endif

  RunPixelTest(
      GetPixelTestType(), background,
      base::FilePath(FILE_PATH_LITERAL("backdrop_filter_on_scaled_layer_.png"))
          .InsertBeforeExtensionASCII(GetRendererSuffix()));
}

TEST_P(LayerTreeHostFiltersPixelTestNonSkia, BackdropFilterRotated) {
  // Add a white background with a rotated red rect in the center.
  scoped_refptr<SolidColorLayer> background =
      CreateSolidColorLayer(gfx::Rect(200, 200), SK_ColorWHITE);

  gfx::Rect rect(50, 50, 100, 100);

  scoped_refptr<SolidColorLayer> layer =
      CreateSolidColorLayer(rect, SK_ColorRED);

  gfx::Transform transform;
  transform.Translate(rect.width() / 2.0, rect.height() / 2.0);
  transform.RotateAboutZAxis(30.0);
  transform.Translate(-rect.width() / 2.0, -rect.height() / 2.0);
  layer->SetTransform(transform);

  background->AddChild(layer);

  // Add a slightly transparent blue layer.
  SkColor transparent_blue = SkColorSetARGB(0x23, 0x00, 0x00, 0xFF);
  scoped_refptr<SolidColorLayer> filter_layer =
      CreateSolidColorLayer(gfx::Rect(100, 0, 100, 200), transparent_blue);

  // Add some rotation so that we can see that it blurs only under the layer.
  gfx::Transform transform_filter;
  transform_filter.RotateAboutZAxis(10.0);
  filter_layer->SetTransform(transform_filter);

  // Add a blur filter to the blue layer.
  FilterOperations filters;
  filters.Append(FilterOperation::CreateBlurFilter(
      5.0f, SkBlurImageFilter::kClamp_TileMode));
  filter_layer->SetBackdropFilters(filters);
  // TODO(916311): Adding filter bounds here should work, but it clips
  // the corner of the red box.
  // gfx::RectF backdrop_filter_bounds(gfx::SizeF(filter_layer->bounds()));
  gfx::RRectF backdrop_filter_bounds;
  filter_layer->SetBackdropFilterBounds(backdrop_filter_bounds);

  background->AddChild(filter_layer);

  // Allow some fuzziness so that this doesn't fail when Skia makes minor
  // changes to blur or rectangle rendering.
  float percentage_pixels_large_error = 4.f;
  float percentage_pixels_small_error = 0.0f;
  float average_error_allowed_in_bad_pixels = 2.f;
  int large_error_allowed = 2;
  int small_error_allowed = 0;
  pixel_comparator_.reset(new FuzzyPixelComparator(
      true,  // discard_alpha
      percentage_pixels_large_error, percentage_pixels_small_error,
      average_error_allowed_in_bad_pixels, large_error_allowed,
      small_error_allowed));

  RunPixelTest(GetPixelTestType(), background,
               base::FilePath(FILE_PATH_LITERAL("backdrop_filter_rotated_.png"))
                   .InsertBeforeExtensionASCII(GetRendererSuffix()));
}

TEST_P(LayerTreeHostFiltersPixelTest, ImageRenderSurfaceScaled) {
  // A filter will cause a render surface to be used.  Here we force the
  // render surface on, and scale the result to make sure that we rasterize at
  // the correct resolution.
  scoped_refptr<SolidColorLayer> background =
      CreateSolidColorLayer(gfx::Rect(300, 300), SK_ColorBLUE);

  scoped_refptr<SolidColorLayer> render_surface_layer =
      CreateSolidColorLayer(gfx::Rect(0, 0, 200, 200), SK_ColorWHITE);

  gfx::Rect rect(50, 50, 100, 100);

  scoped_refptr<SolidColorLayer> child =
      CreateSolidColorLayer(rect, SK_ColorRED);

  gfx::Transform transform;
  transform.Translate(rect.width() / 2.0, rect.height() / 2.0);
  transform.RotateAboutZAxis(30.0);
  transform.Translate(-rect.width() / 2.0, -rect.height() / 2.0);
  child->SetTransform(transform);

  render_surface_layer->AddChild(child);

  gfx::Transform render_surface_transform;
  render_surface_transform.Scale(1.5f, 1.5f);
  render_surface_layer->SetTransform(render_surface_transform);
  render_surface_layer->SetForceRenderSurfaceForTesting(true);

  background->AddChild(render_surface_layer);

  // Software has some huge differences in the AA'd pixels on the different
  // trybots. See crbug.com/452198.
  if (GetPixelTestType() == LayerTreePixelTest::PIXEL_TEST_SOFTWARE) {
    float percentage_pixels_large_error = 0.686f;
    float percentage_pixels_small_error = 0.0f;
    float average_error_allowed_in_bad_pixels = 16.f;
    int large_error_allowed = 17;
    int small_error_allowed = 0;
    pixel_comparator_.reset(new FuzzyPixelComparator(
        true,  // discard_alpha
        percentage_pixels_large_error, percentage_pixels_small_error,
        average_error_allowed_in_bad_pixels, large_error_allowed,
        small_error_allowed));
  }

  RunPixelTest(
      GetPixelTestType(), background,
      base::FilePath(FILE_PATH_LITERAL("scaled_render_surface_layer_.png"))
          .InsertBeforeExtensionASCII(GetRendererSuffix()));
}

TEST_P(LayerTreeHostFiltersPixelTestNonSkia, ZoomFilter) {
  scoped_refptr<SolidColorLayer> root =
      CreateSolidColorLayer(gfx::Rect(300, 300), SK_ColorWHITE);

  // Create the pattern. Two blue/yellow side by side blocks with a horizontal
  // green strip.
  scoped_refptr<SolidColorLayer> left =
      CreateSolidColorLayer(gfx::Rect(0, 0, 100, 150), SK_ColorBLUE);
  root->AddChild(left);
  scoped_refptr<SolidColorLayer> right =
      CreateSolidColorLayer(gfx::Rect(100, 0, 200, 150), SK_ColorYELLOW);
  root->AddChild(right);
  scoped_refptr<SolidColorLayer> horizontal_strip =
      CreateSolidColorLayer(gfx::Rect(0, 10, 300, 10), SK_ColorGREEN);
  root->AddChild(horizontal_strip);

  // Test a zoom that extends past the edge of the screen.
  scoped_refptr<SolidColorLayer> border_edge_zoom =
      CreateSolidColorLayer(gfx::Rect(-10, -10, 50, 50), SK_ColorTRANSPARENT);
  FilterOperations border_filters;
  border_filters.Append(
      FilterOperation::CreateZoomFilter(2.f /* zoom */, 0 /* inset */));
  border_edge_zoom->SetBackdropFilters(border_filters);
  gfx::RectF border_filter_bounds(gfx::SizeF(border_edge_zoom->bounds()));
  border_edge_zoom->SetBackdropFilterBounds(
      gfx::RRectF(border_filter_bounds, 0));
  root->AddChild(border_edge_zoom);

  // Test a zoom that extends past the edge of the screen.
  scoped_refptr<SolidColorLayer> top_edge_zoom =
      CreateSolidColorLayer(gfx::Rect(70, -10, 50, 50), SK_ColorTRANSPARENT);
  FilterOperations top_filters;
  top_filters.Append(
      FilterOperation::CreateZoomFilter(2.f /* zoom */, 0 /* inset */));
  top_edge_zoom->SetBackdropFilters(top_filters);
  gfx::RectF top_filter_bounds(gfx::SizeF(top_edge_zoom->bounds()));
  top_edge_zoom->SetBackdropFilterBounds(gfx::RRectF(top_filter_bounds, 0));
  root->AddChild(top_edge_zoom);

  // Test a zoom that is fully within the screen.
  scoped_refptr<SolidColorLayer> contained_zoom =
      CreateSolidColorLayer(gfx::Rect(150, 5, 50, 50), SK_ColorTRANSPARENT);
  FilterOperations mid_filters;
  mid_filters.Append(
      FilterOperation::CreateZoomFilter(2.f /* zoom */, 0 /* inset */));
  contained_zoom->SetBackdropFilters(mid_filters);
  gfx::RectF mid_filter_bounds(gfx::SizeF(contained_zoom->bounds()));
  contained_zoom->SetBackdropFilterBounds(gfx::RRectF(mid_filter_bounds, 0));
  root->AddChild(contained_zoom);

#if defined(OS_WIN)
  // Windows has 1 pixel off by 1: crbug.com/259915
  float percentage_pixels_large_error = 0.00111112f;  // 1px / (300*300)
  float percentage_pixels_small_error = 0.0f;
  float average_error_allowed_in_bad_pixels = 1.f;
  int large_error_allowed = 1;
  int small_error_allowed = 0;
  pixel_comparator_.reset(new FuzzyPixelComparator(
      true,  // discard_alpha
      percentage_pixels_large_error, percentage_pixels_small_error,
      average_error_allowed_in_bad_pixels, large_error_allowed,
      small_error_allowed));
#endif

  RunPixelTest(GetPixelTestType(), std::move(root),
               base::FilePath(FILE_PATH_LITERAL("zoom_filter_.png"))
                   .InsertBeforeExtensionASCII(GetRendererSuffix()));
}

TEST_P(LayerTreeHostFiltersPixelTest, RotatedFilter) {
  scoped_refptr<SolidColorLayer> background =
      CreateSolidColorLayer(gfx::Rect(300, 300), SK_ColorWHITE);

  gfx::Rect rect(50, 50, 100, 100);

  scoped_refptr<SolidColorLayer> child =
      CreateSolidColorLayer(rect, SK_ColorBLUE);

  gfx::Transform transform;
  transform.Translate(50.0f, 50.0f);
  transform.RotateAboutZAxis(1.0);
  transform.Translate(-50.0f, -50.0f);
  child->SetTransform(transform);
  FilterOperations filters;
  // We need a clamping brightness filter here to force the Skia filter path.
  filters.Append(FilterOperation::CreateBrightnessFilter(1.1f));
  filters.Append(FilterOperation::CreateGrayscaleFilter(1.0f));
  child->SetFilters(filters);

  background->AddChild(child);

#if defined(OS_WIN)
  // Windows has 1 pixel off by 1: crbug.com/259915
  float percentage_pixels_large_error = 0.00111112f;  // 1px / (300*300)
  float percentage_pixels_small_error = 0.0f;
  float average_error_allowed_in_bad_pixels = 1.f;
  int large_error_allowed = 1;
  int small_error_allowed = 0;
  pixel_comparator_.reset(new FuzzyPixelComparator(
      true,  // discard_alpha
      percentage_pixels_large_error, percentage_pixels_small_error,
      average_error_allowed_in_bad_pixels, large_error_allowed,
      small_error_allowed));
#endif

  RunPixelTest(GetPixelTestType(), background,
               base::FilePath(FILE_PATH_LITERAL("rotated_filter_.png"))
                   .InsertBeforeExtensionASCII(GetRendererSuffix()));
}

TEST_P(LayerTreeHostFiltersPixelTestNonSkia, RotatedDropShadowFilter) {
  scoped_refptr<SolidColorLayer> background =
      CreateSolidColorLayer(gfx::Rect(300, 300), SK_ColorWHITE);

  gfx::Rect rect(50, 50, 100, 100);

  // Use a border to defeat solid color detection to force a tile quad.
  // This is intended to test render pass removal optimizations.
  SolidColorContentLayerClient blue_client(SK_ColorBLUE, rect.size(), 1,
                                           SK_ColorBLACK);
  scoped_refptr<PictureLayer> child = PictureLayer::Create(&blue_client);
  child->SetBounds(rect.size());
  child->SetPosition(gfx::PointF(rect.origin()));
  child->SetIsDrawable(true);

  gfx::Transform transform;
  transform.Translate(50.0f, 50.0f);
  transform.RotateAboutZAxis(1.0);
  transform.Translate(-50.0f, -50.0f);
  child->SetTransform(transform);
  FilterOperations filters;
  filters.Append(FilterOperation::CreateDropShadowFilter(
      gfx::Point(10.0f, 10.0f), 0.0f, SK_ColorBLACK));
  child->SetFilters(filters);

  background->AddChild(child);

#if defined(OS_WIN) || defined(ARCH_CPU_ARM64)
  // Windows and ARM64 have 3 pixels off by 1: crbug.com/259915
  float percentage_pixels_large_error = 0.00333334f;  // 3px / (300*300)
  float percentage_pixels_small_error = 0.0f;
  float average_error_allowed_in_bad_pixels = 1.f;
  int large_error_allowed = 1;
  int small_error_allowed = 0;
  pixel_comparator_.reset(new FuzzyPixelComparator(
      true,  // discard_alpha
      percentage_pixels_large_error, percentage_pixels_small_error,
      average_error_allowed_in_bad_pixels, large_error_allowed,
      small_error_allowed));
#endif

  RunPixelTest(
      GetPixelTestType(), background,
      base::FilePath(FILE_PATH_LITERAL("rotated_drop_shadow_filter_.png"))
          .InsertBeforeExtensionASCII(GetRendererSuffix()));
}

TEST_P(LayerTreeHostFiltersPixelTestNonSkia, TranslatedFilter) {
  scoped_refptr<Layer> clip = Layer::Create();
  clip->SetBounds(gfx::Size(300, 300));
  clip->SetMasksToBounds(true);

  scoped_refptr<Layer> parent = Layer::Create();
  parent->SetPosition(gfx::PointF(30.f, 30.f));

  gfx::Rect child_rect(100, 100);
  // Use two colors to bypass solid color detection, so we get a tile quad.
  // This is intended to test render pass removal optimizations.
  FakeContentLayerClient client;
  client.set_bounds(child_rect.size());
  PaintFlags flags;
  flags.setColor(SK_ColorGREEN);
  client.add_draw_rect(child_rect, flags);
  flags.setColor(SK_ColorBLUE);
  client.add_draw_rect(gfx::Rect(100, 50), flags);
  scoped_refptr<PictureLayer> child = PictureLayer::Create(&client);
  child->SetBounds(child_rect.size());
  child->SetIsDrawable(true);
  FilterOperations filters;
  filters.Append(FilterOperation::CreateOpacityFilter(0.5f));
  child->SetFilters(filters);

  // This layer will be clipped by |clip|, so the RenderPass created for
  // |child| will only have one visible quad.
  scoped_refptr<SolidColorLayer> grand_child =
      CreateSolidColorLayer(gfx::Rect(-300, -300, 100, 100), SK_ColorRED);

  child->AddChild(grand_child);
  parent->AddChild(child);
  clip->AddChild(parent);

  RunPixelTest(
      GetPixelTestType(), clip,
      base::FilePath(FILE_PATH_LITERAL("translated_blue_green_alpha_.png"))
          .InsertBeforeExtensionASCII(GetRendererSuffix()));
}

TEST_P(LayerTreeHostFiltersPixelTest, EnlargedTextureWithAlphaThresholdFilter) {
  // Rectangles chosen so that if flipped, the test will fail.
  gfx::Rect rect1(10, 10, 10, 15);
  gfx::Rect rect2(20, 25, 70, 65);

  scoped_refptr<SolidColorLayer> child1 =
      CreateSolidColorLayer(rect1, SK_ColorRED);
  scoped_refptr<SolidColorLayer> child2 =
      CreateSolidColorLayer(rect2, SK_ColorGREEN);
  scoped_refptr<SolidColorLayer> background =
      CreateSolidColorLayer(gfx::Rect(200, 200), SK_ColorBLUE);
  scoped_refptr<SolidColorLayer> filter_layer =
      CreateSolidColorLayer(gfx::Rect(100, 100), SK_ColorWHITE);

  // Make sure a transformation does not cause misregistration of the filter
  // and source texture.
  gfx::Transform filter_transform;
  filter_transform.Scale(2.f, 2.f);
  filter_layer->SetTransform(filter_transform);
  filter_layer->AddChild(child1);
  filter_layer->AddChild(child2);

  rect1.Inset(-5, -5);
  rect2.Inset(-5, -5);
  FilterOperation::ShapeRects alpha_shape = {rect1, rect2};
  FilterOperations filters;
  filters.Append(
      FilterOperation::CreateAlphaThresholdFilter(alpha_shape, 0.f, 0.f));
  filter_layer->SetFilters(filters);

  background->AddChild(filter_layer);

  // Force the allocation a larger textures.
  set_enlarge_texture_amount(gfx::Size(50, 50));

  RunPixelTest(
      GetPixelTestType(), background,
      base::FilePath(FILE_PATH_LITERAL("enlarged_texture_on_threshold.png")));
}

TEST_P(LayerTreeHostFiltersPixelTest, EnlargedTextureWithCropOffsetFilter) {
  // Rectangles chosen so that if flipped, the test will fail.
  gfx::Rect rect1(10, 10, 10, 15);
  gfx::Rect rect2(20, 25, 70, 65);

  scoped_refptr<SolidColorLayer> child1 =
      CreateSolidColorLayer(rect1, SK_ColorRED);
  scoped_refptr<SolidColorLayer> child2 =
      CreateSolidColorLayer(rect2, SK_ColorGREEN);
  scoped_refptr<SolidColorLayer> background =
      CreateSolidColorLayer(gfx::Rect(200, 200), SK_ColorBLUE);
  scoped_refptr<SolidColorLayer> filter_layer =
      CreateSolidColorLayer(gfx::Rect(100, 100), SK_ColorWHITE);

  // Make sure a transformation does not cause misregistration of the filter
  // and source texture.
  gfx::Transform filter_transform;
  filter_transform.Scale(2.f, 2.f);
  filter_layer->SetTransform(filter_transform);
  filter_layer->AddChild(child1);
  filter_layer->AddChild(child2);

  FilterOperations filters;
  SkImageFilter::CropRect cropRect(SkRect::MakeXYWH(10, 10, 80, 80));
  filters.Append(FilterOperation::CreateReferenceFilter(
      sk_make_sp<OffsetPaintFilter>(0, 0, nullptr, &cropRect)));
  filter_layer->SetFilters(filters);

  background->AddChild(filter_layer);

  // Force the allocation a larger textures.
  set_enlarge_texture_amount(gfx::Size(50, 50));

  RunPixelTest(
      GetPixelTestType(), background,
      base::FilePath(FILE_PATH_LITERAL("enlarged_texture_on_crop_offset.png")));
}

// TODO(crbug.com/948128): Enable this test for SkiaRenderer.
TEST_P(LayerTreeHostFiltersPixelTestNonSkia, BlurFilterWithClip) {
  scoped_refptr<SolidColorLayer> child1 =
      CreateSolidColorLayer(gfx::Rect(200, 200), SK_ColorBLUE);
  scoped_refptr<SolidColorLayer> child2 =
      CreateSolidColorLayer(gfx::Rect(20, 20, 160, 160), SK_ColorWHITE);
  scoped_refptr<SolidColorLayer> child3 =
      CreateSolidColorLayer(gfx::Rect(40, 40, 20, 30), SK_ColorRED);
  scoped_refptr<SolidColorLayer> child4 =
      CreateSolidColorLayer(gfx::Rect(60, 70, 100, 90), SK_ColorGREEN);
  scoped_refptr<SolidColorLayer> filter_layer =
      CreateSolidColorLayer(gfx::Rect(200, 200), SK_ColorWHITE);

  filter_layer->AddChild(child1);
  filter_layer->AddChild(child2);
  filter_layer->AddChild(child3);
  filter_layer->AddChild(child4);

  FilterOperations filters;
  filters.Append(FilterOperation::CreateBlurFilter(
      2.f, SkBlurImageFilter::kClamp_TileMode));
  filter_layer->SetFilters(filters);

  // Force the allocation a larger textures.
  set_enlarge_texture_amount(gfx::Size(50, 50));

#if defined(OS_WIN) || defined(ARCH_CPU_ARM64)
#if defined(OS_WIN)
  // Windows has 1880 pixels off by 1: crbug.com/259915
  float percentage_pixels_large_error = 4.7f;  // 1880px / (200*200)
#else
  // Differences in floating point calculation on ARM means a small percentage
  // of pixels will have small differences.
  float percentage_pixels_large_error = 2.76f;  // 1104px / (200*200)
#endif
  float percentage_pixels_small_error = 0.0f;
  float average_error_allowed_in_bad_pixels = 1.f;
  int large_error_allowed = 2;
  int small_error_allowed = 0;
  pixel_comparator_.reset(new FuzzyPixelComparator(
      true,  // discard_alpha
      percentage_pixels_large_error, percentage_pixels_small_error,
      average_error_allowed_in_bad_pixels, large_error_allowed,
      small_error_allowed));
#endif

  RunPixelTest(GetPixelTestType(), filter_layer,
               base::FilePath(FILE_PATH_LITERAL("blur_filter_with_clip_.png"))
                   .InsertBeforeExtensionASCII(GetRendererSuffix()));
}

TEST_P(LayerTreeHostFiltersPixelTestGPU, FilterWithGiantCropRect) {
  scoped_refptr<SolidColorLayer> tree = BuildFilterWithGiantCropRect(true);
  RunPixelTest(
      GetPixelTestType(), tree,
      base::FilePath(FILE_PATH_LITERAL("filter_with_giant_crop_rect.png")));
}

TEST_P(LayerTreeHostFiltersPixelTestGPU, FilterWithGiantCropRectNoClip) {
  scoped_refptr<SolidColorLayer> tree = BuildFilterWithGiantCropRect(false);
  RunPixelTest(
      GetPixelTestType(), tree,
      base::FilePath(FILE_PATH_LITERAL("filter_with_giant_crop_rect.png")));
}

class BackdropFilterWithDeviceScaleFactorTest
    : public LayerTreeHostFiltersPixelTest {
 protected:
  void RunPixelTestType(float device_scale_factor,
                        const base::FilePath& expected_result) {
    device_scale_factor_ = device_scale_factor;

    scoped_refptr<Layer> root =
        CreateSolidColorLayer(gfx::Rect(200, 200), SK_ColorWHITE);

    scoped_refptr<SolidColorLayer> background =
        CreateSolidColorLayer(gfx::Rect(100, 120), SK_ColorBLACK);
    root->AddChild(background);

    scoped_refptr<SolidColorLayer> filtered = CreateSolidColorLayer(
        gfx::Rect(0, 100, 200, 100), SkColorSetA(SK_ColorGREEN, 127));
    FilterOperations filters;
    filters.Append(FilterOperation::CreateReferenceFilter(
        sk_make_sp<OffsetPaintFilter>(0, 80, nullptr)));
    filtered->SetBackdropFilters(filters);
    filtered->SetBackdropFilterBounds(gfx::RRectF());
    root->AddChild(filtered);

    // This should appear as a grid of 4 100x100 squares which are:
    // BLACK       WHITE
    // DARK GREEN  LIGHT GREEN
    RunPixelTest(GetPixelTestType(), std::move(root), expected_result);
  }

 private:
  // LayerTreePixelTest overrides
  void SetupTree() override {
    SetInitialDeviceScaleFactor(device_scale_factor_);
    LayerTreeHostFiltersPixelTest::SetupTree();
  }

  float device_scale_factor_ = 1;
};

// TODO(crbug.com/948128): Enable these tests for Skia.
INSTANTIATE_TEST_SUITE_P(
    ,
    BackdropFilterWithDeviceScaleFactorTest,
    ::testing::Values(LayerTreePixelTest::PIXEL_TEST_GL,
                      LayerTreePixelTest::PIXEL_TEST_SOFTWARE));

TEST_P(BackdropFilterWithDeviceScaleFactorTest, StandardDpi) {
  RunPixelTestType(
      1.f, base::FilePath(FILE_PATH_LITERAL("offset_backdrop_filter_1x.png")));
}

TEST_P(BackdropFilterWithDeviceScaleFactorTest, HiDpi) {
  RunPixelTestType(
      2.f, base::FilePath(FILE_PATH_LITERAL("offset_backdrop_filter_2x.png")));
}

}  // namespace
}  // namespace cc

#endif  // OS_ANDROID
