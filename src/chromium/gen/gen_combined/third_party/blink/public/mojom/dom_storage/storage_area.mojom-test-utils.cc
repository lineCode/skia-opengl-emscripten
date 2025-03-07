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


#include "third_party/blink/public/mojom/dom_storage/storage_area.mojom-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"


#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_DOM_STORAGE_STORAGE_AREA_MOJOM_JUMBO_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_DOM_STORAGE_STORAGE_AREA_MOJOM_JUMBO_H_
#endif


namespace blink {
namespace mojom {


void StorageAreaObserverInterceptorForTesting::KeyAdded(const std::vector<uint8_t>& key, const std::vector<uint8_t>& value, const std::string& source) {
  GetForwardingInterface()->KeyAdded(std::move(key), std::move(value), std::move(source));
}
void StorageAreaObserverInterceptorForTesting::KeyChanged(const std::vector<uint8_t>& key, const std::vector<uint8_t>& new_value, const std::vector<uint8_t>& old_value, const std::string& source) {
  GetForwardingInterface()->KeyChanged(std::move(key), std::move(new_value), std::move(old_value), std::move(source));
}
void StorageAreaObserverInterceptorForTesting::KeyDeleted(const std::vector<uint8_t>& key, const std::vector<uint8_t>& old_value, const std::string& source) {
  GetForwardingInterface()->KeyDeleted(std::move(key), std::move(old_value), std::move(source));
}
void StorageAreaObserverInterceptorForTesting::AllDeleted(const std::string& source) {
  GetForwardingInterface()->AllDeleted(std::move(source));
}
void StorageAreaObserverInterceptorForTesting::ShouldSendOldValueOnMutations(bool value) {
  GetForwardingInterface()->ShouldSendOldValueOnMutations(std::move(value));
}
StorageAreaObserverAsyncWaiter::StorageAreaObserverAsyncWaiter(
    StorageAreaObserver* proxy) : proxy_(proxy) {}

StorageAreaObserverAsyncWaiter::~StorageAreaObserverAsyncWaiter() = default;




void StorageAreaGetAllCallbackInterceptorForTesting::Complete(bool success) {
  GetForwardingInterface()->Complete(std::move(success));
}
StorageAreaGetAllCallbackAsyncWaiter::StorageAreaGetAllCallbackAsyncWaiter(
    StorageAreaGetAllCallback* proxy) : proxy_(proxy) {}

StorageAreaGetAllCallbackAsyncWaiter::~StorageAreaGetAllCallbackAsyncWaiter() = default;




void StorageAreaInterceptorForTesting::AddObserver(StorageAreaObserverAssociatedPtrInfo observer) {
  GetForwardingInterface()->AddObserver(std::move(observer));
}
void StorageAreaInterceptorForTesting::Put(const std::vector<uint8_t>& key, const std::vector<uint8_t>& value, const base::Optional<std::vector<uint8_t>>& client_old_value, const std::string& source, PutCallback callback) {
  GetForwardingInterface()->Put(std::move(key), std::move(value), std::move(client_old_value), std::move(source), std::move(callback));
}
void StorageAreaInterceptorForTesting::Delete(const std::vector<uint8_t>& key, const base::Optional<std::vector<uint8_t>>& client_old_value, const std::string& source, DeleteCallback callback) {
  GetForwardingInterface()->Delete(std::move(key), std::move(client_old_value), std::move(source), std::move(callback));
}
void StorageAreaInterceptorForTesting::DeleteAll(const std::string& source, DeleteAllCallback callback) {
  GetForwardingInterface()->DeleteAll(std::move(source), std::move(callback));
}
void StorageAreaInterceptorForTesting::Get(const std::vector<uint8_t>& key, GetCallback callback) {
  GetForwardingInterface()->Get(std::move(key), std::move(callback));
}
void StorageAreaInterceptorForTesting::GetAll(StorageAreaGetAllCallbackAssociatedPtrInfo complete_callback, GetAllCallback callback) {
  GetForwardingInterface()->GetAll(std::move(complete_callback), std::move(callback));
}
StorageAreaAsyncWaiter::StorageAreaAsyncWaiter(
    StorageArea* proxy) : proxy_(proxy) {}

StorageAreaAsyncWaiter::~StorageAreaAsyncWaiter() = default;

void StorageAreaAsyncWaiter::Put(
    const std::vector<uint8_t>& key, const std::vector<uint8_t>& value, const base::Optional<std::vector<uint8_t>>& client_old_value, const std::string& source, bool* out_success) {
  base::RunLoop loop;
  proxy_->Put(std::move(key),std::move(value),std::move(client_old_value),std::move(source),
      base::BindOnce(
          [](base::RunLoop* loop,
             bool* out_success
,
             bool success) {*out_success = std::move(success);
            loop->Quit();
          },
          &loop,
          out_success));
  loop.Run();
}
void StorageAreaAsyncWaiter::Delete(
    const std::vector<uint8_t>& key, const base::Optional<std::vector<uint8_t>>& client_old_value, const std::string& source, bool* out_success) {
  base::RunLoop loop;
  proxy_->Delete(std::move(key),std::move(client_old_value),std::move(source),
      base::BindOnce(
          [](base::RunLoop* loop,
             bool* out_success
,
             bool success) {*out_success = std::move(success);
            loop->Quit();
          },
          &loop,
          out_success));
  loop.Run();
}
void StorageAreaAsyncWaiter::DeleteAll(
    const std::string& source, bool* out_success) {
  base::RunLoop loop;
  proxy_->DeleteAll(std::move(source),
      base::BindOnce(
          [](base::RunLoop* loop,
             bool* out_success
,
             bool success) {*out_success = std::move(success);
            loop->Quit();
          },
          &loop,
          out_success));
  loop.Run();
}
void StorageAreaAsyncWaiter::Get(
    const std::vector<uint8_t>& key, bool* out_success, std::vector<uint8_t>* out_value) {
  base::RunLoop loop;
  proxy_->Get(std::move(key),
      base::BindOnce(
          [](base::RunLoop* loop,
             bool* out_success
,
             std::vector<uint8_t>* out_value
,
             bool success,
             const std::vector<uint8_t>& value) {*out_success = std::move(success);*out_value = std::move(value);
            loop->Quit();
          },
          &loop,
          out_success,
          out_value));
  loop.Run();
}
void StorageAreaAsyncWaiter::GetAll(
    StorageAreaGetAllCallbackAssociatedPtrInfo complete_callback, bool* out_success, std::vector<KeyValuePtr>* out_data) {
  base::RunLoop loop;
  proxy_->GetAll(std::move(complete_callback),
      base::BindOnce(
          [](base::RunLoop* loop,
             bool* out_success
,
             std::vector<KeyValuePtr>* out_data
,
             bool success,
             std::vector<KeyValuePtr> data) {*out_success = std::move(success);*out_data = std::move(data);
            loop->Quit();
          },
          &loop,
          out_success,
          out_data));
  loop.Run();
}





}  // namespace mojom
}  // namespace blink

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif