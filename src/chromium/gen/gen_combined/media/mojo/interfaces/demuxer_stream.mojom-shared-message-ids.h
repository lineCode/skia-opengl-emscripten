// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MOJO_INTERFACES_DEMUXER_STREAM_MOJOM_SHARED_MESSAGE_IDS_H_
#define MEDIA_MOJO_INTERFACES_DEMUXER_STREAM_MOJOM_SHARED_MESSAGE_IDS_H_

#include <stdint.h>
namespace media {
namespace mojom {

namespace internal {


// The 1637726428 value is based on sha256(salt + "DemuxerStream1").
constexpr uint32_t kDemuxerStream_Initialize_Name = 1637726428;
// The 317593736 value is based on sha256(salt + "DemuxerStream2").
constexpr uint32_t kDemuxerStream_Read_Name = 317593736;
// The 667176591 value is based on sha256(salt + "DemuxerStream3").
constexpr uint32_t kDemuxerStream_EnableBitstreamConverter_Name = 667176591;

}  // namespace internal
}  // namespace mojom
}  // namespace media

#endif  // MEDIA_MOJO_INTERFACES_DEMUXER_STREAM_MOJOM_SHARED_MESSAGE_IDS_H_