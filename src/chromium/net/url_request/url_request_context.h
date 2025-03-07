// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This class represents contextual information (cookies, cache, etc.)
// that's necessary when processing resource requests.

#ifndef NET_URL_REQUEST_URL_REQUEST_CONTEXT_H_
#define NET_URL_REQUEST_URL_REQUEST_CONTEXT_H_

#include <memory>
#include <set>
#include <string>

#include "base/macros.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"
#include "base/trace_event/memory_dump_provider.h"
#include "net/base/net_export.h"
#include "net/base/request_priority.h"
#include "net/http/http_network_session.h"
#include "net/http/http_server_properties.h"
#include "net/http/transport_security_state.h"
#include "net/net_buildflags.h"
#include "net/traffic_annotation/network_traffic_annotation.h"
#include "net/url_request/url_request.h"

class ChromeBrowserStateImplIOData;
class ProfileImplIOData;

namespace base {
namespace trace_event {
class ProcessMemoryDump;
}
}

namespace safe_browsing {
class SafeBrowsingURLRequestContextGetter;
}  // namespace safe_browsing

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
class HttpTransactionFactory;
class HttpUserAgentSettings;
class NetLog;
class NetworkDelegate;
#if defined(ENABLE_NQE)
class NetworkQualityEstimator;
#endif
class ProxyDelegate;
#if defined(ENABLE_PROXY)
class ProxyResolutionService;
#endif

#if defined(ENABLE_BORINGSSL)
class SSLConfigService;
#endif // ENABLE_BORINGSSL

class URLRequest;
class URLRequestJobFactory;
class URLRequestThrottlerManager;

#if BUILDFLAG(ENABLE_REPORTING)
class NetworkErrorLoggingService;
class ReportingService;
#endif  // BUILDFLAG(ENABLE_REPORTING)

// Subclass to provide application-specific context for URLRequest
// instances. URLRequestContext does not own these member variables, since they
// may be shared with other contexts. URLRequestContextStorage can be used for
// automatic lifetime management. Most callers should use an existing
// URLRequestContext rather than creating a new one, as guaranteeing that the
// URLRequestContext is destroyed before its members can be difficult.
class NET_EXPORT URLRequestContext
    : public base::trace_event::MemoryDumpProvider {
 public:
  // Contexts that are known to not currently be copied should set |allow_copy|
  // to false to prevent added copying.
  explicit URLRequestContext(bool allow_copy = true);
  ~URLRequestContext() override;

  // May return nullptr if this context doesn't have an associated network
  // session.
  const HttpNetworkSession::Params* GetNetworkSessionParams() const;

  // May return nullptr if this context doesn't have an associated network
  // session.
  const HttpNetworkSession::Context* GetNetworkSessionContext() const;

  // This function should not be used in Chromium, please use the version with
  // NetworkTrafficAnnotationTag in the future.
  std::unique_ptr<URLRequest> CreateRequest(
      const GURL& url,
      RequestPriority priority,
      URLRequest::Delegate* delegate) const;

  // |traffic_annotation| is metadata about the network traffic send via this
  // URLRequest, see net::DefineNetworkTrafficAnnotation. Note that:
  // - net provides the API for tagging requests with an opaque identifier.
  // - tools/traffic_annotation/traffic_annotation.proto contains the Chrome
  // specific .proto describing the verbose annotation format that Chrome's
  // callsites are expected to follow.
  // - tools/traffic_annotation/ contains sample and template for annotation and
  // tools will be added for verification following crbug.com/690323.
  std::unique_ptr<URLRequest> CreateRequest(
      const GURL& url,
      RequestPriority priority,
      URLRequest::Delegate* delegate,
      NetworkTrafficAnnotationTag traffic_annotation) const;

  NetLog* net_log() const { return net_log_; }

  void set_net_log(NetLog* net_log) {
    net_log_ = net_log;
  }

#if defined(ENABLE_DNS)
  HostResolver* host_resolver() const {
    return host_resolver_;
  }

  void set_host_resolver(HostResolver* host_resolver) {
    host_resolver_ = host_resolver;
  }
#endif


#if defined(ENABLE_BORINGSSL)
  CertVerifier* cert_verifier() const {
    return cert_verifier_;
  }

  void set_cert_verifier(CertVerifier* cert_verifier) {
    cert_verifier_ = cert_verifier;
  }
#endif // ENABLE_BORINGSSL

#if defined(ENABLE_PROXY)
  // Get the proxy service for this context.
  ProxyResolutionService* proxy_resolution_service() const {
    return proxy_resolution_service_;
  }
  void set_proxy_resolution_service(
      ProxyResolutionService* proxy_resolution_service) {
    proxy_resolution_service_ = proxy_resolution_service;
  }
#endif

  ProxyDelegate* proxy_delegate() const { return proxy_delegate_; }
  void set_proxy_delegate(ProxyDelegate* proxy_delegate) {
    proxy_delegate_ = proxy_delegate;
  }

#if defined(ENABLE_BORINGSSL)
  // Get the ssl config service for this context.
  SSLConfigService* ssl_config_service() const { return ssl_config_service_; }
  void set_ssl_config_service(SSLConfigService* service) {
    ssl_config_service_ = service;
  }
#endif // ENABLE_BORINGSSL

  // Gets the HTTP Authentication Handler Factory for this context.
  // The factory is only valid for the lifetime of this URLRequestContext
  HttpAuthHandlerFactory* http_auth_handler_factory() const {
    return http_auth_handler_factory_;
  }
  void set_http_auth_handler_factory(HttpAuthHandlerFactory* factory) {
    http_auth_handler_factory_ = factory;
  }

  // Gets the http transaction factory for this context.
  HttpTransactionFactory* http_transaction_factory() const {
    return http_transaction_factory_;
  }
  void set_http_transaction_factory(HttpTransactionFactory* factory) {
    http_transaction_factory_ = factory;
  }

  void set_network_delegate(NetworkDelegate* network_delegate) {
    network_delegate_ = network_delegate;
  }
  NetworkDelegate* network_delegate() const { return network_delegate_; }

  void set_http_server_properties(
      HttpServerProperties* http_server_properties) {
    http_server_properties_ = http_server_properties;
  }
  HttpServerProperties* http_server_properties() const {
    return http_server_properties_;
  }

  // Gets the cookie store for this context (may be null, in which case
  // cookies are not stored).
  CookieStore* cookie_store() const { return cookie_store_; }
  void set_cookie_store(CookieStore* cookie_store);

  TransportSecurityState* transport_security_state() const {
    return transport_security_state_;
  }
  void set_transport_security_state(
      TransportSecurityState* state) {
    transport_security_state_ = state;
  }

#if defined(ENABLE_BORINGSSL)
  CTVerifier* cert_transparency_verifier() const {
    return cert_transparency_verifier_;
  }
  void set_cert_transparency_verifier(CTVerifier* verifier) {
    cert_transparency_verifier_ = verifier;
  }

  CTPolicyEnforcer* ct_policy_enforcer() const { return ct_policy_enforcer_; }
  void set_ct_policy_enforcer(CTPolicyEnforcer* enforcer) {
    ct_policy_enforcer_ = enforcer;
  }
#endif // ENABLE_BORINGSSL

  const URLRequestJobFactory* job_factory() const { return job_factory_; }
  void set_job_factory(const URLRequestJobFactory* job_factory) {
    job_factory_ = job_factory;
  }

  // May return nullptr.
  URLRequestThrottlerManager* throttler_manager() const {
    return throttler_manager_;
  }
  void set_throttler_manager(URLRequestThrottlerManager* throttler_manager) {
    throttler_manager_ = throttler_manager;
  }

  // Gets the URLRequest objects that hold a reference to this
  // URLRequestContext.
  std::set<const URLRequest*>* url_requests() const {
    return url_requests_.get();
  }

  // CHECKs that no URLRequests using this context remain. Subclasses should
  // additionally call AssertNoURLRequests() within their own destructor,
  // prior to implicit destruction of subclass-owned state.
  void AssertNoURLRequests() const;

  // Get the underlying |HttpUserAgentSettings| implementation that provides
  // the HTTP Accept-Language and User-Agent header values.
  const HttpUserAgentSettings* http_user_agent_settings() const {
    return http_user_agent_settings_;
  }
  void set_http_user_agent_settings(
      const HttpUserAgentSettings* http_user_agent_settings) {
    http_user_agent_settings_ = http_user_agent_settings;
  }

#if defined(ENABLE_NQE)
  // Gets the NetworkQualityEstimator associated with this context.
  // May return nullptr.
  NetworkQualityEstimator* network_quality_estimator() const {
    return network_quality_estimator_;
  }
  void set_network_quality_estimator(
      NetworkQualityEstimator* network_quality_estimator) {
    network_quality_estimator_ = network_quality_estimator;
  }
#endif

#if BUILDFLAG(ENABLE_REPORTING)
  ReportingService* reporting_service() const { return reporting_service_; }
  void set_reporting_service(ReportingService* reporting_service) {
    reporting_service_ = reporting_service;
  }

  NetworkErrorLoggingService* network_error_logging_service() const {
    return network_error_logging_service_;
  }
  void set_network_error_logging_service(
      NetworkErrorLoggingService* network_error_logging_service) {
    network_error_logging_service_ = network_error_logging_service;
  }
#endif  // BUILDFLAG(ENABLE_REPORTING)

  void set_enable_brotli(bool enable_brotli) { enable_brotli_ = enable_brotli; }

  bool enable_brotli() const { return enable_brotli_; }

  // Sets the |check_cleartext_permitted| flag, which controls whether to check
  // system policy before allowing a cleartext http or ws request.
  void set_check_cleartext_permitted(bool check_cleartext_permitted) {
    check_cleartext_permitted_ = check_cleartext_permitted;
  }

  // Returns current value of the |check_cleartext_permitted| flag.
  bool check_cleartext_permitted() const { return check_cleartext_permitted_; }

  // Sets a name for this URLRequestContext. Currently the name is used in
  // MemoryDumpProvier to annotate memory usage. The name does not need to be
  // unique.
  void set_name(const std::string& name) { name_ = name; }
  const std::string& name() const { return name_; }

  // MemoryDumpProvider implementation:
  // This is reported as
  // "memory:chrome:all_processes:reported_by_chrome:net:effective_size_avg."
  bool OnMemoryDump(const base::trace_event::MemoryDumpArgs& args,
                    base::trace_event::ProcessMemoryDump* pmd) override;

  void AssertCalledOnValidThread() {
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  }

 private:
  // Whitelist legacy usage of now-deprecated CopyFrom().
  friend class ::ChromeBrowserStateImplIOData;
  friend class ::ProfileImplIOData;
  friend class safe_browsing::SafeBrowsingURLRequestContextGetter;

  // Copies the state from |other| into this context.
  //
  // Due to complex interdependencies between various fields as well as fields
  // that should be unique to each context, copy is fundamentally broken, and
  // should not be done. If a modified context is needed (and that is not
  // typical), a new context should always be fully created (via
  // URLRequestContextBuilder) rather than copying from a previous one.
  void CopyFrom(const URLRequestContext* other);

  // ---------------------------------------------------------------------------
  // Important: When adding any new members below, consider whether they need to
  // be added to CopyFrom.
  // ---------------------------------------------------------------------------

  // Ownership for these members are not defined here. Clients should either
  // provide storage elsewhere or have a subclass take ownership.
  NetLog* net_log_;

#if defined(ENABLE_DNS)
  HostResolver* host_resolver_;
#endif


#if defined(ENABLE_BORINGSSL)
  CertVerifier* cert_verifier_;
#endif // ENABLE_BORINGSSL
  HttpAuthHandlerFactory* http_auth_handler_factory_;
#if defined(ENABLE_PROXY)
  ProxyResolutionService* proxy_resolution_service_;
#endif
  ProxyDelegate* proxy_delegate_;

#if defined(ENABLE_BORINGSSL)
  SSLConfigService* ssl_config_service_;
#endif // ENABLE_BORINGSSL
  NetworkDelegate* network_delegate_;
  HttpServerProperties* http_server_properties_;
  const HttpUserAgentSettings* http_user_agent_settings_;
  CookieStore* cookie_store_;
  TransportSecurityState* transport_security_state_;

#if defined(ENABLE_BORINGSSL)
  CTVerifier* cert_transparency_verifier_;
  CTPolicyEnforcer* ct_policy_enforcer_;
#endif // ENABLE_BORINGSSL
  HttpTransactionFactory* http_transaction_factory_;
  const URLRequestJobFactory* job_factory_;
  URLRequestThrottlerManager* throttler_manager_;
#if defined(ENABLE_NQE)
  NetworkQualityEstimator* network_quality_estimator_;
#endif
#if BUILDFLAG(ENABLE_REPORTING)
  ReportingService* reporting_service_;
  NetworkErrorLoggingService* network_error_logging_service_;
#endif  // BUILDFLAG(ENABLE_REPORTING)

  // ---------------------------------------------------------------------------
  // Important: When adding any new members below, consider whether they need to
  // be added to CopyFrom.
  // ---------------------------------------------------------------------------

  std::unique_ptr<std::set<const URLRequest*>> url_requests_;

  // Enables Brotli Content-Encoding support.
  bool enable_brotli_;
  // Enables checking system policy before allowing a cleartext http or ws
  // request. Only used on Android.
  bool check_cleartext_permitted_;

  // An optional name which can be set to describe this URLRequestContext.
  // Used in MemoryDumpProvier to annotate memory usage. The name does not need
  // to be unique.
  std::string name_;

  const bool allow_copy_;

  THREAD_CHECKER(thread_checker_);

  DISALLOW_COPY_AND_ASSIGN(URLRequestContext);
};

}  // namespace net

#endif  // NET_URL_REQUEST_URL_REQUEST_CONTEXT_H_
