// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_NATIVE_TYPES_MOJOM_BLINK_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_NATIVE_TYPES_MOJOM_BLINK_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/test_native_types.mojom-blink.h"


namespace mojo {
namespace test {
namespace blink {


class  PicklePasserInterceptorForTesting : public PicklePasser {
  virtual PicklePasser* GetForwardingInterface() = 0;
  void PassPickledStruct(mojo::test::PickledStructBlink pickle, PassPickledStructCallback callback) override;
  void PassPickledEnum(mojo::test::PickledEnumBlink pickle, PassPickledEnumCallback callback) override;
  void PassPickleContainer(PickleContainerPtr container, PassPickleContainerCallback callback) override;
  void PassPickles(WTF::Vector<mojo::test::PickledStructBlink> pickles, PassPicklesCallback callback) override;
  void PassPickleArrays(WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> pickle_arrays, PassPickleArraysCallback callback) override;
};
class  PicklePasserAsyncWaiter {
 public:
  explicit PicklePasserAsyncWaiter(PicklePasser* proxy);
  ~PicklePasserAsyncWaiter();
  void PassPickledStruct(
      mojo::test::PickledStructBlink pickle, mojo::test::PickledStructBlink* out_passed);
  void PassPickledEnum(
      mojo::test::PickledEnumBlink pickle, mojo::test::PickledEnumBlink* out_passed);
  void PassPickleContainer(
      PickleContainerPtr container, PickleContainerPtr* out_passed);
  void PassPickles(
      WTF::Vector<mojo::test::PickledStructBlink> pickles, WTF::Vector<mojo::test::PickledStructBlink>* out_passed);
  void PassPickleArrays(
      WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>> pickle_arrays, WTF::Vector<WTF::Vector<mojo::test::PickledStructBlink>>* out_passed);

 private:
  PicklePasser* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasserAsyncWaiter);
};


class  RectServiceInterceptorForTesting : public RectService {
  virtual RectService* GetForwardingInterface() = 0;
  void AddRect(const mojo::test::RectBlink& r) override;
  void GetLargestRect(GetLargestRectCallback callback) override;
  void PassSharedRect(const mojo::test::SharedRect& r, PassSharedRectCallback callback) override;
};
class  RectServiceAsyncWaiter {
 public:
  explicit RectServiceAsyncWaiter(RectService* proxy);
  ~RectServiceAsyncWaiter();
  void GetLargestRect(
      mojo::test::RectBlink* out_largest);
  void PassSharedRect(
      const mojo::test::SharedRect& r, mojo::test::SharedRect* out_passed);

 private:
  RectService* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(RectServiceAsyncWaiter);
};


class  NativeTypeTesterInterceptorForTesting : public NativeTypeTester {
  virtual NativeTypeTester* GetForwardingInterface() = 0;
  void PassNativeStruct(TestNativeStructMojomPtr s, PassNativeStructCallback callback) override;
  void PassNativeStructWithAttachments(TestNativeStructWithAttachmentsMojomPtr s, PassNativeStructWithAttachmentsCallback callback) override;
};
class  NativeTypeTesterAsyncWaiter {
 public:
  explicit NativeTypeTesterAsyncWaiter(NativeTypeTester* proxy);
  ~NativeTypeTesterAsyncWaiter();
  void PassNativeStruct(
      TestNativeStructMojomPtr s, TestNativeStructMojomPtr* out_passed);
  void PassNativeStructWithAttachments(
      TestNativeStructWithAttachmentsMojomPtr s, TestNativeStructWithAttachmentsMojomPtr* out_s);

 private:
  NativeTypeTester* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(NativeTypeTesterAsyncWaiter);
};




}  // namespace blink
}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_NATIVE_TYPES_MOJOM_BLINK_TEST_UTILS_H_