// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/union_container.h.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#ifndef THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_STRING_TREAT_NULL_AS_EMPTY_STRING_OR_TRUSTED_SCRIPT_H_
#define THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_STRING_TREAT_NULL_AS_EMPTY_STRING_OR_TRUSTED_SCRIPT_H_

#include "base/optional.h"
#include "third_party/blink/renderer/bindings/core/v8/dictionary.h"
#include "third_party/blink/renderer/bindings/core/v8/native_value_traits.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_binding_for_core.h"
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class TrustedScript;

class CORE_EXPORT StringTreatNullAsEmptyStringOrTrustedScript final {
  DISALLOW_NEW();
 public:
  StringTreatNullAsEmptyStringOrTrustedScript();
  bool IsNull() const { return type_ == SpecificType::kNone; }

  bool IsString() const { return type_ == SpecificType::kStringTreatNullAsEmptyString; }
  const String& GetAsString() const;
  void SetString(const String&);
  static StringTreatNullAsEmptyStringOrTrustedScript FromString(const String&);

  bool IsTrustedScript() const { return type_ == SpecificType::kTrustedScript; }
  TrustedScript* GetAsTrustedScript() const;
  void SetTrustedScript(TrustedScript*);
  static StringTreatNullAsEmptyStringOrTrustedScript FromTrustedScript(TrustedScript*);

  StringTreatNullAsEmptyStringOrTrustedScript(const StringTreatNullAsEmptyStringOrTrustedScript&);
  ~StringTreatNullAsEmptyStringOrTrustedScript();
  StringTreatNullAsEmptyStringOrTrustedScript& operator=(const StringTreatNullAsEmptyStringOrTrustedScript&);
  void Trace(blink::Visitor*);

 private:
  enum class SpecificType {
    kNone,
    kStringTreatNullAsEmptyString,
    kTrustedScript,
  };
  SpecificType type_;

  String string_treat_null_as_empty_string_;
  Member<TrustedScript> trusted_script_;

  friend CORE_EXPORT v8::Local<v8::Value> ToV8(const StringTreatNullAsEmptyStringOrTrustedScript&, v8::Local<v8::Object>, v8::Isolate*);
};

class V8StringTreatNullAsEmptyStringOrTrustedScript final {
 public:
  CORE_EXPORT static void ToImpl(v8::Isolate*, v8::Local<v8::Value>, StringTreatNullAsEmptyStringOrTrustedScript&, UnionTypeConversionMode, ExceptionState&);
};

CORE_EXPORT v8::Local<v8::Value> ToV8(const StringTreatNullAsEmptyStringOrTrustedScript&, v8::Local<v8::Object>, v8::Isolate*);

template <class CallbackInfo>
inline void V8SetReturnValue(const CallbackInfo& callbackInfo, StringTreatNullAsEmptyStringOrTrustedScript& impl) {
  V8SetReturnValue(callbackInfo, ToV8(impl, callbackInfo.Holder(), callbackInfo.GetIsolate()));
}

template <class CallbackInfo>
inline void V8SetReturnValue(const CallbackInfo& callbackInfo, StringTreatNullAsEmptyStringOrTrustedScript& impl, v8::Local<v8::Object> creationContext) {
  V8SetReturnValue(callbackInfo, ToV8(impl, creationContext, callbackInfo.GetIsolate()));
}

template <>
struct NativeValueTraits<StringTreatNullAsEmptyStringOrTrustedScript> : public NativeValueTraitsBase<StringTreatNullAsEmptyStringOrTrustedScript> {
  CORE_EXPORT static StringTreatNullAsEmptyStringOrTrustedScript NativeValue(v8::Isolate*, v8::Local<v8::Value>, ExceptionState&);
  CORE_EXPORT static StringTreatNullAsEmptyStringOrTrustedScript NullValue() { return StringTreatNullAsEmptyStringOrTrustedScript(); }
};

template <>
struct V8TypeOf<StringTreatNullAsEmptyStringOrTrustedScript> {
  typedef V8StringTreatNullAsEmptyStringOrTrustedScript Type;
};

}  // namespace blink

// We need to set canInitializeWithMemset=true because HeapVector supports
// items that can initialize with memset or have a vtable. It is safe to
// set canInitializeWithMemset=true for a union type object in practice.
// See https://codereview.chromium.org/1118993002/#msg5 for more details.
WTF_ALLOW_MOVE_AND_INIT_WITH_MEM_FUNCTIONS(blink::StringTreatNullAsEmptyStringOrTrustedScript)

#endif  // THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_STRING_TREAT_NULL_AS_EMPTY_STRING_OR_TRUSTED_SCRIPT_H_
