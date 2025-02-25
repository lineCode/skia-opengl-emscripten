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


#include "mojo/public/interfaces/bindings/tests/test_data_view.mojom-blink-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_DATA_VIEW_MOJOM_BLINK_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_DATA_VIEW_MOJOM_BLINK_JUMBO_H_
#endif


namespace mojo {
namespace test {
namespace data_view {
namespace blink {


void TestInterfaceInterceptorForTesting::Echo(int32_t value, EchoCallback callback) {
  GetForwardingInterface()->Echo(std::move(value), std::move(callback));
}
TestInterfaceAsyncWaiter::TestInterfaceAsyncWaiter(
    TestInterface* proxy) : proxy_(proxy) {}

TestInterfaceAsyncWaiter::~TestInterfaceAsyncWaiter() = default;

void TestInterfaceAsyncWaiter::Echo(
    int32_t value, int32_t* out_out_value) {
  base::RunLoop loop;
  proxy_->Echo(std::move(value),
      base::BindOnce(
          [](base::RunLoop* loop,
             int32_t* out_out_value
,
             int32_t out_value) {*out_out_value = std::move(out_value);
            loop->Quit();
          },
          &loop,
          out_out_value));
  loop.Run();
}





}  // namespace blink
}  // namespace data_view
}  // namespace test
}  // namespace mojo

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif