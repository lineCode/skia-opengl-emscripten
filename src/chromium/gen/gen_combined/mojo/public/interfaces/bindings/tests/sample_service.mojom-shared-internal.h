// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_SERVICE_MOJOM_SHARED_INTERNAL_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_SERVICE_MOJOM_SHARED_INTERNAL_H_

#include "mojo/public/cpp/bindings/lib/array_internal.h"
#include "mojo/public/cpp/bindings/lib/bindings_internal.h"
#include "mojo/public/cpp/bindings/lib/map_data_internal.h"
#include "mojo/public/cpp/bindings/lib/buffer.h"
#include "mojo/public/interfaces/bindings/tests/sample_import.mojom-shared-internal.h"
#include "mojo/public/interfaces/bindings/tests/sample_import2.mojom-shared-internal.h"
#include "mojo/public/cpp/bindings/lib/native_enum_data.h"
#include "mojo/public/interfaces/bindings/native_struct.mojom-shared-internal.h"



namespace mojo {
namespace internal {
class ValidationContext;
}
}
namespace sample {
namespace internal {
class Bar_Data;
class Foo_Data;
class DefaultsTest_Data;
class StructWithHoleV1_Data;
class StructWithHoleV2_Data;

struct Bar_Type_Data {
 public:
  static bool constexpr kIsExtensible = false;

  static bool IsKnownValue(int32_t value) {
    switch (value) {
      case 1:
      case 2:
      case 3:
      case 4:
        return true;
    }
    return false;
  }

  static bool Validate(int32_t value,
                       mojo::internal::ValidationContext* validation_context) {
    if (kIsExtensible || IsKnownValue(value))
      return true;

    ReportValidationError(validation_context,
                          mojo::internal::VALIDATION_ERROR_UNKNOWN_ENUM_VALUE);
    return false;
  }
};

struct Service_BazOptions_Data {
 public:
  static bool constexpr kIsExtensible = false;

  static bool IsKnownValue(int32_t value) {
    switch (value) {
      case 0:
      case 1:
        return true;
    }
    return false;
  }

  static bool Validate(int32_t value,
                       mojo::internal::ValidationContext* validation_context) {
    if (kIsExtensible || IsKnownValue(value))
      return true;

    ReportValidationError(validation_context,
                          mojo::internal::VALIDATION_ERROR_UNKNOWN_ENUM_VALUE);
    return false;
  }
};

#pragma pack(push, 1)
class  Bar_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(Bar_Data));
      new (data()) Bar_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    Bar_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<Bar_Data>(index_);
    }
    Bar_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;
  uint8_t alpha;
  uint8_t beta;
  uint8_t gamma;
  uint8_t pad2_[1];
  int32_t type;

 private:
  Bar_Data();
  ~Bar_Data() = delete;
};
static_assert(sizeof(Bar_Data) == 16,
              "Bad sizeof(Bar_Data)");
// Used by Bar::WrapAsMessage to lazily serialize the struct.
template <typename UserType, typename DataView>
struct Bar_UnserializedMessageContext
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  Bar_UnserializedMessageContext(
    uint32_t message_name,
    uint32_t message_flags,
    UserType input)
      : mojo::internal::UnserializedMessageContext(&kMessageTag, message_name, message_flags)
      , user_data_(std::move(input)) {}
  ~Bar_UnserializedMessageContext() override = default;

  UserType TakeData() {
    return std::move(user_data_);
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* context,
                 mojo::internal::Buffer* buffer) override {
    Bar_Data::BufferWriter writer;
    mojo::internal::Serialize<DataView>(user_data_, buffer, &writer, context);
  }

  UserType user_data_;
};

template <typename UserType, typename DataView>
const mojo::internal::UnserializedMessageContext::Tag
    Bar_UnserializedMessageContext<UserType, DataView>::kMessageTag = {};
class  Foo_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(Foo_Data));
      new (data()) Foo_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    Foo_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<Foo_Data>(index_);
    }
    Foo_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;
  int32_t x;
  int32_t y;
  uint8_t a : 1;
  uint8_t b : 1;
  uint8_t c : 1;
  uint8_t pad4_[3];
  mojo::internal::Handle_Data source;
  mojo::internal::Pointer<internal::Bar_Data> bar;
  mojo::internal::Pointer<mojo::internal::Array_Data<uint8_t>> data;
  mojo::internal::Pointer<mojo::internal::Array_Data<mojo::internal::Pointer<internal::Bar_Data>>> extra_bars;
  mojo::internal::Pointer<mojo::internal::String_Data> name;
  mojo::internal::Pointer<mojo::internal::Array_Data<mojo::internal::Handle_Data>> input_streams;
  mojo::internal::Pointer<mojo::internal::Array_Data<mojo::internal::Handle_Data>> output_streams;
  mojo::internal::Pointer<mojo::internal::Array_Data<mojo::internal::Pointer<mojo::internal::Array_Data<bool>>>> array_of_array_of_bools;
  mojo::internal::Pointer<mojo::internal::Array_Data<mojo::internal::Pointer<mojo::internal::Array_Data<mojo::internal::Pointer<mojo::internal::Array_Data<mojo::internal::Pointer<mojo::internal::String_Data>>>>>>> multi_array_of_strings;
  mojo::internal::Pointer<mojo::internal::Array_Data<bool>> array_of_bools;

 private:
  Foo_Data();
  ~Foo_Data() = delete;
};
static_assert(sizeof(Foo_Data) == 96,
              "Bad sizeof(Foo_Data)");
// Used by Foo::WrapAsMessage to lazily serialize the struct.
template <typename UserType, typename DataView>
struct Foo_UnserializedMessageContext
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  Foo_UnserializedMessageContext(
    uint32_t message_name,
    uint32_t message_flags,
    UserType input)
      : mojo::internal::UnserializedMessageContext(&kMessageTag, message_name, message_flags)
      , user_data_(std::move(input)) {}
  ~Foo_UnserializedMessageContext() override = default;

  UserType TakeData() {
    return std::move(user_data_);
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* context,
                 mojo::internal::Buffer* buffer) override {
    Foo_Data::BufferWriter writer;
    mojo::internal::Serialize<DataView>(user_data_, buffer, &writer, context);
  }

  UserType user_data_;
};

template <typename UserType, typename DataView>
const mojo::internal::UnserializedMessageContext::Tag
    Foo_UnserializedMessageContext<UserType, DataView>::kMessageTag = {};
class  DefaultsTest_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(DefaultsTest_Data));
      new (data()) DefaultsTest_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    DefaultsTest_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<DefaultsTest_Data>(index_);
    }
    DefaultsTest_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;
  int8_t a0;
  uint8_t a1;
  int16_t a2;
  uint16_t a3;
  uint8_t a11 : 1;
  uint8_t a12 : 1;
  uint8_t pad5_[1];
  int32_t a4;
  uint32_t a5;
  int64_t a6;
  uint64_t a7;
  int32_t a8;
  int32_t a9;
  int32_t a10;
  float a13;
  double a14;
  double a15;
  double a16;
  double a17;
  mojo::internal::Pointer<mojo::internal::Array_Data<uint8_t>> a18;
  mojo::internal::Pointer<mojo::internal::String_Data> a19;
  int32_t a20;
  float a29;
  mojo::internal::Pointer<::imported::internal::Point_Data> a21;
  mojo::internal::Pointer<::imported::internal::Thing_Data> a22;
  uint64_t a23;
  int64_t a24;
  int64_t a25;
  double a26;
  double a27;
  double a28;
  float a30;
  float a31;

 private:
  DefaultsTest_Data();
  ~DefaultsTest_Data() = delete;
};
static_assert(sizeof(DefaultsTest_Data) == 184,
              "Bad sizeof(DefaultsTest_Data)");
// Used by DefaultsTest::WrapAsMessage to lazily serialize the struct.
template <typename UserType, typename DataView>
struct DefaultsTest_UnserializedMessageContext
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  DefaultsTest_UnserializedMessageContext(
    uint32_t message_name,
    uint32_t message_flags,
    UserType input)
      : mojo::internal::UnserializedMessageContext(&kMessageTag, message_name, message_flags)
      , user_data_(std::move(input)) {}
  ~DefaultsTest_UnserializedMessageContext() override = default;

  UserType TakeData() {
    return std::move(user_data_);
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* context,
                 mojo::internal::Buffer* buffer) override {
    DefaultsTest_Data::BufferWriter writer;
    mojo::internal::Serialize<DataView>(user_data_, buffer, &writer, context);
  }

  UserType user_data_;
};

template <typename UserType, typename DataView>
const mojo::internal::UnserializedMessageContext::Tag
    DefaultsTest_UnserializedMessageContext<UserType, DataView>::kMessageTag = {};
class  StructWithHoleV1_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(StructWithHoleV1_Data));
      new (data()) StructWithHoleV1_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    StructWithHoleV1_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<StructWithHoleV1_Data>(index_);
    }
    StructWithHoleV1_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;
  int32_t v1;
  uint8_t pad0_[4];
  int64_t v2;

 private:
  StructWithHoleV1_Data();
  ~StructWithHoleV1_Data() = delete;
};
static_assert(sizeof(StructWithHoleV1_Data) == 24,
              "Bad sizeof(StructWithHoleV1_Data)");
// Used by StructWithHoleV1::WrapAsMessage to lazily serialize the struct.
template <typename UserType, typename DataView>
struct StructWithHoleV1_UnserializedMessageContext
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  StructWithHoleV1_UnserializedMessageContext(
    uint32_t message_name,
    uint32_t message_flags,
    UserType input)
      : mojo::internal::UnserializedMessageContext(&kMessageTag, message_name, message_flags)
      , user_data_(std::move(input)) {}
  ~StructWithHoleV1_UnserializedMessageContext() override = default;

  UserType TakeData() {
    return std::move(user_data_);
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* context,
                 mojo::internal::Buffer* buffer) override {
    StructWithHoleV1_Data::BufferWriter writer;
    mojo::internal::Serialize<DataView>(user_data_, buffer, &writer, context);
  }

  UserType user_data_;
};

template <typename UserType, typename DataView>
const mojo::internal::UnserializedMessageContext::Tag
    StructWithHoleV1_UnserializedMessageContext<UserType, DataView>::kMessageTag = {};
class  StructWithHoleV2_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(StructWithHoleV2_Data));
      new (data()) StructWithHoleV2_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    StructWithHoleV2_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<StructWithHoleV2_Data>(index_);
    }
    StructWithHoleV2_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;
  int32_t v1;
  int32_t v3;
  int64_t v2;

 private:
  StructWithHoleV2_Data();
  ~StructWithHoleV2_Data() = delete;
};
static_assert(sizeof(StructWithHoleV2_Data) == 24,
              "Bad sizeof(StructWithHoleV2_Data)");
// Used by StructWithHoleV2::WrapAsMessage to lazily serialize the struct.
template <typename UserType, typename DataView>
struct StructWithHoleV2_UnserializedMessageContext
    : public mojo::internal::UnserializedMessageContext {
 public:
  static const mojo::internal::UnserializedMessageContext::Tag kMessageTag;

  StructWithHoleV2_UnserializedMessageContext(
    uint32_t message_name,
    uint32_t message_flags,
    UserType input)
      : mojo::internal::UnserializedMessageContext(&kMessageTag, message_name, message_flags)
      , user_data_(std::move(input)) {}
  ~StructWithHoleV2_UnserializedMessageContext() override = default;

  UserType TakeData() {
    return std::move(user_data_);
  }

 private:
  // mojo::internal::UnserializedMessageContext:
  void Serialize(mojo::internal::SerializationContext* context,
                 mojo::internal::Buffer* buffer) override {
    StructWithHoleV2_Data::BufferWriter writer;
    mojo::internal::Serialize<DataView>(user_data_, buffer, &writer, context);
  }

  UserType user_data_;
};

template <typename UserType, typename DataView>
const mojo::internal::UnserializedMessageContext::Tag
    StructWithHoleV2_UnserializedMessageContext<UserType, DataView>::kMessageTag = {};

#pragma pack(pop)

}  // namespace internal
}  // namespace sample

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_SAMPLE_SERVICE_MOJOM_SHARED_INTERNAL_H_