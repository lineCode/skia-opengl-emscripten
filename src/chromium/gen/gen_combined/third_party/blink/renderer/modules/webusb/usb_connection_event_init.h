// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/dictionary_impl.h.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_WEBUSB_USB_CONNECTION_EVENT_INIT_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_WEBUSB_USB_CONNECTION_EVENT_INIT_H_

#include "third_party/blink/renderer/core/dom/events/event_init.h"
#include "third_party/blink/renderer/modules/modules_export.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class USBDevice;

class MODULES_EXPORT USBConnectionEventInit : public EventInit {
 public:
  static USBConnectionEventInit* Create() { return MakeGarbageCollected<USBConnectionEventInit>(); }

  USBConnectionEventInit();
  virtual ~USBConnectionEventInit();

  bool hasDevice() const { return device_; }
  USBDevice* device() const {
    return device_;
  }
  inline void setDevice(USBDevice*);

  v8::Local<v8::Value> ToV8Impl(v8::Local<v8::Object>, v8::Isolate*) const override;
  void Trace(blink::Visitor*) override;

 private:

  Member<USBDevice> device_;

  friend class V8USBConnectionEventInit;
};

void USBConnectionEventInit::setDevice(USBDevice* value) {
  device_ = value;
}

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_WEBUSB_USB_CONNECTION_EVENT_INIT_H_
