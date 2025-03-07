// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_BAD_MESSAGES_MOJOM_SHARED_MESSAGE_IDS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_BAD_MESSAGES_MOJOM_SHARED_MESSAGE_IDS_H_

#include <stdint.h>
namespace mojo {
namespace test {

namespace internal {


constexpr uint32_t kTestBadMessages_RejectEventually_Name = 0;
constexpr uint32_t kTestBadMessages_RequestResponse_Name = 1;
constexpr uint32_t kTestBadMessages_RejectSync_Name = 2;
constexpr uint32_t kTestBadMessages_RequestResponseSync_Name = 3;

}  // namespace internal
}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_BAD_MESSAGES_MOJOM_SHARED_MESSAGE_IDS_H_