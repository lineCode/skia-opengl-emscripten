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


#include "mojo/public/interfaces/bindings/tests/test_unions.mojom-blink-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"
#include "mojo/public/interfaces/bindings/tests/sample_import.mojom-blink.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_UNIONS_MOJOM_BLINK_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_UNIONS_MOJOM_BLINK_JUMBO_H_
#endif


namespace mojo {
namespace test {
namespace blink {


void SmallCacheInterceptorForTesting::SetIntValue(int64_t int_value) {
  GetForwardingInterface()->SetIntValue(std::move(int_value));
}
void SmallCacheInterceptorForTesting::GetIntValue(GetIntValueCallback callback) {
  GetForwardingInterface()->GetIntValue(std::move(callback));
}
SmallCacheAsyncWaiter::SmallCacheAsyncWaiter(
    SmallCache* proxy) : proxy_(proxy) {}

SmallCacheAsyncWaiter::~SmallCacheAsyncWaiter() = default;

void SmallCacheAsyncWaiter::GetIntValue(
    int64_t* out_int_value) {
  base::RunLoop loop;
  proxy_->GetIntValue(
      base::BindOnce(
          [](base::RunLoop* loop,
             int64_t* out_int_value
,
             int64_t int_value) {*out_int_value = std::move(int_value);
            loop->Quit();
          },
          &loop,
          out_int_value));
  loop.Run();
}



void UnionInterfaceInterceptorForTesting::Echo(PodUnionPtr in_val, EchoCallback callback) {
  GetForwardingInterface()->Echo(std::move(in_val), std::move(callback));
}
UnionInterfaceAsyncWaiter::UnionInterfaceAsyncWaiter(
    UnionInterface* proxy) : proxy_(proxy) {}

UnionInterfaceAsyncWaiter::~UnionInterfaceAsyncWaiter() = default;

void UnionInterfaceAsyncWaiter::Echo(
    PodUnionPtr in_val, PodUnionPtr* out_out_val) {
  base::RunLoop loop;
  proxy_->Echo(std::move(in_val),
      base::BindOnce(
          [](base::RunLoop* loop,
             PodUnionPtr* out_out_val
,
             PodUnionPtr out_val) {*out_out_val = std::move(out_val);
            loop->Quit();
          },
          &loop,
          out_out_val));
  loop.Run();
}





}  // namespace blink
}  // namespace test
}  // namespace mojo

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif