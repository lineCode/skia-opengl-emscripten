// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/union_container.h.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#ifndef THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_BYTE_STRING_SEQUENCE_SEQUENCE_OR_BYTE_STRING_BYTE_STRING_RECORD_H_
#define THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_BYTE_STRING_SEQUENCE_SEQUENCE_OR_BYTE_STRING_BYTE_STRING_RECORD_H_

#include "base/optional.h"
#include "third_party/blink/renderer/bindings/core/v8/dictionary.h"
#include "third_party/blink/renderer/bindings/core/v8/native_value_traits.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_binding_for_core.h"
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class CORE_EXPORT ByteStringSequenceSequenceOrByteStringByteStringRecord final {
  DISALLOW_NEW();
 public:
  ByteStringSequenceSequenceOrByteStringByteStringRecord();
  bool IsNull() const { return type_ == SpecificType::kNone; }

  bool IsByteStringByteStringRecord() const { return type_ == SpecificType::kByteStringByteStringRecord; }
  const Vector<std::pair<String, String>>& GetAsByteStringByteStringRecord() const;
  void SetByteStringByteStringRecord(const Vector<std::pair<String, String>>&);
  static ByteStringSequenceSequenceOrByteStringByteStringRecord FromByteStringByteStringRecord(const Vector<std::pair<String, String>>&);

  bool IsByteStringSequenceSequence() const { return type_ == SpecificType::kByteStringSequenceSequence; }
  const Vector<Vector<String>>& GetAsByteStringSequenceSequence() const;
  void SetByteStringSequenceSequence(const Vector<Vector<String>>&);
  static ByteStringSequenceSequenceOrByteStringByteStringRecord FromByteStringSequenceSequence(const Vector<Vector<String>>&);

  ByteStringSequenceSequenceOrByteStringByteStringRecord(const ByteStringSequenceSequenceOrByteStringByteStringRecord&);
  ~ByteStringSequenceSequenceOrByteStringByteStringRecord();
  ByteStringSequenceSequenceOrByteStringByteStringRecord& operator=(const ByteStringSequenceSequenceOrByteStringByteStringRecord&);
  void Trace(blink::Visitor*);

 private:
  enum class SpecificType {
    kNone,
    kByteStringByteStringRecord,
    kByteStringSequenceSequence,
  };
  SpecificType type_;

  Vector<std::pair<String, String>> byte_string_byte_string_record_;
  Vector<Vector<String>> byte_string_sequence_sequence_;

  friend CORE_EXPORT v8::Local<v8::Value> ToV8(const ByteStringSequenceSequenceOrByteStringByteStringRecord&, v8::Local<v8::Object>, v8::Isolate*);
};

class V8ByteStringSequenceSequenceOrByteStringByteStringRecord final {
 public:
  CORE_EXPORT static void ToImpl(v8::Isolate*, v8::Local<v8::Value>, ByteStringSequenceSequenceOrByteStringByteStringRecord&, UnionTypeConversionMode, ExceptionState&);
};

CORE_EXPORT v8::Local<v8::Value> ToV8(const ByteStringSequenceSequenceOrByteStringByteStringRecord&, v8::Local<v8::Object>, v8::Isolate*);

template <class CallbackInfo>
inline void V8SetReturnValue(const CallbackInfo& callbackInfo, ByteStringSequenceSequenceOrByteStringByteStringRecord& impl) {
  V8SetReturnValue(callbackInfo, ToV8(impl, callbackInfo.Holder(), callbackInfo.GetIsolate()));
}

template <class CallbackInfo>
inline void V8SetReturnValue(const CallbackInfo& callbackInfo, ByteStringSequenceSequenceOrByteStringByteStringRecord& impl, v8::Local<v8::Object> creationContext) {
  V8SetReturnValue(callbackInfo, ToV8(impl, creationContext, callbackInfo.GetIsolate()));
}

template <>
struct NativeValueTraits<ByteStringSequenceSequenceOrByteStringByteStringRecord> : public NativeValueTraitsBase<ByteStringSequenceSequenceOrByteStringByteStringRecord> {
  CORE_EXPORT static ByteStringSequenceSequenceOrByteStringByteStringRecord NativeValue(v8::Isolate*, v8::Local<v8::Value>, ExceptionState&);
  CORE_EXPORT static ByteStringSequenceSequenceOrByteStringByteStringRecord NullValue() { return ByteStringSequenceSequenceOrByteStringByteStringRecord(); }
};

template <>
struct V8TypeOf<ByteStringSequenceSequenceOrByteStringByteStringRecord> {
  typedef V8ByteStringSequenceSequenceOrByteStringByteStringRecord Type;
};

}  // namespace blink

// We need to set canInitializeWithMemset=true because HeapVector supports
// items that can initialize with memset or have a vtable. It is safe to
// set canInitializeWithMemset=true for a union type object in practice.
// See https://codereview.chromium.org/1118993002/#msg5 for more details.
WTF_ALLOW_MOVE_AND_INIT_WITH_MEM_FUNCTIONS(blink::ByteStringSequenceSequenceOrByteStringByteStringRecord)

#endif  // THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_BYTE_STRING_SEQUENCE_SEQUENCE_OR_BYTE_STRING_BYTE_STRING_RECORD_H_
