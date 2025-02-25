// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4065)
#endif

#include "mojo/public/interfaces/bindings/tests/test_structs.mojom-shared.h"

#include <utility>

#include "base/logging.h"
#include "base/stl_util.h"  // for base::size()
#include "mojo/public/cpp/bindings/lib/validate_params.h"
#include "mojo/public/cpp/bindings/lib/validation_context.h"
#include "mojo/public/cpp/bindings/lib/validation_errors.h"
#include "mojo/public/cpp/bindings/lib/validation_util.h"

#include "mojo/public/interfaces/bindings/tests/test_structs.mojom-params-data.h"
namespace mojo {
namespace test {

std::ostream& operator<<(std::ostream& os, ScopedConstants_EType value) {
  switch(value) {
    case ScopedConstants_EType::E0:
      return os << "ScopedConstants_EType::E0";
    case ScopedConstants_EType::E1:
      return os << "ScopedConstants_EType::E1";
    case ScopedConstants_EType::E2:
      return os << "ScopedConstants_EType::{E2, E3}";
    case ScopedConstants_EType::E4:
      return os << "ScopedConstants_EType::E4";
    default:
      return os << "Unknown ScopedConstants_EType value: " << static_cast<int32_t>(value);
  }
}

namespace internal {


// static
bool NamedRegion_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const NamedRegion_Data* object = static_cast<const NamedRegion_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 24 }};

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

  const mojo::internal::ContainerValidateParams name_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->name, validation_context,
                                         &name_validate_params)) {
    return false;
  }

  const mojo::internal::ContainerValidateParams rects_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->rects, validation_context,
                                         &rects_validate_params)) {
    return false;
  }

  return true;
}

NamedRegion_Data::NamedRegion_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool RectPair_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const RectPair_Data* object = static_cast<const RectPair_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 24 }};

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

  if (!mojo::internal::ValidateStruct(object->first, validation_context))
    return false;

  if (!mojo::internal::ValidateStruct(object->second, validation_context))
    return false;

  return true;
}

RectPair_Data::RectPair_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool EmptyStruct_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const EmptyStruct_Data* object = static_cast<const EmptyStruct_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 8 }};

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

  return true;
}

EmptyStruct_Data::EmptyStruct_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool NoDefaultFieldValues_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const NoDefaultFieldValues_Data* object = static_cast<const NoDefaultFieldValues_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 160 }};

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
          object->f11, 12, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f11_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f11, validation_context,
                                         &f11_validate_params)) {
    return false;
  }

  const mojo::internal::ContainerValidateParams f12_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f12, validation_context,
                                         &f12_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidateHandleOrInterfaceNonNullable(
          object->f13, 14, validation_context)) {
    return false;
  }
  if (!mojo::internal::ValidateHandleOrInterface(object->f13,
                                                 validation_context)) {
    return false;
  }

  if (!mojo::internal::ValidateHandleOrInterfaceNonNullable(
          object->f14, 15, validation_context)) {
    return false;
  }
  if (!mojo::internal::ValidateHandleOrInterface(object->f14,
                                                 validation_context)) {
    return false;
  }

  if (!mojo::internal::ValidateHandleOrInterfaceNonNullable(
          object->f15, 16, validation_context)) {
    return false;
  }
  if (!mojo::internal::ValidateHandleOrInterface(object->f15,
                                                 validation_context)) {
    return false;
  }

  if (!mojo::internal::ValidateHandleOrInterface(object->f16,
                                                 validation_context)) {
    return false;
  }

  if (!mojo::internal::ValidateHandleOrInterface(object->f17,
                                                 validation_context)) {
    return false;
  }

  if (!mojo::internal::ValidateHandleOrInterface(object->f18,
                                                 validation_context)) {
    return false;
  }

  if (!mojo::internal::ValidateHandleOrInterfaceNonNullable(
          object->f19, 20, validation_context)) {
    return false;
  }
  if (!mojo::internal::ValidateHandleOrInterface(object->f19,
                                                 validation_context)) {
    return false;
  }

  if (!mojo::internal::ValidateHandleOrInterface(object->f20,
                                                 validation_context)) {
    return false;
  }

  if (!mojo::internal::ValidateHandleOrInterfaceNonNullable(
          object->f21, 22, validation_context)) {
    return false;
  }
  if (!mojo::internal::ValidateHandleOrInterface(object->f21,
                                                 validation_context)) {
    return false;
  }

  if (!mojo::internal::ValidateHandleOrInterface(object->f22,
                                                 validation_context)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f23, 24, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f23_validate_params(
      0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f23, validation_context,
                                         &f23_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f24, 25, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f24_validate_params(
      0, true, new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f24, validation_context,
                                         &f24_validate_params)) {
    return false;
  }

  const mojo::internal::ContainerValidateParams f25_validate_params(
      0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f25, validation_context,
                                         &f25_validate_params)) {
    return false;
  }

  const mojo::internal::ContainerValidateParams f26_validate_params(
      0, true, new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f26, validation_context,
                                         &f26_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f27, 28, validation_context)) {
    return false;
  }
  if (!mojo::internal::ValidateStruct(object->f27, validation_context))
    return false;

  if (!mojo::internal::ValidateStruct(object->f28, validation_context))
    return false;

  return true;
}

NoDefaultFieldValues_Data::NoDefaultFieldValues_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool DefaultFieldValues_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const DefaultFieldValues_Data* object = static_cast<const DefaultFieldValues_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 96 }};

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
          object->f13, 14, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f13_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f13, validation_context,
                                         &f13_validate_params)) {
    return false;
  }

  const mojo::internal::ContainerValidateParams f14_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f14, validation_context,
                                         &f14_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f15, 16, validation_context)) {
    return false;
  }
  if (!mojo::internal::ValidateStruct(object->f15, validation_context))
    return false;

  if (!mojo::internal::ValidateStruct(object->f16, validation_context))
    return false;

  return true;
}

DefaultFieldValues_Data::DefaultFieldValues_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool ScopedConstants_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const ScopedConstants_Data* object = static_cast<const ScopedConstants_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 40 }};

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


  if (!::mojo::test::internal::ScopedConstants_EType_Data
        ::Validate(object->f0, validation_context))
    return false;


  if (!::mojo::test::internal::ScopedConstants_EType_Data
        ::Validate(object->f1, validation_context))
    return false;


  if (!::mojo::test::internal::ScopedConstants_EType_Data
        ::Validate(object->f2, validation_context))
    return false;


  if (!::mojo::test::internal::ScopedConstants_EType_Data
        ::Validate(object->f3, validation_context))
    return false;


  if (!::mojo::test::internal::ScopedConstants_EType_Data
        ::Validate(object->f4, validation_context))
    return false;

  return true;
}

ScopedConstants_Data::ScopedConstants_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool MapKeyTypes_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const MapKeyTypes_Data* object = static_cast<const MapKeyTypes_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 96 }};

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
          object->f1, 1, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f1_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, nullptr), new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f1, validation_context,
                                         &f1_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f2, 2, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f2_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, nullptr), new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f2, validation_context,
                                         &f2_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f3, 3, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f3_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, nullptr), new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f3, validation_context,
                                         &f3_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f4, 4, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f4_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, nullptr), new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f4, validation_context,
                                         &f4_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f5, 5, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f5_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, nullptr), new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f5, validation_context,
                                         &f5_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f6, 6, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f6_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, nullptr), new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f6, validation_context,
                                         &f6_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f7, 7, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f7_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, nullptr), new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f7, validation_context,
                                         &f7_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f8, 8, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f8_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, nullptr), new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f8, validation_context,
                                         &f8_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f9, 9, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f9_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, nullptr), new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f9, validation_context,
                                         &f9_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f10, 10, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f10_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, nullptr), new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f10, validation_context,
                                         &f10_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f11, 11, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f11_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)));
  if (!mojo::internal::ValidateContainer(object->f11, validation_context,
                                         &f11_validate_params)) {
    return false;
  }

  return true;
}

MapKeyTypes_Data::MapKeyTypes_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool MapValueTypes_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const MapValueTypes_Data* object = static_cast<const MapValueTypes_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 104 }};

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
          object->f0, 1, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f0_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr))));
  if (!mojo::internal::ValidateContainer(object->f0, validation_context,
                                         &f0_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f1, 2, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f1_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, true, new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr))));
  if (!mojo::internal::ValidateContainer(object->f1, validation_context,
                                         &f1_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f2, 3, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f2_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, true, new mojo::internal::ContainerValidateParams(0, false, nullptr))));
  if (!mojo::internal::ValidateContainer(object->f2, validation_context,
                                         &f2_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f3, 4, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f3_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(2, false, new mojo::internal::ContainerValidateParams(0, false, nullptr))));
  if (!mojo::internal::ValidateContainer(object->f3, validation_context,
                                         &f3_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f4, 5, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f4_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, true, new mojo::internal::ContainerValidateParams(2, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)))));
  if (!mojo::internal::ValidateContainer(object->f4, validation_context,
                                         &f4_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f5, 6, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f5_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(1, false, new mojo::internal::ContainerValidateParams(2, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)))));
  if (!mojo::internal::ValidateContainer(object->f5, validation_context,
                                         &f5_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f6, 7, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f6_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, true, nullptr));
  if (!mojo::internal::ValidateContainer(object->f6, validation_context,
                                         &f6_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f7, 8, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f7_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)))));
  if (!mojo::internal::ValidateContainer(object->f7, validation_context,
                                         &f7_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f8, 9, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f8_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr))))));
  if (!mojo::internal::ValidateContainer(object->f8, validation_context,
                                         &f8_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f9, 10, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f9_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f9, validation_context,
                                         &f9_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f10, 11, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f10_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)));
  if (!mojo::internal::ValidateContainer(object->f10, validation_context,
                                         &f10_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f11, 12, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f11_validate_params(
      new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(new mojo::internal::ContainerValidateParams(0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr)), new mojo::internal::ContainerValidateParams(0, false, nullptr))));
  if (!mojo::internal::ValidateContainer(object->f11, validation_context,
                                         &f11_validate_params)) {
    return false;
  }

  return true;
}

MapValueTypes_Data::MapValueTypes_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool ArrayValueTypes_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const ArrayValueTypes_Data* object = static_cast<const ArrayValueTypes_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 72 }};

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
          object->f0, 1, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f0_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f0, validation_context,
                                         &f0_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f1, 2, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f1_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f1, validation_context,
                                         &f1_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f2, 3, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f2_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f2, validation_context,
                                         &f2_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f3, 4, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f3_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f3, validation_context,
                                         &f3_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f4, 5, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f4_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f4, validation_context,
                                         &f4_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f5, 6, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f5_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f5, validation_context,
                                         &f5_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f6, 7, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f6_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f6, validation_context,
                                         &f6_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f7, 8, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f7_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f7, validation_context,
                                         &f7_validate_params)) {
    return false;
  }

  return true;
}

ArrayValueTypes_Data::ArrayValueTypes_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool FloatNumberValues_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const FloatNumberValues_Data* object = static_cast<const FloatNumberValues_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 72 }};

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

  return true;
}

FloatNumberValues_Data::FloatNumberValues_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool IntegerNumberValues_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const IntegerNumberValues_Data* object = static_cast<const IntegerNumberValues_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 88 }};

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

  return true;
}

IntegerNumberValues_Data::IntegerNumberValues_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool UnsignedNumberValues_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const UnsignedNumberValues_Data* object = static_cast<const UnsignedNumberValues_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 56 }};

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

  return true;
}

UnsignedNumberValues_Data::UnsignedNumberValues_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool BitArrayValues_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const BitArrayValues_Data* object = static_cast<const BitArrayValues_Data*>(data);

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
          object->f0, 1, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f0_validate_params(
      1, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f0, validation_context,
                                         &f0_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f1, 2, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f1_validate_params(
      7, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f1, validation_context,
                                         &f1_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f2, 3, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f2_validate_params(
      9, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f2, validation_context,
                                         &f2_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f3, 4, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f3_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f3, validation_context,
                                         &f3_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f4, 5, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f4_validate_params(
      0, false, new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f4, validation_context,
                                         &f4_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f5, 6, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f5_validate_params(
      0, true, new mojo::internal::ContainerValidateParams(0, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f5, validation_context,
                                         &f5_validate_params)) {
    return false;
  }

  if (!mojo::internal::ValidatePointerNonNullable(
          object->f6, 7, validation_context)) {
    return false;
  }
  const mojo::internal::ContainerValidateParams f6_validate_params(
      0, true, new mojo::internal::ContainerValidateParams(2, false, nullptr));
  if (!mojo::internal::ValidateContainer(object->f6, validation_context,
                                         &f6_validate_params)) {
    return false;
  }

  return true;
}

BitArrayValues_Data::BitArrayValues_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool MultiVersionStruct_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const MultiVersionStruct_Data* object = static_cast<const MultiVersionStruct_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 16 }, { 1, 24 }, { 3, 32 }, { 5, 40 }, { 7, 48 }, { 9, 48 }};

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
  if (object->header_.version < 1)
    return true;

  if (!mojo::internal::ValidateStruct(object->f_rect, validation_context))
    return false;
  if (object->header_.version < 3)
    return true;

  const mojo::internal::ContainerValidateParams f_string_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f_string, validation_context,
                                         &f_string_validate_params)) {
    return false;
  }
  if (object->header_.version < 5)
    return true;

  const mojo::internal::ContainerValidateParams f_array_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f_array, validation_context,
                                         &f_array_validate_params)) {
    return false;
  }
  if (object->header_.version < 7)
    return true;

  if (!mojo::internal::ValidateHandleOrInterface(object->f_message_pipe,
                                                 validation_context)) {
    return false;
  }

  return true;
}

MultiVersionStruct_Data::MultiVersionStruct_Data()
    : header_({sizeof(*this), 9}) {}


// static
bool MultiVersionStructV0_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const MultiVersionStructV0_Data* object = static_cast<const MultiVersionStructV0_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 16 }};

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

  return true;
}

MultiVersionStructV0_Data::MultiVersionStructV0_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool MultiVersionStructV1_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const MultiVersionStructV1_Data* object = static_cast<const MultiVersionStructV1_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 16 }, { 1, 24 }};

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
  if (object->header_.version < 1)
    return true;

  if (!mojo::internal::ValidateStruct(object->f_rect, validation_context))
    return false;

  return true;
}

MultiVersionStructV1_Data::MultiVersionStructV1_Data()
    : header_({sizeof(*this), 1}) {}


// static
bool MultiVersionStructV3_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const MultiVersionStructV3_Data* object = static_cast<const MultiVersionStructV3_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 16 }, { 1, 24 }, { 3, 32 }};

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
  if (object->header_.version < 1)
    return true;

  if (!mojo::internal::ValidateStruct(object->f_rect, validation_context))
    return false;
  if (object->header_.version < 3)
    return true;

  const mojo::internal::ContainerValidateParams f_string_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f_string, validation_context,
                                         &f_string_validate_params)) {
    return false;
  }

  return true;
}

MultiVersionStructV3_Data::MultiVersionStructV3_Data()
    : header_({sizeof(*this), 3}) {}


// static
bool MultiVersionStructV5_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const MultiVersionStructV5_Data* object = static_cast<const MultiVersionStructV5_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 16 }, { 1, 24 }, { 3, 32 }, { 5, 40 }};

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
  if (object->header_.version < 1)
    return true;

  if (!mojo::internal::ValidateStruct(object->f_rect, validation_context))
    return false;
  if (object->header_.version < 3)
    return true;

  const mojo::internal::ContainerValidateParams f_string_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f_string, validation_context,
                                         &f_string_validate_params)) {
    return false;
  }
  if (object->header_.version < 5)
    return true;

  const mojo::internal::ContainerValidateParams f_array_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f_array, validation_context,
                                         &f_array_validate_params)) {
    return false;
  }

  return true;
}

MultiVersionStructV5_Data::MultiVersionStructV5_Data()
    : header_({sizeof(*this), 5}) {}


// static
bool MultiVersionStructV7_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const MultiVersionStructV7_Data* object = static_cast<const MultiVersionStructV7_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 16 }, { 1, 24 }, { 3, 32 }, { 5, 40 }, { 7, 48 }};

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
  if (object->header_.version < 1)
    return true;

  if (!mojo::internal::ValidateStruct(object->f_rect, validation_context))
    return false;
  if (object->header_.version < 3)
    return true;

  const mojo::internal::ContainerValidateParams f_string_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f_string, validation_context,
                                         &f_string_validate_params)) {
    return false;
  }
  if (object->header_.version < 5)
    return true;

  const mojo::internal::ContainerValidateParams f_array_validate_params(
      0, false, nullptr);
  if (!mojo::internal::ValidateContainer(object->f_array, validation_context,
                                         &f_array_validate_params)) {
    return false;
  }
  if (object->header_.version < 7)
    return true;

  if (!mojo::internal::ValidateHandleOrInterface(object->f_message_pipe,
                                                 validation_context)) {
    return false;
  }

  return true;
}

MultiVersionStructV7_Data::MultiVersionStructV7_Data()
    : header_({sizeof(*this), 7}) {}


// static
bool ReorderedStruct_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const ReorderedStruct_Data* object = static_cast<const ReorderedStruct_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 8 }, { 1, 16 }, { 2, 16 }, { 4, 24 }};

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

  return true;
}

ReorderedStruct_Data::ReorderedStruct_Data()
    : header_({sizeof(*this), 4}) {}


// static
bool ContainsInterface_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const ContainsInterface_Data* object = static_cast<const ContainsInterface_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 16 }};

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

  if (!mojo::internal::ValidateHandleOrInterfaceNonNullable(
          object->some_interface, 1, validation_context)) {
    return false;
  }
  if (!mojo::internal::ValidateHandleOrInterface(object->some_interface,
                                                 validation_context)) {
    return false;
  }

  return true;
}

ContainsInterface_Data::ContainsInterface_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool ContainsOther_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const ContainsOther_Data* object = static_cast<const ContainsOther_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 16 }};

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

  return true;
}

ContainsOther_Data::ContainsOther_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool ContainsInterfaceRequest_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const ContainsInterfaceRequest_Data* object = static_cast<const ContainsInterfaceRequest_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 16 }};

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

  if (!mojo::internal::ValidateHandleOrInterfaceNonNullable(
          object->request, 1, validation_context)) {
    return false;
  }
  if (!mojo::internal::ValidateHandleOrInterface(object->request,
                                                 validation_context)) {
    return false;
  }

  return true;
}

ContainsInterfaceRequest_Data::ContainsInterfaceRequest_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool SingleBoolStruct_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const SingleBoolStruct_Data* object = static_cast<const SingleBoolStruct_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 16 }};

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

  return true;
}

SingleBoolStruct_Data::SingleBoolStruct_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool ContainsHashable_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const ContainsHashable_Data* object = static_cast<const ContainsHashable_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 16 }};

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
          object->rect, 1, validation_context)) {
    return false;
  }
  if (!mojo::internal::ValidateStruct(object->rect, validation_context))
    return false;

  return true;
}

ContainsHashable_Data::ContainsHashable_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool SimpleNestedStruct_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const SimpleNestedStruct_Data* object = static_cast<const SimpleNestedStruct_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 16 }};

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
          object->nested, 1, validation_context)) {
    return false;
  }
  if (!mojo::internal::ValidateStruct(object->nested, validation_context))
    return false;

  return true;
}

SimpleNestedStruct_Data::SimpleNestedStruct_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool SomeInterface_SomeMethod_Params_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const SomeInterface_SomeMethod_Params_Data* object = static_cast<const SomeInterface_SomeMethod_Params_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 16 }};

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
          object->pair, 1, validation_context)) {
    return false;
  }
  if (!mojo::internal::ValidateStruct(object->pair, validation_context))
    return false;

  return true;
}

SomeInterface_SomeMethod_Params_Data::SomeInterface_SomeMethod_Params_Data()
    : header_({sizeof(*this), 0}) {}


// static
bool SomeInterface_SomeMethod_ResponseParams_Data::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  if (!data)
    return true;

  if (!ValidateStructHeaderAndClaimMemory(data, validation_context))
    return false;

  // NOTE: The memory backing |object| may be smaller than |sizeof(*object)| if
  // the message comes from an older version.
  const SomeInterface_SomeMethod_ResponseParams_Data* object = static_cast<const SomeInterface_SomeMethod_ResponseParams_Data*>(data);

  static constexpr struct {
    uint32_t version;
    uint32_t num_bytes;
  } kVersionSizes[] = {{ 0, 16 }};

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
          object->other_pair, 1, validation_context)) {
    return false;
  }
  if (!mojo::internal::ValidateStruct(object->other_pair, validation_context))
    return false;

  return true;
}

SomeInterface_SomeMethod_ResponseParams_Data::SomeInterface_SomeMethod_ResponseParams_Data()
    : header_({sizeof(*this), 0}) {}

}  // namespace internal
}  // namespace test
}  // namespace mojo

#if defined(_MSC_VER)
#pragma warning(pop)
#endif