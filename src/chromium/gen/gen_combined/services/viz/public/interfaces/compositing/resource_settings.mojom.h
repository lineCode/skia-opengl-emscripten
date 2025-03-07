// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_VIZ_PUBLIC_INTERFACES_COMPOSITING_RESOURCE_SETTINGS_MOJOM_H_
#define SERVICES_VIZ_PUBLIC_INTERFACES_COMPOSITING_RESOURCE_SETTINGS_MOJOM_H_

#include <stdint.h>

#include <limits>
#include <type_traits>
#include <utility>

#include "base/callback.h"
#include "base/macros.h"
#include "base/optional.h"

#include "mojo/public/cpp/bindings/mojo_buildflags.h"
#if BUILDFLAG(MOJO_TRACE_ENABLED)
#include "base/trace_event/trace_event.h"
#endif
#include "mojo/public/cpp/bindings/clone_traits.h"
#include "mojo/public/cpp/bindings/equals_traits.h"
#include "mojo/public/cpp/bindings/lib/serialization.h"
#include "mojo/public/cpp/bindings/struct_ptr.h"
#include "mojo/public/cpp/bindings/struct_traits.h"
#include "mojo/public/cpp/bindings/union_traits.h"
#include "services/viz/public/interfaces/compositing/resource_settings.mojom-shared.h"
#include "services/viz/public/interfaces/compositing/resource_settings.mojom-forward.h"
#include <string>
#include <vector>

#include "mojo/public/cpp/bindings/associated_interface_ptr.h"
#include "mojo/public/cpp/bindings/associated_interface_ptr_info.h"
#include "mojo/public/cpp/bindings/associated_interface_request.h"
#include "mojo/public/cpp/bindings/interface_ptr.h"
#include "mojo/public/cpp/bindings/interface_request.h"
#include "mojo/public/cpp/bindings/lib/control_message_handler.h"
#include "mojo/public/cpp/bindings/raw_ptr_impl_ref_traits.h"
#include "mojo/public/cpp/bindings/thread_safe_interface_ptr.h"


#include "mojo/public/cpp/bindings/lib/native_enum_serialization.h"
#include "mojo/public/cpp/bindings/lib/native_struct_serialization.h"
#include "components/viz/common/resources/resource_settings.h"




namespace viz {
namespace mojom {





class  ResourceSettings {
 public:
  using DataView = ResourceSettingsDataView;
  using Data_ = internal::ResourceSettings_Data;

  template <typename... Args>
  static ResourceSettingsPtr New(Args&&... args) {
    return ResourceSettingsPtr(
        base::in_place, std::forward<Args>(args)...);
  }

  template <typename U>
  static ResourceSettingsPtr From(const U& u) {
    return mojo::TypeConverter<ResourceSettingsPtr, U>::Convert(u);
  }

  template <typename U>
  U To() const {
    return mojo::TypeConverter<U, ResourceSettings>::Convert(*this);
  }


  ResourceSettings();

  explicit ResourceSettings(
      bool use_gpu_memory_buffer_resources);

  ~ResourceSettings();

  // Clone() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Clone() or copy
  // constructor/assignment are available for members.
  template <typename StructPtrType = ResourceSettingsPtr>
  ResourceSettingsPtr Clone() const;

  // Equals() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Equals() or == operator
  // are available for members.
  template <typename T,
            typename std::enable_if<std::is_same<
                T, ResourceSettings>::value>::type* = nullptr>
  bool Equals(const T& other) const;

  template <typename UserType>
  static std::vector<uint8_t> Serialize(UserType* input) {
    return mojo::internal::SerializeImpl<
        ResourceSettings::DataView, std::vector<uint8_t>>(input);
  }

  template <typename UserType>
  static mojo::Message SerializeAsMessage(UserType* input) {
    return mojo::internal::SerializeAsMessageImpl<
        ResourceSettings::DataView>(input);
  }

  // The returned Message is serialized only if the message is moved
  // cross-process or cross-language. Otherwise if the message is Deserialized
  // as the same UserType |input| will just be moved to |output| in
  // DeserializeFromMessage.
  template <typename UserType>
  static mojo::Message WrapAsMessage(UserType input) {
    return mojo::Message(std::make_unique<
        internal::ResourceSettings_UnserializedMessageContext<
            UserType, ResourceSettings::DataView>>(0, 0, std::move(input)));
  }

  template <typename UserType>
  static bool Deserialize(const void* data,
                          size_t data_num_bytes,
                          UserType* output) {
    return mojo::internal::DeserializeImpl<ResourceSettings::DataView>(
        data, data_num_bytes, std::vector<mojo::ScopedHandle>(), output, Validate);
  }

  template <typename UserType>
  static bool Deserialize(const std::vector<uint8_t>& input,
                          UserType* output) {
    return ResourceSettings::Deserialize(
        input.size() == 0 ? nullptr : &input.front(), input.size(), output);
  }

  template <typename UserType>
  static bool DeserializeFromMessage(mojo::Message input,
                                     UserType* output) {
    auto context = input.TakeUnserializedContext<
        internal::ResourceSettings_UnserializedMessageContext<
            UserType, ResourceSettings::DataView>>();
    if (context) {
      *output = std::move(context->TakeData());
      return true;
    }
    input.SerializeIfNecessary();
    return mojo::internal::DeserializeImpl<ResourceSettings::DataView>(
        input.payload(), input.payload_num_bytes(),
        std::move(*input.mutable_handles()), output, Validate);
  }

  
  bool use_gpu_memory_buffer_resources;

 private:
  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);
};




template <typename StructPtrType>
ResourceSettingsPtr ResourceSettings::Clone() const {
  return New(
      mojo::Clone(use_gpu_memory_buffer_resources)
  );
}

template <typename T,
          typename std::enable_if<std::is_same<
              T, ResourceSettings>::value>::type*>
bool ResourceSettings::Equals(const T& other_struct) const {
  if (!mojo::Equals(this->use_gpu_memory_buffer_resources, other_struct.use_gpu_memory_buffer_resources))
    return false;
  return true;
}


}  // namespace mojom
}  // namespace viz

namespace mojo {


template <>
struct  StructTraits<::viz::mojom::ResourceSettings::DataView,
                                         ::viz::mojom::ResourceSettingsPtr> {
  static bool IsNull(const ::viz::mojom::ResourceSettingsPtr& input) { return !input; }
  static void SetToNull(::viz::mojom::ResourceSettingsPtr* output) { output->reset(); }

  static decltype(::viz::mojom::ResourceSettings::use_gpu_memory_buffer_resources) use_gpu_memory_buffer_resources(
      const ::viz::mojom::ResourceSettingsPtr& input) {
    return input->use_gpu_memory_buffer_resources;
  }

  static bool Read(::viz::mojom::ResourceSettings::DataView input, ::viz::mojom::ResourceSettingsPtr* output);
};

}  // namespace mojo

#endif  // SERVICES_VIZ_PUBLIC_INTERFACES_COMPOSITING_RESOURCE_SETTINGS_MOJOM_H_