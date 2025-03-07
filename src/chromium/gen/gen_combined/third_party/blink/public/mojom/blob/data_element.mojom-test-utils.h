// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_BLOB_DATA_ELEMENT_MOJOM_TEST_UTILS_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_BLOB_DATA_ELEMENT_MOJOM_TEST_UTILS_H_

#include "third_party/blink/public/mojom/blob/data_element.mojom.h"
#include "third_party/blink/public/common/common_export.h"


namespace blink {
namespace mojom {


class BLINK_COMMON_EXPORT BytesProviderInterceptorForTesting : public BytesProvider {
  virtual BytesProvider* GetForwardingInterface() = 0;
  void RequestAsReply(RequestAsReplyCallback callback) override;
  void RequestAsStream(mojo::ScopedDataPipeProducerHandle pipe) override;
  void RequestAsFile(uint64_t source_offset, uint64_t source_size, base::File file, uint64_t file_offset, RequestAsFileCallback callback) override;
};
class BLINK_COMMON_EXPORT BytesProviderAsyncWaiter {
 public:
  explicit BytesProviderAsyncWaiter(BytesProvider* proxy);
  ~BytesProviderAsyncWaiter();
  void RequestAsReply(
      std::vector<uint8_t>* out_data);
  void RequestAsFile(
      uint64_t source_offset, uint64_t source_size, base::File file, uint64_t file_offset, base::Optional<base::Time>* out_time_file_modified);

 private:
  BytesProvider* const proxy_;

  DISALLOW_COPY_AND_ASSIGN(BytesProviderAsyncWaiter);
};




}  // namespace mojom
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_BLOB_DATA_ELEMENT_MOJOM_TEST_UTILS_H_