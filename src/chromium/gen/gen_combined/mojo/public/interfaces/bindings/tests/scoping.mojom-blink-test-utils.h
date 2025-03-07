// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SCOPING_MOJOM_BLINK_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SCOPING_MOJOM_BLINK_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/scoping.mojom-blink.h"


namespace mojo {
namespace test {
namespace blink {


class  AInterceptorForTesting : public A {
  virtual A* GetForwardingInterface() = 0;
  void GetB(mojo::PendingReceiver<B> b) override;
};
class  AAsyncWaiter {
 public:
  explicit AAsyncWaiter(A* proxy);
  ~AAsyncWaiter();

 private:
  A* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(AAsyncWaiter);
};


class  BInterceptorForTesting : public B {
  virtual B* GetForwardingInterface() = 0;
  void GetC(mojo::PendingReceiver<C> c) override;
};
class  BAsyncWaiter {
 public:
  explicit BAsyncWaiter(B* proxy);
  ~BAsyncWaiter();

 private:
  B* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(BAsyncWaiter);
};


class  CInterceptorForTesting : public C {
  virtual C* GetForwardingInterface() = 0;
  void D() override;
};
class  CAsyncWaiter {
 public:
  explicit CAsyncWaiter(C* proxy);
  ~CAsyncWaiter();

 private:
  C* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(CAsyncWaiter);
};




}  // namespace blink
}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SCOPING_MOJOM_BLINK_TEST_UTILS_H_