// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_MOJOM_TEST_UTILS_H_
#define IPC_IPC_MOJOM_TEST_UTILS_H_

#if defined(ENABLE_GIPC)
#include "ipc/ipc.mojom.h"
#endif // ENABLE_GIPC
#include "base/component_export.h"


namespace IPC {
namespace mojom {


class COMPONENT_EXPORT(IPC_MOJOM) GenericInterfaceInterceptorForTesting : public GenericInterface {
  virtual GenericInterface* GetForwardingInterface() = 0;
};
class COMPONENT_EXPORT(IPC_MOJOM) GenericInterfaceAsyncWaiter {
 public:
  explicit GenericInterfaceAsyncWaiter(GenericInterface* proxy);
  ~GenericInterfaceAsyncWaiter();

 private:
  GenericInterface* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(GenericInterfaceAsyncWaiter);
};


class COMPONENT_EXPORT(IPC_MOJOM) ChannelInterceptorForTesting : public Channel {
  virtual Channel* GetForwardingInterface() = 0;
  void SetPeerPid(int32_t pid) override;
  void Receive(IPC::MessageView message) override;
  void GetAssociatedInterface(const std::string& name, GenericInterfaceAssociatedRequest request) override;
};
class COMPONENT_EXPORT(IPC_MOJOM) ChannelAsyncWaiter {
 public:
  explicit ChannelAsyncWaiter(Channel* proxy);
  ~ChannelAsyncWaiter();

 private:
  Channel* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(ChannelAsyncWaiter);
};


class COMPONENT_EXPORT(IPC_MOJOM) ChannelBootstrapInterceptorForTesting : public ChannelBootstrap {
  virtual ChannelBootstrap* GetForwardingInterface() = 0;
};
class COMPONENT_EXPORT(IPC_MOJOM) ChannelBootstrapAsyncWaiter {
 public:
  explicit ChannelBootstrapAsyncWaiter(ChannelBootstrap* proxy);
  ~ChannelBootstrapAsyncWaiter();

 private:
  ChannelBootstrap* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(ChannelBootstrapAsyncWaiter);
};




}  // namespace mojom
}  // namespace IPC

#endif  // IPC_IPC_MOJOM_TEST_UTILS_H_
