// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_PUBLIC_MOJOM_WEBAUDIO_AUDIO_CONTEXT_MANAGER_MOJOM_SHARED_MESSAGE_IDS_H_
#define THIRD_PARTY_BLINK_PUBLIC_MOJOM_WEBAUDIO_AUDIO_CONTEXT_MANAGER_MOJOM_SHARED_MESSAGE_IDS_H_

#include <stdint.h>
namespace blink {
namespace mojom {

namespace internal {


// The 185552297 value is based on sha256(salt + "AudioContextManager1").
constexpr uint32_t kAudioContextManager_AudioContextAudiblePlaybackStarted_Name = 185552297;
// The 1486109653 value is based on sha256(salt + "AudioContextManager2").
constexpr uint32_t kAudioContextManager_AudioContextAudiblePlaybackStopped_Name = 1486109653;

}  // namespace internal
}  // namespace mojom
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_PUBLIC_MOJOM_WEBAUDIO_AUDIO_CONTEXT_MANAGER_MOJOM_SHARED_MESSAGE_IDS_H_