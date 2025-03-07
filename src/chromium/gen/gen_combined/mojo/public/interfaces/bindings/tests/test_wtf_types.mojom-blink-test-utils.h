// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_WTF_TYPES_MOJOM_BLINK_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_WTF_TYPES_MOJOM_BLINK_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/test_wtf_types.mojom-blink.h"


namespace mojo {
namespace test {
namespace blink {


class  TestWTFInterceptorForTesting : public TestWTF {
  virtual TestWTF* GetForwardingInterface() = 0;
  void EchoString(const WTF::String& str, EchoStringCallback callback) override;
  void EchoStringArray(const base::Optional<WTF::Vector<WTF::String>>& arr, EchoStringArrayCallback callback) override;
  void EchoStringMap(const base::Optional<WTF::HashMap<WTF::String, WTF::String>>& str_map, EchoStringMapCallback callback) override;
};
class  TestWTFAsyncWaiter {
 public:
  explicit TestWTFAsyncWaiter(TestWTF* proxy);
  ~TestWTFAsyncWaiter();
  void EchoString(
      const WTF::String& str, WTF::String* out_str);
  void EchoStringArray(
      const base::Optional<WTF::Vector<WTF::String>>& arr, base::Optional<WTF::Vector<WTF::String>>* out_arr);
  void EchoStringMap(
      const base::Optional<WTF::HashMap<WTF::String, WTF::String>>& str_map, base::Optional<WTF::HashMap<WTF::String, WTF::String>>* out_str_map);

 private:
  TestWTF* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(TestWTFAsyncWaiter);
};




}  // namespace blink
}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_WTF_TYPES_MOJOM_BLINK_TEST_UTILS_H_