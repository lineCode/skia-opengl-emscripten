// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_FETCH_FETCH_API_REQUEST_MOJOM_BLINK_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_FETCH_FETCH_API_REQUEST_MOJOM_BLINK_H_

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
#include "third_party/blink/public/mojom/fetch/fetch_api_request.mojom-shared.h"
#include "third_party/blink/public/mojom/fetch/fetch_api_request.mojom-blink-forward.h"
#include "mojo/public/mojom/base/unguessable_token.mojom-blink.h"
#if defined(ENABLE_GNET)
#include "services/network/public/mojom/fetch_api.mojom-blink.h"
#include "services/network/public/mojom/request_context_frame_type.mojom-blink.h"
#include "services/network/public/mojom/url_loader.mojom-blink.h"
#endif // ENABLE_GNET
#include "third_party/blink/public/mojom/blob/serialized_blob.mojom-blink.h"
#include "third_party/blink/public/mojom/referrer.mojom-blink.h"
#include "url/mojom/url.mojom-blink.h"

#include "mojo/public/cpp/bindings/lib/wtf_clone_equals_util.h"
#include "mojo/public/cpp/bindings/lib/wtf_hash_util.h"
#include "third_party/blink/renderer/platform/wtf/hash_functions.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

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
#include "third_party/blink/renderer/platform/wtf/hash_map.h"
#include "third_party/blink/renderer/platform/wtf/text/string_hash.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"
#include "third_party/blink/renderer/platform/platform_export.h"




namespace WTF {
struct blink_mojom_internal_RequestContextType_DataHashFn {
  static unsigned GetHash(const ::blink::mojom::RequestContextType& value) {
    using utype = std::underlying_type<::blink::mojom::RequestContextType>::type;
    return DefaultHash<utype>::Hash().GetHash(static_cast<utype>(value));
  }
  static bool Equal(const ::blink::mojom::RequestContextType& left, const ::blink::mojom::RequestContextType& right) {
    return left == right;
  }
  static const bool safe_to_compare_to_empty_or_deleted = true;
};

template <>
struct HashTraits<::blink::mojom::RequestContextType>
    : public GenericHashTraits<::blink::mojom::RequestContextType> {
  static_assert(true,
                "-1000000 is a reserved enum value");
  static_assert(true,
                "-1000001 is a reserved enum value");
  static const bool hasIsEmptyValueFunction = true;
  static bool IsEmptyValue(const ::blink::mojom::RequestContextType& value) {
    return value == static_cast<::blink::mojom::RequestContextType>(-1000000);
  }
  static void ConstructDeletedValue(::blink::mojom::RequestContextType& slot, bool) {
    slot = static_cast<::blink::mojom::RequestContextType>(-1000001);
  }
  static bool IsDeletedValue(const ::blink::mojom::RequestContextType& value) {
    return value == static_cast<::blink::mojom::RequestContextType>(-1000001);
  }
};
}  // namespace WTF


namespace WTF {
struct blink_mojom_internal_FetchCacheMode_DataHashFn {
  static unsigned GetHash(const ::blink::mojom::FetchCacheMode& value) {
    using utype = std::underlying_type<::blink::mojom::FetchCacheMode>::type;
    return DefaultHash<utype>::Hash().GetHash(static_cast<utype>(value));
  }
  static bool Equal(const ::blink::mojom::FetchCacheMode& left, const ::blink::mojom::FetchCacheMode& right) {
    return left == right;
  }
  static const bool safe_to_compare_to_empty_or_deleted = true;
};

template <>
struct HashTraits<::blink::mojom::FetchCacheMode>
    : public GenericHashTraits<::blink::mojom::FetchCacheMode> {
  static_assert(true,
                "-1000000 is a reserved enum value");
  static_assert(true,
                "-1000001 is a reserved enum value");
  static const bool hasIsEmptyValueFunction = true;
  static bool IsEmptyValue(const ::blink::mojom::FetchCacheMode& value) {
    return value == static_cast<::blink::mojom::FetchCacheMode>(-1000000);
  }
  static void ConstructDeletedValue(::blink::mojom::FetchCacheMode& slot, bool) {
    slot = static_cast<::blink::mojom::FetchCacheMode>(-1000001);
  }
  static bool IsDeletedValue(const ::blink::mojom::FetchCacheMode& value) {
    return value == static_cast<::blink::mojom::FetchCacheMode>(-1000001);
  }
};
}  // namespace WTF


namespace WTF {
struct blink_mojom_internal_FetchImportanceMode_DataHashFn {
  static unsigned GetHash(const ::blink::mojom::FetchImportanceMode& value) {
    using utype = std::underlying_type<::blink::mojom::FetchImportanceMode>::type;
    return DefaultHash<utype>::Hash().GetHash(static_cast<utype>(value));
  }
  static bool Equal(const ::blink::mojom::FetchImportanceMode& left, const ::blink::mojom::FetchImportanceMode& right) {
    return left == right;
  }
  static const bool safe_to_compare_to_empty_or_deleted = true;
};

template <>
struct HashTraits<::blink::mojom::FetchImportanceMode>
    : public GenericHashTraits<::blink::mojom::FetchImportanceMode> {
  static_assert(true,
                "-1000000 is a reserved enum value");
  static_assert(true,
                "-1000001 is a reserved enum value");
  static const bool hasIsEmptyValueFunction = true;
  static bool IsEmptyValue(const ::blink::mojom::FetchImportanceMode& value) {
    return value == static_cast<::blink::mojom::FetchImportanceMode>(-1000000);
  }
  static void ConstructDeletedValue(::blink::mojom::FetchImportanceMode& slot, bool) {
    slot = static_cast<::blink::mojom::FetchImportanceMode>(-1000001);
  }
  static bool IsDeletedValue(const ::blink::mojom::FetchImportanceMode& value) {
    return value == static_cast<::blink::mojom::FetchImportanceMode>(-1000001);
  }
};
}  // namespace WTF


namespace blink {
namespace mojom {
namespace blink {









class PLATFORM_EXPORT FetchAPIRequestHeaders {
 public:
  using DataView = FetchAPIRequestHeadersDataView;
  using Data_ = internal::FetchAPIRequestHeaders_Data;

  template <typename... Args>
  static FetchAPIRequestHeadersPtr New(Args&&... args) {
    return FetchAPIRequestHeadersPtr(
        base::in_place, std::forward<Args>(args)...);
  }

  template <typename U>
  static FetchAPIRequestHeadersPtr From(const U& u) {
    return mojo::TypeConverter<FetchAPIRequestHeadersPtr, U>::Convert(u);
  }

  template <typename U>
  U To() const {
    return mojo::TypeConverter<U, FetchAPIRequestHeaders>::Convert(*this);
  }


  FetchAPIRequestHeaders();

  explicit FetchAPIRequestHeaders(
      const WTF::HashMap<WTF::String, WTF::String>& headers);

  ~FetchAPIRequestHeaders();

  // Clone() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Clone() or copy
  // constructor/assignment are available for members.
  template <typename StructPtrType = FetchAPIRequestHeadersPtr>
  FetchAPIRequestHeadersPtr Clone() const;

  // Equals() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Equals() or == operator
  // are available for members.
  template <typename T,
            typename std::enable_if<std::is_same<
                T, FetchAPIRequestHeaders>::value>::type* = nullptr>
  bool Equals(const T& other) const;

  template <typename UserType>
  static WTF::Vector<uint8_t> Serialize(UserType* input) {
    return mojo::internal::SerializeImpl<
        FetchAPIRequestHeaders::DataView, WTF::Vector<uint8_t>>(input);
  }

  template <typename UserType>
  static mojo::Message SerializeAsMessage(UserType* input) {
    return mojo::internal::SerializeAsMessageImpl<
        FetchAPIRequestHeaders::DataView>(input);
  }

  // The returned Message is serialized only if the message is moved
  // cross-process or cross-language. Otherwise if the message is Deserialized
  // as the same UserType |input| will just be moved to |output| in
  // DeserializeFromMessage.
  template <typename UserType>
  static mojo::Message WrapAsMessage(UserType input) {
    return mojo::Message(std::make_unique<
        internal::FetchAPIRequestHeaders_UnserializedMessageContext<
            UserType, FetchAPIRequestHeaders::DataView>>(0, 0, std::move(input)));
  }

  template <typename UserType>
  static bool Deserialize(const void* data,
                          size_t data_num_bytes,
                          UserType* output) {
    return mojo::internal::DeserializeImpl<FetchAPIRequestHeaders::DataView>(
        data, data_num_bytes, std::vector<mojo::ScopedHandle>(), output, Validate);
  }

  template <typename UserType>
  static bool Deserialize(const WTF::Vector<uint8_t>& input,
                          UserType* output) {
    return FetchAPIRequestHeaders::Deserialize(
        input.size() == 0 ? nullptr : &input.front(), input.size(), output);
  }

  template <typename UserType>
  static bool DeserializeFromMessage(mojo::Message input,
                                     UserType* output) {
    auto context = input.TakeUnserializedContext<
        internal::FetchAPIRequestHeaders_UnserializedMessageContext<
            UserType, FetchAPIRequestHeaders::DataView>>();
    if (context) {
      *output = std::move(context->TakeData());
      return true;
    }
    input.SerializeIfNecessary();
    return mojo::internal::DeserializeImpl<FetchAPIRequestHeaders::DataView>(
        input.payload(), input.payload_num_bytes(),
        std::move(*input.mutable_handles()), output, Validate);
  }

  
  WTF::HashMap<WTF::String, WTF::String> headers;

 private:
  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);
};





class PLATFORM_EXPORT FetchAPIRequest {
 public:
  using DataView = FetchAPIRequestDataView;
  using Data_ = internal::FetchAPIRequest_Data;

  template <typename... Args>
  static FetchAPIRequestPtr New(Args&&... args) {
    return FetchAPIRequestPtr(
        base::in_place, std::forward<Args>(args)...);
  }

  template <typename U>
  static FetchAPIRequestPtr From(const U& u) {
    return mojo::TypeConverter<FetchAPIRequestPtr, U>::Convert(u);
  }

  template <typename U>
  U To() const {
    return mojo::TypeConverter<U, FetchAPIRequest>::Convert(*this);
  }


  FetchAPIRequest();

  FetchAPIRequest(
      ::network::mojom::blink::FetchRequestMode mode,
      bool is_main_resource_load,
      RequestContextType request_context_type,
      ::network::mojom::blink::RequestContextFrameType frame_type,
      const ::blink::KURL& url,
      const WTF::String& method,
      WTF::HashMap<WTF::String, WTF::String, WTF::CaseFoldingHash> headers,
      const scoped_refptr<::blink::BlobDataHandle>& blob,
      scoped_refptr<::blink::EncodedFormData> body,
      ::blink::mojom::blink::ReferrerPtr referrer,
      ::network::mojom::blink::FetchCredentialsMode credentials_mode,
      FetchCacheMode cache_mode,
      ::network::mojom::blink::FetchRedirectMode redirect_mode,
      const WTF::String& integrity,
      ::network::mojom::blink::RequestPriority priority,
      const base::Optional<base::UnguessableToken>& fetch_window_id,
      bool keepalive,
      bool is_reload,
      bool is_history_navigation);

  ~FetchAPIRequest();

  // Clone() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Clone() or copy
  // constructor/assignment are available for members.
  template <typename StructPtrType = FetchAPIRequestPtr>
  FetchAPIRequestPtr Clone() const;

  // Equals() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Equals() or == operator
  // are available for members.
  template <typename T,
            typename std::enable_if<std::is_same<
                T, FetchAPIRequest>::value>::type* = nullptr>
  bool Equals(const T& other) const;

  template <typename UserType>
  static WTF::Vector<uint8_t> Serialize(UserType* input) {
    return mojo::internal::SerializeImpl<
        FetchAPIRequest::DataView, WTF::Vector<uint8_t>>(input);
  }

  template <typename UserType>
  static mojo::Message SerializeAsMessage(UserType* input) {
    return mojo::internal::SerializeAsMessageImpl<
        FetchAPIRequest::DataView>(input);
  }

  // The returned Message is serialized only if the message is moved
  // cross-process or cross-language. Otherwise if the message is Deserialized
  // as the same UserType |input| will just be moved to |output| in
  // DeserializeFromMessage.
  template <typename UserType>
  static mojo::Message WrapAsMessage(UserType input) {
    return mojo::Message(std::make_unique<
        internal::FetchAPIRequest_UnserializedMessageContext<
            UserType, FetchAPIRequest::DataView>>(0, 0, std::move(input)));
  }

  template <typename UserType>
  static bool Deserialize(const void* data,
                          size_t data_num_bytes,
                          UserType* output) {
    return mojo::internal::DeserializeImpl<FetchAPIRequest::DataView>(
        data, data_num_bytes, std::vector<mojo::ScopedHandle>(), output, Validate);
  }

  template <typename UserType>
  static bool Deserialize(const WTF::Vector<uint8_t>& input,
                          UserType* output) {
    return FetchAPIRequest::Deserialize(
        input.size() == 0 ? nullptr : &input.front(), input.size(), output);
  }

  template <typename UserType>
  static bool DeserializeFromMessage(mojo::Message input,
                                     UserType* output) {
    auto context = input.TakeUnserializedContext<
        internal::FetchAPIRequest_UnserializedMessageContext<
            UserType, FetchAPIRequest::DataView>>();
    if (context) {
      *output = std::move(context->TakeData());
      return true;
    }
    input.SerializeIfNecessary();
    return mojo::internal::DeserializeImpl<FetchAPIRequest::DataView>(
        input.payload(), input.payload_num_bytes(),
        std::move(*input.mutable_handles()), output, Validate);
  }

  
  ::network::mojom::blink::FetchRequestMode mode;
  
  bool is_main_resource_load;
  
  RequestContextType request_context_type;
  
  ::network::mojom::blink::RequestContextFrameType frame_type;
  
  ::blink::KURL url;
  
  WTF::String method;
  
  WTF::HashMap<WTF::String, WTF::String, WTF::CaseFoldingHash> headers;
  
  scoped_refptr<::blink::BlobDataHandle> blob;
  
  scoped_refptr<::blink::EncodedFormData> body;
  
  ::blink::mojom::blink::ReferrerPtr referrer;
  
  ::network::mojom::blink::FetchCredentialsMode credentials_mode;
  
  FetchCacheMode cache_mode;
  
  ::network::mojom::blink::FetchRedirectMode redirect_mode;
  
  WTF::String integrity;
  
  ::network::mojom::blink::RequestPriority priority;
  
  base::Optional<base::UnguessableToken> fetch_window_id;
  
  bool keepalive;
  
  bool is_reload;
  
  bool is_history_navigation;

 private:
  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);
  DISALLOW_COPY_AND_ASSIGN(FetchAPIRequest);
};

template <typename StructPtrType>
FetchAPIRequestHeadersPtr FetchAPIRequestHeaders::Clone() const {
  return New(
      mojo::Clone(headers)
  );
}

template <typename T,
          typename std::enable_if<std::is_same<
              T, FetchAPIRequestHeaders>::value>::type*>
bool FetchAPIRequestHeaders::Equals(const T& other_struct) const {
  if (!mojo::Equals(this->headers, other_struct.headers))
    return false;
  return true;
}
template <typename StructPtrType>
FetchAPIRequestPtr FetchAPIRequest::Clone() const {
  return New(
      mojo::Clone(mode),
      mojo::Clone(is_main_resource_load),
      mojo::Clone(request_context_type),
      mojo::Clone(frame_type),
      mojo::Clone(url),
      mojo::Clone(method),
      mojo::Clone(headers),
      mojo::Clone(blob),
      mojo::Clone(body),
      mojo::Clone(referrer),
      mojo::Clone(credentials_mode),
      mojo::Clone(cache_mode),
      mojo::Clone(redirect_mode),
      mojo::Clone(integrity),
      mojo::Clone(priority),
      mojo::Clone(fetch_window_id),
      mojo::Clone(keepalive),
      mojo::Clone(is_reload),
      mojo::Clone(is_history_navigation)
  );
}

template <typename T,
          typename std::enable_if<std::is_same<
              T, FetchAPIRequest>::value>::type*>
bool FetchAPIRequest::Equals(const T& other_struct) const {
  if (!mojo::Equals(this->mode, other_struct.mode))
    return false;
  if (!mojo::Equals(this->is_main_resource_load, other_struct.is_main_resource_load))
    return false;
  if (!mojo::Equals(this->request_context_type, other_struct.request_context_type))
    return false;
  if (!mojo::Equals(this->frame_type, other_struct.frame_type))
    return false;
  if (!mojo::Equals(this->url, other_struct.url))
    return false;
  if (!mojo::Equals(this->method, other_struct.method))
    return false;
  if (!mojo::Equals(this->headers, other_struct.headers))
    return false;
  if (!mojo::Equals(this->blob, other_struct.blob))
    return false;
  if (!mojo::Equals(this->body, other_struct.body))
    return false;
  if (!mojo::Equals(this->referrer, other_struct.referrer))
    return false;
  if (!mojo::Equals(this->credentials_mode, other_struct.credentials_mode))
    return false;
  if (!mojo::Equals(this->cache_mode, other_struct.cache_mode))
    return false;
  if (!mojo::Equals(this->redirect_mode, other_struct.redirect_mode))
    return false;
  if (!mojo::Equals(this->integrity, other_struct.integrity))
    return false;
  if (!mojo::Equals(this->priority, other_struct.priority))
    return false;
  if (!mojo::Equals(this->fetch_window_id, other_struct.fetch_window_id))
    return false;
  if (!mojo::Equals(this->keepalive, other_struct.keepalive))
    return false;
  if (!mojo::Equals(this->is_reload, other_struct.is_reload))
    return false;
  if (!mojo::Equals(this->is_history_navigation, other_struct.is_history_navigation))
    return false;
  return true;
}


}  // namespace blink
}  // namespace mojom
}  // namespace blink

namespace mojo {


template <>
struct PLATFORM_EXPORT StructTraits<::blink::mojom::blink::FetchAPIRequestHeaders::DataView,
                                         ::blink::mojom::blink::FetchAPIRequestHeadersPtr> {
  static bool IsNull(const ::blink::mojom::blink::FetchAPIRequestHeadersPtr& input) { return !input; }
  static void SetToNull(::blink::mojom::blink::FetchAPIRequestHeadersPtr* output) { output->reset(); }

  static const decltype(::blink::mojom::blink::FetchAPIRequestHeaders::headers)& headers(
      const ::blink::mojom::blink::FetchAPIRequestHeadersPtr& input) {
    return input->headers;
  }

  static bool Read(::blink::mojom::blink::FetchAPIRequestHeaders::DataView input, ::blink::mojom::blink::FetchAPIRequestHeadersPtr* output);
};


template <>
struct PLATFORM_EXPORT StructTraits<::blink::mojom::blink::FetchAPIRequest::DataView,
                                         ::blink::mojom::blink::FetchAPIRequestPtr> {
  static bool IsNull(const ::blink::mojom::blink::FetchAPIRequestPtr& input) { return !input; }
  static void SetToNull(::blink::mojom::blink::FetchAPIRequestPtr* output) { output->reset(); }

  static decltype(::blink::mojom::blink::FetchAPIRequest::mode) mode(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->mode;
  }

  static decltype(::blink::mojom::blink::FetchAPIRequest::is_main_resource_load) is_main_resource_load(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->is_main_resource_load;
  }

  static decltype(::blink::mojom::blink::FetchAPIRequest::request_context_type) request_context_type(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->request_context_type;
  }

  static decltype(::blink::mojom::blink::FetchAPIRequest::frame_type) frame_type(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->frame_type;
  }

  static const decltype(::blink::mojom::blink::FetchAPIRequest::url)& url(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->url;
  }

  static const decltype(::blink::mojom::blink::FetchAPIRequest::method)& method(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->method;
  }

  static const decltype(::blink::mojom::blink::FetchAPIRequest::headers)& headers(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->headers;
  }

  static  decltype(::blink::mojom::blink::FetchAPIRequest::blob)& blob(
       ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->blob;
  }

  static  decltype(::blink::mojom::blink::FetchAPIRequest::body)& body(
       ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->body;
  }

  static const decltype(::blink::mojom::blink::FetchAPIRequest::referrer)& referrer(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->referrer;
  }

  static decltype(::blink::mojom::blink::FetchAPIRequest::credentials_mode) credentials_mode(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->credentials_mode;
  }

  static decltype(::blink::mojom::blink::FetchAPIRequest::cache_mode) cache_mode(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->cache_mode;
  }

  static decltype(::blink::mojom::blink::FetchAPIRequest::redirect_mode) redirect_mode(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->redirect_mode;
  }

  static const decltype(::blink::mojom::blink::FetchAPIRequest::integrity)& integrity(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->integrity;
  }

  static decltype(::blink::mojom::blink::FetchAPIRequest::priority) priority(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->priority;
  }

  static const decltype(::blink::mojom::blink::FetchAPIRequest::fetch_window_id)& fetch_window_id(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->fetch_window_id;
  }

  static decltype(::blink::mojom::blink::FetchAPIRequest::keepalive) keepalive(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->keepalive;
  }

  static decltype(::blink::mojom::blink::FetchAPIRequest::is_reload) is_reload(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->is_reload;
  }

  static decltype(::blink::mojom::blink::FetchAPIRequest::is_history_navigation) is_history_navigation(
      const ::blink::mojom::blink::FetchAPIRequestPtr& input) {
    return input->is_history_navigation;
  }

  static bool Read(::blink::mojom::blink::FetchAPIRequest::DataView input, ::blink::mojom::blink::FetchAPIRequestPtr* output);
};

}  // namespace mojo

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_FETCH_FETCH_API_REQUEST_MOJOM_BLINK_H_
