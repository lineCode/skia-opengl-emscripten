// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_REGRESSION_TESTS_MOJOM_BLINK_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_REGRESSION_TESTS_MOJOM_BLINK_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/regression_tests.mojom-blink.h"


namespace regression_tests {
namespace blink {


class  CheckMethodWithEmptyResponseInterceptorForTesting : public CheckMethodWithEmptyResponse {
  virtual CheckMethodWithEmptyResponse* GetForwardingInterface() = 0;
  void WithouParameterAndEmptyResponse(WithouParameterAndEmptyResponseCallback callback) override;
  void WithParameterAndEmptyResponse(bool b, WithParameterAndEmptyResponseCallback callback) override;
};
class  CheckMethodWithEmptyResponseAsyncWaiter {
 public:
  explicit CheckMethodWithEmptyResponseAsyncWaiter(CheckMethodWithEmptyResponse* proxy);
  ~CheckMethodWithEmptyResponseAsyncWaiter();
  void WithouParameterAndEmptyResponse(
      );
  void WithParameterAndEmptyResponse(
      bool b);

 private:
  CheckMethodWithEmptyResponse* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(CheckMethodWithEmptyResponseAsyncWaiter);
};


class  CheckNameCollisionInterceptorForTesting : public CheckNameCollision {
  virtual CheckNameCollision* GetForwardingInterface() = 0;
  void WithNameCollision(bool message, bool response, WithNameCollisionCallback callback) override;
};
class  CheckNameCollisionAsyncWaiter {
 public:
  explicit CheckNameCollisionAsyncWaiter(CheckNameCollision* proxy);
  ~CheckNameCollisionAsyncWaiter();
  void WithNameCollision(
      bool message, bool response, bool* out_message, bool* out_response);

 private:
  CheckNameCollision* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(CheckNameCollisionAsyncWaiter);
};


class  HandlesNameCollisionInterfaceInterceptorForTesting : public HandlesNameCollisionInterface {
  virtual HandlesNameCollisionInterface* GetForwardingInterface() = 0;
  void Method(EmptyStructPtr handles, MethodCallback callback) override;
};
class  HandlesNameCollisionInterfaceAsyncWaiter {
 public:
  explicit HandlesNameCollisionInterfaceAsyncWaiter(HandlesNameCollisionInterface* proxy);
  ~HandlesNameCollisionInterfaceAsyncWaiter();
  void Method(
      EmptyStructPtr handles, mojo::ScopedHandle* out_handles);

 private:
  HandlesNameCollisionInterface* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(HandlesNameCollisionInterfaceAsyncWaiter);
};




}  // namespace blink
}  // namespace regression_tests

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_REGRESSION_TESTS_MOJOM_BLINK_TEST_UTILS_H_