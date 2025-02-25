// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_ECHO_MOJOM_SHARED_MESSAGE_IDS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_ECHO_MOJOM_SHARED_MESSAGE_IDS_H_

#include <stdint.h>
namespace test {
namespace echo {
namespace mojom {

namespace internal {


// The 1119155912 value is based on sha256(salt + "Echo1").
constexpr uint32_t kEcho_EchoPoint_Name = 1119155912;

}  // namespace internal
}  // namespace mojom
}  // namespace echo
}  // namespace test

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_ECHO_MOJOM_SHARED_MESSAGE_IDS_H_