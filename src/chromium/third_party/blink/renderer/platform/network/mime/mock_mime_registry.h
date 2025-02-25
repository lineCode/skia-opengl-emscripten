// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_NETWORK_MIME_MOCK_MIME_REGISTRY_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_NETWORK_MIME_MOCK_MIME_REGISTRY_H_

#if defined(ENABLE_GNET)
#include "net/base/mime_util.h"
#endif // ENABLE_GNET
#include "third_party/blink/public/mojom/mime/mime_registry.mojom-blink.h"
#include "third_party/blink/public/platform/file_path_conversion.h"

namespace blink {

// Used for unit tests.
class MockMimeRegistry : public mojom::blink::MimeRegistry {
 public:
  MockMimeRegistry() = default;
  ~MockMimeRegistry() override = default;

  void GetMimeTypeFromExtension(
      const String& ext,
      GetMimeTypeFromExtensionCallback callback) override {
    std::string mime_type;
    net::GetMimeTypeFromExtension(WebStringToFilePath(ext).value(), &mime_type);
    std::move(callback).Run(
        String::FromUTF8(mime_type.data(), mime_type.length()));
  }
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_NETWORK_MIME_MOCK_MIME_REGISTRY_H_
