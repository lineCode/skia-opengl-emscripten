// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_FETCH_FETCH_API_REQUEST_MOJOM_BLINK_FORWARD_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_FETCH_FETCH_API_REQUEST_MOJOM_BLINK_FORWARD_H_

#include "mojo/public/cpp/bindings/struct_ptr.h"


#include "mojo/public/cpp/bindings/lib/buffer.h"


#include "mojo/public/cpp/bindings/lib/native_enum_serialization.h"
#include "mojo/public/cpp/bindings/lib/native_struct_serialization.h"
#include "third_party/blink/renderer/platform/platform_export.h"


namespace blink {
namespace mojom {

enum class RequestContextType : int32_t;

enum class FetchCacheMode : int32_t;

enum class FetchImportanceMode : int32_t;
}  // namespace blink
}  // namespace mojom
#include "third_party/blink/renderer/platform/wtf/hash_functions.h"

namespace WTF {
struct blink_mojom_internal_RequestContextType_DataHashFn;

template <>
struct DefaultHash<::blink::mojom::RequestContextType> {
  using Hash = blink_mojom_internal_RequestContextType_DataHashFn;
};
}  // namespace WTF

namespace WTF {
struct blink_mojom_internal_FetchCacheMode_DataHashFn;

template <>
struct DefaultHash<::blink::mojom::FetchCacheMode> {
  using Hash = blink_mojom_internal_FetchCacheMode_DataHashFn;
};
}  // namespace WTF

namespace WTF {
struct blink_mojom_internal_FetchImportanceMode_DataHashFn;

template <>
struct DefaultHash<::blink::mojom::FetchImportanceMode> {
  using Hash = blink_mojom_internal_FetchImportanceMode_DataHashFn;
};
}  // namespace WTF


namespace blink {
namespace mojom {
namespace blink {
using RequestContextType = RequestContextType;  // Alias for definition in the parent namespace.
using FetchCacheMode = FetchCacheMode;  // Alias for definition in the parent namespace.
using FetchImportanceMode = FetchImportanceMode;  // Alias for definition in the parent namespace.
class FetchAPIRequestHeaders;
using FetchAPIRequestHeadersPtr = mojo::StructPtr<FetchAPIRequestHeaders>;

class FetchAPIRequest;
using FetchAPIRequestPtr = mojo::StructPtr<FetchAPIRequest>;




}  // namespace blink
}  // namespace mojom
}  // namespace blink
#include "third_party/blink/renderer/platform/wtf/hash_map.h"
#include "third_party/blink/renderer/platform/wtf/text/string_hash.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_FETCH_FETCH_API_REQUEST_MOJOM_BLINK_FORWARD_H_