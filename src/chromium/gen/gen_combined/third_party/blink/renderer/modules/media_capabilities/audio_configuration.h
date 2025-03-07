// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/dictionary_impl.h.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIA_CAPABILITIES_AUDIO_CONFIGURATION_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIA_CAPABILITIES_AUDIO_CONFIGURATION_H_

#include "third_party/blink/renderer/bindings/core/v8/idl_dictionary_base.h"
#include "third_party/blink/renderer/modules/modules_export.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace blink {

class MODULES_EXPORT AudioConfiguration : public IDLDictionaryBase {
 public:
  static AudioConfiguration* Create() { return MakeGarbageCollected<AudioConfiguration>(); }

  AudioConfiguration();
  virtual ~AudioConfiguration();

  bool hasBitrate() const { return has_bitrate_; }
  uint32_t bitrate() const {
    DCHECK(has_bitrate_);
    return bitrate_;
  }
  inline void setBitrate(uint32_t);

  bool hasChannels() const { return !channels_.IsNull(); }
  const String& channels() const {
    return channels_;
  }
  inline void setChannels(const String&);

  bool hasContentType() const { return !content_type_.IsNull(); }
  const String& contentType() const {
    return content_type_;
  }
  inline void setContentType(const String&);

  bool hasSamplerate() const { return has_samplerate_; }
  uint32_t samplerate() const {
    DCHECK(has_samplerate_);
    return samplerate_;
  }
  inline void setSamplerate(uint32_t);

  v8::Local<v8::Value> ToV8Impl(v8::Local<v8::Object>, v8::Isolate*) const override;
  void Trace(blink::Visitor*) override;

 private:
  bool has_bitrate_ = false;
  bool has_samplerate_ = false;

  uint32_t bitrate_;
  String channels_;
  String content_type_;
  uint32_t samplerate_;

  friend class V8AudioConfiguration;
};

void AudioConfiguration::setBitrate(uint32_t value) {
  bitrate_ = value;
  has_bitrate_ = true;
}

void AudioConfiguration::setChannels(const String& value) {
  channels_ = value;
}

void AudioConfiguration::setContentType(const String& value) {
  content_type_ = value;
}

void AudioConfiguration::setSamplerate(uint32_t value) {
  samplerate_ = value;
  has_samplerate_ = true;
}

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIA_CAPABILITIES_AUDIO_CONFIGURATION_H_
