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


#include "mojo/public/interfaces/bindings/tests/test_bad_messages.mojom-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_BAD_MESSAGES_MOJOM_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_BAD_MESSAGES_MOJOM_JUMBO_H_
#endif


namespace mojo {
namespace test {


void TestBadMessagesInterceptorForTesting::RejectEventually(RejectEventuallyCallback callback) {
  GetForwardingInterface()->RejectEventually(std::move(callback));
}
void TestBadMessagesInterceptorForTesting::RequestResponse(RequestResponseCallback callback) {
  GetForwardingInterface()->RequestResponse(std::move(callback));
}
void TestBadMessagesInterceptorForTesting::RejectSync(RejectSyncCallback callback) {
  GetForwardingInterface()->RejectSync(std::move(callback));
}
void TestBadMessagesInterceptorForTesting::RequestResponseSync(RequestResponseSyncCallback callback) {
  GetForwardingInterface()->RequestResponseSync(std::move(callback));
}
TestBadMessagesAsyncWaiter::TestBadMessagesAsyncWaiter(
    TestBadMessages* proxy) : proxy_(proxy) {}

TestBadMessagesAsyncWaiter::~TestBadMessagesAsyncWaiter() = default;

void TestBadMessagesAsyncWaiter::RejectEventually(
    ) {
  base::RunLoop loop;
  proxy_->RejectEventually(
      base::BindOnce(
          [](base::RunLoop* loop) {
            loop->Quit();
          },
          &loop));
  loop.Run();
}
void TestBadMessagesAsyncWaiter::RequestResponse(
    ) {
  base::RunLoop loop;
  proxy_->RequestResponse(
      base::BindOnce(
          [](base::RunLoop* loop) {
            loop->Quit();
          },
          &loop));
  loop.Run();
}
void TestBadMessagesAsyncWaiter::RejectSync(
    ) {
  base::RunLoop loop;
  proxy_->RejectSync(
      base::BindOnce(
          [](base::RunLoop* loop) {
            loop->Quit();
          },
          &loop));
  loop.Run();
}
void TestBadMessagesAsyncWaiter::RequestResponseSync(
    ) {
  base::RunLoop loop;
  proxy_->RequestResponseSync(
      base::BindOnce(
          [](base::RunLoop* loop) {
            loop->Quit();
          },
          &loop));
  loop.Run();
}





}  // namespace test
}  // namespace mojo

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif