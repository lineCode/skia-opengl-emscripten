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


#include "mojo/public/interfaces/bindings/tests/test_native_types.mojom-blink-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"
#include "mojo/public/interfaces/bindings/tests/rect.mojom-blink.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_NATIVE_TYPES_MOJOM_BLINK_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_NATIVE_TYPES_MOJOM_BLINK_JUMBO_H_
#include "mojo/public/cpp/bindings/tests/rect_blink_traits.h"
#include "mojo/public/cpp/bindings/tests/shared_rect_traits.h"
#endif


namespace mojo {
namespace test {
namespace blink {


void PicklePasserInterceptorForTesting::PassPickledStruct(mojo::test::PickledStructBlink pickle, PassPickledStructCallback callback) {
  GetForwardingInterface()->PassPickledStruct(std::move(pickle), std::move(callback));
}
void PicklePasserInterceptorForTesting::PassPickledEnum(mojo::test::PickledEnumBlink pickle, PassPickledEnumCallback callback) {
  GetForwardingInterface()->PassPickledEnum(std::move(pickle), std::move(callback));
}
void PicklePasserInterceptorForTesting::PassPickleContainer(PickleContainerPtr container, PassPickleContainerCallback callback) {
  GetForwardingInterface()->PassPickleContainer(std::move(container), std::move(callback));
}
void PicklePasserInterceptorForTesting::PassPickles(WTF::Vector<mojo::test::PickledStructBlink> pickles, PassPicklesCallback callback) {
  GetForwardingInterface()->PassPickles(std::move(pickles), std::move(callback));
}
void PicklePasserInterceptorForTesting::PassPickleArrays(WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> pickle_arrays, PassPickleArraysCallback callback) {
  GetForwardingInterface()->PassPickleArrays(std::move(pickle_arrays), std::move(callback));
}
PicklePasserAsyncWaiter::PicklePasserAsyncWaiter(
    PicklePasser* proxy) : proxy_(proxy) {}

PicklePasserAsyncWaiter::~PicklePasserAsyncWaiter() = default;

void PicklePasserAsyncWaiter::PassPickledStruct(
    mojo::test::PickledStructBlink pickle, mojo::test::PickledStructBlink* out_passed) {
  base::RunLoop loop;
  proxy_->PassPickledStruct(std::move(pickle),
      base::BindOnce(
          [](base::RunLoop* loop,
             mojo::test::PickledStructBlink* out_passed
,
             mojo::test::PickledStructBlink passed) {*out_passed = std::move(passed);
            loop->Quit();
          },
          &loop,
          out_passed));
  loop.Run();
}
void PicklePasserAsyncWaiter::PassPickledEnum(
    mojo::test::PickledEnumBlink pickle, mojo::test::PickledEnumBlink* out_passed) {
  base::RunLoop loop;
  proxy_->PassPickledEnum(std::move(pickle),
      base::BindOnce(
          [](base::RunLoop* loop,
             mojo::test::PickledEnumBlink* out_passed
,
             mojo::test::PickledEnumBlink passed) {*out_passed = std::move(passed);
            loop->Quit();
          },
          &loop,
          out_passed));
  loop.Run();
}
void PicklePasserAsyncWaiter::PassPickleContainer(
    PickleContainerPtr container, PickleContainerPtr* out_passed) {
  base::RunLoop loop;
  proxy_->PassPickleContainer(std::move(container),
      base::BindOnce(
          [](base::RunLoop* loop,
             PickleContainerPtr* out_passed
,
             PickleContainerPtr passed) {*out_passed = std::move(passed);
            loop->Quit();
          },
          &loop,
          out_passed));
  loop.Run();
}
void PicklePasserAsyncWaiter::PassPickles(
    WTF::Vector<mojo::test::PickledStructBlink> pickles, WTF::Vector<mojo::test::PickledStructBlink>* out_passed) {
  base::RunLoop loop;
  proxy_->PassPickles(std::move(pickles),
      base::BindOnce(
          [](base::RunLoop* loop,
             WTF::Vector<mojo::test::PickledStructBlink>* out_passed
,
             WTF::Vector<mojo::test::PickledStructBlink> passed) {*out_passed = std::move(passed);
            loop->Quit();
          },
          &loop,
          out_passed));
  loop.Run();
}
void PicklePasserAsyncWaiter::PassPickleArrays(
    WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> pickle_arrays, WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>>* out_passed) {
  base::RunLoop loop;
  proxy_->PassPickleArrays(std::move(pickle_arrays),
      base::BindOnce(
          [](base::RunLoop* loop,
             WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>>* out_passed
,
             WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> passed) {*out_passed = std::move(passed);
            loop->Quit();
          },
          &loop,
          out_passed));
  loop.Run();
}



void RectServiceInterceptorForTesting::AddRect(const mojo::test::RectBlink& r) {
  GetForwardingInterface()->AddRect(std::move(r));
}
void RectServiceInterceptorForTesting::GetLargestRect(GetLargestRectCallback callback) {
  GetForwardingInterface()->GetLargestRect(std::move(callback));
}
void RectServiceInterceptorForTesting::PassSharedRect(const mojo::test::SharedRect& r, PassSharedRectCallback callback) {
  GetForwardingInterface()->PassSharedRect(std::move(r), std::move(callback));
}
RectServiceAsyncWaiter::RectServiceAsyncWaiter(
    RectService* proxy) : proxy_(proxy) {}

RectServiceAsyncWaiter::~RectServiceAsyncWaiter() = default;

void RectServiceAsyncWaiter::GetLargestRect(
    mojo::test::RectBlink* out_largest) {
  base::RunLoop loop;
  proxy_->GetLargestRect(
      base::BindOnce(
          [](base::RunLoop* loop,
             mojo::test::RectBlink* out_largest
,
             const mojo::test::RectBlink& largest) {*out_largest = std::move(largest);
            loop->Quit();
          },
          &loop,
          out_largest));
  loop.Run();
}
void RectServiceAsyncWaiter::PassSharedRect(
    const mojo::test::SharedRect& r, mojo::test::SharedRect* out_passed) {
  base::RunLoop loop;
  proxy_->PassSharedRect(std::move(r),
      base::BindOnce(
          [](base::RunLoop* loop,
             mojo::test::SharedRect* out_passed
,
             const mojo::test::SharedRect& passed) {*out_passed = std::move(passed);
            loop->Quit();
          },
          &loop,
          out_passed));
  loop.Run();
}



void NativeTypeTesterInterceptorForTesting::PassNativeStruct(TestNativeStructMojomPtr s, PassNativeStructCallback callback) {
  GetForwardingInterface()->PassNativeStruct(std::move(s), std::move(callback));
}
void NativeTypeTesterInterceptorForTesting::PassNativeStructWithAttachments(TestNativeStructWithAttachmentsMojomPtr s, PassNativeStructWithAttachmentsCallback callback) {
  GetForwardingInterface()->PassNativeStructWithAttachments(std::move(s), std::move(callback));
}
NativeTypeTesterAsyncWaiter::NativeTypeTesterAsyncWaiter(
    NativeTypeTester* proxy) : proxy_(proxy) {}

NativeTypeTesterAsyncWaiter::~NativeTypeTesterAsyncWaiter() = default;

void NativeTypeTesterAsyncWaiter::PassNativeStruct(
    TestNativeStructMojomPtr s, TestNativeStructMojomPtr* out_passed) {
  base::RunLoop loop;
  proxy_->PassNativeStruct(std::move(s),
      base::BindOnce(
          [](base::RunLoop* loop,
             TestNativeStructMojomPtr* out_passed
,
             TestNativeStructMojomPtr passed) {*out_passed = std::move(passed);
            loop->Quit();
          },
          &loop,
          out_passed));
  loop.Run();
}
void NativeTypeTesterAsyncWaiter::PassNativeStructWithAttachments(
    TestNativeStructWithAttachmentsMojomPtr s, TestNativeStructWithAttachmentsMojomPtr* out_s) {
  base::RunLoop loop;
  proxy_->PassNativeStructWithAttachments(std::move(s),
      base::BindOnce(
          [](base::RunLoop* loop,
             TestNativeStructWithAttachmentsMojomPtr* out_s
,
             TestNativeStructWithAttachmentsMojomPtr s) {*out_s = std::move(s);
            loop->Quit();
          },
          &loop,
          out_s));
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