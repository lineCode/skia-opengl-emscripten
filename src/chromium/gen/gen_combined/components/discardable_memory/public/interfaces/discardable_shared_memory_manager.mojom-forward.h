// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_DISCARDABLE_MEMORY_PUBLIC_INTERFACES_DISCARDABLE_SHARED_MEMORY_MANAGER_MOJOM_FORWARD_H_
#define COMPONENTS_DISCARDABLE_MEMORY_PUBLIC_INTERFACES_DISCARDABLE_SHARED_MEMORY_MANAGER_MOJOM_FORWARD_H_



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






namespace discardable_memory {
namespace mojom {
class DiscardableSharedMemoryManager;
using DiscardableSharedMemoryManagerPtr = mojo::InterfacePtr<DiscardableSharedMemoryManager>;
using DiscardableSharedMemoryManagerPtrInfo = mojo::InterfacePtrInfo<DiscardableSharedMemoryManager>;
using ThreadSafeDiscardableSharedMemoryManagerPtr =
    mojo::ThreadSafeInterfacePtr<DiscardableSharedMemoryManager>;
using DiscardableSharedMemoryManagerRequest = mojo::InterfaceRequest<DiscardableSharedMemoryManager>;
using DiscardableSharedMemoryManagerAssociatedPtr =
    mojo::AssociatedInterfacePtr<DiscardableSharedMemoryManager>;
using ThreadSafeDiscardableSharedMemoryManagerAssociatedPtr =
    mojo::ThreadSafeAssociatedInterfacePtr<DiscardableSharedMemoryManager>;
using DiscardableSharedMemoryManagerAssociatedPtrInfo =
    mojo::AssociatedInterfacePtrInfo<DiscardableSharedMemoryManager>;
using DiscardableSharedMemoryManagerAssociatedRequest =
    mojo::AssociatedInterfaceRequest<DiscardableSharedMemoryManager>;




}  // namespace mojom
}  // namespace discardable_memory

#endif  // COMPONENTS_DISCARDABLE_MEMORY_PUBLIC_INTERFACES_DISCARDABLE_SHARED_MEMORY_MANAGER_MOJOM_FORWARD_H_