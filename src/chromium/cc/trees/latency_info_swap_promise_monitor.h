// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TREES_LATENCY_INFO_SWAP_PROMISE_MONITOR_H_
#define CC_TREES_LATENCY_INFO_SWAP_PROMISE_MONITOR_H_

#include "base/compiler_specific.h"
#include "cc/trees/swap_promise_monitor.h"

#if defined(ENABLE_LATENCY)
namespace ui {
class LatencyInfo;
}  // namespace ui
#endif // ENABLE_LATENCY

namespace cc {

#if defined(ENABLE_LATENCY)
// A LatencyInfoSwapPromiseMonitor queues a LatencyInfoSwapPromise into
// LayerTreeHost or LayerTreeHostImpl if there is compositor state change
// while it is being mointored.
class CC_EXPORT LatencyInfoSwapPromiseMonitor : public SwapPromiseMonitor {
 public:
  LatencyInfoSwapPromiseMonitor(ui::LatencyInfo* latency,
                                SwapPromiseManager* swap_promise_manager,
                                LayerTreeHostImpl* host_impl);
  ~LatencyInfoSwapPromiseMonitor() override;

  void OnSetNeedsCommitOnMain() override;
  void OnSetNeedsRedrawOnImpl() override;
  void OnForwardScrollUpdateToMainThreadOnImpl() override;

 private:
  ui::LatencyInfo* latency_;
};
#endif // ENABLE_LATENCY

}  // namespace cc

#endif  // CC_TREES_LATENCY_INFO_SWAP_PROMISE_MONITOR_H_
