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


#include "services/network/public/mojom/network_change_manager.mojom-blink-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"


#ifndef SERVICES_NETWORK_PUBLIC_MOJOM_NETWORK_CHANGE_MANAGER_MOJOM_BLINK_JUMBO_H_
#define SERVICES_NETWORK_PUBLIC_MOJOM_NETWORK_CHANGE_MANAGER_MOJOM_BLINK_JUMBO_H_
#endif


namespace network {
namespace mojom {
namespace blink {


void NetworkChangeManagerClientInterceptorForTesting::OnInitialConnectionType(ConnectionType type) {
  GetForwardingInterface()->OnInitialConnectionType(std::move(type));
}
void NetworkChangeManagerClientInterceptorForTesting::OnNetworkChanged(ConnectionType type) {
  GetForwardingInterface()->OnNetworkChanged(std::move(type));
}
NetworkChangeManagerClientAsyncWaiter::NetworkChangeManagerClientAsyncWaiter(
    NetworkChangeManagerClient* proxy) : proxy_(proxy) {}

NetworkChangeManagerClientAsyncWaiter::~NetworkChangeManagerClientAsyncWaiter() = default;




void NetworkChangeManagerInterceptorForTesting::RequestNotifications(NetworkChangeManagerClientPtr client_ptr) {
  GetForwardingInterface()->RequestNotifications(std::move(client_ptr));
}
NetworkChangeManagerAsyncWaiter::NetworkChangeManagerAsyncWaiter(
    NetworkChangeManager* proxy) : proxy_(proxy) {}

NetworkChangeManagerAsyncWaiter::~NetworkChangeManagerAsyncWaiter() = default;






}  // namespace blink
}  // namespace mojom
}  // namespace network

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif