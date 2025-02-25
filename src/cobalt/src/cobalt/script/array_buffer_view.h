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

#ifndef COBALT_SCRIPT_ARRAY_BUFFER_VIEW_H_
#define COBALT_SCRIPT_ARRAY_BUFFER_VIEW_H_

#include "cobalt/script/array_buffer.h"
#include "cobalt/script/script_value.h"
#include "cobalt/script/global_environment.h"

namespace cobalt {
namespace script {

// https://heycam.github.io/webidl/#ArrayBufferView
class ArrayBufferView {
 public:
  // This constructor is only used for the purpose of creating an empty class
  // that has a valid ScriptValue*.
  static Handle<ArrayBufferView> New(GlobalEnvironment* global_environment,
                                     ArrayBufferView* copy_target);

  virtual ~ArrayBufferView() {}

  virtual Handle<ArrayBuffer> Buffer() const = 0;

  virtual size_t ByteOffset() const = 0;
  virtual size_t ByteLength() const = 0;
  virtual void* RawData() const = 0;
};

}  // namespace script
}  // namespace cobalt

#endif  // COBALT_SCRIPT_ARRAY_BUFFER_VIEW_H_
