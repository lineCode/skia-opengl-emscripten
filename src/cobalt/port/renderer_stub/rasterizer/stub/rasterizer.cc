﻿// Copyright 2016 The Cobalt Authors. All Rights Reserved.
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

#include "renderer_stub/rasterizer/stub/rasterizer.h"

namespace cobalt {
namespace renderer {
namespace rasterizer {
namespace stub {

Rasterizer::Rasterizer() {}

render_tree::ResourceProvider* Rasterizer::GetResourceProvider() {
  return &resource_provider_stub_;
}

}  // namespace stub
}  // namespace rasterizer
}  // namespace renderer
}  // namespace cobalt
