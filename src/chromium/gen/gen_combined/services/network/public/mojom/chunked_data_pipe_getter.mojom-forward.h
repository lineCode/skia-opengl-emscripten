// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_NETWORK_PUBLIC_MOJOM_CHUNKED_DATA_PIPE_GETTER_MOJOM_FORWARD_H_
#define SERVICES_NETWORK_PUBLIC_MOJOM_CHUNKED_DATA_PIPE_GETTER_MOJOM_FORWARD_H_



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






namespace network {
namespace mojom {
class ChunkedDataPipeGetter;
using ChunkedDataPipeGetterPtr = mojo::InterfacePtr<ChunkedDataPipeGetter>;
using ChunkedDataPipeGetterPtrInfo = mojo::InterfacePtrInfo<ChunkedDataPipeGetter>;
using ThreadSafeChunkedDataPipeGetterPtr =
    mojo::ThreadSafeInterfacePtr<ChunkedDataPipeGetter>;
using ChunkedDataPipeGetterRequest = mojo::InterfaceRequest<ChunkedDataPipeGetter>;
using ChunkedDataPipeGetterAssociatedPtr =
    mojo::AssociatedInterfacePtr<ChunkedDataPipeGetter>;
using ThreadSafeChunkedDataPipeGetterAssociatedPtr =
    mojo::ThreadSafeAssociatedInterfacePtr<ChunkedDataPipeGetter>;
using ChunkedDataPipeGetterAssociatedPtrInfo =
    mojo::AssociatedInterfacePtrInfo<ChunkedDataPipeGetter>;
using ChunkedDataPipeGetterAssociatedRequest =
    mojo::AssociatedInterfaceRequest<ChunkedDataPipeGetter>;




}  // namespace mojom
}  // namespace network

#endif  // SERVICES_NETWORK_PUBLIC_MOJOM_CHUNKED_DATA_PIPE_GETTER_MOJOM_FORWARD_H_