/*
 * Copyright 2013 The Cobalt Authors. All Rights Reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "renderer_stub/rasterizer/skia/skia/src/ports/SkOSFile_cobalt.h"

#include "SkString.h"
#include "SkTFitsIn.h"
#include "SkTemplates.h"
#include "SkTypes.h"

#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/optional.h"
#include "base/path_service.h"

#include "starboard/file.h"
#include "base/files/platform_file.h"

// Implement functionality declared in SkOSFile.h via primitives provided
// by Chromium.  In doing this, we need only ensure that support for Chromium
// file utilities is working and then Skia file utilities will also work.

namespace {

SbFile ToSbFile(FILE* sk_file) {
  // PlatformFile is a pointer type in Starboard, so we cannot use static_cast
  // from intptr_t.
  return reinterpret_cast<SbFile>(sk_file);
}

FILE* ToFILE(SbFile starboard_file) {
  return reinterpret_cast<FILE*>(starboard_file);
}

int ToSbFileFlags(SkFILE_Flags sk_flags) {
  int flags = 0;
  if (sk_flags & kRead_SkFILE_Flag) {
    if (sk_flags & kWrite_SkFILE_Flag) {
      flags |= kSbFileWrite;
    }
    flags |= kSbFileOpenOnly | kSbFileRead;
  } else if (sk_flags & kWrite_SkFILE_Flag) {
    flags |= kSbFileOpenAlways | kSbFileWrite;
  }
  return flags;
}

}  // namespace

FILE* sk_fopen(const char path[], SkFILE_Flags sk_flags) {
  printf("SkOSFile_cobalt.cc: sk_fopen\n");

  SbFile starboard_file = SbFileOpen(path, ToSbFileFlags(sk_flags), NULL, NULL);
  ///if (starboard_file == base::kInvalidPlatformFile) {
  if(!SbFileIsValid(starboard_file)) {
    return nullptr;
  }

  return ToFILE(starboard_file);
}

void sk_fclose(FILE* sk_file) {
  printf("SkOSFile_cobalt.cc: sk_fclose\n");

  SkASSERT(sk_file);
  SbFileClose(ToSbFile(sk_file));
}

size_t sk_fgetsize(FILE* sk_file) {
  printf("SkOSFile_cobalt.cc: sk_fgetsize\n");

  SkASSERT(sk_file);
  SbFile file = ToSbFile(sk_file);

  // Save current position so we can restore it.
  int64_t current_position = SbFileSeek(file, kSbFileFromCurrent, 0);
  if (current_position < 0) {
    return 0;
  }

  // Find the file size by seeking to the end.
  int64_t size = SbFileSeek(file, kSbFileFromEnd, 0);
  if (size < 0) {
    size = 0;
  }

  // Restore original file position.
  SbFileSeek(file, kSbFileFromBegin, current_position);
  return size;
}

size_t sk_fwrite(const void* buffer, size_t byteCount, FILE* sk_file) {
  printf("SkOSFile_cobalt.cc: sk_fwrite\n");

  SkASSERT(sk_file);
  SbFile file = ToSbFile(sk_file);
  return SbFileWrite(file, reinterpret_cast<const char*>(buffer), byteCount);
}

void sk_fflush(FILE* sk_file) {
  printf("SkOSFile_cobalt.cc: sk_fflush\n");

  SkASSERT(sk_file);
  SbFile file = ToSbFile(sk_file);
  SbFileFlush(file);
}

bool sk_fseek(FILE* sk_file, size_t position) {
  printf("SkOSFile_cobalt.cc: sk_fseek\n");

  SkASSERT(sk_file);
  SbFile file = ToSbFile(sk_file);
  int64_t new_position = SbFileSeek(file, kSbFileFromBegin, position);
  return new_position == position;
}

size_t sk_ftell(FILE* sk_file) {
  printf("SkOSFile_cobalt.cc: sk_ftell\n");

  SkASSERT(sk_file);
  SbFile file = ToSbFile(sk_file);
  return SbFileSeek(file, kSbFileFromCurrent, 0);
}

void* sk_fmmap(FILE* sk_file, size_t* length) {
  printf("SkOSFile_cobalt.cc: sk_fmmap\n");

  // Not supported, but clients may try to call to see if it is supported.
  return NULL;
}

void* sk_fdmmap(int fd, size_t* length) {
  printf("SkOSFile_cobalt.cc: sk_fdmmap\n");

  NOTREACHED() << __FUNCTION__;
  return NULL;
}

void sk_fmunmap(const void* addr, size_t length) {
  printf("SkOSFile_cobalt.cc: sk_fmunmap\n");

  NOTREACHED() << __FUNCTION__;
}

bool sk_fidentical(FILE* sk_file_a, FILE* sk_file_b) {
  printf("SkOSFile_cobalt.cc: sk_fidentical\n");

  NOTREACHED() << __FUNCTION__;
  return false;
}

int sk_fileno(FILE* sk_file_a) {
  printf("SkOSFile_cobalt.cc: sk_fileno\n");

  NOTREACHED() << __FUNCTION__;
  return -1;
}

bool sk_exists(const char* path, SkFILE_Flags) {
  printf("SkOSFile_cobalt.cc: sk_exists\n");

  return base::PathExists(base::FilePath(path));
}

bool sk_isdir(const char* path) {
  printf("SkOSFile_cobalt.cc: sk_isdir\n");

  return base::DirectoryExists(base::FilePath(path));
}

bool sk_mkdir(const char* path) {
  printf("SkOSFile_cobalt.cc: sk_mkdir\n");

  // Strange linking error on windows when calling base::CreateDirectory,
  // having something to do with a system #define. This isn't used, anyway.
  NOTREACHED() << __FUNCTION__;
  return false;
}

void sk_fsync(FILE* f) {
  printf("SkOSFile_cobalt.cc: sk_fsync\n");

  SkASSERT(f);
  SbFile file = ToSbFile(f);
  // Technically, flush doesn't have to call sync... but this is the best
  // effort we can make.
  SbFileFlush(file);
}

size_t sk_qread(FILE* file, void* buffer, size_t count, size_t offset) {
  printf("SkOSFile_cobalt.cc: sk_qread\n");

  SkASSERT(file);
  SbFile starboard_file = ToSbFile(file);

  int original_position = SbFileSeek(starboard_file, kSbFileFromCurrent, 0);
  if (original_position < 0) {
    return SIZE_MAX;
  }

  int position = SbFileSeek(starboard_file, kSbFileFromBegin, offset);
  int result = 0;
  if (position == offset) {
    result = SbFileReadAll(starboard_file, reinterpret_cast<char*>(buffer),
                           static_cast<int>(count));
  }
  position = SbFileSeek(starboard_file, kSbFileFromBegin, original_position);
  if (result < 0 || position < 0) {
    return SIZE_MAX;
  } else {
    return result;
  }
}

size_t sk_fread(void* buffer, size_t byteCount, FILE* file) {
  printf("SkOSFile_cobalt.cc: sk_fread\n");

  SkASSERT(file);
  SbFile starboard_file = ToSbFile(file);
  return SbFileReadAll(starboard_file, reinterpret_cast<char*>(buffer),
                       byteCount);
}
