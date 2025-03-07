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


#include "mojo/public/interfaces/bindings/tests/validation_test_interfaces.mojom-blink-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_VALIDATION_TEST_INTERFACES_MOJOM_BLINK_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_VALIDATION_TEST_INTERFACES_MOJOM_BLINK_JUMBO_H_
#endif


namespace mojo {
namespace test {
namespace blink {


InterfaceAAsyncWaiter::InterfaceAAsyncWaiter(
    InterfaceA* proxy) : proxy_(proxy) {}

InterfaceAAsyncWaiter::~InterfaceAAsyncWaiter() = default;




void BoundsCheckTestInterfaceInterceptorForTesting::Method0(uint8_t param0, Method0Callback callback) {
  GetForwardingInterface()->Method0(std::move(param0), std::move(callback));
}
void BoundsCheckTestInterfaceInterceptorForTesting::Method1(uint8_t param0) {
  GetForwardingInterface()->Method1(std::move(param0));
}
BoundsCheckTestInterfaceAsyncWaiter::BoundsCheckTestInterfaceAsyncWaiter(
    BoundsCheckTestInterface* proxy) : proxy_(proxy) {}

BoundsCheckTestInterfaceAsyncWaiter::~BoundsCheckTestInterfaceAsyncWaiter() = default;

void BoundsCheckTestInterfaceAsyncWaiter::Method0(
    uint8_t param0, uint8_t* out_param0) {
  base::RunLoop loop;
  proxy_->Method0(std::move(param0),
      base::BindOnce(
          [](base::RunLoop* loop,
             uint8_t* out_param0
,
             uint8_t param0) {*out_param0 = std::move(param0);
            loop->Quit();
          },
          &loop,
          out_param0));
  loop.Run();
}



void ConformanceTestInterfaceInterceptorForTesting::Method0(float param0) {
  GetForwardingInterface()->Method0(std::move(param0));
}
void ConformanceTestInterfaceInterceptorForTesting::Method1(StructAPtr param0) {
  GetForwardingInterface()->Method1(std::move(param0));
}
void ConformanceTestInterfaceInterceptorForTesting::Method2(StructBPtr param0, StructAPtr param1) {
  GetForwardingInterface()->Method2(std::move(param0), std::move(param1));
}
void ConformanceTestInterfaceInterceptorForTesting::Method3(const WTF::Vector<bool>& param0) {
  GetForwardingInterface()->Method3(std::move(param0));
}
void ConformanceTestInterfaceInterceptorForTesting::Method4(StructCPtr param0, const WTF::Vector<uint8_t>& param1) {
  GetForwardingInterface()->Method4(std::move(param0), std::move(param1));
}
void ConformanceTestInterfaceInterceptorForTesting::Method5(StructEPtr param0, mojo::ScopedDataPipeProducerHandle param1) {
  GetForwardingInterface()->Method5(std::move(param0), std::move(param1));
}
void ConformanceTestInterfaceInterceptorForTesting::Method6(const WTF::Vector<WTF::Vector<uint8_t>>& param0) {
  GetForwardingInterface()->Method6(std::move(param0));
}
void ConformanceTestInterfaceInterceptorForTesting::Method7(StructFPtr param0, const WTF::Vector<base::Optional<WTF::Vector<uint8_t>>>& param1) {
  GetForwardingInterface()->Method7(std::move(param0), std::move(param1));
}
void ConformanceTestInterfaceInterceptorForTesting::Method8(const WTF::Vector<base::Optional<WTF::Vector<WTF::String>>>& param0) {
  GetForwardingInterface()->Method8(std::move(param0));
}
void ConformanceTestInterfaceInterceptorForTesting::Method9(base::Optional<WTF::Vector<WTF::Vector<mojo::ScopedHandle>>> param0) {
  GetForwardingInterface()->Method9(std::move(param0));
}
void ConformanceTestInterfaceInterceptorForTesting::Method10(const WTF::HashMap<WTF::String, uint8_t>& param0) {
  GetForwardingInterface()->Method10(std::move(param0));
}
void ConformanceTestInterfaceInterceptorForTesting::Method11(StructGPtr param0) {
  GetForwardingInterface()->Method11(std::move(param0));
}
void ConformanceTestInterfaceInterceptorForTesting::Method12(float param0, Method12Callback callback) {
  GetForwardingInterface()->Method12(std::move(param0), std::move(callback));
}
void ConformanceTestInterfaceInterceptorForTesting::Method13(InterfaceAPtr param0, uint32_t param1, InterfaceAPtr param2) {
  GetForwardingInterface()->Method13(std::move(param0), std::move(param1), std::move(param2));
}
void ConformanceTestInterfaceInterceptorForTesting::Method14(EnumA param0, EnumB param1) {
  GetForwardingInterface()->Method14(std::move(param0), std::move(param1));
}
void ConformanceTestInterfaceInterceptorForTesting::Method15(const base::Optional<WTF::Vector<EnumA>>& param0, const base::Optional<WTF::Vector<EnumB>>& param1) {
  GetForwardingInterface()->Method15(std::move(param0), std::move(param1));
}
void ConformanceTestInterfaceInterceptorForTesting::Method16(const base::Optional<WTF::HashMap<EnumA, EnumA>>& param0) {
  GetForwardingInterface()->Method16(std::move(param0));
}
void ConformanceTestInterfaceInterceptorForTesting::Method17(WTF::Vector<mojo::PendingRemote<InterfaceA>> param0) {
  GetForwardingInterface()->Method17(std::move(param0));
}
void ConformanceTestInterfaceInterceptorForTesting::Method18(UnionAPtr param0) {
  GetForwardingInterface()->Method18(std::move(param0));
}
void ConformanceTestInterfaceInterceptorForTesting::Method19(RecursivePtr recursive) {
  GetForwardingInterface()->Method19(std::move(recursive));
}
void ConformanceTestInterfaceInterceptorForTesting::Method20(WTF::HashMap<StructBPtr, uint8_t> param0) {
  GetForwardingInterface()->Method20(std::move(param0));
}
void ConformanceTestInterfaceInterceptorForTesting::Method21(ExtensibleEmptyEnum param0) {
  GetForwardingInterface()->Method21(std::move(param0));
}
void ConformanceTestInterfaceInterceptorForTesting::Method22(EmptyEnum param0) {
  GetForwardingInterface()->Method22(std::move(param0));
}
ConformanceTestInterfaceAsyncWaiter::ConformanceTestInterfaceAsyncWaiter(
    ConformanceTestInterface* proxy) : proxy_(proxy) {}

ConformanceTestInterfaceAsyncWaiter::~ConformanceTestInterfaceAsyncWaiter() = default;

void ConformanceTestInterfaceAsyncWaiter::Method12(
    float param0, float* out_param0) {
  base::RunLoop loop;
  proxy_->Method12(std::move(param0),
      base::BindOnce(
          [](base::RunLoop* loop,
             float* out_param0
,
             float param0) {*out_param0 = std::move(param0);
            loop->Quit();
          },
          &loop,
          out_param0));
  loop.Run();
}



void IntegrationTestInterfaceInterceptorForTesting::Method0(BasicStructPtr param0, Method0Callback callback) {
  GetForwardingInterface()->Method0(std::move(param0), std::move(callback));
}
IntegrationTestInterfaceAsyncWaiter::IntegrationTestInterfaceAsyncWaiter(
    IntegrationTestInterface* proxy) : proxy_(proxy) {}

IntegrationTestInterfaceAsyncWaiter::~IntegrationTestInterfaceAsyncWaiter() = default;

void IntegrationTestInterfaceAsyncWaiter::Method0(
    BasicStructPtr param0, WTF::Vector<uint8_t>* out_param0) {
  base::RunLoop loop;
  proxy_->Method0(std::move(param0),
      base::BindOnce(
          [](base::RunLoop* loop,
             WTF::Vector<uint8_t>* out_param0
,
             const WTF::Vector<uint8_t>& param0) {*out_param0 = std::move(param0);
            loop->Quit();
          },
          &loop,
          out_param0));
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