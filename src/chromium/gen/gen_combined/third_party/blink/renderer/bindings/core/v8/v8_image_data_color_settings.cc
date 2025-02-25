// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/dictionary_v8.cc.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#include "third_party/blink/renderer/bindings/core/v8/v8_image_data_color_settings.h"

#include "base/stl_util.h"
#include "third_party/blink/renderer/bindings/core/v8/idl_types.h"
#include "third_party/blink/renderer/bindings/core/v8/native_value_traits_impl.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"

namespace blink {

static const v8::Eternal<v8::Name>* eternalV8ImageDataColorSettingsKeys(v8::Isolate* isolate) {
  static const char* const kKeys[] = {
    "colorSpace",
    "storageFormat",
  };
  return V8PerIsolateData::From(isolate)->FindOrCreateEternalNameCache(
      kKeys, kKeys, base::size(kKeys));
}

void V8ImageDataColorSettings::ToImpl(v8::Isolate* isolate, v8::Local<v8::Value> v8_value, ImageDataColorSettings* impl, ExceptionState& exception_state) {
  if (IsUndefinedOrNull(v8_value)) {
    return;
  }
  if (!v8_value->IsObject()) {
    exception_state.ThrowTypeError("cannot convert to dictionary.");
    return;
  }
  v8::Local<v8::Object> v8Object = v8_value.As<v8::Object>();
  ALLOW_UNUSED_LOCAL(v8Object);

  const v8::Eternal<v8::Name>* keys = eternalV8ImageDataColorSettingsKeys(isolate);
  v8::TryCatch block(isolate);
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Value> color_space_value;
  if (!v8Object->Get(context, keys[0].Get(isolate)).ToLocal(&color_space_value)) {
    exception_state.RethrowV8Exception(block.Exception());
    return;
  }
  if (color_space_value.IsEmpty() || color_space_value->IsUndefined()) {
    // Do nothing.
  } else {
    V8StringResource<> color_space_cpp_value = color_space_value;
    if (!color_space_cpp_value.Prepare(exception_state))
      return;
    const char* kValidValues[] = {
        "srgb",
        "linear-rgb",
        "rec2020",
        "p3",
    };
    if (!IsValidEnum(color_space_cpp_value, kValidValues, base::size(kValidValues), "CanvasColorSpace", exception_state))
      return;
    impl->setColorSpace(color_space_cpp_value);
  }

  v8::Local<v8::Value> storage_format_value;
  if (!v8Object->Get(context, keys[1].Get(isolate)).ToLocal(&storage_format_value)) {
    exception_state.RethrowV8Exception(block.Exception());
    return;
  }
  if (storage_format_value.IsEmpty() || storage_format_value->IsUndefined()) {
    // Do nothing.
  } else {
    V8StringResource<> storage_format_cpp_value = storage_format_value;
    if (!storage_format_cpp_value.Prepare(exception_state))
      return;
    const char* kValidValues[] = {
        "uint8",
        "uint16",
        "float32",
    };
    if (!IsValidEnum(storage_format_cpp_value, kValidValues, base::size(kValidValues), "ImageDataStorageFormat", exception_state))
      return;
    impl->setStorageFormat(storage_format_cpp_value);
  }
}

v8::Local<v8::Value> ImageDataColorSettings::ToV8Impl(v8::Local<v8::Object> creationContext, v8::Isolate* isolate) const {
  v8::Local<v8::Object> v8Object = v8::Object::New(isolate);
  if (!toV8ImageDataColorSettings(this, v8Object, creationContext, isolate))
    return v8::Undefined(isolate);
  return v8Object;
}

bool toV8ImageDataColorSettings(const ImageDataColorSettings* impl, v8::Local<v8::Object> dictionary, v8::Local<v8::Object> creationContext, v8::Isolate* isolate) {
  const v8::Eternal<v8::Name>* keys = eternalV8ImageDataColorSettingsKeys(isolate);
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

  v8::Local<v8::Value> color_space_value;
  bool color_space_has_value_or_default = false;
  if (impl->hasColorSpace()) {
    color_space_value = V8String(isolate, impl->colorSpace());
    color_space_has_value_or_default = true;
  } else {
    color_space_value = V8String(isolate, "srgb");
    color_space_has_value_or_default = true;
  }
  if (color_space_has_value_or_default &&
      !create_property(0, color_space_value)) {
    return false;
  }

  v8::Local<v8::Value> storage_format_value;
  bool storage_format_has_value_or_default = false;
  if (impl->hasStorageFormat()) {
    storage_format_value = V8String(isolate, impl->storageFormat());
    storage_format_has_value_or_default = true;
  } else {
    storage_format_value = V8String(isolate, "uint8");
    storage_format_has_value_or_default = true;
  }
  if (storage_format_has_value_or_default &&
      !create_property(1, storage_format_value)) {
    return false;
  }

  return true;
}

ImageDataColorSettings* NativeValueTraits<ImageDataColorSettings>::NativeValue(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exception_state) {
  ImageDataColorSettings* impl = ImageDataColorSettings::Create();
  V8ImageDataColorSettings::ToImpl(isolate, value, impl, exception_state);
  return impl;
}

}  // namespace blink
