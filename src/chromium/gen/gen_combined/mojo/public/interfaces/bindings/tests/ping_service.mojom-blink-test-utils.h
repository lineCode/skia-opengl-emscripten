// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_PING_SERVICE_MOJOM_BLINK_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_PING_SERVICE_MOJOM_BLINK_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/ping_service.mojom-blink.h"


namespace mojo {
namespace test {
namespace blink {


class  PingServiceInterceptorForTesting : public PingService {
  virtual PingService* GetForwardingInterface() = 0;
  void Ping(PingCallback callback) override;
};
class  PingServiceAsyncWaiter {
 public:
  explicit PingServiceAsyncWaiter(PingService* proxy);
  ~PingServiceAsyncWaiter();
  void Ping(
      );

 private:
  PingService* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(PingServiceAsyncWaiter);
};


class  EchoServiceInterceptorForTesting : public EchoService {
  virtual EchoService* GetForwardingInterface() = 0;
  void Echo(const WTF::String& test_data, EchoCallback callback) override;
};
class  EchoServiceAsyncWaiter {
 public:
  explicit EchoServiceAsyncWaiter(EchoService* proxy);
  ~EchoServiceAsyncWaiter();
  void Echo(
      const WTF::String& test_data, WTF::String* out_echo_data);

 private:
  EchoService* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(EchoServiceAsyncWaiter);
};


class  HandleTrampolineInterceptorForTesting : public HandleTrampoline {
  virtual HandleTrampoline* GetForwardingInterface() = 0;
  void BounceOne(mojo::ScopedMessagePipeHandle one, BounceOneCallback callback) override;
  void BounceTwo(mojo::ScopedMessagePipeHandle one, mojo::ScopedMessagePipeHandle two, BounceTwoCallback callback) override;
};
class  HandleTrampolineAsyncWaiter {
 public:
  explicit HandleTrampolineAsyncWaiter(HandleTrampoline* proxy);
  ~HandleTrampolineAsyncWaiter();
  void BounceOne(
      mojo::ScopedMessagePipeHandle one, mojo::ScopedMessagePipeHandle* out_one);
  void BounceTwo(
      mojo::ScopedMessagePipeHandle one, mojo::ScopedMessagePipeHandle two, mojo::ScopedMessagePipeHandle* out_one, mojo::ScopedMessagePipeHandle* out_two);

 private:
  HandleTrampoline* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(HandleTrampolineAsyncWaiter);
};




}  // namespace blink
}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_PING_SERVICE_MOJOM_BLINK_TEST_UTILS_H_