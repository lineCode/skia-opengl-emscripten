// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/dictionary_impl.h.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_PEERCONNECTION_RTC_QUIC_PARAMETERS_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_PEERCONNECTION_RTC_QUIC_PARAMETERS_H_

#include "third_party/blink/renderer/bindings/core/v8/idl_dictionary_base.h"
#include "third_party/blink/renderer/modules/modules_export.h"
#include "third_party/blink/renderer/modules/peerconnection/rtc_dtls_fingerprint.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"
#include "third_party/blink/renderer/platform/wtf/vector.h"

namespace blink {

class MODULES_EXPORT RTCQuicParameters : public IDLDictionaryBase {
 public:
  static RTCQuicParameters* Create() { return MakeGarbageCollected<RTCQuicParameters>(); }

  RTCQuicParameters();
  virtual ~RTCQuicParameters();

  bool hasFingerprints() const { return has_fingerprints_; }
  const HeapVector<Member<RTCDtlsFingerprint>>& fingerprints() const {
    DCHECK(has_fingerprints_);
    return fingerprints_;
  }
  void setFingerprints(const HeapVector<Member<RTCDtlsFingerprint>>&);

  bool hasRole() const { return !role_.IsNull(); }
  const String& role() const {
    return role_;
  }
  inline void setRole(const String&);

  v8::Local<v8::Value> ToV8Impl(v8::Local<v8::Object>, v8::Isolate*) const override;
  void Trace(blink::Visitor*) override;

 private:
  bool has_fingerprints_ = false;

  HeapVector<Member<RTCDtlsFingerprint>> fingerprints_;
  String role_;

  friend class V8RTCQuicParameters;
};

void RTCQuicParameters::setRole(const String& value) {
  role_ = value;
}

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_PEERCONNECTION_RTC_QUIC_PARAMETERS_H_
