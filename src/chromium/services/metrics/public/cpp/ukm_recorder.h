// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_METRICS_PUBLIC_CPP_UKM_RECORDER_H_
#define SERVICES_METRICS_PUBLIC_CPP_UKM_RECORDER_H_

#include <memory>

#include "base/callback.h"
#include "base/feature_list.h"
#include "base/macros.h"
#include "base/threading/thread_checker.h"
#include "services/metrics/public/cpp/metrics_export.h"
#include "services/metrics/public/cpp/ukm_source.h"
#include "services/metrics/public/cpp/ukm_source_id.h"
#include "services/metrics/public/mojom/ukm_interface.mojom-forward.h"
#include "url/gurl.h"

class IOSChromePasswordManagerClient;
class MediaEngagementSession;
class PluginInfoHostImpl;

namespace autofill {
class TestAutofillClient;
}  // namespace autofill

namespace blink {
class Document;
}  // namespace blink

namespace cc {
class UkmManager;
}  // namespace cc

namespace content {
class CrossSiteDocumentResourceHandler;
class WebContentsImpl;
class PluginServiceImpl;
}  // namespace content

namespace download {
class DownloadUkmHelper;
}  // namespace download

namespace password_manager {
class PasswordManagerMetricsRecorder;
}  // namespace password_manager

namespace payments {
class JourneyLogger;
}  // namespace payments

namespace metrics {
class UkmRecorderInterface;
}  // namespace metrics

namespace translate {
class TranslateRankerImpl;
}  // namespace translate

namespace ukm {

class DelegatingUkmRecorder;
class TestRecordingHelper;
class UkmBackgroundRecorderService;

namespace internal {
class SourceUrlRecorderWebContentsObserver;
class SourceUrlRecorderWebStateObserver;
}  // namespace internal

// This feature controls whether UkmService should be created.
METRICS_EXPORT extern const base::Feature kUkmFeature;

// Interface for recording UKM
class METRICS_EXPORT UkmRecorder {
 public:
  UkmRecorder();
  virtual ~UkmRecorder();

  // Provides access to a global UkmRecorder instance for recording metrics.
  // This is typically passed to the Record() method of a entry object from
  // ukm_builders.h.
  // Use TestAutoSetUkmRecorder for capturing data written this way in tests.
  static UkmRecorder* Get();

  // Get the new source ID, which is unique for the duration of a browser
  // session.
  static SourceId GetNewSourceID();

  // Add an entry to the UkmEntry list.
  virtual void AddEntry(mojom::UkmEntryPtr entry) = 0;

  // Disables sampling for testing purposes.
  virtual void DisableSamplingForTesting() {}

 protected:
  // Type-safe wrappers for Update<X> functions.
  void RecordOtherURL(base::UkmSourceId source_id, const GURL& url);
  void RecordAppURL(base::UkmSourceId source_id, const GURL& url);

 private:
  friend DelegatingUkmRecorder;
  friend IOSChromePasswordManagerClient;
  friend MediaEngagementSession;
  friend PluginInfoHostImpl;
  friend TestRecordingHelper;
  friend UkmBackgroundRecorderService;
  friend autofill::TestAutofillClient;
  friend blink::Document;
  friend content::CrossSiteDocumentResourceHandler;
  friend content::PluginServiceImpl;
  friend content::WebContentsImpl;
  friend download::DownloadUkmHelper;
  friend internal::SourceUrlRecorderWebContentsObserver;
  friend internal::SourceUrlRecorderWebStateObserver;
  friend metrics::UkmRecorderInterface;
  friend password_manager::PasswordManagerMetricsRecorder;
  friend payments::JourneyLogger;
  friend translate::TranslateRankerImpl;

  // Associates the SourceId with a URL. Most UKM recording code should prefer
  // to use a shared SourceId that is already associated with a URL, rather
  // than using this API directly. New uses of this API must be auditted to
  // maintain privacy constraints.
  virtual void UpdateSourceURL(SourceId source_id, const GURL& url) = 0;

  // Associates the SourceId with an app URL for APP_ID sources. This method
  // should only be called by AppSourceUrlRecorder and DelegatingUkmRecorder.
  virtual void UpdateAppURL(SourceId source_id, const GURL& url) = 0;

  // Associates navigation data with the UkmSource keyed by |source_id|. This
  // should only be called by SourceUrlRecorderWebContentsObserver, for
  // navigation sources.
  virtual void RecordNavigation(
      SourceId source_id,
      const UkmSource::NavigationData& navigation_data) = 0;

  DISALLOW_COPY_AND_ASSIGN(UkmRecorder);
};

}  // namespace ukm

#endif  // SERVICES_METRICS_PUBLIC_CPP_UKM_RECORDER_H_
