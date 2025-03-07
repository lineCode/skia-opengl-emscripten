// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_SERVICE_WORKER_SERVICE_WORKER_STREAM_HANDLE_MOJOM_PARAMS_DATA_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_SERVICE_WORKER_SERVICE_WORKER_STREAM_HANDLE_MOJOM_PARAMS_DATA_H_

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
namespace blink {
namespace mojom {
namespace internal {
class COMPONENT_EXPORT(MOJOM_SHARED_BLINK_COMMON_EXPORT) ServiceWorkerStreamCallback_OnCompleted_Params_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(ServiceWorkerStreamCallback_OnCompleted_Params_Data));
      new (data()) ServiceWorkerStreamCallback_OnCompleted_Params_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    ServiceWorkerStreamCallback_OnCompleted_Params_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<ServiceWorkerStreamCallback_OnCompleted_Params_Data>(index_);
    }
    ServiceWorkerStreamCallback_OnCompleted_Params_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;

 private:
  ServiceWorkerStreamCallback_OnCompleted_Params_Data();
  ~ServiceWorkerStreamCallback_OnCompleted_Params_Data() = delete;
};
static_assert(sizeof(ServiceWorkerStreamCallback_OnCompleted_Params_Data) == 8,
              "Bad sizeof(ServiceWorkerStreamCallback_OnCompleted_Params_Data)");
class COMPONENT_EXPORT(MOJOM_SHARED_BLINK_COMMON_EXPORT) ServiceWorkerStreamCallback_OnAborted_Params_Data {
 public:
  class BufferWriter {
   public:
    BufferWriter() = default;

    void Allocate(mojo::internal::Buffer* serialization_buffer) {
      serialization_buffer_ = serialization_buffer;
      index_ = serialization_buffer_->Allocate(sizeof(ServiceWorkerStreamCallback_OnAborted_Params_Data));
      new (data()) ServiceWorkerStreamCallback_OnAborted_Params_Data();
    }

    bool is_null() const { return !serialization_buffer_; }
    ServiceWorkerStreamCallback_OnAborted_Params_Data* data() {
      DCHECK(!is_null());
      return serialization_buffer_->Get<ServiceWorkerStreamCallback_OnAborted_Params_Data>(index_);
    }
    ServiceWorkerStreamCallback_OnAborted_Params_Data* operator->() { return data(); }

   private:
    mojo::internal::Buffer* serialization_buffer_ = nullptr;
    size_t index_ = 0;

    DISALLOW_COPY_AND_ASSIGN(BufferWriter);
  };

  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);

  mojo::internal::StructHeader header_;

 private:
  ServiceWorkerStreamCallback_OnAborted_Params_Data();
  ~ServiceWorkerStreamCallback_OnAborted_Params_Data() = delete;
};
static_assert(sizeof(ServiceWorkerStreamCallback_OnAborted_Params_Data) == 8,
              "Bad sizeof(ServiceWorkerStreamCallback_OnAborted_Params_Data)");

}  // namespace internal
class ServiceWorkerStreamCallback_OnCompleted_ParamsDataView {
 public:
  ServiceWorkerStreamCallback_OnCompleted_ParamsDataView() {}

  ServiceWorkerStreamCallback_OnCompleted_ParamsDataView(
      internal::ServiceWorkerStreamCallback_OnCompleted_Params_Data* data,
      mojo::internal::SerializationContext* context)
      : data_(data) {}

  bool is_null() const { return !data_; }
 private:
  internal::ServiceWorkerStreamCallback_OnCompleted_Params_Data* data_ = nullptr;
};

class ServiceWorkerStreamCallback_OnAborted_ParamsDataView {
 public:
  ServiceWorkerStreamCallback_OnAborted_ParamsDataView() {}

  ServiceWorkerStreamCallback_OnAborted_ParamsDataView(
      internal::ServiceWorkerStreamCallback_OnAborted_Params_Data* data,
      mojo::internal::SerializationContext* context)
      : data_(data) {}

  bool is_null() const { return !data_; }
 private:
  internal::ServiceWorkerStreamCallback_OnAborted_Params_Data* data_ = nullptr;
};





}  // namespace mojom
}  // namespace blink

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_SERVICE_WORKER_SERVICE_WORKER_STREAM_HANDLE_MOJOM_PARAMS_DATA_H_