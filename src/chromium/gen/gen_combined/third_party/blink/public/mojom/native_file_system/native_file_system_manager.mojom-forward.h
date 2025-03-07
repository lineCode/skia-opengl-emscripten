// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_NATIVE_FILE_SYSTEM_NATIVE_FILE_SYSTEM_MANAGER_MOJOM_FORWARD_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_NATIVE_FILE_SYSTEM_NATIVE_FILE_SYSTEM_MANAGER_MOJOM_FORWARD_H_

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
#include "third_party/blink/public/common/common_export.h"




namespace blink {
namespace mojom {
class ChooseFileSystemEntryAcceptsOptionDataView;


enum class ChooseFileSystemEntryType : int32_t;
class ChooseFileSystemEntryAcceptsOption;
using ChooseFileSystemEntryAcceptsOptionPtr = mojo::StructPtr<ChooseFileSystemEntryAcceptsOption>;

class NativeFileSystemManager;
using NativeFileSystemManagerPtr = mojo::InterfacePtr<NativeFileSystemManager>;
using NativeFileSystemManagerPtrInfo = mojo::InterfacePtrInfo<NativeFileSystemManager>;
using ThreadSafeNativeFileSystemManagerPtr =
    mojo::ThreadSafeInterfacePtr<NativeFileSystemManager>;
using NativeFileSystemManagerRequest = mojo::InterfaceRequest<NativeFileSystemManager>;
using NativeFileSystemManagerAssociatedPtr =
    mojo::AssociatedInterfacePtr<NativeFileSystemManager>;
using ThreadSafeNativeFileSystemManagerAssociatedPtr =
    mojo::ThreadSafeAssociatedInterfacePtr<NativeFileSystemManager>;
using NativeFileSystemManagerAssociatedPtrInfo =
    mojo::AssociatedInterfacePtrInfo<NativeFileSystemManager>;
using NativeFileSystemManagerAssociatedRequest =
    mojo::AssociatedInterfaceRequest<NativeFileSystemManager>;




}  // namespace mojom
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_NATIVE_FILE_SYSTEM_NATIVE_FILE_SYSTEM_MANAGER_MOJOM_FORWARD_H_