/*
 * Copyright (C) 2014 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THIRD_PARTY_BLINK_PUBLIC_PLATFORM_MODULES_SERVICE_WORKER_WEB_SERVICE_WORKER_NETWORK_PROVIDER_H_
#define THIRD_PARTY_BLINK_PUBLIC_PLATFORM_MODULES_SERVICE_WORKER_WEB_SERVICE_WORKER_NETWORK_PROVIDER_H_

#include <memory>

#include "base/memory/scoped_refptr.h"
#include "third_party/blink/public/mojom/service_worker/controller_service_worker_mode.mojom-shared.h"
#include "third_party/blink/public/platform/scheduler/web_resource_loading_task_runner_handle.h"
#include "third_party/blink/public/platform/web_url_loader.h"

namespace blink {

#if defined(ENABLE_GNET)
class WebURLRequest;
#endif // ENABLE_GNET

// This interface allows the Blink embedder to implement loading functionality
// related to service workers. It is consulted whenever a DocumentLoader makes a
// request.
//
// It is owned by DocumentLoader and only used on the main thread.
//
// Currently the Blink embedder has implementations for service worker clients
// (frames and shared workers), and service workers themselves. Note that for
// workers, the interface is only used for requests from the shadow page
// (WorkerShadowPage). For hooking into off-the-main-thread loading from
// workers, the embedder can implement WebWorkerFetchContext.
class WebServiceWorkerNetworkProvider {
 public:
  virtual ~WebServiceWorkerNetworkProvider() = default;

  // A request is about to be sent out, and the embedder may modify it. The
  // request is writable, and changes to the URL, for example, will change the
  // request made.
  virtual void WillSendRequest(WebURLRequest&) = 0;

  // Returns a URLLoader for loading |request|. May return nullptr to fall back
  // to the default loading behavior.
  virtual std::unique_ptr<WebURLLoader> CreateURLLoader(
      const WebURLRequest& request,
      std::unique_ptr<scheduler::WebResourceLoadingTaskRunnerHandle>) = 0;

  // For service worker clients.
  virtual blink::mojom::ControllerServiceWorkerMode
  IsControlledByServiceWorker() = 0;

  // For service worker clients. Returns an identifier of the controller service
  // worker associated with the loading context.
  virtual int64_t ControllerServiceWorkerID() = 0;

  // For service worker clients. Called when IdlenessDetector emits its network
  // idle signal.
  virtual void DispatchNetworkQuiet() = 0;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_PUBLIC_PLATFORM_MODULES_SERVICE_WORKER_WEB_SERVICE_WORKER_NETWORK_PROVIDER_H_
