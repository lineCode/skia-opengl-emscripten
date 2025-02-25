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


#include "mojo/public/interfaces/bindings/tests/sample_interfaces.mojom-blink-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_INTERFACES_MOJOM_BLINK_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_INTERFACES_MOJOM_BLINK_JUMBO_H_
#endif


namespace sample {
namespace blink {


void PingTestInterceptorForTesting::Ping(PingCallback callback) {
  GetForwardingInterface()->Ping(std::move(callback));
}
PingTestAsyncWaiter::PingTestAsyncWaiter(
    PingTest* proxy) : proxy_(proxy) {}

PingTestAsyncWaiter::~PingTestAsyncWaiter() = default;

void PingTestAsyncWaiter::Ping(
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



void ProviderInterceptorForTesting::EchoString(const WTF::String& a, EchoStringCallback callback) {
  GetForwardingInterface()->EchoString(std::move(a), std::move(callback));
}
void ProviderInterceptorForTesting::EchoStrings(const WTF::String& a, const WTF::String& b, EchoStringsCallback callback) {
  GetForwardingInterface()->EchoStrings(std::move(a), std::move(b), std::move(callback));
}
void ProviderInterceptorForTesting::EchoMessagePipeHandle(mojo::ScopedMessagePipeHandle a, EchoMessagePipeHandleCallback callback) {
  GetForwardingInterface()->EchoMessagePipeHandle(std::move(a), std::move(callback));
}
void ProviderInterceptorForTesting::EchoEnum(Enum a, EchoEnumCallback callback) {
  GetForwardingInterface()->EchoEnum(std::move(a), std::move(callback));
}
void ProviderInterceptorForTesting::EchoInt(int32_t a, EchoIntCallback callback) {
  GetForwardingInterface()->EchoInt(std::move(a), std::move(callback));
}
ProviderAsyncWaiter::ProviderAsyncWaiter(
    Provider* proxy) : proxy_(proxy) {}

ProviderAsyncWaiter::~ProviderAsyncWaiter() = default;

void ProviderAsyncWaiter::EchoString(
    const WTF::String& a, WTF::String* out_a) {
  base::RunLoop loop;
  proxy_->EchoString(std::move(a),
      base::BindOnce(
          [](base::RunLoop* loop,
             WTF::String* out_a
,
             const WTF::String& a) {*out_a = std::move(a);
            loop->Quit();
          },
          &loop,
          out_a));
  loop.Run();
}
void ProviderAsyncWaiter::EchoStrings(
    const WTF::String& a, const WTF::String& b, WTF::String* out_a, WTF::String* out_b) {
  base::RunLoop loop;
  proxy_->EchoStrings(std::move(a),std::move(b),
      base::BindOnce(
          [](base::RunLoop* loop,
             WTF::String* out_a
,
             WTF::String* out_b
,
             const WTF::String& a,
             const WTF::String& b) {*out_a = std::move(a);*out_b = std::move(b);
            loop->Quit();
          },
          &loop,
          out_a,
          out_b));
  loop.Run();
}
void ProviderAsyncWaiter::EchoMessagePipeHandle(
    mojo::ScopedMessagePipeHandle a, mojo::ScopedMessagePipeHandle* out_a) {
  base::RunLoop loop;
  proxy_->EchoMessagePipeHandle(std::move(a),
      base::BindOnce(
          [](base::RunLoop* loop,
             mojo::ScopedMessagePipeHandle* out_a
,
             mojo::ScopedMessagePipeHandle a) {*out_a = std::move(a);
            loop->Quit();
          },
          &loop,
          out_a));
  loop.Run();
}
void ProviderAsyncWaiter::EchoEnum(
    Enum a, Enum* out_a) {
  base::RunLoop loop;
  proxy_->EchoEnum(std::move(a),
      base::BindOnce(
          [](base::RunLoop* loop,
             Enum* out_a
,
             Enum a) {*out_a = std::move(a);
            loop->Quit();
          },
          &loop,
          out_a));
  loop.Run();
}
void ProviderAsyncWaiter::EchoInt(
    int32_t a, int32_t* out_a) {
  base::RunLoop loop;
  proxy_->EchoInt(std::move(a),
      base::BindOnce(
          [](base::RunLoop* loop,
             int32_t* out_a
,
             int32_t a) {*out_a = std::move(a);
            loop->Quit();
          },
          &loop,
          out_a));
  loop.Run();
}



void IntegerAccessorInterceptorForTesting::GetInteger(GetIntegerCallback callback) {
  GetForwardingInterface()->GetInteger(std::move(callback));
}
void IntegerAccessorInterceptorForTesting::SetInteger(int64_t data, Enum type) {
  GetForwardingInterface()->SetInteger(std::move(data), std::move(type));
}
IntegerAccessorAsyncWaiter::IntegerAccessorAsyncWaiter(
    IntegerAccessor* proxy) : proxy_(proxy) {}

IntegerAccessorAsyncWaiter::~IntegerAccessorAsyncWaiter() = default;

void IntegerAccessorAsyncWaiter::GetInteger(
    int64_t* out_data, Enum* out_type) {
  base::RunLoop loop;
  proxy_->GetInteger(
      base::BindOnce(
          [](base::RunLoop* loop,
             int64_t* out_data
,
             Enum* out_type
,
             int64_t data,
             Enum type) {*out_data = std::move(data);*out_type = std::move(type);
            loop->Quit();
          },
          &loop,
          out_data,
          out_type));
  loop.Run();
}





}  // namespace blink
}  // namespace sample

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif