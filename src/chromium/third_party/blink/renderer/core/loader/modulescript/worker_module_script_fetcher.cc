// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/loader/modulescript/worker_module_script_fetcher.h"

#include "services/network/public/mojom/referrer_policy.mojom-shared.h"
#include "third_party/blink/renderer/core/inspector/console_message.h"
#include "third_party/blink/renderer/core/workers/worker_global_scope.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_fetcher.h"
#include "third_party/blink/renderer/platform/network/content_security_policy_response_headers.h"
#include "third_party/blink/renderer/platform/network/http_names.h"
#include "third_party/blink/renderer/platform/network/network_utils.h"
#include "third_party/blink/renderer/platform/weborigin/security_policy.h"

namespace blink {

WorkerModuleScriptFetcher::WorkerModuleScriptFetcher(
    WorkerGlobalScope* global_scope)
    : global_scope_(global_scope) {}

// https://html.spec.whatwg.org/C/#worker-processing-model
void WorkerModuleScriptFetcher::Fetch(
    FetchParameters& fetch_params,
    ResourceFetcher* fetch_client_settings_object_fetcher,
    const Modulator* modulator_for_built_in_modules,
    ModuleGraphLevel level,
    ModuleScriptFetcher::Client* client) {
  DCHECK(global_scope_->IsContextThread());
  client_ = client;
  level_ = level;

  // Step 12. "In both cases, to perform the fetch given request, perform the
  // following steps if the is top-level flag is set:" [spec text]
  // Step 12.1. "Set request's reserved client to inside settings." [spec text]
  // This is implemented in the browser process.

  // Step 12.2. "Fetch request, and asynchronously wait to run the remaining
  // steps as part of fetch's process response for the response response." [spec
  // text]
  ScriptResource::Fetch(fetch_params, fetch_client_settings_object_fetcher,
                        this, ScriptResource::kNoStreaming);
}

void WorkerModuleScriptFetcher::Trace(blink::Visitor* visitor) {
  ModuleScriptFetcher::Trace(visitor);
  visitor->Trace(client_);
  visitor->Trace(global_scope_);
}

// https://html.spec.whatwg.org/C/#worker-processing-model
void WorkerModuleScriptFetcher::NotifyFinished(Resource* resource) {
  DCHECK(global_scope_->IsContextThread());
  ClearResource();

  ScriptResource* script_resource = ToScriptResource(resource);
  HeapVector<Member<ConsoleMessage>> error_messages;
  if (!WasModuleLoadSuccessful(script_resource, &error_messages)) {
    client_->NotifyFetchFinished(base::nullopt, error_messages);
    return;
  }

  if (level_ == ModuleGraphLevel::kTopLevelModuleFetch) {
    // TODO(nhiroki, hiroshige): Access to WorkerGlobalScope in module loaders
    // is a layering violation. Also, updating WorkerGlobalScope ('module map
    // settigns object') in flight can be dangerous because module loaders may
    // refers to it. We should move these steps out of core/loader/modulescript/
    // and run them after module loading. This may require the spec change.
    // (https://crbug.com/845285)

    // Ensure redirects don't affect SecurityOrigin.
    const KURL request_url = resource->Url();
    const KURL response_url = resource->GetResponse().CurrentRequestUrl();
    if (request_url != response_url &&
        !global_scope_->GetSecurityOrigin()->IsSameSchemeHostPort(
            SecurityOrigin::Create(response_url).get())) {
      error_messages.push_back(ConsoleMessage::Create(
          mojom::ConsoleMessageSource::kSecurity,
          mojom::ConsoleMessageLevel::kError,
          "Refused to cross-origin redirects of the top-level worker script."));
      client_->NotifyFetchFinished(base::nullopt, error_messages);
      return;
    }

    auto response_referrer_policy = network::mojom::ReferrerPolicy::kDefault;
    const String response_referrer_policy_header =
        resource->GetResponse().HttpHeaderField(http_names::kReferrerPolicy);
    if (!response_referrer_policy_header.IsNull()) {
      SecurityPolicy::ReferrerPolicyFromHeaderValue(
          response_referrer_policy_header,
          kDoNotSupportReferrerPolicyLegacyKeywords, &response_referrer_policy);
    }

    // Calculate an address space from worker script's response url according to
    // the "CORS and RFC1918" spec:
    // https://wicg.github.io/cors-rfc1918/#integration-html
    //
    // Currently this implementation is not fully consistent with the spec for
    // historical reasons.
    // TODO(https://crbug.com/955213): Make this consistent with the spec.
    // TODO(https://crbug.com/955213): Move this function to a more appropriate
    // place so that this is shareable out of worker code.
    auto response_address_space = mojom::IPAddressSpace::kPublic;
    if (network_utils::IsReservedIPAddress(
            resource->GetResponse().RemoteIPAddress())) {
      response_address_space = mojom::IPAddressSpace::kPrivate;
    }
    if (SecurityOrigin::Create(response_url)->IsLocalhost())
      response_address_space = mojom::IPAddressSpace::kLocal;

    auto* response_content_security_policy =
        MakeGarbageCollected<ContentSecurityPolicy>();
    response_content_security_policy->DidReceiveHeaders(
        ContentSecurityPolicyResponseHeaders(resource->GetResponse()));

    // Step 12.3-12.6 are implemented in Initialize().
    global_scope_->Initialize(response_url, response_referrer_policy,
                              response_address_space,
                              response_content_security_policy->Headers());
  }

  ModuleScriptCreationParams params(
      script_resource->GetResponse().CurrentRequestUrl(),
      script_resource->SourceText(), script_resource->CacheHandler(),
      script_resource->GetResourceRequest().GetFetchCredentialsMode());

  // Step 12.7. "Asynchronously complete the perform the fetch steps with
  // response." [spec text]
  client_->NotifyFetchFinished(params, error_messages);
}

}  // namespace blink
