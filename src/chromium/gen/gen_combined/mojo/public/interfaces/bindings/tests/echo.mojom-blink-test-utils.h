// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_ECHO_MOJOM_BLINK_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_ECHO_MOJOM_BLINK_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/echo.mojom-blink.h"


namespace test {
namespace echo {
namespace mojom {
namespace blink {


class  EchoInterceptorForTesting : public Echo {
  virtual Echo* GetForwardingInterface() = 0;
  void EchoPoint(::test::echo_import::mojom::blink::PointPtr point, EchoPointCallback callback) override;
};
class  EchoAsyncWaiter {
 public:
  explicit EchoAsyncWaiter(Echo* proxy);
  ~EchoAsyncWaiter();
  void EchoPoint(
      ::test::echo_import::mojom::blink::PointPtr point, ::test::echo_import::mojom::blink::PointPtr* out_result);

 private:
  Echo* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(EchoAsyncWaiter);
};




}  // namespace blink
}  // namespace mojom
}  // namespace echo
}  // namespace test

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_ECHO_MOJOM_BLINK_TEST_UTILS_H_