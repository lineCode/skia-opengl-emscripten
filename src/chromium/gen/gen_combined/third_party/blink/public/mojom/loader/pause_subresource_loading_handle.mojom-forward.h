// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_LOADER_PAUSE_SUBRESOURCE_LOADING_HANDLE_MOJOM_FORWARD_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_LOADER_PAUSE_SUBRESOURCE_LOADING_HANDLE_MOJOM_FORWARD_H_



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


#include "third_party/blink/public/common/common_export.h"




namespace blink {
namespace mojom {
class PauseSubresourceLoadingHandle;
using PauseSubresourceLoadingHandlePtr = mojo::InterfacePtr<PauseSubresourceLoadingHandle>;
using PauseSubresourceLoadingHandlePtrInfo = mojo::InterfacePtrInfo<PauseSubresourceLoadingHandle>;
using ThreadSafePauseSubresourceLoadingHandlePtr =
    mojo::ThreadSafeInterfacePtr<PauseSubresourceLoadingHandle>;
using PauseSubresourceLoadingHandleRequest = mojo::InterfaceRequest<PauseSubresourceLoadingHandle>;
using PauseSubresourceLoadingHandleAssociatedPtr =
    mojo::AssociatedInterfacePtr<PauseSubresourceLoadingHandle>;
using ThreadSafePauseSubresourceLoadingHandleAssociatedPtr =
    mojo::ThreadSafeAssociatedInterfacePtr<PauseSubresourceLoadingHandle>;
using PauseSubresourceLoadingHandleAssociatedPtrInfo =
    mojo::AssociatedInterfacePtrInfo<PauseSubresourceLoadingHandle>;
using PauseSubresourceLoadingHandleAssociatedRequest =
    mojo::AssociatedInterfaceRequest<PauseSubresourceLoadingHandle>;




}  // namespace mojom
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_LOADER_PAUSE_SUBRESOURCE_LOADING_HANDLE_MOJOM_FORWARD_H_