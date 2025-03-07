// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated from the Jinja2 template
// third_party/blink/renderer/bindings/templates/dictionary_impl.cc.tmpl
// by the script code_generator_v8.py.
// DO NOT MODIFY!

// clang-format off
#include "third_party/blink/renderer/modules/payments/android_pay_method_data.h"

namespace blink {

AndroidPayMethodData::AndroidPayMethodData() {
}

AndroidPayMethodData::~AndroidPayMethodData() = default;

void AndroidPayMethodData::setAllowedCardNetworks(const Vector<String>& value) {
  allowed_card_networks_ = value;
  has_allowed_card_networks_ = true;
}

void AndroidPayMethodData::setPaymentMethodTokenizationParameters(AndroidPayTokenization* value) {
  payment_method_tokenization_parameters_ = value;
}

void AndroidPayMethodData::Trace(blink::Visitor* visitor) {
  visitor->Trace(payment_method_tokenization_parameters_);
  IDLDictionaryBase::Trace(visitor);
}

}  // namespace blink
