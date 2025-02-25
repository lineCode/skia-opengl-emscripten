// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_FACTORY_MOJOM_SHARED_MESSAGE_IDS_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_FACTORY_MOJOM_SHARED_MESSAGE_IDS_H_

#include <stdint.h>
namespace sample {

namespace internal {


constexpr uint32_t kNamedObject_SetName_Name = 0;
constexpr uint32_t kNamedObject_GetName_Name = 1;
constexpr uint32_t kFactory_DoStuff_Name = 0;
constexpr uint32_t kFactory_DoStuff2_Name = 1;
constexpr uint32_t kFactory_CreateNamedObject_Name = 2;
constexpr uint32_t kFactory_RequestImportedInterface_Name = 3;
constexpr uint32_t kFactory_TakeImportedInterface_Name = 4;

}  // namespace internal
}  // namespace sample

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_FACTORY_MOJOM_SHARED_MESSAGE_IDS_H_