// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_WTF_TYPES_MOJOM_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_WTF_TYPES_MOJOM_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/test_wtf_types.mojom.h"


namespace mojo {
namespace test {


class  TestWTFInterceptorForTesting : public TestWTF {
  virtual TestWTF* GetForwardingInterface() = 0;
  void EchoString(const base::Optional<std::string>& str, EchoStringCallback callback) override;
  void EchoStringArray(const base::Optional<std::vector<base::Optional<std::string>>>& arr, EchoStringArrayCallback callback) override;
  void EchoStringMap(const base::Optional<base::flat_map<std::string, base::Optional<std::string>>>& str_map, EchoStringMapCallback callback) override;
};
class  TestWTFAsyncWaiter {
 public:
  explicit TestWTFAsyncWaiter(TestWTF* proxy);
  ~TestWTFAsyncWaiter();
  void EchoString(
      const base::Optional<std::string>& str, base::Optional<std::string>* out_str);
  void EchoStringArray(
      const base::Optional<std::vector<base::Optional<std::string>>>& arr, base::Optional<std::vector<base::Optional<std::string>>>* out_arr);
  void EchoStringMap(
      const base::Optional<base::flat_map<std::string, base::Optional<std::string>>>& str_map, base::Optional<base::flat_map<std::string, base::Optional<std::string>>>* out_str_map);

 private:
  TestWTF* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(TestWTFAsyncWaiter);
};




}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_WTF_TYPES_MOJOM_TEST_UTILS_H_