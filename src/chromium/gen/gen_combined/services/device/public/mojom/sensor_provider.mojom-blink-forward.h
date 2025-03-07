// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_DEVICE_PUBLIC_MOJOM_SENSOR_PROVIDER_MOJOM_BLINK_FORWARD_H_
#define SERVICES_DEVICE_PUBLIC_MOJOM_SENSOR_PROVIDER_MOJOM_BLINK_FORWARD_H_

#include "mojo/public/cpp/bindings/struct_ptr.h"

#include "mojo/public/cpp/bindings/associated_interface_ptr.h"
#include "mojo/public/cpp/bindings/associated_interface_ptr_info.h"
#include "mojo/public/cpp/bindings/associated_interface_request.h"
#include "mojo/public/cpp/bindings/interface_ptr.h"
#include "mojo/public/cpp/bindings/interface_request.h"
#include "mojo/public/cpp/bindings/lib/control_message_handler.h"
#include "mojo/public/cpp/bindings/pending_associated_receiver.h"
#include "mojo/public/cpp/bindings/pending_associated_remote.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/raw_ptr_impl_ref_traits.h"
#include "mojo/public/cpp/bindings/thread_safe_interface_ptr.h"


#include "mojo/public/cpp/bindings/lib/native_enum_serialization.h"
#include "mojo/public/cpp/bindings/lib/native_struct_serialization.h"
#include "third_party/blink/public/platform/web_common.h"


namespace device {
namespace mojom {

enum class SensorCreationResult : int32_t;
}  // namespace device
}  // namespace mojom
#include "third_party/blink/renderer/platform/wtf/hash_functions.h"

namespace WTF {
struct device_mojom_internal_SensorCreationResult_DataHashFn;

template <>
struct DefaultHash<::device::mojom::SensorCreationResult> {
  using Hash = device_mojom_internal_SensorCreationResult_DataHashFn;
};
}  // namespace WTF


namespace device {
namespace mojom {
namespace blink {
using SensorCreationResult = SensorCreationResult;  // Alias for definition in the parent namespace.
class SensorInitParams;
using SensorInitParamsPtr = mojo::StructPtr<SensorInitParams>;

class SensorProvider;
using SensorProviderPtr = mojo::InterfacePtr<SensorProvider>;
using SensorProviderPtrInfo = mojo::InterfacePtrInfo<SensorProvider>;
using ThreadSafeSensorProviderPtr =
    mojo::ThreadSafeInterfacePtr<SensorProvider>;
using SensorProviderRequest = mojo::InterfaceRequest<SensorProvider>;
using SensorProviderAssociatedPtr =
    mojo::AssociatedInterfacePtr<SensorProvider>;
using ThreadSafeSensorProviderAssociatedPtr =
    mojo::ThreadSafeAssociatedInterfacePtr<SensorProvider>;
using SensorProviderAssociatedPtrInfo =
    mojo::AssociatedInterfacePtrInfo<SensorProvider>;
using SensorProviderAssociatedRequest =
    mojo::AssociatedInterfaceRequest<SensorProvider>;




}  // namespace blink
}  // namespace mojom
}  // namespace device

#endif  // SERVICES_DEVICE_PUBLIC_MOJOM_SENSOR_PROVIDER_MOJOM_BLINK_FORWARD_H_