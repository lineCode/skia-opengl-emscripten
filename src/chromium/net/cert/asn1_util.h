// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_ASN1_UTIL_H_
#define NET_CERT_ASN1_UTIL_H_

#include <vector>

#include "base/strings/string_piece.h"
#include "net/base/net_export.h"

#if defined(ENABLE_BORINGSSL)
namespace net {

namespace asn1 {

// ExtractSubjectFromDERCert parses the DER encoded certificate in |cert| and
// extracts the bytes of the X.501 Subject. On successful return, |subject_out|
// is set to contain the Subject, pointing into |cert|.
NET_EXPORT_PRIVATE bool ExtractSubjectFromDERCert(
    base::StringPiece cert,
    base::StringPiece* subject_out);

// ExtractSPKIFromDERCert parses the DER encoded certificate in |cert| and
// extracts the bytes of the SubjectPublicKeyInfo. On successful return,
// |spki_out| is set to contain the SPKI, pointing into |cert|.
NET_EXPORT_PRIVATE bool ExtractSPKIFromDERCert(base::StringPiece cert,
                                               base::StringPiece* spki_out);

// ExtractSubjectPublicKeyFromSPKI parses the DER encoded SubjectPublicKeyInfo
// in |spki| and extracts the bytes of the SubjectPublicKey. On successful
// return, |spk_out| is set to contain the public key, pointing into |spki|.
NET_EXPORT_PRIVATE bool ExtractSubjectPublicKeyFromSPKI(
    base::StringPiece spki,
    base::StringPiece* spk_out);

// HasTLSFeatureExtension parses the DER encoded certificate in |cert|
// and extracts the TLS feature extension
// (https://tools.ietf.org/html/rfc7633) if present. Returns true if the
// TLS feature extension was present, and false if the extension was not
// present or if there was a parsing failure.
NET_EXPORT_PRIVATE bool HasTLSFeatureExtension(base::StringPiece cert);

// HasCanSignHttpExchangesDraftExtension parses the DER encoded certificate
// in |cert| and extracts the canSignHttpExchangesDraft extension
// (https://wicg.github.io/webpackage/draft-yasskin-http-origin-signed-responses.html)
// if present. Returns true if the extension was present, and false if
// the extension was not present or if there was a parsing failure.
NET_EXPORT bool HasCanSignHttpExchangesDraftExtension(base::StringPiece cert);

// Extracts the two (SEQUENCE) tag-length-values for the signature
// AlgorithmIdentifiers in a DER encoded certificate. Does not use strict
// parsing or validate the resulting AlgorithmIdentifiers.
//
// On success returns true, and assigns |cert_signature_algorithm_sequence| and
// |tbs_signature_algorithm_sequence| to point into |cert|:
//
// * |cert_signature_algorithm_sequence| points at the TLV for
//   Certificate.signatureAlgorithm.
//
// * |tbs_signature_algorithm_sequence| points at the TLV for
//   TBSCertificate.algorithm.
NET_EXPORT_PRIVATE bool ExtractSignatureAlgorithmsFromDERCert(
    base::StringPiece cert,
    base::StringPiece* cert_signature_algorithm_sequence,
    base::StringPiece* tbs_signature_algorithm_sequence);

// Extracts the contents of the extension (if any) with OID |extension_oid| from
// the DER-encoded, X.509 certificate in |cert|.
//
// Returns false on parse error or true if the parse was successful. Sets
// |*out_extension_present| to whether or not the extension was found. If found,
// sets |*out_extension_critical| to match the extension's "critical" flag, and
// sets |*out_contents| to the contents of the extension (after unwrapping the
// OCTET STRING).
NET_EXPORT bool ExtractExtensionFromDERCert(base::StringPiece cert,
                                            base::StringPiece extension_oid,
                                            bool* out_extension_present,
                                            bool* out_extension_critical,
                                            base::StringPiece* out_contents);

} // namespace asn1

} // namespace net
#endif // ENABLE_BORINGSSL

#endif // NET_CERT_ASN1_UTIL_H_
