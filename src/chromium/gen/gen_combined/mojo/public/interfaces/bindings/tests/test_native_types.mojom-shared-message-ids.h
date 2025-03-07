// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_NATIVE_TYPES_MOJOM_SHARED_MESSAGE_IDS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_NATIVE_TYPES_MOJOM_SHARED_MESSAGE_IDS_H_

#include <stdint.h>
namespace mojo {
namespace test {

namespace internal {


constexpr uint32_t kPicklePasser_PassPickledStruct_Name = 0;
constexpr uint32_t kPicklePasser_PassPickledEnum_Name = 1;
constexpr uint32_t kPicklePasser_PassPickleContainer_Name = 2;
constexpr uint32_t kPicklePasser_PassPickles_Name = 3;
constexpr uint32_t kPicklePasser_PassPickleArrays_Name = 4;
constexpr uint32_t kRectService_AddRect_Name = 0;
constexpr uint32_t kRectService_GetLargestRect_Name = 1;
constexpr uint32_t kRectService_PassSharedRect_Name = 2;
constexpr uint32_t kNativeTypeTester_PassNativeStruct_Name = 0;
constexpr uint32_t kNativeTypeTester_PassNativeStructWithAttachments_Name = 1;

}  // namespace internal
}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_NATIVE_TYPES_MOJOM_SHARED_MESSAGE_IDS_H_