// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/strings/string_util.h"
#include "net/http/http_auth_filter.h"
#include "url/gurl.h"

namespace net {

// Using a std::set<> has the benefit of removing duplicates automatically.
typedef std::set<base::string16> RegistryWhitelist;

// TODO(ahendrickson) -- Determine if we want separate whitelists for HTTP and
// HTTPS, one for both, or only an HTTP one.  My understanding is that the HTTPS
// entries in the registry mean that you are only allowed to connect to the site
// via HTTPS and still be considered 'safe'.

HttpAuthFilterWhitelist::HttpAuthFilterWhitelist(
    const std::string& server_whitelist) {
  SetWhitelist(server_whitelist);
}

HttpAuthFilterWhitelist::~HttpAuthFilterWhitelist() = default;

// Add a new domain |filter| to the whitelist, if it's not already there
bool HttpAuthFilterWhitelist::AddFilter(const std::string& filter,
                                        HttpAuth::Target target) {
  if ((target != HttpAuth::AUTH_SERVER) && (target != HttpAuth::AUTH_PROXY))
    return false;
  // All proxies pass
  if (target == HttpAuth::AUTH_PROXY)
    return true;

#if defined(ENABLE_PROXY)
  rules_.AddRuleFromString(filter);
#endif

  return true;
}

bool HttpAuthFilterWhitelist::IsValid(const GURL& url,
                                      HttpAuth::Target target) const {
  if ((target != HttpAuth::AUTH_SERVER) && (target != HttpAuth::AUTH_PROXY))
    return false;
  // All proxies pass
  if (target == HttpAuth::AUTH_PROXY)
    return true;
#if defined(ENABLE_PROXY)
  return rules_.Matches(url);
#else
  return false;
#endif
}

void HttpAuthFilterWhitelist::SetWhitelist(
    const std::string& server_whitelist) {
  // TODO(eroman): Is this necessary? The issue is that
  // HttpAuthFilterWhitelist is trying to use ProxyBypassRules as a generic
  // URL filter. However internally it has some implicit rules for localhost
  // and linklocal addresses.

#if defined(ENABLE_PROXY)
  rules_.ParseFromString(ProxyBypassRules::GetRulesToSubtractImplicit() + ";" +
                         server_whitelist);
#endif
}

}  // namespace net
