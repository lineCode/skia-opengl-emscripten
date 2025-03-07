// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MOJO_INTERFACES_RENDERER_EXTENSIONS_MOJOM_SHARED_H_
#define MEDIA_MOJO_INTERFACES_RENDERER_EXTENSIONS_MOJOM_SHARED_H_

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
#include "media/mojo/interfaces/renderer_extensions.mojom-shared-internal.h"
#include "media/mojo/interfaces/media_types.mojom-shared.h"
#include "mojo/public/mojom/base/time.mojom-shared.h"
#include "mojo/public/mojom/base/unguessable_token.mojom-shared.h"
#include "ui/gfx/geometry/mojo/geometry.mojom-shared.h"

#include "mojo/public/cpp/bindings/lib/interface_serialization.h"


#include "mojo/public/cpp/bindings/native_enum.h"
#include "mojo/public/cpp/bindings/lib/native_struct_serialization.h"




namespace media {
namespace mojom {


}  // namespace mojom
}  // namespace media

namespace mojo {
namespace internal {

}  // namespace internal
}  // namespace mojo


namespace media {
namespace mojom {
// Interface base classes. They are used for type safety check.
class MediaPlayerRendererClientExtensionInterfaceBase {};

using MediaPlayerRendererClientExtensionPtrDataView =
    mojo::InterfacePtrDataView<MediaPlayerRendererClientExtensionInterfaceBase>;
using MediaPlayerRendererClientExtensionRequestDataView =
    mojo::InterfaceRequestDataView<MediaPlayerRendererClientExtensionInterfaceBase>;
using MediaPlayerRendererClientExtensionAssociatedPtrInfoDataView =
    mojo::AssociatedInterfacePtrInfoDataView<MediaPlayerRendererClientExtensionInterfaceBase>;
using MediaPlayerRendererClientExtensionAssociatedRequestDataView =
    mojo::AssociatedInterfaceRequestDataView<MediaPlayerRendererClientExtensionInterfaceBase>;
class MediaPlayerRendererExtensionInterfaceBase {};

using MediaPlayerRendererExtensionPtrDataView =
    mojo::InterfacePtrDataView<MediaPlayerRendererExtensionInterfaceBase>;
using MediaPlayerRendererExtensionRequestDataView =
    mojo::InterfaceRequestDataView<MediaPlayerRendererExtensionInterfaceBase>;
using MediaPlayerRendererExtensionAssociatedPtrInfoDataView =
    mojo::AssociatedInterfacePtrInfoDataView<MediaPlayerRendererExtensionInterfaceBase>;
using MediaPlayerRendererExtensionAssociatedRequestDataView =
    mojo::AssociatedInterfaceRequestDataView<MediaPlayerRendererExtensionInterfaceBase>;
class FlingingRendererClientExtensionInterfaceBase {};

using FlingingRendererClientExtensionPtrDataView =
    mojo::InterfacePtrDataView<FlingingRendererClientExtensionInterfaceBase>;
using FlingingRendererClientExtensionRequestDataView =
    mojo::InterfaceRequestDataView<FlingingRendererClientExtensionInterfaceBase>;
using FlingingRendererClientExtensionAssociatedPtrInfoDataView =
    mojo::AssociatedInterfacePtrInfoDataView<FlingingRendererClientExtensionInterfaceBase>;
using FlingingRendererClientExtensionAssociatedRequestDataView =
    mojo::AssociatedInterfaceRequestDataView<FlingingRendererClientExtensionInterfaceBase>;


}  // namespace mojom
}  // namespace media

namespace std {

}  // namespace std

namespace mojo {

}  // namespace mojo


namespace media {
namespace mojom {


}  // namespace mojom
}  // namespace media

#endif  // MEDIA_MOJO_INTERFACES_RENDERER_EXTENSIONS_MOJOM_SHARED_H_