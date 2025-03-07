// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_RESOURCE_COORDINATOR_PUBLIC_MOJOM_COORDINATION_UNIT_MOJOM_BLINK_FORWARD_H_
#define SERVICES_RESOURCE_COORDINATOR_PUBLIC_MOJOM_COORDINATION_UNIT_MOJOM_BLINK_FORWARD_H_



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


namespace resource_coordinator {
namespace mojom {

enum class CoordinationUnitType : int32_t;

enum class InterventionPolicy : int32_t;

enum class PolicyControlledIntervention : int32_t;
}  // namespace resource_coordinator
}  // namespace mojom
#include "third_party/blink/renderer/platform/wtf/hash_functions.h"

namespace WTF {
struct resource_coordinator_mojom_internal_CoordinationUnitType_DataHashFn;

template <>
struct DefaultHash<::resource_coordinator::mojom::CoordinationUnitType> {
  using Hash = resource_coordinator_mojom_internal_CoordinationUnitType_DataHashFn;
};
}  // namespace WTF

namespace WTF {
struct resource_coordinator_mojom_internal_InterventionPolicy_DataHashFn;

template <>
struct DefaultHash<::resource_coordinator::mojom::InterventionPolicy> {
  using Hash = resource_coordinator_mojom_internal_InterventionPolicy_DataHashFn;
};
}  // namespace WTF

namespace WTF {
struct resource_coordinator_mojom_internal_PolicyControlledIntervention_DataHashFn;

template <>
struct DefaultHash<::resource_coordinator::mojom::PolicyControlledIntervention> {
  using Hash = resource_coordinator_mojom_internal_PolicyControlledIntervention_DataHashFn;
};
}  // namespace WTF


namespace resource_coordinator {
namespace mojom {
namespace blink {
using CoordinationUnitType = CoordinationUnitType;  // Alias for definition in the parent namespace.
using InterventionPolicy = InterventionPolicy;  // Alias for definition in the parent namespace.
using PolicyControlledIntervention = PolicyControlledIntervention;  // Alias for definition in the parent namespace.
class DocumentCoordinationUnit;
using DocumentCoordinationUnitPtr = mojo::InterfacePtr<DocumentCoordinationUnit>;
using DocumentCoordinationUnitPtrInfo = mojo::InterfacePtrInfo<DocumentCoordinationUnit>;
using ThreadSafeDocumentCoordinationUnitPtr =
    mojo::ThreadSafeInterfacePtr<DocumentCoordinationUnit>;
using DocumentCoordinationUnitRequest = mojo::InterfaceRequest<DocumentCoordinationUnit>;
using DocumentCoordinationUnitAssociatedPtr =
    mojo::AssociatedInterfacePtr<DocumentCoordinationUnit>;
using ThreadSafeDocumentCoordinationUnitAssociatedPtr =
    mojo::ThreadSafeAssociatedInterfacePtr<DocumentCoordinationUnit>;
using DocumentCoordinationUnitAssociatedPtrInfo =
    mojo::AssociatedInterfacePtrInfo<DocumentCoordinationUnit>;
using DocumentCoordinationUnitAssociatedRequest =
    mojo::AssociatedInterfaceRequest<DocumentCoordinationUnit>;

class ProcessCoordinationUnit;
using ProcessCoordinationUnitPtr = mojo::InterfacePtr<ProcessCoordinationUnit>;
using ProcessCoordinationUnitPtrInfo = mojo::InterfacePtrInfo<ProcessCoordinationUnit>;
using ThreadSafeProcessCoordinationUnitPtr =
    mojo::ThreadSafeInterfacePtr<ProcessCoordinationUnit>;
using ProcessCoordinationUnitRequest = mojo::InterfaceRequest<ProcessCoordinationUnit>;
using ProcessCoordinationUnitAssociatedPtr =
    mojo::AssociatedInterfacePtr<ProcessCoordinationUnit>;
using ThreadSafeProcessCoordinationUnitAssociatedPtr =
    mojo::ThreadSafeAssociatedInterfacePtr<ProcessCoordinationUnit>;
using ProcessCoordinationUnitAssociatedPtrInfo =
    mojo::AssociatedInterfacePtrInfo<ProcessCoordinationUnit>;
using ProcessCoordinationUnitAssociatedRequest =
    mojo::AssociatedInterfaceRequest<ProcessCoordinationUnit>;




}  // namespace blink
}  // namespace mojom
}  // namespace resource_coordinator

#endif  // SERVICES_RESOURCE_COORDINATOR_PUBLIC_MOJOM_COORDINATION_UNIT_MOJOM_BLINK_FORWARD_H_