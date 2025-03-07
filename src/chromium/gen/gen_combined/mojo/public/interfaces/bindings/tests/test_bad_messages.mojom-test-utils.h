// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_BAD_MESSAGES_MOJOM_TEST_UTILS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_BAD_MESSAGES_MOJOM_TEST_UTILS_H_

#include "mojo/public/interfaces/bindings/tests/test_bad_messages.mojom.h"


namespace mojo {
namespace test {


class  TestBadMessagesInterceptorForTesting : public TestBadMessages {
  virtual TestBadMessages* GetForwardingInterface() = 0;
  void RejectEventually(RejectEventuallyCallback callback) override;
  void RequestResponse(RequestResponseCallback callback) override;
  void RejectSync(RejectSyncCallback callback) override;
  void RequestResponseSync(RequestResponseSyncCallback callback) override;
};
class  TestBadMessagesAsyncWaiter {
 public:
  explicit TestBadMessagesAsyncWaiter(TestBadMessages* proxy);
  ~TestBadMessagesAsyncWaiter();
  void RejectEventually(
      );
  void RequestResponse(
      );
  void RejectSync(
      );
  void RequestResponseSync(
      );

 private:
  TestBadMessages* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(TestBadMessagesAsyncWaiter);
};




}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_BAD_MESSAGES_MOJOM_TEST_UTILS_H_