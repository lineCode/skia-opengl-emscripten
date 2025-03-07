
// Copyright 2019 The Cobalt Authors. All Rights Reserved.
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

// clang-format off

// This file has been auto-generated by bindings/code_generator_cobalt.py. DO NOT MODIFY!
// Auto-generated from template: bindings/v8c/templates/callback-interface.h.template

#ifndef V8cOnErrorEventListener_h
#define V8cOnErrorEventListener_h

#include "cobalt/script/callback_interface_traits.h"
// Headers for other bindings wrapper classes
#include "cobalt/dom/on_error_event_listener.h"

#include "cobalt/script/v8c/scoped_persistent.h"


namespace cobalt {
namespace dom {

class V8cOnErrorEventListener : public script::v8c::ScopedPersistent<v8::Value>,
                          public OnErrorEventListener {
 public:
  typedef OnErrorEventListener BaseType;

  V8cOnErrorEventListener() {}
  V8cOnErrorEventListener(v8::Isolate* isolate, v8::Local<v8::Value> handle)
      : ScopedPersistent(isolate, handle), isolate_(isolate) {}

  base::Optional<bool > HandleEvent(
      const scoped_refptr<script::Wrappable>& callback_this,
      ::cobalt::script::UnionType2<scoped_refptr<Event>, std::string > event,
      const std::string& filename,
      uint32_t lineno,
      uint32_t colno,
      const ::cobalt::script::ScriptValue<::cobalt::script::ValueHandle>* error,
      bool* had_exception) const override;

 private:
  v8::Isolate* isolate_;
};

}  // namespace dom
}  // namespace cobalt

namespace cobalt {
namespace script {

// Explicit instantiation of CallbackInterfaceTraits struct so we can infer
// the type of the generated class from the type of the callback interface.
template<>
struct CallbackInterfaceTraits<cobalt::dom::OnErrorEventListener> {
  typedef cobalt::dom::V8cOnErrorEventListener V8cCallbackInterfaceClass;
};

}  // namespace script
}  // namespace cobalt


#endif  // V8cOnErrorEventListener_h
