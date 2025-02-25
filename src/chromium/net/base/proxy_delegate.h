// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_PROXY_DELEGATE_H_
#define NET_BASE_PROXY_DELEGATE_H_

#include <string>

#include "base/macros.h"
#include "net/base/net_errors.h"
#include "net/base/net_export.h"
#if defined(ENABLE_PROXY)
#include "net/proxy_resolution/proxy_retry_info.h"
#endif
class GURL;

namespace net {

class HttpRequestHeaders;
class HttpResponseHeaders;
#if defined(ENABLE_PROXY)
class ProxyInfo;
#endif
class ProxyServer;

// Delegate for setting up a connection.
class NET_EXPORT ProxyDelegate {
 public:
  ProxyDelegate() {
  }

  virtual ~ProxyDelegate() {
  }

#if defined(ENABLE_PROXY)
  // Called as the proxy is being resolved for |url| for a |method| request.
  // The caller may pass an empty string to get method agnostic resoulution.
  // Allows the delegate to override the proxy resolution decision made by
  // ProxyResolutionService. The delegate may override the decision by modifying
  // the ProxyInfo |result|.
  virtual void OnResolveProxy(const GURL& url,
                              const std::string& method,
                              const ProxyRetryInfoMap& proxy_retry_info,
                              ProxyInfo* result) = 0;
#endif

  // Called when use of |bad_proxy| fails due to |net_error|. |net_error| is
  // the network error encountered, if any, and OK if the fallback was
  // for a reason other than a network error (e.g. the proxy service was
  // explicitly directed to skip a proxy).
  virtual void OnFallback(const ProxyServer& bad_proxy,
                          int net_error) = 0;

  // Called immediately before a HTTP/1.x proxy tunnel request is sent.
  // Provides the embedder an opportunity to add extra request headers.
  // Not called for HTTP/2 or QUIC tunnels.
  virtual void OnBeforeHttp1TunnelRequest(
      const ProxyServer& proxy_server,
      HttpRequestHeaders* extra_headers) = 0;

  // Called when the response headers for the HTTP/1.x proxy tunnel request
  // have been received. Allows the delegate to override the net error code of
  // the tunnel request. Returning OK causes the standard tunnel response
  // handling to be performed. Implementations should make sure they can trust
  // |proxy_server| before making decisions based on |response_headers|.
  // Not called for HTTP/2 or QUIC tunnels.
  virtual Error OnHttp1TunnelHeadersReceived(
      const ProxyServer& proxy_server,
      const HttpResponseHeaders& response_headers) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ProxyDelegate);
};

}  // namespace net

#endif  // NET_BASE_PROXY_DELEGATE_H_
