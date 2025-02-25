// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_REGRESSION_TESTS_MOJOM_SHARED_MESSAGE_IDS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_REGRESSION_TESTS_MOJOM_SHARED_MESSAGE_IDS_H_

#include <stdint.h>
namespace regression_tests {

namespace internal {


constexpr uint32_t kCheckMethodWithEmptyResponse_WithouParameterAndEmptyResponse_Name = 0;
constexpr uint32_t kCheckMethodWithEmptyResponse_WithParameterAndEmptyResponse_Name = 1;
constexpr uint32_t kCheckNameCollision_WithNameCollision_Name = 0;
constexpr uint32_t kHandlesNameCollisionInterface_Method_Name = 0;

}  // namespace internal
}  // namespace regression_tests

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_REGRESSION_TESTS_MOJOM_SHARED_MESSAGE_IDS_H_