// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_VALIDATION_TEST_INTERFACES_MOJOM_BLINK_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_VALIDATION_TEST_INTERFACES_MOJOM_BLINK_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/validation_test_interfaces.mojom-blink.h"


namespace mojo {
namespace test {
namespace blink {


class  InterfaceAInterceptorForTesting : public InterfaceA {
  virtual InterfaceA* GetForwardingInterface() = 0;
};
class  InterfaceAAsyncWaiter {
 public:
  explicit InterfaceAAsyncWaiter(InterfaceA* proxy);
  ~InterfaceAAsyncWaiter();

 private:
  InterfaceA* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(InterfaceAAsyncWaiter);
};


class  BoundsCheckTestInterfaceInterceptorForTesting : public BoundsCheckTestInterface {
  virtual BoundsCheckTestInterface* GetForwardingInterface() = 0;
  void Method0(uint8_t param0, Method0Callback callback) override;
  void Method1(uint8_t param0) override;
};
class  BoundsCheckTestInterfaceAsyncWaiter {
 public:
  explicit BoundsCheckTestInterfaceAsyncWaiter(BoundsCheckTestInterface* proxy);
  ~BoundsCheckTestInterfaceAsyncWaiter();
  void Method0(
      uint8_t param0, uint8_t* out_param0);

 private:
  BoundsCheckTestInterface* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(BoundsCheckTestInterfaceAsyncWaiter);
};


class  ConformanceTestInterfaceInterceptorForTesting : public ConformanceTestInterface {
  virtual ConformanceTestInterface* GetForwardingInterface() = 0;
  void Method0(float param0) override;
  void Method1(StructAPtr param0) override;
  void Method2(StructBPtr param0, StructAPtr param1) override;
  void Method3(const WTF::Vector<bool>& param0) override;
  void Method4(StructCPtr param0, const WTF::Vector<uint8_t>& param1) override;
  void Method5(StructEPtr param0, mojo::ScopedDataPipeProducerHandle param1) override;
  void Method6(const WTF::Vector<WTF::Vector<uint8_t>>& param0) override;
  void Method7(StructFPtr param0, const WTF::Vector<base::Optional<WTF::Vector<uint8_t>>>& param1) override;
  void Method8(const WTF::Vector<base::Optional<WTF::Vector<WTF::String>>>& param0) override;
  void Method9(base::Optional<WTF::Vector<WTF::Vector<mojo::ScopedHandle>>> param0) override;
  void Method10(const WTF::HashMap<WTF::String, uint8_t>& param0) override;
  void Method11(StructGPtr param0) override;
  void Method12(float param0, Method12Callback callback) override;
  void Method13(InterfaceAPtr param0, uint32_t param1, InterfaceAPtr param2) override;
  void Method14(EnumA param0, EnumB param1) override;
  void Method15(const base::Optional<WTF::Vector<EnumA>>& param0, const base::Optional<WTF::Vector<EnumB>>& param1) override;
  void Method16(const base::Optional<WTF::HashMap<EnumA, EnumA>>& param0) override;
  void Method17(WTF::Vector<mojo::PendingRemote<InterfaceA>> param0) override;
  void Method18(UnionAPtr param0) override;
  void Method19(RecursivePtr recursive) override;
  void Method20(WTF::HashMap<StructBPtr, uint8_t> param0) override;
  void Method21(ExtensibleEmptyEnum param0) override;
  void Method22(EmptyEnum param0) override;
};
class  ConformanceTestInterfaceAsyncWaiter {
 public:
  explicit ConformanceTestInterfaceAsyncWaiter(ConformanceTestInterface* proxy);
  ~ConformanceTestInterfaceAsyncWaiter();
  void Method12(
      float param0, float* out_param0);

 private:
  ConformanceTestInterface* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(ConformanceTestInterfaceAsyncWaiter);
};


class  IntegrationTestInterfaceInterceptorForTesting : public IntegrationTestInterface {
  virtual IntegrationTestInterface* GetForwardingInterface() = 0;
  void Method0(BasicStructPtr param0, Method0Callback callback) override;
};
class  IntegrationTestInterfaceAsyncWaiter {
 public:
  explicit IntegrationTestInterfaceAsyncWaiter(IntegrationTestInterface* proxy);
  ~IntegrationTestInterfaceAsyncWaiter();
  void Method0(
      BasicStructPtr param0, WTF::Vector<uint8_t>* out_param0);

 private:
  IntegrationTestInterface* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(IntegrationTestInterfaceAsyncWaiter);
};




}  // namespace blink
}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_VALIDATION_TEST_INTERFACES_MOJOM_BLINK_TEST_UTILS_H_