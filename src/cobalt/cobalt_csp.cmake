cmake_minimum_required(VERSION 2.8)

#'dependencies': [
#  '<(DEPTH)/cobalt/base/base.gyp:base',
#  '<(DEPTH)/cobalt/network/network.gyp:network',
#  '<(DEPTH)/net/net.gyp:net',
#  '<(DEPTH)/url/url.gyp:url',
#],
set(COBALT_CSP_SOURCES
  ${COBALT_CORE_DIR}csp/content_security_policy.cc
  ${COBALT_CORE_DIR}csp/content_security_policy.h
  ${COBALT_CORE_DIR}csp/crypto.cc
  ${COBALT_CORE_DIR}csp/crypto.h
  ${COBALT_CORE_DIR}csp/directive.cc
  ${COBALT_CORE_DIR}csp/directive.h
  ${COBALT_CORE_DIR}csp/directive_list.cc
  ${COBALT_CORE_DIR}csp/directive_list.h
  ${COBALT_CORE_DIR}csp/media_list_directive.cc
  ${COBALT_CORE_DIR}csp/media_list_directive.h
  ${COBALT_CORE_DIR}csp/parsers.h
  ${COBALT_CORE_DIR}csp/source.cc
  ${COBALT_CORE_DIR}csp/source.h
  ${COBALT_CORE_DIR}csp/source_list.cc
  ${COBALT_CORE_DIR}csp/source_list.h
  ${COBALT_CORE_DIR}csp/source_list_directive.cc
  ${COBALT_CORE_DIR}csp/source_list_directive.h
)

add_library(cobalt_csp STATIC
  ${COBALT_CSP_SOURCES}
)

target_link_libraries(cobalt_csp PUBLIC
  cobalt_base
  base # TODO
  modp_b64
  #GFX_GEOMETRY
  #${BASE_LIBRARIES}
  #base
  #SKIA
  dynamic_annotations
  #UI_GFX
  ##BLINK_RENDERER_CORE
  #BLINK_PUBLIC_COMMON
  #BLINK_PUBLIC_MOJOM
  ##BLINK_RENDERER_NETWORK
  #BLINK_RENDERER_PLATFORM
  GURL
  ${GNET_LIBS}
  #GCRYPTO
  #GFX_GEOMETRY
  #UI_GFX
  ## mojo
  ## services/service_manager
  ## services/ws/public/cpp/gpu
  ##${BASE_LIBRARIES}
  GLIBXML
  #SKIA
  ##skcms
  #ced
  ## emoji-segmenter
  ## webrtc
  ## zlib
  icu
  ced
  glm
  #CC
  ##G_GPU
  #ANIMATION_CC
  #BASE_CC
  #PAINT_CC
  #SERVICES_NETWORK_PUBLIC_CPP
  #${WEBP_LIB} # requires libpng
  #${libjpeg_LIB}
  #${libjpeg_TURBO_LIB}
  #${libpng_LIB}
  #${iccjpeg_LIB}
  #MOJO
  ##
  ## khronos
  #${khronos_LIB}
  #LIB_V8_INTERFACE
  #COMPONENTS_SCHEDULING_METRICS
  #${HARFBUZZ_LIBRARIES}
  #GMEDIA
  #GZLIB_EXT
  #SERVICES_SERVICE_MANAGER_PUBLIC_CPP
  #GFX_CODEC
)

set_property(TARGET cobalt_csp PROPERTY CXX_STANDARD 17)

target_include_directories(cobalt_csp PRIVATE
  #${UI_PARENT_DIR}
  ${COBALT_CORE_DIR}
  ${COBALT_CORE_PARENT_DIR}
  ${COBALT_COMMON_INCLUDES}
  ${CHROMIUM_DIR} # to third_party/skia/include/core/SkWriteBuffer.h
)

target_compile_definitions(cobalt_csp PRIVATE
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
