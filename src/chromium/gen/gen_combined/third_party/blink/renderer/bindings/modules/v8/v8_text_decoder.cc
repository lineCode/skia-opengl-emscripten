// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/interface.cc.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#include "third_party/blink/renderer/bindings/modules/v8/v8_text_decoder.h"

#include <algorithm>

#include "base/memory/scoped_refptr.h"
#include "third_party/blink/renderer/bindings/core/v8/idl_types.h"
#include "third_party/blink/renderer/bindings/core/v8/native_value_traits_impl.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_array_buffer.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_array_buffer_view.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_dom_configuration.h"
#include "third_party/blink/renderer/bindings/modules/v8/v8_text_decode_options.h"
#include "third_party/blink/renderer/bindings/modules/v8/v8_text_decoder_options.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/use_counter.h"
#include "third_party/blink/renderer/core/typed_arrays/array_buffer_view_helpers.h"
#include "third_party/blink/renderer/core/typed_arrays/flexible_array_buffer_view.h"
#include "third_party/blink/renderer/platform/bindings/exception_messages.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include "third_party/blink/renderer/platform/bindings/runtime_call_stats.h"
#include "third_party/blink/renderer/platform/bindings/v8_object_constructor.h"
#include "third_party/blink/renderer/platform/scheduler/public/cooperative_scheduling_manager.h"
#include "third_party/blink/renderer/platform/wtf/get_ptr.h"

namespace blink {

// Suppress warning: global constructors, because struct WrapperTypeInfo is trivial
// and does not depend on another global objects.
#if defined(COMPONENT_BUILD) && defined(WIN32) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif
const WrapperTypeInfo v8_text_decoder_wrapper_type_info = {
    gin::kEmbedderBlink,
    V8TextDecoder::DomTemplate,
    nullptr,
    "TextDecoder",
    nullptr,
    WrapperTypeInfo::kWrapperTypeObjectPrototype,
    WrapperTypeInfo::kObjectClassId,
    WrapperTypeInfo::kNotInheritFromActiveScriptWrappable,
};
#if defined(COMPONENT_BUILD) && defined(WIN32) && defined(__clang__)
#pragma clang diagnostic pop
#endif

// This static member must be declared by DEFINE_WRAPPERTYPEINFO in TextDecoder.h.
// For details, see the comment of DEFINE_WRAPPERTYPEINFO in
// platform/bindings/ScriptWrappable.h.
const WrapperTypeInfo& TextDecoder::wrapper_type_info_ = v8_text_decoder_wrapper_type_info;

// not [ActiveScriptWrappable]
static_assert(
    !std::is_base_of<ActiveScriptWrappableBase, TextDecoder>::value,
    "TextDecoder inherits from ActiveScriptWrappable<>, but is not specifying "
    "[ActiveScriptWrappable] extended attribute in the IDL file.  "
    "Be consistent.");
static_assert(
    std::is_same<decltype(&TextDecoder::HasPendingActivity),
                 decltype(&ScriptWrappable::HasPendingActivity)>::value,
    "TextDecoder is overriding hasPendingActivity(), but is not specifying "
    "[ActiveScriptWrappable] extended attribute in the IDL file.  "
    "Be consistent.");

namespace text_decoder_v8_internal {

static void EncodingAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TextDecoder* impl = V8TextDecoder::ToImpl(holder);

  V8SetReturnValueString(info, impl->encoding(), info.GetIsolate());
}

static void FatalAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TextDecoder* impl = V8TextDecoder::ToImpl(holder);

  V8SetReturnValueBool(info, impl->fatal());
}

static void IgnoreBOMAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TextDecoder* impl = V8TextDecoder::ToImpl(holder);

  V8SetReturnValueBool(info, impl->ignoreBOM());
}

static void DecodeMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TextDecoder", "decode");

  TextDecoder* impl = V8TextDecoder::ToImpl(info.Holder());

  ArrayBufferOrArrayBufferView input;
  TextDecodeOptions* options;
  int num_args_passed = info.Length();
  while (num_args_passed > 0) {
    if (!info[num_args_passed - 1]->IsUndefined())
      break;
    --num_args_passed;
  }
  if (UNLIKELY(num_args_passed <= 0)) {
    String result = impl->decode(exception_state);
    if (exception_state.HadException()) {
      return;
    }
    V8SetReturnValueString(info, result, info.GetIsolate());
    return;
  }
  V8ArrayBufferOrArrayBufferView::ToImpl(info.GetIsolate(), info[0], input, UnionTypeConversionMode::kNotNullable, exception_state);
  if (exception_state.HadException())
    return;

  if (!info[1]->IsNullOrUndefined() && !info[1]->IsObject()) {
    exception_state.ThrowTypeError("parameter 2 ('options') is not an object.");
    return;
  }
  options = NativeValueTraits<TextDecodeOptions>::NativeValue(info.GetIsolate(), info[1], exception_state);
  if (exception_state.HadException())
    return;

  String result = impl->decode(input, options, exception_state);
  if (exception_state.HadException()) {
    return;
  }
  V8SetReturnValueString(info, result, info.GetIsolate());
}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TextDecoder_ConstructorCallback");

  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kConstructionContext, "TextDecoder");

  V8StringResource<> label;
  TextDecoderOptions* options;
  if (!info[0]->IsUndefined()) {
    label = info[0];
    if (!label.Prepare())
      return;
  } else {
    label = "utf-8";
  }
  if (!info[1]->IsNullOrUndefined() && !info[1]->IsObject()) {
    exception_state.ThrowTypeError("parameter 2 ('options') is not an object.");
    return;
  }
  options = NativeValueTraits<TextDecoderOptions>::NativeValue(info.GetIsolate(), info[1], exception_state);
  if (exception_state.HadException())
    return;

  TextDecoder* impl = TextDecoder::Create(label, options, exception_state);
  if (exception_state.HadException()) {
    return;
  }
  v8::Local<v8::Object> wrapper = info.Holder();
  wrapper = impl->AssociateWithWrapper(info.GetIsolate(), V8TextDecoder::GetWrapperTypeInfo(), wrapper);
  V8SetReturnValue(info, wrapper);
}

MODULES_EXPORT void ConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TextDecoder_Constructor");

  ExecutionContext* execution_context_for_measurement = CurrentExecutionContext(info.GetIsolate());
  UseCounter::Count(execution_context_for_measurement, WebFeature::kTextDecoderConstructor);
  if (!info.IsConstructCall()) {
    V8ThrowException::ThrowTypeError(
        info.GetIsolate(),
        ExceptionMessages::ConstructorNotCallableAsFunction("TextDecoder"));
    return;
  }

  if (ConstructorMode::Current(info.GetIsolate()) == ConstructorMode::kWrapExistingObject) {
    V8SetReturnValue(info, info.Holder());
    return;
  }

  text_decoder_v8_internal::Constructor(info);
}

}  // namespace text_decoder_v8_internal

void V8TextDecoder::EncodingAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TextDecoder_encoding_Getter");

  text_decoder_v8_internal::EncodingAttributeGetter(info);
}

void V8TextDecoder::FatalAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TextDecoder_fatal_Getter");

  text_decoder_v8_internal::FatalAttributeGetter(info);
}

void V8TextDecoder::IgnoreBOMAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TextDecoder_ignoreBOM_Getter");

  text_decoder_v8_internal::IgnoreBOMAttributeGetter(info);
}

void V8TextDecoder::DecodeMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_TextDecoder_decode");

  ExecutionContext* execution_context_for_measurement = CurrentExecutionContext(info.GetIsolate());
  UseCounter::Count(execution_context_for_measurement, WebFeature::kTextDecoderDecode);
  text_decoder_v8_internal::DecodeMethod(info);
}

static constexpr V8DOMConfiguration::AccessorConfiguration kV8TextDecoderAccessors[] = {
    { "encoding", V8TextDecoder::EncodingAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
    { "fatal", V8TextDecoder::FatalAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
    { "ignoreBOM", V8TextDecoder::IgnoreBOMAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
};

static constexpr V8DOMConfiguration::MethodConfiguration kV8TextDecoderMethods[] = {
    {"decode", V8TextDecoder::DecodeMethodCallback, 0, v8::None, V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kDoNotCheckAccess, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAllWorlds},
};

static void InstallV8TextDecoderTemplate(
    v8::Isolate* isolate,
    const DOMWrapperWorld& world,
    v8::Local<v8::FunctionTemplate> interface_template) {
  // Initialize the interface object's template.
  V8DOMConfiguration::InitializeDOMInterfaceTemplate(isolate, interface_template, V8TextDecoder::GetWrapperTypeInfo()->interface_name, v8::Local<v8::FunctionTemplate>(), V8TextDecoder::kInternalFieldCount);
  interface_template->SetCallHandler(text_decoder_v8_internal::ConstructorCallback);
  interface_template->SetLength(0);

  v8::Local<v8::Signature> signature = v8::Signature::New(isolate, interface_template);
  ALLOW_UNUSED_LOCAL(signature);
  v8::Local<v8::ObjectTemplate> instance_template = interface_template->InstanceTemplate();
  ALLOW_UNUSED_LOCAL(instance_template);
  v8::Local<v8::ObjectTemplate> prototype_template = interface_template->PrototypeTemplate();
  ALLOW_UNUSED_LOCAL(prototype_template);

  // Register IDL constants, attributes and operations.
  V8DOMConfiguration::InstallAccessors(
      isolate, world, instance_template, prototype_template, interface_template,
      signature, kV8TextDecoderAccessors, base::size(kV8TextDecoderAccessors));
  V8DOMConfiguration::InstallMethods(
      isolate, world, instance_template, prototype_template, interface_template,
      signature, kV8TextDecoderMethods, base::size(kV8TextDecoderMethods));

  // Custom signature

  V8TextDecoder::InstallRuntimeEnabledFeaturesOnTemplate(
      isolate, world, interface_template);
}

void V8TextDecoder::InstallRuntimeEnabledFeaturesOnTemplate(
    v8::Isolate* isolate,
    const DOMWrapperWorld& world,
    v8::Local<v8::FunctionTemplate> interface_template) {
  v8::Local<v8::Signature> signature = v8::Signature::New(isolate, interface_template);
  ALLOW_UNUSED_LOCAL(signature);
  v8::Local<v8::ObjectTemplate> instance_template = interface_template->InstanceTemplate();
  ALLOW_UNUSED_LOCAL(instance_template);
  v8::Local<v8::ObjectTemplate> prototype_template = interface_template->PrototypeTemplate();
  ALLOW_UNUSED_LOCAL(prototype_template);

  // Register IDL constants, attributes and operations.

  // Custom signature
}

v8::Local<v8::FunctionTemplate> V8TextDecoder::DomTemplate(
    v8::Isolate* isolate, const DOMWrapperWorld& world) {
  return V8DOMConfiguration::DomClassTemplate(
      isolate, world, const_cast<WrapperTypeInfo*>(V8TextDecoder::GetWrapperTypeInfo()),
      InstallV8TextDecoderTemplate);
}

bool V8TextDecoder::HasInstance(v8::Local<v8::Value> v8_value, v8::Isolate* isolate) {
  return V8PerIsolateData::From(isolate)->HasInstance(V8TextDecoder::GetWrapperTypeInfo(), v8_value);
}

v8::Local<v8::Object> V8TextDecoder::FindInstanceInPrototypeChain(
    v8::Local<v8::Value> v8_value, v8::Isolate* isolate) {
  return V8PerIsolateData::From(isolate)->FindInstanceInPrototypeChain(
      V8TextDecoder::GetWrapperTypeInfo(), v8_value);
}

TextDecoder* V8TextDecoder::ToImplWithTypeCheck(
    v8::Isolate* isolate, v8::Local<v8::Value> value) {
  return HasInstance(value, isolate) ? ToImpl(v8::Local<v8::Object>::Cast(value)) : nullptr;
}

TextDecoder* NativeValueTraits<TextDecoder>::NativeValue(
    v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exception_state) {
  TextDecoder* native_value = V8TextDecoder::ToImplWithTypeCheck(isolate, value);
  if (!native_value) {
    exception_state.ThrowTypeError(ExceptionMessages::FailedToConvertJSValue(
        "TextDecoder"));
  }
  return native_value;
}

}  // namespace blink
