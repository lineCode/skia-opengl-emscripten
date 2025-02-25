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


#include "mojo/public/interfaces/bindings/tests/test_native_types.mojom-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"
#include "mojo/public/interfaces/bindings/tests/rect.mojom.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_NATIVE_TYPES_MOJOM_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_NATIVE_TYPES_MOJOM_JUMBO_H_
#include "mojo/public/cpp/bindings/tests/rect_chromium_traits.h"
#include "mojo/public/cpp/bindings/tests/shared_rect_traits.h"
#endif


namespace mojo {
namespace test {


void PicklePasserInterceptorForTesting::PassPickledStruct(mojo::test::PickledStructChromium pickle, PassPickledStructCallback callback) {
  GetForwardingInterface()->PassPickledStruct(std::move(pickle), std::move(callback));
}
void PicklePasserInterceptorForTesting::PassPickledEnum(mojo::test::PickledEnumChromium pickle, PassPickledEnumCallback callback) {
  GetForwardingInterface()->PassPickledEnum(std::move(pickle), std::move(callback));
}
void PicklePasserInterceptorForTesting::PassPickleContainer(PickleContainerPtr container, PassPickleContainerCallback callback) {
  GetForwardingInterface()->PassPickleContainer(std::move(container), std::move(callback));
}
void PicklePasserInterceptorForTesting::PassPickles(std::vector<mojo::test::PickledStructChromium> pickles, PassPicklesCallback callback) {
  GetForwardingInterface()->PassPickles(std::move(pickles), std::move(callback));
}
void PicklePasserInterceptorForTesting::PassPickleArrays(std::vector<std::vector<mojo::test::PickledStructChromium>> pickle_arrays, PassPickleArraysCallback callback) {
  GetForwardingInterface()->PassPickleArrays(std::move(pickle_arrays), std::move(callback));
}
PicklePasserAsyncWaiter::PicklePasserAsyncWaiter(
    PicklePasser* proxy) : proxy_(proxy) {}

PicklePasserAsyncWaiter::~PicklePasserAsyncWaiter() = default;

void PicklePasserAsyncWaiter::PassPickledStruct(
    mojo::test::PickledStructChromium pickle, mojo::test::PickledStructChromium* out_passed) {
  base::RunLoop loop;
  proxy_->PassPickledStruct(std::move(pickle),
      base::BindOnce(
          [](base::RunLoop* loop,
             mojo::test::PickledStructChromium* out_passed
,
             mojo::test::PickledStructChromium passed) {*out_passed = std::move(passed);
            loop->Quit();
          },
          &loop,
          out_passed));
  loop.Run();
}
void PicklePasserAsyncWaiter::PassPickledEnum(
    mojo::test::PickledEnumChromium pickle, mojo::test::PickledEnumChromium* out_passed) {
  base::RunLoop loop;
  proxy_->PassPickledEnum(std::move(pickle),
      base::BindOnce(
          [](base::RunLoop* loop,
             mojo::test::PickledEnumChromium* out_passed
,
             mojo::test::PickledEnumChromium passed) {*out_passed = std::move(passed);
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
    std::vector<mojo::test::PickledStructChromium> pickles, std::vector<mojo::test::PickledStructChromium>* out_passed) {
  base::RunLoop loop;
  proxy_->PassPickles(std::move(pickles),
      base::BindOnce(
          [](base::RunLoop* loop,
             std::vector<mojo::test::PickledStructChromium>* out_passed
,
             std::vector<mojo::test::PickledStructChromium> passed) {*out_passed = std::move(passed);
            loop->Quit();
          },
          &loop,
          out_passed));
  loop.Run();
}
void PicklePasserAsyncWaiter::PassPickleArrays(
    std::vector<std::vector<mojo::test::PickledStructChromium>> pickle_arrays, std::vector<std::vector<mojo::test::PickledStructChromium>>* out_passed) {
  base::RunLoop loop;
  proxy_->PassPickleArrays(std::move(pickle_arrays),
      base::BindOnce(
          [](base::RunLoop* loop,
             std::vector<std::vector<mojo::test::PickledStructChromium>>* out_passed
,
             std::vector<std::vector<mojo::test::PickledStructChromium>> passed) {*out_passed = std::move(passed);
            loop->Quit();
          },
          &loop,
          out_passed));
  loop.Run();
}



void RectServiceInterceptorForTesting::AddRect(const mojo::test::RectChromium& r) {
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
    mojo::test::RectChromium* out_largest) {
  base::RunLoop loop;
  proxy_->GetLargestRect(
      base::BindOnce(
          [](base::RunLoop* loop,
             mojo::test::RectChromium* out_largest
,
             const mojo::test::RectChromium& largest) {*out_largest = std::move(largest);
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



void NativeTypeTesterInterceptorForTesting::PassNativeStruct(const mojo::test::TestNativeStruct& s, PassNativeStructCallback callback) {
  GetForwardingInterface()->PassNativeStruct(std::move(s), std::move(callback));
}
void NativeTypeTesterInterceptorForTesting::PassNativeStructWithAttachments(mojo::test::TestNativeStructWithAttachments s, PassNativeStructWithAttachmentsCallback callback) {
  GetForwardingInterface()->PassNativeStructWithAttachments(std::move(s), std::move(callback));
}
NativeTypeTesterAsyncWaiter::NativeTypeTesterAsyncWaiter(
    NativeTypeTester* proxy) : proxy_(proxy) {}

NativeTypeTesterAsyncWaiter::~NativeTypeTesterAsyncWaiter() = default;

void NativeTypeTesterAsyncWaiter::PassNativeStruct(
    const mojo::test::TestNativeStruct& s, mojo::test::TestNativeStruct* out_passed) {
  base::RunLoop loop;
  proxy_->PassNativeStruct(std::move(s),
      base::BindOnce(
          [](base::RunLoop* loop,
             mojo::test::TestNativeStruct* out_passed
,
             const mojo::test::TestNativeStruct& passed) {*out_passed = std::move(passed);
            loop->Quit();
          },
          &loop,
          out_passed));
  loop.Run();
}
void NativeTypeTesterAsyncWaiter::PassNativeStructWithAttachments(
    mojo::test::TestNativeStructWithAttachments s, mojo::test::TestNativeStructWithAttachments* out_s) {
  base::RunLoop loop;
  proxy_->PassNativeStructWithAttachments(std::move(s),
      base::BindOnce(
          [](base::RunLoop* loop,
             mojo::test::TestNativeStructWithAttachments* out_s
,
             mojo::test::TestNativeStructWithAttachments s) {*out_s = std::move(s);
            loop->Quit();
          },
          &loop,
          out_s));
  loop.Run();
}





}  // namespace test
}  // namespace mojo

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif