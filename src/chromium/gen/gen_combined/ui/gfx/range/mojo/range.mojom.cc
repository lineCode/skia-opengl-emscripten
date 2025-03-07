// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4056)
#pragma warning(disable:4065)
#pragma warning(disable:4756)
#endif

#include "ui/gfx/range/mojo/range.mojom.h"

#include <math.h>
#include <stdint.h>
#include <utility>

#include "base/location.h"
#include "base/logging.h"
#include "base/run_loop.h"
#include "base/task/common/task_annotator.h"
#include "mojo/public/cpp/bindings/lib/message_internal.h"
#include "mojo/public/cpp/bindings/lib/serialization_util.h"
#include "mojo/public/cpp/bindings/lib/unserialized_message_context.h"
#include "mojo/public/cpp/bindings/lib/validate_params.h"
#include "mojo/public/cpp/bindings/lib/validation_context.h"
#include "mojo/public/cpp/bindings/lib/validation_errors.h"
#include "mojo/public/interfaces/bindings/interface_control_messages.mojom.h"

#include "ui/gfx/range/mojo/range.mojom-params-data.h"
#include "ui/gfx/range/mojo/range.mojom-shared-message-ids.h"

#include "ui/gfx/range/mojo/range.mojom-import-headers.h"


#ifndef UI_GFX_RANGE_MOJO_RANGE_MOJOM_JUMBO_H_
#define UI_GFX_RANGE_MOJO_RANGE_MOJOM_JUMBO_H_
#endif
namespace gfx {
namespace mojom {
Range::Range()
    : start(),
      end() {}

Range::Range(
    uint32_t start_in,
    uint32_t end_in)
    : start(std::move(start_in)),
      end(std::move(end_in)) {}

Range::~Range() = default;

bool Range::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
RangeF::RangeF()
    : start(),
      end() {}

RangeF::RangeF(
    float start_in,
    float end_in)
    : start(std::move(start_in)),
      end(std::move(end_in)) {}

RangeF::~RangeF() = default;

bool RangeF::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
}  // namespace mojom
}  // namespace gfx

namespace mojo {


// static
bool StructTraits<::gfx::mojom::Range::DataView, ::gfx::mojom::RangePtr>::Read(
    ::gfx::mojom::Range::DataView input,
    ::gfx::mojom::RangePtr* output) {
  bool success = true;
  ::gfx::mojom::RangePtr result(::gfx::mojom::Range::New());
  
      result->start = input.start();
      result->end = input.end();
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::gfx::mojom::RangeF::DataView, ::gfx::mojom::RangeFPtr>::Read(
    ::gfx::mojom::RangeF::DataView input,
    ::gfx::mojom::RangeFPtr* output) {
  bool success = true;
  ::gfx::mojom::RangeFPtr result(::gfx::mojom::RangeF::New());
  
      result->start = input.start();
      result->end = input.end();
  *output = std::move(result);
  return success;
}

}  // namespace mojo

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif