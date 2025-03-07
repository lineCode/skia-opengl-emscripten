// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MOJO_INTERFACES_CDM_STORAGE_MOJOM_PARAMS_DATA_H_
#define MEDIA_MOJO_INTERFACES_CDM_STORAGE_MOJOM_PARAMS_DATA_H_

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
namespace media {
namespace mojom {
namespace internal {
class  CdmStorage_Open_Params_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(CdmStorage_Open_Params_Data));
      new (data()) CdmStorage_Open_Params_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    CdmStorage_Open_Params_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<CdmStorage_Open_Params_Data>(index_);
    }
    CdmStorage_Open_Params_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;
  mojo::internal::Pointer<mojo::internal::String_Data> file_name;

 private:
  CdmStorage_Open_Params_Data();
  ~CdmStorage_Open_Params_Data() = delete;
};
static_assert(sizeof(CdmStorage_Open_Params_Data) == 16,
              "Bad sizeof(CdmStorage_Open_Params_Data)");
class  CdmStorage_Open_ResponseParams_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(CdmStorage_Open_ResponseParams_Data));
      new (data()) CdmStorage_Open_ResponseParams_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    CdmStorage_Open_ResponseParams_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<CdmStorage_Open_ResponseParams_Data>(index_);
    }
    CdmStorage_Open_ResponseParams_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;
  int32_t status;
  uint8_t pad0_[4];
  mojo::internal::Pointer<::mojo_base::mojom::internal::File_Data> file_for_reading;
  mojo::internal::AssociatedInterface_Data cdm_file;

 private:
  CdmStorage_Open_ResponseParams_Data();
  ~CdmStorage_Open_ResponseParams_Data() = delete;
};
static_assert(sizeof(CdmStorage_Open_ResponseParams_Data) == 32,
              "Bad sizeof(CdmStorage_Open_ResponseParams_Data)");
class  CdmFile_OpenFileForWriting_Params_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(CdmFile_OpenFileForWriting_Params_Data));
      new (data()) CdmFile_OpenFileForWriting_Params_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    CdmFile_OpenFileForWriting_Params_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<CdmFile_OpenFileForWriting_Params_Data>(index_);
    }
    CdmFile_OpenFileForWriting_Params_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;

 private:
  CdmFile_OpenFileForWriting_Params_Data();
  ~CdmFile_OpenFileForWriting_Params_Data() = delete;
};
static_assert(sizeof(CdmFile_OpenFileForWriting_Params_Data) == 8,
              "Bad sizeof(CdmFile_OpenFileForWriting_Params_Data)");
class  CdmFile_OpenFileForWriting_ResponseParams_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(CdmFile_OpenFileForWriting_ResponseParams_Data));
      new (data()) CdmFile_OpenFileForWriting_ResponseParams_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    CdmFile_OpenFileForWriting_ResponseParams_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<CdmFile_OpenFileForWriting_ResponseParams_Data>(index_);
    }
    CdmFile_OpenFileForWriting_ResponseParams_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;
  mojo::internal::Pointer<::mojo_base::mojom::internal::File_Data> file_for_writing;

 private:
  CdmFile_OpenFileForWriting_ResponseParams_Data();
  ~CdmFile_OpenFileForWriting_ResponseParams_Data() = delete;
};
static_assert(sizeof(CdmFile_OpenFileForWriting_ResponseParams_Data) == 16,
              "Bad sizeof(CdmFile_OpenFileForWriting_ResponseParams_Data)");
class  CdmFile_CommitWrite_Params_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(CdmFile_CommitWrite_Params_Data));
      new (data()) CdmFile_CommitWrite_Params_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    CdmFile_CommitWrite_Params_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<CdmFile_CommitWrite_Params_Data>(index_);
    }
    CdmFile_CommitWrite_Params_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;

 private:
  CdmFile_CommitWrite_Params_Data();
  ~CdmFile_CommitWrite_Params_Data() = delete;
};
static_assert(sizeof(CdmFile_CommitWrite_Params_Data) == 8,
              "Bad sizeof(CdmFile_CommitWrite_Params_Data)");
class  CdmFile_CommitWrite_ResponseParams_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(CdmFile_CommitWrite_ResponseParams_Data));
      new (data()) CdmFile_CommitWrite_ResponseParams_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    CdmFile_CommitWrite_ResponseParams_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<CdmFile_CommitWrite_ResponseParams_Data>(index_);
    }
    CdmFile_CommitWrite_ResponseParams_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;
  mojo::internal::Pointer<::mojo_base::mojom::internal::File_Data> updated_file_for_reading;

 private:
  CdmFile_CommitWrite_ResponseParams_Data();
  ~CdmFile_CommitWrite_ResponseParams_Data() = delete;
};
static_assert(sizeof(CdmFile_CommitWrite_ResponseParams_Data) == 16,
              "Bad sizeof(CdmFile_CommitWrite_ResponseParams_Data)");

}  // namespace internal
class CdmStorage_Open_ParamsDataView {
 public:
  CdmStorage_Open_ParamsDataView() {}

  CdmStorage_Open_ParamsDataView(
      internal::CdmStorage_Open_Params_Data* data,
      mojo::internal::SerializationContext* context)
      : data_(data), context_(context) {}

  bool is_null() const { return !data_; }
  inline void GetFileNameDataView(
      mojo::StringDataView* output);

  template <typename UserType>
  WARN_UNUSED_RESULT bool ReadFileName(UserType* output) {
    auto* pointer = data_->file_name.Get();
    return mojo::internal::Deserialize<mojo::StringDataView>(
        pointer, output, context_);
  }
 private:
  internal::CdmStorage_Open_Params_Data* data_ = nullptr;
  mojo::internal::SerializationContext* context_ = nullptr;
};

class CdmStorage_Open_ResponseParamsDataView {
 public:
  CdmStorage_Open_ResponseParamsDataView() {}

  CdmStorage_Open_ResponseParamsDataView(
      internal::CdmStorage_Open_ResponseParams_Data* data,
      mojo::internal::SerializationContext* context)
      : data_(data), context_(context) {}

  bool is_null() const { return !data_; }
  template <typename UserType>
  WARN_UNUSED_RESULT bool ReadStatus(UserType* output) const {
    auto data_value = data_->status;
    return mojo::internal::Deserialize<::media::mojom::CdmStorage_Status>(
        data_value, output);
  }

  CdmStorage_Status status() const {
    return static_cast<CdmStorage_Status>(data_->status);
  }
  inline void GetFileForReadingDataView(
      ::mojo_base::mojom::FileDataView* output);

  template <typename UserType>
  WARN_UNUSED_RESULT bool ReadFileForReading(UserType* output) {
    auto* pointer = data_->file_for_reading.Get();
    return mojo::internal::Deserialize<::mojo_base::mojom::FileDataView>(
        pointer, output, context_);
  }
  template <typename UserType>
  UserType TakeCdmFile() {
    UserType result;
    bool ret =
        mojo::internal::Deserialize<::media::mojom::CdmFileAssociatedPtrInfoDataView>(
            &data_->cdm_file, &result, context_);
    DCHECK(ret);
    return result;
  }
 private:
  internal::CdmStorage_Open_ResponseParams_Data* data_ = nullptr;
  mojo::internal::SerializationContext* context_ = nullptr;
};

class CdmFile_OpenFileForWriting_ParamsDataView {
 public:
  CdmFile_OpenFileForWriting_ParamsDataView() {}

  CdmFile_OpenFileForWriting_ParamsDataView(
      internal::CdmFile_OpenFileForWriting_Params_Data* data,
      mojo::internal::SerializationContext* context)
      : data_(data) {}

  bool is_null() const { return !data_; }
 private:
  internal::CdmFile_OpenFileForWriting_Params_Data* data_ = nullptr;
};

class CdmFile_OpenFileForWriting_ResponseParamsDataView {
 public:
  CdmFile_OpenFileForWriting_ResponseParamsDataView() {}

  CdmFile_OpenFileForWriting_ResponseParamsDataView(
      internal::CdmFile_OpenFileForWriting_ResponseParams_Data* data,
      mojo::internal::SerializationContext* context)
      : data_(data), context_(context) {}

  bool is_null() const { return !data_; }
  inline void GetFileForWritingDataView(
      ::mojo_base::mojom::FileDataView* output);

  template <typename UserType>
  WARN_UNUSED_RESULT bool ReadFileForWriting(UserType* output) {
    auto* pointer = data_->file_for_writing.Get();
    return mojo::internal::Deserialize<::mojo_base::mojom::FileDataView>(
        pointer, output, context_);
  }
 private:
  internal::CdmFile_OpenFileForWriting_ResponseParams_Data* data_ = nullptr;
  mojo::internal::SerializationContext* context_ = nullptr;
};

class CdmFile_CommitWrite_ParamsDataView {
 public:
  CdmFile_CommitWrite_ParamsDataView() {}

  CdmFile_CommitWrite_ParamsDataView(
      internal::CdmFile_CommitWrite_Params_Data* data,
      mojo::internal::SerializationContext* context)
      : data_(data) {}

  bool is_null() const { return !data_; }
 private:
  internal::CdmFile_CommitWrite_Params_Data* data_ = nullptr;
};

class CdmFile_CommitWrite_ResponseParamsDataView {
 public:
  CdmFile_CommitWrite_ResponseParamsDataView() {}

  CdmFile_CommitWrite_ResponseParamsDataView(
      internal::CdmFile_CommitWrite_ResponseParams_Data* data,
      mojo::internal::SerializationContext* context)
      : data_(data), context_(context) {}

  bool is_null() const { return !data_; }
  inline void GetUpdatedFileForReadingDataView(
      ::mojo_base::mojom::FileDataView* output);

  template <typename UserType>
  WARN_UNUSED_RESULT bool ReadUpdatedFileForReading(UserType* output) {
    auto* pointer = data_->updated_file_for_reading.Get();
    return mojo::internal::Deserialize<::mojo_base::mojom::FileDataView>(
        pointer, output, context_);
  }
 private:
  internal::CdmFile_CommitWrite_ResponseParams_Data* data_ = nullptr;
  mojo::internal::SerializationContext* context_ = nullptr;
};


inline void CdmStorage_Open_ParamsDataView::GetFileNameDataView(
    mojo::StringDataView* output) {
  auto pointer = data_->file_name.Get();
  *output = mojo::StringDataView(pointer, context_);
}


inline void CdmStorage_Open_ResponseParamsDataView::GetFileForReadingDataView(
    ::mojo_base::mojom::FileDataView* output) {
  auto pointer = data_->file_for_reading.Get();
  *output = ::mojo_base::mojom::FileDataView(pointer, context_);
}




inline void CdmFile_OpenFileForWriting_ResponseParamsDataView::GetFileForWritingDataView(
    ::mojo_base::mojom::FileDataView* output) {
  auto pointer = data_->file_for_writing.Get();
  *output = ::mojo_base::mojom::FileDataView(pointer, context_);
}




inline void CdmFile_CommitWrite_ResponseParamsDataView::GetUpdatedFileForReadingDataView(
    ::mojo_base::mojom::FileDataView* output) {
  auto pointer = data_->updated_file_for_reading.Get();
  *output = ::mojo_base::mojom::FileDataView(pointer, context_);
}

}  // namespace mojom
}  // namespace media

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif  // MEDIA_MOJO_INTERFACES_CDM_STORAGE_MOJOM_PARAMS_DATA_H_