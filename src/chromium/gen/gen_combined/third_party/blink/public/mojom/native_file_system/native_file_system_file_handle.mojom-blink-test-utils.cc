// Copyright 2019 The Chromium Authors. All rights reserved.
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


#include "third_party/blink/public/mojom/native_file_system/native_file_system_file_handle.mojom-blink-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"
#include "third_party/blink/public/mojom/blob/blob.mojom-blink.h"
#include "third_party/blink/public/mojom/blob/serialized_blob.mojom-blink.h"
#include "third_party/blink/public/mojom/native_file_system/native_file_system_error.mojom-blink.h"
#include "third_party/blink/public/mojom/native_file_system/native_file_system_transfer_token.mojom-blink.h"


#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_NATIVE_FILE_SYSTEM_NATIVE_FILE_SYSTEM_FILE_HANDLE_MOJOM_BLINK_JUMBO_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_NATIVE_FILE_SYSTEM_NATIVE_FILE_SYSTEM_FILE_HANDLE_MOJOM_BLINK_JUMBO_H_
#include "mojo/public/cpp/base/file_error_mojom_traits.h"
#include "third_party/blink/renderer/platform/blob/serialized_blob_struct_traits.h"
#endif


namespace blink {
namespace mojom {
namespace blink {


void NativeFileSystemFileHandleInterceptorForTesting::AsBlob(AsBlobCallback callback) {
  GetForwardingInterface()->AsBlob(std::move(callback));
}
void NativeFileSystemFileHandleInterceptorForTesting::Remove(RemoveCallback callback) {
  GetForwardingInterface()->Remove(std::move(callback));
}
void NativeFileSystemFileHandleInterceptorForTesting::Write(uint64_t offset, ::blink::mojom::blink::BlobPtr data, WriteCallback callback) {
  GetForwardingInterface()->Write(std::move(offset), std::move(data), std::move(callback));
}
void NativeFileSystemFileHandleInterceptorForTesting::WriteStream(uint64_t offset, mojo::ScopedDataPipeConsumerHandle stream, WriteStreamCallback callback) {
  GetForwardingInterface()->WriteStream(std::move(offset), std::move(stream), std::move(callback));
}
void NativeFileSystemFileHandleInterceptorForTesting::Truncate(uint64_t length, TruncateCallback callback) {
  GetForwardingInterface()->Truncate(std::move(length), std::move(callback));
}
void NativeFileSystemFileHandleInterceptorForTesting::Transfer(::blink::mojom::blink::NativeFileSystemTransferTokenRequest token) {
  GetForwardingInterface()->Transfer(std::move(token));
}
NativeFileSystemFileHandleAsyncWaiter::NativeFileSystemFileHandleAsyncWaiter(
    NativeFileSystemFileHandle* proxy) : proxy_(proxy) {}

NativeFileSystemFileHandleAsyncWaiter::~NativeFileSystemFileHandleAsyncWaiter() = default;

void NativeFileSystemFileHandleAsyncWaiter::AsBlob(
    ::blink::mojom::blink::NativeFileSystemErrorPtr* out_result, scoped_refptr<::blink::BlobDataHandle>* out_blob) {
  base::RunLoop loop;
  proxy_->AsBlob(
      base::BindOnce(
          [](base::RunLoop* loop,
             ::blink::mojom::blink::NativeFileSystemErrorPtr* out_result
,
             scoped_refptr<::blink::BlobDataHandle>* out_blob
,
             ::blink::mojom::blink::NativeFileSystemErrorPtr result,
             const scoped_refptr<::blink::BlobDataHandle>& blob) {*out_result = std::move(result);*out_blob = std::move(blob);
            loop->Quit();
          },
          &loop,
          out_result,
          out_blob));
  loop.Run();
}
void NativeFileSystemFileHandleAsyncWaiter::Remove(
    ::blink::mojom::blink::NativeFileSystemErrorPtr* out_result) {
  base::RunLoop loop;
  proxy_->Remove(
      base::BindOnce(
          [](base::RunLoop* loop,
             ::blink::mojom::blink::NativeFileSystemErrorPtr* out_result
,
             ::blink::mojom::blink::NativeFileSystemErrorPtr result) {*out_result = std::move(result);
            loop->Quit();
          },
          &loop,
          out_result));
  loop.Run();
}
void NativeFileSystemFileHandleAsyncWaiter::Write(
    uint64_t offset, ::blink::mojom::blink::BlobPtr data, ::blink::mojom::blink::NativeFileSystemErrorPtr* out_result, uint64_t* out_bytes_written) {
  base::RunLoop loop;
  proxy_->Write(std::move(offset),std::move(data),
      base::BindOnce(
          [](base::RunLoop* loop,
             ::blink::mojom::blink::NativeFileSystemErrorPtr* out_result
,
             uint64_t* out_bytes_written
,
             ::blink::mojom::blink::NativeFileSystemErrorPtr result,
             uint64_t bytes_written) {*out_result = std::move(result);*out_bytes_written = std::move(bytes_written);
            loop->Quit();
          },
          &loop,
          out_result,
          out_bytes_written));
  loop.Run();
}
void NativeFileSystemFileHandleAsyncWaiter::WriteStream(
    uint64_t offset, mojo::ScopedDataPipeConsumerHandle stream, ::blink::mojom::blink::NativeFileSystemErrorPtr* out_result, uint64_t* out_bytes_written) {
  base::RunLoop loop;
  proxy_->WriteStream(std::move(offset),std::move(stream),
      base::BindOnce(
          [](base::RunLoop* loop,
             ::blink::mojom::blink::NativeFileSystemErrorPtr* out_result
,
             uint64_t* out_bytes_written
,
             ::blink::mojom::blink::NativeFileSystemErrorPtr result,
             uint64_t bytes_written) {*out_result = std::move(result);*out_bytes_written = std::move(bytes_written);
            loop->Quit();
          },
          &loop,
          out_result,
          out_bytes_written));
  loop.Run();
}
void NativeFileSystemFileHandleAsyncWaiter::Truncate(
    uint64_t length, ::blink::mojom::blink::NativeFileSystemErrorPtr* out_result) {
  base::RunLoop loop;
  proxy_->Truncate(std::move(length),
      base::BindOnce(
          [](base::RunLoop* loop,
             ::blink::mojom::blink::NativeFileSystemErrorPtr* out_result
,
             ::blink::mojom::blink::NativeFileSystemErrorPtr result) {*out_result = std::move(result);
            loop->Quit();
          },
          &loop,
          out_result));
  loop.Run();
}





}  // namespace blink
}  // namespace mojom
}  // namespace blink

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif