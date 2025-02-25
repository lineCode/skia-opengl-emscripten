// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/dictionary_v8.cc.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#include "third_party/blink/renderer/bindings/modules/v8/v8_rtc_rtp_capabilities.h"

#include "base/stl_util.h"
#include "third_party/blink/renderer/bindings/core/v8/idl_types.h"
#include "third_party/blink/renderer/bindings/core/v8/native_value_traits_impl.h"
#include "third_party/blink/renderer/bindings/modules/v8/v8_rtc_rtp_codec_capability.h"
#include "third_party/blink/renderer/bindings/modules/v8/v8_rtc_rtp_header_extension_capability.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"

namespace blink {

static const v8::Eternal<v8::Name>* eternalV8RTCRtpCapabilitiesKeys(v8::Isolate* isolate) {
  static const char* const kKeys[] = {
    "codecs",
    "headerExtensions",
  };
  return V8PerIsolateData::From(isolate)->FindOrCreateEternalNameCache(
      kKeys, kKeys, base::size(kKeys));
}

void V8RTCRtpCapabilities::ToImpl(v8::Isolate* isolate, v8::Local<v8::Value> v8_value, RTCRtpCapabilities* impl, ExceptionState& exception_state) {
  if (IsUndefinedOrNull(v8_value)) {
    exception_state.ThrowTypeError("Missing required member(s): codecs, headerExtensions.");
    return;
  }
  if (!v8_value->IsObject()) {
    exception_state.ThrowTypeError("cannot convert to dictionary.");
    return;
  }
  v8::Local<v8::Object> v8Object = v8_value.As<v8::Object>();
  ALLOW_UNUSED_LOCAL(v8Object);

  const v8::Eternal<v8::Name>* keys = eternalV8RTCRtpCapabilitiesKeys(isolate);
  v8::TryCatch block(isolate);
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Value> codecs_value;
  if (!v8Object->Get(context, keys[0].Get(isolate)).ToLocal(&codecs_value)) {
    exception_state.RethrowV8Exception(block.Exception());
    return;
  }
  if (codecs_value.IsEmpty() || codecs_value->IsUndefined()) {
    exception_state.ThrowTypeError("required member codecs is undefined.");
    return;
  } else {
    HeapVector<Member<RTCRtpCodecCapability>> codecs_cpp_value = NativeValueTraits<IDLSequence<RTCRtpCodecCapability>>::NativeValue(isolate, codecs_value, exception_state);
    if (exception_state.HadException())
      return;
    impl->setCodecs(codecs_cpp_value);
  }

  v8::Local<v8::Value> header_extensions_value;
  if (!v8Object->Get(context, keys[1].Get(isolate)).ToLocal(&header_extensions_value)) {
    exception_state.RethrowV8Exception(block.Exception());
    return;
  }
  if (header_extensions_value.IsEmpty() || header_extensions_value->IsUndefined()) {
    exception_state.ThrowTypeError("required member headerExtensions is undefined.");
    return;
  } else {
    HeapVector<Member<RTCRtpHeaderExtensionCapability>> header_extensions_cpp_value = NativeValueTraits<IDLSequence<RTCRtpHeaderExtensionCapability>>::NativeValue(isolate, header_extensions_value, exception_state);
    if (exception_state.HadException())
      return;
    impl->setHeaderExtensions(header_extensions_cpp_value);
  }
}

v8::Local<v8::Value> RTCRtpCapabilities::ToV8Impl(v8::Local<v8::Object> creationContext, v8::Isolate* isolate) const {
  v8::Local<v8::Object> v8Object = v8::Object::New(isolate);
  if (!toV8RTCRtpCapabilities(this, v8Object, creationContext, isolate))
    return v8::Undefined(isolate);
  return v8Object;
}

bool toV8RTCRtpCapabilities(const RTCRtpCapabilities* impl, v8::Local<v8::Object> dictionary, v8::Local<v8::Object> creationContext, v8::Isolate* isolate) {
  const v8::Eternal<v8::Name>* keys = eternalV8RTCRtpCapabilitiesKeys(isolate);
  v8::Local<v8::Context> context = isolate->GetCurrentContext();

  auto create_property = [dictionary, context, keys, isolate](
                             size_t key_index, v8::Local<v8::Value> value) {
    bool added_property;
    v8::Local<v8::Name> key = keys[key_index].Get(isolate);
    if (!dictionary->CreateDataProperty(context, key, value)
             .To(&added_property)) {
      return false;
    }
    return added_property;
  };

  v8::Local<v8::Value> codecs_value;
  bool codecs_has_value_or_default = false;
  if (impl->hasCodecs()) {
    codecs_value = ToV8(impl->codecs(), creationContext, isolate);
    codecs_has_value_or_default = true;
  } else {
    NOTREACHED();
  }
  if (codecs_has_value_or_default &&
      !create_property(0, codecs_value)) {
    return false;
  }

  v8::Local<v8::Value> header_extensions_value;
  bool header_extensions_has_value_or_default = false;
  if (impl->hasHeaderExtensions()) {
    header_extensions_value = ToV8(impl->headerExtensions(), creationContext, isolate);
    header_extensions_has_value_or_default = true;
  } else {
    NOTREACHED();
  }
  if (header_extensions_has_value_or_default &&
      !create_property(1, header_extensions_value)) {
    return false;
  }

  return true;
}

RTCRtpCapabilities* NativeValueTraits<RTCRtpCapabilities>::NativeValue(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exception_state) {
  RTCRtpCapabilities* impl = RTCRtpCapabilities::Create();
  V8RTCRtpCapabilities::ToImpl(isolate, value, impl, exception_state);
  return impl;
}

}  // namespace blink
