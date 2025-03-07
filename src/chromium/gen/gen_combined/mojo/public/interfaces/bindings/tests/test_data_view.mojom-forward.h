// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_DATA_VIEW_MOJOM_FORWARD_H_
#define MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_DATA_VIEW_MOJOM_FORWARD_H_

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




namespace mojo {
namespace test {
namespace data_view {
class NestedStructDataView;

using TestNativeStructDataView = mojo::native::NativeStructDataView;

class TestStructDataView;

class TestUnionDataView;

enum class TestEnum : int32_t;
class NestedStruct;
using NestedStructPtr = mojo::InlinedStructPtr<NestedStruct>;

using TestNativeStruct = mojo::native::NativeStruct;
using TestNativeStructPtr = mojo::native::NativeStructPtr;

class TestStruct;
using TestStructPtr = mojo::StructPtr<TestStruct>;

class TestUnion;

typedef mojo::InlinedStructPtr<TestUnion> TestUnionPtr;

class TestInterface;
using TestInterfacePtr = mojo::InterfacePtr<TestInterface>;
using TestInterfacePtrInfo = mojo::InterfacePtrInfo<TestInterface>;
using ThreadSafeTestInterfacePtr =
    mojo::ThreadSafeInterfacePtr<TestInterface>;
using TestInterfaceRequest = mojo::InterfaceRequest<TestInterface>;
using TestInterfaceAssociatedPtr =
    mojo::AssociatedInterfacePtr<TestInterface>;
using ThreadSafeTestInterfaceAssociatedPtr =
    mojo::ThreadSafeAssociatedInterfacePtr<TestInterface>;
using TestInterfaceAssociatedPtrInfo =
    mojo::AssociatedInterfacePtrInfo<TestInterface>;
using TestInterfaceAssociatedRequest =
    mojo::AssociatedInterfaceRequest<TestInterface>;




}  // namespace data_view
}  // namespace test
}  // namespace mojo

#endif  // MOJO_PUBLIC_INTERFACES_BINDINGS_TESTS_TEST_DATA_VIEW_MOJOM_FORWARD_H_