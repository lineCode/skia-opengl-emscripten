// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4065)
#endif

#include "gpu/ipc/common/context_result.mojom-shared.h"

#include <utility>

#include "base/logging.h"
#include "base/stl_util.h"  // for base::size()
#include "mojo/public/cpp/bindings/lib/validate_params.h"
#include "mojo/public/cpp/bindings/lib/validation_context.h"
#include "mojo/public/cpp/bindings/lib/validation_errors.h"
#include "mojo/public/cpp/bindings/lib/validation_util.h"

#include "gpu/ipc/common/context_result.mojom-params-data.h"
namespace gpu {
namespace mojom {

std::ostream& operator<<(std::ostream& os, ContextResult value) {
  switch(value) {
    case ContextResult::Success:
      return os << "ContextResult::Success";
    case ContextResult::TransientFailure:
      return os << "ContextResult::TransientFailure";
    case ContextResult::FatalFailure:
      return os << "ContextResult::FatalFailure";
    case ContextResult::SurfaceFailure:
      return os << "ContextResult::SurfaceFailure";
    default:
      return os << "Unknown ContextResult value: " << static_cast<int32_t>(value);
  }
}

namespace internal {

}  // namespace internal
}  // namespace mojom
}  // namespace gpu

#if defined(_MSC_VER)
#pragma warning(pop)
#endif