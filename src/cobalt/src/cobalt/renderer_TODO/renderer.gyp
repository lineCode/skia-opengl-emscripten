# Copyright 2015 The Cobalt Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

{
  'targets': [
    {
      # This target conveniently aggregates all sub-modules required to declare
      # and implement rasterization commands as well as display/GPU resource
      # management and communication.  It will eventually also contain source
      # code to help glue all of the components together.
      'target_name': 'renderer',
      'type': 'static_library',
      'sources': [
        'fps_overlay.cc',
        'fps_overlay.h',
        'pipeline.cc',
        'pipeline.h',
        'renderer_module.cc',
        'renderer_module.h',
        'renderer_module_default_options.cc',
        'smoothed_value.cc',
        'smoothed_value.h',
        'submission.h',
        'submission_queue.cc',
        'submission_queue.h',
      ],
      'defines': [
        'COBALT_MINIMUM_FRAME_TIME_IN_MILLISECONDS=<(cobalt_minimum_frame_time_in_milliseconds)',
      ],
      'includes': [
        'renderer_parameters_setup.gypi',
      ],
      'dependencies': [
        '<(DEPTH)/cobalt/base/base.gyp:base',
        '<(DEPTH)/cobalt/content/fonts/fonts.gyp:copy_font_data',
        '<(DEPTH)/cobalt/debug/debug.gyp:console_command_manager',
        '<(DEPTH)/cobalt/math/math.gyp:math',
        '<(DEPTH)/cobalt/render_tree/render_tree.gyp:animations',
        '<(DEPTH)/cobalt/render_tree/render_tree.gyp:render_tree',
        '<(DEPTH)/cobalt/renderer/backend/backend.gyp:renderer_backend',
        '<(DEPTH)/cobalt/renderer/rasterizer/rasterizer.gyp:rasterizer',
        '<(DEPTH)/cobalt/system_window/system_window.gyp:system_window',
        '<(DEPTH)/nb/nb.gyp:nb',
      ],
      'conditions': [
        ['OS=="starboard"', {
          'dependencies': [
            '<(default_renderer_options_dependency)',
          ],
        }],
      ],
    },

    {
      # This target provides functionality for testing that rasterization
      # results for a given render tree match up with a pre-existing
      # "expected output" image that is stored offline as a file.
      'target_name': 'render_tree_pixel_tester',
      'type': 'static_library',
      'sources': [
        'render_tree_pixel_tester.cc',
        'render_tree_pixel_tester.h',
      ],
      'dependencies': [
        '<(DEPTH)/cobalt/base/base.gyp:base',
        '<(DEPTH)/cobalt/renderer/rasterizer/skia/skia/skia.gyp:skia',
        '<(DEPTH)/cobalt/renderer/test/png_utils/png_utils.gyp:png_utils',
        'renderer',
      ],
    },

    {
      'target_name': 'renderer_test',
      'type': '<(gtest_target_type)',
      'sources': [
        'animations_test.cc',
        'pipeline_test.cc',
        'rasterizer/pixel_test.cc',
        'rasterizer/pixel_test_fixture.cc',
        'rasterizer/pixel_test_fixture.h',
        'rasterizer/stress_test.cc',
        'resource_provider_test.cc',
        'smoothed_value_test.cc',
        'submission_queue_test.cc',
      ],
      'dependencies': [
        '<(DEPTH)/cobalt/base/base.gyp:base',
        '<(DEPTH)/cobalt/test/test.gyp:run_all_unittests',
        '<(DEPTH)/testing/gmock.gyp:gmock',
        '<(DEPTH)/testing/gtest.gyp:gtest',
        'render_tree_pixel_tester',
        'renderer_copy_test_data',
      ],
      'conditions': [
        ['enable_map_to_mesh == 1', {
          'defines' : ['ENABLE_MAP_TO_MESH'],
        }],
      ],
    },
    {
      'target_name': 'renderer_copy_test_data',
      'type': 'none',
      'variables': {
        'content_test_input_files': [
          '<(DEPTH)/cobalt/renderer/rasterizer/testdata',
        ],
        'content_test_output_subdir': 'cobalt/renderer/rasterizer',
      },
      'includes': ['<(DEPTH)/starboard/build/copy_test_data.gypi'],
    },
    {
      'target_name': 'renderer_test_deploy',
      'type': 'none',
      'dependencies': [
        'renderer_test',
      ],
      'variables': {
        'executable_name': 'renderer_test',
      },
      'includes': [ '<(DEPTH)/starboard/build/deploy.gypi' ],
    },

  ],
}
