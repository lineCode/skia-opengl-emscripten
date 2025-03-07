// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_DEVICE_PUBLIC_MOJOM_BATTERY_STATUS_MOJOM_FORWARD_H_
#define SERVICES_DEVICE_PUBLIC_MOJOM_BATTERY_STATUS_MOJOM_FORWARD_H_

#include "mojo/public/cpp/bindings/struct_ptr.h"


#include "mojo/public/cpp/bindings/lib/buffer.h"


#include "mojo/public/cpp/bindings/lib/native_enum_serialization.h"
#include "mojo/public/cpp/bindings/lib/native_struct_serialization.h"




namespace device {
namespace mojom {
class BatteryStatusDataView;

class BatteryStatus;
using BatteryStatusPtr = mojo::InlinedStructPtr<BatteryStatus>;




}  // namespace mojom
}  // namespace device

#endif  // SERVICES_DEVICE_PUBLIC_MOJOM_BATTERY_STATUS_MOJOM_FORWARD_H_