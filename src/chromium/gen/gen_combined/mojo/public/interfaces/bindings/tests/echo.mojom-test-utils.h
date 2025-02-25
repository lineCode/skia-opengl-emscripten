// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_ECHO_MOJOM_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_ECHO_MOJOM_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/echo.mojom.h"


namespace test {
namespace echo {
namespace mojom {


class  EchoInterceptorForTesting : public Echo {
  virtual Echo* GetForwardingInterface() = 0;
  void EchoPoint(::test::echo_import::mojom::PointPtr point, EchoPointCallback callback) override;
};
class  EchoAsyncWaiter {
 public:
  explicit EchoAsyncWaiter(Echo* proxy);
  ~EchoAsyncWaiter();
  void EchoPoint(
      ::test::echo_import::mojom::PointPtr point, ::test::echo_import::mojom::PointPtr* out_result);

 private:
  Echo* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(EchoAsyncWaiter);
};




}  // namespace mojom
}  // namespace echo
}  // namespace test

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_ECHO_MOJOM_TEST_UTILS_H_