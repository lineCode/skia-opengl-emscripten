// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_SERVICE_MANAGER_PUBLIC_MOJOM_INTERFACE_PROVIDER_MOJOM_BLINK_FORWARD_H_
#define SERVICES_SERVICE_MANAGER_PUBLIC_MOJOM_INTERFACE_PROVIDER_MOJOM_BLINK_FORWARD_H_



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


#include "base/component_export.h"


namespace service_manager {
namespace mojom {
}  // namespace service_manager
}  // namespace mojom


namespace service_manager {
namespace mojom {
namespace blink {
class InterfaceProvider;
using InterfaceProviderPtr = mojo::InterfacePtr<InterfaceProvider>;
using InterfaceProviderPtrInfo = mojo::InterfacePtrInfo<InterfaceProvider>;
using ThreadSafeInterfaceProviderPtr =
    mojo::ThreadSafeInterfacePtr<InterfaceProvider>;
using InterfaceProviderRequest = mojo::InterfaceRequest<InterfaceProvider>;
using InterfaceProviderAssociatedPtr =
    mojo::AssociatedInterfacePtr<InterfaceProvider>;
using ThreadSafeInterfaceProviderAssociatedPtr =
    mojo::ThreadSafeAssociatedInterfacePtr<InterfaceProvider>;
using InterfaceProviderAssociatedPtrInfo =
    mojo::AssociatedInterfacePtrInfo<InterfaceProvider>;
using InterfaceProviderAssociatedRequest =
    mojo::AssociatedInterfaceRequest<InterfaceProvider>;




}  // namespace blink
}  // namespace mojom
}  // namespace service_manager

#endif  // SERVICES_SERVICE_MANAGER_PUBLIC_MOJOM_INTERFACE_PROVIDER_MOJOM_BLINK_FORWARD_H_