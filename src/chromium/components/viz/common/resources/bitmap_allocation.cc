// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/viz/common/resources/bitmap_allocation.h"

#include <stdint.h>

#include "base/debug/alias.h"
#include "base/memory/read_only_shared_memory_region.h"
#include "base/process/memory.h"
#include "build/build_config.h"
#include "components/viz/common/resources/resource_format_utils.h"
#include "components/viz/common/resources/resource_sizes.h"
//#if !defined(VIZ_COMMON_PORT)
#include "mojo/public/cpp/base/shared_memory_utils.h"
#include "mojo/public/cpp/system/platform_handle.h"
//#endif // VIZ_COMMON_PORT
#include "ui/gfx/geometry/size.h"

namespace viz {

namespace {
// Collect extra information for debugging bitmap creation failures.
void CollectMemoryUsageAndDie(const gfx::Size& size,
                              ResourceFormat format,
                              size_t alloc_size) {
#if defined(OS_WIN)
  DWORD last_error = GetLastError();
  base::debug::Alias(&last_error);
#endif

  int width = size.width();
  int height = size.height();

  base::debug::Alias(&width);
  base::debug::Alias(&height);
  base::debug::Alias(&format);

  base::TerminateBecauseOutOfMemory(alloc_size);
}
}  // namespace

namespace bitmap_allocation {

base::MappedReadOnlyRegion AllocateSharedBitmap(const gfx::Size& size,
                                                ResourceFormat format) {
  DCHECK(IsBitmapFormatSupported(format));
  size_t bytes = 0;
  if (!ResourceSizes::MaybeSizeInBytes(size, format, &bytes)) {
    DLOG(ERROR) << "AllocateMappedBitmap with size that overflows";
    CollectMemoryUsageAndDie(size, format, std::numeric_limits<int>::max());
  }

  // NOTE: Need to use mojo::CreateReadOnlySharedMemoryRegion() instead of
  // base::ReadOnlySharedMemoryRegion::Create() to ensure that this always work,
  // even in sandboxed processes.
  base::MappedReadOnlyRegion shm =
      mojo::CreateReadOnlySharedMemoryRegion(bytes);
  if (!shm.IsValid()) {
    DLOG(ERROR) << "Browser failed to allocate shared memory";
    CollectMemoryUsageAndDie(size, format, bytes);
  }
  return shm;
}

mojo::ScopedSharedBufferHandle ToMojoHandle(
    base::ReadOnlySharedMemoryRegion region) {
  return mojo::WrapReadOnlySharedMemoryRegion(std::move(region));
}

base::ReadOnlySharedMemoryRegion FromMojoHandle(
    mojo::ScopedSharedBufferHandle handle) {
  return mojo::UnwrapReadOnlySharedMemoryRegion(std::move(handle));
}

}  // namespace bitmap_allocation

}  // namespace viz
