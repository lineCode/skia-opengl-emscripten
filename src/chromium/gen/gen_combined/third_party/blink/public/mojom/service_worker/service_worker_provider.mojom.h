// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_SERVICE_WORKER_SERVICE_WORKER_PROVIDER_MOJOM_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_SERVICE_WORKER_SERVICE_WORKER_PROVIDER_MOJOM_H_

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
#include "third_party/blink/public/mojom/service_worker/service_worker_provider.mojom-shared.h"
#include "third_party/blink/public/mojom/service_worker/service_worker_provider.mojom-forward.h"
#if defined(ENABLE_GNET)
#include "services/network/public/mojom/url_loader_factory.mojom-forward.h"
#endif // ENABLE_GNET
#include "services/service_manager/public/mojom/interface_provider.mojom-forward.h"
#include "third_party/blink/public/mojom/cache_storage/cache_storage.mojom-forward.h"
#include "third_party/blink/public/mojom/service_worker/controller_service_worker_mode.mojom-forward.h"
#include "third_party/blink/public/mojom/service_worker/service_worker_container.mojom-forward.h"
#include "third_party/blink/public/mojom/service_worker/service_worker_object.mojom-forward.h"
#include "third_party/blink/public/mojom/service_worker/service_worker_provider_type.mojom-forward.h"
#include "third_party/blink/public/mojom/service_worker/service_worker_registration.mojom-forward.h"
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
#include "third_party/blink/public/common/common_export.h"




namespace blink {
namespace mojom {

class ServiceWorkerWorkerClientProxy;

template <typename ImplRefTraits>
class ServiceWorkerWorkerClientStub;

class ServiceWorkerWorkerClientRequestValidator;


class BLINK_COMMON_EXPORT ServiceWorkerWorkerClient
    : public ServiceWorkerWorkerClientInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = ServiceWorkerWorkerClientInterfaceBase;
  using Proxy_ = ServiceWorkerWorkerClientProxy;

  template <typename ImplRefTraits>
  using Stub_ = ServiceWorkerWorkerClientStub<ImplRefTraits>;

  using RequestValidator_ = ServiceWorkerWorkerClientRequestValidator;
  using ResponseValidator_ = mojo::PassThroughFilter;
  enum MethodMinVersions : uint32_t {
    kOnControllerChangedMinVersion = 0,
  };
  virtual ~ServiceWorkerWorkerClient() {}

  
  virtual void OnControllerChanged(::blink::mojom::ControllerServiceWorkerMode mode) = 0;
};

class ServiceWorkerWorkerClientRegistryProxy;

template <typename ImplRefTraits>
class ServiceWorkerWorkerClientRegistryStub;

class ServiceWorkerWorkerClientRegistryRequestValidator;


class BLINK_COMMON_EXPORT ServiceWorkerWorkerClientRegistry
    : public ServiceWorkerWorkerClientRegistryInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = ServiceWorkerWorkerClientRegistryInterfaceBase;
  using Proxy_ = ServiceWorkerWorkerClientRegistryProxy;

  template <typename ImplRefTraits>
  using Stub_ = ServiceWorkerWorkerClientRegistryStub<ImplRefTraits>;

  using RequestValidator_ = ServiceWorkerWorkerClientRegistryRequestValidator;
  using ResponseValidator_ = mojo::PassThroughFilter;
  enum MethodMinVersions : uint32_t {
    kRegisterWorkerClientMinVersion = 0,
    kCloneWorkerClientRegistryMinVersion = 0,
  };
  virtual ~ServiceWorkerWorkerClientRegistry() {}

  
  virtual void RegisterWorkerClient(ServiceWorkerWorkerClientPtr client) = 0;

  
  virtual void CloneWorkerClientRegistry(ServiceWorkerWorkerClientRegistryRequest host) = 0;
};

class BLINK_COMMON_EXPORT ServiceWorkerWorkerClientProxy
    : public ServiceWorkerWorkerClient {
 public:
  using InterfaceType = ServiceWorkerWorkerClient;

  explicit ServiceWorkerWorkerClientProxy(mojo::MessageReceiverWithResponder* receiver);
  void OnControllerChanged(::blink::mojom::ControllerServiceWorkerMode mode) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};

class BLINK_COMMON_EXPORT ServiceWorkerWorkerClientRegistryProxy
    : public ServiceWorkerWorkerClientRegistry {
 public:
  using InterfaceType = ServiceWorkerWorkerClientRegistry;

  explicit ServiceWorkerWorkerClientRegistryProxy(mojo::MessageReceiverWithResponder* receiver);
  void RegisterWorkerClient(ServiceWorkerWorkerClientPtr client) final;
  void CloneWorkerClientRegistry(ServiceWorkerWorkerClientRegistryRequest host) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};
class BLINK_COMMON_EXPORT ServiceWorkerWorkerClientStubDispatch {
 public:
  static bool Accept(ServiceWorkerWorkerClient* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      ServiceWorkerWorkerClient* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<ServiceWorkerWorkerClient>>
class ServiceWorkerWorkerClientStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  ServiceWorkerWorkerClientStub() {}
  ~ServiceWorkerWorkerClientStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return ServiceWorkerWorkerClientStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return ServiceWorkerWorkerClientStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class BLINK_COMMON_EXPORT ServiceWorkerWorkerClientRegistryStubDispatch {
 public:
  static bool Accept(ServiceWorkerWorkerClientRegistry* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      ServiceWorkerWorkerClientRegistry* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<ServiceWorkerWorkerClientRegistry>>
class ServiceWorkerWorkerClientRegistryStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  ServiceWorkerWorkerClientRegistryStub() {}
  ~ServiceWorkerWorkerClientRegistryStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return ServiceWorkerWorkerClientRegistryStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return ServiceWorkerWorkerClientRegistryStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class BLINK_COMMON_EXPORT ServiceWorkerWorkerClientRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class BLINK_COMMON_EXPORT ServiceWorkerWorkerClientRegistryRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};










class BLINK_COMMON_EXPORT ServiceWorkerProviderInfoForWorker {
 public:
  using DataView = ServiceWorkerProviderInfoForWorkerDataView;
  using Data_ = internal::ServiceWorkerProviderInfoForWorker_Data;

  template <typename... Args>
  static ServiceWorkerProviderInfoForWorkerPtr New(Args&&... args) {
    return ServiceWorkerProviderInfoForWorkerPtr(
        base::in_place, std::forward<Args>(args)...);
  }

  template <typename U>
  static ServiceWorkerProviderInfoForWorkerPtr From(const U& u) {
    return mojo::TypeConverter<ServiceWorkerProviderInfoForWorkerPtr, U>::Convert(u);
  }

  template <typename U>
  U To() const {
    return mojo::TypeConverter<U, ServiceWorkerProviderInfoForWorker>::Convert(*this);
  }


  ServiceWorkerProviderInfoForWorker();

  ServiceWorkerProviderInfoForWorker(
      ::blink::mojom::ServiceWorkerContainerHostAssociatedPtrInfo host_ptr_info,
      ::blink::mojom::ServiceWorkerContainerAssociatedRequest client_request);

  ~ServiceWorkerProviderInfoForWorker();

  // Clone() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Clone() or copy
  // constructor/assignment are available for members.
  template <typename StructPtrType = ServiceWorkerProviderInfoForWorkerPtr>
  ServiceWorkerProviderInfoForWorkerPtr Clone() const;

  // Equals() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Equals() or == operator
  // are available for members.
  template <typename T,
            typename std::enable_if<std::is_same<
                T, ServiceWorkerProviderInfoForWorker>::value>::type* = nullptr>
  bool Equals(const T& other) const;

  template <typename UserType>
  static std::vector<uint8_t> Serialize(UserType* input) {
    return mojo::internal::SerializeImpl<
        ServiceWorkerProviderInfoForWorker::DataView, std::vector<uint8_t>>(input);
  }

  template <typename UserType>
  static mojo::Message SerializeAsMessage(UserType* input) {
    return mojo::internal::SerializeAsMessageImpl<
        ServiceWorkerProviderInfoForWorker::DataView>(input);
  }

  // The returned Message is serialized only if the message is moved
  // cross-process or cross-language. Otherwise if the message is Deserialized
  // as the same UserType |input| will just be moved to |output| in
  // DeserializeFromMessage.
  template <typename UserType>
  static mojo::Message WrapAsMessage(UserType input) {
    return mojo::Message(std::make_unique<
        internal::ServiceWorkerProviderInfoForWorker_UnserializedMessageContext<
            UserType, ServiceWorkerProviderInfoForWorker::DataView>>(0, 0, std::move(input)));
  }

  template <typename UserType>
  static bool Deserialize(const void* data,
                          size_t data_num_bytes,
                          UserType* output) {
    return mojo::internal::DeserializeImpl<ServiceWorkerProviderInfoForWorker::DataView>(
        data, data_num_bytes, std::vector<mojo::ScopedHandle>(), output, Validate);
  }

  template <typename UserType>
  static bool Deserialize(const std::vector<uint8_t>& input,
                          UserType* output) {
    return ServiceWorkerProviderInfoForWorker::Deserialize(
        input.size() == 0 ? nullptr : &input.front(), input.size(), output);
  }

  template <typename UserType>
  static bool DeserializeFromMessage(mojo::Message input,
                                     UserType* output) {
    auto context = input.TakeUnserializedContext<
        internal::ServiceWorkerProviderInfoForWorker_UnserializedMessageContext<
            UserType, ServiceWorkerProviderInfoForWorker::DataView>>();
    if (context) {
      *output = std::move(context->TakeData());
      return true;
    }
    input.SerializeIfNecessary();
    return mojo::internal::DeserializeImpl<ServiceWorkerProviderInfoForWorker::DataView>(
        input.payload(), input.payload_num_bytes(),
        std::move(*input.mutable_handles()), output, Validate);
  }

  
  ::blink::mojom::ServiceWorkerContainerHostAssociatedPtrInfo host_ptr_info;
  
  ::blink::mojom::ServiceWorkerContainerAssociatedRequest client_request;

 private:
  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);
  DISALLOW_COPY_AND_ASSIGN(ServiceWorkerProviderInfoForWorker);
};





class BLINK_COMMON_EXPORT ServiceWorkerProviderInfoForStartWorker {
 public:
  using DataView = ServiceWorkerProviderInfoForStartWorkerDataView;
  using Data_ = internal::ServiceWorkerProviderInfoForStartWorker_Data;

  template <typename... Args>
  static ServiceWorkerProviderInfoForStartWorkerPtr New(Args&&... args) {
    return ServiceWorkerProviderInfoForStartWorkerPtr(
        base::in_place, std::forward<Args>(args)...);
  }

  template <typename U>
  static ServiceWorkerProviderInfoForStartWorkerPtr From(const U& u) {
    return mojo::TypeConverter<ServiceWorkerProviderInfoForStartWorkerPtr, U>::Convert(u);
  }

  template <typename U>
  U To() const {
    return mojo::TypeConverter<U, ServiceWorkerProviderInfoForStartWorker>::Convert(*this);
  }


  ServiceWorkerProviderInfoForStartWorker();

  ServiceWorkerProviderInfoForStartWorker(
      ::blink::mojom::ServiceWorkerContainerHostAssociatedPtrInfo host_ptr_info,
      ::blink::mojom::ServiceWorkerContainerAssociatedRequest client_request,
      ::network::mojom::URLLoaderFactoryPtrInfo script_loader_factory_ptr_info,
      ::blink::mojom::CacheStoragePtrInfo cache_storage,
      ::service_manager::mojom::InterfaceProviderPtrInfo interface_provider);

  ~ServiceWorkerProviderInfoForStartWorker();

  // Clone() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Clone() or copy
  // constructor/assignment are available for members.
  template <typename StructPtrType = ServiceWorkerProviderInfoForStartWorkerPtr>
  ServiceWorkerProviderInfoForStartWorkerPtr Clone() const;

  // Equals() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Equals() or == operator
  // are available for members.
  template <typename T,
            typename std::enable_if<std::is_same<
                T, ServiceWorkerProviderInfoForStartWorker>::value>::type* = nullptr>
  bool Equals(const T& other) const;

  template <typename UserType>
  static std::vector<uint8_t> Serialize(UserType* input) {
    return mojo::internal::SerializeImpl<
        ServiceWorkerProviderInfoForStartWorker::DataView, std::vector<uint8_t>>(input);
  }

  template <typename UserType>
  static mojo::Message SerializeAsMessage(UserType* input) {
    return mojo::internal::SerializeAsMessageImpl<
        ServiceWorkerProviderInfoForStartWorker::DataView>(input);
  }

  // The returned Message is serialized only if the message is moved
  // cross-process or cross-language. Otherwise if the message is Deserialized
  // as the same UserType |input| will just be moved to |output| in
  // DeserializeFromMessage.
  template <typename UserType>
  static mojo::Message WrapAsMessage(UserType input) {
    return mojo::Message(std::make_unique<
        internal::ServiceWorkerProviderInfoForStartWorker_UnserializedMessageContext<
            UserType, ServiceWorkerProviderInfoForStartWorker::DataView>>(0, 0, std::move(input)));
  }

  template <typename UserType>
  static bool Deserialize(const void* data,
                          size_t data_num_bytes,
                          UserType* output) {
    return mojo::internal::DeserializeImpl<ServiceWorkerProviderInfoForStartWorker::DataView>(
        data, data_num_bytes, std::vector<mojo::ScopedHandle>(), output, Validate);
  }

  template <typename UserType>
  static bool Deserialize(const std::vector<uint8_t>& input,
                          UserType* output) {
    return ServiceWorkerProviderInfoForStartWorker::Deserialize(
        input.size() == 0 ? nullptr : &input.front(), input.size(), output);
  }

  template <typename UserType>
  static bool DeserializeFromMessage(mojo::Message input,
                                     UserType* output) {
    auto context = input.TakeUnserializedContext<
        internal::ServiceWorkerProviderInfoForStartWorker_UnserializedMessageContext<
            UserType, ServiceWorkerProviderInfoForStartWorker::DataView>>();
    if (context) {
      *output = std::move(context->TakeData());
      return true;
    }
    input.SerializeIfNecessary();
    return mojo::internal::DeserializeImpl<ServiceWorkerProviderInfoForStartWorker::DataView>(
        input.payload(), input.payload_num_bytes(),
        std::move(*input.mutable_handles()), output, Validate);
  }

  
  ::blink::mojom::ServiceWorkerContainerHostAssociatedPtrInfo host_ptr_info;
  
  ::blink::mojom::ServiceWorkerContainerAssociatedRequest client_request;
  
  ::network::mojom::URLLoaderFactoryPtrInfo script_loader_factory_ptr_info;
  
  ::blink::mojom::CacheStoragePtrInfo cache_storage;
  
  ::service_manager::mojom::InterfaceProviderPtrInfo interface_provider;

 private:
  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);
  DISALLOW_COPY_AND_ASSIGN(ServiceWorkerProviderInfoForStartWorker);
};





class BLINK_COMMON_EXPORT ServiceWorkerProviderInfoForWindow {
 public:
  using DataView = ServiceWorkerProviderInfoForWindowDataView;
  using Data_ = internal::ServiceWorkerProviderInfoForWindow_Data;

  template <typename... Args>
  static ServiceWorkerProviderInfoForWindowPtr New(Args&&... args) {
    return ServiceWorkerProviderInfoForWindowPtr(
        base::in_place, std::forward<Args>(args)...);
  }

  template <typename U>
  static ServiceWorkerProviderInfoForWindowPtr From(const U& u) {
    return mojo::TypeConverter<ServiceWorkerProviderInfoForWindowPtr, U>::Convert(u);
  }

  template <typename U>
  U To() const {
    return mojo::TypeConverter<U, ServiceWorkerProviderInfoForWindow>::Convert(*this);
  }


  ServiceWorkerProviderInfoForWindow();

  ServiceWorkerProviderInfoForWindow(
      ::blink::mojom::ServiceWorkerContainerHostAssociatedPtrInfo host_ptr_info,
      ::blink::mojom::ServiceWorkerContainerAssociatedRequest client_request);

  ~ServiceWorkerProviderInfoForWindow();

  // Clone() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Clone() or copy
  // constructor/assignment are available for members.
  template <typename StructPtrType = ServiceWorkerProviderInfoForWindowPtr>
  ServiceWorkerProviderInfoForWindowPtr Clone() const;

  // Equals() is a template so it is only instantiated if it is used. Thus, the
  // bindings generator does not need to know whether Equals() or == operator
  // are available for members.
  template <typename T,
            typename std::enable_if<std::is_same<
                T, ServiceWorkerProviderInfoForWindow>::value>::type* = nullptr>
  bool Equals(const T& other) const;

  template <typename UserType>
  static std::vector<uint8_t> Serialize(UserType* input) {
    return mojo::internal::SerializeImpl<
        ServiceWorkerProviderInfoForWindow::DataView, std::vector<uint8_t>>(input);
  }

  template <typename UserType>
  static mojo::Message SerializeAsMessage(UserType* input) {
    return mojo::internal::SerializeAsMessageImpl<
        ServiceWorkerProviderInfoForWindow::DataView>(input);
  }

  // The returned Message is serialized only if the message is moved
  // cross-process or cross-language. Otherwise if the message is Deserialized
  // as the same UserType |input| will just be moved to |output| in
  // DeserializeFromMessage.
  template <typename UserType>
  static mojo::Message WrapAsMessage(UserType input) {
    return mojo::Message(std::make_unique<
        internal::ServiceWorkerProviderInfoForWindow_UnserializedMessageContext<
            UserType, ServiceWorkerProviderInfoForWindow::DataView>>(0, 0, std::move(input)));
  }

  template <typename UserType>
  static bool Deserialize(const void* data,
                          size_t data_num_bytes,
                          UserType* output) {
    return mojo::internal::DeserializeImpl<ServiceWorkerProviderInfoForWindow::DataView>(
        data, data_num_bytes, std::vector<mojo::ScopedHandle>(), output, Validate);
  }

  template <typename UserType>
  static bool Deserialize(const std::vector<uint8_t>& input,
                          UserType* output) {
    return ServiceWorkerProviderInfoForWindow::Deserialize(
        input.size() == 0 ? nullptr : &input.front(), input.size(), output);
  }

  template <typename UserType>
  static bool DeserializeFromMessage(mojo::Message input,
                                     UserType* output) {
    auto context = input.TakeUnserializedContext<
        internal::ServiceWorkerProviderInfoForWindow_UnserializedMessageContext<
            UserType, ServiceWorkerProviderInfoForWindow::DataView>>();
    if (context) {
      *output = std::move(context->TakeData());
      return true;
    }
    input.SerializeIfNecessary();
    return mojo::internal::DeserializeImpl<ServiceWorkerProviderInfoForWindow::DataView>(
        input.payload(), input.payload_num_bytes(),
        std::move(*input.mutable_handles()), output, Validate);
  }

  
  ::blink::mojom::ServiceWorkerContainerHostAssociatedPtrInfo host_ptr_info;
  
  ::blink::mojom::ServiceWorkerContainerAssociatedRequest client_request;

 private:
  static bool Validate(const void* data,
                       mojo::internal::ValidationContext* validation_context);
  DISALLOW_COPY_AND_ASSIGN(ServiceWorkerProviderInfoForWindow);
};

template <typename StructPtrType>
ServiceWorkerProviderInfoForWorkerPtr ServiceWorkerProviderInfoForWorker::Clone() const {
  return New(
      mojo::Clone(host_ptr_info),
      mojo::Clone(client_request)
  );
}

template <typename T,
          typename std::enable_if<std::is_same<
              T, ServiceWorkerProviderInfoForWorker>::value>::type*>
bool ServiceWorkerProviderInfoForWorker::Equals(const T& other_struct) const {
  if (!mojo::Equals(this->host_ptr_info, other_struct.host_ptr_info))
    return false;
  if (!mojo::Equals(this->client_request, other_struct.client_request))
    return false;
  return true;
}
template <typename StructPtrType>
ServiceWorkerProviderInfoForStartWorkerPtr ServiceWorkerProviderInfoForStartWorker::Clone() const {
  return New(
      mojo::Clone(host_ptr_info),
      mojo::Clone(client_request),
      mojo::Clone(script_loader_factory_ptr_info),
      mojo::Clone(cache_storage),
      mojo::Clone(interface_provider)
  );
}

template <typename T,
          typename std::enable_if<std::is_same<
              T, ServiceWorkerProviderInfoForStartWorker>::value>::type*>
bool ServiceWorkerProviderInfoForStartWorker::Equals(const T& other_struct) const {
  if (!mojo::Equals(this->host_ptr_info, other_struct.host_ptr_info))
    return false;
  if (!mojo::Equals(this->client_request, other_struct.client_request))
    return false;
  if (!mojo::Equals(this->script_loader_factory_ptr_info, other_struct.script_loader_factory_ptr_info))
    return false;
  if (!mojo::Equals(this->cache_storage, other_struct.cache_storage))
    return false;
  if (!mojo::Equals(this->interface_provider, other_struct.interface_provider))
    return false;
  return true;
}
template <typename StructPtrType>
ServiceWorkerProviderInfoForWindowPtr ServiceWorkerProviderInfoForWindow::Clone() const {
  return New(
      mojo::Clone(host_ptr_info),
      mojo::Clone(client_request)
  );
}

template <typename T,
          typename std::enable_if<std::is_same<
              T, ServiceWorkerProviderInfoForWindow>::value>::type*>
bool ServiceWorkerProviderInfoForWindow::Equals(const T& other_struct) const {
  if (!mojo::Equals(this->host_ptr_info, other_struct.host_ptr_info))
    return false;
  if (!mojo::Equals(this->client_request, other_struct.client_request))
    return false;
  return true;
}


}  // namespace mojom
}  // namespace blink

namespace mojo {


template <>
struct BLINK_COMMON_EXPORT StructTraits<::blink::mojom::ServiceWorkerProviderInfoForWorker::DataView,
                                         ::blink::mojom::ServiceWorkerProviderInfoForWorkerPtr> {
  static bool IsNull(const ::blink::mojom::ServiceWorkerProviderInfoForWorkerPtr& input) { return !input; }
  static void SetToNull(::blink::mojom::ServiceWorkerProviderInfoForWorkerPtr* output) { output->reset(); }

  static  decltype(::blink::mojom::ServiceWorkerProviderInfoForWorker::host_ptr_info)& host_ptr_info(
       ::blink::mojom::ServiceWorkerProviderInfoForWorkerPtr& input) {
    return input->host_ptr_info;
  }

  static  decltype(::blink::mojom::ServiceWorkerProviderInfoForWorker::client_request)& client_request(
       ::blink::mojom::ServiceWorkerProviderInfoForWorkerPtr& input) {
    return input->client_request;
  }

  static bool Read(::blink::mojom::ServiceWorkerProviderInfoForWorker::DataView input, ::blink::mojom::ServiceWorkerProviderInfoForWorkerPtr* output);
};


template <>
struct BLINK_COMMON_EXPORT StructTraits<::blink::mojom::ServiceWorkerProviderInfoForStartWorker::DataView,
                                         ::blink::mojom::ServiceWorkerProviderInfoForStartWorkerPtr> {
  static bool IsNull(const ::blink::mojom::ServiceWorkerProviderInfoForStartWorkerPtr& input) { return !input; }
  static void SetToNull(::blink::mojom::ServiceWorkerProviderInfoForStartWorkerPtr* output) { output->reset(); }

  static  decltype(::blink::mojom::ServiceWorkerProviderInfoForStartWorker::host_ptr_info)& host_ptr_info(
       ::blink::mojom::ServiceWorkerProviderInfoForStartWorkerPtr& input) {
    return input->host_ptr_info;
  }

  static  decltype(::blink::mojom::ServiceWorkerProviderInfoForStartWorker::client_request)& client_request(
       ::blink::mojom::ServiceWorkerProviderInfoForStartWorkerPtr& input) {
    return input->client_request;
  }

  static  decltype(::blink::mojom::ServiceWorkerProviderInfoForStartWorker::script_loader_factory_ptr_info)& script_loader_factory_ptr_info(
       ::blink::mojom::ServiceWorkerProviderInfoForStartWorkerPtr& input) {
    return input->script_loader_factory_ptr_info;
  }

  static  decltype(::blink::mojom::ServiceWorkerProviderInfoForStartWorker::cache_storage)& cache_storage(
       ::blink::mojom::ServiceWorkerProviderInfoForStartWorkerPtr& input) {
    return input->cache_storage;
  }

  static  decltype(::blink::mojom::ServiceWorkerProviderInfoForStartWorker::interface_provider)& interface_provider(
       ::blink::mojom::ServiceWorkerProviderInfoForStartWorkerPtr& input) {
    return input->interface_provider;
  }

  static bool Read(::blink::mojom::ServiceWorkerProviderInfoForStartWorker::DataView input, ::blink::mojom::ServiceWorkerProviderInfoForStartWorkerPtr* output);
};


template <>
struct BLINK_COMMON_EXPORT StructTraits<::blink::mojom::ServiceWorkerProviderInfoForWindow::DataView,
                                         ::blink::mojom::ServiceWorkerProviderInfoForWindowPtr> {
  static bool IsNull(const ::blink::mojom::ServiceWorkerProviderInfoForWindowPtr& input) { return !input; }
  static void SetToNull(::blink::mojom::ServiceWorkerProviderInfoForWindowPtr* output) { output->reset(); }

  static  decltype(::blink::mojom::ServiceWorkerProviderInfoForWindow::host_ptr_info)& host_ptr_info(
       ::blink::mojom::ServiceWorkerProviderInfoForWindowPtr& input) {
    return input->host_ptr_info;
  }

  static  decltype(::blink::mojom::ServiceWorkerProviderInfoForWindow::client_request)& client_request(
       ::blink::mojom::ServiceWorkerProviderInfoForWindowPtr& input) {
    return input->client_request;
  }

  static bool Read(::blink::mojom::ServiceWorkerProviderInfoForWindow::DataView input, ::blink::mojom::ServiceWorkerProviderInfoForWindowPtr* output);
};

}  // namespace mojo

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_SERVICE_WORKER_SERVICE_WORKER_PROVIDER_MOJOM_H_
