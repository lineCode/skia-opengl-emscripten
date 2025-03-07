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

#include "mojo/public/interfaces/bindings/tests/test_unions.mojom-blink.h"

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

#include "mojo/public/interfaces/bindings/tests/test_unions.mojom-params-data.h"
#include "mojo/public/interfaces/bindings/tests/test_unions.mojom-shared-message-ids.h"

#include "mojo/public/interfaces/bindings/tests/test_unions.mojom-blink-import-headers.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"


#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_UNIONS_MOJOM_BLINK_JUMBO_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_UNIONS_MOJOM_BLINK_JUMBO_H_
#endif
namespace mojo {
namespace test {
namespace blink {
WrapperStruct::WrapperStruct()
    : object_union(),
      pod_union(),
      handle_union() {}

WrapperStruct::WrapperStruct(
    ObjectUnionPtr object_union_in,
    PodUnionPtr pod_union_in,
    HandleUnionPtr handle_union_in)
    : object_union(std::move(object_union_in)),
      pod_union(std::move(pod_union_in)),
      handle_union(std::move(handle_union_in)) {}

WrapperStruct::~WrapperStruct() = default;

bool WrapperStruct::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
DummyStruct::DummyStruct()
    : f_int8() {}

DummyStruct::DummyStruct(
    int8_t f_int8_in)
    : f_int8(std::move(f_int8_in)) {}

DummyStruct::~DummyStruct() = default;
size_t DummyStruct::Hash(size_t seed) const {
  seed = mojo::internal::WTFHash(seed, this->f_int8);
  return seed;
}

bool DummyStruct::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
SmallStruct::SmallStruct()
    : dummy_struct(),
      pod_union(),
      pod_union_array(),
      nullable_pod_union_array(),
      s_array(),
      pod_union_map(),
      nullable_pod_union_map() {}

SmallStruct::SmallStruct(
    DummyStructPtr dummy_struct_in,
    PodUnionPtr pod_union_in,
    base::Optional<WTF::Vector<PodUnionPtr>> pod_union_array_in,
    base::Optional<WTF::Vector<PodUnionPtr>> nullable_pod_union_array_in,
    base::Optional<WTF::Vector<DummyStructPtr>> s_array_in,
    base::Optional<WTF::HashMap<WTF::String, PodUnionPtr>> pod_union_map_in,
    base::Optional<WTF::HashMap<WTF::String, PodUnionPtr>> nullable_pod_union_map_in)
    : dummy_struct(std::move(dummy_struct_in)),
      pod_union(std::move(pod_union_in)),
      pod_union_array(std::move(pod_union_array_in)),
      nullable_pod_union_array(std::move(nullable_pod_union_array_in)),
      s_array(std::move(s_array_in)),
      pod_union_map(std::move(pod_union_map_in)),
      nullable_pod_union_map(std::move(nullable_pod_union_map_in)) {}

SmallStruct::~SmallStruct() = default;

bool SmallStruct::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
SmallStructNonNullableUnion::SmallStructNonNullableUnion()
    : pod_union() {}

SmallStructNonNullableUnion::SmallStructNonNullableUnion(
    PodUnionPtr pod_union_in)
    : pod_union(std::move(pod_union_in)) {}

SmallStructNonNullableUnion::~SmallStructNonNullableUnion() = default;
size_t SmallStructNonNullableUnion::Hash(size_t seed) const {
  seed = mojo::internal::WTFHash(seed, this->pod_union);
  return seed;
}

bool SmallStructNonNullableUnion::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
SmallObjStruct::SmallObjStruct()
    : obj_union(),
      f_int8() {}

SmallObjStruct::SmallObjStruct(
    ObjectUnionPtr obj_union_in,
    int8_t f_int8_in)
    : obj_union(std::move(obj_union_in)),
      f_int8(std::move(f_int8_in)) {}

SmallObjStruct::~SmallObjStruct() = default;

bool SmallObjStruct::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
TryNonNullStruct::TryNonNullStruct()
    : nullable(),
      non_nullable() {}

TryNonNullStruct::TryNonNullStruct(
    DummyStructPtr nullable_in,
    DummyStructPtr non_nullable_in)
    : nullable(std::move(nullable_in)),
      non_nullable(std::move(non_nullable_in)) {}

TryNonNullStruct::~TryNonNullStruct() = default;

bool TryNonNullStruct::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
ImportedUnionStruct::ImportedUnionStruct()
    : point_or_shape() {}

ImportedUnionStruct::ImportedUnionStruct(
    ::imported::blink::PointOrShapePtr point_or_shape_in)
    : point_or_shape(std::move(point_or_shape_in)) {}

ImportedUnionStruct::~ImportedUnionStruct() = default;
size_t ImportedUnionStruct::Hash(size_t seed) const {
  seed = mojo::internal::WTFHash(seed, this->point_or_shape);
  return seed;
}

bool ImportedUnionStruct::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
PodUnion::PodUnion() : tag_(Tag::F_INT8) {
  data_.f_int8 = int8_t();
}

PodUnion::~PodUnion() {
  DestroyActive();
}


void PodUnion::set_f_int8(
    int8_t f_int8) {
  if (tag_ != Tag::F_INT8) {
    DestroyActive();
    tag_ = Tag::F_INT8;
  }
  data_.f_int8 = f_int8;
}
void PodUnion::set_f_int8_other(
    int8_t f_int8_other) {
  if (tag_ != Tag::F_INT8_OTHER) {
    DestroyActive();
    tag_ = Tag::F_INT8_OTHER;
  }
  data_.f_int8_other = f_int8_other;
}
void PodUnion::set_f_uint8(
    uint8_t f_uint8) {
  if (tag_ != Tag::F_UINT8) {
    DestroyActive();
    tag_ = Tag::F_UINT8;
  }
  data_.f_uint8 = f_uint8;
}
void PodUnion::set_f_int16(
    int16_t f_int16) {
  if (tag_ != Tag::F_INT16) {
    DestroyActive();
    tag_ = Tag::F_INT16;
  }
  data_.f_int16 = f_int16;
}
void PodUnion::set_f_uint16(
    uint16_t f_uint16) {
  if (tag_ != Tag::F_UINT16) {
    DestroyActive();
    tag_ = Tag::F_UINT16;
  }
  data_.f_uint16 = f_uint16;
}
void PodUnion::set_f_int32(
    int32_t f_int32) {
  if (tag_ != Tag::F_INT32) {
    DestroyActive();
    tag_ = Tag::F_INT32;
  }
  data_.f_int32 = f_int32;
}
void PodUnion::set_f_uint32(
    uint32_t f_uint32) {
  if (tag_ != Tag::F_UINT32) {
    DestroyActive();
    tag_ = Tag::F_UINT32;
  }
  data_.f_uint32 = f_uint32;
}
void PodUnion::set_f_int64(
    int64_t f_int64) {
  if (tag_ != Tag::F_INT64) {
    DestroyActive();
    tag_ = Tag::F_INT64;
  }
  data_.f_int64 = f_int64;
}
void PodUnion::set_f_uint64(
    uint64_t f_uint64) {
  if (tag_ != Tag::F_UINT64) {
    DestroyActive();
    tag_ = Tag::F_UINT64;
  }
  data_.f_uint64 = f_uint64;
}
void PodUnion::set_f_float(
    float f_float) {
  if (tag_ != Tag::F_FLOAT) {
    DestroyActive();
    tag_ = Tag::F_FLOAT;
  }
  data_.f_float = f_float;
}
void PodUnion::set_f_double(
    double f_double) {
  if (tag_ != Tag::F_DOUBLE) {
    DestroyActive();
    tag_ = Tag::F_DOUBLE;
  }
  data_.f_double = f_double;
}
void PodUnion::set_f_bool(
    bool f_bool) {
  if (tag_ != Tag::F_BOOL) {
    DestroyActive();
    tag_ = Tag::F_BOOL;
  }
  data_.f_bool = f_bool;
}
void PodUnion::set_f_enum(
    AnEnum f_enum) {
  if (tag_ != Tag::F_ENUM) {
    DestroyActive();
    tag_ = Tag::F_ENUM;
  }
  data_.f_enum = f_enum;
}
void PodUnion::set_f_extensible_enum(
    AnExtensibleEnum f_extensible_enum) {
  if (tag_ != Tag::F_EXTENSIBLE_ENUM) {
    DestroyActive();
    tag_ = Tag::F_EXTENSIBLE_ENUM;
  }
  data_.f_extensible_enum = f_extensible_enum;
}

void PodUnion::DestroyActive() {
  switch (tag_) {

    case Tag::F_INT8:

      break;
    case Tag::F_INT8_OTHER:

      break;
    case Tag::F_UINT8:

      break;
    case Tag::F_INT16:

      break;
    case Tag::F_UINT16:

      break;
    case Tag::F_INT32:

      break;
    case Tag::F_UINT32:

      break;
    case Tag::F_INT64:

      break;
    case Tag::F_UINT64:

      break;
    case Tag::F_FLOAT:

      break;
    case Tag::F_DOUBLE:

      break;
    case Tag::F_BOOL:

      break;
    case Tag::F_ENUM:

      break;
    case Tag::F_EXTENSIBLE_ENUM:

      break;
  }
}
size_t PodUnion::Hash(size_t seed) const {
  seed = mojo::internal::HashCombine(seed, static_cast<uint32_t>(tag_));
  switch (tag_) {

    case Tag::F_INT8:
      return mojo::internal::WTFHash(seed, data_.f_int8);
    case Tag::F_INT8_OTHER:
      return mojo::internal::WTFHash(seed, data_.f_int8_other);
    case Tag::F_UINT8:
      return mojo::internal::WTFHash(seed, data_.f_uint8);
    case Tag::F_INT16:
      return mojo::internal::WTFHash(seed, data_.f_int16);
    case Tag::F_UINT16:
      return mojo::internal::WTFHash(seed, data_.f_uint16);
    case Tag::F_INT32:
      return mojo::internal::WTFHash(seed, data_.f_int32);
    case Tag::F_UINT32:
      return mojo::internal::WTFHash(seed, data_.f_uint32);
    case Tag::F_INT64:
      return mojo::internal::WTFHash(seed, data_.f_int64);
    case Tag::F_UINT64:
      return mojo::internal::WTFHash(seed, data_.f_uint64);
    case Tag::F_FLOAT:
      return mojo::internal::WTFHash(seed, data_.f_float);
    case Tag::F_DOUBLE:
      return mojo::internal::WTFHash(seed, data_.f_double);
    case Tag::F_BOOL:
      return mojo::internal::WTFHash(seed, data_.f_bool);
    case Tag::F_ENUM:
      return mojo::internal::WTFHash(seed, data_.f_enum);
    case Tag::F_EXTENSIBLE_ENUM:
      return mojo::internal::WTFHash(seed, data_.f_extensible_enum);
    default:
      NOTREACHED();
      return seed;
  }
}

bool PodUnion::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context, false);
}
ObjectUnion::ObjectUnion() : tag_(Tag::F_INT8) {
  data_.f_int8 = int8_t();
}

ObjectUnion::~ObjectUnion() {
  DestroyActive();
}


void ObjectUnion::set_f_int8(
    int8_t f_int8) {
  if (tag_ != Tag::F_INT8) {
    DestroyActive();
    tag_ = Tag::F_INT8;
  }
  data_.f_int8 = f_int8;
}
void ObjectUnion::set_f_string(
    const WTF::String& f_string) {
  if (tag_ == Tag::F_STRING) {
    *(data_.f_string) = std::move(f_string);
  } else {
    DestroyActive();
    tag_ = Tag::F_STRING;
    data_.f_string = new WTF::String(
        std::move(f_string));
  }
}
void ObjectUnion::set_f_dummy(
    DummyStructPtr f_dummy) {
  if (tag_ == Tag::F_DUMMY) {
    *(data_.f_dummy) = std::move(f_dummy);
  } else {
    DestroyActive();
    tag_ = Tag::F_DUMMY;
    data_.f_dummy = new DummyStructPtr(
        std::move(f_dummy));
  }
}
void ObjectUnion::set_f_nullable(
    DummyStructPtr f_nullable) {
  if (tag_ == Tag::F_NULLABLE) {
    *(data_.f_nullable) = std::move(f_nullable);
  } else {
    DestroyActive();
    tag_ = Tag::F_NULLABLE;
    data_.f_nullable = new DummyStructPtr(
        std::move(f_nullable));
  }
}
void ObjectUnion::set_f_array_int8(
    const WTF::Vector<int8_t>& f_array_int8) {
  if (tag_ == Tag::F_ARRAY_INT8) {
    *(data_.f_array_int8) = std::move(f_array_int8);
  } else {
    DestroyActive();
    tag_ = Tag::F_ARRAY_INT8;
    data_.f_array_int8 = new WTF::Vector<int8_t>(
        std::move(f_array_int8));
  }
}
void ObjectUnion::set_f_map_int8(
    const WTF::HashMap<WTF::String, int8_t>& f_map_int8) {
  if (tag_ == Tag::F_MAP_INT8) {
    *(data_.f_map_int8) = std::move(f_map_int8);
  } else {
    DestroyActive();
    tag_ = Tag::F_MAP_INT8;
    data_.f_map_int8 = new WTF::HashMap<WTF::String, int8_t>(
        std::move(f_map_int8));
  }
}
void ObjectUnion::set_f_pod_union(
    PodUnionPtr f_pod_union) {
  if (tag_ == Tag::F_POD_UNION) {
    *(data_.f_pod_union) = std::move(f_pod_union);
  } else {
    DestroyActive();
    tag_ = Tag::F_POD_UNION;
    data_.f_pod_union = new PodUnionPtr(
        std::move(f_pod_union));
  }
}
void ObjectUnion::set_f_small_structs(
    WTF::Vector<SmallStructPtr> f_small_structs) {
  if (tag_ == Tag::F_SMALL_STRUCTS) {
    *(data_.f_small_structs) = std::move(f_small_structs);
  } else {
    DestroyActive();
    tag_ = Tag::F_SMALL_STRUCTS;
    data_.f_small_structs = new WTF::Vector<SmallStructPtr>(
        std::move(f_small_structs));
  }
}

void ObjectUnion::DestroyActive() {
  switch (tag_) {

    case Tag::F_INT8:

      break;
    case Tag::F_STRING:

      delete data_.f_string;
      break;
    case Tag::F_DUMMY:

      delete data_.f_dummy;
      break;
    case Tag::F_NULLABLE:

      delete data_.f_nullable;
      break;
    case Tag::F_ARRAY_INT8:

      delete data_.f_array_int8;
      break;
    case Tag::F_MAP_INT8:

      delete data_.f_map_int8;
      break;
    case Tag::F_POD_UNION:

      delete data_.f_pod_union;
      break;
    case Tag::F_SMALL_STRUCTS:

      delete data_.f_small_structs;
      break;
  }
}

bool ObjectUnion::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context, false);
}
HandleUnion::HandleUnion() : tag_(Tag::F_HANDLE) {
  data_.f_handle = new mojo::ScopedHandle;
}

HandleUnion::~HandleUnion() {
  DestroyActive();
}


void HandleUnion::set_f_handle(
    mojo::ScopedHandle f_handle) {
  if (tag_ == Tag::F_HANDLE) {
    *(data_.f_handle) = std::move(f_handle);
  } else {
    DestroyActive();
    tag_ = Tag::F_HANDLE;
    data_.f_handle = new mojo::ScopedHandle(
        std::move(f_handle));
  }
}
void HandleUnion::set_f_message_pipe(
    mojo::ScopedMessagePipeHandle f_message_pipe) {
  if (tag_ == Tag::F_MESSAGE_PIPE) {
    *(data_.f_message_pipe) = std::move(f_message_pipe);
  } else {
    DestroyActive();
    tag_ = Tag::F_MESSAGE_PIPE;
    data_.f_message_pipe = new mojo::ScopedMessagePipeHandle(
        std::move(f_message_pipe));
  }
}
void HandleUnion::set_f_data_pipe_consumer(
    mojo::ScopedDataPipeConsumerHandle f_data_pipe_consumer) {
  if (tag_ == Tag::F_DATA_PIPE_CONSUMER) {
    *(data_.f_data_pipe_consumer) = std::move(f_data_pipe_consumer);
  } else {
    DestroyActive();
    tag_ = Tag::F_DATA_PIPE_CONSUMER;
    data_.f_data_pipe_consumer = new mojo::ScopedDataPipeConsumerHandle(
        std::move(f_data_pipe_consumer));
  }
}
void HandleUnion::set_f_data_pipe_producer(
    mojo::ScopedDataPipeProducerHandle f_data_pipe_producer) {
  if (tag_ == Tag::F_DATA_PIPE_PRODUCER) {
    *(data_.f_data_pipe_producer) = std::move(f_data_pipe_producer);
  } else {
    DestroyActive();
    tag_ = Tag::F_DATA_PIPE_PRODUCER;
    data_.f_data_pipe_producer = new mojo::ScopedDataPipeProducerHandle(
        std::move(f_data_pipe_producer));
  }
}
void HandleUnion::set_f_shared_buffer(
    mojo::ScopedSharedBufferHandle f_shared_buffer) {
  if (tag_ == Tag::F_SHARED_BUFFER) {
    *(data_.f_shared_buffer) = std::move(f_shared_buffer);
  } else {
    DestroyActive();
    tag_ = Tag::F_SHARED_BUFFER;
    data_.f_shared_buffer = new mojo::ScopedSharedBufferHandle(
        std::move(f_shared_buffer));
  }
}
void HandleUnion::set_f_small_cache(
    mojo::PendingRemote<SmallCache> f_small_cache) {
  if (tag_ == Tag::F_SMALL_CACHE) {
    *(data_.f_small_cache) = std::move(f_small_cache);
  } else {
    DestroyActive();
    tag_ = Tag::F_SMALL_CACHE;
    data_.f_small_cache = new mojo::PendingRemote<SmallCache>(
        std::move(f_small_cache));
  }
}
void HandleUnion::set_f_small_cache_receiver(
    mojo::PendingReceiver<SmallCache> f_small_cache_receiver) {
  if (tag_ == Tag::F_SMALL_CACHE_RECEIVER) {
    *(data_.f_small_cache_receiver) = std::move(f_small_cache_receiver);
  } else {
    DestroyActive();
    tag_ = Tag::F_SMALL_CACHE_RECEIVER;
    data_.f_small_cache_receiver = new mojo::PendingReceiver<SmallCache>(
        std::move(f_small_cache_receiver));
  }
}

void HandleUnion::DestroyActive() {
  switch (tag_) {

    case Tag::F_HANDLE:

      delete data_.f_handle;
      break;
    case Tag::F_MESSAGE_PIPE:

      delete data_.f_message_pipe;
      break;
    case Tag::F_DATA_PIPE_CONSUMER:

      delete data_.f_data_pipe_consumer;
      break;
    case Tag::F_DATA_PIPE_PRODUCER:

      delete data_.f_data_pipe_producer;
      break;
    case Tag::F_SHARED_BUFFER:

      delete data_.f_shared_buffer;
      break;
    case Tag::F_SMALL_CACHE:

      delete data_.f_small_cache;
      break;
    case Tag::F_SMALL_CACHE_RECEIVER:

      delete data_.f_small_cache_receiver;
      break;
  }
}

bool HandleUnion::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context, false);
}
OldUnion::OldUnion() : tag_(Tag::F_INT8) {
  data_.f_int8 = int8_t();
}

OldUnion::~OldUnion() {
  DestroyActive();
}


void OldUnion::set_f_int8(
    int8_t f_int8) {
  if (tag_ != Tag::F_INT8) {
    DestroyActive();
    tag_ = Tag::F_INT8;
  }
  data_.f_int8 = f_int8;
}

void OldUnion::DestroyActive() {
  switch (tag_) {

    case Tag::F_INT8:

      break;
  }
}
size_t OldUnion::Hash(size_t seed) const {
  seed = mojo::internal::HashCombine(seed, static_cast<uint32_t>(tag_));
  switch (tag_) {

    case Tag::F_INT8:
      return mojo::internal::WTFHash(seed, data_.f_int8);
    default:
      NOTREACHED();
      return seed;
  }
}

bool OldUnion::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context, false);
}
NewUnion::NewUnion() : tag_(Tag::F_INT8) {
  data_.f_int8 = int8_t();
}

NewUnion::~NewUnion() {
  DestroyActive();
}


void NewUnion::set_f_int8(
    int8_t f_int8) {
  if (tag_ != Tag::F_INT8) {
    DestroyActive();
    tag_ = Tag::F_INT8;
  }
  data_.f_int8 = f_int8;
}
void NewUnion::set_f_int16(
    int16_t f_int16) {
  if (tag_ != Tag::F_INT16) {
    DestroyActive();
    tag_ = Tag::F_INT16;
  }
  data_.f_int16 = f_int16;
}

void NewUnion::DestroyActive() {
  switch (tag_) {

    case Tag::F_INT8:

      break;
    case Tag::F_INT16:

      break;
  }
}
size_t NewUnion::Hash(size_t seed) const {
  seed = mojo::internal::HashCombine(seed, static_cast<uint32_t>(tag_));
  switch (tag_) {

    case Tag::F_INT8:
      return mojo::internal::WTFHash(seed, data_.f_int8);
    case Tag::F_INT16:
      return mojo::internal::WTFHash(seed, data_.f_int16);
    default:
      NOTREACHED();
      return seed;
  }
}

bool NewUnion::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context, false);
}
ImportedUnionUnion::ImportedUnionUnion() : tag_(Tag::POINT_OR_SHAPE) {
  data_.point_or_shape = new ::imported::blink::PointOrShapePtr;
}

ImportedUnionUnion::~ImportedUnionUnion() {
  DestroyActive();
}


void ImportedUnionUnion::set_point_or_shape(
    ::imported::blink::PointOrShapePtr point_or_shape) {
  if (tag_ == Tag::POINT_OR_SHAPE) {
    *(data_.point_or_shape) = std::move(point_or_shape);
  } else {
    DestroyActive();
    tag_ = Tag::POINT_OR_SHAPE;
    data_.point_or_shape = new ::imported::blink::PointOrShapePtr(
        std::move(point_or_shape));
  }
}

void ImportedUnionUnion::DestroyActive() {
  switch (tag_) {

    case Tag::POINT_OR_SHAPE:

      delete data_.point_or_shape;
      break;
  }
}
size_t ImportedUnionUnion::Hash(size_t seed) const {
  seed = mojo::internal::HashCombine(seed, static_cast<uint32_t>(tag_));
  switch (tag_) {

    case Tag::POINT_OR_SHAPE:
      return mojo::internal::WTFHash(seed, data_.point_or_shape);
    default:
      NOTREACHED();
      return seed;
  }
}

bool ImportedUnionUnion::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context, false);
}
const char SmallCache::Name_[] = "mojo.test.SmallCache";

class SmallCache_GetIntValue_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  SmallCache_GetIntValue_ForwardToCallback(
      SmallCache::GetIntValueCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  SmallCache::GetIntValueCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(SmallCache_GetIntValue_ForwardToCallback);
};

SmallCacheProxy::SmallCacheProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class SmallCacheProxy_SetIntValue_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit SmallCacheProxy_SetIntValue_Message(
      uint32_t message_flags
      , int64_t param_int_value
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kSmallCache_SetIntValue_Name,
          message_flags)
      , param_int_value_(std::move(param_int_value)){}
  ~SmallCacheProxy_SetIntValue_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      int64_t param_int_value) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<SmallCacheProxy_SetIntValue_Message>(
          kFlags
          , std::move(param_int_value)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kSmallCache_SetIntValue_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::SmallCache_SetIntValue_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  params->int_value = param_int_value;
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(SmallCache* impl) {
    impl->SetIntValue(
        std::move(param_int_value_));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::SmallCache_SetIntValue_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  params->int_value = param_int_value_;
  }
  int64_t param_int_value_;

  DISALLOW_COPY_AND_ASSIGN(SmallCacheProxy_SetIntValue_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
SmallCacheProxy_SetIntValue_Message::kMessageTag = {};

void SmallCacheProxy::SetIntValue(
    int64_t in_int_value) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::SmallCache::SetIntValue");
#endif
  const bool kExpectsResponse = false;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = SmallCacheProxy_SetIntValue_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_int_value));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(SmallCache::Name_);
  message.set_method_name("SetIntValue");
#endif
  // This return value may be ignored as false implies the Connector has
  // encountered an error, which will be visible through other means.
  ignore_result(receiver_->Accept(&message));
}
class SmallCacheProxy_GetIntValue_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit SmallCacheProxy_GetIntValue_Message(
      uint32_t message_flags
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kSmallCache_GetIntValue_Name,
          message_flags){}
  ~SmallCacheProxy_GetIntValue_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<SmallCacheProxy_GetIntValue_Message>(
          kFlags
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kSmallCache_GetIntValue_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::SmallCache_GetIntValue_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(SmallCache* impl, SmallCache::GetIntValueCallback callback) {
    impl->GetIntValue(std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::SmallCache_GetIntValue_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(SmallCacheProxy_GetIntValue_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
SmallCacheProxy_GetIntValue_Message::kMessageTag = {};

void SmallCacheProxy::GetIntValue(
    GetIntValueCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::SmallCache::GetIntValue");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = SmallCacheProxy_GetIntValue_Message::Build(
      kSerialize, kExpectsResponse, kIsSync);

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(SmallCache::Name_);
  message.set_method_name("GetIntValue");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new SmallCache_GetIntValue_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class SmallCache_GetIntValue_ProxyToResponder {
 public:
  static SmallCache::GetIntValueCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<SmallCache_GetIntValue_ProxyToResponder> proxy(
        new SmallCache_GetIntValue_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&SmallCache_GetIntValue_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~SmallCache_GetIntValue_ProxyToResponder() {
#if DCHECK_IS_ON()
    if (responder_) {
      // If we're being destroyed without being run, we want to ensure the
      // binding endpoint has been closed. This checks for that asynchronously.
      // We pass a bound generated callback to handle the response so that any
      // resulting DCHECK stack will have useful interface type information.
      responder_->IsConnectedAsync(base::BindOnce(&OnIsConnectedComplete));
    }
#endif
    // If the Callback was dropped then deleting the responder will close
    // the pipe so the calling application knows to stop waiting for a reply.
    responder_ = nullptr;
  }

 private:
  SmallCache_GetIntValue_ProxyToResponder(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder)
      : request_id_(request_id),
        is_sync_(is_sync),
        responder_(std::move(responder)) {
  }

#if DCHECK_IS_ON()
  static void OnIsConnectedComplete(bool connected) {
    DCHECK(!connected)
        << "SmallCache::GetIntValueCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      int64_t in_int_value);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(SmallCache_GetIntValue_ProxyToResponder);
};
class SmallCache_GetIntValue_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit SmallCache_GetIntValue_Response_Message(
      uint32_t message_flags
      , int64_t param_int_value
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kSmallCache_GetIntValue_Name,
          message_flags)
      , param_int_value_(std::move(param_int_value)){}
  ~SmallCache_GetIntValue_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      int64_t param_int_value) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<SmallCache_GetIntValue_Response_Message>(
          kFlags
          , std::move(param_int_value)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kSmallCache_GetIntValue_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::SmallCache_GetIntValue_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  params->int_value = param_int_value;
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(SmallCache::GetIntValueCallback* callback) {
    std::move(*callback).Run(
        std::move(param_int_value_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::SmallCache_GetIntValue_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  params->int_value = param_int_value_;
  }
  int64_t param_int_value_;

  DISALLOW_COPY_AND_ASSIGN(SmallCache_GetIntValue_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
SmallCache_GetIntValue_Response_Message::kMessageTag = {};

bool SmallCache_GetIntValue_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::SmallCache::GetIntValueCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<SmallCache_GetIntValue_Response_Message>();
    if (!context) {
      // The Message was not of the expected type. It may be a valid message
      // which was build using a different variant of these bindings. Force
      // serialization before dispatch in this case.
      message->SerializeIfNecessary();
    } else {
      if (!callback_.is_null())
        context->Dispatch(&callback_);
      return true;
    }
  }

  DCHECK(message->is_serialized());
  internal::SmallCache_GetIntValue_ResponseParams_Data* params =
      reinterpret_cast<
          internal::SmallCache_GetIntValue_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  int64_t p_int_value{};
  SmallCache_GetIntValue_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  p_int_value = input_data_view.int_value();
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        SmallCache::Name_, 1, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_int_value));
  return true;
}

void SmallCache_GetIntValue_ProxyToResponder::Run(
    int64_t in_int_value) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = SmallCache_GetIntValue_Response_Message::Build(kSerialize, is_sync_, std::move(in_int_value));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::SmallCache::GetIntValueCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(SmallCache::Name_);
  message.set_method_name("GetIntValue");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool SmallCacheStubDispatch::Accept(
    SmallCache* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kSmallCache_SetIntValue_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::SmallCache::SetIntValue",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            SmallCacheProxy_SetIntValue_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was serialized using a different variant of these bindings.
          // Force serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          context->Dispatch(impl);
          return true;
        }
      }

      DCHECK(message->is_serialized());
      internal::SmallCache_SetIntValue_Params_Data* params =
          reinterpret_cast<internal::SmallCache_SetIntValue_Params_Data*>(
              message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      int64_t p_int_value{};
      SmallCache_SetIntValue_ParamsDataView input_data_view(params, &serialization_context);
      
      p_int_value = input_data_view.int_value();
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            SmallCache::Name_, 0, false);
        return false;
      }
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->SetIntValue(
std::move(p_int_value));
      return true;
    }
    case internal::kSmallCache_GetIntValue_Name: {
      break;
    }
  }
  return false;
}

// static
bool SmallCacheStubDispatch::AcceptWithResponder(
    SmallCache* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kSmallCache_SetIntValue_Name: {
      break;
    }
    case internal::kSmallCache_GetIntValue_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::SmallCache::GetIntValue",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            SmallCacheProxy_GetIntValue_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          SmallCache::GetIntValueCallback callback =
              SmallCache_GetIntValue_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::SmallCache_GetIntValue_Params_Data* params =
          reinterpret_cast<
              internal::SmallCache_GetIntValue_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      SmallCache_GetIntValue_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            SmallCache::Name_, 1, false);
        return false;
      }
      SmallCache::GetIntValueCallback callback =
          SmallCache_GetIntValue_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->GetIntValue(std::move(callback));
      return true;
    }
  }
  return false;
}

bool SmallCacheRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "SmallCache RequestValidator");

  switch (message->header()->name) {
    case internal::kSmallCache_SetIntValue_Name: {
      if (!mojo::internal::ValidateMessageIsRequestWithoutResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::SmallCache_SetIntValue_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    case internal::kSmallCache_GetIntValue_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::SmallCache_GetIntValue_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    default:
      break;
  }

  // Unrecognized message.
  ReportValidationError(
      &validation_context,
      mojo::internal::VALIDATION_ERROR_MESSAGE_HEADER_UNKNOWN_METHOD);
  return false;
}

bool SmallCacheResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "SmallCache ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kSmallCache_GetIntValue_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::SmallCache_GetIntValue_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    default:
      break;
  }

  // Unrecognized message.
  ReportValidationError(
      &validation_context,
      mojo::internal::VALIDATION_ERROR_MESSAGE_HEADER_UNKNOWN_METHOD);
  return false;
}
const char UnionInterface::Name_[] = "mojo.test.UnionInterface";

class UnionInterface_Echo_ForwardToCallback
    : public mojo::MessageReceiver {
 public:
  UnionInterface_Echo_ForwardToCallback(
      UnionInterface::EchoCallback callback
      ) : callback_(std::move(callback)) {
  }
  bool Accept(mojo::Message* message) override;
 private:
  UnionInterface::EchoCallback callback_;
  DISALLOW_COPY_AND_ASSIGN(UnionInterface_Echo_ForwardToCallback);
};

UnionInterfaceProxy::UnionInterfaceProxy(mojo::MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {
}
class UnionInterfaceProxy_Echo_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit UnionInterfaceProxy_Echo_Message(
      uint32_t message_flags
      , PodUnionPtr param_in_val
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kUnionInterface_Echo_Name,
          message_flags)
      , param_in_val_(std::move(param_in_val)){}
  ~UnionInterfaceProxy_Echo_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool expects_response,
      bool is_sync,
      PodUnionPtr param_in_val) {

    
  const uint32_t kFlags =
      ((expects_response) ? mojo::Message::kFlagExpectsResponse : 0) |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<UnionInterfaceProxy_Echo_Message>(
          kFlags
          , std::move(param_in_val)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kUnionInterface_Echo_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::UnionInterface_Echo_Params_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->in_val)::BufferWriter in_val_writer;
  in_val_writer.AllocateInline(buffer, &params->in_val);
  mojo::internal::Serialize<::mojo::test::PodUnionDataView>(
      param_in_val, buffer, &in_val_writer, true, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->in_val.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null in_val in UnionInterface.Echo request");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(UnionInterface* impl, UnionInterface::EchoCallback callback) {
    impl->Echo(
        std::move(param_in_val_), std::move(callback));
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::UnionInterface_Echo_Params_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->in_val)::BufferWriter in_val_writer;
  in_val_writer.AllocateInline(buffer, &params->in_val);
  mojo::internal::Serialize<::mojo::test::PodUnionDataView>(
      param_in_val_, buffer, &in_val_writer, true, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->in_val.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null in_val in UnionInterface.Echo request");
  }
  PodUnionPtr param_in_val_;

  DISALLOW_COPY_AND_ASSIGN(UnionInterfaceProxy_Echo_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
UnionInterfaceProxy_Echo_Message::kMessageTag = {};

void UnionInterfaceProxy::Echo(
    PodUnionPtr in_in_val, EchoCallback callback) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT0("mojom", "<class 'jinja2::utils::Namespace'>::UnionInterface::Echo");
#endif
  const bool kExpectsResponse = true;
  const bool kIsSync = false;
  const bool kSerialize = receiver_->PrefersSerializedMessages();
  auto message = UnionInterfaceProxy_Echo_Message::Build(
      kSerialize, kExpectsResponse, kIsSync, std::move(in_in_val));

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(UnionInterface::Name_);
  message.set_method_name("Echo");
#endif
  std::unique_ptr<mojo::MessageReceiver> responder(
      new UnionInterface_Echo_ForwardToCallback(
          std::move(callback)));
  ignore_result(receiver_->AcceptWithResponder(&message, std::move(responder)));
}
class UnionInterface_Echo_ProxyToResponder {
 public:
  static UnionInterface::EchoCallback CreateCallback(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
    std::unique_ptr<UnionInterface_Echo_ProxyToResponder> proxy(
        new UnionInterface_Echo_ProxyToResponder(
            request_id, is_sync, std::move(responder)));
    return base::BindOnce(&UnionInterface_Echo_ProxyToResponder::Run,
                          std::move(proxy));
  }

  ~UnionInterface_Echo_ProxyToResponder() {
#if DCHECK_IS_ON()
    if (responder_) {
      // If we're being destroyed without being run, we want to ensure the
      // binding endpoint has been closed. This checks for that asynchronously.
      // We pass a bound generated callback to handle the response so that any
      // resulting DCHECK stack will have useful interface type information.
      responder_->IsConnectedAsync(base::BindOnce(&OnIsConnectedComplete));
    }
#endif
    // If the Callback was dropped then deleting the responder will close
    // the pipe so the calling application knows to stop waiting for a reply.
    responder_ = nullptr;
  }

 private:
  UnionInterface_Echo_ProxyToResponder(
      uint64_t request_id,
      bool is_sync,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder)
      : request_id_(request_id),
        is_sync_(is_sync),
        responder_(std::move(responder)) {
  }

#if DCHECK_IS_ON()
  static void OnIsConnectedComplete(bool connected) {
    DCHECK(!connected)
        << "UnionInterface::EchoCallback was destroyed without "
        << "first either being run or its corresponding binding being closed. "
        << "It is an error to drop response callbacks which still correspond "
        << "to an open interface pipe.";
  }
#endif

  void Run(
      PodUnionPtr in_out_val);

  uint64_t request_id_;
  bool is_sync_;
  std::unique_ptr<mojo::MessageReceiverWithStatus> responder_;

  DISALLOW_COPY_AND_ASSIGN(UnionInterface_Echo_ProxyToResponder);
};
class UnionInterface_Echo_Response_Message
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  explicit UnionInterface_Echo_Response_Message(
      uint32_t message_flags
      , PodUnionPtr param_out_val
  )
      : mojo::internal::UnserializedMessageContext(
          &kMessageTag,
          internal::kUnionInterface_Echo_Name,
          message_flags)
      , param_out_val_(std::move(param_out_val)){}
  ~UnionInterface_Echo_Response_Message() override = default;

  static mojo::Message Build(
      bool serialize,
      bool is_sync,
      PodUnionPtr param_out_val) {

    
  const uint32_t kFlags = mojo::Message::kFlagIsResponse |
      ((is_sync) ? mojo::Message::kFlagIsSync : 0);

    if (!serialize) {
      return mojo::Message(std::make_unique<UnionInterface_Echo_Response_Message>(
          kFlags
          , std::move(param_out_val)
          ));
    }

    DCHECK(serialize);
    
  mojo::Message message(
      internal::kUnionInterface_Echo_Name, kFlags, 0, 0, nullptr);
  auto* buffer = message.payload_buffer();
  ::mojo::test::internal::UnionInterface_Echo_ResponseParams_Data::BufferWriter
      params;
  mojo::internal::SerializationContext serialization_context;
  params.Allocate(buffer);
  typename decltype(params->out_val)::BufferWriter out_val_writer;
  out_val_writer.AllocateInline(buffer, &params->out_val);
  mojo::internal::Serialize<::mojo::test::PodUnionDataView>(
      param_out_val, buffer, &out_val_writer, true, &serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->out_val.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null out_val in UnionInterface.Echo response");
  message.AttachHandlesFromSerializationContext(
      &serialization_context);
    return message;
  }


  void Dispatch(UnionInterface::EchoCallback* callback) {
    std::move(*callback).Run(
        std::move(param_out_val_));
  }



 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* serialization_context,
                 mojo::internal::Buffer* buffer) override {
    ::mojo::test::internal::UnionInterface_Echo_ResponseParams_Data::BufferWriter
        params;
    params.Allocate(buffer);
  typename decltype(params->out_val)::BufferWriter out_val_writer;
  out_val_writer.AllocateInline(buffer, &params->out_val);
  mojo::internal::Serialize<::mojo::test::PodUnionDataView>(
      param_out_val_, buffer, &out_val_writer, true, serialization_context);
  MOJO_INTERNAL_DLOG_SERIALIZATION_WARNING(
      params->out_val.is_null(),
      mojo::internal::VALIDATION_ERROR_UNEXPECTED_NULL_POINTER,
      "null out_val in UnionInterface.Echo response");
  }
  PodUnionPtr param_out_val_;

  DISALLOW_COPY_AND_ASSIGN(UnionInterface_Echo_Response_Message);
};

const mojo::internal::UnserializedMessageContext::Tag
UnionInterface_Echo_Response_Message::kMessageTag = {};

bool UnionInterface_Echo_ForwardToCallback::Accept(
    mojo::Message* message) {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "<class 'jinja2::utils::Namespace'>::UnionInterface::EchoCallback",
               "message", message->name());
#endif
  mojo::internal::MessageDispatchContext dispatch_context(message);
  if (!message->is_serialized()) {
    auto context =
        message->TakeUnserializedContext<UnionInterface_Echo_Response_Message>();
    if (!context) {
      // The Message was not of the expected type. It may be a valid message
      // which was build using a different variant of these bindings. Force
      // serialization before dispatch in this case.
      message->SerializeIfNecessary();
    } else {
      if (!callback_.is_null())
        context->Dispatch(&callback_);
      return true;
    }
  }

  DCHECK(message->is_serialized());
  internal::UnionInterface_Echo_ResponseParams_Data* params =
      reinterpret_cast<
          internal::UnionInterface_Echo_ResponseParams_Data*>(
              message->mutable_payload());
  
  mojo::internal::SerializationContext serialization_context;
  serialization_context.TakeHandlesFromMessage(message);
  bool success = true;
  PodUnionPtr p_out_val{};
  UnionInterface_Echo_ResponseParamsDataView input_data_view(params, &serialization_context);
  
  if (!input_data_view.ReadOutVal(&p_out_val))
    success = false;
  if (!success) {
    ReportValidationErrorForMessage(
        message,
        mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
        UnionInterface::Name_, 0, true);
    return false;
  }
  if (!callback_.is_null())
    std::move(callback_).Run(
std::move(p_out_val));
  return true;
}

void UnionInterface_Echo_ProxyToResponder::Run(
    PodUnionPtr in_out_val) {
  const bool kSerialize = responder_->PrefersSerializedMessages();
  auto message = UnionInterface_Echo_Response_Message::Build(kSerialize, is_sync_, std::move(in_out_val));

#if BUILDFLAG(MOJO_TRACE_ENABLED)
  TRACE_EVENT1("mojom", "(Impl)<class 'jinja2::utils::Namespace'>::UnionInterface::EchoCallback",
               "message", message.name());
#endif

#if defined(ENABLE_IPC_FUZZER)
  message.set_interface_name(UnionInterface::Name_);
  message.set_method_name("Echo");
#endif

  message.set_request_id(request_id_);
  ignore_result(responder_->Accept(&message));
  // TODO(darin): Accept() returning false indicates a malformed message, and
  // that may be good reason to close the connection. However, we don't have a
  // way to do that from here. We should add a way.
  responder_ = nullptr;
}

// static
bool UnionInterfaceStubDispatch::Accept(
    UnionInterface* impl,
    mojo::Message* message) {
  switch (message->header()->name) {
    case internal::kUnionInterface_Echo_Name: {
      break;
    }
  }
  return false;
}

// static
bool UnionInterfaceStubDispatch::AcceptWithResponder(
    UnionInterface* impl,
    mojo::Message* message,
    std::unique_ptr<mojo::MessageReceiverWithStatus> responder) {
  switch (message->header()->name) {
    case internal::kUnionInterface_Echo_Name: {
#if BUILDFLAG(MOJO_TRACE_ENABLED)
      TRACE_EVENT1(
          "mojom",
          "(Impl)<class 'jinja2::utils::Namespace'>::UnionInterface::Echo",
          "message", message->name());
#endif
      base::TaskAnnotator::ScopedSetIpcProgramCounter scoped_ipc_pc(
          base::GetProgramCounter());
      mojo::internal::MessageDispatchContext context(message);
      if (!message->is_serialized()) {
        auto context = message->TakeUnserializedContext<
            UnionInterfaceProxy_Echo_Message>();
        if (!context) {
          // The Message was not of the expected type. It may be a valid message
          // which was built using a different variant of these bindings. Force
          // serialization before dispatch in this case.
          message->SerializeIfNecessary();
        } else {
          UnionInterface::EchoCallback callback =
              UnionInterface_Echo_ProxyToResponder::CreateCallback(
                  message->request_id(),
                  message->has_flag(mojo::Message::kFlagIsSync),
                  std::move(responder));
          context->Dispatch(impl, std::move(callback));
          return true;
        }
      }

      internal::UnionInterface_Echo_Params_Data* params =
          reinterpret_cast<
              internal::UnionInterface_Echo_Params_Data*>(
                  message->mutable_payload());
      
      mojo::internal::SerializationContext serialization_context;
      serialization_context.TakeHandlesFromMessage(message);
      bool success = true;
      PodUnionPtr p_in_val{};
      UnionInterface_Echo_ParamsDataView input_data_view(params, &serialization_context);
      
      if (!input_data_view.ReadInVal(&p_in_val))
        success = false;
      if (!success) {
        ReportValidationErrorForMessage(
            message,
            mojo::internal::VALIDATION_ERROR_DESERIALIZATION_FAILED,
            UnionInterface::Name_, 0, false);
        return false;
      }
      UnionInterface::EchoCallback callback =
          UnionInterface_Echo_ProxyToResponder::CreateCallback(
              message->request_id(),
              message->has_flag(mojo::Message::kFlagIsSync),
              std::move(responder));
      // A null |impl| means no implementation was bound.
      DCHECK(impl);
      impl->Echo(
std::move(p_in_val), std::move(callback));
      return true;
    }
  }
  return false;
}

bool UnionInterfaceRequestValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "UnionInterface RequestValidator");

  switch (message->header()->name) {
    case internal::kUnionInterface_Echo_Name: {
      if (!mojo::internal::ValidateMessageIsRequestExpectingResponse(
              message, &validation_context)) {
        return false;
      }
      if (!mojo::internal::ValidateMessagePayload<
               internal::UnionInterface_Echo_Params_Data>(
                  message, &validation_context)) {
        return false;
      }
      return true;
    }
    default:
      break;
  }

  // Unrecognized message.
  ReportValidationError(
      &validation_context,
      mojo::internal::VALIDATION_ERROR_MESSAGE_HEADER_UNKNOWN_METHOD);
  return false;
}

bool UnionInterfaceResponseValidator::Accept(mojo::Message* message) {
  if (!message->is_serialized() ||
      mojo::internal::ControlMessageHandler::IsControlMessage(message)) {
    return true;
  }

  mojo::internal::ValidationContext validation_context(
    message->payload(), message->payload_num_bytes(),
    message->handles()->size(), message->payload_num_interface_ids(), message,
    "UnionInterface ResponseValidator");

  if (!mojo::internal::ValidateMessageIsResponse(message, &validation_context))
    return false;
  switch (message->header()->name) {
    case internal::kUnionInterface_Echo_Name: {
      if (!mojo::internal::ValidateMessagePayload<
               internal::UnionInterface_Echo_ResponseParams_Data>(
                    message, &validation_context)) {
        return false;
      }
      return true;
    }
    default:
      break;
  }

  // Unrecognized message.
  ReportValidationError(
      &validation_context,
      mojo::internal::VALIDATION_ERROR_MESSAGE_HEADER_UNKNOWN_METHOD);
  return false;
}
}  // namespace blink
}  // namespace test
}  // namespace mojo

namespace mojo {


// static
bool StructTraits<::mojo::test::blink::WrapperStruct::DataView, ::mojo::test::blink::WrapperStructPtr>::Read(
    ::mojo::test::blink::WrapperStruct::DataView input,
    ::mojo::test::blink::WrapperStructPtr* output) {
  bool success = true;
  ::mojo::test::blink::WrapperStructPtr result(::mojo::test::blink::WrapperStruct::New());
  
      if (!input.ReadObjectUnion(&result->object_union))
        success = false;
      if (!input.ReadPodUnion(&result->pod_union))
        success = false;
      if (!input.ReadHandleUnion(&result->handle_union))
        success = false;
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::blink::DummyStruct::DataView, ::mojo::test::blink::DummyStructPtr>::Read(
    ::mojo::test::blink::DummyStruct::DataView input,
    ::mojo::test::blink::DummyStructPtr* output) {
  bool success = true;
  ::mojo::test::blink::DummyStructPtr result(::mojo::test::blink::DummyStruct::New());
  
      result->f_int8 = input.f_int8();
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::blink::SmallStruct::DataView, ::mojo::test::blink::SmallStructPtr>::Read(
    ::mojo::test::blink::SmallStruct::DataView input,
    ::mojo::test::blink::SmallStructPtr* output) {
  bool success = true;
  ::mojo::test::blink::SmallStructPtr result(::mojo::test::blink::SmallStruct::New());
  
      if (!input.ReadDummyStruct(&result->dummy_struct))
        success = false;
      if (!input.ReadPodUnion(&result->pod_union))
        success = false;
      if (!input.ReadPodUnionArray(&result->pod_union_array))
        success = false;
      if (!input.ReadNullablePodUnionArray(&result->nullable_pod_union_array))
        success = false;
      if (!input.ReadSArray(&result->s_array))
        success = false;
      if (!input.ReadPodUnionMap(&result->pod_union_map))
        success = false;
      if (!input.ReadNullablePodUnionMap(&result->nullable_pod_union_map))
        success = false;
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::blink::SmallStructNonNullableUnion::DataView, ::mojo::test::blink::SmallStructNonNullableUnionPtr>::Read(
    ::mojo::test::blink::SmallStructNonNullableUnion::DataView input,
    ::mojo::test::blink::SmallStructNonNullableUnionPtr* output) {
  bool success = true;
  ::mojo::test::blink::SmallStructNonNullableUnionPtr result(::mojo::test::blink::SmallStructNonNullableUnion::New());
  
      if (!input.ReadPodUnion(&result->pod_union))
        success = false;
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::blink::SmallObjStruct::DataView, ::mojo::test::blink::SmallObjStructPtr>::Read(
    ::mojo::test::blink::SmallObjStruct::DataView input,
    ::mojo::test::blink::SmallObjStructPtr* output) {
  bool success = true;
  ::mojo::test::blink::SmallObjStructPtr result(::mojo::test::blink::SmallObjStruct::New());
  
      if (!input.ReadObjUnion(&result->obj_union))
        success = false;
      result->f_int8 = input.f_int8();
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::blink::TryNonNullStruct::DataView, ::mojo::test::blink::TryNonNullStructPtr>::Read(
    ::mojo::test::blink::TryNonNullStruct::DataView input,
    ::mojo::test::blink::TryNonNullStructPtr* output) {
  bool success = true;
  ::mojo::test::blink::TryNonNullStructPtr result(::mojo::test::blink::TryNonNullStruct::New());
  
      if (!input.ReadNullable(&result->nullable))
        success = false;
      if (!input.ReadNonNullable(&result->non_nullable))
        success = false;
  *output = std::move(result);
  return success;
}


// static
bool StructTraits<::mojo::test::blink::ImportedUnionStruct::DataView, ::mojo::test::blink::ImportedUnionStructPtr>::Read(
    ::mojo::test::blink::ImportedUnionStruct::DataView input,
    ::mojo::test::blink::ImportedUnionStructPtr* output) {
  bool success = true;
  ::mojo::test::blink::ImportedUnionStructPtr result(::mojo::test::blink::ImportedUnionStruct::New());
  
      if (!input.ReadPointOrShape(&result->point_or_shape))
        success = false;
  *output = std::move(result);
  return success;
}

// static
bool UnionTraits<::mojo::test::blink::PodUnion::DataView, ::mojo::test::blink::PodUnionPtr>::Read(
    ::mojo::test::blink::PodUnion::DataView input,
    ::mojo::test::blink::PodUnionPtr* output) {
  using UnionType = ::mojo::test::blink::PodUnion;
  using Tag = UnionType::Tag;

  switch (input.tag()) {
    case Tag::F_INT8: {
      *output = UnionType::NewFInt8(input.f_int8());
      break;
    }
    case Tag::F_INT8_OTHER: {
      *output = UnionType::NewFInt8Other(input.f_int8_other());
      break;
    }
    case Tag::F_UINT8: {
      *output = UnionType::NewFUint8(input.f_uint8());
      break;
    }
    case Tag::F_INT16: {
      *output = UnionType::NewFInt16(input.f_int16());
      break;
    }
    case Tag::F_UINT16: {
      *output = UnionType::NewFUint16(input.f_uint16());
      break;
    }
    case Tag::F_INT32: {
      *output = UnionType::NewFInt32(input.f_int32());
      break;
    }
    case Tag::F_UINT32: {
      *output = UnionType::NewFUint32(input.f_uint32());
      break;
    }
    case Tag::F_INT64: {
      *output = UnionType::NewFInt64(input.f_int64());
      break;
    }
    case Tag::F_UINT64: {
      *output = UnionType::NewFUint64(input.f_uint64());
      break;
    }
    case Tag::F_FLOAT: {
      *output = UnionType::NewFFloat(input.f_float());
      break;
    }
    case Tag::F_DOUBLE: {
      *output = UnionType::NewFDouble(input.f_double());
      break;
    }
    case Tag::F_BOOL: {
      *output = UnionType::NewFBool(input.f_bool());
      break;
    }
    case Tag::F_ENUM: {
      ::mojo::test::blink::AnEnum result_f_enum;
      if (!input.ReadFEnum(&result_f_enum))
        return false;

      *output = UnionType::NewFEnum(result_f_enum);
      break;
    }
    case Tag::F_EXTENSIBLE_ENUM: {
      ::mojo::test::blink::AnExtensibleEnum result_f_extensible_enum;
      if (!input.ReadFExtensibleEnum(&result_f_extensible_enum))
        return false;

      *output = UnionType::NewFExtensibleEnum(result_f_extensible_enum);
      break;
    }
    default:
      return false;
  }
  return true;
}

// static
bool UnionTraits<::mojo::test::blink::ObjectUnion::DataView, ::mojo::test::blink::ObjectUnionPtr>::Read(
    ::mojo::test::blink::ObjectUnion::DataView input,
    ::mojo::test::blink::ObjectUnionPtr* output) {
  using UnionType = ::mojo::test::blink::ObjectUnion;
  using Tag = UnionType::Tag;

  switch (input.tag()) {
    case Tag::F_INT8: {
      *output = UnionType::NewFInt8(input.f_int8());
      break;
    }
    case Tag::F_STRING: {
      WTF::String result_f_string;
      if (!input.ReadFString(&result_f_string))
        return false;

      *output = UnionType::NewFString(
          std::move(result_f_string));
      break;
    }
    case Tag::F_DUMMY: {
      ::mojo::test::blink::DummyStructPtr result_f_dummy;
      if (!input.ReadFDummy(&result_f_dummy))
        return false;

      *output = UnionType::NewFDummy(
          std::move(result_f_dummy));
      break;
    }
    case Tag::F_NULLABLE: {
      ::mojo::test::blink::DummyStructPtr result_f_nullable;
      if (!input.ReadFNullable(&result_f_nullable))
        return false;

      *output = UnionType::NewFNullable(
          std::move(result_f_nullable));
      break;
    }
    case Tag::F_ARRAY_INT8: {
      WTF::Vector<int8_t> result_f_array_int8;
      if (!input.ReadFArrayInt8(&result_f_array_int8))
        return false;

      *output = UnionType::NewFArrayInt8(
          std::move(result_f_array_int8));
      break;
    }
    case Tag::F_MAP_INT8: {
      WTF::HashMap<WTF::String, int8_t> result_f_map_int8;
      if (!input.ReadFMapInt8(&result_f_map_int8))
        return false;

      *output = UnionType::NewFMapInt8(
          std::move(result_f_map_int8));
      break;
    }
    case Tag::F_POD_UNION: {
      ::mojo::test::blink::PodUnionPtr result_f_pod_union;
      if (!input.ReadFPodUnion(&result_f_pod_union))
        return false;

      *output = UnionType::NewFPodUnion(
          std::move(result_f_pod_union));
      break;
    }
    case Tag::F_SMALL_STRUCTS: {
      WTF::Vector<::mojo::test::blink::SmallStructPtr> result_f_small_structs;
      if (!input.ReadFSmallStructs(&result_f_small_structs))
        return false;

      *output = UnionType::NewFSmallStructs(
          std::move(result_f_small_structs));
      break;
    }
    default:
      return false;
  }
  return true;
}

// static
bool UnionTraits<::mojo::test::blink::HandleUnion::DataView, ::mojo::test::blink::HandleUnionPtr>::Read(
    ::mojo::test::blink::HandleUnion::DataView input,
    ::mojo::test::blink::HandleUnionPtr* output) {
  using UnionType = ::mojo::test::blink::HandleUnion;
  using Tag = UnionType::Tag;

  switch (input.tag()) {
    case Tag::F_HANDLE: {
      *output = UnionType::NewFHandle(
          input.TakeFHandle());
      break;
    }
    case Tag::F_MESSAGE_PIPE: {
      *output = UnionType::NewFMessagePipe(
          input.TakeFMessagePipe());
      break;
    }
    case Tag::F_DATA_PIPE_CONSUMER: {
      *output = UnionType::NewFDataPipeConsumer(
          input.TakeFDataPipeConsumer());
      break;
    }
    case Tag::F_DATA_PIPE_PRODUCER: {
      *output = UnionType::NewFDataPipeProducer(
          input.TakeFDataPipeProducer());
      break;
    }
    case Tag::F_SHARED_BUFFER: {
      *output = UnionType::NewFSharedBuffer(
          input.TakeFSharedBuffer());
      break;
    }
    case Tag::F_SMALL_CACHE: {
      *output = UnionType::NewFSmallCache(
          input.TakeFSmallCache<mojo::PendingRemote<::mojo::test::blink::SmallCache>>());
      break;
    }
    case Tag::F_SMALL_CACHE_RECEIVER: {
      *output = UnionType::NewFSmallCacheReceiver(
          input.TakeFSmallCacheReceiver<mojo::PendingReceiver<::mojo::test::blink::SmallCache>>());
      break;
    }
    default:
      return false;
  }
  return true;
}

// static
bool UnionTraits<::mojo::test::blink::OldUnion::DataView, ::mojo::test::blink::OldUnionPtr>::Read(
    ::mojo::test::blink::OldUnion::DataView input,
    ::mojo::test::blink::OldUnionPtr* output) {
  using UnionType = ::mojo::test::blink::OldUnion;
  using Tag = UnionType::Tag;

  switch (input.tag()) {
    case Tag::F_INT8: {
      *output = UnionType::NewFInt8(input.f_int8());
      break;
    }
    default:
      return false;
  }
  return true;
}

// static
bool UnionTraits<::mojo::test::blink::NewUnion::DataView, ::mojo::test::blink::NewUnionPtr>::Read(
    ::mojo::test::blink::NewUnion::DataView input,
    ::mojo::test::blink::NewUnionPtr* output) {
  using UnionType = ::mojo::test::blink::NewUnion;
  using Tag = UnionType::Tag;

  switch (input.tag()) {
    case Tag::F_INT8: {
      *output = UnionType::NewFInt8(input.f_int8());
      break;
    }
    case Tag::F_INT16: {
      *output = UnionType::NewFInt16(input.f_int16());
      break;
    }
    default:
      return false;
  }
  return true;
}

// static
bool UnionTraits<::mojo::test::blink::ImportedUnionUnion::DataView, ::mojo::test::blink::ImportedUnionUnionPtr>::Read(
    ::mojo::test::blink::ImportedUnionUnion::DataView input,
    ::mojo::test::blink::ImportedUnionUnionPtr* output) {
  using UnionType = ::mojo::test::blink::ImportedUnionUnion;
  using Tag = UnionType::Tag;

  switch (input.tag()) {
    case Tag::POINT_OR_SHAPE: {
      ::imported::blink::PointOrShapePtr result_point_or_shape;
      if (!input.ReadPointOrShape(&result_point_or_shape))
        return false;

      *output = UnionType::NewPointOrShape(
          std::move(result_point_or_shape));
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