/*
 * Copyright 2017 The Cobalt Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cobalt/dom/device_orientation_event.h"

//#include "cobalt/base/token.h"
#include "cobalt/base/cobalt_token.h"

#include "cobalt/base/tokens.h"
#include "cobalt/dom/storage.h"

namespace cobalt {
namespace dom {

DeviceOrientationEvent::DeviceOrientationEvent()
    : Event(base::Tokens::deviceorientation()) {}

DeviceOrientationEvent::DeviceOrientationEvent(const std::string& type)
    : Event(type) {}

DeviceOrientationEvent::DeviceOrientationEvent(
    const std::string& type, const DeviceOrientationEventInit& init)
    : Event(type),
      alpha_(init.alpha()),
      beta_(init.beta()),
      gamma_(init.gamma()),
      absolute_(init.absolute()) {}

}  // namespace dom
}  // namespace cobalt
