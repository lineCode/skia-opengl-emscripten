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

#ifndef COBALT_UI_NAVIGATION_INTERFACE_H_
#define COBALT_UI_NAVIGATION_INTERFACE_H_

#include "starboard/configuration.h"
#include "starboard/ui_navigation.h"

namespace cobalt {
namespace ui_navigation {

#if SB_API_VERSION >= SB_UI_NAVIGATION_VERSION

// Alias the starboard interface. See starboard/ui_navigation.h for details.
using NativeItem = SbUiNavItem;
using NativeItemType = SbUiNavItemType;
constexpr NativeItemType kNativeItemTypeFocus = kSbUiNavItemTypeFocus;
constexpr NativeItemType kNativeItemTypeContainer = kSbUiNavItemTypeContainer;
using NativeTransform = SbUiNavTransform;
using NativeCallbacks = SbUiNavCallbacks;
using NativeInterface = SbUiNavInterface;
#define kNativeItemInvalid kSbUiNavItemInvalid

#else

// Mimic the starboard interface. See starboard/ui_navigation.h for details.
typedef void* NativeItem;

enum NativeItemType {
  kNativeItemTypeFocus,
  kNativeItemTypeContainer,
};

struct NativeTransform {
  float m[16];
};

struct NativeCallbacks {
  void (*onblur)(NativeItem item, void* callback_context);
  void (*onfocus)(NativeItem item, void* callback_context);
  void (*onscroll)(NativeItem item, void* callback_context);
};

struct NativeInterface {
  NativeItem (*create_item)(NativeItemType type,
                            const NativeCallbacks* callbacks,
                            void* callback_context);
  void (*destroy_item)(NativeItem item);
  void (*set_focus)(NativeItem item);
  void (*set_item_enabled)(NativeItem item, bool enabled);
  void (*set_item_size)(NativeItem item, float width, float height);
  void (*set_item_position)(NativeItem item, float x, float y);
  bool (*get_item_local_transform)(NativeItem item,
      NativeTransform* out_transform);
  bool (*get_item_focus_vector)(NativeItem item, float* out_x, float* out_y);
  void (*set_item_container_window)(NativeItem item, SbWindow window);
  void (*set_item_container_item)(NativeItem item, NativeItem container);
  void (*set_item_content_offset)(NativeItem item,
      float content_offset_x, float content_offset_y);
  void (*get_item_content_offset)(NativeItem item,
      float* out_content_offset_x, float* out_content_offset_y);
};

#define kNativeItemInvalid nullptr

#endif  // SB_API_VERSION >= SB_UI_NAVIGATION_VERSION

// Retrieve the interface to use for UI navigation.
extern const NativeInterface& GetInterface();

}  // namespace ui_navigation
}  // namespace cobalt

#endif  // COBALT_UI_NAVIGATION_INTERFACE_H_
