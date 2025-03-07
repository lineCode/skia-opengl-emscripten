// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4056)
#pragma warning(disable:4065)
#pragma warning(disable:4756)
#endif


#include "third_party/blink/public/mojom/document_metadata/copyless_paste.mojom-test-utils.h"

#include <utility>

#include "base/bind.h"
#include "base/run_loop.h"
#include "url/mojom/url.mojom.h"


#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_DOCUMENT_METADATA_COPYLESS_PASTE_MOJOM_JUMBO_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_DOCUMENT_METADATA_COPYLESS_PASTE_MOJOM_JUMBO_H_
#include "url/mojom/url_gurl_mojom_traits.h"
#endif


namespace blink {
namespace mojom {
namespace document_metadata {


void CopylessPasteInterceptorForTesting::GetEntities(GetEntitiesCallback callback) {
  GetForwardingInterface()->GetEntities(std::move(callback));
}
CopylessPasteAsyncWaiter::CopylessPasteAsyncWaiter(
    CopylessPaste* proxy) : proxy_(proxy) {}

CopylessPasteAsyncWaiter::~CopylessPasteAsyncWaiter() = default;

void CopylessPasteAsyncWaiter::GetEntities(
    WebPagePtr* out_page) {
  base::RunLoop loop;
  proxy_->GetEntities(
      base::BindOnce(
          [](base::RunLoop* loop,
             WebPagePtr* out_page
,
             WebPagePtr page) {*out_page = std::move(page);
            loop->Quit();
          },
          &loop,
          out_page));
  loop.Run();
}





}  // namespace document_metadata
}  // namespace mojom
}  // namespace blink

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif