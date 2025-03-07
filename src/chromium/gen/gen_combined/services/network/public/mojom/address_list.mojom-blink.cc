// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4056)
#pragma warning(disable:4065)
#pragma warning(disable:4756)
#endif

#include "services/network/public/mojom/address_list.mojom-blink.h"

#include <math.h>
#include <stdint.h>
#include <utility>

#include "base/location.h"
#include "base/logging.h"
#include "base/run_loop.h"
#include "base/task/common/task_annotator.h"
#include "mojo/public/cpp/bindings/lib/message_internal.h"
#include "mojo/public/cpp/bindings/lib/serialization_util.h"
#include "mojo/public/cpp/bindings/lib/unserialized_message_context.h"
#include "mojo/public/cpp/bindings/lib/validate_params.h"
#include "mojo/public/cpp/bindings/lib/validation_context.h"
#include "mojo/public/cpp/bindings/lib/validation_errors.h"
#include "mojo/public/interfaces/bindings/interface_control_messages.mojom.h"

#include "services/network/public/mojom/address_list.mojom-params-data.h"
#include "services/network/public/mojom/address_list.mojom-shared-message-ids.h"

#include "services/network/public/mojom/address_list.mojom-blink-import-headers.h"
#include "mojo/public/cpp/bindings/lib/wtf_serialization.h"


#ifndef SERVICES_NETWORK_PUBLIC_MOJOM_ADDRESS_LIST_MOJOM_BLINK_JUMBO_H_
#define SERVICES_NETWORK_PUBLIC_MOJOM_ADDRESS_LIST_MOJOM_BLINK_JUMBO_H_
#endif
namespace network {
namespace mojom {
namespace blink {
AddressList::AddressList()
    : addresses(),
      canonical_name() {}

AddressList::AddressList(
    WTF::Vector<::network::mojom::blink::IPEndPointPtr> addresses_in,
    const WTF::String& canonical_name_in)
    : addresses(std::move(addresses_in)),
      canonical_name(std::move(canonical_name_in)) {}

AddressList::~AddressList() = default;

bool AddressList::Validate(
    const void* data,
    mojo::internal::ValidationContext* validation_context) {
  return Data_::Validate(data, validation_context);
}
}  // namespace blink
}  // namespace mojom
}  // namespace network

namespace mojo {


// static
bool StructTraits<::network::mojom::blink::AddressList::DataView, ::network::mojom::blink::AddressListPtr>::Read(
    ::network::mojom::blink::AddressList::DataView input,
    ::network::mojom::blink::AddressListPtr* output) {
  bool success = true;
  ::network::mojom::blink::AddressListPtr result(::network::mojom::blink::AddressList::New());
  
      if (!input.ReadAddresses(&result->addresses))
        success = false;
      if (!input.ReadCanonicalName(&result->canonical_name))
        success = false;
  *output = std::move(result);
  return success;
}

}  // namespace mojo

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif