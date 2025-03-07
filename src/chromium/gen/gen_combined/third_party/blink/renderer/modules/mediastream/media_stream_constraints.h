// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/dictionary_impl.h.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIASTREAM_MEDIA_STREAM_CONSTRAINTS_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIASTREAM_MEDIA_STREAM_CONSTRAINTS_H_

#include "third_party/blink/renderer/bindings/core/v8/idl_dictionary_base.h"
#include "third_party/blink/renderer/bindings/modules/v8/boolean_or_media_track_constraints.h"
#include "third_party/blink/renderer/modules/mediastream/media_track_constraints.h"
#include "third_party/blink/renderer/modules/modules_export.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class MODULES_EXPORT MediaStreamConstraints : public IDLDictionaryBase {
 public:
  static MediaStreamConstraints* Create() { return MakeGarbageCollected<MediaStreamConstraints>(); }

  MediaStreamConstraints();
  virtual ~MediaStreamConstraints();

  bool hasAudio() const { return !audio_.IsNull(); }
  const BooleanOrMediaTrackConstraints& audio() const {
    return audio_;
  }
  void setAudio(const BooleanOrMediaTrackConstraints&);

  bool hasVideo() const { return !video_.IsNull(); }
  const BooleanOrMediaTrackConstraints& video() const {
    return video_;
  }
  void setVideo(const BooleanOrMediaTrackConstraints&);

  v8::Local<v8::Value> ToV8Impl(v8::Local<v8::Object>, v8::Isolate*) const override;
  void Trace(blink::Visitor*) override;

 private:

  BooleanOrMediaTrackConstraints audio_;
  BooleanOrMediaTrackConstraints video_;

  friend class V8MediaStreamConstraints;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIASTREAM_MEDIA_STREAM_CONSTRAINTS_H_
