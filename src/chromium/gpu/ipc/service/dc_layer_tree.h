// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_IPC_SERVICE_DC_LAYER_TREE_H_
#define GPU_IPC_SERVICE_DC_LAYER_TREE_H_

#include <windows.h>
#include <d3d11.h>
#include <dcomp.h>
#include <wrl/client.h>

#include <memory>

#include "gpu/config/gpu_driver_bug_workarounds.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gl/dc_renderer_layer_params.h"

namespace gpu {

class DirectCompositionChildSurfaceWin;
class SwapChainPresenter;

// DCLayerTree manages a tree of direct composition visuals, and associated
// swap chains for given overlay layers.  It maintains a list of pending layers
// submitted using ScheduleDCLayer() that are presented and committed in
// CommitAndClearPendingOverlays().
class DCLayerTree {
 public:
  explicit DCLayerTree(const GpuDriverBugWorkarounds& workarounds);
  ~DCLayerTree();

  // Returns true on success.
  bool Initialize(HWND window,
                  Microsoft::WRL::ComPtr<ID3D11Device> d3d11_device,
                  Microsoft::WRL::ComPtr<IDCompositionDevice2> dcomp_device);

  // Present pending overlay layers, and perform a direct composition commit if
  // necessary.  Returns true if presentation and commit succeeded.
  bool CommitAndClearPendingOverlays(
      DirectCompositionChildSurfaceWin* root_surface);

  // Schedule an overlay layer for the next CommitAndClearPendingOverlays call.
  bool ScheduleDCLayer(const ui::DCRendererLayerParams& params);

  // Called by SwapChainPresenter to initialize video processor that can handle
  // at least given input and output size.  The video processor is shared across
  // layers so the same one can be reused if it's large enough.  Returns true on
  // success.
  bool InitializeVideoProcessor(const gfx::Size& input_size,
                                const gfx::Size& output_size);

  void SetNeedsCommit() { needs_commit_ = true; }

  const Microsoft::WRL::ComPtr<ID3D11VideoDevice>& video_device() const {
    return video_device_;
  }

  const Microsoft::WRL::ComPtr<ID3D11VideoContext>& video_context() const {
    return video_context_;
  }

  const Microsoft::WRL::ComPtr<ID3D11VideoProcessor>& video_processor() const {
    return video_processor_;
  }

  const Microsoft::WRL::ComPtr<ID3D11VideoProcessorEnumerator>&
  video_processor_enumerator() const {
    return video_processor_enumerator_;
  }

  Microsoft::WRL::ComPtr<IDXGISwapChain1> GetLayerSwapChainForTesting(
      size_t index) const;

  const GpuDriverBugWorkarounds& workarounds() const { return workarounds_; }

 private:
  const GpuDriverBugWorkarounds workarounds_;

  Microsoft::WRL::ComPtr<ID3D11Device> d3d11_device_;
  Microsoft::WRL::ComPtr<IDCompositionDevice2> dcomp_device_;
  Microsoft::WRL::ComPtr<IDCompositionTarget> dcomp_target_;

  // The video processor is cached so SwapChains don't have to recreate it
  // whenever they're created.
  Microsoft::WRL::ComPtr<ID3D11VideoDevice> video_device_;
  Microsoft::WRL::ComPtr<ID3D11VideoContext> video_context_;
  Microsoft::WRL::ComPtr<ID3D11VideoProcessor> video_processor_;
  Microsoft::WRL::ComPtr<ID3D11VideoProcessorEnumerator>
      video_processor_enumerator_;

  // Current video processor input and output size.
  gfx::Size video_input_size_;
  gfx::Size video_output_size_;

  // Set to true if a direct composition commit is needed.
  bool needs_commit_ = false;

  // Set if root surface is using a swap chain currently.
  Microsoft::WRL::ComPtr<IDXGISwapChain1> root_swap_chain_;

  // Set if root surface is using a direct composition surface currently.
  Microsoft::WRL::ComPtr<IDCompositionSurface> root_dcomp_surface_;
  uint64_t root_dcomp_surface_serial_;

  // Direct composition visual for root surface.
  Microsoft::WRL::ComPtr<IDCompositionVisual2> root_surface_visual_;

  // Root direct composition visual for window dcomp target.
  Microsoft::WRL::ComPtr<IDCompositionVisual2> dcomp_root_visual_;

  // List of pending overlay layers from ScheduleDCLayer().
  std::vector<std::unique_ptr<ui::DCRendererLayerParams>> pending_overlays_;

  // List of swap chain presenters for previous frame.
  std::vector<std::unique_ptr<SwapChainPresenter>> video_swap_chains_;

  DISALLOW_COPY_AND_ASSIGN(DCLayerTree);
};

}  // namespace gpu

#endif  // GPU_IPC_SERVICE_DC_LAYER_TREE_H_
