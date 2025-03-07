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


#include "mojo/public/interfaces/bindings/tests/ping_service.mojom-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_PING_SERVICE_MOJOM_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_PING_SERVICE_MOJOM_JUMBO_H_
#endif


namespace mojo {
namespace test {


void PingServiceInterceptorForTesting::Ping(PingCallback callback) {
  GetForwardingInterface()->Ping(std::move(callback));
}
PingServiceAsyncWaiter::PingServiceAsyncWaiter(
    PingService* proxy) : proxy_(proxy) {}

PingServiceAsyncWaiter::~PingServiceAsyncWaiter() = default;

void PingServiceAsyncWaiter::Ping(
    ) {
  base::RunLoop loop;
  proxy_->Ping(
      base::BindOnce(
          [](base::RunLoop* loop) {
            loop->Quit();
          },
          &loop));
  loop.Run();
}



void EchoServiceInterceptorForTesting::Echo(const std::string& test_data, EchoCallback callback) {
  GetForwardingInterface()->Echo(std::move(test_data), std::move(callback));
}
EchoServiceAsyncWaiter::EchoServiceAsyncWaiter(
    EchoService* proxy) : proxy_(proxy) {}

EchoServiceAsyncWaiter::~EchoServiceAsyncWaiter() = default;

void EchoServiceAsyncWaiter::Echo(
    const std::string& test_data, std::string* out_echo_data) {
  base::RunLoop loop;
  proxy_->Echo(std::move(test_data),
      base::BindOnce(
          [](base::RunLoop* loop,
             std::string* out_echo_data
,
             const std::string& echo_data) {*out_echo_data = std::move(echo_data);
            loop->Quit();
          },
          &loop,
          out_echo_data));
  loop.Run();
}



void HandleTrampolineInterceptorForTesting::BounceOne(mojo::ScopedMessagePipeHandle one, BounceOneCallback callback) {
  GetForwardingInterface()->BounceOne(std::move(one), std::move(callback));
}
void HandleTrampolineInterceptorForTesting::BounceTwo(mojo::ScopedMessagePipeHandle one, mojo::ScopedMessagePipeHandle two, BounceTwoCallback callback) {
  GetForwardingInterface()->BounceTwo(std::move(one), std::move(two), std::move(callback));
}
HandleTrampolineAsyncWaiter::HandleTrampolineAsyncWaiter(
    HandleTrampoline* proxy) : proxy_(proxy) {}

HandleTrampolineAsyncWaiter::~HandleTrampolineAsyncWaiter() = default;

void HandleTrampolineAsyncWaiter::BounceOne(
    mojo::ScopedMessagePipeHandle one, mojo::ScopedMessagePipeHandle* out_one) {
  base::RunLoop loop;
  proxy_->BounceOne(std::move(one),
      base::BindOnce(
          [](base::RunLoop* loop,
             mojo::ScopedMessagePipeHandle* out_one
,
             mojo::ScopedMessagePipeHandle one) {*out_one = std::move(one);
            loop->Quit();
          },
          &loop,
          out_one));
  loop.Run();
}
void HandleTrampolineAsyncWaiter::BounceTwo(
    mojo::ScopedMessagePipeHandle one, mojo::ScopedMessagePipeHandle two, mojo::ScopedMessagePipeHandle* out_one, mojo::ScopedMessagePipeHandle* out_two) {
  base::RunLoop loop;
  proxy_->BounceTwo(std::move(one),std::move(two),
      base::BindOnce(
          [](base::RunLoop* loop,
             mojo::ScopedMessagePipeHandle* out_one
,
             mojo::ScopedMessagePipeHandle* out_two
,
             mojo::ScopedMessagePipeHandle one,
             mojo::ScopedMessagePipeHandle two) {*out_one = std::move(one);*out_two = std::move(two);
            loop->Quit();
          },
          &loop,
          out_one,
          out_two));
  loop.Run();
}





}  // namespace test
}  // namespace mojo

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif