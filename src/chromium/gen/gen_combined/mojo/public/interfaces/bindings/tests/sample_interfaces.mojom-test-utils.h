// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_INTERFACES_MOJOM_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_INTERFACES_MOJOM_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/sample_interfaces.mojom.h"


namespace sample {


class  PingTestInterceptorForTesting : public PingTest {
  virtual PingTest* GetForwardingInterface() = 0;
  void Ping(PingCallback callback) override;
};
class  PingTestAsyncWaiter {
 public:
  explicit PingTestAsyncWaiter(PingTest* proxy);
  ~PingTestAsyncWaiter();
  void Ping(
      );

 private:
  PingTest* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(PingTestAsyncWaiter);
};


class  ProviderInterceptorForTesting : public Provider {
  virtual Provider* GetForwardingInterface() = 0;
  void EchoString(const std::string& a, EchoStringCallback callback) override;
  void EchoStrings(const std::string& a, const std::string& b, EchoStringsCallback callback) override;
  void EchoMessagePipeHandle(mojo::ScopedMessagePipeHandle a, EchoMessagePipeHandleCallback callback) override;
  void EchoEnum(Enum a, EchoEnumCallback callback) override;
  void EchoInt(int32_t a, EchoIntCallback callback) override;
};
class  ProviderAsyncWaiter {
 public:
  explicit ProviderAsyncWaiter(Provider* proxy);
  ~ProviderAsyncWaiter();
  void EchoString(
      const std::string& a, std::string* out_a);
  void EchoStrings(
      const std::string& a, const std::string& b, std::string* out_a, std::string* out_b);
  void EchoMessagePipeHandle(
      mojo::ScopedMessagePipeHandle a, mojo::ScopedMessagePipeHandle* out_a);
  void EchoEnum(
      Enum a, Enum* out_a);
  void EchoInt(
      int32_t a, int32_t* out_a);

 private:
  Provider* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(ProviderAsyncWaiter);
};


class  IntegerAccessorInterceptorForTesting : public IntegerAccessor {
  virtual IntegerAccessor* GetForwardingInterface() = 0;
  void GetInteger(GetIntegerCallback callback) override;
  void SetInteger(int64_t data, Enum type) override;
};
class  IntegerAccessorAsyncWaiter {
 public:
  explicit IntegerAccessorAsyncWaiter(IntegerAccessor* proxy);
  ~IntegerAccessorAsyncWaiter();
  void GetInteger(
      int64_t* out_data, Enum* out_type);

 private:
  IntegerAccessor* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(IntegerAccessorAsyncWaiter);
};




}  // namespace sample

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_INTERFACES_MOJOM_TEST_UTILS_H_