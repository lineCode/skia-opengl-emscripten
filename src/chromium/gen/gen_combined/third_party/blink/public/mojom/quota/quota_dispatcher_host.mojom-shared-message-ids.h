// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_QUOTA_QUOTA_DISPATCHER_HOST_MOJOM_SHARED_MESSAGE_IDS_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_QUOTA_QUOTA_DISPATCHER_HOST_MOJOM_SHARED_MESSAGE_IDS_H_

#include <stdint.h>
namespace blink {
namespace mojom {

namespace internal {


// The 688079203 value is based on sha256(salt + "QuotaDispatcherHost1").
constexpr uint32_t kQuotaDispatcherHost_QueryStorageUsageAndQuota_Name = 688079203;
// The 872411632 value is based on sha256(salt + "QuotaDispatcherHost2").
constexpr uint32_t kQuotaDispatcherHost_RequestStorageQuota_Name = 872411632;

}  // namespace internal
}  // namespace mojom
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_QUOTA_QUOTA_DISPATCHER_HOST_MOJOM_SHARED_MESSAGE_IDS_H_