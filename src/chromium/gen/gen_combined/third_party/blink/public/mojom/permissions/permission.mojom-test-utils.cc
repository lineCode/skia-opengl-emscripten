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


#include "third_party/blink/public/mojom/permissions/permission.mojom-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"
#include "third_party/blink/public/mojom/permissions/permission_status.mojom.h"


#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_PERMISSIONS_PERMISSION_MOJOM_JUMBO_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_PERMISSIONS_PERMISSION_MOJOM_JUMBO_H_
#endif


namespace blink {
namespace mojom {


void PermissionObserverInterceptorForTesting::OnPermissionStatusChange(::blink::mojom::PermissionStatus status) {
  GetForwardingInterface()->OnPermissionStatusChange(std::move(status));
}
PermissionObserverAsyncWaiter::PermissionObserverAsyncWaiter(
    PermissionObserver* proxy) : proxy_(proxy) {}

PermissionObserverAsyncWaiter::~PermissionObserverAsyncWaiter() = default;




void PermissionServiceInterceptorForTesting::HasPermission(PermissionDescriptorPtr permission, HasPermissionCallback callback) {
  GetForwardingInterface()->HasPermission(std::move(permission), std::move(callback));
}
void PermissionServiceInterceptorForTesting::RequestPermission(PermissionDescriptorPtr permission, bool user_gesture, RequestPermissionCallback callback) {
  GetForwardingInterface()->RequestPermission(std::move(permission), std::move(user_gesture), std::move(callback));
}
void PermissionServiceInterceptorForTesting::RequestPermissions(std::vector<PermissionDescriptorPtr> permission, bool user_gesture, RequestPermissionsCallback callback) {
  GetForwardingInterface()->RequestPermissions(std::move(permission), std::move(user_gesture), std::move(callback));
}
void PermissionServiceInterceptorForTesting::RevokePermission(PermissionDescriptorPtr permission, RevokePermissionCallback callback) {
  GetForwardingInterface()->RevokePermission(std::move(permission), std::move(callback));
}
void PermissionServiceInterceptorForTesting::AddPermissionObserver(PermissionDescriptorPtr permission, ::blink::mojom::PermissionStatus last_known_status, PermissionObserverPtr observer) {
  GetForwardingInterface()->AddPermissionObserver(std::move(permission), std::move(last_known_status), std::move(observer));
}
PermissionServiceAsyncWaiter::PermissionServiceAsyncWaiter(
    PermissionService* proxy) : proxy_(proxy) {}

PermissionServiceAsyncWaiter::~PermissionServiceAsyncWaiter() = default;

void PermissionServiceAsyncWaiter::HasPermission(
    PermissionDescriptorPtr permission, ::blink::mojom::PermissionStatus* out_status) {
  base::RunLoop loop;
  proxy_->HasPermission(std::move(permission),
      base::BindOnce(
          [](base::RunLoop* loop,
             ::blink::mojom::PermissionStatus* out_status
,
             ::blink::mojom::PermissionStatus status) {*out_status = std::move(status);
            loop->Quit();
          },
          &loop,
          out_status));
  loop.Run();
}
void PermissionServiceAsyncWaiter::RequestPermission(
    PermissionDescriptorPtr permission, bool user_gesture, ::blink::mojom::PermissionStatus* out_status) {
  base::RunLoop loop;
  proxy_->RequestPermission(std::move(permission),std::move(user_gesture),
      base::BindOnce(
          [](base::RunLoop* loop,
             ::blink::mojom::PermissionStatus* out_status
,
             ::blink::mojom::PermissionStatus status) {*out_status = std::move(status);
            loop->Quit();
          },
          &loop,
          out_status));
  loop.Run();
}
void PermissionServiceAsyncWaiter::RequestPermissions(
    std::vector<PermissionDescriptorPtr> permission, bool user_gesture, std::vector<::blink::mojom::PermissionStatus>* out_statuses) {
  base::RunLoop loop;
  proxy_->RequestPermissions(std::move(permission),std::move(user_gesture),
      base::BindOnce(
          [](base::RunLoop* loop,
             std::vector<::blink::mojom::PermissionStatus>* out_statuses
,
             const std::vector<::blink::mojom::PermissionStatus>& statuses) {*out_statuses = std::move(statuses);
            loop->Quit();
          },
          &loop,
          out_statuses));
  loop.Run();
}
void PermissionServiceAsyncWaiter::RevokePermission(
    PermissionDescriptorPtr permission, ::blink::mojom::PermissionStatus* out_status) {
  base::RunLoop loop;
  proxy_->RevokePermission(std::move(permission),
      base::BindOnce(
          [](base::RunLoop* loop,
             ::blink::mojom::PermissionStatus* out_status
,
             ::blink::mojom::PermissionStatus status) {*out_status = std::move(status);
            loop->Quit();
          },
          &loop,
          out_status));
  loop.Run();
}





}  // namespace mojom
}  // namespace blink

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif