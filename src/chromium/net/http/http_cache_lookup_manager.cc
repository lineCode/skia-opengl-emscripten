// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/http/http_cache_lookup_manager.h"

#include <memory>

#include "base/bind.h"
#include "base/values.h"
#include "net/base/load_flags.h"

namespace net {

// Returns parameters associated with the start of a server push lookup
// transaction.
base::Value NetLogPushLookupTransactionCallback(
    const NetLogSource& net_log,
#if defined(ENABLE_SPDY)
    const ServerPushDelegate::ServerPushHelper* push_helper,
#endif
    NetLogCaptureMode /* capture_mode */) {
  base::DictionaryValue dict;
  net_log.AddToEventParameters(&dict);
#if defined(ENABLE_SPDY)
  dict.SetString("push_url", push_helper->GetURL().possibly_invalid_spec());
#endif
  return std::move(dict);
}

HttpCacheLookupManager::LookupTransaction::LookupTransaction(
#if defined(ENABLE_SPDY)
    std::unique_ptr<ServerPushHelper> server_push_helper,
#endif
    NetLog* net_log)
    :
#if defined(ENABLE_SPDY)
    push_helper_(std::move(server_push_helper)),
#endif
      request_(new HttpRequestInfo()),
      transaction_(nullptr),
      net_log_(NetLogWithSource::Make(
          net_log,
          NetLogSourceType::SERVER_PUSH_LOOKUP_TRANSACTION)) {}

HttpCacheLookupManager::LookupTransaction::~LookupTransaction() = default;

int HttpCacheLookupManager::LookupTransaction::StartLookup(
    HttpCache* cache,
    CompletionOnceCallback callback,
    const NetLogWithSource& session_net_log) {
#if defined(ENABLE_SPDY)
  net_log_.BeginEvent(NetLogEventType::SERVER_PUSH_LOOKUP_TRANSACTION,
                      base::Bind(&NetLogPushLookupTransactionCallback,
                                 session_net_log.source(), push_helper_.get()));

  request_->url = push_helper_->GetURL();
#endif
  request_->method = "GET";
  request_->load_flags = LOAD_ONLY_FROM_CACHE | LOAD_SKIP_CACHE_VALIDATION;
  cache->CreateTransaction(DEFAULT_PRIORITY, &transaction_);
  return transaction_->Start(request_.get(), std::move(callback), net_log_);
}

void HttpCacheLookupManager::LookupTransaction::OnLookupComplete(int result) {
#if defined(ENABLE_SPDY)
  if (result == OK) {
    DCHECK(push_helper_.get());
    push_helper_->Cancel();
  }
  net_log_.EndEventWithNetErrorCode(
      NetLogEventType::SERVER_PUSH_LOOKUP_TRANSACTION, result);
#endif
}

HttpCacheLookupManager::HttpCacheLookupManager(HttpCache* http_cache)
    : http_cache_(http_cache), weak_factory_(this) {}

HttpCacheLookupManager::~HttpCacheLookupManager() = default;

#if defined(ENABLE_SPDY)
void HttpCacheLookupManager::OnPush(
#if defined(ENABLE_SPDY)
    std::unique_ptr<ServerPushHelper> push_helper,
#endif
    const NetLogWithSource& session_net_log) {

#if defined(ENABLE_SPDY)
  GURL pushed_url = push_helper->GetURL();

  // There's a pending lookup transaction sent over already.
  if (base::ContainsKey(lookup_transactions_, pushed_url))
    return;

  auto lookup = std::make_unique<LookupTransaction>(std::move(push_helper),
                                                    session_net_log.net_log());
  // TODO(zhongyi): add events in session net log to log the creation of
  // LookupTransaction.

  int rv = lookup->StartLookup(
      http_cache_, base::Bind(&HttpCacheLookupManager::OnLookupComplete,
                              weak_factory_.GetWeakPtr(), pushed_url),
      session_net_log);

  if (rv == ERR_IO_PENDING) {
    lookup_transactions_[pushed_url] = std::move(lookup);
  } else {
    lookup->OnLookupComplete(rv);
  }
#endif
}

void HttpCacheLookupManager::OnLookupComplete(const GURL& url, int rv) {
  auto it = lookup_transactions_.find(url);
  DCHECK(it != lookup_transactions_.end());

  it->second->OnLookupComplete(rv);

  lookup_transactions_.erase(it);
}
#endif

}  // namespace net
