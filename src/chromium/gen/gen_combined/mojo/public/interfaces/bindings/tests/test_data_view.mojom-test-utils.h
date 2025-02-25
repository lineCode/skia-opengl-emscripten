// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_DATA_VIEW_MOJOM_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_DATA_VIEW_MOJOM_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/test_data_view.mojom.h"


namespace mojo {
namespace test {
namespace data_view {


class  TestInterfaceInterceptorForTesting : public TestInterface {
  virtual TestInterface* GetForwardingInterface() = 0;
  void Echo(int32_t value, EchoCallback callback) override;
};
class  TestInterfaceAsyncWaiter {
 public:
  explicit TestInterfaceAsyncWaiter(TestInterface* proxy);
  ~TestInterfaceAsyncWaiter();
  void Echo(
      int32_t value, int32_t* out_out_value);

 private:
  TestInterface* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(TestInterfaceAsyncWaiter);
};




}  // namespace data_view
}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_DATA_VIEW_MOJOM_TEST_UTILS_H_