// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_SERIAL_SERIAL_MOJOM_FORWARD_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_SERIAL_SERIAL_MOJOM_FORWARD_H_

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
class SerialPortInfoDataView;

class SerialPortFilterDataView;

class SerialPortInfo;
using SerialPortInfoPtr = mojo::StructPtr<SerialPortInfo>;

class SerialPortFilter;
using SerialPortFilterPtr = mojo::InlinedStructPtr<SerialPortFilter>;

class SerialService;
using SerialServicePtr = mojo::InterfacePtr<SerialService>;
using SerialServicePtrInfo = mojo::InterfacePtrInfo<SerialService>;
using ThreadSafeSerialServicePtr =
    mojo::ThreadSafeInterfacePtr<SerialService>;
using SerialServiceRequest = mojo::InterfaceRequest<SerialService>;
using SerialServiceAssociatedPtr =
    mojo::AssociatedInterfacePtr<SerialService>;
using ThreadSafeSerialServiceAssociatedPtr =
    mojo::ThreadSafeAssociatedInterfacePtr<SerialService>;
using SerialServiceAssociatedPtrInfo =
    mojo::AssociatedInterfacePtrInfo<SerialService>;
using SerialServiceAssociatedRequest =
    mojo::AssociatedInterfaceRequest<SerialService>;




}  // namespace mojom
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_SERIAL_SERIAL_MOJOM_FORWARD_H_