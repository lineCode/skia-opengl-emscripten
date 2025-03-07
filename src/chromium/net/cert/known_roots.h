// Copyright (c) 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_KNOWN_ROOTS_H_
#define NET_CERT_KNOWN_ROOTS_H_

#include <stdint.h>

#include "build/build_config.h"
#include "net/base/net_export.h"

#if defined(ENABLE_BORINGSSL)
namespace net {

class HashValue;

// Returns a value within the NetRootCert histogram enum indicating the
// ID of the trust anchor whose subjectPublicKeyInfo hash is |spki_hash|, or
// 0 if it cannot be found.
NET_EXPORT int32_t
GetNetTrustAnchorHistogramIdForSPKI(const HashValue& spki_hash);

// Returns true if the CA identified by |spki_hash| is known as a Legacy CA,
// which means that they are known as a well-known root, but are not trusted
// on the ChromiumOS or Android platforms. This indicates a CA that either has
// been removed from public trust (generally, voluntarily) or has not (or not
// yet) undergone a public review and discussion for broad public trust.
NET_EXPORT bool IsLegacyPubliclyTrustedCA(const HashValue& spki_hash);

}  // namespace net
#endif // ENABLE_BORINGSSL

#endif  // NET_CERT_KNOWN_ROOTS_H_
