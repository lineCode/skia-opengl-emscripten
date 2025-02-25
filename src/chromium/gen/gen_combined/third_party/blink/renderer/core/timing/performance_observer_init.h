// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/dictionary_impl.h.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_TIMING_PERFORMANCE_OBSERVER_INIT_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_TIMING_PERFORMANCE_OBSERVER_INIT_H_

#include "third_party/blink/renderer/bindings/core/v8/idl_dictionary_base.h"
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"
#include "third_party/blink/renderer/platform/wtf/vector.h"

namespace blink {

class CORE_EXPORT PerformanceObserverInit : public IDLDictionaryBase {
 public:
  static PerformanceObserverInit* Create() { return MakeGarbageCollected<PerformanceObserverInit>(); }

  PerformanceObserverInit();
  virtual ~PerformanceObserverInit();

  bool hasBuffered() const { return has_buffered_; }
  bool buffered() const {
    DCHECK(has_buffered_);
    return buffered_;
  }
  inline void setBuffered(bool);

  bool hasEntryTypes() const { return has_entry_types_; }
  const Vector<String>& entryTypes() const {
    DCHECK(has_entry_types_);
    return entry_types_;
  }
  void setEntryTypes(const Vector<String>&);

  bool hasType() const { return !type_.IsNull(); }
  const String& type() const {
    return type_;
  }
  inline void setType(const String&);

  v8::Local<v8::Value> ToV8Impl(v8::Local<v8::Object>, v8::Isolate*) const override;
  void Trace(blink::Visitor*) override;

 private:
  bool has_buffered_ = false;
  bool has_entry_types_ = false;

  bool buffered_;
  Vector<String> entry_types_;
  String type_;

  friend class V8PerformanceObserverInit;
};

void PerformanceObserverInit::setBuffered(bool value) {
  buffered_ = value;
  has_buffered_ = true;
}

void PerformanceObserverInit::setType(const String& value) {
  type_ = value;
}

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_TIMING_PERFORMANCE_OBSERVER_INIT_H_
