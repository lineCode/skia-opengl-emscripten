// Copyright 2019 The Cobalt Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "starboard/shared/starboard/media/media_support_internal.h"

#include "starboard/configuration.h"
#include "starboard/media.h"

SB_EXPORT bool SbMediaIsAudioSupported(SbMediaAudioCodec audio_codec,
                                       int64_t bitrate) {
  if (audio_codec == kSbMediaAudioCodecAac) {
    return bitrate <= SB_MEDIA_MAX_AUDIO_BITRATE_IN_BITS_PER_SECOND;
  }

#if SB_HAS(AUDIO_SPECIFIC_CONFIG_AS_POINTER)
  if (audio_codec == kSbMediaAudioCodecOpus) {
    return bitrate <= SB_MEDIA_MAX_AUDIO_BITRATE_IN_BITS_PER_SECOND;
  }

#if SB_HAS(AC3_AUDIO)
  if (audio_codec == kSbMediaAudioCodecAc3 ||
      audio_codec == kSbMediaAudioCodecEac3) {
    return bitrate <= SB_MEDIA_MAX_AUDIO_BITRATE_IN_BITS_PER_SECOND;
  }
#endif  // SB_HAS(AC3_AUDIO)
#endif  // SB_HAS(AUDIO_SPECIFIC_CONFIG_AS_POINTER)

  return false;
}
