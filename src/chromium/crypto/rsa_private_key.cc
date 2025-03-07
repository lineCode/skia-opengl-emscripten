// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "crypto/rsa_private_key.h"

#include <stdint.h>

#include <memory>
#include <utility>

#include "base/logging.h"
#include "crypto/openssl_util.h"
#if defined(ENABLE_BORINGSSL)
#include "third_party/boringssl/src/include/openssl/bn.h"
#include "third_party/boringssl/src/include/openssl/bytestring.h"
#include "third_party/boringssl/src/include/openssl/evp.h"
#include "third_party/boringssl/src/include/openssl/mem.h"
#include "third_party/boringssl/src/include/openssl/rsa.h"
#endif

namespace crypto {

// static
std::unique_ptr<RSAPrivateKey> RSAPrivateKey::Create(uint16_t num_bits) {
#if defined(ENABLE_BORINGSSL)
  OpenSSLErrStackTracer err_tracer(FROM_HERE);

  bssl::UniquePtr<RSA> rsa_key(RSA_new());
  bssl::UniquePtr<BIGNUM> bn(BN_new());
  if (!rsa_key.get() || !bn.get() || !BN_set_word(bn.get(), 65537L))
    return nullptr;

  if (!RSA_generate_key_ex(rsa_key.get(), num_bits, bn.get(), nullptr))
    return nullptr;

  std::unique_ptr<RSAPrivateKey> result(new RSAPrivateKey);
  result->key_.reset(EVP_PKEY_new());
  if (!result->key_ || !EVP_PKEY_set1_RSA(result->key_.get(), rsa_key.get()))
    return nullptr;

  return result;
#else
  return nullptr;
#endif
}

// static
std::unique_ptr<RSAPrivateKey> RSAPrivateKey::CreateFromPrivateKeyInfo(
    const std::vector<uint8_t>& input) {

#if defined(ENABLE_BORINGSSL)
  OpenSSLErrStackTracer err_tracer(FROM_HERE);

  CBS cbs;
  CBS_init(&cbs, input.data(), input.size());
  bssl::UniquePtr<EVP_PKEY> pkey(EVP_parse_private_key(&cbs));
  if (!pkey || CBS_len(&cbs) != 0 || EVP_PKEY_id(pkey.get()) != EVP_PKEY_RSA)
    return nullptr;

  std::unique_ptr<RSAPrivateKey> result(new RSAPrivateKey);
  result->key_ = std::move(pkey);
  return result;
#else
  return nullptr;
#endif
}

#if defined(ENABLE_BORINGSSL)
// static
std::unique_ptr<RSAPrivateKey> RSAPrivateKey::CreateFromKey(EVP_PKEY* key) {
  DCHECK(key);
  if (EVP_PKEY_type(key->type) != EVP_PKEY_RSA)
    return nullptr;
  std::unique_ptr<RSAPrivateKey> copy(new RSAPrivateKey);
  copy->key_ = bssl::UpRef(key);
  return copy;
}
#endif

RSAPrivateKey::RSAPrivateKey() = default;

RSAPrivateKey::~RSAPrivateKey() = default;

std::unique_ptr<RSAPrivateKey> RSAPrivateKey::Copy() const {
#if defined(ENABLE_BORINGSSL)
  std::unique_ptr<RSAPrivateKey> copy(new RSAPrivateKey);
  bssl::UniquePtr<RSA> rsa(EVP_PKEY_get1_RSA(key_.get()));
  if (!rsa)
    return nullptr;
  copy->key_.reset(EVP_PKEY_new());
  if (!EVP_PKEY_set1_RSA(copy->key_.get(), rsa.get()))
    return nullptr;
  return copy;
#else
  return nullptr;
#endif
}

bool RSAPrivateKey::ExportPrivateKey(std::vector<uint8_t>* output) const {
#if defined(ENABLE_BORINGSSL)
  OpenSSLErrStackTracer err_tracer(FROM_HERE);
  uint8_t *der;
  size_t der_len;
  bssl::ScopedCBB cbb;
  if (!CBB_init(cbb.get(), 0) ||
      !EVP_marshal_private_key(cbb.get(), key_.get()) ||
      !CBB_finish(cbb.get(), &der, &der_len)) {
    return false;
  }
  output->assign(der, der + der_len);
  OPENSSL_free(der);
#endif
  return true;
}

bool RSAPrivateKey::ExportPublicKey(std::vector<uint8_t>* output) const {
#if defined(ENABLE_BORINGSSL)
  OpenSSLErrStackTracer err_tracer(FROM_HERE);
  uint8_t *der;
  size_t der_len;
  bssl::ScopedCBB cbb;
  if (!CBB_init(cbb.get(), 0) ||
      !EVP_marshal_public_key(cbb.get(), key_.get()) ||
      !CBB_finish(cbb.get(), &der, &der_len)) {
    return false;
  }
  output->assign(der, der + der_len);
  OPENSSL_free(der);
#endif
  return true;
}

}  // namespace crypto
