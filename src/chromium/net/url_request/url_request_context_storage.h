// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_CONTEXT_STORAGE_H_
#define NET_URL_REQUEST_URL_REQUEST_CONTEXT_STORAGE_H_

#include <memory>

#include "base/macros.h"
#include "base/memory/ref_counted.h"
#include "build/buildflag.h"
#include "net/base/net_export.h"
#include "net/net_buildflags.h"

namespace net {

#if defined(ENABLE_BORINGSSL)
class CertVerifier;
#endif // ENABLE_BORINGSSL

class CookieStore;

#if defined(ENABLE_BORINGSSL)
class CTPolicyEnforcer;
class CTVerifier;
#endif // ENABLE_BORINGSSL

#if defined(ENABLE_DNS)
class HostResolver;
#endif
class HttpAuthHandlerFactory;
class HttpNetworkSession;
class HttpServerProperties;
class HttpTransactionFactory;
class HttpUserAgentSettings;
class NetLog;
class NetworkDelegate;
class ProxyDelegate;
#if defined(ENABLE_PROXY)
class ProxyResolutionService;
#endif

#if defined(ENABLE_BORINGSSL)
class SSLConfigService;
#endif // ENABLE_BORINGSSL

class TransportSecurityState;
class URLRequestContext;
class URLRequestJobFactory;
class URLRequestThrottlerManager;

#if BUILDFLAG(ENABLE_REPORTING)
class NetworkErrorLoggingService;
class ReportingService;
#endif  // BUILDFLAG(ENABLE_REPORTING)

// URLRequestContextStorage is a helper class that provides storage for unowned
// member variables of URLRequestContext.
class NET_EXPORT URLRequestContextStorage {
 public:
  // Note that URLRequestContextStorage does not acquire a reference to
  // URLRequestContext, since it is often designed to be embedded in a
  // URLRequestContext subclass.
  explicit URLRequestContextStorage(URLRequestContext* context);
  ~URLRequestContextStorage();

  // These setters will set both the member variables and call the setter on the
  // URLRequestContext object. In all cases, ownership is passed to |this|.

  void set_net_log(std::unique_ptr<NetLog> net_log);

#if defined(ENABLE_DNS)
  void set_host_resolver(std::unique_ptr<HostResolver> host_resolver);
#endif

#if defined(ENABLE_BORINGSSL)
  void set_cert_verifier(std::unique_ptr<CertVerifier> cert_verifier);
#endif // ENABLE_BORINGSSL
  void set_http_auth_handler_factory(
      std::unique_ptr<HttpAuthHandlerFactory> http_auth_handler_factory);
  void set_proxy_delegate(std::unique_ptr<ProxyDelegate> proxy_delegate);
  void set_network_delegate(std::unique_ptr<NetworkDelegate> network_delegate);

#if defined(ENABLE_PROXY)
  void set_proxy_resolution_service(
      std::unique_ptr<ProxyResolutionService> proxy_resolution_service);
#endif
#if defined(ENABLE_BORINGSSL)
  void set_ssl_config_service(
      std::unique_ptr<SSLConfigService> ssl_config_service);
#endif // ENABLE_BORINGSSL
  void set_http_server_properties(
      std::unique_ptr<HttpServerProperties> http_server_properties);
  void set_cookie_store(std::unique_ptr<CookieStore> cookie_store);
  void set_transport_security_state(
      std::unique_ptr<TransportSecurityState> transport_security_state);

#if defined(ENABLE_BORINGSSL)
  void set_cert_transparency_verifier(
      std::unique_ptr<CTVerifier> cert_transparency_verifier);
  void set_ct_policy_enforcer(
      std::unique_ptr<CTPolicyEnforcer> ct_policy_enforcer);
#endif // ENABLE_BORINGSSL

  void set_http_network_session(
      std::unique_ptr<HttpNetworkSession> http_network_session);
  void set_http_transaction_factory(
      std::unique_ptr<HttpTransactionFactory> http_transaction_factory);
  void set_job_factory(std::unique_ptr<URLRequestJobFactory> job_factory);
  void set_throttler_manager(
      std::unique_ptr<URLRequestThrottlerManager> throttler_manager);
  void set_http_user_agent_settings(
      std::unique_ptr<HttpUserAgentSettings> http_user_agent_settings);

#if BUILDFLAG(ENABLE_REPORTING)
  void set_reporting_service(
      std::unique_ptr<ReportingService> reporting_service);

  void set_network_error_logging_service(
      std::unique_ptr<NetworkErrorLoggingService>
          network_error_logging_service);
#endif  // BUILDFLAG(ENABLE_REPORTING)

  // Everything else can be access through the URLRequestContext, but this
  // cannot.  Having an accessor for it makes usage a little cleaner.
  HttpNetworkSession* http_network_session() const {
    return http_network_session_.get();
  }

 private:
  // Not owned.
  URLRequestContext* const context_;

  // Owned members.
  std::unique_ptr<NetLog> net_log_;
#if defined(ENABLE_DNS)
  std::unique_ptr<HostResolver> host_resolver_;
#endif

#if defined(ENABLE_BORINGSSL)
  std::unique_ptr<CertVerifier> cert_verifier_;
#endif // ENABLE_BORINGSSL
  std::unique_ptr<HttpAuthHandlerFactory> http_auth_handler_factory_;
  std::unique_ptr<ProxyDelegate> proxy_delegate_;
  std::unique_ptr<NetworkDelegate> network_delegate_;
#if defined(ENABLE_PROXY)
  std::unique_ptr<ProxyResolutionService> proxy_resolution_service_;
#endif
#if defined(ENABLE_BORINGSSL)
  std::unique_ptr<SSLConfigService> ssl_config_service_;
#endif // ENABLE_BORINGSSL
  std::unique_ptr<HttpServerProperties> http_server_properties_;
  std::unique_ptr<HttpUserAgentSettings> http_user_agent_settings_;
  std::unique_ptr<CookieStore> cookie_store_;
  std::unique_ptr<TransportSecurityState> transport_security_state_;

#if defined(ENABLE_BORINGSSL)
  std::unique_ptr<CTVerifier> cert_transparency_verifier_;
  std::unique_ptr<CTPolicyEnforcer> ct_policy_enforcer_;
#endif // ENABLE_BORINGSSL

  // Not actually pointed at by the URLRequestContext, but may be used (but not
  // owned) by the HttpTransactionFactory.
  std::unique_ptr<HttpNetworkSession> http_network_session_;

  std::unique_ptr<HttpTransactionFactory> http_transaction_factory_;
  std::unique_ptr<URLRequestJobFactory> job_factory_;
  std::unique_ptr<URLRequestThrottlerManager> throttler_manager_;

#if BUILDFLAG(ENABLE_REPORTING)
  std::unique_ptr<ReportingService> reporting_service_;
  std::unique_ptr<NetworkErrorLoggingService> network_error_logging_service_;
#endif  // BUILDFLAG(ENABLE_REPORTING)

  DISALLOW_COPY_AND_ASSIGN(URLRequestContextStorage);
};

}  // namespace net

#endif  // NET_URL_REQUEST_URL_REQUEST_CONTEXT_STORAGE_H_
