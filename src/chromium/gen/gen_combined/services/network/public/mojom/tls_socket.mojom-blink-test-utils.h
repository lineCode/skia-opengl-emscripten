// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_NETWORK_PUBLIC_MOJOM_TLS_SOCKET_MOJOM_BLINK_TEST_UTILS_H_
#define SERVICES_NETWORK_PUBLIC_MOJOM_TLS_SOCKET_MOJOM_BLINK_TEST_UTILS_H_

#include "services/network/public/mojom/tls_socket.mojom-blink.h"
#include "third_party/blink/public/platform/web_common.h"


namespace network {
namespace mojom {
namespace blink {


class BLINK_PLATFORM_EXPORT TLSClientSocketInterceptorForTesting : public TLSClientSocket {
  virtual TLSClientSocket* GetForwardingInterface() = 0;
};
class BLINK_PLATFORM_EXPORT TLSClientSocketAsyncWaiter {
 public:
  explicit TLSClientSocketAsyncWaiter(TLSClientSocket* proxy);
  ~TLSClientSocketAsyncWaiter();

 private:
  TLSClientSocket* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(TLSClientSocketAsyncWaiter);
};




}  // namespace blink
}  // namespace mojom
}  // namespace network

#endif  // SERVICES_NETWORK_PUBLIC_MOJOM_TLS_SOCKET_MOJOM_BLINK_TEST_UTILS_H_