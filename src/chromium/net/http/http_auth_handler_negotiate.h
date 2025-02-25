// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_AUTH_HANDLER_NEGOTIATE_H_
#define NET_HTTP_HTTP_AUTH_HANDLER_NEGOTIATE_H_

#include <memory>
#include <string>
#include <utility>

#include "build/build_config.h"
#include "net/base/completion_once_callback.h"
#include "net/base/net_export.h"

#if defined(ENABLE_QUIC)
#include "net/dns/host_resolver.h"
#endif

#include "net/http/http_auth_handler.h"
#include "net/http/http_auth_handler_factory.h"
#include "net/http/http_negotiate_auth_system.h"


#if !defined(__EMSCRIPTEN__) && defined(__TODO__)

#if defined(OS_ANDROID)
#include "net/android/http_auth_negotiate_android.h"
#elif defined(OS_WIN)
#include "net/http/http_auth_sspi_win.h"
#elif defined(OS_POSIX)
#include "net/http/http_auth_gssapi_posix.h"
#endif

#endif

namespace net {

class HttpAuthPreferences;

// Handler for WWW-Authenticate: Negotiate protocol.
//
// See http://tools.ietf.org/html/rfc4178 and http://tools.ietf.org/html/rfc4559
// for more information about the protocol.

class NET_EXPORT_PRIVATE HttpAuthHandlerNegotiate : public HttpAuthHandler {
 public:
#if defined(OS_WIN)
  typedef SSPILibrary AuthLibrary;
#elif defined(OS_POSIX) && !defined(OS_ANDROID) && defined(__TODO__)
  typedef GSSAPILibrary AuthLibrary;
#endif

  class NET_EXPORT_PRIVATE Factory : public HttpAuthHandlerFactory {
   public:
    explicit Factory(NegotiateAuthSystemFactory negotiate_auth_system_factory);
    ~Factory() override;

#if !defined(OS_ANDROID) && defined(__TODO__)
    // Sets the system library to use, thereby assuming ownership of
    // |auth_library|.
    void set_library(std::unique_ptr<AuthLibrary> auth_provider) {
      auth_library_ = std::move(auth_provider);
    }

#if defined(OS_POSIX) && defined(__TODO__)
    const std::string& GetLibraryNameForTesting() const;

    void set_allow_gssapi_library_load(bool allow_gssapi_library_load) {
      allow_gssapi_library_load_ = allow_gssapi_library_load;
    }
    bool allow_gssapi_library_load_for_testing() const {
      return allow_gssapi_library_load_;
    }
#endif  // defined(OS_POSIX)
#endif  // !defined(OS_ANDROID)

    // HttpAuthHandlerFactory overrides
    int CreateAuthHandler(HttpAuthChallengeTokenizer* challenge,
                          HttpAuth::Target target,
#if defined(ENABLE_BORINGSSL)
                          const SSLInfo& ssl_info,
#endif // ENABLE_BORINGSSL
                          const GURL& origin,
                          CreateReason reason,
                          int digest_nonce_count,
                          const NetLogWithSource& net_log,
#if defined(ENABLE_DNS)
                          HostResolver* host_resolver,
#endif
                          std::unique_ptr<HttpAuthHandler>* handler) override;

   private:
    NegotiateAuthSystemFactory negotiate_auth_system_factory_;
#if defined(OS_WIN)
    ULONG max_token_length_ = 0;
#endif
    bool is_unsupported_ = false;
#if !defined(OS_ANDROID) && defined(__TODO__)
    std::unique_ptr<AuthLibrary> auth_library_;
#if defined(OS_POSIX)
    bool allow_gssapi_library_load_ = true;
#endif  // defined(OS_POSIX)
#endif  // !defined(OS_ANDROID)
  };

  HttpAuthHandlerNegotiate(std::unique_ptr<HttpNegotiateAuthSystem> auth_system,
                           const HttpAuthPreferences* prefs
#if defined(ENABLE_DNS)
                           ,
                           HostResolver* host_resolver
#endif
                           );

  ~HttpAuthHandlerNegotiate() override;

  // HttpAuthHandler:
  HttpAuth::AuthorizationResult HandleAnotherChallenge(
      HttpAuthChallengeTokenizer* challenge) override;
  bool NeedsIdentity() override;
  bool AllowsDefaultCredentials() override;
  bool AllowsExplicitCredentials() override;

  const std::string& spn_for_testing() const { return spn_; }

 protected:
  bool Init(HttpAuthChallengeTokenizer* challenge
#if defined(ENABLE_BORINGSSL)
  ,
            const SSLInfo& ssl_info
#endif // ENABLE_BORINGSSL
  ) override;

  int GenerateAuthTokenImpl(const AuthCredentials* credentials,
                            const HttpRequestInfo* request,
                            CompletionOnceCallback callback,
                            std::string* auth_token) override;

 private:
  enum State {
    STATE_RESOLVE_CANONICAL_NAME,
    STATE_RESOLVE_CANONICAL_NAME_COMPLETE,
    STATE_GENERATE_AUTH_TOKEN,
    STATE_GENERATE_AUTH_TOKEN_COMPLETE,
    STATE_NONE,
  };

  std::string CreateSPN(const std::string& server, const GURL& orign);

  void OnIOComplete(int result);
  void DoCallback(int result);
  int DoLoop(int result);

  int DoResolveCanonicalName();
  int DoResolveCanonicalNameComplete(int rv);
  int DoGenerateAuthToken();
  int DoGenerateAuthTokenComplete(int rv);
  HttpAuth::DelegationType GetDelegationType() const;

  std::unique_ptr<HttpNegotiateAuthSystem> auth_system_;

#if defined(ENABLE_DNS)
  HostResolver* const resolver_;
#endif

#if defined(ENABLE_QUIC)
  // Members which are needed for DNS lookup + SPN.
  std::unique_ptr<HostResolver::ResolveHostRequest> resolve_host_request_;
#endif

  // Things which should be consistent after first call to GenerateAuthToken.
  bool already_called_;
  bool has_credentials_;
  AuthCredentials credentials_;
  std::string spn_;
  std::string channel_bindings_;

  // Things which vary each round.
  CompletionOnceCallback callback_;
  std::string* auth_token_;

  State next_state_;

  const HttpAuthPreferences* http_auth_preferences_;
};

}  // namespace net

#endif  // NET_HTTP_HTTP_AUTH_HANDLER_NEGOTIATE_H_
