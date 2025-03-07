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


#include "media/mojo/interfaces/platform_verification.mojom-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"


#ifndef MEDIA_MOJO_INTERFACES_PLATFORM_VERIFICATION_MOJOM_JUMBO_H_
#define MEDIA_MOJO_INTERFACES_PLATFORM_VERIFICATION_MOJOM_JUMBO_H_
#endif


namespace media {
namespace mojom {


void PlatformVerificationInterceptorForTesting::ChallengePlatform(const std::string& service_id, const std::string& challenge, ChallengePlatformCallback callback) {
  GetForwardingInterface()->ChallengePlatform(std::move(service_id), std::move(challenge), std::move(callback));
}
void PlatformVerificationInterceptorForTesting::GetStorageId(uint32_t version, GetStorageIdCallback callback) {
  GetForwardingInterface()->GetStorageId(std::move(version), std::move(callback));
}
PlatformVerificationAsyncWaiter::PlatformVerificationAsyncWaiter(
    PlatformVerification* proxy) : proxy_(proxy) {}

PlatformVerificationAsyncWaiter::~PlatformVerificationAsyncWaiter() = default;

void PlatformVerificationAsyncWaiter::ChallengePlatform(
    const std::string& service_id, const std::string& challenge, bool* out_success, std::string* out_signed_data, std::string* out_signed_data_signature, std::string* out_platform_key_certificate) {
  base::RunLoop loop;
  proxy_->ChallengePlatform(std::move(service_id),std::move(challenge),
      base::BindOnce(
          [](base::RunLoop* loop,
             bool* out_success
,
             std::string* out_signed_data
,
             std::string* out_signed_data_signature
,
             std::string* out_platform_key_certificate
,
             bool success,
             const std::string& signed_data,
             const std::string& signed_data_signature,
             const std::string& platform_key_certificate) {*out_success = std::move(success);*out_signed_data = std::move(signed_data);*out_signed_data_signature = std::move(signed_data_signature);*out_platform_key_certificate = std::move(platform_key_certificate);
            loop->Quit();
          },
          &loop,
          out_success,
          out_signed_data,
          out_signed_data_signature,
          out_platform_key_certificate));
  loop.Run();
}
void PlatformVerificationAsyncWaiter::GetStorageId(
    uint32_t version, uint32_t* out_version, std::vector<uint8_t>* out_storage_id) {
  base::RunLoop loop;
  proxy_->GetStorageId(std::move(version),
      base::BindOnce(
          [](base::RunLoop* loop,
             uint32_t* out_version
,
             std::vector<uint8_t>* out_storage_id
,
             uint32_t version,
             const std::vector<uint8_t>& storage_id) {*out_version = std::move(version);*out_storage_id = std::move(storage_id);
            loop->Quit();
          },
          &loop,
          out_version,
          out_storage_id));
  loop.Run();
}





}  // namespace mojom
}  // namespace media

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif