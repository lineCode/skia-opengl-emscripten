// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/dictionary_v8.cc.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#include "third_party/blink/renderer/bindings/core/v8/v8_progress_event_init.h"

#include "base/stl_util.h"
#include "third_party/blink/renderer/bindings/core/v8/idl_types.h"
#include "third_party/blink/renderer/bindings/core/v8/native_value_traits_impl.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_event_init.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"

namespace blink {

static const v8::Eternal<v8::Name>* eternalV8ProgressEventInitKeys(v8::Isolate* isolate) {
  static const char* const kKeys[] = {
    "lengthComputable",
    "loaded",
    "total",
  };
  return V8PerIsolateData::From(isolate)->FindOrCreateEternalNameCache(
      kKeys, kKeys, base::size(kKeys));
}

void V8ProgressEventInit::ToImpl(v8::Isolate* isolate, v8::Local<v8::Value> v8_value, ProgressEventInit* impl, ExceptionState& exception_state) {
  if (IsUndefinedOrNull(v8_value)) {
    return;
  }
  if (!v8_value->IsObject()) {
    exception_state.ThrowTypeError("cannot convert to dictionary.");
    return;
  }
  v8::Local<v8::Object> v8Object = v8_value.As<v8::Object>();
  ALLOW_UNUSED_LOCAL(v8Object);

  V8EventInit::ToImpl(isolate, v8_value, impl, exception_state);
  if (exception_state.HadException())
    return;

  const v8::Eternal<v8::Name>* keys = eternalV8ProgressEventInitKeys(isolate);
  v8::TryCatch block(isolate);
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Value> length_computable_value;
  if (!v8Object->Get(context, keys[0].Get(isolate)).ToLocal(&length_computable_value)) {
    exception_state.RethrowV8Exception(block.Exception());
    return;
  }
  if (length_computable_value.IsEmpty() || length_computable_value->IsUndefined()) {
    // Do nothing.
  } else {
    bool length_computable_cpp_value = NativeValueTraits<IDLBoolean>::NativeValue(isolate, length_computable_value, exception_state);
    if (exception_state.HadException())
      return;
    impl->setLengthComputable(length_computable_cpp_value);
  }

  v8::Local<v8::Value> loaded_value;
  if (!v8Object->Get(context, keys[1].Get(isolate)).ToLocal(&loaded_value)) {
    exception_state.RethrowV8Exception(block.Exception());
    return;
  }
  if (loaded_value.IsEmpty() || loaded_value->IsUndefined()) {
    // Do nothing.
  } else {
    uint64_t loaded_cpp_value = NativeValueTraits<IDLUnsignedLongLong>::NativeValue(isolate, loaded_value, exception_state);
    if (exception_state.HadException())
      return;
    impl->setLoaded(loaded_cpp_value);
  }

  v8::Local<v8::Value> total_value;
  if (!v8Object->Get(context, keys[2].Get(isolate)).ToLocal(&total_value)) {
    exception_state.RethrowV8Exception(block.Exception());
    return;
  }
  if (total_value.IsEmpty() || total_value->IsUndefined()) {
    // Do nothing.
  } else {
    uint64_t total_cpp_value = NativeValueTraits<IDLUnsignedLongLong>::NativeValue(isolate, total_value, exception_state);
    if (exception_state.HadException())
      return;
    impl->setTotal(total_cpp_value);
  }
}

v8::Local<v8::Value> ProgressEventInit::ToV8Impl(v8::Local<v8::Object> creationContext, v8::Isolate* isolate) const {
  v8::Local<v8::Object> v8Object = v8::Object::New(isolate);
  if (!toV8ProgressEventInit(this, v8Object, creationContext, isolate))
    return v8::Undefined(isolate);
  return v8Object;
}

bool toV8ProgressEventInit(const ProgressEventInit* impl, v8::Local<v8::Object> dictionary, v8::Local<v8::Object> creationContext, v8::Isolate* isolate) {
  if (!toV8EventInit(impl, dictionary, creationContext, isolate))
    return false;

  const v8::Eternal<v8::Name>* keys = eternalV8ProgressEventInitKeys(isolate);
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

  v8::Local<v8::Value> length_computable_value;
  bool length_computable_has_value_or_default = false;
  if (impl->hasLengthComputable()) {
    length_computable_value = v8::Boolean::New(isolate, impl->lengthComputable());
    length_computable_has_value_or_default = true;
  } else {
    length_computable_value = v8::Boolean::New(isolate, false);
    length_computable_has_value_or_default = true;
  }
  if (length_computable_has_value_or_default &&
      !create_property(0, length_computable_value)) {
    return false;
  }

  v8::Local<v8::Value> loaded_value;
  bool loaded_has_value_or_default = false;
  if (impl->hasLoaded()) {
    loaded_value = v8::Number::New(isolate, static_cast<double>(impl->loaded()));
    loaded_has_value_or_default = true;
  } else {
    loaded_value = v8::Number::New(isolate, static_cast<double>(0));
    loaded_has_value_or_default = true;
  }
  if (loaded_has_value_or_default &&
      !create_property(1, loaded_value)) {
    return false;
  }

  v8::Local<v8::Value> total_value;
  bool total_has_value_or_default = false;
  if (impl->hasTotal()) {
    total_value = v8::Number::New(isolate, static_cast<double>(impl->total()));
    total_has_value_or_default = true;
  } else {
    total_value = v8::Number::New(isolate, static_cast<double>(0));
    total_has_value_or_default = true;
  }
  if (total_has_value_or_default &&
      !create_property(2, total_value)) {
    return false;
  }

  return true;
}

ProgressEventInit* NativeValueTraits<ProgressEventInit>::NativeValue(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exception_state) {
  ProgressEventInit* impl = ProgressEventInit::Create();
  V8ProgressEventInit::ToImpl(isolate, value, impl, exception_state);
  return impl;
}

}  // namespace blink
