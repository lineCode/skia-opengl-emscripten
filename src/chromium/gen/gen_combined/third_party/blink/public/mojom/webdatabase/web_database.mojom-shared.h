// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_WEBDATABASE_WEB_DATABASE_MOJOM_SHARED_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_WEBDATABASE_WEB_DATABASE_MOJOM_SHARED_H_

#include <stdint.h>

#include <functional>
#include <ostream>
#include <type_traits>
#include <utility>

#include "base/compiler_specific.h"
#include "base/containers/flat_map.h"
#include "mojo/public/cpp/bindings/array_data_view.h"
#include "mojo/public/cpp/bindings/enum_traits.h"
#include "mojo/public/cpp/bindings/interface_data_view.h"
#include "mojo/public/cpp/bindings/lib/bindings_internal.h"
#include "mojo/public/cpp/bindings/lib/serialization.h"
#include "mojo/public/cpp/bindings/map_data_view.h"
#include "mojo/public/cpp/bindings/string_data_view.h"
#include "third_party/blink/public/mojom/webdatabase/web_database.mojom-shared-internal.h"
#include "mojo/public/mojom/base/file.mojom-shared.h"
#include "mojo/public/mojom/base/string16.mojom-shared.h"
#include "url/mojom/origin.mojom-shared.h"

#include "mojo/public/cpp/bindings/lib/interface_serialization.h"


#include "mojo/public/cpp/bindings/native_enum.h"
#include "mojo/public/cpp/bindings/lib/native_struct_serialization.h"
#include "base/component_export.h"




namespace blink {
namespace mojom {


}  // namespace mojom
}  // namespace blink

namespace mojo {
namespace internal {

}  // namespace internal
}  // namespace mojo


namespace blink {
namespace mojom {
// Interface base classes. They are used for type safety check.
class WebDatabaseInterfaceBase {};

using WebDatabasePtrDataView =
    mojo::InterfacePtrDataView<WebDatabaseInterfaceBase>;
using WebDatabaseRequestDataView =
    mojo::InterfaceRequestDataView<WebDatabaseInterfaceBase>;
using WebDatabaseAssociatedPtrInfoDataView =
    mojo::AssociatedInterfacePtrInfoDataView<WebDatabaseInterfaceBase>;
using WebDatabaseAssociatedRequestDataView =
    mojo::AssociatedInterfaceRequestDataView<WebDatabaseInterfaceBase>;
class WebDatabaseHostInterfaceBase {};

using WebDatabaseHostPtrDataView =
    mojo::InterfacePtrDataView<WebDatabaseHostInterfaceBase>;
using WebDatabaseHostRequestDataView =
    mojo::InterfaceRequestDataView<WebDatabaseHostInterfaceBase>;
using WebDatabaseHostAssociatedPtrInfoDataView =
    mojo::AssociatedInterfacePtrInfoDataView<WebDatabaseHostInterfaceBase>;
using WebDatabaseHostAssociatedRequestDataView =
    mojo::AssociatedInterfaceRequestDataView<WebDatabaseHostInterfaceBase>;


}  // namespace mojom
}  // namespace blink

namespace std {

}  // namespace std

namespace mojo {

}  // namespace mojo


namespace blink {
namespace mojom {


}  // namespace mojom
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_WEBDATABASE_WEB_DATABASE_MOJOM_SHARED_H_