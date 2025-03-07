// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_LOADER_PREVIEWS_RESOURCE_LOADING_HINTS_MOJOM_TEST_UTILS_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_LOADER_PREVIEWS_RESOURCE_LOADING_HINTS_MOJOM_TEST_UTILS_H_

#include "third_party/blink/public/mojom/loader/previews_resource_loading_hints.mojom.h"
#include "third_party/blink/public/common/common_export.h"


namespace blink {
namespace mojom {


class BLINK_COMMON_EXPORT PreviewsResourceLoadingHintsReceiverInterceptorForTesting : public PreviewsResourceLoadingHintsReceiver {
  virtual PreviewsResourceLoadingHintsReceiver* GetForwardingInterface() = 0;
  void SetResourceLoadingHints(PreviewsResourceLoadingHintsPtr previews_resource_loading_hints) override;
};
class BLINK_COMMON_EXPORT PreviewsResourceLoadingHintsReceiverAsyncWaiter {
 public:
  explicit PreviewsResourceLoadingHintsReceiverAsyncWaiter(PreviewsResourceLoadingHintsReceiver* proxy);
  ~PreviewsResourceLoadingHintsReceiverAsyncWaiter();

 private:
  PreviewsResourceLoadingHintsReceiver* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(PreviewsResourceLoadingHintsReceiverAsyncWaiter);
};




}  // namespace mojom
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_LOADER_PREVIEWS_RESOURCE_LOADING_HINTS_MOJOM_TEST_UTILS_H_