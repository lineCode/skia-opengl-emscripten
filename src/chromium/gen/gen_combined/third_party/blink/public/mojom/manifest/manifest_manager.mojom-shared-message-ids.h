// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_MANIFEST_MANIFEST_MANAGER_MOJOM_SHARED_MESSAGE_IDS_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_MANIFEST_MANIFEST_MANAGER_MOJOM_SHARED_MESSAGE_IDS_H_

#include <stdint.h>
namespace blink {
namespace mojom {

namespace internal {


// The 1496913509 value is based on sha256(salt + "ManifestManager1").
constexpr uint32_t kManifestManager_RequestManifest_Name = 1496913509;
// The 881560064 value is based on sha256(salt + "ManifestManager2").
constexpr uint32_t kManifestManager_RequestManifestDebugInfo_Name = 881560064;

}  // namespace internal
}  // namespace mojom
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_MANIFEST_MANIFEST_MANAGER_MOJOM_SHARED_MESSAGE_IDS_H_