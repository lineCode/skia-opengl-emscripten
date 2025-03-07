// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4065)
#endif

#include "third_party/blink/public/mojom/messaging/transferable_message.mojom-shared.h"

#include <utility>

#include "base/logging.h"
#include "base/stl_util.h"  // for base::size()
#include "mojo/public/cpp/bindings/lib/validate_params.h"
#include "mojo/public/cpp/bindings/lib/validation_context.h"
#include "mojo/public/cpp/bindings/lib/validation_errors.h"
#include "mojo/public/cpp/bindings/lib/validation_util.h"

#include "third_party/blink/public/mojom/messaging/transferable_message.mojom-params-data.h"
namespace blink {
namespace mojom {

namespace internal {


// static
bool TransferableMessage_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const TransferableMessage_Data* object = static_cast<const TransferableMessage_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 64 }};

  if (object->header_.version <=
          kVersionSizes[base::size(kVersionSizes) - 1].version) {
    // Scan in reverse order to optimize for more recent versions.
    for (int i = base::size(kVersionSizes) - 1; i >= 0; --i) {
      if (object->header_.version >= kVersionSizes[i].version) {
        if (object->header_.num_bytes == kVersionSizes[i].num_bytes)
          break;

        ReportValidationError(
            validation_context,
            mojo::internal::VALIDATION_ERROR_UNEXPECTED_STRUCT_HEADER);
        return false;
      }
    }
  } else if (object->header_.num_bytes <
                 kVersionSizes[base::size(kVersionSizes) - 1].num_bytes) {
    ReportValidationError(
        validation_context,
        mojo::internal::VALIDATION_ERROR_UNEXPECTED_STRUCT_HEADER);
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->message, 1, validation_context)) {
    return false;
  }
  if (!mojo::internal::ValidateStruct(object->message, validation_context))
    return false;

  if (!mojo::internal::ValidatePointerNonNullable(
          object->ports, 2, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams ports_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->ports, validation_context,
                                         &ports_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->stream_channels, 3, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams stream_channels_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->stream_channels, validation_context,
                                         &stream_channels_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->array_buffer_contents_array, 4, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams array_buffer_contents_array_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->array_buffer_contents_array, validation_context,
                                         &array_buffer_contents_array_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->image_bitmap_contents_array, 5, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams image_bitmap_contents_array_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->image_bitmap_contents_array, validation_context,
                                         &image_bitmap_contents_array_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidateStruct(object->user_activation, validation_context))
    return false;

  return true;
}

TransferableMessage_Data::TransferableMessage_Data()
    : header_({sizeof(*this), 0}) {}

}  // namespace internal
}  // namespace mojom
}  // namespace blink

#if defined(_MSC_VER)
#pragma warning(pop)
#endif