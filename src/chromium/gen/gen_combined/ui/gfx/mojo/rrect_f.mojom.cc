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

#include "ui/gfx/mojo/rrect_f.mojom.h"

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

#include "ui/gfx/mojo/rrect_f.mojom-params-data.h"
#include "ui/gfx/mojo/rrect_f.mojom-shared-message-ids.h"

#include "ui/gfx/mojo/rrect_f.mojom-import-headers.h"


#ifndef UI_GFX_MOJO_RRECT_F_MOJOM_JUMBO_H_
#define UI_GFX_MOJO_RRECT_F_MOJOM_JUMBO_H_
#include "ui/gfx/geometry/mojo/geometry_struct_traits.h"
#endif
namespace gfx {
namespace mojom {
RRectF::RRectF()
    : type(),
      rect(),
      upper_left(),
      upper_right(),
      lower_right(),
      lower_left() {}

RRectF::RRectF(
    RRectFType type_in,
    const gfx::RectF& rect_in,
    const gfx::Vector2dF& upper_left_in,
    const gfx::Vector2dF& upper_right_in,
    const gfx::Vector2dF& lower_right_in,
    const gfx::Vector2dF& lower_left_in)
    : type(std::move(type_in)),
      rect(std::move(rect_in)),
      upper_left(std::move(upper_left_in)),
      upper_right(std::move(upper_right_in)),
      lower_right(std::move(lower_right_in)),
      lower_left(std::move(lower_left_in)) {}

RRectF::~RRectF() = default;

bool RRectF::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
}  // namespace mojom
}  // namespace gfx

namespace mojo {


// static
bool StructTraits<::gfx::mojom::RRectF::DataView, ::gfx::mojom::RRectFPtr>::Read(
    ::gfx::mojom::RRectF::DataView input,
    ::gfx::mojom::RRectFPtr* output) {
  bool success = true;
  ::gfx::mojom::RRectFPtr result(::gfx::mojom::RRectF::New());
  
      if (!input.ReadType(&result->type))
        success = false;
      if (!input.ReadRect(&result->rect))
        success = false;
      if (!input.ReadUpperLeft(&result->upper_left))
        success = false;
      if (!input.ReadUpperRight(&result->upper_right))
        success = false;
      if (!input.ReadLowerRight(&result->lower_right))
        success = false;
      if (!input.ReadLowerLeft(&result->lower_left))
        success = false;
  *output = std::move(result);
  return success;
}

}  // namespace mojo

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif