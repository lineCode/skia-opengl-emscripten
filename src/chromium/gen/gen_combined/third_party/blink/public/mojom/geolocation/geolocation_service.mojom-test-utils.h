// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_GEOLOCATION_GEOLOCATION_SERVICE_MOJOM_TEST_UTILS_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_GEOLOCATION_GEOLOCATION_SERVICE_MOJOM_TEST_UTILS_H_

#include "third_party/blink/public/mojom/geolocation/geolocation_service.mojom.h"
#include "third_party/blink/public/common/common_export.h"


namespace blink {
namespace mojom {


class BLINK_COMMON_EXPORT GeolocationServiceInterceptorForTesting : public GeolocationService {
  virtual GeolocationService* GetForwardingInterface() = 0;
  void CreateGeolocation(::device::mojom::GeolocationRequest request, bool user_gesture, CreateGeolocationCallback callback) override;
};
class BLINK_COMMON_EXPORT GeolocationServiceAsyncWaiter {
 public:
  explicit GeolocationServiceAsyncWaiter(GeolocationService* proxy);
  ~GeolocationServiceAsyncWaiter();
  void CreateGeolocation(
      ::device::mojom::GeolocationRequest request, bool user_gesture, ::blink::mojom::PermissionStatus* out_status);

 private:
  GeolocationService* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(GeolocationServiceAsyncWaiter);
};




}  // namespace mojom
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_GEOLOCATION_GEOLOCATION_SERVICE_MOJOM_TEST_UTILS_H_