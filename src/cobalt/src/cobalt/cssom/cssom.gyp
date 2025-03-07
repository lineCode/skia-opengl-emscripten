# Copyright 2014 The Cobalt Authors. All Rights Reserved.
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
  'variables': {
    'sb_pedantic_warnings': 1,
  },
  'targets': [
    {
      'target_name': 'cssom',
      'type': 'static_library',
      'sources': [
        'absolute_url_value.cc',
        'absolute_url_value.h',
        'active_pseudo_class.cc',
        'active_pseudo_class.h',
        'after_pseudo_element.cc',
        'after_pseudo_element.h',
        'animation.h',
        'animation_set.cc',
        'animation_set.h',
        'attribute_selector.cc',
        'attribute_selector.h',
        'before_pseudo_element.cc',
        'before_pseudo_element.h',
        'calc_value.cc',
        'calc_value.h',
        'cascade_precedence.h',
        'cascaded_style.cc',
        'cascaded_style.h',
        'character_classification.h',
        'child_combinator.cc',
        'child_combinator.h',
        'class_selector.cc',
        'class_selector.h',
        'cobalt_ui_nav_focus_transform_function.cc',
        'cobalt_ui_nav_focus_transform_function.h',
        'cobalt_ui_nav_spotlight_transform_function.cc',
        'cobalt_ui_nav_spotlight_transform_function.h',
        'color_stop.cc',
        'color_stop.h',
        'combinator.cc',
        'combinator.h',
        'combinator_visitor.h',
        'complex_selector.cc',
        'complex_selector.h',
        'compound_selector.cc',
        'compound_selector.h',
        'computed_style.cc',
        'computed_style.h',
        'css.cc',
        'css.h',
        'css_computed_style_data.cc',
        'css_computed_style_data.h',
        'css_computed_style_declaration.cc',
        'css_computed_style_declaration.h',
        'css_condition_rule.cc',
        'css_condition_rule.h',
        'css_declaration_data.h',
        'css_declaration_util.cc',
        'css_declaration_util.h',
        'css_declared_style_data.cc',
        'css_declared_style_data.h',
        'css_declared_style_declaration.cc',
        'css_declared_style_declaration.h',
        'css_font_face_declaration_data.cc',
        'css_font_face_declaration_data.h',
        'css_font_face_rule.cc',
        'css_font_face_rule.h',
        'css_grouping_rule.cc',
        'css_grouping_rule.h',
        'css_keyframe_rule.cc',
        'css_keyframe_rule.h',
        'css_keyframes_rule.cc',
        'css_keyframes_rule.h',
        'css_media_rule.cc',
        'css_media_rule.h',
        'css_parser.h',
        'css_rule.cc',
        'css_rule.h',
        'css_rule_list.cc',
        'css_rule_list.h',
        'css_rule_style_declaration.cc',
        'css_rule_style_declaration.h',
        'css_rule_visitor.h',
        'css_style_declaration.cc',
        'css_style_declaration.h',
        'css_style_rule.cc',
        'css_style_rule.h',
        'css_style_sheet.cc',
        'css_style_sheet.h',
        'css_transition.cc',
        'css_transition.h',
        'css_transition_set.cc',
        'css_transition_set.h',
        'descendant_combinator.cc',
        'descendant_combinator.h',
        'empty_pseudo_class.cc',
        'empty_pseudo_class.h',
        'filter_function.h',
        'filter_function_list_value.cc',
        'filter_function_list_value.h',
        'focus_pseudo_class.cc',
        'focus_pseudo_class.h',
        'following_sibling_combinator.cc',
        'following_sibling_combinator.h',
        'font_style_value.cc',
        'font_style_value.h',
        'font_weight_value.cc',
        'font_weight_value.h',
        'hover_pseudo_class.cc',
        'hover_pseudo_class.h',
        'id_selector.cc',
        'id_selector.h',
        'integer_value.cc',
        'integer_value.h',
        'interpolate_property_value.cc',
        'interpolate_property_value.h',
        'interpolated_transform_property_value.cc',
        'interpolated_transform_property_value.h',
        'keyword_names.cc',
        'keyword_names.h',
        'keyword_value.cc',
        'keyword_value.h',
        'length_value.cc',
        'length_value.h',
        'linear_gradient_value.cc',
        'linear_gradient_value.h',
        'list_value.h',
        'local_src_value.cc',
        'local_src_value.h',
        'map_to_mesh_function.cc',
        'map_to_mesh_function.h',
        'matrix_function.cc',
        'matrix_function.h',
        'media_feature.cc',
        'media_feature.h',
        'media_feature_keyword_value.cc',
        'media_feature_keyword_value.h',
        'media_feature_keyword_value_names.cc',
        'media_feature_keyword_value_names.h',
        'media_feature_names.cc',
        'media_feature_names.h',
        'media_list.cc',
        'media_list.h',
        'media_query.cc',
        'media_query.h',
        'media_type_names.cc',
        'media_type_names.h',
        'mutation_observer.h',
        'next_sibling_combinator.cc',
        'next_sibling_combinator.h',
        'not_pseudo_class.cc',
        'not_pseudo_class.h',
        'number_value.cc',
        'number_value.h',
        'percentage_value.cc',
        'percentage_value.h',
        'property_definitions.cc',
        'property_definitions.h',
        'property_key_list_value.cc',
        'property_key_list_value.h',
        'property_list_value.h',
        'property_value.h',
        'property_value_visitor.cc',
        'property_value_visitor.h',
        'pseudo_class.h',
        'pseudo_class_names.cc',
        'pseudo_class_names.h',
        'pseudo_element.h',
        'pseudo_element_names.cc',
        'pseudo_element_names.h',
        'radial_gradient_value.cc',
        'radial_gradient_value.h',
        'ratio_value.cc',
        'ratio_value.h',
        'resolution_value.cc',
        'resolution_value.h',
        'rgba_color_value.cc',
        'rgba_color_value.h',
        'rotate_function.cc',
        'rotate_function.h',
        'scale_function.cc',
        'scale_function.h',
        'scoped_list_value.h',
        'scoped_ref_list_value.h',
        'selector.h',
        'selector_tree.cc',
        'selector_tree.h',
        'selector_visitor.h',
        'shadow_value.cc',
        'shadow_value.h',
        'simple_selector.h',
        'specificity.cc',
        'specificity.h',
        'serializer.cc',
        'serializer.h',
        'string_value.cc',
        'string_value.h',
        'style_sheet.cc',
        'style_sheet.h',
        'style_sheet_list.cc',
        'style_sheet_list.h',
        'testing/mock_css_parser.h',
        'time_list_value.cc',
        'time_list_value.h',
        'timing_function.cc',
        'timing_function.h',
        'timing_function_list_value.cc',
        'timing_function_list_value.h',
        'transform_function.h',
        'transform_function_list_value.cc',
        'transform_function_list_value.h',
        'transform_function_visitor.h',
        'transform_property_value.h',
        'translate_function.cc',
        'translate_function.h',
        'type_selector.cc',
        'type_selector.h',
        'unicode_range_value.cc',
        'unicode_range_value.h',
        'universal_selector.cc',
        'universal_selector.h',
        'user_agent_style_sheet.cc',
        'user_agent_style_sheet.h',
        'url_src_value.cc',
        'url_src_value.h',
        'url_value.cc',
        'url_value.h',
        'viewport_size.h',
      ],
      'export_dependent_settings': [
        # Additionally, ensure that the include directories for generated
        # headers are put on the include directories for targets that depend
        # on this one.
        '<(DEPTH)/cobalt/browser/browser_bindings_gen.gyp:generated_types',
      ],
      'dependencies': [
        '<(DEPTH)/cobalt/base/base.gyp:base',
        '<(DEPTH)/cobalt/network/network.gyp:network',
        '<(DEPTH)/cobalt/browser/browser_bindings_gen.gyp:generated_types',
        '<(DEPTH)/cobalt/dom/dom_exception.gyp:dom_exception',
        '<(DEPTH)/cobalt/math/math.gyp:math',
        '<(DEPTH)/cobalt/ui_navigation/ui_navigation.gyp:ui_navigation',
        '<(DEPTH)/url/url.gyp:url',
        'embed_resources_as_header_files',
      ],
    },

    {
      # This target takes all files in the embedded_resources directory (e.g.
      # the user agent style sheet) and embeds them as header files for
      # inclusion into the binary.
      'target_name': 'embed_resources_as_header_files',
      'type': 'none',
      # Because we generate a header, we must set the hard_dependency flag.
      'hard_dependency': 1,
      'variables': {
        'script_path': '<(DEPTH)/cobalt/build/generate_data_header.py',
        'output_path': '<(SHARED_INTERMEDIATE_DIR)/cobalt/cssom/embedded_resources.h',
        'input_directory': 'embedded_resources',
      },
      'sources': [
        '<(input_directory)/user_agent_style_sheet.css',
      ],
      'actions': [
        {
          'action_name': 'embed_resources_as_header_files',
          'inputs': [
            '<(script_path)',
            '<@(_sources)',
          ],
          'outputs': [
            '<(output_path)',
          ],
          'action': ['python', '<(script_path)', 'CSSOMEmbeddedResources', '<(output_path)', '<(input_directory)'],
          'message': 'Embedding cssom resources in "<(input_directory)" into header file, "<(output_path)".',
          'msvs_cygwin_shell': 1,
        },
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '<(SHARED_INTERMEDIATE_DIR)',
        ],
      },
    },
  ],
}
