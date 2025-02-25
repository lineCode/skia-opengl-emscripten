// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_SERVICE_MANAGER_PUBLIC_MOJOM_INTERFACE_PROVIDER_SPEC_MOJOM_FORWARD_H_
#define SERVICES_SERVICE_MANAGER_PUBLIC_MOJOM_INTERFACE_PROVIDER_SPEC_MOJOM_FORWARD_H_

#include "mojo/public/cpp/bindings/struct_ptr.h"


#include "mojo/public/cpp/bindings/lib/buffer.h"


#include "mojo/public/cpp/bindings/lib/native_enum_serialization.h"
#include "mojo/public/cpp/bindings/lib/native_struct_serialization.h"
#include "base/component_export.h"




namespace service_manager {
namespace mojom {
class InterfaceSetDataView;

class CapabilitySetDataView;

class InterfaceProviderSpecDataView;


COMPONENT_EXPORT(SERVICE_MANAGER_MOJOM) extern const char kServiceManager_ConnectorSpec[];
class InterfaceSet;
using InterfaceSetPtr = mojo::StructPtr<InterfaceSet>;

class CapabilitySet;
using CapabilitySetPtr = mojo::StructPtr<CapabilitySet>;

class InterfaceProviderSpec;
using InterfaceProviderSpecPtr = mojo::StructPtr<InterfaceProviderSpec>;




}  // namespace mojom
}  // namespace service_manager
#include "services/service_manager/public/cpp/interface_provider_spec.h"

#endif  // SERVICES_SERVICE_MANAGER_PUBLIC_MOJOM_INTERFACE_PROVIDER_SPEC_MOJOM_FORWARD_H_