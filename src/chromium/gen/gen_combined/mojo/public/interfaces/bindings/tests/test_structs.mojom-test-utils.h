// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_STRUCTS_MOJOM_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_STRUCTS_MOJOM_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/test_structs.mojom.h"


namespace mojo {
namespace test {


class  SomeInterfaceInterceptorForTesting : public SomeInterface {
  virtual SomeInterface* GetForwardingInterface() = 0;
  void SomeMethod(RectPairPtr pair, SomeMethodCallback callback) override;
};
class  SomeInterfaceAsyncWaiter {
 public:
  explicit SomeInterfaceAsyncWaiter(SomeInterface* proxy);
  ~SomeInterfaceAsyncWaiter();
  void SomeMethod(
      RectPairPtr pair, RectPairPtr* out_other_pair);

 private:
  SomeInterface* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(SomeInterfaceAsyncWaiter);
};




}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_STRUCTS_MOJOM_TEST_UTILS_H_