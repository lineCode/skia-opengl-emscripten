// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/interface.cc.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#include "third_party/blink/renderer/bindings/core/v8/v8_csp_violation_report_body.h"

#include <algorithm>

#include "base/memory/scoped_refptr.h"
#include "third_party/blink/renderer/bindings/core/v8/idl_types.h"
#include "third_party/blink/renderer/bindings/core/v8/native_value_traits_impl.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_dom_configuration.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/core/inspector/console_message.h"
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
const WrapperTypeInfo v8_csp_violation_report_body_wrapper_type_info = {
    gin::kEmbedderBlink,
    V8CSPViolationReportBody::DomTemplate,
    nullptr,
    "CSPViolationReportBody",
    V8ReportBody::GetWrapperTypeInfo(),
    WrapperTypeInfo::kWrapperTypeObjectPrototype,
    WrapperTypeInfo::kObjectClassId,
    WrapperTypeInfo::kNotInheritFromActiveScriptWrappable,
};
#if defined(COMPONENT_BUILD) && defined(WIN32) && defined(__clang__)
#pragma clang diagnostic pop
#endif

// This static member must be declared by DEFINE_WRAPPERTYPEINFO in CSPViolationReportBody.h.
// For details, see the comment of DEFINE_WRAPPERTYPEINFO in
// platform/bindings/ScriptWrappable.h.
const WrapperTypeInfo& CSPViolationReportBody::wrapper_type_info_ = v8_csp_violation_report_body_wrapper_type_info;

// not [ActiveScriptWrappable]
static_assert(
    !std::is_base_of<ActiveScriptWrappableBase, CSPViolationReportBody>::value,
    "CSPViolationReportBody inherits from ActiveScriptWrappable<>, but is not specifying "
    "[ActiveScriptWrappable] extended attribute in the IDL file.  "
    "Be consistent.");
static_assert(
    std::is_same<decltype(&CSPViolationReportBody::HasPendingActivity),
                 decltype(&ScriptWrappable::HasPendingActivity)>::value,
    "CSPViolationReportBody is overriding hasPendingActivity(), but is not specifying "
    "[ActiveScriptWrappable] extended attribute in the IDL file.  "
    "Be consistent.");

namespace csp_violation_report_body_v8_internal {

static void DocumentURLAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  CSPViolationReportBody* impl = V8CSPViolationReportBody::ToImpl(holder);

  V8SetReturnValueString(info, impl->documentURL(), info.GetIsolate());
}

static void ReferrerAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  CSPViolationReportBody* impl = V8CSPViolationReportBody::ToImpl(holder);

  V8SetReturnValueStringOrNull(info, impl->referrer(), info.GetIsolate());
}

static void BlockedURLAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  CSPViolationReportBody* impl = V8CSPViolationReportBody::ToImpl(holder);

  V8SetReturnValueStringOrNull(info, impl->blockedURL(), info.GetIsolate());
}

static void EffectiveDirectiveAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  CSPViolationReportBody* impl = V8CSPViolationReportBody::ToImpl(holder);

  V8SetReturnValueString(info, impl->effectiveDirective(), info.GetIsolate());
}

static void OriginalPolicyAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  CSPViolationReportBody* impl = V8CSPViolationReportBody::ToImpl(holder);

  V8SetReturnValueString(info, impl->originalPolicy(), info.GetIsolate());
}

static void SourceFileAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  CSPViolationReportBody* impl = V8CSPViolationReportBody::ToImpl(holder);

  V8SetReturnValueStringOrNull(info, impl->sourceFile(), info.GetIsolate());
}

static void SampleAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  CSPViolationReportBody* impl = V8CSPViolationReportBody::ToImpl(holder);

  V8SetReturnValueStringOrNull(info, impl->sample(), info.GetIsolate());
}

static void DispositionAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  CSPViolationReportBody* impl = V8CSPViolationReportBody::ToImpl(holder);

  V8SetReturnValueString(info, impl->disposition(), info.GetIsolate());
}

static void StatusCodeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  CSPViolationReportBody* impl = V8CSPViolationReportBody::ToImpl(holder);

  V8SetReturnValueUnsigned(info, impl->statusCode());
}

static void LineNumberAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  CSPViolationReportBody* impl = V8CSPViolationReportBody::ToImpl(holder);

  bool is_null = false;

  uint32_t cpp_value(impl->lineNumber(is_null));

  if (is_null) {
    V8SetReturnValueNull(info);
    return;
  }

  V8SetReturnValueUnsigned(info, cpp_value);
}

static void ColumnNumberAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  CSPViolationReportBody* impl = V8CSPViolationReportBody::ToImpl(holder);

  bool is_null = false;

  uint32_t cpp_value(impl->columnNumber(is_null));

  if (is_null) {
    V8SetReturnValueNull(info);
    return;
  }

  V8SetReturnValueUnsigned(info, cpp_value);
}

}  // namespace csp_violation_report_body_v8_internal

void V8CSPViolationReportBody::DocumentURLAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_CSPViolationReportBody_documentURL_Getter");

  csp_violation_report_body_v8_internal::DocumentURLAttributeGetter(info);
}

void V8CSPViolationReportBody::ReferrerAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_CSPViolationReportBody_referrer_Getter");

  csp_violation_report_body_v8_internal::ReferrerAttributeGetter(info);
}

void V8CSPViolationReportBody::BlockedURLAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_CSPViolationReportBody_blockedURL_Getter");

  csp_violation_report_body_v8_internal::BlockedURLAttributeGetter(info);
}

void V8CSPViolationReportBody::EffectiveDirectiveAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_CSPViolationReportBody_effectiveDirective_Getter");

  csp_violation_report_body_v8_internal::EffectiveDirectiveAttributeGetter(info);
}

void V8CSPViolationReportBody::OriginalPolicyAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_CSPViolationReportBody_originalPolicy_Getter");

  csp_violation_report_body_v8_internal::OriginalPolicyAttributeGetter(info);
}

void V8CSPViolationReportBody::SourceFileAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_CSPViolationReportBody_sourceFile_Getter");

  csp_violation_report_body_v8_internal::SourceFileAttributeGetter(info);
}

void V8CSPViolationReportBody::SampleAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_CSPViolationReportBody_sample_Getter");

  csp_violation_report_body_v8_internal::SampleAttributeGetter(info);
}

void V8CSPViolationReportBody::DispositionAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_CSPViolationReportBody_disposition_Getter");

  csp_violation_report_body_v8_internal::DispositionAttributeGetter(info);
}

void V8CSPViolationReportBody::StatusCodeAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_CSPViolationReportBody_statusCode_Getter");

  csp_violation_report_body_v8_internal::StatusCodeAttributeGetter(info);
}

void V8CSPViolationReportBody::LineNumberAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_CSPViolationReportBody_lineNumber_Getter");

  csp_violation_report_body_v8_internal::LineNumberAttributeGetter(info);
}

void V8CSPViolationReportBody::ColumnNumberAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  RUNTIME_CALL_TIMER_SCOPE_DISABLED_BY_DEFAULT(info.GetIsolate(), "Blink_CSPViolationReportBody_columnNumber_Getter");

  csp_violation_report_body_v8_internal::ColumnNumberAttributeGetter(info);
}

static constexpr V8DOMConfiguration::AccessorConfiguration kV8CSPViolationReportBodyAccessors[] = {
    { "documentURL", V8CSPViolationReportBody::DocumentURLAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
    { "referrer", V8CSPViolationReportBody::ReferrerAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
    { "blockedURL", V8CSPViolationReportBody::BlockedURLAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
    { "effectiveDirective", V8CSPViolationReportBody::EffectiveDirectiveAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
    { "originalPolicy", V8CSPViolationReportBody::OriginalPolicyAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
    { "sourceFile", V8CSPViolationReportBody::SourceFileAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
    { "sample", V8CSPViolationReportBody::SampleAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
    { "disposition", V8CSPViolationReportBody::DispositionAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
    { "statusCode", V8CSPViolationReportBody::StatusCodeAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
    { "lineNumber", V8CSPViolationReportBody::LineNumberAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
    { "columnNumber", V8CSPViolationReportBody::ColumnNumberAttributeGetterCallback, nullptr, V8PrivateProperty::kNoCachedAccessor, static_cast<v8::PropertyAttribute>(v8::ReadOnly), V8DOMConfiguration::kOnPrototype, V8DOMConfiguration::kCheckHolder, V8DOMConfiguration::kHasSideEffect, V8DOMConfiguration::kAlwaysCallGetter, V8DOMConfiguration::kAllWorlds },
};

static void InstallV8CSPViolationReportBodyTemplate(
    v8::Isolate* isolate,
    const DOMWrapperWorld& world,
    v8::Local<v8::FunctionTemplate> interface_template) {
  // Initialize the interface object's template.
  V8DOMConfiguration::InitializeDOMInterfaceTemplate(isolate, interface_template, V8CSPViolationReportBody::GetWrapperTypeInfo()->interface_name, V8ReportBody::DomTemplate(isolate, world), V8CSPViolationReportBody::kInternalFieldCount);

  v8::Local<v8::Signature> signature = v8::Signature::New(isolate, interface_template);
  ALLOW_UNUSED_LOCAL(signature);
  v8::Local<v8::ObjectTemplate> instance_template = interface_template->InstanceTemplate();
  ALLOW_UNUSED_LOCAL(instance_template);
  v8::Local<v8::ObjectTemplate> prototype_template = interface_template->PrototypeTemplate();
  ALLOW_UNUSED_LOCAL(prototype_template);

  // Register IDL constants, attributes and operations.
  V8DOMConfiguration::InstallAccessors(
      isolate, world, instance_template, prototype_template, interface_template,
      signature, kV8CSPViolationReportBodyAccessors, base::size(kV8CSPViolationReportBodyAccessors));

  // Custom signature

  V8CSPViolationReportBody::InstallRuntimeEnabledFeaturesOnTemplate(
      isolate, world, interface_template);
}

void V8CSPViolationReportBody::InstallRuntimeEnabledFeaturesOnTemplate(
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

v8::Local<v8::FunctionTemplate> V8CSPViolationReportBody::DomTemplate(
    v8::Isolate* isolate, const DOMWrapperWorld& world) {
  return V8DOMConfiguration::DomClassTemplate(
      isolate, world, const_cast<WrapperTypeInfo*>(V8CSPViolationReportBody::GetWrapperTypeInfo()),
      InstallV8CSPViolationReportBodyTemplate);
}

bool V8CSPViolationReportBody::HasInstance(v8::Local<v8::Value> v8_value, v8::Isolate* isolate) {
  return V8PerIsolateData::From(isolate)->HasInstance(V8CSPViolationReportBody::GetWrapperTypeInfo(), v8_value);
}

v8::Local<v8::Object> V8CSPViolationReportBody::FindInstanceInPrototypeChain(
    v8::Local<v8::Value> v8_value, v8::Isolate* isolate) {
  return V8PerIsolateData::From(isolate)->FindInstanceInPrototypeChain(
      V8CSPViolationReportBody::GetWrapperTypeInfo(), v8_value);
}

CSPViolationReportBody* V8CSPViolationReportBody::ToImplWithTypeCheck(
    v8::Isolate* isolate, v8::Local<v8::Value> value) {
  return HasInstance(value, isolate) ? ToImpl(v8::Local<v8::Object>::Cast(value)) : nullptr;
}

CSPViolationReportBody* NativeValueTraits<CSPViolationReportBody>::NativeValue(
    v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exception_state) {
  CSPViolationReportBody* native_value = V8CSPViolationReportBody::ToImplWithTypeCheck(isolate, value);
  if (!native_value) {
    exception_state.ThrowTypeError(ExceptionMessages::FailedToConvertJSValue(
        "CSPViolationReportBody"));
  }
  return native_value;
}

}  // namespace blink
