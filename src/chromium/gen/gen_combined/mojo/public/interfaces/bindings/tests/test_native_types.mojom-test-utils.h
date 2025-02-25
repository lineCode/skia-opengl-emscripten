// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_NATIVE_TYPES_MOJOM_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_NATIVE_TYPES_MOJOM_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/test_native_types.mojom.h"


namespace mojo {
namespace test {


class  PicklePasserInterceptorForTesting : public PicklePasser {
  virtual PicklePasser* GetForwardingInterface() = 0;
  void PassPickledStruct(mojo::test::PickledStructChromium pickle, PassPickledStructCallback callback) override;
  void PassPickledEnum(mojo::test::PickledEnumChromium pickle, PassPickledEnumCallback callback) override;
  void PassPickleContainer(PickleContainerPtr container, PassPickleContainerCallback callback) override;
  void PassPickles(std::vector<mojo::test::PickledStructChromium> pickles, PassPicklesCallback callback) override;
  void PassPickleArrays(std::vector<std::vector<mojo::test::PickledStructChromium>> pickle_arrays, PassPickleArraysCallback callback) override;
};
class  PicklePasserAsyncWaiter {
 public:
  explicit PicklePasserAsyncWaiter(PicklePasser* proxy);
  ~PicklePasserAsyncWaiter();
  void PassPickledStruct(
      mojo::test::PickledStructChromium pickle, mojo::test::PickledStructChromium* out_passed);
  void PassPickledEnum(
      mojo::test::PickledEnumChromium pickle, mojo::test::PickledEnumChromium* out_passed);
  void PassPickleContainer(
      PickleContainerPtr container, PickleContainerPtr* out_passed);
  void PassPickles(
      std::vector<mojo::test::PickledStructChromium> pickles, std::vector<mojo::test::PickledStructChromium>* out_passed);
  void PassPickleArrays(
      std::vector<std::vector<mojo::test::PickledStructChromium>> pickle_arrays, std::vector<std::vector<mojo::test::PickledStructChromium>>* out_passed);

 private:
  PicklePasser* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(PicklePasserAsyncWaiter);
};


class  RectServiceInterceptorForTesting : public RectService {
  virtual RectService* GetForwardingInterface() = 0;
  void AddRect(const mojo::test::RectChromium& r) override;
  void GetLargestRect(GetLargestRectCallback callback) override;
  void PassSharedRect(const mojo::test::SharedRect& r, PassSharedRectCallback callback) override;
};
class  RectServiceAsyncWaiter {
 public:
  explicit RectServiceAsyncWaiter(RectService* proxy);
  ~RectServiceAsyncWaiter();
  void GetLargestRect(
      mojo::test::RectChromium* out_largest);
  void PassSharedRect(
      const mojo::test::SharedRect& r, mojo::test::SharedRect* out_passed);

 private:
  RectService* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(RectServiceAsyncWaiter);
};


class  NativeTypeTesterInterceptorForTesting : public NativeTypeTester {
  virtual NativeTypeTester* GetForwardingInterface() = 0;
  void PassNativeStruct(const mojo::test::TestNativeStruct& s, PassNativeStructCallback callback) override;
  void PassNativeStructWithAttachments(mojo::test::TestNativeStructWithAttachments s, PassNativeStructWithAttachmentsCallback callback) override;
};
class  NativeTypeTesterAsyncWaiter {
 public:
  explicit NativeTypeTesterAsyncWaiter(NativeTypeTester* proxy);
  ~NativeTypeTesterAsyncWaiter();
  void PassNativeStruct(
      const mojo::test::TestNativeStruct& s, mojo::test::TestNativeStruct* out_passed);
  void PassNativeStructWithAttachments(
      mojo::test::TestNativeStructWithAttachments s, mojo::test::TestNativeStructWithAttachments* out_s);

 private:
  NativeTypeTester* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(NativeTypeTesterAsyncWaiter);
};




}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_NATIVE_TYPES_MOJOM_TEST_UTILS_H_