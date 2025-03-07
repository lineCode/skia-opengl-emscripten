// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/http/http_auth_handler.h"

#include <utility>

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/logging.h"
#include "net/base/net_errors.h"
#include "net/http/http_auth_challenge_tokenizer.h"
#include "net/log/net_log_event_type.h"

namespace net {

HttpAuthHandler::HttpAuthHandler()
    : auth_scheme_(HttpAuth::AUTH_SCHEME_MAX),
      score_(-1),
      target_(HttpAuth::AUTH_NONE),
      properties_(-1) {
}

HttpAuthHandler::~HttpAuthHandler() = default;

bool HttpAuthHandler::InitFromChallenge(HttpAuthChallengeTokenizer* challenge,
                                        HttpAuth::Target target,

#if defined(ENABLE_BORINGSSL)
                                        const SSLInfo& ssl_info,
#endif // ENABLE_BORINGSSL
                                        const GURL& origin,
                                        const NetLogWithSource& net_log) {
  origin_ = origin;
  target_ = target;
  score_ = -1;
  properties_ = -1;
  net_log_ = net_log;

  auth_challenge_ = challenge->challenge_text();

#if defined(ENABLE_BORINGSSL)
  bool ok = Init(challenge, ssl_info);

  // Init() is expected to set the scheme, realm, score, and properties.  The
  // realm may be empty.
  DCHECK(!ok || score_ != -1);
  DCHECK(!ok || properties_ != -1);
  DCHECK(!ok || auth_scheme_ != HttpAuth::AUTH_SCHEME_MAX);
#else
  bool ok = Init(challenge);
#endif // ENABLE_BORINGSSL

  return ok;
}

namespace {

NetLogEventType EventTypeFromAuthTarget(HttpAuth::Target target) {
  switch (target) {
    case HttpAuth::AUTH_PROXY:
      return NetLogEventType::AUTH_PROXY;
    case HttpAuth::AUTH_SERVER:
      return NetLogEventType::AUTH_SERVER;
    default:
      NOTREACHED();
      return NetLogEventType::CANCELLED;
  }
}

}  // namespace

int HttpAuthHandler::GenerateAuthToken(const AuthCredentials* credentials,
                                       const HttpRequestInfo* request,
                                       CompletionOnceCallback callback,
                                       std::string* auth_token) {
  DCHECK(!callback.is_null());
  DCHECK(request);
  DCHECK(credentials != nullptr || AllowsDefaultCredentials());
  DCHECK(auth_token != nullptr);
  DCHECK(callback_.is_null());
  callback_ = std::move(callback);
  net_log_.BeginEvent(EventTypeFromAuthTarget(target_));
  int rv = GenerateAuthTokenImpl(
      credentials, request,
      base::BindOnce(&HttpAuthHandler::OnGenerateAuthTokenComplete,
                     base::Unretained(this)),
      auth_token);
  if (rv != ERR_IO_PENDING)
    FinishGenerateAuthToken();
  return rv;
}

bool HttpAuthHandler::NeedsIdentity() {
  return true;
}

bool HttpAuthHandler::AllowsDefaultCredentials() {
  return false;
}

bool HttpAuthHandler::AllowsExplicitCredentials() {
  return true;
}

void HttpAuthHandler::OnGenerateAuthTokenComplete(int rv) {
  CompletionOnceCallback callback = std::move(callback_);
  FinishGenerateAuthToken();
  DCHECK(!callback.is_null());
  std::move(callback).Run(rv);
}

void HttpAuthHandler::FinishGenerateAuthToken() {
  // TODO(cbentzel): Should this be done in OK case only?
  net_log_.EndEvent(EventTypeFromAuthTarget(target_));
  callback_.Reset();
}

}  // namespace net
