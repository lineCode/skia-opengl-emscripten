/*
 * Copyright (C) 2006 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2008, 2009, 2012 Apple Inc. All rights reserved.
 * Copyright (C) Research In Motion Limited 2011. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/core/loader/empty_clients.h"

#include <memory>
#include "cc/layers/layer.h"
#include "third_party/blink/public/platform/modules/service_worker/web_service_worker_provider.h"
#include "third_party/blink/public/platform/modules/service_worker/web_service_worker_provider_client.h"
#include "third_party/blink/public/platform/platform.h"
#include "third_party/blink/public/platform/web_application_cache_host.h"
#include "third_party/blink/public/platform/web_media_player.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/visual_viewport.h"
#include "third_party/blink/renderer/core/html/forms/color_chooser.h"
#include "third_party/blink/renderer/core/html/forms/date_time_chooser.h"
#include "third_party/blink/renderer/core/html/forms/file_chooser.h"
#include "third_party/blink/renderer/core/html/forms/html_form_element.h"
#include "third_party/blink/renderer/core/loader/document_loader.h"

namespace blink {

void FillWithEmptyClients(Page::PageClients& page_clients) {
  DEFINE_STATIC_LOCAL(Persistent<ChromeClient>, dummy_chrome_client,
                      (MakeGarbageCollected<EmptyChromeClient>()));
  page_clients.chrome_client = dummy_chrome_client;
}

class EmptyPopupMenu : public PopupMenu {
 public:
  void Show() override {}
  void Hide() override {}
  void UpdateFromElement(UpdateReason) override {}
  void DisconnectClient() override {}
};

PopupMenu* EmptyChromeClient::OpenPopupMenu(LocalFrame&, HTMLSelectElement&) {
  return MakeGarbageCollected<EmptyPopupMenu>();
}

ColorChooser* EmptyChromeClient::OpenColorChooser(LocalFrame*,
                                                  ColorChooserClient*,
                                                  const Color&) {
  return nullptr;
}

DateTimeChooser* EmptyChromeClient::OpenDateTimeChooser(
    LocalFrame* frame,
    DateTimeChooserClient*,
    const DateTimeChooserParameters&) {
  return nullptr;
}

void EmptyChromeClient::OpenTextDataListChooser(HTMLInputElement&) {}

void EmptyChromeClient::OpenFileChooser(LocalFrame*,
                                        scoped_refptr<FileChooser>) {}

void EmptyChromeClient::AttachRootGraphicsLayer(GraphicsLayer* layer,
                                                LocalFrame* local_root) {
  Page* page = local_root ? local_root->GetPage() : nullptr;
  if (!page)
    return;
  page->GetVisualViewport().AttachLayerTree(layer);
}

void EmptyChromeClient::AttachRootLayer(scoped_refptr<cc::Layer>, LocalFrame*) {
}

String EmptyChromeClient::AcceptLanguages() {
  return String();
}

void EmptyLocalFrameClient::BeginNavigation(
    const ResourceRequest&,
    network::mojom::RequestContextFrameType,
    Document* origin_document,
    DocumentLoader*,
    WebNavigationType,
    NavigationPolicy,
    bool,
    WebFrameLoadType,
    bool,
    WebTriggeringEventInfo,
    HTMLFormElement*,
    ContentSecurityPolicyDisposition,
    mojom::blink::BlobURLTokenPtr,
    base::TimeTicks,
    const String&,
    WebContentSecurityPolicyList,
    mojom::blink::NavigationInitiatorPtr) {}

void EmptyLocalFrameClient::DispatchWillSendSubmitEvent(HTMLFormElement*) {}

DocumentLoader* EmptyLocalFrameClient::CreateDocumentLoader(
    LocalFrame* frame,
    WebNavigationType navigation_type,
    std::unique_ptr<WebNavigationParams> navigation_params,
    std::unique_ptr<WebDocumentLoader::ExtraData> extra_data) {
  DCHECK(frame);
  return MakeGarbageCollected<DocumentLoader>(frame, navigation_type,
                                              std::move(navigation_params));
}

mojom::blink::DocumentInterfaceBroker*
EmptyLocalFrameClient::GetDocumentInterfaceBroker() {
  if (!document_interface_broker_.is_bound())
    mojo::MakeRequest(&document_interface_broker_);
  return document_interface_broker_.get();
}

mojo::ScopedMessagePipeHandle
EmptyLocalFrameClient::SetDocumentInterfaceBrokerForTesting(
    mojo::ScopedMessagePipeHandle blink_handle) {
  mojom::blink::DocumentInterfaceBrokerPtr test_broker(
      mojom::blink::DocumentInterfaceBrokerPtrInfo(
          std::move(blink_handle),
          mojom::blink::DocumentInterfaceBroker::Version_));

  mojo::ScopedMessagePipeHandle real_handle =
      document_interface_broker_.PassInterface().PassHandle();
  document_interface_broker_ = std::move(test_broker);

  return real_handle;
}

LocalFrame* EmptyLocalFrameClient::CreateFrame(const AtomicString&,
                                               HTMLFrameOwnerElement*) {
  return nullptr;
}

std::pair<RemoteFrame*, base::UnguessableToken>
EmptyLocalFrameClient::CreatePortal(
    HTMLPortalElement*,
    mojom::blink::PortalAssociatedRequest,
    mojom::blink::PortalClientAssociatedPtrInfo) {
  return std::pair<RemoteFrame*, base::UnguessableToken>(
      nullptr, base::UnguessableToken());
}

RemoteFrame* EmptyLocalFrameClient::AdoptPortal(HTMLPortalElement*) {
  return nullptr;
}

WebPluginContainerImpl* EmptyLocalFrameClient::CreatePlugin(
    HTMLPlugInElement&,
    const KURL&,
    const Vector<String>&,
    const Vector<String>&,
    const String&,
    bool) {
  return nullptr;
}

std::unique_ptr<WebMediaPlayer> EmptyLocalFrameClient::CreateWebMediaPlayer(
    HTMLMediaElement&,
    const WebMediaPlayerSource&,
    WebMediaPlayerClient*,
    WebLayerTreeView*) {
  return nullptr;
}

WebRemotePlaybackClient* EmptyLocalFrameClient::CreateWebRemotePlaybackClient(
    HTMLMediaElement&) {
  return nullptr;
}

WebTextCheckClient* EmptyLocalFrameClient::GetTextCheckerClient() const {
  return text_check_client_;
}

void EmptyLocalFrameClient::SetTextCheckerClientForTesting(
    WebTextCheckClient* client) {
  text_check_client_ = client;
}

Frame* EmptyLocalFrameClient::FindFrame(const AtomicString& name) const {
  return nullptr;
}

std::unique_ptr<WebServiceWorkerProvider>
EmptyLocalFrameClient::CreateServiceWorkerProvider() {
  return nullptr;
}

std::unique_ptr<WebApplicationCacheHost>
EmptyLocalFrameClient::CreateApplicationCacheHost(
    DocumentLoader*,
    WebApplicationCacheHostClient*) {
  return nullptr;
}

EmptyRemoteFrameClient::EmptyRemoteFrameClient() = default;

}  // namespace blink
