// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_UNIONS_MOJOM_PARAMS_DATA_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_UNIONS_MOJOM_PARAMS_DATA_H_

#include "base/logging.h"
#include "base/macros.h"
#include "mojo/public/cpp/bindings/lib/bindings_internal.h"
#include "mojo/public/cpp/bindings/lib/buffer.h"
#include "mojo/public/cpp/bindings/lib/validation_context.h"

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4056)
#pragma warning(disable:4065)
#pragma warning(disable:4756)
#endif
namespace mojo {
namespace test {
namespace internal {
class  SmallCache_SetIntValue_Params_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(SmallCache_SetIntValue_Params_Data));
      new (data()) SmallCache_SetIntValue_Params_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    SmallCache_SetIntValue_Params_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<SmallCache_SetIntValue_Params_Data>(index_);
    }
    SmallCache_SetIntValue_Params_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;
  int64_t int_value;

 private:
  SmallCache_SetIntValue_Params_Data();
  ~SmallCache_SetIntValue_Params_Data() = delete;
};
static_assert(sizeof(SmallCache_SetIntValue_Params_Data) == 16,
              "Bad sizeof(SmallCache_SetIntValue_Params_Data)");
class  SmallCache_GetIntValue_Params_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(SmallCache_GetIntValue_Params_Data));
      new (data()) SmallCache_GetIntValue_Params_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    SmallCache_GetIntValue_Params_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<SmallCache_GetIntValue_Params_Data>(index_);
    }
    SmallCache_GetIntValue_Params_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;

 private:
  SmallCache_GetIntValue_Params_Data();
  ~SmallCache_GetIntValue_Params_Data() = delete;
};
static_assert(sizeof(SmallCache_GetIntValue_Params_Data) == 8,
              "Bad sizeof(SmallCache_GetIntValue_Params_Data)");
class  SmallCache_GetIntValue_ResponseParams_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(SmallCache_GetIntValue_ResponseParams_Data));
      new (data()) SmallCache_GetIntValue_ResponseParams_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    SmallCache_GetIntValue_ResponseParams_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<SmallCache_GetIntValue_ResponseParams_Data>(index_);
    }
    SmallCache_GetIntValue_ResponseParams_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;
  int64_t int_value;

 private:
  SmallCache_GetIntValue_ResponseParams_Data();
  ~SmallCache_GetIntValue_ResponseParams_Data() = delete;
};
static_assert(sizeof(SmallCache_GetIntValue_ResponseParams_Data) == 16,
              "Bad sizeof(SmallCache_GetIntValue_ResponseParams_Data)");
class  UnionInterface_Echo_Params_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(UnionInterface_Echo_Params_Data));
      new (data()) UnionInterface_Echo_Params_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    UnionInterface_Echo_Params_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<UnionInterface_Echo_Params_Data>(index_);
    }
    UnionInterface_Echo_Params_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;
  internal::PodUnion_Data in_val;

 private:
  UnionInterface_Echo_Params_Data();
  ~UnionInterface_Echo_Params_Data() = delete;
};
static_assert(sizeof(UnionInterface_Echo_Params_Data) == 24,
              "Bad sizeof(UnionInterface_Echo_Params_Data)");
class  UnionInterface_Echo_ResponseParams_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(UnionInterface_Echo_ResponseParams_Data));
      new (data()) UnionInterface_Echo_ResponseParams_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    UnionInterface_Echo_ResponseParams_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<UnionInterface_Echo_ResponseParams_Data>(index_);
    }
    UnionInterface_Echo_ResponseParams_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;
  internal::PodUnion_Data out_val;

 private:
  UnionInterface_Echo_ResponseParams_Data();
  ~UnionInterface_Echo_ResponseParams_Data() = delete;
};
static_assert(sizeof(UnionInterface_Echo_ResponseParams_Data) == 24,
              "Bad sizeof(UnionInterface_Echo_ResponseParams_Data)");

}  // namespace internal
class SmallCache_SetIntValue_ParamsDataView {
 public:
  SmallCache_SetIntValue_ParamsDataView() {}

  SmallCache_SetIntValue_ParamsDataView(
      internal::SmallCache_SetIntValue_Params_Data* data,
      mojo::internal::SerializationContext* context)
      : data_(data) {}

  bool is_null() const { return !data_; }
  int64_t int_value() const {
    return data_->int_value;
  }
 private:
  internal::SmallCache_SetIntValue_Params_Data* data_ = nullptr;
};

class SmallCache_GetIntValue_ParamsDataView {
 public:
  SmallCache_GetIntValue_ParamsDataView() {}

  SmallCache_GetIntValue_ParamsDataView(
      internal::SmallCache_GetIntValue_Params_Data* data,
      mojo::internal::SerializationContext* context)
      : data_(data) {}

  bool is_null() const { return !data_; }
 private:
  internal::SmallCache_GetIntValue_Params_Data* data_ = nullptr;
};

class SmallCache_GetIntValue_ResponseParamsDataView {
 public:
  SmallCache_GetIntValue_ResponseParamsDataView() {}

  SmallCache_GetIntValue_ResponseParamsDataView(
      internal::SmallCache_GetIntValue_ResponseParams_Data* data,
      mojo::internal::SerializationContext* context)
      : data_(data) {}

  bool is_null() const { return !data_; }
  int64_t int_value() const {
    return data_->int_value;
  }
 private:
  internal::SmallCache_GetIntValue_ResponseParams_Data* data_ = nullptr;
};

class UnionInterface_Echo_ParamsDataView {
 public:
  UnionInterface_Echo_ParamsDataView() {}

  UnionInterface_Echo_ParamsDataView(
      internal::UnionInterface_Echo_Params_Data* data,
      mojo::internal::SerializationContext* context)
      : data_(data), context_(context) {}

  bool is_null() const { return !data_; }
  inline void GetInValDataView(
      PodUnionDataView* output);

  template <typename UserType>
  WARN_UNUSED_RESULT bool ReadInVal(UserType* output) {
    auto* pointer = !data_->in_val.is_null() ? &data_->in_val : nullptr;
    return mojo::internal::Deserialize<::mojo::test::PodUnionDataView>(
        pointer, output, context_);
  }
 private:
  internal::UnionInterface_Echo_Params_Data* data_ = nullptr;
  mojo::internal::SerializationContext* context_ = nullptr;
};

class UnionInterface_Echo_ResponseParamsDataView {
 public:
  UnionInterface_Echo_ResponseParamsDataView() {}

  UnionInterface_Echo_ResponseParamsDataView(
      internal::UnionInterface_Echo_ResponseParams_Data* data,
      mojo::internal::SerializationContext* context)
      : data_(data), context_(context) {}

  bool is_null() const { return !data_; }
  inline void GetOutValDataView(
      PodUnionDataView* output);

  template <typename UserType>
  WARN_UNUSED_RESULT bool ReadOutVal(UserType* output) {
    auto* pointer = !data_->out_val.is_null() ? &data_->out_val : nullptr;
    return mojo::internal::Deserialize<::mojo::test::PodUnionDataView>(
        pointer, output, context_);
  }
 private:
  internal::UnionInterface_Echo_ResponseParams_Data* data_ = nullptr;
  mojo::internal::SerializationContext* context_ = nullptr;
};








inline void UnionInterface_Echo_ParamsDataView::GetInValDataView(
    PodUnionDataView* output) {
  auto pointer = &data_->in_val;
  *output = PodUnionDataView(pointer, context_);
}


inline void UnionInterface_Echo_ResponseParamsDataView::GetOutValDataView(
    PodUnionDataView* output) {
  auto pointer = &data_->out_val;
  *output = PodUnionDataView(pointer, context_);
}

}  // namespace test
}  // namespace mojo

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_UNIONS_MOJOM_PARAMS_DATA_H_