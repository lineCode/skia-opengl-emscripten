// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_NETWORK_PROXY_RESOLVER_FACTORY_MOJO_H_
#define SERVICES_NETWORK_PROXY_RESOLVER_FACTORY_MOJO_H_

#include <memory>

#include "base/component_export.h"
#include "base/macros.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "mojo/public/cpp/bindings/binding.h"
#include "net/base/completion_once_callback.h"
#include "net/proxy_resolution/proxy_resolver_factory.h"
#include "services/proxy_resolver/public/mojom/proxy_resolver.mojom.h"

namespace net {
#if defined(ENABLE_DNS)
class HostResolver;
#endif
class NetLog;
class ProxyResolverErrorObserver;
class PacFileData;
}  // namespace net

namespace network {

// Implementation of ProxyResolverFactory that connects to a Mojo service to
// create implementations of a Mojo proxy resolver to back a ProxyResolverMojo.
class COMPONENT_EXPORT(NETWORK_SERVICE) ProxyResolverFactoryMojo
    : public net::ProxyResolverFactory {
 public:
  ProxyResolverFactoryMojo(
      proxy_resolver::mojom::ProxyResolverFactoryPtr mojo_proxy_factory,
      net::HostResolver* host_resolver,
      const base::Callback<std::unique_ptr<net::ProxyResolverErrorObserver>()>&
          error_observer_factory,
      net::NetLog* net_log);
  ~ProxyResolverFactoryMojo() override;

  // ProxyResolverFactory override.
  int CreateProxyResolver(const scoped_refptr<net::PacFileData>& pac_script,
                          std::unique_ptr<net::ProxyResolver>* resolver,
                          net::CompletionOnceCallback callback,
                          std::unique_ptr<Request>* request) override;

 private:
  class Job;

  proxy_resolver::mojom::ProxyResolverFactoryPtr mojo_proxy_factory_;
  net::HostResolver* const host_resolver_;
  const base::Callback<std::unique_ptr<net::ProxyResolverErrorObserver>()>
      error_observer_factory_;
  net::NetLog* const net_log_;

  base::WeakPtrFactory<ProxyResolverFactoryMojo> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ProxyResolverFactoryMojo);
};

}  // namespace network

#endif  // SERVICES_NETWORK_PROXY_RESOLVER_FACTORY_MOJO_H_
