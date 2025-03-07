// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_SYNC_METHODS_MOJOM_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_SYNC_METHODS_MOJOM_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/test_sync_methods.mojom.h"


namespace mojo {
namespace test {


class  TestSyncCodeGenerationInterceptorForTesting : public TestSyncCodeGeneration {
  virtual TestSyncCodeGeneration* GetForwardingInterface() = 0;
  void NoInput(NoInputCallback callback) override;
  void NoOutput(int32_t value, NoOutputCallback callback) override;
  void NoInOut(NoInOutCallback callback) override;
  void HaveInOut(int32_t value1, int32_t value2, HaveInOutCallback callback) override;
};
class  TestSyncCodeGenerationAsyncWaiter {
 public:
  explicit TestSyncCodeGenerationAsyncWaiter(TestSyncCodeGeneration* proxy);
  ~TestSyncCodeGenerationAsyncWaiter();
  void NoInput(
      int32_t* out_result);
  void NoOutput(
      int32_t value);
  void NoInOut(
      );
  void HaveInOut(
      int32_t value1, int32_t value2, int32_t* out_result1, int32_t* out_result2);

 private:
  TestSyncCodeGeneration* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(TestSyncCodeGenerationAsyncWaiter);
};


class  TestSyncInterceptorForTesting : public TestSync {
  virtual TestSync* GetForwardingInterface() = 0;
  void Ping(PingCallback callback) override;
  void Echo(int32_t value, EchoCallback callback) override;
  void AsyncEcho(int32_t value, AsyncEchoCallback callback) override;
};
class  TestSyncAsyncWaiter {
 public:
  explicit TestSyncAsyncWaiter(TestSync* proxy);
  ~TestSyncAsyncWaiter();
  void Ping(
      );
  void Echo(
      int32_t value, int32_t* out_result);
  void AsyncEcho(
      int32_t value, int32_t* out_result);

 private:
  TestSync* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(TestSyncAsyncWaiter);
};


class  TestSyncMasterInterceptorForTesting : public TestSyncMaster {
  virtual TestSyncMaster* GetForwardingInterface() = 0;
  void Ping(PingCallback callback) override;
  void Echo(int32_t value, EchoCallback callback) override;
  void AsyncEcho(int32_t value, AsyncEchoCallback callback) override;
  void SendRemote(mojo::PendingAssociatedRemote<TestSync> remote) override;
  void SendReceiver(mojo::PendingAssociatedReceiver<TestSync> receiver) override;
};
class  TestSyncMasterAsyncWaiter {
 public:
  explicit TestSyncMasterAsyncWaiter(TestSyncMaster* proxy);
  ~TestSyncMasterAsyncWaiter();
  void Ping(
      );
  void Echo(
      int32_t value, int32_t* out_result);
  void AsyncEcho(
      int32_t value, int32_t* out_result);

 private:
  TestSyncMaster* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(TestSyncMasterAsyncWaiter);
};




}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_SYNC_METHODS_MOJOM_TEST_UTILS_H_