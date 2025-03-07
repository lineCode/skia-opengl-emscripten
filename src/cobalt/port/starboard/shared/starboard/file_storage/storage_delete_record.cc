// Copyright 2016 The Cobalt Authors. All Rights Reserved.
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

#include "starboard/storage.h"

#include "starboard/file.h"
#include "starboard/shared/starboard/file_storage/storage_internal.h"
#include "starboard/user.h"

bool SbStorageDeleteRecord(SbUser user
#if SB_API_VERSION >= 6
                           ,
                           const char* name
#endif  // SB_API_VERSION >= 6
                           ) {
  if (!SbUserIsValid(user)) {
    return false;
  }

#if SB_API_VERSION < 6
  const char* name = NULL;
#endif  // SB_API_VERSION < 6

  char path[SB_FILE_MAX_PATH];
  bool success = starboard::shared::starboard::GetUserStorageFilePath(
      user, name, path, SB_ARRAY_SIZE_INT(path));
  if (!success) {
    return false;
  }

  return SbFileDelete(path);
}
