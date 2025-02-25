// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/loader/frame_load_request.h"

#include "third_party/blink/public/common/blob/blob_utils.h"
#include "third_party/blink/public/platform/web_url_request.h"
#include "third_party/blink/renderer/core/events/current_input_event.h"
#include "third_party/blink/renderer/core/fileapi/public_url_manager.h"
#include "third_party/blink/renderer/core/frame/csp/content_security_policy.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_request.h"
#include "third_party/blink/renderer/platform/weborigin/security_policy.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {

static void SetReferrerForRequest(Document* origin_document,
                                  ResourceRequest& request) {
  DCHECK(origin_document);

  // Always use the initiating document to generate the referrer. We need to
  // generateReferrer(), because we haven't enforced
  // network::mojom::ReferrerPolicy or https->http referrer suppression yet.
  String referrer_to_use = request.ReferrerString();
  network::mojom::ReferrerPolicy referrer_policy_to_use =
      request.GetReferrerPolicy();

  if (referrer_to_use == Referrer::ClientReferrerString())
    referrer_to_use = origin_document->OutgoingReferrer();

  if (referrer_policy_to_use == network::mojom::ReferrerPolicy::kDefault)
    referrer_policy_to_use = origin_document->GetReferrerPolicy();

  Referrer referrer = SecurityPolicy::GenerateReferrer(
      referrer_policy_to_use, request.Url(), referrer_to_use);

  // TODO(domfarolino): Stop storing ResourceRequest's generated referrer as a
  // header and instead use a separate member. See https://crbug.com/850813.
  request.SetHttpReferrer(referrer);
  request.SetHTTPOriginToMatchReferrerIfNeeded();
}

FrameLoadRequest::FrameLoadRequest(Document* origin_document,
                                   const ResourceRequest& resource_request)
    : FrameLoadRequest(origin_document, resource_request, AtomicString()) {}

FrameLoadRequest::FrameLoadRequest(Document* origin_document,
                                   const ResourceRequest& resource_request,
                                   const AtomicString& frame_name)
    : origin_document_(origin_document),
      resource_request_(resource_request),
      frame_name_(frame_name),
      client_redirect_(ClientRedirectPolicy::kNotClientRedirect),
      should_send_referrer_(kMaybeSendReferrer) {
  // These flags are passed to a service worker which controls the page.
  resource_request_.SetFetchRequestMode(
      network::mojom::FetchRequestMode::kNavigate);
  resource_request_.SetFetchCredentialsMode(
      network::mojom::FetchCredentialsMode::kInclude);
  resource_request_.SetFetchRedirectMode(
      network::mojom::FetchRedirectMode::kManual);

  if (const WebInputEvent* input_event = CurrentInputEvent::Get())
    SetInputStartTime(input_event->TimeStamp());

  should_check_main_world_content_security_policy_ =
      origin_document &&
              ContentSecurityPolicy::ShouldBypassMainWorld(origin_document)
          ? kDoNotCheckContentSecurityPolicy
          : kCheckContentSecurityPolicy;

  if (origin_document) {
    DCHECK(!resource_request_.RequestorOrigin());
    resource_request_.SetRequestorOrigin(origin_document->GetSecurityOrigin());

    if (resource_request.Url().ProtocolIs("blob") &&
        BlobUtils::MojoBlobURLsEnabled()) {
      blob_url_token_ = base::MakeRefCounted<
          base::RefCountedData<mojom::blink::BlobURLTokenPtr>>();
      origin_document->GetPublicURLManager().Resolve(
          resource_request.Url(), MakeRequest(&blob_url_token_->data));
    }

    SetReferrerForRequest(origin_document_, resource_request_);
  }
}

}  // namespace blink
