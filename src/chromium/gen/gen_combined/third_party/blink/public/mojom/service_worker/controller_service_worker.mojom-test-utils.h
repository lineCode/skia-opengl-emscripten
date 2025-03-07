// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_SERVICE_WORKER_CONTROLLER_SERVICE_WORKER_MOJOM_TEST_UTILS_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_SERVICE_WORKER_CONTROLLER_SERVICE_WORKER_MOJOM_TEST_UTILS_H_

#include "third_party/blink/public/mojom/service_worker/controller_service_worker.mojom.h"
#include "third_party/blink/public/common/common_export.h"


namespace blink {
namespace mojom {


class BLINK_COMMON_EXPORT ControllerServiceWorkerInterceptorForTesting : public ControllerServiceWorker {
  virtual ControllerServiceWorker* GetForwardingInterface() = 0;
  void DispatchFetchEvent(::blink::mojom::DispatchFetchEventParamsPtr params, ::blink::mojom::ServiceWorkerFetchResponseCallbackPtr response_callback, DispatchFetchEventCallback callback) override;
  void Clone(ControllerServiceWorkerRequest controller) override;
};
class BLINK_COMMON_EXPORT ControllerServiceWorkerAsyncWaiter {
 public:
  explicit ControllerServiceWorkerAsyncWaiter(ControllerServiceWorker* proxy);
  ~ControllerServiceWorkerAsyncWaiter();
  void DispatchFetchEvent(
      ::blink::mojom::DispatchFetchEventParamsPtr params, ::blink::mojom::ServiceWorkerFetchResponseCallbackPtr response_callback, ::blink::mojom::ServiceWorkerEventStatus* out_status);

 private:
  ControllerServiceWorker* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(ControllerServiceWorkerAsyncWaiter);
};


class BLINK_COMMON_EXPORT ControllerServiceWorkerConnectorInterceptorForTesting : public ControllerServiceWorkerConnector {
  virtual ControllerServiceWorkerConnector* GetForwardingInterface() = 0;
  void UpdateController(ControllerServiceWorkerPtr controller) override;
};
class BLINK_COMMON_EXPORT ControllerServiceWorkerConnectorAsyncWaiter {
 public:
  explicit ControllerServiceWorkerConnectorAsyncWaiter(ControllerServiceWorkerConnector* proxy);
  ~ControllerServiceWorkerConnectorAsyncWaiter();

 private:
  ControllerServiceWorkerConnector* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(ControllerServiceWorkerConnectorAsyncWaiter);
};




}  // namespace mojom
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_SERVICE_WORKER_CONTROLLER_SERVICE_WORKER_MOJOM_TEST_UTILS_H_