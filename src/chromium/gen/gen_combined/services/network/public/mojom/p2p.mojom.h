// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_NETWORK_PUBLIC_MOJOM_P2P_MOJOM_H_
#define SERVICES_NETWORK_PUBLIC_MOJOM_P2P_MOJOM_H_

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
#include "services/network/public/mojom/p2p.mojom-shared.h"
#include "services/network/public/mojom/p2p.mojom-forward.h"
#include "mojo/public/mojom/base/time.mojom-forward.h"
#include "services/network/public/mojom/network_interface.mojom-forward.h"
#include "services/network/public/mojom/ip_address.mojom-forward.h"
#include "services/network/public/mojom/ip_endpoint.mojom-forward.h"
#include "services/network/public/mojom/mutable_network_traffic_annotation_tag.mojom-forward.h"
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
#include "services/network/public/cpp/p2p_socket_type.h"




namespace network {
namespace mojom {

class P2PNetworkNotificationClientProxy;

template <typename ImplRefTraits>
class P2PNetworkNotificationClientStub;

class P2PNetworkNotificationClientRequestValidator;


class  P2PNetworkNotificationClient
    : public P2PNetworkNotificationClientInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = P2PNetworkNotificationClientInterfaceBase;
  using Proxy_ = P2PNetworkNotificationClientProxy;

  template <typename ImplRefTraits>
  using Stub_ = P2PNetworkNotificationClientStub<ImplRefTraits>;

  using RequestValidator_ = P2PNetworkNotificationClientRequestValidator;
  using ResponseValidator_ = mojo::PassThroughFilter;
  enum MethodMinVersions : uint32_t {
    kNetworkListChangedMinVersion = 0,
  };
  virtual ~P2PNetworkNotificationClient() {}

  
  virtual void NetworkListChanged(const std::vector<net::NetworkInterface>& networks, const net::IPAddress& default_ipv4_local_address, const net::IPAddress& default_ipv6_local_address) = 0;
};

class P2PSocketManagerProxy;

template <typename ImplRefTraits>
class P2PSocketManagerStub;

class P2PSocketManagerRequestValidator;
class P2PSocketManagerResponseValidator;


class  P2PSocketManager
    : public P2PSocketManagerInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = P2PSocketManagerInterfaceBase;
  using Proxy_ = P2PSocketManagerProxy;

  template <typename ImplRefTraits>
  using Stub_ = P2PSocketManagerStub<ImplRefTraits>;

  using RequestValidator_ = P2PSocketManagerRequestValidator;
  using ResponseValidator_ = P2PSocketManagerResponseValidator;
  enum MethodMinVersions : uint32_t {
    kStartNetworkNotificationsMinVersion = 0,
    kGetHostAddressMinVersion = 0,
    kCreateSocketMinVersion = 0,
  };
  virtual ~P2PSocketManager() {}

  
  virtual void StartNetworkNotifications(P2PNetworkNotificationClientPtr client) = 0;


  using GetHostAddressCallback = base::OnceCallback<void(const std::vector<net::IPAddress>&)>;
  
  virtual void GetHostAddress(const std::string& host_name, bool enable_mdns, GetHostAddressCallback callback) = 0;

  
  virtual void CreateSocket(network::P2PSocketType type, const net::IPEndPoint& local_address, const network::P2PPortRange& port_range, const network::P2PHostAndIPEndPoint& remote_address, P2PSocketClientPtr client, P2PSocketRequest socket) = 0;
};

class P2PSocketProxy;

template <typename ImplRefTraits>
class P2PSocketStub;

class P2PSocketRequestValidator;


class  P2PSocket
    : public P2PSocketInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = P2PSocketInterfaceBase;
  using Proxy_ = P2PSocketProxy;

  template <typename ImplRefTraits>
  using Stub_ = P2PSocketStub<ImplRefTraits>;

  using RequestValidator_ = P2PSocketRequestValidator;
  using ResponseValidator_ = mojo::PassThroughFilter;
  enum MethodMinVersions : uint32_t {
    kSendMinVersion = 0,
    kSetOptionMinVersion = 0,
  };
  virtual ~P2PSocket() {}

  
  virtual void Send(const std::vector<int8_t>& data, const network::P2PPacketInfo& packet_info, const net::MutableNetworkTrafficAnnotationTag& traffic_annotation) = 0;

  
  virtual void SetOption(network::P2PSocketOption option, int32_t value) = 0;
};

class P2PSocketClientProxy;

template <typename ImplRefTraits>
class P2PSocketClientStub;

class P2PSocketClientRequestValidator;


class  P2PSocketClient
    : public P2PSocketClientInterfaceBase {
 public:
  static const char Name_[];
  static constexpr uint32_t Version_ = 0;
  static constexpr bool PassesAssociatedKinds_ = false;
  static constexpr bool HasSyncMethods_ = false;

  using Base_ = P2PSocketClientInterfaceBase;
  using Proxy_ = P2PSocketClientProxy;

  template <typename ImplRefTraits>
  using Stub_ = P2PSocketClientStub<ImplRefTraits>;

  using RequestValidator_ = P2PSocketClientRequestValidator;
  using ResponseValidator_ = mojo::PassThroughFilter;
  enum MethodMinVersions : uint32_t {
    kSocketCreatedMinVersion = 0,
    kSendCompleteMinVersion = 0,
    kIncomingTcpConnectionMinVersion = 0,
    kDataReceivedMinVersion = 0,
  };
  virtual ~P2PSocketClient() {}

  
  virtual void SocketCreated(const net::IPEndPoint& local_address, const net::IPEndPoint& remote_address) = 0;

  
  virtual void SendComplete(const network::P2PSendPacketMetrics& send_metrics) = 0;

  
  virtual void IncomingTcpConnection(const net::IPEndPoint& socket_address, P2PSocketPtr socket, P2PSocketClientRequest client) = 0;

  
  virtual void DataReceived(const net::IPEndPoint& socket_address, const std::vector<int8_t>& data, base::TimeTicks timestamp) = 0;
};

class  P2PNetworkNotificationClientProxy
    : public P2PNetworkNotificationClient {
 public:
  using InterfaceType = P2PNetworkNotificationClient;

  explicit P2PNetworkNotificationClientProxy(mojo::MessageReceiverWithResponder* receiver);
  void NetworkListChanged(const std::vector<net::NetworkInterface>& networks, const net::IPAddress& default_ipv4_local_address, const net::IPAddress& default_ipv6_local_address) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};

class  P2PSocketManagerProxy
    : public P2PSocketManager {
 public:
  using InterfaceType = P2PSocketManager;

  explicit P2PSocketManagerProxy(mojo::MessageReceiverWithResponder* receiver);
  void StartNetworkNotifications(P2PNetworkNotificationClientPtr client) final;
  void GetHostAddress(const std::string& host_name, bool enable_mdns, GetHostAddressCallback callback) final;
  void CreateSocket(network::P2PSocketType type, const net::IPEndPoint& local_address, const network::P2PPortRange& port_range, const network::P2PHostAndIPEndPoint& remote_address, P2PSocketClientPtr client, P2PSocketRequest socket) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};

class  P2PSocketProxy
    : public P2PSocket {
 public:
  using InterfaceType = P2PSocket;

  explicit P2PSocketProxy(mojo::MessageReceiverWithResponder* receiver);
  void Send(const std::vector<int8_t>& data, const network::P2PPacketInfo& packet_info, const net::MutableNetworkTrafficAnnotationTag& traffic_annotation) final;
  void SetOption(network::P2PSocketOption option, int32_t value) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};

class  P2PSocketClientProxy
    : public P2PSocketClient {
 public:
  using InterfaceType = P2PSocketClient;

  explicit P2PSocketClientProxy(mojo::MessageReceiverWithResponder* receiver);
  void SocketCreated(const net::IPEndPoint& local_address, const net::IPEndPoint& remote_address) final;
  void SendComplete(const network::P2PSendPacketMetrics& send_metrics) final;
  void IncomingTcpConnection(const net::IPEndPoint& socket_address, P2PSocketPtr socket, P2PSocketClientRequest client) final;
  void DataReceived(const net::IPEndPoint& socket_address, const std::vector<int8_t>& data, base::TimeTicks timestamp) final;

 private:
  mojo::MessageReceiverWithResponder* receiver_;
};
class  P2PNetworkNotificationClientStubDispatch {
 public:
  static bool Accept(P2PNetworkNotificationClient* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      P2PNetworkNotificationClient* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<P2PNetworkNotificationClient>>
class P2PNetworkNotificationClientStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  P2PNetworkNotificationClientStub() {}
  ~P2PNetworkNotificationClientStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return P2PNetworkNotificationClientStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return P2PNetworkNotificationClientStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class  P2PSocketManagerStubDispatch {
 public:
  static bool Accept(P2PSocketManager* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      P2PSocketManager* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<P2PSocketManager>>
class P2PSocketManagerStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  P2PSocketManagerStub() {}
  ~P2PSocketManagerStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return P2PSocketManagerStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return P2PSocketManagerStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class  P2PSocketStubDispatch {
 public:
  static bool Accept(P2PSocket* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      P2PSocket* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<P2PSocket>>
class P2PSocketStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  P2PSocketStub() {}
  ~P2PSocketStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return P2PSocketStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return P2PSocketStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class  P2PSocketClientStubDispatch {
 public:
  static bool Accept(P2PSocketClient* impl, mojo::Message* message);
  static bool AcceptWithResponder(
      P2PSocketClient* impl,
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder);
};

template <typename ImplRefTraits =
              mojo::RawPtrImplRefTraits<P2PSocketClient>>
class P2PSocketClientStub
    : public mojo::MessageReceiverWithResponderStatus {
 public:
  using ImplPointerType = typename ImplRefTraits::PointerType;

  P2PSocketClientStub() {}
  ~P2PSocketClientStub() override {}

  void set_sink(ImplPointerType sink) { sink_ = std::move(sink); }
  ImplPointerType& sink() { return sink_; }

  bool Accept(mojo::Message* message) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return P2PSocketClientStubDispatch::Accept(
        ImplRefTraits::GetRawPointer(&sink_), message);
  }

  bool AcceptWithResponder(
      mojo::Message* message,
      std::unique_ptr<mojo::MessageReceiverWithStatus> responder) override {
    if (ImplRefTraits::IsNull(sink_))
      return false;
    return P2PSocketClientStubDispatch::AcceptWithResponder(
        ImplRefTraits::GetRawPointer(&sink_), message, std::move(responder));
  }

 private:
  ImplPointerType sink_;
};
class  P2PNetworkNotificationClientRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class  P2PSocketManagerRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class  P2PSocketRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class  P2PSocketClientRequestValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};
class  P2PSocketManagerResponseValidator : public mojo::MessageReceiver {
 public:
  bool Accept(mojo::Message* message) override;
};













}  // namespace mojom
}  // namespace network

namespace mojo {

}  // namespace mojo

#endif  // SERVICES_NETWORK_PUBLIC_MOJOM_P2P_MOJOM_H_