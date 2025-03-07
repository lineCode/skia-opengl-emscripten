// Copyright 2018 The Cobalt Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cobalt/script/v8c/v8c_script_debugger.h"

#include <memory>
#include <sstream>
#include <string>

#include "base/logging.h"
#include "base/trace_event/trace_event.h"
#include "cobalt/base/polymorphic_downcast.h"
#include "cobalt/script/v8c/conversion_helpers.h"
#include "cobalt/script/v8c/v8c_tracing_controller.h"
#include "include/inspector/Runtime.h"  // generated
#include "nb/memory_scope.h"
#include "v8/include/libplatform/v8-tracing.h"
#include "v8/include/v8-inspector.h"

namespace cobalt {
namespace script {
namespace v8c {

namespace {
constexpr const char* kInspectorDomains[] = {
    "Runtime",
    "Debugger",
    "Profiler",
};

constexpr int kContextGroupId = 1;
constexpr char kContextName[] = "Cobalt";

V8cTracingController* GetTracingController() {
  return base::polymorphic_downcast<V8cTracingController*>(
      IsolateFellowship::GetInstance()->platform->GetTracingController());
}

// Inspired by |CopyCharsUnsigned| in v8/src/utils.h
std::string FromStringView(const v8_inspector::StringView& string_view) {
  std::string string;
  if (string_view.is8Bit()) {
    string.assign(reinterpret_cast<const char*>(string_view.characters8()),
                  string_view.length());
  } else {
    string.reserve(string_view.length());
    const uint16_t* chars =
        reinterpret_cast<const uint16_t*>(string_view.characters16());
    for (int i = 0; i < string_view.length(); i++) {
      string += chars[i];
    }
  }
  return string;
}

v8_inspector::StringView ToStringView(const std::string& string) {
  return v8_inspector::StringView(
      reinterpret_cast<const uint8_t*>(string.c_str()), string.length());
}

}  // namespace

V8cScriptDebugger::V8cScriptDebugger(
    V8cGlobalEnvironment* v8c_global_environment, Delegate* delegate)
    : global_environment_(v8c_global_environment),
      delegate_(delegate),
      supported_domains_(std::begin(kInspectorDomains),
                         std::end(kInspectorDomains)),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          inspector_(v8_inspector::V8Inspector::create(
              global_environment_->isolate(), /* client */ this))),
      pause_on_exception_state_(kAll) {
  // Register our one-and-only context with the inspector.
  v8::Isolate* isolate = global_environment_->isolate();
  EntryScope entry_scope(isolate);
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  inspector_->contextCreated(v8_inspector::V8ContextInfo(
      context, kContextGroupId,
      v8_inspector::StringView(reinterpret_cast<const uint8_t*>(kContextName),
                               sizeof(kContextName) - 1)));
}

V8cScriptDebugger::~V8cScriptDebugger() {}

void V8cScriptDebugger::Attach(const std::string& state) {
  DCHECK(!inspector_session_);
  inspector_session_ =
      inspector_->connect(kContextGroupId, this, ToStringView(state));
}

std::string V8cScriptDebugger::Detach() {
  // First inform the inspector that the context is destroyed. This will send
  // an event to inform the DevTools frontend just before we kill the session.
  v8::Isolate* isolate = global_environment_->isolate();
  EntryScope entry_scope(isolate);
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  inspector_->contextDestroyed(context);

  // After the frontend knows the context is destroyed we can kill the session.
  // If/when we re-atttach we'll connect a new session and the V8 inspector will
  // then inform the frontend about the sources, etc. in the new context.
  DCHECK(inspector_session_);
  std::unique_ptr<v8_inspector::StringBuffer> state =
      inspector_session_->stateJSON();
  inspector_session_.reset();
  return FromStringView(state->string());
}

bool V8cScriptDebugger::EvaluateDebuggerScript(const std::string& js_code,
                                               std::string* out_result_utf8) {
  TRACK_MEMORY_SCOPE("Javascript");
  TRACE_EVENT0("cobalt::script", "V8cScriptDebugger::EvaluateDebuggerScript()");

  v8::Isolate* isolate = global_environment_->isolate();
  EntryScope entry_scope(isolate);
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::TryCatch try_catch(isolate);
  v8::MicrotasksScope microtasksScope(isolate,
                                      v8::MicrotasksScope::kDoNotRunMicrotasks);

  v8::Local<v8::String> source;
  if (!v8::String::NewFromUtf8(isolate, js_code.c_str(),
                               v8::NewStringType::kNormal, js_code.length())
           .ToLocal(&source)) {
    LOG(ERROR) << "Failed to convert source code to V8 UTF-8 string.";
    return false;
  }

  v8::Local<v8::Value> result;
  if (!inspector_->compileAndRunInternalScript(context, source)
           .ToLocal(&result)) {
    v8::String::Utf8Value exception(try_catch.Exception());
    std::string string(*exception, exception.length());
    if (string.empty()) string.assign("Unknown error");
    LOG(ERROR) << "Debugger script error: " << string;
    if (out_result_utf8) {
      out_result_utf8->assign(std::move(string));
    }
    return false;
  }

  if (out_result_utf8) {
    V8cExceptionState exception_state(isolate);
    FromJSValue(isolate, result, kNoConversionFlags, &exception_state,
                out_result_utf8);
  }

  return true;
}

ScriptDebugger::PauseOnExceptionsState V8cScriptDebugger::SetPauseOnExceptions(
    ScriptDebugger::PauseOnExceptionsState state) {
  DCHECK(inspector_session_);
  auto previous_state = pause_on_exception_state_;
  pause_on_exception_state_ = state;
  inspector_session_->setSkipAllPauses(state == kNone);
  return previous_state;
}

bool V8cScriptDebugger::DispatchProtocolMessage(const std::string& method,
                                                const std::string& message) {
  DCHECK(inspector_session_);
  if (!inspector_session_->canDispatchMethod(ToStringView(method))) {
    return false;
  }
  inspector_session_->dispatchProtocolMessage(v8_inspector::StringView(
      reinterpret_cast<const uint8_t*>(message.c_str()), message.length()));
  return true;
}

std::string V8cScriptDebugger::CreateRemoteObject(
    const ValueHandleHolder& object, const std::string& group) {
  DCHECK(inspector_session_);
  const V8cValueHandleHolder* v8_value_handle_holder =
      base::polymorphic_downcast<const V8cValueHandleHolder*>(&object);

  v8::Isolate* isolate = v8_value_handle_holder->isolate();
  EntryScope entry_scope(isolate);
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Value> v8_value = v8_value_handle_holder->v8_value();

  std::unique_ptr<v8_inspector::protocol::Runtime::API::RemoteObject>
      remote_object = inspector_session_->wrapObject(
          context, v8_value, ToStringView(group), false /*generatePreview*/);
  return FromStringView(remote_object->toJSONString()->string());
}

const script::ValueHandleHolder* V8cScriptDebugger::LookupRemoteObjectId(
    const std::string& object_id) {
  DCHECK(inspector_session_);

  v8::Isolate* isolate = global_environment_->isolate();
  EntryScope entry_scope(isolate);
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  V8cExceptionState exception_state(isolate);

  std::unique_ptr<v8_inspector::StringBuffer> error;
  v8::Local<v8::Value> v8_value;
  if (!inspector_session_->unwrapObject(&error, ToStringView(object_id),
                                        &v8_value, &context,
                                        nullptr /*objectGroup*/)) {
    std::string err_string = FromStringView(error->string());
    exception_state.SetSimpleException(kRangeError, "%s", err_string.c_str());
    return nullptr;
  }

  std::unique_ptr<V8cValueHandleHolder> holder(new V8cValueHandleHolder());
  FromJSValue(isolate, v8_value, 0 /*conversion_flags*/, &exception_state,
              holder.get());

  V8cValueHandleHolder* retval = holder.get();
  // Keep the scoped_ptr alive in a no-op task so the holder stays valid until
  // the bindings code gets the v8::Value out of it through the raw pointer.
  base::MessageLoop::current()->task_runner()->PostTask(
      FROM_HERE, base::Bind([](std::unique_ptr<V8cValueHandleHolder>) {},
                            base::Passed(&holder)));
  return retval;
}

void V8cScriptDebugger::StartTracing(const std::vector<std::string>& categories,
                                     TraceDelegate* trace_delegate) {
  V8cTracingController* tracing_controller = GetTracingController();
  CHECK(tracing_controller);
  tracing_controller->StartTracing(categories, trace_delegate);
}

void V8cScriptDebugger::StopTracing() {
  V8cTracingController* tracing_controller = GetTracingController();
  CHECK(tracing_controller);
  tracing_controller->StopTracing();
}

// v8_inspector::V8InspectorClient implementation.
void V8cScriptDebugger::runMessageLoopOnPause(int contextGroupId) {
  DCHECK(contextGroupId == kContextGroupId);
  delegate_->OnScriptDebuggerPause();
}

// v8_inspector::V8InspectorClient implementation.
void V8cScriptDebugger::quitMessageLoopOnPause() {
  delegate_->OnScriptDebuggerResume();
}

// v8_inspector::V8InspectorClient implementation.
void V8cScriptDebugger::runIfWaitingForDebugger(int contextGroupId) {
  delegate_->OnScriptDebuggerResume();
}

// v8_inspector::V8InspectorClient implementation.
void V8cScriptDebugger::consoleAPIMessage(
    int contextGroupId, v8::Isolate::MessageErrorLevel level,
    const v8_inspector::StringView& message,
    const v8_inspector::StringView& url, unsigned lineNumber,
    unsigned columnNumber, v8_inspector::V8StackTrace* trace) {
  SB_UNREFERENCED_PARAMETER(contextGroupId);
  SB_UNREFERENCED_PARAMETER(trace);

  std::stringstream log;
  if (url.length()) {
    log << '[' << FromStringView(url) << ", Line " << lineNumber << ", Col "
        << columnNumber << ']';
  }
  log << FromStringView(message);

  switch (level) {
    case v8::Isolate::kMessageError:
      LOG(ERROR) << log.str();
      break;
    case v8::Isolate::kMessageWarning:
      LOG(WARNING) << log.str();
      break;
    default:
      LOG(INFO) << log.str();
  }
}

// v8_inspector::V8InspectorClient implementation.
v8::Local<v8::Context> V8cScriptDebugger::ensureDefaultContextInGroup(
    int contextGroupId) {
  v8::Isolate* isolate = global_environment_->isolate();
  EntryScope entry_scope(isolate);
  if (contextGroupId == kContextGroupId) {
    return isolate->GetCurrentContext();
  }
  DLOG(WARNING) << "No default context for group " << contextGroupId;
  return v8::Local<v8::Context>();
}

// v8_inspector::V8Inspector::Channel implementation.
void V8cScriptDebugger::sendResponse(
    int callId, std::unique_ptr<v8_inspector::StringBuffer> message) {
  std::string response = FromStringView(message->string());
  delegate_->OnScriptDebuggerResponse(response);
}

// v8_inspector::V8Inspector::Channel implementation.
void V8cScriptDebugger::sendNotification(
    std::unique_ptr<v8_inspector::StringBuffer> message) {
  std::string event = FromStringView(message->string());
  delegate_->OnScriptDebuggerEvent(event);
}

}  // namespace v8c

// Static factory method declared in public interface.
std::unique_ptr<ScriptDebugger> ScriptDebugger::CreateDebugger(
    GlobalEnvironment* global_environment, Delegate* delegate) {
  auto* v8c_global_environment =
      base::polymorphic_downcast<v8c::V8cGlobalEnvironment*>(
          global_environment);
  return std::unique_ptr<ScriptDebugger>(
      new v8c::V8cScriptDebugger(v8c_global_environment, delegate));
}

}  // namespace script
}  // namespace cobalt
