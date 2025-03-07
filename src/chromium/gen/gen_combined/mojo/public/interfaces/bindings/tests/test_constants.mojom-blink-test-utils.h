// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_CONSTANTS_MOJOM_BLINK_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_CONSTANTS_MOJOM_BLINK_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/test_constants.mojom-blink.h"


namespace mojo {
namespace test {
namespace blink {


class  InterfaceWithConstantsInterceptorForTesting : public InterfaceWithConstants {
  virtual InterfaceWithConstants* GetForwardingInterface() = 0;
};
class  InterfaceWithConstantsAsyncWaiter {
 public:
  explicit InterfaceWithConstantsAsyncWaiter(InterfaceWithConstants* proxy);
  ~InterfaceWithConstantsAsyncWaiter();

 private:
  InterfaceWithConstants* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(InterfaceWithConstantsAsyncWaiter);
};




}  // namespace blink
}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_CONSTANTS_MOJOM_BLINK_TEST_UTILS_H_