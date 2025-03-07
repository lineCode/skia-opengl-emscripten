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

#include "mojo/public/mojom/base/values.mojom.h"

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

#include "mojo/public/mojom/base/values.mojom-params-data.h"
#include "mojo/public/mojom/base/values.mojom-shared-message-ids.h"

#include "mojo/public/mojom/base/values.mojom-import-headers.h"


#ifndef MOJO_PUBLIC_MOJOM_BASE_VALUES_MOJOM_JUMBO_H_
#define MOJO_PUBLIC_MOJOM_BASE_VALUES_MOJOM_JUMBO_H_
#include "mojo/public/cpp/base/values_mojom_traits.h"
#endif
namespace mojo_base {
namespace mojom {
DictionaryValue::DictionaryValue()
    : storage() {}

DictionaryValue::DictionaryValue(
    base::flat_map<std::string, base::Value> storage_in)
    : storage(std::move(storage_in)) {}

DictionaryValue::~DictionaryValue() = default;

bool DictionaryValue::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
ListValue::ListValue()
    : storage() {}

ListValue::ListValue(
    std::vector<base::Value> storage_in)
    : storage(std::move(storage_in)) {}

ListValue::~ListValue() = default;

bool ListValue::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
Value::Value() : tag_(Tag::NULL_VALUE) {
  data_.null_value = uint8_t();
}

Value::~Value() {
  DestroyActive();
}


void Value::set_null_value(
    uint8_t null_value) {
  if (tag_ != Tag::NULL_VALUE) {
    DestroyActive();
    tag_ = Tag::NULL_VALUE;
  }
  data_.null_value = null_value;
}
void Value::set_bool_value(
    bool bool_value) {
  if (tag_ != Tag::BOOL_VALUE) {
    DestroyActive();
    tag_ = Tag::BOOL_VALUE;
  }
  data_.bool_value = bool_value;
}
void Value::set_int_value(
    int32_t int_value) {
  if (tag_ != Tag::INT_VALUE) {
    DestroyActive();
    tag_ = Tag::INT_VALUE;
  }
  data_.int_value = int_value;
}
void Value::set_double_value(
    double double_value) {
  if (tag_ != Tag::DOUBLE_VALUE) {
    DestroyActive();
    tag_ = Tag::DOUBLE_VALUE;
  }
  data_.double_value = double_value;
}
void Value::set_string_value(
    const std::string& string_value) {
  if (tag_ == Tag::STRING_VALUE) {
    *(data_.string_value) = std::move(string_value);
  } else {
    DestroyActive();
    tag_ = Tag::STRING_VALUE;
    data_.string_value = new std::string(
        std::move(string_value));
  }
}
void Value::set_binary_value(
    const std::vector<uint8_t>& binary_value) {
  if (tag_ == Tag::BINARY_VALUE) {
    *(data_.binary_value) = std::move(binary_value);
  } else {
    DestroyActive();
    tag_ = Tag::BINARY_VALUE;
    data_.binary_value = new std::vector<uint8_t>(
        std::move(binary_value));
  }
}
void Value::set_dictionary_value(
    base::Value dictionary_value) {
  if (tag_ == Tag::DICTIONARY_VALUE) {
    *(data_.dictionary_value) = std::move(dictionary_value);
  } else {
    DestroyActive();
    tag_ = Tag::DICTIONARY_VALUE;
    data_.dictionary_value = new base::Value(
        std::move(dictionary_value));
  }
}
void Value::set_list_value(
    base::Value list_value) {
  if (tag_ == Tag::LIST_VALUE) {
    *(data_.list_value) = std::move(list_value);
  } else {
    DestroyActive();
    tag_ = Tag::LIST_VALUE;
    data_.list_value = new base::Value(
        std::move(list_value));
  }
}

void Value::DestroyActive() {
  switch (tag_) {

    case Tag::NULL_VALUE:

      break;
    case Tag::BOOL_VALUE:

      break;
    case Tag::INT_VALUE:

      break;
    case Tag::DOUBLE_VALUE:

      break;
    case Tag::STRING_VALUE:

      delete data_.string_value;
      break;
    case Tag::BINARY_VALUE:

      delete data_.binary_value;
      break;
    case Tag::DICTIONARY_VALUE:

      delete data_.dictionary_value;
      break;
    case Tag::LIST_VALUE:

      delete data_.list_value;
      break;
  }
}

bool Value::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context, false);
}
}  // namespace mojom
}  // namespace mojo_base

namespace mojo {


// static
bool StructTraits<::mojo_base::mojom::DictionaryValue::DataView, ::mojo_base::mojom::DictionaryValuePtr>::Read(
    ::mojo_base::mojom::DictionaryValue::DataView input,
    ::mojo_base::mojom::DictionaryValuePtr* output) {
  bool success = true;
  ::mojo_base::mojom::DictionaryValuePtr result(::mojo_base::mojom::DictionaryValue::New());
  
      if (!input.ReadStorage(&result->storage))
        success = false;
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo_base::mojom::ListValue::DataView, ::mojo_base::mojom::ListValuePtr>::Read(
    ::mojo_base::mojom::ListValue::DataView input,
    ::mojo_base::mojom::ListValuePtr* output) {
  bool success = true;
  ::mojo_base::mojom::ListValuePtr result(::mojo_base::mojom::ListValue::New());
  
      if (!input.ReadStorage(&result->storage))
        success = false;
  *output = std::move(result);
  return success;
}

// static
bool UnionTraits<::mojo_base::mojom::Value::DataView, ::mojo_base::mojom::ValuePtr>::Read(
    ::mojo_base::mojom::Value::DataView input,
    ::mojo_base::mojom::ValuePtr* output) {
  using UnionType = ::mojo_base::mojom::Value;
  using Tag = UnionType::Tag;

  switch (input.tag()) {
    case Tag::NULL_VALUE: {
      *output = UnionType::NewNullValue(input.null_value());
      break;
    }
    case Tag::BOOL_VALUE: {
      *output = UnionType::NewBoolValue(input.bool_value());
      break;
    }
    case Tag::INT_VALUE: {
      *output = UnionType::NewIntValue(input.int_value());
      break;
    }
    case Tag::DOUBLE_VALUE: {
      *output = UnionType::NewDoubleValue(input.double_value());
      break;
    }
    case Tag::STRING_VALUE: {
      std::string result_string_value;
      if (!input.ReadStringValue(&result_string_value))
        return false;

      *output = UnionType::NewStringValue(
          std::move(result_string_value));
      break;
    }
    case Tag::BINARY_VALUE: {
      std::vector<uint8_t> result_binary_value;
      if (!input.ReadBinaryValue(&result_binary_value))
        return false;

      *output = UnionType::NewBinaryValue(
          std::move(result_binary_value));
      break;
    }
    case Tag::DICTIONARY_VALUE: {
      base::Value result_dictionary_value;
      if (!input.ReadDictionaryValue(&result_dictionary_value))
        return false;

      *output = UnionType::NewDictionaryValue(
          std::move(result_dictionary_value));
      break;
    }
    case Tag::LIST_VALUE: {
      base::Value result_list_value;
      if (!input.ReadListValue(&result_list_value))
        return false;

      *output = UnionType::NewListValue(
          std::move(result_list_value));
      break;
    }
    default:
      return false;
  }
  return true;
}

}  // namespace mojo

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif