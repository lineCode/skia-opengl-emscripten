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
// Auto-generated from template: bindings/templates/dictionary.h.template

#ifndef MediaKeySystemConfiguration_h
#define MediaKeySystemConfiguration_h

#include <string>

#include "base/optional.h"
#include "cobalt/script/array_buffer.h"
#include "cobalt/script/array_buffer_view.h"
#include "cobalt/script/script_value.h"
#include "cobalt/script/sequence.h"
#include "cobalt/script/typed_arrays.h"
#include "cobalt/script/value_handle.h"
#include "cobalt/dom/eme/media_key_system_media_capability.h"

using cobalt::dom::eme::MediaKeySystemMediaCapability;

namespace cobalt {
namespace dom {
namespace eme {

class MediaKeySystemConfiguration {
 public:
  MediaKeySystemConfiguration() {
    label_ = "";
    has_init_data_types_ = false;
    init_data_types_ = ::cobalt::script::Sequence< std::string >();
    has_audio_capabilities_ = false;
    audio_capabilities_ = ::cobalt::script::Sequence< MediaKeySystemMediaCapability >();
    has_video_capabilities_ = false;
    video_capabilities_ = ::cobalt::script::Sequence< MediaKeySystemMediaCapability >();
  }

  MediaKeySystemConfiguration(const MediaKeySystemConfiguration& other) {
    label_ = other.label_;
    has_init_data_types_ = other.has_init_data_types_;
    init_data_types_ = other.init_data_types_;
    has_audio_capabilities_ = other.has_audio_capabilities_;
    audio_capabilities_ = other.audio_capabilities_;
    has_video_capabilities_ = other.has_video_capabilities_;
    video_capabilities_ = other.video_capabilities_;
  }

  MediaKeySystemConfiguration& operator=(const MediaKeySystemConfiguration& other) {
    label_ = other.label_;
    has_init_data_types_ = other.has_init_data_types_;
    init_data_types_ = other.init_data_types_;
    has_audio_capabilities_ = other.has_audio_capabilities_;
    audio_capabilities_ = other.audio_capabilities_;
    has_video_capabilities_ = other.has_video_capabilities_;
    video_capabilities_ = other.video_capabilities_;
    return *this;
  }

  bool has_label() const {
    return true;
  }
  const std::string& label() const {
    return label_;
  }
  void set_label(const std::string& value) {
    label_ = value;
  }

  bool has_init_data_types() const {
    return has_init_data_types_;
  }
  ::cobalt::script::Sequence< std::string > init_data_types() const {
    DCHECK(has_init_data_types_);
    return init_data_types_;
  }
  void set_init_data_types(::cobalt::script::Sequence< std::string > value) {
    has_init_data_types_ = true;
    init_data_types_ = value;
  }

  bool has_audio_capabilities() const {
    return has_audio_capabilities_;
  }
  ::cobalt::script::Sequence< MediaKeySystemMediaCapability > audio_capabilities() const {
    DCHECK(has_audio_capabilities_);
    return audio_capabilities_;
  }
  void set_audio_capabilities(::cobalt::script::Sequence< MediaKeySystemMediaCapability > value) {
    has_audio_capabilities_ = true;
    audio_capabilities_ = value;
  }

  bool has_video_capabilities() const {
    return has_video_capabilities_;
  }
  ::cobalt::script::Sequence< MediaKeySystemMediaCapability > video_capabilities() const {
    DCHECK(has_video_capabilities_);
    return video_capabilities_;
  }
  void set_video_capabilities(::cobalt::script::Sequence< MediaKeySystemMediaCapability > value) {
    has_video_capabilities_ = true;
    video_capabilities_ = value;
  }


 private:
  std::string label_;
  bool has_init_data_types_;
  ::cobalt::script::Sequence< std::string > init_data_types_;
  bool has_audio_capabilities_;
  ::cobalt::script::Sequence< MediaKeySystemMediaCapability > audio_capabilities_;
  bool has_video_capabilities_;
  ::cobalt::script::Sequence< MediaKeySystemMediaCapability > video_capabilities_;
};

// This ostream override is necessary for MOCK_METHODs commonly used
// in idl test code
inline std::ostream& operator<<(
    std::ostream& stream, const cobalt::dom::eme::MediaKeySystemConfiguration& in) {
  SB_UNREFERENCED_PARAMETER(in);
  stream << "[MediaKeySystemConfiguration]";
  return stream;
}

}  // namespace cobalt
}  // namespace dom
}  // namespace eme

#endif  // MediaKeySystemConfiguration_h
