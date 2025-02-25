// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "crypto/aead.h"

#include <stddef.h>
#include <stdint.h>
#include <string>

#include "base/strings/string_util.h"
#include "crypto/openssl_util.h"
#if defined(ENABLE_BORINGSSL)
#include "third_party/boringssl/src/include/openssl/aes.h"
#include "third_party/boringssl/src/include/openssl/evp.h"
#endif

namespace crypto {

Aead::Aead(AeadAlgorithm algorithm) : key_(nullptr) {
#if defined(ENABLE_BORINGSSL)
  EnsureOpenSSLInit();
  switch (algorithm) {
    case AES_128_CTR_HMAC_SHA256:
      aead_ = EVP_aead_aes_128_ctr_hmac_sha256();
      break;
    case AES_256_GCM:
      aead_ = EVP_aead_aes_256_gcm();
      break;
    case AES_256_GCM_SIV:
      aead_ = EVP_aead_aes_256_gcm_siv();
      break;
  }
#endif
}

Aead::~Aead() = default;

void Aead::Init(const std::string* key) {
  DCHECK(!key_);
  DCHECK_EQ(KeyLength(), key->size());
  key_ = key;
}

bool Aead::Seal(base::StringPiece plaintext,
                base::StringPiece nonce,
                base::StringPiece additional_data,
                std::string* ciphertext) const {
#if defined(ENABLE_BORINGSSL)
  DCHECK(key_);
  DCHECK_EQ(NonceLength(), nonce.size());
  EVP_AEAD_CTX ctx;

  if (!EVP_AEAD_CTX_init(&ctx, aead_,
                         reinterpret_cast<const uint8_t*>(key_->data()),
                         key_->size(), EVP_AEAD_DEFAULT_TAG_LENGTH, nullptr)) {
    return false;
  }

  std::string result;
  const size_t max_output_length =
      EVP_AEAD_max_overhead(aead_) + plaintext.size();
  size_t output_length;
  uint8_t* out_ptr = reinterpret_cast<uint8_t*>(
      base::WriteInto(&result, max_output_length + 1));

  if (!EVP_AEAD_CTX_seal(
          &ctx, out_ptr, &output_length, max_output_length,
          reinterpret_cast<const uint8_t*>(nonce.data()), nonce.size(),
          reinterpret_cast<const uint8_t*>(plaintext.data()), plaintext.size(),
          reinterpret_cast<const uint8_t*>(additional_data.data()),
          additional_data.size())) {
    EVP_AEAD_CTX_cleanup(&ctx);
    return false;
  }

  DCHECK_LE(output_length, max_output_length);
  result.resize(output_length);

  ciphertext->swap(result);
  EVP_AEAD_CTX_cleanup(&ctx);
#endif
  return true;
}

bool Aead::Open(base::StringPiece ciphertext,
                base::StringPiece nonce,
                base::StringPiece additional_data,
                std::string* plaintext) const {
#if defined(ENABLE_BORINGSSL)
  DCHECK(key_);
  EVP_AEAD_CTX ctx;

  if (!EVP_AEAD_CTX_init(&ctx, aead_,
                         reinterpret_cast<const uint8_t*>(key_->data()),
                         key_->size(), EVP_AEAD_DEFAULT_TAG_LENGTH, nullptr)) {
    return false;
  }

  std::string result;
  const size_t max_output_length = ciphertext.size();
  size_t output_length;
  uint8_t* out_ptr = reinterpret_cast<uint8_t*>(
      base::WriteInto(&result, max_output_length + 1));

  if (!EVP_AEAD_CTX_open(
          &ctx, out_ptr, &output_length, max_output_length,
          reinterpret_cast<const uint8_t*>(nonce.data()), nonce.size(),
          reinterpret_cast<const uint8_t*>(ciphertext.data()),
          ciphertext.size(),
          reinterpret_cast<const uint8_t*>(additional_data.data()),
          additional_data.size())) {
    EVP_AEAD_CTX_cleanup(&ctx);
    return false;
  }

  DCHECK_LE(output_length, max_output_length);
  result.resize(output_length);

  plaintext->swap(result);
  EVP_AEAD_CTX_cleanup(&ctx);
#endif
  return true;
}

size_t Aead::KeyLength() const {
#if defined(ENABLE_BORINGSSL)
  return EVP_AEAD_key_length(aead_);
#else
  return 0;
#endif
}

size_t Aead::NonceLength() const {
#if defined(ENABLE_BORINGSSL)
  return EVP_AEAD_nonce_length(aead_);
#else
  return 0;
#endif
}

}  // namespace crypto
