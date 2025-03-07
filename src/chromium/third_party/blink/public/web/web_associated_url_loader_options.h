/*
 * Copyright (C) 2009, 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THIRD_PARTY_BLINK_PUBLIC_WEB_WEB_ASSOCIATED_URL_LOADER_OPTIONS_H_
#define THIRD_PARTY_BLINK_PUBLIC_WEB_WEB_ASSOCIATED_URL_LOADER_OPTIONS_H_

#if defined(ENABLE_GNET)
#include "services/network/public/mojom/cors.mojom-shared.h"
#endif // ENABLE_GNET

namespace blink {

struct WebAssociatedURLLoaderOptions {
  // Whether to validate the method and headers as if this was an
  // XMLHttpRequest.
  bool untrusted_http = false;

  // If policy is to use access control, whether to expose non-whitelisted
  // response headers to the client.
  bool expose_all_response_headers = false;

  // When true, omit origin related checks. USE WITH CARE.
  bool grant_universal_access = false;

  network::mojom::CorsPreflightPolicy preflight_policy =
      network::mojom::CorsPreflightPolicy::kConsiderPreflight;
};

}  // namespace blink

#endif
