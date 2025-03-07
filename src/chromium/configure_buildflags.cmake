﻿find_package(Git)

set(RUN_GENERATORS TRUE)

# https://cmake.org/cmake/help/latest/module/FindPythonInterp.html
find_package( PythonInterp 2.7 REQUIRED )
#find_package( PythonLibs 2.7 REQUIRED )
message("Found python ${PYTHON_EXECUTABLE}")

# TODO: mkdir chromium/ui/strings/grit

# see https://github.com/dckristiono/chromium/tree/master/out/Default/gen/ui
#configure_file(${CHROMIUM_DIR}/web_ports/ui_strings.h.tpl
#  ${CHROMIUM_DIR}/ui/strings/ui_strings.h COPYONLY)

# see https://github.com/dckristiono/chromium/tree/master/out/Default/gen/ui
#configure_file(${CHROMIUM_DIR}/web_ports/app_locale_settings.h.tpl
#  ${CHROMIUM_DIR}/ui/strings/app_locale_settings.h COPYONLY)

set(NEED_GEN_MOJO FALSE) # TODO: not supported for now

# TODO: mkdir chromium/ui/resources/grit

# TODO
if(RUN_GENERATORS)
  set(NEED_GEN_BUILD_DATE TRUE) # TODO
  set(NEED_GEN_BUILDFLAGS TRUE) # TODO
else()
  set(NEED_GEN_BUILD_DATE FALSE) # TODO
  set(NEED_GEN_BUILDFLAGS FALSE) # TODO
endif()

if(NEED_GEN_BUILD_DATE)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} log -n1 --format="%at"
    WORKING_DIRECTORY ${CHROMIUM_DIR}
    OUTPUT_VARIABLE _build_timestamp
    #ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_VARIABLE _ERROR_VARIABLE
    RESULT_VARIABLE retcode
  )
  if(NOT "${retcode}" STREQUAL "0")
    message( FATAL_ERROR "Bad exit status ${_ERROR_VARIABLE}")
  endif()
endif()

# src/chromium/third_party/blink/renderer/platform/runtime_enabled_features.cc
# src/chromium/gen/gen_blink_public/third_party/blink/renderer/platform/runtime_enabled_features.cc
#execute_process(COMMAND "mkdir"
#  "-p" "${GEN_COMBINED_DIR}third_party/blink/renderer/platform"
#  WORKING_DIRECTORY ${GEN_COMBINED_DIR})
##message(FATAL_ERROR "${GEN_COMBINED_DIR}third_party/blink/renderer/platform")
##
#execute_process(
#  COMMAND ${PYTHON_EXECUTABLE}
#    ${CHROMIUM_DIR}/third_party/blink/renderer/build/scripts/make_runtime_features.py
#    ${CHROMIUM_DIR}/third_party/blink/renderer/platform/runtime_enabled_features.json5
#    --output_dir ${CHROMIUM_DIR}/third_party/blink/renderer/platform
#    #--developer_dir
#  WORKING_DIRECTORY ${CHROMIUM_DIR}/third_party/blink/renderer/platform
#  OUTPUT_VARIABLE _OUTPUT_VARIABLE
#  #ERROR_QUIET
#  OUTPUT_STRIP_TRAILING_WHITESPACE
#  RESULT_VARIABLE retcode
#  ERROR_VARIABLE _ERROR_VARIABLE
#)
#if(NOT "${retcode}" STREQUAL "0")
#  if(RELEASE_BUILD)
#    message( FATAL_ERROR "Bad exit status ${retcode} ${_ERROR_VARIABLE}")
#  else()
#    message( WARNING "Bad exit status ${retcode} ${_ERROR_VARIABLE}")
#  endif()
#endif()

### MOJO ###

if(NEED_GEN_MOJO)
  execute_process(COMMAND mkdir
    -p ${CMAKE_BINARY_DIR}/mojo/public/tools/bindings
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif(NEED_GEN_MOJO)
#
set(MOJO_BYTECODE_DIR ${CMAKE_BINARY_DIR}/mojo/public/tools/bindings)
set(MOJO_OUT_DIR ${CMAKE_BINARY_DIR}/mojo)
set(MOJO_DIR ${CHROMIUM_DIR}/mojo)

# creates:
# * ./{build-dir-here}/mojo/public/tools/bindings/java_templates.zip
# * ./{build-dir-here}/mojo/public/tools/bindings/cpp_templates.zip
# * ./{build-dir-here}/mojo/public/tools/bindings/js_templates.zip
macro(mojom_bindings_compile ARG_WORKING_DIRECTORY)
  execute_process(
    COMMAND ${PYTHON_EXECUTABLE}
      ${MOJO_DIR}/public/tools/bindings/mojom_bindings_generator.py
      --use_bundled_pylibs
      precompile # (choose from 'parse', 'generate', 'precompile', 'verify')
      -o ${MOJO_BYTECODE_DIR}
      #./third_party/blink/renderer/bindings/templates/interface.cc.tmpl
      ##${CHROMIUM_DIR}/third_party/blink/public/mojom/feature_policy/feature_policy.mojom
      #feature_policy.mojom
      #-d .
      #-I .
      #--output_dir ${CHROMIUM_DIR}/third_party/blink/public/mojom/feature_policy
      ##--disallow_native_types
      ##--generate_message_ids
      #--for_blink
      #--generators c++
      #--gen_dir ${CHROMIUM_DIR}/third_party/blink/public/mojom/feature_policy
      ##generate

    WORKING_DIRECTORY ${ARG_WORKING_DIRECTORY}
    OUTPUT_VARIABLE _OUTPUT_VARIABLE
    #ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE retcode
    ERROR_VARIABLE _ERROR_VARIABLE
  )
  if(NOT "${retcode}" STREQUAL "0")
    if(RELEASE_BUILD)
      message( FATAL_ERROR "Bad exit status ${retcode} ${_ERROR_VARIABLE}")
    else()
      message( WARNING "Bad exit status ${retcode} ${_ERROR_VARIABLE}")
    endif()
  endif()
endmacro(mojom_bindings_compile)

set(BLINK_DIR ${CHROMIUM_DIR}/third_party/blink/)

macro(mojom_bindings_gen ARG_MOJO_FILE ARG_OUT_DIR ARG_WORKING_DIRECTORY)
  message( STATUS "mojom_bindings_gen for ${ARG_MOJO_FILE}...")
  execute_process(
    COMMAND ${PYTHON_EXECUTABLE}
      ${MOJO_DIR}/public/tools/bindings/mojom_bindings_generator.py
      --use_bundled_pylibs
      generate # (choose from 'parse', 'generate', 'precompile', 'verify')
      ${ARG_MOJO_FILE}
      #${BLINK_DIR}public/mojom/feature_policy/feature_policy.mojom
      #feature_policy.mojom
      --bytecode_path ${MOJO_BYTECODE_DIR}
      -d ${ARG_WORKING_DIRECTORY}
      -I ${ARG_WORKING_DIRECTORY}
      -o ${ARG_OUT_DIR}
      #--output_dir ${CHROMIUM_DIR}/third_party/blink/public/mojom/feature_policy
      #--disallow_native_types
      #--generate_message_ids
      #--for_blink
      --generators c++
      #--gen_dir ${CHROMIUM_DIR}/third_party/blink/public/mojom/feature_policy
    #
    WORKING_DIRECTORY ${CHROMIUM_DIR}
    OUTPUT_VARIABLE _OUTPUT_VARIABLE
    #ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE retcode
    ERROR_VARIABLE _ERROR_VARIABLE
  )
  if(NOT "${retcode}" STREQUAL "0")
    if(RELEASE_BUILD)
      message( FATAL_ERROR "Bad exit status ${retcode} ${_ERROR_VARIABLE}")
    else()
      message( WARNING "Bad exit status ${retcode} ${_ERROR_VARIABLE}")
    endif()
  endif()
endmacro(mojom_bindings_gen)

if(NEED_GEN_MOJO)
#third_party/blink/public/mojom/feature_policy/feature_policy.mojom.h
  mojom_bindings_gen(
    ${BLINK_DIR}public/mojom/feature_policy/feature_policy.mojom
    ${MOJO_OUT_DIR}
  )

  mojom_bindings_compile(
    ${CHROMIUM_DIR}/services
  )

  # services/network/public/mojom/cors.mojom-shared.h
  mojom_bindings_gen(
    ${CHROMIUM_DIR}/services/network/public/mojom/cors.mojom
    #${MOJO_OUT_DIR}
    ${CHROMIUM_DIR}/services/network/public/mojom/
    ${CHROMIUM_DIR}/services/network
  )
endif(NEED_GEN_MOJO)

#
#
#
if(NEED_GEN_BUILD_DATE)
  string(REPLACE "\"" "" _build_timestamp ${_build_timestamp})
  #message(FATAL_ERROR ${_build_timestamp})

  # PRE-GENERATION GENERATED_BUILD_DATE.H (NEEDED BY "build_time.cc")
  execute_process(
    COMMAND sh -c "python ${BUILD_TOOLS_DIR}/write_build_date_header.py generated_build_date.h ${_build_timestamp}"
    WORKING_DIRECTORY ${BASE_DIR})
endif(NEED_GEN_BUILD_DATE)

# TODO: replace configure_file to write_buildflag_header.py https://github.com/Tarnyko/chromium-lite/blob/7d7b2b8a96bbc8370f3c2423885140b074ced151/01a-base/CMakeLists.txt#L70

if(NEED_GEN_BUILDFLAGS)
  # https://github.com/chromium/chromium/blob/master/base/allocator/BUILD.gn#L291
  # https://github.com/ruslanch/quic-cmake/blob/master/base/CMakeLists.txt#L35
  configure_file(${CHROMIUM_DIR}/allocator_buildflags.h.inc
    ${BASE_DIR}allocator/buildflags.h COPYONLY)

  configure_file(${CHROMIUM_DIR}/cfi_buildflags.h.inc
    ${BASE_DIR}/base/cfi_buildflags.h COPYONLY)

  # https://github.com/chromium/chromium/blob/master/base/BUILD.gn#L1980
  configure_file(${CHROMIUM_DIR}/debugging_buildflags.h.inc
    ${BASE_DIR}/debug/debugging_buildflags.h COPYONLY)

  # https://github.com/chromium/chromium/blob/master/base/BUILD.gn#L2017
  # https://github.com/geanix/courgette/blob/master/gen/base/synchronization/synchronization_buildflags.h
  configure_file(${CHROMIUM_DIR}/synchronization_buildflags.h.inc
    ${BASE_DIR}/base/synchronization/synchronization_buildflags.h COPYONLY)

  configure_file(${CHROMIUM_DIR}/protected_memory_buildflags.h.inc
    ${BASE_DIR}/base/memory/protected_memory_buildflags.h COPYONLY)

  configure_file(${CHROMIUM_DIR}/partition_alloc_buildflags.h.inc
    ${BASE_DIR}/base/partition_alloc_buildflags.h COPYONLY)

  # https://github.com/chromium/chromium/blob/master/base/BUILD.gn#L2044
  configure_file(${CHROMIUM_DIR}/clang_coverage_buildflags.h.inc
    ${BASE_DIR}/base/clang_coverage_buildflags.h COPYONLY)

  # https://github.com/stormcenter/QuicDemo/blob/master/app/src/main/jni/third_party/chromium/include/net/net_buildflags.h
  configure_file(${CHROMIUM_DIR}/gpu_vulkan_buildflags.h.inc
    ${CHROMIUM_DIR}/gpu/vulkan/buildflags.h COPYONLY)

  configure_file(${CHROMIUM_DIR}/gnet_buildflags.h.inc
    ${CHROMIUM_DIR}/net/net_buildflags.h COPYONLY)

  configure_file(${CHROMIUM_DIR}/device_vr_buildflags.h.inc
    ${CHROMIUM_DIR}/device/vr/buildflags/buildflags.h COPYONLY)

  configure_file(${CHROMIUM_DIR}/ui_gl_buildflags.h.inc
    ${CHROMIUM_DIR}/ui/gl/buildflags.h COPYONLY)

  configure_file(${CHROMIUM_DIR}/ui_base_buildflags.h.inc
    ${CHROMIUM_DIR}/ui/base/buildflags.h COPYONLY)

  # RAW_HEAP_SNAPSHOTS=$v8_enable_raw_heap_snapshots",
  # RCS_COUNT_EVERYTHING=$runtime_call_stats_count_everything",
  configure_file(${CHROMIUM_DIR}/blink_renderer_platform_bindings_buildflags.h.inc
    ${CHROMIUM_DIR}/third_party/blink/renderer/platform/bindings/buildflags.h COPYONLY)

  #if(NOT EXISTS "${CHROMIUM_DIR}/third_party/blink/renderer/platform/bindings/buildflags.h")
  #  message(FATAL_ERROR "can`t configure ${CHROMIUM_DIR}/third_party/blink/renderer/platform/bindings/buildflags.h")
  #endif()

  # BLINK_HEAP_VERIFICATION
  configure_file(${CHROMIUM_DIR}/blink_renderer_platform_heap_buildflags.h.inc
    ${CHROMIUM_DIR}/third_party/blink/renderer/platform/heap/heap_buildflags.h COPYONLY)

  # IS_CT_SUPPORTED
  configure_file(${CHROMIUM_DIR}/network_service_buildflags.h.inc
    ${CHROMIUM_DIR}/services/network/public/cpp/network_service_buildflags.h COPYONLY)

  #define BUILDFLAG_INTERNAL_MOJO_TRACE_ENABLED() (0)
  #define BUILDFLAG_INTERNAL_MOJO_RANDOM_DELAYS_ENABLED() (0)
  configure_file(${CHROMIUM_DIR}/mojo_buildflags.h.inc
    ${CHROMIUM_DIR}/mojo/public/cpp/bindings/mojo_buildflags.h COPYONLY)

  #define BUILDFLAG_INTERNAL_ALTERNATE_CDM_STORAGE_ID_KEY() ()
  #define BUILDFLAG_INTERNAL_ENABLE_AC3_EAC3_AUDIO_DEMUXING() (0)
  #define BUILDFLAG_INTERNAL_ENABLE_CBCS_ENCRYPTION_SCHEME() (1)
  #define BUILDFLAG_INTERNAL_ENABLE_CDM_HOST_VERIFICATION() (0)
  #define BUILDFLAG_INTERNAL_ENABLE_CDM_STORAGE_ID() (0)
  #define BUILDFLAG_INTERNAL_ENABLE_DAV1D_DECODER() (1)
  #define BUILDFLAG_INTERNAL_ENABLE_AV1_DECODER() (1)
  #define BUILDFLAG_INTERNAL_ENABLE_DOLBY_VISION_DEMUXING() (0)
  #define BUILDFLAG_INTERNAL_ENABLE_FFMPEG() (1)
  #define BUILDFLAG_INTERNAL_ENABLE_FFMPEG_VIDEO_DECODERS() (1)
  #define BUILDFLAG_INTERNAL_ENABLE_HEVC_DEMUXING() (0)
  #define BUILDFLAG_INTERNAL_ENABLE_HLS_SAMPLE_AES() (0)
  #define BUILDFLAG_INTERNAL_ENABLE_LIBRARY_CDMS() (1)
  #define BUILDFLAG_INTERNAL_ENABLE_LIBVPX() (1)
  #define BUILDFLAG_INTERNAL_ENABLE_LOGGING_OVERRIDE() (0)
  #define BUILDFLAG_INTERNAL_ENABLE_MEDIA_REMOTING() (1)
  #define BUILDFLAG_INTERNAL_ENABLE_MEDIA_REMOTING_RPC() (1)
  #define BUILDFLAG_INTERNAL_ENABLE_MPEG_H_AUDIO_DEMUXING() (0)
  #define BUILDFLAG_INTERNAL_ENABLE_MSE_MPEG2TS_STREAM_PARSER() (0)
  #define BUILDFLAG_INTERNAL_USE_PROPRIETARY_CODECS() (0)
  configure_file(${CHROMIUM_DIR}/media_buildflags.h.inc
    ${CHROMIUM_DIR}/media/media_buildflags.h COPYONLY)

  ## uses BUILDFLAG_INTERNAL_IPC_MESSAGE_LOG_ENABLED from ipc_buildflags.h
  #configure_file(${CHROMIUM_DIR}/gipc_buildflags.h.inc
  #  ${GIPC_DIR}/ipc_buildflags.h COPYONLY)
  #

  # USE_MPRIS
  configure_file(${CHROMIUM_DIR}/mpris_buildflags.h.inc
    ${CHROMIUM_DIR}/ui/base/mpris/buildflags/buildflags.h COPYONLY)

  configure_file(${CHROMIUM_DIR}/ui_views_buildflags.h.inc
    ${CHROMIUM_DIR}ui/views/buildflags.h COPYONLY)
endif(NEED_GEN_BUILDFLAGS)
