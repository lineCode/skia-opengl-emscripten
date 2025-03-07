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


#include "mojo/public/interfaces/bindings/tests/test_wtf_types.mojom-blink-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_WTF_TYPES_MOJOM_BLINK_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_WTF_TYPES_MOJOM_BLINK_JUMBO_H_
#endif


namespace mojo {
namespace test {
namespace blink {


void TestWTFInterceptorForTesting::EchoString(const WTF::String& str, EchoStringCallback callback) {
  GetForwardingInterface()->EchoString(std::move(str), std::move(callback));
}
void TestWTFInterceptorForTesting::EchoStringArray(const base::Optional<WTF::Vector<WTF::String>>& arr, EchoStringArrayCallback callback) {
  GetForwardingInterface()->EchoStringArray(std::move(arr), std::move(callback));
}
void TestWTFInterceptorForTesting::EchoStringMap(const base::Optional<WTF::HashMap<WTF::String, WTF::String>>& str_map, EchoStringMapCallback callback) {
  GetForwardingInterface()->EchoStringMap(std::move(str_map), std::move(callback));
}
TestWTFAsyncWaiter::TestWTFAsyncWaiter(
    TestWTF* proxy) : proxy_(proxy) {}

TestWTFAsyncWaiter::~TestWTFAsyncWaiter() = default;

void TestWTFAsyncWaiter::EchoString(
    const WTF::String& str, WTF::String* out_str) {
  base::RunLoop loop;
  proxy_->EchoString(std::move(str),
      base::BindOnce(
          [](base::RunLoop* loop,
             WTF::String* out_str
,
             const WTF::String& str) {*out_str = std::move(str);
            loop->Quit();
          },
          &loop,
          out_str));
  loop.Run();
}
void TestWTFAsyncWaiter::EchoStringArray(
    const base::Optional<WTF::Vector<WTF::String>>& arr, base::Optional<WTF::Vector<WTF::String>>* out_arr) {
  base::RunLoop loop;
  proxy_->EchoStringArray(std::move(arr),
      base::BindOnce(
          [](base::RunLoop* loop,
             base::Optional<WTF::Vector<WTF::String>>* out_arr
,
             const base::Optional<WTF::Vector<WTF::String>>& arr) {*out_arr = std::move(arr);
            loop->Quit();
          },
          &loop,
          out_arr));
  loop.Run();
}
void TestWTFAsyncWaiter::EchoStringMap(
    const base::Optional<WTF::HashMap<WTF::String, WTF::String>>& str_map, base::Optional<WTF::HashMap<WTF::String, WTF::String>>* out_str_map) {
  base::RunLoop loop;
  proxy_->EchoStringMap(std::move(str_map),
      base::BindOnce(
          [](base::RunLoop* loop,
             base::Optional<WTF::HashMap<WTF::String, WTF::String>>* out_str_map
,
             const base::Optional<WTF::HashMap<WTF::String, WTF::String>>& str_map) {*out_str_map = std::move(str_map);
            loop->Quit();
          },
          &loop,
          out_str_map));
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