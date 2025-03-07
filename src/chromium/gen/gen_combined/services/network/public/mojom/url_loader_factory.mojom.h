// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_NETWORK_PUBLIC_MOJOM_URL_LOADER_FACTORY_MOJOM_H_
#define SERVICES_NETWORK_PUBLIC_MOJOM_URL_LOADER_FACTORY_MOJOM_H_

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
#include "services/network/public/mojom/url_loader_factory.mojom-shared.h"
#include "services/network/public/mojom/url_loader_factory.mojom-forward.h"
#include "services/network/public/mojom/mutable_network_traffic_annotation_tag.mojom-forward.h"
#include "services/network/public/mojom/url_loader.mojom-forward.h"
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




namespace network {
namespace mojom {

class URLLoaderFactoryProxy;

template <typename ImplRefTraits>
class URLLoaderFactoryStub;

class URLLoaderFactoryRequestValidator;


class  URLLoaderFactory
    : public URLLoaderFactoryInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = URLLoaderFactoryInterfaceBase;
  using Proxy_ = URLLoaderFactoryProxy;

  template <typename ImplRefTraits>
  using Stub_ = URLLoaderFactoryStub<ImplRefTraits>;

  using RequestValidator_ = URLLoaderFactoryRequestValidator;
  using ResponseValidator_ = mojo::PassThroughFilter;
  enum MethodMinVersions : uint32_t {
    kCreateLoaderAndStartMinVersion = 0,
    kCloneMinVersion = 0,
  };
  virtual ~URLLoaderFactory() {}

  
  virtual void CreateLoaderAndStart(::network::mojom::URLLoaderRequest loader, int32_t routing_id, int32_t request_id, uint32_t options
#if defined(ENABLE_GNET)
  , const network::ResourceRequest& request
#endif
  ,
  ::network::mojom::URLLoaderClientPtr client
#if defined(ENABLE_GNET)
  ,
  const net::MutableNetworkTrafficAnnotationTag& traffic_annotation
#endif
  ) = 0;

  
  virtual void Clone(URLLoaderFactoryRequest factory) = 0;
};

class  URLLoaderFactoryProxy
    : public URLLoaderFactory {
 public:
  using InterfaceType = URLLoaderFactory;

  explicit URLLoaderFactoryProxy(mojo::MessageReceiverWithResponder* receiver);
  void CreateLoaderAndStart(::network::mojom::URLLoaderRequest loader, int32_t routing_id, int32_t request_id, uint32_t options
#if defined(ENABLE_GNET)
  ,
  const network::ResourceRequest& request
#endif
  ,
  ::network::mojom::URLLoaderClientPtr client
#if defined(ENABLE_GNET)
  ,
  const net::MutableNetworkTrafficAnnotationTag& traffic_annotation
#endif
  ) final;
  void Clone(URLLoaderFactoryRequest factory) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};
class  URLLoaderFactoryStubDispatch {
 public:
  static bool Accept(URLLoaderFactory* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      URLLoaderFactory* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<URLLoaderFactory>>
class URLLoaderFactoryStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  URLLoaderFactoryStub() {}
  ~URLLoaderFactoryStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return URLLoaderFactoryStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return URLLoaderFactoryStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class  URLLoaderFactoryRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};





}  // namespace mojom
}  // namespace network

namespace mojo {

}  // namespace mojo

#endif  // SERVICES_NETWORK_PUBLIC_MOJOM_URL_LOADER_FACTORY_MOJOM_H_
