﻿cmake_minimum_required(VERSION 2.8)

# 'dependencies': [
#   '<(DEPTH)/cobalt/base/base.gyp:base',
# ],
set(COBALT_ui_navigation_SOURCES
  ${COBALT_CORE_DIR}ui_navigation/interface.cc
  ${COBALT_CORE_DIR}ui_navigation/interface.h
  ${COBALT_CORE_DIR}ui_navigation/nav_item.cc
  ${COBALT_CORE_DIR}ui_navigation/nav_item.h
)

add_library(cobalt_ui_navigation STATIC
  ${COBALT_ui_navigation_SOURCES}
)

target_link_libraries(cobalt_ui_navigation PUBLIC
  cobalt_base
  base # TODO
  #modp_b64
)

set_property(TARGET cobalt_ui_navigation PROPERTY CXX_STANDARD 17)

target_include_directories(cobalt_ui_navigation PRIVATE
  #${UI_PARENT_DIR}
  ${COBALT_CORE_DIR}
  ${COBALT_CORE_PARENT_DIR}
  ${COBALT_COMMON_INCLUDES}
  ${CHROMIUM_DIR} # to third_party/skia/include/core/SkWriteBuffer.h
)

target_compile_definitions(cobalt_ui_navigation PRIVATE
  # starboard/linux/shared/BUILD.gn
  #STARBOARD_IMPLEMENTATION=1
  #
  #BASE_IMPLEMENTATION=1
  #BASE_I18N_IMPLEMENTATION=1
  #
  #COBALT_ENABLE_VERSION_COMPATIBILITY_VALIDATIONS=1
  #
  ${COBALT_COMMON_DEFINES}
)
