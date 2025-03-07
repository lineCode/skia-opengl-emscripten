// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/dictionary_v8.cc.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#include "third_party/blink/renderer/bindings/core/v8/v8_application_cache_error_event_init.h"

#include "base/stl_util.h"
#include "third_party/blink/renderer/bindings/core/v8/idl_types.h"
#include "third_party/blink/renderer/bindings/core/v8/native_value_traits_impl.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_event_init.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"

namespace blink {

static const v8::Eternal<v8::Name>* eternalV8ApplicationCacheErrorEventInitKeys(v8::Isolate* isolate) {
  static const char* const kKeys[] = {
    "message",
    "reason",
    "status",
    "url",
  };
  return V8PerIsolateData::From(isolate)->FindOrCreateEternalNameCache(
      kKeys, kKeys, base::size(kKeys));
}

void V8ApplicationCacheErrorEventInit::ToImpl(v8::Isolate* isolate, v8::Local<v8::Value> v8_value, ApplicationCacheErrorEventInit* impl, ExceptionState& exception_state) {
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

  const v8::Eternal<v8::Name>* keys = eternalV8ApplicationCacheErrorEventInitKeys(isolate);
  v8::TryCatch block(isolate);
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Value> message_value;
  if (!v8Object->Get(context, keys[0].Get(isolate)).ToLocal(&message_value)) {
    exception_state.RethrowV8Exception(block.Exception());
    return;
  }
  if (message_value.IsEmpty() || message_value->IsUndefined()) {
    // Do nothing.
  } else {
    V8StringResource<> message_cpp_value = message_value;
    if (!message_cpp_value.Prepare(exception_state))
      return;
    impl->setMessage(message_cpp_value);
  }

  v8::Local<v8::Value> reason_value;
  if (!v8Object->Get(context, keys[1].Get(isolate)).ToLocal(&reason_value)) {
    exception_state.RethrowV8Exception(block.Exception());
    return;
  }
  if (reason_value.IsEmpty() || reason_value->IsUndefined()) {
    // Do nothing.
  } else {
    V8StringResource<> reason_cpp_value = reason_value;
    if (!reason_cpp_value.Prepare(exception_state))
      return;
    impl->setReason(reason_cpp_value);
  }

  v8::Local<v8::Value> status_value;
  if (!v8Object->Get(context, keys[2].Get(isolate)).ToLocal(&status_value)) {
    exception_state.RethrowV8Exception(block.Exception());
    return;
  }
  if (status_value.IsEmpty() || status_value->IsUndefined()) {
    // Do nothing.
  } else {
    uint16_t status_cpp_value = NativeValueTraits<IDLUnsignedShort>::NativeValue(isolate, status_value, exception_state);
    if (exception_state.HadException())
      return;
    impl->setStatus(status_cpp_value);
  }

  v8::Local<v8::Value> url_value;
  if (!v8Object->Get(context, keys[3].Get(isolate)).ToLocal(&url_value)) {
    exception_state.RethrowV8Exception(block.Exception());
    return;
  }
  if (url_value.IsEmpty() || url_value->IsUndefined()) {
    // Do nothing.
  } else {
    V8StringResource<> url_cpp_value = url_value;
    if (!url_cpp_value.Prepare(exception_state))
      return;
    impl->setURL(url_cpp_value);
  }
}

v8::Local<v8::Value> ApplicationCacheErrorEventInit::ToV8Impl(v8::Local<v8::Object> creationContext, v8::Isolate* isolate) const {
  v8::Local<v8::Object> v8Object = v8::Object::New(isolate);
  if (!toV8ApplicationCacheErrorEventInit(this, v8Object, creationContext, isolate))
    return v8::Undefined(isolate);
  return v8Object;
}

bool toV8ApplicationCacheErrorEventInit(const ApplicationCacheErrorEventInit* impl, v8::Local<v8::Object> dictionary, v8::Local<v8::Object> creationContext, v8::Isolate* isolate) {
  if (!toV8EventInit(impl, dictionary, creationContext, isolate))
    return false;

  const v8::Eternal<v8::Name>* keys = eternalV8ApplicationCacheErrorEventInitKeys(isolate);
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

  v8::Local<v8::Value> message_value;
  bool message_has_value_or_default = false;
  if (impl->hasMessage()) {
    message_value = V8String(isolate, impl->message());
    message_has_value_or_default = true;
  }
  if (message_has_value_or_default &&
      !create_property(0, message_value)) {
    return false;
  }

  v8::Local<v8::Value> reason_value;
  bool reason_has_value_or_default = false;
  if (impl->hasReason()) {
    reason_value = V8String(isolate, impl->reason());
    reason_has_value_or_default = true;
  }
  if (reason_has_value_or_default &&
      !create_property(1, reason_value)) {
    return false;
  }

  v8::Local<v8::Value> status_value;
  bool status_has_value_or_default = false;
  if (impl->hasStatus()) {
    status_value = v8::Integer::NewFromUnsigned(isolate, impl->status());
    status_has_value_or_default = true;
  }
  if (status_has_value_or_default &&
      !create_property(2, status_value)) {
    return false;
  }

  v8::Local<v8::Value> url_value;
  bool url_has_value_or_default = false;
  if (impl->hasURL()) {
    url_value = V8String(isolate, impl->url());
    url_has_value_or_default = true;
  }
  if (url_has_value_or_default &&
      !create_property(3, url_value)) {
    return false;
  }

  return true;
}

ApplicationCacheErrorEventInit* NativeValueTraits<ApplicationCacheErrorEventInit>::NativeValue(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exception_state) {
  ApplicationCacheErrorEventInit* impl = ApplicationCacheErrorEventInit::Create();
  V8ApplicationCacheErrorEventInit::ToImpl(isolate, value, impl, exception_state);
  return impl;
}

}  // namespace blink
