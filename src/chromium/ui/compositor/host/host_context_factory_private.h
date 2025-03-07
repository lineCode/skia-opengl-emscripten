// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_HOST_HOST_CONTEXT_FACTORY_PRIVATE_H_
#define UI_COMPOSITOR_HOST_HOST_CONTEXT_FACTORY_PRIVATE_H_

#include <stdint.h>

#include "base/containers/flat_map.h"
#include "base/containers/flat_set.h"
#include "base/macros.h"
#include "base/memory/scoped_refptr.h"
#include "components/viz/common/display/renderer_settings.h"
#include "components/viz/common/surfaces/frame_sink_id_allocator.h"
#if !defined(COMPOSITOR_PORTED)
#include "services/viz/privileged/interfaces/compositing/display_private.mojom.h"
#endif // COMPOSITOR_PORTED
#include "ui/compositor/compositor.h"

namespace base {
class SingleThreadTaskRunner;
}

namespace viz {
class ContextProvider;
class HostDisplayClient;
class RasterContextProvider;
}  // namespace viz

#if !defined(COMPOSITOR_PORTED)
namespace ui {

class ExternalBeginFrameControllerClientImpl;

class HostContextFactoryPrivate : public ContextFactoryPrivate {
 public:
  HostContextFactoryPrivate(
      uint32_t client_id,
      viz::HostFrameSinkManager* host_frame_sink_manager,
      scoped_refptr<base::SingleThreadTaskRunner> resize_task_runner);
  ~HostContextFactoryPrivate() override;

  // Call this when a compositor is created to ensure a data map entry exists
  // for it, so that the data can be accessed before the compositor is
  // configured. Could be called twice, e.g. if the GPU process crashes.
  void AddCompositor(Compositor* compositor);

  void ConfigureCompositor(
      Compositor* compositor,
      scoped_refptr<viz::ContextProvider> context_provider,
      scoped_refptr<viz::RasterContextProvider> worker_context_provider);

  void UnconfigureCompositor(Compositor* compositor);

  void set_is_gpu_compositing_disabled(bool value) {
    is_gpu_compositing_disabled_ = value;
  }
  bool is_gpu_compositing_disabled() const {
    return is_gpu_compositing_disabled_;
  }

  scoped_refptr<base::SingleThreadTaskRunner> resize_task_runner() {
    return resize_task_runner_;
  }

  base::flat_set<Compositor*> GetAllCompositors();

  // ContextFactoryPrivate implementation.
  std::unique_ptr<Reflector> CreateReflector(Compositor* source,
                                             Layer* target) override;
  void RemoveReflector(Reflector* reflector) override;
  viz::FrameSinkId AllocateFrameSinkId() override;
  viz::HostFrameSinkManager* GetHostFrameSinkManager() override;
  void SetDisplayVisible(Compositor* compositor, bool visible) override;
  void ResizeDisplay(Compositor* compositor, const gfx::Size& size) override;
  void DisableSwapUntilResize(Compositor* compositor) override;
  void SetDisplayColorMatrix(Compositor* compositor,
                             const SkMatrix44& matrix) override;
  void SetDisplayColorSpace(Compositor* compositor,
                            const gfx::ColorSpace& blending_color_space,
                            const gfx::ColorSpace& output_color_space) override;
  void SetDisplayVSyncParameters(Compositor* compositor,
                                 base::TimeTicks timebase,
                                 base::TimeDelta interval) override;
  void IssueExternalBeginFrame(Compositor* compositor,
                               const viz::BeginFrameArgs& args) override;
  void SetOutputIsSecure(Compositor* compositor, bool secure) override;
  viz::FrameSinkManagerImpl* GetFrameSinkManager() override;
#if !defined(COMPOSITOR_PORTED)
  void AddVSyncParameterObserver(
      Compositor* compositor,
      viz::mojom::VSyncParameterObserverPtr observer) override;
#endif // COMPOSITOR_PORTED

 private:
  struct CompositorData {
    CompositorData();
    CompositorData(CompositorData&& other);
    ~CompositorData();
    CompositorData& operator=(CompositorData&& other);

    // Privileged interface that controls the display for a root
    // CompositorFrameSink.
#if !defined(COMPOSITOR_PORTED)
    viz::mojom::DisplayPrivateAssociatedPtr display_private;
#endif // COMPOSITOR_PORTED
    std::unique_ptr<viz::HostDisplayClient> display_client;

    // Controls external BeginFrames for the display. Only set if external
    // BeginFrames are enabled for the compositor.
    std::unique_ptr<ExternalBeginFrameControllerClientImpl>
        external_begin_frame_controller_client;

    // SetOutputIsSecure is called before the compositor is ready, so remember
    // the status and apply it during configuration.
    bool output_is_secure = false;

   private:
    DISALLOW_COPY_AND_ASSIGN(CompositorData);
  };

  base::flat_map<Compositor*, CompositorData> compositor_data_map_;

  viz::FrameSinkIdAllocator frame_sink_id_allocator_;
  viz::HostFrameSinkManager* host_frame_sink_manager_;

  bool is_gpu_compositing_disabled_ = false;

  scoped_refptr<base::SingleThreadTaskRunner> const resize_task_runner_;

  DISALLOW_COPY_AND_ASSIGN(HostContextFactoryPrivate);
};

}  // namespace ui
#endif // COMPOSITOR_PORTED

#endif  // UI_COMPOSITOR_HOST_HOST_CONTEXT_FACTORY_PRIVATE_H_
