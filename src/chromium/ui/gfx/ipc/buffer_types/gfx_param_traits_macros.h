// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Singly or multiply-included shared traits file depending upon circumstances.
// This allows the use of IPC serialization macros in more than one IPC message
// file.
#ifndef UI_GFX_IPC_GFX_BUFFER_TYPES_PARAM_TRAITS_MACROS_H_
#define UI_GFX_IPC_GFX_BUFFER_TYPES_PARAM_TRAITS_MACROS_H_

#include "build/build_config.h"
#if defined(ENABLE_GIPC)
#include "ipc/ipc_message_macros.h"
#include "ui/gfx/buffer_types.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT GFX_IPC_BUFFER_TYPES_EXPORT

IPC_ENUM_TRAITS_MAX_VALUE(gfx::BufferFormat, gfx::BufferFormat::LAST)

IPC_ENUM_TRAITS_MAX_VALUE(gfx::BufferUsage, gfx::BufferUsage::LAST)

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT
#endif // ENABLE_GIPC

#endif  // UI_GFX_IPC_GFX_BUFFER_TYPES_PARAM_TRAITS_MACROS_H_
