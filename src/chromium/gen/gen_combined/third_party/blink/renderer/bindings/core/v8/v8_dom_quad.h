// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/interface.h.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#ifndef THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_V8_DOM_QUAD_H_
#define THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_V8_DOM_QUAD_H_

#include "third_party/blink/renderer/bindings/core/v8/generated_code_helper.h"
#include "third_party/blink/renderer/bindings/core/v8/native_value_traits.h"
#include "third_party/blink/renderer/bindings/core/v8/to_v8_for_core.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_binding_for_core.h"
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/geometry/dom_quad.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"
#include "third_party/blink/renderer/platform/bindings/v8_dom_wrapper.h"
#include "third_party/blink/renderer/platform/bindings/wrapper_type_info.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

CORE_EXPORT extern const WrapperTypeInfo v8_dom_quad_wrapper_type_info;

class V8DOMQuad {
  STATIC_ONLY(V8DOMQuad);
 public:
  CORE_EXPORT static bool HasInstance(v8::Local<v8::Value>, v8::Isolate*);
  static v8::Local<v8::Object> FindInstanceInPrototypeChain(v8::Local<v8::Value>, v8::Isolate*);
  CORE_EXPORT static v8::Local<v8::FunctionTemplate> DomTemplate(v8::Isolate*, const DOMWrapperWorld&);
  static DOMQuad* ToImpl(v8::Local<v8::Object> object) {
    return ToScriptWrappable(object)->ToImpl<DOMQuad>();
  }
  CORE_EXPORT static DOMQuad* ToImplWithTypeCheck(v8::Isolate*, v8::Local<v8::Value>);

  CORE_EXPORT static constexpr const WrapperTypeInfo* GetWrapperTypeInfo() {
    return &v8_dom_quad_wrapper_type_info;
  }

  static constexpr int kInternalFieldCount = kV8DefaultWrapperInternalFieldCount;

  // Callback functions

  CORE_EXPORT static void P1AttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>&);
  CORE_EXPORT static void P2AttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>&);
  CORE_EXPORT static void P3AttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>&);
  CORE_EXPORT static void P4AttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>&);

  CORE_EXPORT static void FromRectMethodCallback(const v8::FunctionCallbackInfo<v8::Value>&);
  CORE_EXPORT static void FromQuadMethodCallback(const v8::FunctionCallbackInfo<v8::Value>&);
  CORE_EXPORT static void GetBoundsMethodCallback(const v8::FunctionCallbackInfo<v8::Value>&);
  CORE_EXPORT static void ToJSONMethodCallback(const v8::FunctionCallbackInfo<v8::Value>&);

  static void InstallRuntimeEnabledFeaturesOnTemplate(
      v8::Isolate*,
      const DOMWrapperWorld&,
      v8::Local<v8::FunctionTemplate> interface_template);
};

template <>
struct NativeValueTraits<DOMQuad> : public NativeValueTraitsBase<DOMQuad> {
  CORE_EXPORT static DOMQuad* NativeValue(v8::Isolate*, v8::Local<v8::Value>, ExceptionState&);
  CORE_EXPORT static DOMQuad* NullValue() { return nullptr; }
};

template <>
struct V8TypeOf<DOMQuad> {
  typedef V8DOMQuad Type;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_V8_DOM_QUAD_H_
