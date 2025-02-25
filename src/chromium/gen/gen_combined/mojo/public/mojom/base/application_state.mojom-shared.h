// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_MOJOM_BASE_APPLICATION_STATE_MOJOM_SHARED_H_
#define MOJO_PUBLIC_MOJOM_BASE_APPLICATION_STATE_MOJOM_SHARED_H_

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
#include "mojo/public/mojom/base/application_state.mojom-shared-internal.h"

#include "mojo/public/cpp/bindings/lib/interface_serialization.h"


#include "mojo/public/cpp/bindings/native_enum.h"
#include "mojo/public/cpp/bindings/lib/native_struct_serialization.h"
#include "base/component_export.h"




namespace mojo_base {
namespace mojom {


}  // namespace mojom
}  // namespace mojo_base

namespace mojo {
namespace internal {

}  // namespace internal
}  // namespace mojo


namespace mojo_base {
namespace mojom {


enum class ApplicationState : int32_t {
  
  UNKNOWN,
  
  HAS_RUNNING_ACTIVITIES,
  
  HAS_PAUSED_ACTIVITIES,
  
  HAS_STOPPED_ACTIVITIES,
  
  HAS_DESTROYED_ACTIVITIES,
  kMinValue = 0,
  kMaxValue = 4,
};

COMPONENT_EXPORT(MOJO_BASE_MOJOM_SHARED) std::ostream& operator<<(std::ostream& os, ApplicationState value);
inline bool IsKnownEnumValue(ApplicationState value) {
  return internal::ApplicationState_Data::IsKnownValue(
      static_cast<int32_t>(value));
}


}  // namespace mojom
}  // namespace mojo_base

namespace std {

template <>
struct hash<::mojo_base::mojom::ApplicationState>
    : public mojo::internal::EnumHashImpl<::mojo_base::mojom::ApplicationState> {};

}  // namespace std

namespace mojo {


template <>
struct EnumTraits<::mojo_base::mojom::ApplicationState, ::mojo_base::mojom::ApplicationState> {
  static ::mojo_base::mojom::ApplicationState ToMojom(::mojo_base::mojom::ApplicationState input) { return input; }
  static bool FromMojom(::mojo_base::mojom::ApplicationState input, ::mojo_base::mojom::ApplicationState* output) {
    *output = input;
    return true;
  }
};

namespace internal {

template <typename MaybeConstUserType>
struct Serializer<::mojo_base::mojom::ApplicationState, MaybeConstUserType> {
  using UserType = typename std::remove_const<MaybeConstUserType>::type;
  using Traits = EnumTraits<::mojo_base::mojom::ApplicationState, UserType>;

  static void Serialize(UserType input, int32_t* output) {
    *output = static_cast<int32_t>(Traits::ToMojom(input));
  }

  static bool Deserialize(int32_t input, UserType* output) {
    return Traits::FromMojom(static_cast<::mojo_base::mojom::ApplicationState>(input), output);
  }
};

}  // namespace internal

}  // namespace mojo


namespace mojo_base {
namespace mojom {


}  // namespace mojom
}  // namespace mojo_base

#endif  // MOJO_PUBLIC_MOJOM_BASE_APPLICATION_STATE_MOJOM_SHARED_H_