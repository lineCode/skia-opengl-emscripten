// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4056)
#pragma warning(disable:4065)
#pragma warning(disable:4756)
#endif


#include "mojo/public/interfaces/bindings/tests/regression_tests.mojom-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_REGRESSION_TESTS_MOJOM_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_REGRESSION_TESTS_MOJOM_JUMBO_H_
#endif


namespace regression_tests {


void CheckMethodWithEmptyResponseInterceptorForTesting::WithouParameterAndEmptyResponse(WithouParameterAndEmptyResponseCallback callback) {
  GetForwardingInterface()->WithouParameterAndEmptyResponse(std::move(callback));
}
void CheckMethodWithEmptyResponseInterceptorForTesting::WithParameterAndEmptyResponse(bool b, WithParameterAndEmptyResponseCallback callback) {
  GetForwardingInterface()->WithParameterAndEmptyResponse(std::move(b), std::move(callback));
}
CheckMethodWithEmptyResponseAsyncWaiter::CheckMethodWithEmptyResponseAsyncWaiter(
    CheckMethodWithEmptyResponse* proxy) : proxy_(proxy) {}

CheckMethodWithEmptyResponseAsyncWaiter::~CheckMethodWithEmptyResponseAsyncWaiter() = default;

void CheckMethodWithEmptyResponseAsyncWaiter::WithouParameterAndEmptyResponse(
    ) {
  base::RunLoop loop;
  proxy_->WithouParameterAndEmptyResponse(
      base::BindOnce(
          [](base::RunLoop* loop) {
            loop->Quit();
          },
          &loop));
  loop.Run();
}
void CheckMethodWithEmptyResponseAsyncWaiter::WithParameterAndEmptyResponse(
    bool b) {
  base::RunLoop loop;
  proxy_->WithParameterAndEmptyResponse(std::move(b),
      base::BindOnce(
          [](base::RunLoop* loop) {
            loop->Quit();
          },
          &loop));
  loop.Run();
}



void CheckNameCollisionInterceptorForTesting::WithNameCollision(bool message, bool response, WithNameCollisionCallback callback) {
  GetForwardingInterface()->WithNameCollision(std::move(message), std::move(response), std::move(callback));
}
CheckNameCollisionAsyncWaiter::CheckNameCollisionAsyncWaiter(
    CheckNameCollision* proxy) : proxy_(proxy) {}

CheckNameCollisionAsyncWaiter::~CheckNameCollisionAsyncWaiter() = default;

void CheckNameCollisionAsyncWaiter::WithNameCollision(
    bool message, bool response, bool* out_message, bool* out_response) {
  base::RunLoop loop;
  proxy_->WithNameCollision(std::move(message),std::move(response),
      base::BindOnce(
          [](base::RunLoop* loop,
             bool* out_message
,
             bool* out_response
,
             bool message,
             bool response) {*out_message = std::move(message);*out_response = std::move(response);
            loop->Quit();
          },
          &loop,
          out_message,
          out_response));
  loop.Run();
}



void HandlesNameCollisionInterfaceInterceptorForTesting::Method(EmptyStructPtr handles, MethodCallback callback) {
  GetForwardingInterface()->Method(std::move(handles), std::move(callback));
}
HandlesNameCollisionInterfaceAsyncWaiter::HandlesNameCollisionInterfaceAsyncWaiter(
    HandlesNameCollisionInterface* proxy) : proxy_(proxy) {}

HandlesNameCollisionInterfaceAsyncWaiter::~HandlesNameCollisionInterfaceAsyncWaiter() = default;

void HandlesNameCollisionInterfaceAsyncWaiter::Method(
    EmptyStructPtr handles, mojo::ScopedHandle* out_handles) {
  base::RunLoop loop;
  proxy_->Method(std::move(handles),
      base::BindOnce(
          [](base::RunLoop* loop,
             mojo::ScopedHandle* out_handles
,
             mojo::ScopedHandle handles) {*out_handles = std::move(handles);
            loop->Quit();
          },
          &loop,
          out_handles));
  loop.Run();
}





}  // namespace regression_tests

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif