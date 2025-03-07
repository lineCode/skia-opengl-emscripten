// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/interface.cc.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#include "third_party/blink/renderer/bindings/modules/v8/v8_federated_credential.h"

#include <algorithm>

#include "base/memory/scoped_refptr.h"
#include "third_party/blink/renderer/bindings/core/v8/idl_types.h"
#include "third_party/blink/renderer/bindings/core/v8/native_value_traits_impl.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_dom_configuration.h"
#include "third_party/blink/renderer/bindings/modules/v8/v8_federated_credential_init.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
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
const WrapperTypeInfo v8_federated_credential_wrapper_type_info = {
    gin::kEmbedderBlink,
    V8FederatedCredential::DomTemplate,
    V8FederatedCredential::InstallConditionalFeatures,
    "FederatedCredential",
    V8Credential::GetWrapperTypeInfo(),
    WrapperTypeInfo::kWrapperTypeObjectPrototype,
    WrapperTypeInfo::kObjectClassId,
    WrapperTypeInfo::kNotInheritFromActiveScriptWrappable,
};
#if defined(COMPONENT_BUILD) && defined(WIN32) && defined(__clang__)
#pragma clang diagnostic pop
#endif

// This static member must be declared by DEFINE_WRAPPERTYPEINFO in FederatedCredential.h.
// For details, see the comment of DEFINE_WRAPPERTYPEINFO in
// platform/bindings/ScriptWrappable.h.
const WrapperTypeInfo& FederatedCredential::wrapper_type_info_ = v8_federated_credential_wrapper_type_info;

// not [ActiveScriptWrappable]
static_assert(
    !std::is_base_of<ActiveScriptWrappableBase, FederatedCredential>::value,
    "FederatedCredential inherits from ActiveScriptWrappable<>, but is not specifying "
    "[ActiveScriptWrappable] extended attribute in the IDL file.  "
    "Be consistent.");
static_assert(
    std::is_same<decltype(&FederatedCredential::HasPendingActivity),
                 decltype(&ScriptWrappable::HasPendingActivity)>::value,
    "FederatedCredential is overriding hasPendingActivity(), but is not specifying "
    "[ActiveScriptWrappable] extended attribute in the IDL file.  "
    "Be consistent.");

namespace federated_credential_v8_internal {

static void ProviderAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  FederatedCredential* impl = V8FederatedCredential::ToImpl(holder);

  V8SetReturnValueString(info, impl->provider(), info.GetIsolate());
}

static void ProtocolAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  FederatedCredential* impl = V8FederatedCredential::ToImpl(holder);

  V8SetReturnValueStringOrNull(info, impl->protocol(), info.GetIsolate());
}

static void NameAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  FederatedCredential* impl = V8FederatedCredential::ToImpl(holder);

  V8SetReturnValueString(info, impl->name(), info.GetIsolate());
}

static void IconURLAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  FederatedCredential* impl = V8FederatedCredential::ToImpl(holder);

  V8SetReturnValueString(info, impl->iconURL(), info.GetIsolate());
}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_FederatedCredential_ConstructorCallback");

  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kConstructionContext, "FederatedCredential");

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  FederatedCredentialInit* data;
  if (!info[0]->IsNullOrUndefined() && !info[0]->IsObject()) {
    exception_state.ThrowTypeError("parameter 1 ('data') is not an object.");
    return;
  }
  data = NativeValueTraits<FederatedCredentialInit>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  FederatedCredential* impl = FederatedCredential::Create(data, exception_state);
  if (exception_state.HadException()) {
    return;
  }
  v8::Local<v8::Object> wrapper = info.Holder();
  wrapper = impl->AssociateWithWrapper(info.GetIsolate(), V8FederatedCredential::GetWrapperTypeInfo(), wrapper);
  V8SetReturnValue(info, wrapper);
}

MODULES_EXPORT void ConstructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_FederatedCredential_Constructor");

  if (!info.IsConstructCall()) {
    V8ThrowException::ThrowTypeError(
        info.GetIsolate(),
        ExceptionMessages::ConstructorNotCallableAsFunction("FederatedCredential"));
    return;
  }

  if (ConstructorMode::Current(info.GetIsolate()) == ConstructorMode::kWrapExistingObject) {
    V8SetReturnValue(info, info.Holder());
    return;
  }

  federated_credential_v8_internal::Constructor(info);
}

}  // namespace federated_credential_v8_internal

void V8FederatedCredential::ProviderAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_FederatedCredential_provider_Getter");

  federated_credential_v8_internal::ProviderAttributeGetter(info);
}

void V8FederatedCredential::ProtocolAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_FederatedCredential_protocol_Getter");

  federated_credential_v8_internal::ProtocolAttributeGetter(info);
}

void V8FederatedCredential::NameAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_FederatedCredential_name_Getter");

  federated_credential_v8_internal::NameAttributeGetter(info);
}

void V8FederatedCredential::IconURLAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_FederatedCredential_iconURL_Getter");

  federated_credential_v8_internal::IconURLAttributeGetter(info);
}

static void InstallV8FederatedCredentialTemplate(
    v8::Isolate* isolate,
    const DOMWrapperWorld& world,
    v8::Local<v8::FunctionTemplate> interface_template) {
  // Initialize the interface object's template.
  V8DOMConfiguration::InitializeDOMInterfaceTemplate(isolate, interface_template, V8FederatedCredential::GetWrapperTypeInfo()->interface_name, V8Credential::DomTemplate(isolate, world), V8FederatedCredential::kInternalFieldCount);
  interface_template->SetCallHandler(federated_credential_v8_internal::ConstructorCallback);
  interface_template->SetLength(1);

  v8::Local<v8::Signature> signature = v8::Signature::New(isolate, interface_template);
  ALLOW_UNUSED_LOCAL(signature);
  v8::Local<v8::ObjectTemplate> instance_template = interface_template->InstanceTemplate();
  ALLOW_UNUSED_LOCAL(instance_template);
  v8::Local<v8::ObjectTemplate> prototype_template = interface_template->PrototypeTemplate();
  ALLOW_UNUSED_LOCAL(prototype_template);

  // Register IDL constants, attributes and operations.

  // Custom signature

  V8FederatedCredential::InstallRuntimeEnabledFeaturesOnTemplate(
      isolate, world, interface_template);
}

void V8FederatedCredential::InstallRuntimeEnabledFeaturesOnTemplate(
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

v8::Local<v8::FunctionTemplate> V8FederatedCredential::DomTemplate(
    v8::Isolate* isolate, const DOMWrapperWorld& world) {
  return V8DOMConfiguration::DomClassTemplate(
      isolate, world, const_cast<WrapperTypeInfo*>(V8FederatedCredential::GetWrapperTypeInfo()),
      InstallV8FederatedCredentialTemplate);
}

bool V8FederatedCredential::HasInstance(v8::Local<v8::Value> v8_value, v8::Isolate* isolate) {
  return V8PerIsolateData::From(isolate)->HasInstance(V8FederatedCredential::GetWrapperTypeInfo(), v8_value);
}

v8::Local<v8::Object> V8FederatedCredential::FindInstanceInPrototypeChain(
    v8::Local<v8::Value> v8_value, v8::Isolate* isolate) {
  return V8PerIsolateData::From(isolate)->FindInstanceInPrototypeChain(
      V8FederatedCredential::GetWrapperTypeInfo(), v8_value);
}

FederatedCredential* V8FederatedCredential::ToImplWithTypeCheck(
    v8::Isolate* isolate, v8::Local<v8::Value> value) {
  return HasInstance(value, isolate) ? ToImpl(v8::Local<v8::Object>::Cast(value)) : nullptr;
}

FederatedCredential* NativeValueTraits<FederatedCredential>::NativeValue(
    v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exception_state) {
  FederatedCredential* native_value = V8FederatedCredential::ToImplWithTypeCheck(isolate, value);
  if (!native_value) {
    exception_state.ThrowTypeError(ExceptionMessages::FailedToConvertJSValue(
        "FederatedCredential"));
  }
  return native_value;
}

void V8FederatedCredential::InstallConditionalFeatures(
    v8::Local<v8::Context> context,
    const DOMWrapperWorld& world,
    v8::Local<v8::Object> instance_object,
    v8::Local<v8::Object> prototype_object,
    v8::Local<v8::Function> interface_object,
    v8::Local<v8::FunctionTemplate> interface_template) {
  CHECK(!interface_template.IsEmpty());
  DCHECK((!prototype_object.IsEmpty() && !interface_object.IsEmpty()) ||
         !instance_object.IsEmpty());

  v8::Isolate* isolate = context->GetIsolate();

  v8::Local<v8::Signature> signature = v8::Signature::New(isolate, interface_template);
  ExecutionContext* execution_context = ToExecutionContext(context);
  DCHECK(execution_context);
  bool is_secure_context = (execution_context && execution_context->IsSecureContext());

  if (!prototype_object.IsEmpty() || !interface_object.IsEmpty()) {
    if (is_secure_context) {
      static const V8DOMConfiguration::AccessorConfiguration accessor_configurations[] = {
          { "iconURL", V8FederatedCredential::IconURLAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
          { "name", V8FederatedCredential::NameAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
          { "protocol", V8FederatedCredential::ProtocolAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
          { "provider", V8FederatedCredential::ProviderAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
      };
      V8DOMConfiguration::InstallAccessors(
          isolate, world, instance_object, prototype_object, interface_object,
          signature, accessor_configurations,
          base::size(accessor_configurations));
    }
  }
}

}  // namespace blink
