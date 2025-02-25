// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_SERVICE_WORKER_SERVICE_WORKER_PROVIDER_MOJOM_SHARED_MESSAGE_IDS_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_SERVICE_WORKER_SERVICE_WORKER_PROVIDER_MOJOM_SHARED_MESSAGE_IDS_H_

#include <stdint.h>
namespace blink {
namespace mojom {

namespace internal {


// The 419363593 value is based on sha256(salt + "ServiceWorkerWorkerClient1").
constexpr uint32_t kServiceWorkerWorkerClient_OnControllerChanged_Name = 419363593;
// The 1609450158 value is based on sha256(salt + "ServiceWorkerWorkerClientRegistry1").
constexpr uint32_t kServiceWorkerWorkerClientRegistry_RegisterWorkerClient_Name = 1609450158;
// The 1644897369 value is based on sha256(salt + "ServiceWorkerWorkerClientRegistry2").
constexpr uint32_t kServiceWorkerWorkerClientRegistry_CloneWorkerClientRegistry_Name = 1644897369;

}  // namespace internal
}  // namespace mojom
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_SERVICE_WORKER_SERVICE_WORKER_PROVIDER_MOJOM_SHARED_MESSAGE_IDS_H_