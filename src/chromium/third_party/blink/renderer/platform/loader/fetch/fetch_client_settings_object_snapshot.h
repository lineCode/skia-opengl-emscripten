// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_LOADER_FETCH_FETCH_CLIENT_SETTINGS_OBJECT_SNAPSHOT_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_LOADER_FETCH_FETCH_CLIENT_SETTINGS_OBJECT_SNAPSHOT_H_

#if defined(ENABLE_GNET)
#include "services/network/public/mojom/referrer_policy.mojom-shared.h"
#endif // ENABLE_GNET
#include "third_party/blink/renderer/platform/cross_thread_copier.h"
#include "third_party/blink/renderer/platform/loader/fetch/fetch_client_settings_object.h"
#include "third_party/blink/renderer/platform/platform_export.h"
#include "third_party/blink/renderer/platform/weborigin/kurl.h"
#include "third_party/blink/renderer/platform/weborigin/security_origin.h"

namespace blink {

// This class is needed to copy a FetchClientSettingsObjectSnapshot across
// threads, because it has some members which cannot be transferred across
// threads (AtomicString for example).
// There are some rules / restrictions:
//   - This struct cannot contain an object that cannot be transferred across
//     threads (e.g., AtomicString)
//   - Non-simple members need explicit copying (e.g., String::IsolatedCopy,
//     KURL::Copy) rather than the copy constructor or the assignment operator.
struct CrossThreadFetchClientSettingsObjectData {
  USING_FAST_MALLOC(CrossThreadFetchClientSettingsObjectData);

 public:
  CrossThreadFetchClientSettingsObjectData(
      KURL global_object_url,
      KURL base_url,
      scoped_refptr<const SecurityOrigin> security_origin,
      network::mojom::ReferrerPolicy referrer_policy,
      String outgoing_referrer,
      HttpsState https_state,
      AllowedByNosniff::MimeTypeCheck mime_type_check_for_classic_worker_script,
      mojom::IPAddressSpace address_space,
      WebInsecureRequestPolicy insecure_requests_policy,
      FetchClientSettingsObject::InsecureNavigationsSet
          insecure_navigations_set,
      bool mixed_autoupgrade_opt_out)
      : global_object_url(std::move(global_object_url)),
        base_url(std::move(base_url)),
        security_origin(std::move(security_origin)),
        referrer_policy(referrer_policy),
        outgoing_referrer(std::move(outgoing_referrer)),
        https_state(https_state),
        mime_type_check_for_classic_worker_script(
            mime_type_check_for_classic_worker_script),
        address_space(address_space),
        insecure_requests_policy(insecure_requests_policy),
        insecure_navigations_set(std::move(insecure_navigations_set)),
        mixed_autoupgrade_opt_out(mixed_autoupgrade_opt_out) {}

  const KURL global_object_url;
  const KURL base_url;
  const scoped_refptr<const SecurityOrigin> security_origin;
  const network::mojom::ReferrerPolicy referrer_policy;
  const String outgoing_referrer;
  const HttpsState https_state;
  const AllowedByNosniff::MimeTypeCheck
      mime_type_check_for_classic_worker_script;
  const mojom::IPAddressSpace address_space;
  const WebInsecureRequestPolicy insecure_requests_policy;
  const FetchClientSettingsObject::InsecureNavigationsSet
      insecure_navigations_set;
  const bool mixed_autoupgrade_opt_out;

 private:
  DISALLOW_COPY_AND_ASSIGN(CrossThreadFetchClientSettingsObjectData);
};

// This takes a partial snapshot of the execution context's states so that an
// instance of this class can be passed to another thread without cross-thread
// synchronization. Don't keep this object persistently, instead create a new
// instance per each "fetch a module script graph" algorithm:
// https://html.spec.whatwg.org/C/#fetch-a-module-script-tree
// https://html.spec.whatwg.org/C/#fetch-a-module-worker-script-tree
//
// This class should be used only for main worker (worklet) script loading. For
// other resources, FetchClientSettingsObjectImpl should be used. See the class
// level comments on that class.
class PLATFORM_EXPORT FetchClientSettingsObjectSnapshot final
    : public FetchClientSettingsObject {
 public:
  explicit FetchClientSettingsObjectSnapshot(const FetchClientSettingsObject&);
  explicit FetchClientSettingsObjectSnapshot(
      std::unique_ptr<CrossThreadFetchClientSettingsObjectData>);
  FetchClientSettingsObjectSnapshot(
      const KURL& global_object_url,
      const KURL& base_url,
      const scoped_refptr<const SecurityOrigin> security_origin,
      network::mojom::ReferrerPolicy referrer_policy,
      const String& outgoing_referrer,
      HttpsState https_state,
      AllowedByNosniff::MimeTypeCheck,
      mojom::IPAddressSpace,
      WebInsecureRequestPolicy,
      InsecureNavigationsSet,
      bool mixed_autoupgrade_opt_out);

  ~FetchClientSettingsObjectSnapshot() override = default;

  const KURL& GlobalObjectUrl() const override { return global_object_url_; }
  const KURL& BaseUrl() const override { return base_url_; }
  const SecurityOrigin* GetSecurityOrigin() const override {
    return security_origin_.get();
  }
  network::mojom::ReferrerPolicy GetReferrerPolicy() const override {
    return referrer_policy_;
  }
  const String GetOutgoingReferrer() const override {
    return outgoing_referrer_;
  }
  HttpsState GetHttpsState() const override { return https_state_; }

  mojom::IPAddressSpace GetAddressSpace() const override {
    return address_space_;
  }

  WebInsecureRequestPolicy GetInsecureRequestsPolicy() const override {
    return insecure_requests_policy_;
  }

  const InsecureNavigationsSet& GetUpgradeInsecureNavigationsSet()
      const override {
    return insecure_navigations_set_;
  }

  bool GetMixedAutoUpgradeOptOut() const override {
    return mixed_autoupgrade_opt_out_;
  }

  AllowedByNosniff::MimeTypeCheck MimeTypeCheckForClassicWorkerScript()
      const override {
    return mime_type_check_for_classic_worker_script_;
  }

  // Gets a copy of the data suitable for passing to another thread.
  std::unique_ptr<CrossThreadFetchClientSettingsObjectData> CopyData() const {
    return std::make_unique<CrossThreadFetchClientSettingsObjectData>(
        global_object_url_.Copy(), base_url_.Copy(),
        security_origin_->IsolatedCopy(), referrer_policy_,
        outgoing_referrer_.IsolatedCopy(), https_state_,
        mime_type_check_for_classic_worker_script_, address_space_,
        insecure_requests_policy_, insecure_navigations_set_,
        mixed_autoupgrade_opt_out_);
  }

 private:
  const KURL global_object_url_;
  const KURL base_url_;
  const scoped_refptr<const SecurityOrigin> security_origin_;
  const network::mojom::ReferrerPolicy referrer_policy_;
  const String outgoing_referrer_;
  const HttpsState https_state_;
  const AllowedByNosniff::MimeTypeCheck
      mime_type_check_for_classic_worker_script_;
  const mojom::IPAddressSpace address_space_;

  const WebInsecureRequestPolicy insecure_requests_policy_;
  const InsecureNavigationsSet insecure_navigations_set_;
  const bool mixed_autoupgrade_opt_out_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_LOADER_FETCH_FETCH_CLIENT_SETTINGS_OBJECT_SNAPSHOT_H_
