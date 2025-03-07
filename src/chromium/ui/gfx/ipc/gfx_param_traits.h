// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_IPC_GFX_PARAM_TRAITS_H_
#define UI_GFX_IPC_GFX_PARAM_TRAITS_H_

#include <string>

#if defined(ENABLE_GIPC)
#include "ipc/ipc_message_utils.h"
#include "ipc/param_traits_macros.h"
#include "ui/gfx/buffer_types.h"
#include "ui/gfx/ipc/gfx_ipc_export.h"
#include "ui/gfx/ipc/gfx_param_traits_macros.h"
#include "ui/gfx/selection_bound.h"

#if defined(OS_MACOSX) && !defined(OS_IOS)
#include "ui/gfx/mac/io_surface.h"
#endif

namespace gfx {
class Range;
}

namespace IPC {

template <>
struct GFX_IPC_EXPORT ParamTraits<gfx::Range> {
  typedef gfx::Range param_type;
  static void Write(base::Pickle* m, const param_type& p);
  static bool Read(const base::Pickle* m,
                   base::PickleIterator* iter,
                   param_type* r);
  static void Log(const param_type& p, std::string* l);
};

#if defined(OS_MACOSX) && !defined(OS_IOS)
template <>
struct GFX_IPC_EXPORT ParamTraits<gfx::ScopedRefCountedIOSurfaceMachPort> {
  typedef gfx::ScopedRefCountedIOSurfaceMachPort param_type;
  static void Write(base::Pickle* m, const param_type p);
  // Note: Read() passes ownership of the Mach send right from the IPC message
  // to the ScopedRefCountedIOSurfaceMachPort. Therefore, Read() may only be
  // called once for a given message, otherwise the singular right will be
  // managed and released by two objects.
  static bool Read(const base::Pickle* m,
                   base::PickleIterator* iter,
                   param_type* r);
  static void Log(const param_type& p, std::string* l);
};
#endif  // defined(OS_MACOSX) && !defined(OS_IOS)

template <>
struct GFX_IPC_EXPORT ParamTraits<gfx::SelectionBound> {
  typedef gfx::SelectionBound param_type;
  static void Write(base::Pickle* m, const param_type& p);
  static bool Read(const base::Pickle* m,
                   base::PickleIterator* iter,
                   param_type* r);
  static void Log(const param_type& p, std::string* l);
};

}  // namespace IPC
#endif // ENABLE_GIPC

#endif  // UI_GFX_IPC_GFX_PARAM_TRAITS_H_
