﻿cmake_minimum_required(VERSION 2.8)

#'defines': [
#  'MEDIA_IMPLEMENTATION',
#],
#'dependencies': [
#  '<(DEPTH)/base/base.gyp:base',
#  '<(DEPTH)/net/net.gyp:net',
#  '<(DEPTH)/url/url.gyp:url',
#  '<(DEPTH)/nb/nb.gyp:nb',
#  '<(DEPTH)/starboard/starboard.gyp:starboard',
#],
list(APPEND cobalt_media_SOURCES
  ${COBALT_CORE_DIR}media/media_module_starboard.cc
  #
  ${COBALT_CORE_DIR}media/decoder_buffer_allocator.cc
  ${COBALT_CORE_DIR}media/decoder_buffer_allocator.h
  ${COBALT_CORE_DIR}media/media_module.cc
  ${COBALT_CORE_DIR}media/media_module.h
  ${COBALT_CORE_DIR}media/base/audio_codecs.cc
  ${COBALT_CORE_DIR}media/base/audio_codecs.h
  ${COBALT_CORE_DIR}media/base/audio_decoder_config.cc
  ${COBALT_CORE_DIR}media/base/audio_decoder_config.h
  ${COBALT_CORE_DIR}media/base/audio_timestamp_helper.cc
  ${COBALT_CORE_DIR}media/base/audio_timestamp_helper.h
  ${COBALT_CORE_DIR}media/base/bit_reader.cc
  ## ${COBALT_CORE_DIR}media/base/bit_reader.h
  ${COBALT_CORE_DIR}media/base/bit_reader_core.cc
  ## ${COBALT_CORE_DIR}media/base/bit_reader_core.h
  ${COBALT_CORE_DIR}media/base/byte_queue.cc
  ${COBALT_CORE_DIR}media/base/byte_queue.h
  ${COBALT_CORE_DIR}media/base/channel_layout.cc
  ${COBALT_CORE_DIR}media/base/channel_layout.h
  ${COBALT_CORE_DIR}media/base/color_space.cc
  ${COBALT_CORE_DIR}media/base/color_space.h
  ${COBALT_CORE_DIR}media/base/data_source.cc
  ${COBALT_CORE_DIR}media/base/data_source.h
  ${COBALT_CORE_DIR}media/base/decoder_buffer.cc
  ${COBALT_CORE_DIR}media/base/decoder_buffer.h
  ${COBALT_CORE_DIR}media/base/decoder_buffer_cache.cc
  ${COBALT_CORE_DIR}media/base/decoder_buffer_cache.h
  ${COBALT_CORE_DIR}media/base/decrypt_config.cc
  ${COBALT_CORE_DIR}media/base/decrypt_config.h
  ${COBALT_CORE_DIR}media/base/demuxer.cc
  ${COBALT_CORE_DIR}media/base/demuxer.h
  ${COBALT_CORE_DIR}media/base/demuxer_stream.cc
  ${COBALT_CORE_DIR}media/base/demuxer_stream.h
  ${COBALT_CORE_DIR}media/base/demuxer_stream_provider.cc
  ${COBALT_CORE_DIR}media/base/demuxer_stream_provider.h
  ${COBALT_CORE_DIR}media/base/drm_system.cc
  ${COBALT_CORE_DIR}media/base/drm_system.h
  ${COBALT_CORE_DIR}media/base/encryption_scheme.cc
  ${COBALT_CORE_DIR}media/base/encryption_scheme.h
  ${COBALT_CORE_DIR}media/base/hdr_metadata.cc
  ${COBALT_CORE_DIR}media/base/hdr_metadata.h
  ${COBALT_CORE_DIR}media/base/interleaved_sinc_resampler.cc
  ${COBALT_CORE_DIR}media/base/interleaved_sinc_resampler.h
  ${COBALT_CORE_DIR}media/base/media_log.cc
  ${COBALT_CORE_DIR}media/base/media_log.h
  ${COBALT_CORE_DIR}media/base/media_track.cc
  ${COBALT_CORE_DIR}media/base/media_track.h
  ${COBALT_CORE_DIR}media/base/media_tracks.cc
  ${COBALT_CORE_DIR}media/base/media_tracks.h
  ${COBALT_CORE_DIR}media/base/media_util.cc
  ${COBALT_CORE_DIR}media/base/media_util.h
  ${COBALT_CORE_DIR}media/base/mime_util.cc
  ${COBALT_CORE_DIR}media/base/mime_util.h
  ${COBALT_CORE_DIR}media/base/mime_util_internal.cc
  ${COBALT_CORE_DIR}media/base/mime_util_internal.h
  ${COBALT_CORE_DIR}media/base/ranges.cc
  ${COBALT_CORE_DIR}media/base/ranges.h
  ${COBALT_CORE_DIR}media/base/sample_format.cc
  ${COBALT_CORE_DIR}media/base/sample_format.h
  ${COBALT_CORE_DIR}media/base/shell_audio_bus.cc
  ${COBALT_CORE_DIR}media/base/shell_audio_bus.h
  ${COBALT_CORE_DIR}media/base/shell_data_source_reader.cc
  ${COBALT_CORE_DIR}media/base/shell_data_source_reader.h
  ${COBALT_CORE_DIR}media/base/shell_media_platform.cc
  ${COBALT_CORE_DIR}media/base/shell_media_platform.h
  ${COBALT_CORE_DIR}media/base/starboard_player.cc
  ${COBALT_CORE_DIR}media/base/starboard_player.h
  ${COBALT_CORE_DIR}media/base/starboard_utils.cc
  ${COBALT_CORE_DIR}media/base/starboard_utils.h
  ${COBALT_CORE_DIR}media/base/stream_parser.cc
  ${COBALT_CORE_DIR}media/base/stream_parser.h
  ${COBALT_CORE_DIR}media/base/stream_parser_buffer.cc
  ${COBALT_CORE_DIR}media/base/stream_parser_buffer.h
  ${COBALT_CORE_DIR}media/base/text_track_config.cc
  ${COBALT_CORE_DIR}media/base/text_track_config.h
)

if(TARGET_LINUX)
  list(APPEND cobalt_media_SOURCES
    ${COBALT_CORE_DIR}media/base/video_codecs.cc
    ${COBALT_CORE_DIR}media/base/video_codecs.h
    ${COBALT_CORE_DIR}media/base/video_decoder_config.cc
    ${COBALT_CORE_DIR}media/base/video_decoder_config.h
    ${COBALT_CORE_DIR}media/base/video_frame_provider.h
    ${COBALT_CORE_DIR}media/base/video_util.cc
    ${COBALT_CORE_DIR}media/base/video_util.h
    ${COBALT_CORE_DIR}media/filters/h264_bit_reader.cc
    ${COBALT_CORE_DIR}media/filters/h264_bit_reader.h
    ${COBALT_CORE_DIR}media/filters/h264_bitstream_buffer.cc
    ${COBALT_CORE_DIR}media/filters/h264_bitstream_buffer.h
    ${COBALT_CORE_DIR}media/filters/h264_parser.cc
    ${COBALT_CORE_DIR}media/filters/h264_parser.h
    ${COBALT_CORE_DIR}media/filters/h264_to_annex_b_bitstream_converter.cc
    ${COBALT_CORE_DIR}media/filters/h264_to_annex_b_bitstream_converter.h
    ${COBALT_CORE_DIR}media/filters/h265_parser.cc
    ${COBALT_CORE_DIR}media/filters/h265_parser.h
    ${COBALT_CORE_DIR}media/filters/shell_au.cc
    ${COBALT_CORE_DIR}media/filters/shell_au.h
    ${COBALT_CORE_DIR}media/filters/shell_avc_parser.cc
    ${COBALT_CORE_DIR}media/filters/shell_avc_parser.h
    ${COBALT_CORE_DIR}media/filters/shell_demuxer.cc
    ${COBALT_CORE_DIR}media/filters/shell_demuxer.h
    ${COBALT_CORE_DIR}media/filters/shell_mp4_map.cc
    ${COBALT_CORE_DIR}media/filters/shell_mp4_map.h
    ${COBALT_CORE_DIR}media/filters/shell_mp4_parser.cc
    ${COBALT_CORE_DIR}media/filters/shell_mp4_parser.h
    ${COBALT_CORE_DIR}media/filters/shell_parser.cc
    ${COBALT_CORE_DIR}media/filters/shell_parser.h
    ${COBALT_CORE_DIR}media/filters/shell_rbsp_stream.cc
    ${COBALT_CORE_DIR}media/filters/shell_rbsp_stream.h
    ${COBALT_CORE_DIR}media/fetcher_buffered_data_source.cc
    ${COBALT_CORE_DIR}media/fetcher_buffered_data_source.h
    ${COBALT_CORE_DIR}media/media_module_starboard.cc
    ${COBALT_CORE_DIR}media/base/sbplayer_pipeline.cc
    ${COBALT_CORE_DIR}media/base/sbplayer_set_bounds_helper.cc
    ${COBALT_CORE_DIR}media/base/sbplayer_set_bounds_helper.h
    ${COBALT_CORE_DIR}media/formats/common/offset_byte_queue.cc
    ${COBALT_CORE_DIR}media/formats/common/offset_byte_queue.h
    ${COBALT_CORE_DIR}media/formats/mp4/aac.cc
    ${COBALT_CORE_DIR}media/formats/mp4/aac.h
    ${COBALT_CORE_DIR}media/formats/mp4/avc.cc
    ${COBALT_CORE_DIR}media/formats/mp4/avc.h
    ${COBALT_CORE_DIR}media/formats/mp4/bitstream_converter.cc
    ${COBALT_CORE_DIR}media/formats/mp4/bitstream_converter.h
    ${COBALT_CORE_DIR}media/formats/mp4/box_definitions.cc
    ${COBALT_CORE_DIR}media/formats/mp4/box_definitions.h
    ${COBALT_CORE_DIR}media/formats/mp4/box_reader.cc
    ${COBALT_CORE_DIR}media/formats/mp4/box_reader.h
    ${COBALT_CORE_DIR}media/formats/mp4/es_descriptor.cc
    ${COBALT_CORE_DIR}media/formats/mp4/es_descriptor.h
    ${COBALT_CORE_DIR}media/formats/mp4/fourccs.h
    ${COBALT_CORE_DIR}media/formats/mp4/hevc.cc
    ${COBALT_CORE_DIR}media/formats/mp4/hevc.h
    ${COBALT_CORE_DIR}media/formats/mp4/mp4_stream_parser.cc
    ${COBALT_CORE_DIR}media/formats/mp4/mp4_stream_parser.h
    ${COBALT_CORE_DIR}media/formats/mp4/rcheck.h
    ${COBALT_CORE_DIR}media/formats/mp4/sample_to_group_iterator.cc
    ${COBALT_CORE_DIR}media/formats/mp4/sample_to_group_iterator.h
    ${COBALT_CORE_DIR}media/formats/mp4/track_run_iterator.cc
    ${COBALT_CORE_DIR}media/formats/mp4/track_run_iterator.h
    ${COBALT_CORE_DIR}media/formats/mpeg/adts_constants.cc
    ${COBALT_CORE_DIR}media/formats/mpeg/adts_constants.h
    ${COBALT_CORE_DIR}media/formats/mpeg/adts_stream_parser.cc
    ${COBALT_CORE_DIR}media/formats/mpeg/adts_stream_parser.h
    ${COBALT_CORE_DIR}media/formats/mpeg/mpeg1_audio_stream_parser.cc
    ${COBALT_CORE_DIR}media/formats/mpeg/mpeg1_audio_stream_parser.h
    ${COBALT_CORE_DIR}media/formats/mpeg/mpeg_audio_stream_parser_base.cc
    ${COBALT_CORE_DIR}media/formats/mpeg/mpeg_audio_stream_parser_base.h
    ${COBALT_CORE_DIR}media/formats/webm/cluster_builder.cc
    ${COBALT_CORE_DIR}media/formats/webm/cluster_builder.h
    ${COBALT_CORE_DIR}media/formats/webm/opus_packet_builder.cc
    ${COBALT_CORE_DIR}media/formats/webm/opus_packet_builder.h
    ${COBALT_CORE_DIR}media/formats/webm/tracks_builder.cc
    ${COBALT_CORE_DIR}media/formats/webm/tracks_builder.h
    ${COBALT_CORE_DIR}media/formats/webm/webm_audio_client.cc
    ${COBALT_CORE_DIR}media/formats/webm/webm_audio_client.h
    ${COBALT_CORE_DIR}media/formats/webm/webm_cluster_parser.cc
    ${COBALT_CORE_DIR}media/formats/webm/webm_cluster_parser.h
    ${COBALT_CORE_DIR}media/formats/webm/webm_colour_parser.cc
    ${COBALT_CORE_DIR}media/formats/webm/webm_colour_parser.h
    ${COBALT_CORE_DIR}media/formats/webm/webm_constants.cc
    ${COBALT_CORE_DIR}media/formats/webm/webm_constants.h
    ${COBALT_CORE_DIR}media/formats/webm/webm_content_encodings.cc
    ${COBALT_CORE_DIR}media/formats/webm/webm_content_encodings.h
    ${COBALT_CORE_DIR}media/formats/webm/webm_content_encodings_client.cc
    ${COBALT_CORE_DIR}media/formats/webm/webm_content_encodings_client.h
    ${COBALT_CORE_DIR}media/formats/webm/webm_crypto_helpers.cc
    ${COBALT_CORE_DIR}media/formats/webm/webm_crypto_helpers.h
    ${COBALT_CORE_DIR}media/formats/webm/webm_info_parser.cc
    ${COBALT_CORE_DIR}media/formats/webm/webm_info_parser.h
    ${COBALT_CORE_DIR}media/formats/webm/webm_parser.cc
    ${COBALT_CORE_DIR}media/formats/webm/webm_parser.h
    ${COBALT_CORE_DIR}media/formats/webm/webm_stream_parser.cc
    ${COBALT_CORE_DIR}media/formats/webm/webm_stream_parser.h
    ${COBALT_CORE_DIR}media/formats/webm/webm_tracks_parser.cc
    ${COBALT_CORE_DIR}media/formats/webm/webm_tracks_parser.h
    ${COBALT_CORE_DIR}media/formats/webm/webm_video_client.cc
    ${COBALT_CORE_DIR}media/formats/webm/webm_video_client.h
    ${COBALT_CORE_DIR}media/formats/webm/webm_webvtt_parser.cc
    ${COBALT_CORE_DIR}media/formats/webm/webm_webvtt_parser.h
    ${COBALT_CORE_DIR}media/player/buffered_data_source.h
    ${COBALT_CORE_DIR}media/player/web_media_player_impl.cc
    ${COBALT_CORE_DIR}media/player/web_media_player_impl.h
    ${COBALT_CORE_DIR}media/player/web_media_player_proxy.cc
    ${COBALT_CORE_DIR}media/player/web_media_player_proxy.h
  )
endif(TARGET_LINUX)

list(APPEND cobalt_media_SOURCES
  ${COBALT_CORE_DIR}media/filters/chunk_demuxer.cc
  ${COBALT_CORE_DIR}media/filters/chunk_demuxer.h
  ${COBALT_CORE_DIR}media/filters/frame_processor.cc
  ## ${COBALT_CORE_DIR}media/filters/frame_processor.h
  ${COBALT_CORE_DIR}media/filters/source_buffer_range.cc
  ${COBALT_CORE_DIR}media/filters/source_buffer_range.h
  ${COBALT_CORE_DIR}media/filters/source_buffer_state.cc
  ## ${COBALT_CORE_DIR}media/filters/source_buffer_state.h
  ${COBALT_CORE_DIR}media/filters/source_buffer_stream.cc
  ## ${COBALT_CORE_DIR}media/filters/source_buffer_stream.h
  ${COBALT_CORE_DIR}media/filters/stream_parser_factory.cc
)

add_library(cobalt_media STATIC
  ${cobalt_media_SOURCES}
)

target_link_libraries(cobalt_media PUBLIC
  base # TODO
  cobalt_base
  cobalt_nanobase
  #cobalt_script
  #v8_stub
  ${COBALT_LOADER_LIB_NAME} ## TODO
  ${COBALT_CSP_LIB_NAME}
  cobalt_system_window # TODO
  starboard_icu_init
  starboard_eztime  # requires starboard_icu_init
  #starboard_stub
  starboard_platform # requires starboard_stub
  starboard_common
  #starboard_core
  modp_b64
  dynamic_annotations
  GURL
  ${GNET_LIBS}
  ${GLIBXML_LIB}
  icu
  ced
  glm
  ${WEBP_LIB} # requires libpng
  #${libjpeg_LIB}
  ${libjpeg_TURBO_LIB}
  ${libpng_LIB}
  #${iccjpeg_LIB}
)

set_property(TARGET cobalt_media PROPERTY CXX_STANDARD 17)

target_include_directories(cobalt_media PRIVATE
  #${UI_PARENT_DIR}
  ${COBALT_CORE_DIR}
  ${COBALT_CORE_PARENT_DIR}
  ${COBALT_COMMON_INCLUDES}
  ${CHROMIUM_DIR} # to third_party/skia/include/core/SkWriteBuffer.h
)

target_compile_definitions(cobalt_media PRIVATE
  # starboard/linux/shared/BUILD.gn
  #STARBOARD_IMPLEMENTATION=1
  #
  #BASE_IMPLEMENTATION=1
  #BASE_I18N_IMPLEMENTATION=1
  #
  #COBALT_ENABLE_VERSION_COMPATIBILITY_VALIDATIONS=1
  #
  MEDIA_IMPLEMENTATION=1
  ${COBALT_COMMON_DEFINES}
  # TODO # COBALT_DISABLE_SPDY
  # TODO # DIAL_SERVER
  # TODO # COBALT_MEDIA_SOURCE_2016=1
  # TODO # COBALT_MEDIA_BUFFER_STORAGE_TYPE_MEMORY=1
  # TODO # COBALT_MEDIA_BUFFER_POOL_ALLOCATE_ON_DEMAND
  # TODO # COBALT_MEDIA_BUFFER_INITIAL_CAPACITY
  # TODO # COBALT_MEDIA_BUFFER_MAX_CAPACITY_1080p
  # TODO # COBALT_MEDIA_BUFFER_MAX_CAPACITY_4K
  # TODO # COBALT_MEDIA_BUFFER_ALLOCATION_UNIT
  # TODO # COBALT_MEDIA_BUFFER_ALIGNMENT
  # TODO # COBALT_MEDIA_BUFFER_PADDING
  # TODO # COBALT_MEDIA_BUFFER_PROGRESSIVE_BUDGET
  # TODO # COBALT_MEDIA_BUFFER_NON_VIDEO_BUDGET
  # TODO # COBALT_MEDIA_BUFFER_VIDEO_BUDGET_1080P
  # TODO # COBALT_MEDIA_BUFFER_VIDEO_BUDGET_4K
  # TODO # COBALT_MEDIA_SOURCE_GARBAGE_COLLECTION_DURATION_THRESHOLD_IN_SECONDS
)
