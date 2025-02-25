// Copyright 2017 The Cobalt Authors. All Rights Reserved.
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

#ifndef STARBOARD_ANDROID_SHARED_COBALT_ANDROID_MEDIA_SESSION_CLIENT_H_
#define STARBOARD_ANDROID_SHARED_COBALT_ANDROID_MEDIA_SESSION_CLIENT_H_

#include "starboard/player.h"

namespace starboard {
namespace android {
namespace shared {
namespace cobalt {

// Duplicated in CobaltMediaSession.java
enum PlaybackState { kPlaying = 0, kPaused = 1, kNone = 2 };

void UpdateActiveSessionPlatformPlaybackState(PlaybackState state);

// TODO: Pass the necessary info through web MediaSession so we don't need to
// short-circuit to the player implementation to get info about the playback.
void UpdateActiveSessionPlatformPlayer(SbPlayer player);

}  // namespace cobalt
}  // namespace shared
}  // namespace android
}  // namespace starboard

#endif  // STARBOARD_ANDROID_SHARED_COBALT_ANDROID_MEDIA_SESSION_CLIENT_H_
