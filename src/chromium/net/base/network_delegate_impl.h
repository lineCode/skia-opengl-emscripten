// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NETWORK_DELEGATE_IMPL_H_
#define NET_BASE_NETWORK_DELEGATE_IMPL_H_

#include <stdint.h>

#include <set>

#include "base/strings/string16.h"
#include "net/base/completion_once_callback.h"
#include "net/base/net_export.h"
#include "net/base/network_delegate.h"
#include "net/cookies/canonical_cookie.h"
#if defined(ENABLE_PROXY)
#include "net/proxy_resolution/proxy_retry_info.h"
#endif

class GURL;

namespace base {
class FilePath;
}

namespace url {
class Origin;
}

namespace net {

class CookieOptions;
class HttpRequestHeaders;
class HttpResponseHeaders;
#if defined(ENABLE_PROXY)
class ProxyInfo;
#endif
class URLRequest;

class NET_EXPORT NetworkDelegateImpl : public NetworkDelegate {
 public:
  ~NetworkDelegateImpl() override {}

 private:
  int OnBeforeURLRequest(URLRequest* request,
                         CompletionOnceCallback callback,
                         GURL* new_url) override;

  int OnBeforeStartTransaction(URLRequest* request,
                               CompletionOnceCallback callback,
                               HttpRequestHeaders* headers) override;

  void OnBeforeSendHeaders(URLRequest* request,

#if defined(ENABLE_PROXY)
                           const ProxyInfo& proxy_info,
                           const ProxyRetryInfoMap& proxy_retry_info,
#endif
                           HttpRequestHeaders* headers) override;

  void OnStartTransaction(URLRequest* request,
                          const HttpRequestHeaders& headers) override;

  int OnHeadersReceived(
      URLRequest* request,
      CompletionOnceCallback callback,
      const HttpResponseHeaders* original_response_headers,
      scoped_refptr<HttpResponseHeaders>* override_response_headers,
      GURL* allowed_unsafe_redirect_url) override;

  void OnBeforeRedirect(URLRequest* request, const GURL& new_location) override;

  void OnResponseStarted(URLRequest* request, int net_error) override;

  void OnNetworkBytesReceived(URLRequest* request,
                              int64_t bytes_received) override;

  void OnNetworkBytesSent(URLRequest* request, int64_t bytes_sent) override;

  void OnCompleted(URLRequest* request, bool started, int net_error) override;

  void OnURLRequestDestroyed(URLRequest* request) override;

  void OnPACScriptError(int line_number, const base::string16& error) override;

  AuthRequiredResponse OnAuthRequired(URLRequest* request,
                                      const AuthChallengeInfo& auth_info,
                                      AuthCallback callback,
                                      AuthCredentials* credentials) override;

  bool OnCanGetCookies(const URLRequest& request,
                       const CookieList& cookie_list,
                       bool allowed_from_caller) override;

  bool OnCanSetCookie(const URLRequest& request,
                      const net::CanonicalCookie& cookie,
                      CookieOptions* options,
                      bool allowed_from_caller) override;

  bool OnCanAccessFile(const URLRequest& request,
                       const base::FilePath& original_path,
                       const base::FilePath& absolute_path) const override;

  bool OnForcePrivacyMode(const GURL& url,
                          const GURL& site_for_cookies) const override;

  bool OnCancelURLRequestWithPolicyViolatingReferrerHeader(
      const URLRequest& request,
      const GURL& target_url,
      const GURL& referrer_url) const override;

  bool OnCanQueueReportingReport(const url::Origin& origin) const override;

  void OnCanSendReportingReports(std::set<url::Origin> origins,
                                 base::OnceCallback<void(std::set<url::Origin>)>
                                     result_callback) const override;

  bool OnCanSetReportingClient(const url::Origin& origin,
                               const GURL& endpoint) const override;

  bool OnCanUseReportingClient(const url::Origin& origin,
                               const GURL& endpoint) const override;
};

}  // namespace net

#endif  // NET_BASE_NETWORK_DELEGATE_IMPL_H_
