﻿### --- GFX_SWITCHES ---###

set(GFX_SWITCHES_SOURCES
  ${GFX_SWITCHES_DIR}switches.cc
  #"switches.h",
  #"switches_export.h",
)

add_library(GFX_SWITCHES STATIC
  ${GFX_SWITCHES_SOURCES}
)

target_link_libraries(GFX_SWITCHES PUBLIC
  #${BASE_LIBRARIES}
  base
  #${OPENGLES2_LIBRARIES}
  ${FOUND_OPENGL_LIBRARIES}
)

set_property(TARGET GFX_SWITCHES PROPERTY CXX_STANDARD 17)

target_include_directories(GFX_SWITCHES PRIVATE
  ${GFX_SWITCHES_DIR}
  ${BASE_DIR}
  ${OPENGL_INCLUDE_DIR}
)

target_compile_definitions(GFX_SWITCHES PRIVATE
  GFX_SWITCHES_IMPLEMENTATION=1
)
