// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/interface.cc.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#include "third_party/blink/renderer/bindings/core/v8/v8_html_label_element.h"

#include <algorithm>

#include "base/memory/scoped_refptr.h"
#include "third_party/blink/renderer/bindings/core/v8/idl_types.h"
#include "third_party/blink/renderer/bindings/core/v8/native_value_traits_impl.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_dom_configuration.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_html_constructor.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_html_element.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_html_form_element.h"
#include "third_party/blink/renderer/core/animation/element_animation.h"
#include "third_party/blink/renderer/core/css/cssom/element_computed_style_map.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/core/fullscreen/element_fullscreen.h"
#include "third_party/blink/renderer/core/html/custom/ce_reactions_scope.h"
#include "third_party/blink/renderer/core/html/custom/v0_custom_element_processing_stack.h"
#include "third_party/blink/renderer/core/html_names.h"
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
const WrapperTypeInfo v8_html_label_element_wrapper_type_info = {
    gin::kEmbedderBlink,
    V8HTMLLabelElement::DomTemplate,
    nullptr,
    "HTMLLabelElement",
    V8HTMLElement::GetWrapperTypeInfo(),
    WrapperTypeInfo::kWrapperTypeObjectPrototype,
    WrapperTypeInfo::kNodeClassId,
    WrapperTypeInfo::kNotInheritFromActiveScriptWrappable,
};
#if defined(COMPONENT_BUILD) && defined(WIN32) && defined(__clang__)
#pragma clang diagnostic pop
#endif

// This static member must be declared by DEFINE_WRAPPERTYPEINFO in HTMLLabelElement.h.
// For details, see the comment of DEFINE_WRAPPERTYPEINFO in
// platform/bindings/ScriptWrappable.h.
const WrapperTypeInfo& HTMLLabelElement::wrapper_type_info_ = v8_html_label_element_wrapper_type_info;

// not [ActiveScriptWrappable]
static_assert(
    !std::is_base_of<ActiveScriptWrappableBase, HTMLLabelElement>::value,
    "HTMLLabelElement inherits from ActiveScriptWrappable<>, but is not specifying "
    "[ActiveScriptWrappable] extended attribute in the IDL file.  "
    "Be consistent.");
static_assert(
    std::is_same<decltype(&HTMLLabelElement::HasPendingActivity),
                 decltype(&ScriptWrappable::HasPendingActivity)>::value,
    "HTMLLabelElement is overriding hasPendingActivity(), but is not specifying "
    "[ActiveScriptWrappable] extended attribute in the IDL file.  "
    "Be consistent.");

namespace html_label_element_v8_internal {

static void FormAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  HTMLLabelElement* impl = V8HTMLLabelElement::ToImpl(holder);

  V8SetReturnValueFast(info, WTF::GetPtr(impl->form()), impl);
}

static void HTMLForAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  HTMLLabelElement* impl = V8HTMLLabelElement::ToImpl(holder);

  V8SetReturnValueString(info, impl->FastGetAttribute(html_names::kForAttr), info.GetIsolate());
}

static void HTMLForAttributeSetter(
    v8::Local<v8::Value> v8_value, const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  ALLOW_UNUSED_LOCAL(isolate);

  v8::Local<v8::Object> holder = info.Holder();
  ALLOW_UNUSED_LOCAL(holder);

  HTMLLabelElement* impl = V8HTMLLabelElement::ToImpl(holder);

  V0CustomElementProcessingStack::CallbackDeliveryScope delivery_scope;

  CEReactionsScope ce_reactions_scope;

  // Prepare the value to be set.
  V8StringResource<> cpp_value = v8_value;
  if (!cpp_value.Prepare())
    return;

  impl->setAttribute(html_names::kForAttr, cpp_value);
}

static void ControlAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  HTMLLabelElement* impl = V8HTMLLabelElement::ToImpl(holder);

  V8SetReturnValueFast(info, WTF::GetPtr(impl->control()), impl);
}

CORE_EXPORT void ConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_HTMLLabelElement_Constructor");

  if (!info.IsConstructCall()) {
    V8ThrowException::ThrowTypeError(
        info.GetIsolate(),
        ExceptionMessages::ConstructorNotCallableAsFunction("HTMLLabelElement"));
    return;
  }

  if (ConstructorMode::Current(info.GetIsolate()) == ConstructorMode::kWrapExistingObject) {
    V8SetReturnValue(info, info.Holder());
    return;
  }

  V8HTMLConstructor::HtmlConstructor(
      info, *V8HTMLLabelElement::GetWrapperTypeInfo(), HTMLElementType::kHTMLLabelElement);
}

}  // namespace html_label_element_v8_internal

void V8HTMLLabelElement::FormAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_HTMLLabelElement_form_Getter");

  html_label_element_v8_internal::FormAttributeGetter(info);
}

void V8HTMLLabelElement::HTMLForAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_HTMLLabelElement_htmlFor_Getter");

  html_label_element_v8_internal::HTMLForAttributeGetter(info);
}

void V8HTMLLabelElement::HTMLForAttributeSetterCallback(
    const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_HTMLLabelElement_htmlFor_Setter");

  v8::Local<v8::Value> v8_value = info[0];

  html_label_element_v8_internal::HTMLForAttributeSetter(v8_value, info);
}

void V8HTMLLabelElement::ControlAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_HTMLLabelElement_control_Getter");

  html_label_element_v8_internal::ControlAttributeGetter(info);
}

static constexpr V8DOMConfiguration::AccessorConfiguration kV8HTMLLabelElementAccessors[] = {
    { "form", V8HTMLLabelElement::FormAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
    { "htmlFor", V8HTMLLabelElement::HTMLForAttributeGetterCallback, V8HTMLLabelElement::HTMLForAttributeSetterCallback, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::None), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
    { "control", V8HTMLLabelElement::ControlAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
};

static void InstallV8HTMLLabelElementTemplate(
    v8::Isolate* isolate,
    const DOMWrapperWorld& world,
    v8::Local<v8::FunctionTemplate> interface_template) {
  // Initialize the interface object's template.
  V8DOMConfiguration::InitializeDOMInterfaceTemplate(isolate, interface_template, V8HTMLLabelElement::GetWrapperTypeInfo()->interface_name, V8HTMLElement::DomTemplate(isolate, world), V8HTMLLabelElement::kInternalFieldCount);
  interface_template->SetCallHandler(html_label_element_v8_internal::ConstructorCallback);
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
      signature, kV8HTMLLabelElementAccessors, base::size(kV8HTMLLabelElementAccessors));

  // Custom signature

  V8HTMLLabelElement::InstallRuntimeEnabledFeaturesOnTemplate(
      isolate, world, interface_template);
}

void V8HTMLLabelElement::InstallRuntimeEnabledFeaturesOnTemplate(
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

v8::Local<v8::FunctionTemplate> V8HTMLLabelElement::DomTemplate(
    v8::Isolate* isolate, const DOMWrapperWorld& world) {
  return V8DOMConfiguration::DomClassTemplate(
      isolate, world, const_cast<WrapperTypeInfo*>(V8HTMLLabelElement::GetWrapperTypeInfo()),
      InstallV8HTMLLabelElementTemplate);
}

bool V8HTMLLabelElement::HasInstance(v8::Local<v8::Value> v8_value, v8::Isolate* isolate) {
  return V8PerIsolateData::From(isolate)->HasInstance(V8HTMLLabelElement::GetWrapperTypeInfo(), v8_value);
}

v8::Local<v8::Object> V8HTMLLabelElement::FindInstanceInPrototypeChain(
    v8::Local<v8::Value> v8_value, v8::Isolate* isolate) {
  return V8PerIsolateData::From(isolate)->FindInstanceInPrototypeChain(
      V8HTMLLabelElement::GetWrapperTypeInfo(), v8_value);
}

HTMLLabelElement* V8HTMLLabelElement::ToImplWithTypeCheck(
    v8::Isolate* isolate, v8::Local<v8::Value> value) {
  return HasInstance(value, isolate) ? ToImpl(v8::Local<v8::Object>::Cast(value)) : nullptr;
}

HTMLLabelElement* NativeValueTraits<HTMLLabelElement>::NativeValue(
    v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exception_state) {
  HTMLLabelElement* native_value = V8HTMLLabelElement::ToImplWithTypeCheck(isolate, value);
  if (!native_value) {
    exception_state.ThrowTypeError(ExceptionMessages::FailedToConvertJSValue(
        "HTMLLabelElement"));
  }
  return native_value;
}

}  // namespace blink
