// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MOJO_INTERFACES_PROVISION_FETCHER_MOJOM_BLINK_H_
#define MEDIA_MOJO_INTERFACES_PROVISION_FETCHER_MOJOM_BLINK_H_

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
#include "media/mojo/interfaces/provision_fetcher.mojom-shared.h"
#include "media/mojo/interfaces/provision_fetcher.mojom-blink-forward.h"

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
#include "third_party/blink/public/platform/web_common.h"




namespace media {
namespace mojom {
namespace blink {

class ProvisionFetcherProxy;

template <typename ImplRefTraits>
class ProvisionFetcherStub;

class ProvisionFetcherRequestValidator;
class ProvisionFetcherResponseValidator;


class BLINK_PLATFORM_EXPORT ProvisionFetcher
    : public ProvisionFetcherInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = ProvisionFetcherInterfaceBase;
  using Proxy_ = ProvisionFetcherProxy;

  template <typename ImplRefTraits>
  using Stub_ = ProvisionFetcherStub<ImplRefTraits>;

  using RequestValidator_ = ProvisionFetcherRequestValidator;
  using ResponseValidator_ = ProvisionFetcherResponseValidator;
  enum MethodMinVersions : uint32_t {
    kRetrieveMinVersion = 0,
  };
  virtual ~ProvisionFetcher() {}


  using RetrieveCallback = base::OnceCallback<void(bool, const WTF::String&)>;
  
  virtual void Retrieve(const WTF::String& default_url, const WTF::String& request_data, RetrieveCallback callback) = 0;
};

class BLINK_PLATFORM_EXPORT ProvisionFetcherProxy
    : public ProvisionFetcher {
 public:
  using InterfaceType = ProvisionFetcher;

  explicit ProvisionFetcherProxy(mojo::MessageReceiverWithResponder* receiver);
  void Retrieve(const WTF::String& default_url, const WTF::String& request_data, RetrieveCallback callback) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};
class BLINK_PLATFORM_EXPORT ProvisionFetcherStubDispatch {
 public:
  static bool Accept(ProvisionFetcher* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      ProvisionFetcher* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<ProvisionFetcher>>
class ProvisionFetcherStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  ProvisionFetcherStub() {}
  ~ProvisionFetcherStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return ProvisionFetcherStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return ProvisionFetcherStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class BLINK_PLATFORM_EXPORT ProvisionFetcherRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class BLINK_PLATFORM_EXPORT ProvisionFetcherResponseValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};





}  // namespace blink
}  // namespace mojom
}  // namespace media

namespace mojo {

}  // namespace mojo

#endif  // MEDIA_MOJO_INTERFACES_PROVISION_FETCHER_MOJOM_BLINK_H_