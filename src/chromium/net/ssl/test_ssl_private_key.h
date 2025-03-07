// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SSL_TEST_SSL_PRIVATE_KEY_H_
#define NET_SSL_TEST_SSL_PRIVATE_KEY_H_

#include "base/memory/ref_counted.h"
#include "net/base/net_export.h"
#if defined(ENABLE_BORINGSSL)
#include "third_party/boringssl/src/include/openssl/base.h"
#endif // ENABLE_BORINGSSL

#if defined(ENABLE_BORINGSSL)
namespace crypto {
class RSAPrivateKey;
}

namespace net {

class SSLPrivateKey;

// Returns a new SSLPrivateKey which uses |key| for signing operations or
// nullptr on error.
NET_EXPORT scoped_refptr<SSLPrivateKey> WrapOpenSSLPrivateKey(
    bssl::UniquePtr<EVP_PKEY> key);
NET_EXPORT scoped_refptr<SSLPrivateKey> WrapRSAPrivateKey(
    crypto::RSAPrivateKey* rsa_private_key);
NET_EXPORT scoped_refptr<SSLPrivateKey> CreateFailSigningSSLPrivateKey();

}  // namespace net
#endif // ENABLE_BORINGSSL

#endif  // NET_SSL_TEST_SSL_PRIVATE_KEY_H_
