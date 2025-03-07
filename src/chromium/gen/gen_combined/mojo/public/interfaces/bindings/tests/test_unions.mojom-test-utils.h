// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_UNIONS_MOJOM_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_UNIONS_MOJOM_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/test_unions.mojom.h"


namespace mojo {
namespace test {


class  SmallCacheInterceptorForTesting : public SmallCache {
  virtual SmallCache* GetForwardingInterface() = 0;
  void SetIntValue(int64_t int_value) override;
  void GetIntValue(GetIntValueCallback callback) override;
};
class  SmallCacheAsyncWaiter {
 public:
  explicit SmallCacheAsyncWaiter(SmallCache* proxy);
  ~SmallCacheAsyncWaiter();
  void GetIntValue(
      int64_t* out_int_value);

 private:
  SmallCache* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(SmallCacheAsyncWaiter);
};


class  UnionInterfaceInterceptorForTesting : public UnionInterface {
  virtual UnionInterface* GetForwardingInterface() = 0;
  void Echo(PodUnionPtr in_val, EchoCallback callback) override;
};
class  UnionInterfaceAsyncWaiter {
 public:
  explicit UnionInterfaceAsyncWaiter(UnionInterface* proxy);
  ~UnionInterfaceAsyncWaiter();
  void Echo(
      PodUnionPtr in_val, PodUnionPtr* out_out_val);

 private:
  UnionInterface* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(UnionInterfaceAsyncWaiter);
};




}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_UNIONS_MOJOM_TEST_UTILS_H_