// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_SERVICE_MOJOM_BLINK_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_SERVICE_MOJOM_BLINK_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/sample_service.mojom-blink.h"


namespace sample {
namespace blink {


class  ServiceInterceptorForTesting : public Service {
  virtual Service* GetForwardingInterface() = 0;
  void Frobinate(FooPtr foo, Service::BazOptions baz, mojo::PendingRemote<Port> port, FrobinateCallback callback) override;
  void GetPort(mojo::PendingReceiver<Port> receiver) override;
};
class  ServiceAsyncWaiter {
 public:
  explicit ServiceAsyncWaiter(Service* proxy);
  ~ServiceAsyncWaiter();
  void Frobinate(
      FooPtr foo, Service::BazOptions baz, mojo::PendingRemote<Port> port, int32_t* out_result);

 private:
  Service* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(ServiceAsyncWaiter);
};


class  PortInterceptorForTesting : public Port {
  virtual Port* GetForwardingInterface() = 0;
  void PostMessageToPort(const WTF::String& message_text, mojo::PendingRemote<Port> port) override;
};
class  PortAsyncWaiter {
 public:
  explicit PortAsyncWaiter(Port* proxy);
  ~PortAsyncWaiter();

 private:
  Port* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(PortAsyncWaiter);
};




}  // namespace blink
}  // namespace sample

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_SERVICE_MOJOM_BLINK_TEST_UTILS_H_