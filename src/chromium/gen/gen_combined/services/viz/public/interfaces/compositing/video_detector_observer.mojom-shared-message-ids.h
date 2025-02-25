// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_VIZ_PUBLIC_INTERFACES_COMPOSITING_VIDEO_DETECTOR_OBSERVER_MOJOM_SHARED_MESSAGE_IDS_H_
#define SERVICES_VIZ_PUBLIC_INTERFACES_COMPOSITING_VIDEO_DETECTOR_OBSERVER_MOJOM_SHARED_MESSAGE_IDS_H_

#include <stdint.h>
namespace viz {
namespace mojom {

namespace internal {


// The 2006734956 value is based on sha256(salt + "VideoDetectorObserver1").
constexpr uint32_t kVideoDetectorObserver_OnVideoActivityStarted_Name = 2006734956;
// The 529079871 value is based on sha256(salt + "VideoDetectorObserver2").
constexpr uint32_t kVideoDetectorObserver_OnVideoActivityEnded_Name = 529079871;

}  // namespace internal
}  // namespace mojom
}  // namespace viz

#endif  // SERVICES_VIZ_PUBLIC_INTERFACES_COMPOSITING_VIDEO_DETECTOR_OBSERVER_MOJOM_SHARED_MESSAGE_IDS_H_