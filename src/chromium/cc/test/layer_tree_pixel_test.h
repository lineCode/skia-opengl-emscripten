// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TEST_LAYER_TREE_PIXEL_TEST_H_
#define CC_TEST_LAYER_TREE_PIXEL_TEST_H_

#include <memory>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "cc/test/layer_tree_test.h"
#include "cc/trees/clip_node.h"
#include "cc/trees/effect_node.h"
#include "cc/trees/scroll_node.h"
#include "cc/trees/transform_node.h"
#include "components/viz/common/resources/single_release_callback.h"
#include "ui/gl/gl_implementation.h"

class SkBitmap;

namespace base {
namespace test {
class ScopedFeatureList;
}
}  // namespace base

namespace gfx {
class ColorSpace;
}

namespace viz {
class CopyOutputRequest;
class CopyOutputResult;
class TestGpuServiceHolder;
}

namespace cc {
class PixelComparator;
class SolidColorLayer;
class TextureLayer;

class LayerTreePixelTest : public LayerTreeTest {
 public:
  // TODO(sgilhuly): Once the pixel tests are working on skia gl, add the option
  // for skia vulkan.
  enum PixelTestType {
    PIXEL_TEST_GL,
    PIXEL_TEST_SKIA_GL,
    PIXEL_TEST_SOFTWARE,
  };

  static std::string TestTypeToString(PixelTestType test_type) {
    switch (test_type) {
      case PIXEL_TEST_GL:
        return "GL";
      case PIXEL_TEST_SKIA_GL:
        return "Skia GL";
      case PIXEL_TEST_SOFTWARE:
        return "Software";
    }
  }

 protected:
  LayerTreePixelTest();
  ~LayerTreePixelTest() override;

  // LayerTreeTest overrides.
  std::unique_ptr<viz::TestLayerTreeFrameSink> CreateLayerTreeFrameSink(
      const viz::RendererSettings& renderer_settings,
      double refresh_rate,
      scoped_refptr<viz::ContextProvider> compositor_context_provider,
      scoped_refptr<viz::RasterContextProvider> worker_context_provider)
      override;
  std::unique_ptr<viz::SkiaOutputSurface>
  CreateDisplaySkiaOutputSurfaceOnThread() override;
  std::unique_ptr<viz::OutputSurface> CreateDisplayOutputSurfaceOnThread(
      scoped_refptr<viz::ContextProvider> compositor_context_provider) override;

  virtual std::unique_ptr<viz::CopyOutputRequest> CreateCopyOutputRequest();

  void ReadbackResult(std::unique_ptr<viz::CopyOutputResult> result);

  void BeginTest() override;
  void SetupTree() override;
  void AfterTest() override;
  void EndTest() override;
  void InitializeSettings(LayerTreeSettings* settings) override;

  void TryEndTest();

  scoped_refptr<SolidColorLayer> CreateSolidColorLayer(const gfx::Rect& rect,
                                                       SkColor color);
  scoped_refptr<SolidColorLayer> CreateSolidColorLayerWithBorder(
      const gfx::Rect& rect,
      SkColor color,
      int border_width,
      SkColor border_color);

  // Initializes the root layer and root PropertyTrees for layer list mode.
  // In this mode, all other layers are direct children of |root_layer| and
  // any property nodes are descendants of node id 1 in the respective trees.
  void InitializeForLayerListMode(scoped_refptr<Layer>* root_layer,
                                  PropertyTrees* property_trees);

  void RunPixelTest(PixelTestType type,
                    scoped_refptr<Layer> content_root,
                    base::FilePath file_name);
  void RunPixelTest(PixelTestType type,
                    scoped_refptr<Layer> content_root,
                    const SkBitmap& expected_bitmap);

  void RunPixelTestWithLayerList(PixelTestType type,
                                 scoped_refptr<Layer> root_layer,
                                 base::FilePath file_name,
                                 PropertyTrees* property_trees);

  void RunSingleThreadedPixelTest(PixelTestType test_type,
                                  scoped_refptr<Layer> content_root,
                                  base::FilePath file_name);

  void RunPixelTestWithReadbackTarget(PixelTestType type,
                                      scoped_refptr<Layer> content_root,
                                      Layer* target,
                                      base::FilePath file_name);

  void SetPixelTestType(PixelTestType test_type) {
    test_type_ = test_type;
    use_skia_renderer_ = test_type == PIXEL_TEST_SKIA_GL;
  }

  SkBitmap CopyMailboxToBitmap(const gfx::Size& size,
                               const gpu::Mailbox& mailbox,
                               const gpu::SyncToken& sync_token,
                               const gfx::ColorSpace& color_space);

  void Finish();

  // Allow tests to enlarge the backing texture for a non-root render pass, to
  // simulate reusing a larger texture from a previous frame for a new
  // render pass. This should be called before the output surface is bound.
  void set_enlarge_texture_amount(const gfx::Size& enlarge_texture_amount) {
    enlarge_texture_amount_ = enlarge_texture_amount;
  }

  // Common CSS colors defined for tests to use.
  static const SkColor kCSSOrange = 0xffffa500;
  static const SkColor kCSSBrown = 0xffa52a2a;
  static const SkColor kCSSGreen = 0xff008000;
  static const SkColor kCSSBlack = 0xff000000;

  gl::DisableNullDrawGLBindings enable_pixel_output_;
  std::unique_ptr<PixelComparator> pixel_comparator_;
  PixelTestType test_type_;
  scoped_refptr<Layer> content_root_;
  PropertyTrees* property_trees_;
  Layer* readback_target_;
  base::FilePath ref_file_;
  SkBitmap expected_bitmap_;
  std::unique_ptr<SkBitmap> result_bitmap_;
  std::vector<scoped_refptr<TextureLayer>> texture_layers_;
  int pending_texture_mailbox_callbacks_;
  gfx::Size enlarge_texture_amount_;

  // Used to create SkiaOutputSurfaceImpl.
  std::unique_ptr<base::test::ScopedFeatureList> scoped_feature_list_;
  std::unique_ptr<viz::TestGpuServiceHolder> gpu_service_holder_;
};

}  // namespace cc

#endif  // CC_TEST_LAYER_TREE_PIXEL_TEST_H_
