// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_MOJOM_BASE_TOKEN_MOJOM_FORWARD_H_
#define MOJO_PUBLIC_MOJOM_BASE_TOKEN_MOJOM_FORWARD_H_

#include "mojo/public/cpp/bindings/struct_ptr.h"


#include "mojo/public/cpp/bindings/lib/buffer.h"


#include "mojo/public/cpp/bindings/lib/native_enum_serialization.h"
#include "mojo/public/cpp/bindings/lib/native_struct_serialization.h"
#include "base/component_export.h"




namespace mojo_base {
namespace mojom {
class TokenDataView;

class Token;
using TokenPtr = mojo::InlinedStructPtr<Token>;




}  // namespace mojom
}  // namespace mojo_base
#include "base/token.h"

#endif  // MOJO_PUBLIC_MOJOM_BASE_TOKEN_MOJOM_FORWARD_H_