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


#include "mojo/public/interfaces/bindings/tests/echo.mojom-blink-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"
#include "mojo/public/interfaces/bindings/tests/echo_import/echo_import.mojom-blink.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_ECHO_MOJOM_BLINK_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_ECHO_MOJOM_BLINK_JUMBO_H_
#endif


namespace test {
namespace echo {
namespace mojom {
namespace blink {


void EchoInterceptorForTesting::EchoPoint(::test::echo_import::mojom::blink::PointPtr point, EchoPointCallback callback) {
  GetForwardingInterface()->EchoPoint(std::move(point), std::move(callback));
}
EchoAsyncWaiter::EchoAsyncWaiter(
    Echo* proxy) : proxy_(proxy) {}

EchoAsyncWaiter::~EchoAsyncWaiter() = default;

void EchoAsyncWaiter::EchoPoint(
    ::test::echo_import::mojom::blink::PointPtr point, ::test::echo_import::mojom::blink::PointPtr* out_result) {
  base::RunLoop loop;
  proxy_->EchoPoint(std::move(point),
      base::BindOnce(
          [](base::RunLoop* loop,
             ::test::echo_import::mojom::blink::PointPtr* out_result
,
             ::test::echo_import::mojom::blink::PointPtr result) {*out_result = std::move(result);
            loop->Quit();
          },
          &loop,
          out_result));
  loop.Run();
}





}  // namespace blink
}  // namespace mojom
}  // namespace echo
}  // namespace test

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif