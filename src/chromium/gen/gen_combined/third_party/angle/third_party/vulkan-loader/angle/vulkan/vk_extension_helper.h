// *** THIS FILE IS GENERATED - DO NOT EDIT ***
// See helper_file_generator.py for modifications


/***************************************************************************
 *
 * Copyright (c) 2015-2017 The Khronos Group Inc.
 * Copyright (c) 2015-2017 Valve Corporation
 * Copyright (c) 2015-2017 LunarG, Inc.
 * Copyright (c) 2015-2017 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Mark Lobodzinski <mark@lunarg.com>
 * Author: Courtney Goeltzenleuchter <courtneygo@google.com>
 * Author: Tobin Ehlis <tobine@google.com>
 * Author: Chris Forbes <chrisforbes@google.com>
 * Author: John Zulauf<jzulauf@lunarg.com>
 *
 ****************************************************************************/


#ifndef VK_EXTENSION_HELPER_H_
#define VK_EXTENSION_HELPER_H_
#include <string>
#include <unordered_map>
#include <utility>

#include <vulkan/vulkan.h>

struct InstanceExtensions {
    bool vk_ext_acquire_xlib_display{false};
    bool vk_ext_debug_report{false};
    bool vk_ext_debug_utils{false};
    bool vk_ext_direct_mode_display{false};
    bool vk_ext_display_surface_counter{false};
    bool vk_ext_metal_surface{false};
    bool vk_ext_swapchain_color_space{false};
    bool vk_ext_validation_features{false};
    bool vk_ext_validation_flags{false};
    bool vk_fuchsia_imagepipe_surface{false};
    bool vk_khr_android_surface{false};
    bool vk_khr_device_group_creation{false};
    bool vk_khr_display{false};
    bool vk_khr_external_fence_capabilities{false};
    bool vk_khr_external_memory_capabilities{false};
    bool vk_khr_external_semaphore_capabilities{false};
    bool vk_khr_get_display_properties_2{false};
    bool vk_khr_get_physical_device_properties_2{false};
    bool vk_khr_get_surface_capabilities_2{false};
    bool vk_khr_surface{false};
    bool vk_khr_wayland_surface{false};
    bool vk_khr_win32_surface{false};
    bool vk_khr_xcb_surface{false};
    bool vk_khr_xlib_surface{false};
    bool vk_mvk_ios_surface{false};
    bool vk_mvk_macos_surface{false};
    bool vk_nn_vi_surface{false};
    bool vk_nv_external_memory_capabilities{false};

    struct InstanceReq {
        const bool InstanceExtensions::* enabled;
        const char *name;
    };
    typedef std::vector<InstanceReq> InstanceReqVec;
    struct InstanceInfo {
       InstanceInfo(bool InstanceExtensions::* state_, const InstanceReqVec requires_): state(state_), requires(requires_) {}
       bool InstanceExtensions::* state;
       InstanceReqVec requires;
    };

    typedef std::unordered_map<std::string,InstanceInfo> InstanceInfoMap;
    static const InstanceInfo &get_info(const char *name) {
        static const InstanceInfoMap info_map = {
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
            std::make_pair(VK_EXT_ACQUIRE_XLIB_DISPLAY_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_ext_acquire_xlib_display, {{
                           {&InstanceExtensions::vk_ext_direct_mode_display, VK_EXT_DIRECT_MODE_DISPLAY_EXTENSION_NAME}}})),
#endif
            std::make_pair(VK_EXT_DEBUG_REPORT_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_ext_debug_report, {})),
            std::make_pair(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_ext_debug_utils, {})),
            std::make_pair(VK_EXT_DIRECT_MODE_DISPLAY_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_ext_direct_mode_display, {{
                           {&InstanceExtensions::vk_khr_display, VK_KHR_DISPLAY_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_DISPLAY_SURFACE_COUNTER_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_ext_display_surface_counter, {{
                           {&InstanceExtensions::vk_khr_display, VK_KHR_DISPLAY_EXTENSION_NAME}}})),
#ifdef VK_USE_PLATFORM_METAL_EXT
            std::make_pair(VK_EXT_METAL_SURFACE_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_ext_metal_surface, {{
                           {&InstanceExtensions::vk_khr_surface, VK_KHR_SURFACE_EXTENSION_NAME}}})),
#endif
            std::make_pair(VK_EXT_SWAPCHAIN_COLOR_SPACE_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_ext_swapchain_color_space, {{
                           {&InstanceExtensions::vk_khr_surface, VK_KHR_SURFACE_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_ext_validation_features, {})),
            std::make_pair(VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_ext_validation_flags, {})),
#ifdef VK_USE_PLATFORM_FUCHSIA
            std::make_pair(VK_FUCHSIA_IMAGEPIPE_SURFACE_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_fuchsia_imagepipe_surface, {{
                           {&InstanceExtensions::vk_khr_surface, VK_KHR_SURFACE_EXTENSION_NAME}}})),
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
            std::make_pair(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_khr_android_surface, {{
                           {&InstanceExtensions::vk_khr_surface, VK_KHR_SURFACE_EXTENSION_NAME}}})),
#endif
            std::make_pair(VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_khr_device_group_creation, {})),
            std::make_pair(VK_KHR_DISPLAY_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_khr_display, {{
                           {&InstanceExtensions::vk_khr_surface, VK_KHR_SURFACE_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_khr_external_fence_capabilities, {{
                           {&InstanceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_khr_external_memory_capabilities, {{
                           {&InstanceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_khr_external_semaphore_capabilities, {{
                           {&InstanceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_GET_DISPLAY_PROPERTIES_2_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_khr_get_display_properties_2, {{
                           {&InstanceExtensions::vk_khr_display, VK_KHR_DISPLAY_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_khr_get_physical_device_properties_2, {})),
            std::make_pair(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_khr_get_surface_capabilities_2, {{
                           {&InstanceExtensions::vk_khr_surface, VK_KHR_SURFACE_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_SURFACE_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_khr_surface, {})),
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
            std::make_pair(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_khr_wayland_surface, {{
                           {&InstanceExtensions::vk_khr_surface, VK_KHR_SURFACE_EXTENSION_NAME}}})),
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
            std::make_pair(VK_KHR_WIN32_SURFACE_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_khr_win32_surface, {{
                           {&InstanceExtensions::vk_khr_surface, VK_KHR_SURFACE_EXTENSION_NAME}}})),
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
            std::make_pair(VK_KHR_XCB_SURFACE_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_khr_xcb_surface, {{
                           {&InstanceExtensions::vk_khr_surface, VK_KHR_SURFACE_EXTENSION_NAME}}})),
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
            std::make_pair(VK_KHR_XLIB_SURFACE_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_khr_xlib_surface, {{
                           {&InstanceExtensions::vk_khr_surface, VK_KHR_SURFACE_EXTENSION_NAME}}})),
#endif
#ifdef VK_USE_PLATFORM_IOS_MVK
            std::make_pair(VK_MVK_IOS_SURFACE_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_mvk_ios_surface, {{
                           {&InstanceExtensions::vk_khr_surface, VK_KHR_SURFACE_EXTENSION_NAME}}})),
#endif
#ifdef VK_USE_PLATFORM_MACOS_MVK
            std::make_pair(VK_MVK_MACOS_SURFACE_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_mvk_macos_surface, {{
                           {&InstanceExtensions::vk_khr_surface, VK_KHR_SURFACE_EXTENSION_NAME}}})),
#endif
#ifdef VK_USE_PLATFORM_VI_NN
            std::make_pair(VK_NN_VI_SURFACE_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_nn_vi_surface, {{
                           {&InstanceExtensions::vk_khr_surface, VK_KHR_SURFACE_EXTENSION_NAME}}})),
#endif
            std::make_pair(VK_NV_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME, InstanceInfo(&InstanceExtensions::vk_nv_external_memory_capabilities, {})),
        };

        static const InstanceInfo empty_info {nullptr, InstanceReqVec()};
        InstanceInfoMap::const_iterator info = info_map.find(name);
        if ( info != info_map.cend()) {
            return info->second;
        }
        return empty_info;
    }

    uint32_t NormalizeApiVersion(uint32_t specified_version) {
        uint32_t api_version = (specified_version < VK_API_VERSION_1_1) ? VK_API_VERSION_1_0 : VK_API_VERSION_1_1;
        return api_version;
    }

    uint32_t InitFromInstanceCreateInfo(uint32_t requested_api_version, const VkInstanceCreateInfo *pCreateInfo) {

        static const std::vector<const char *> V_1_0_promoted_instance_extensions = {
            VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME,
            VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME,
            VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME,
            VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME,
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
        };

        // Initialize struct data, robust to invalid pCreateInfo
        if (pCreateInfo->ppEnabledExtensionNames) {
            for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
                if (!pCreateInfo->ppEnabledExtensionNames[i]) continue;
                auto info = get_info(pCreateInfo->ppEnabledExtensionNames[i]);
                if(info.state) this->*(info.state) = true;
            }
        }
        uint32_t api_version = NormalizeApiVersion(requested_api_version);
        if (api_version >= VK_API_VERSION_1_1) {
            for (auto promoted_ext : V_1_0_promoted_instance_extensions) {
                auto info = get_info(promoted_ext);
                assert(info.state);
                if (info.state) this->*(info.state) = true;
            }
        }
        return api_version;
    }
};

static const char * const kInstanceExtensionNames = 
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
    VK_EXT_ACQUIRE_XLIB_DISPLAY_EXTENSION_NAME
#endif
    VK_EXT_DEBUG_REPORT_EXTENSION_NAME
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    VK_EXT_DIRECT_MODE_DISPLAY_EXTENSION_NAME
    VK_EXT_DISPLAY_SURFACE_COUNTER_EXTENSION_NAME
#ifdef VK_USE_PLATFORM_METAL_EXT
    VK_EXT_METAL_SURFACE_EXTENSION_NAME
#endif
    VK_EXT_SWAPCHAIN_COLOR_SPACE_EXTENSION_NAME
    VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
    VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME
#ifdef VK_USE_PLATFORM_FUCHSIA
    VK_FUCHSIA_IMAGEPIPE_SURFACE_EXTENSION_NAME
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
#endif
    VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME
    VK_KHR_DISPLAY_EXTENSION_NAME
    VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME
    VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME
    VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME
    VK_KHR_GET_DISPLAY_PROPERTIES_2_EXTENSION_NAME
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
    VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME
    VK_KHR_SURFACE_EXTENSION_NAME
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
    VK_KHR_XCB_SURFACE_EXTENSION_NAME
#endif
#ifdef VK_USE_PLATFORM_XLIB_KHR
    VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#endif
#ifdef VK_USE_PLATFORM_IOS_MVK
    VK_MVK_IOS_SURFACE_EXTENSION_NAME
#endif
#ifdef VK_USE_PLATFORM_MACOS_MVK
    VK_MVK_MACOS_SURFACE_EXTENSION_NAME
#endif
#ifdef VK_USE_PLATFORM_VI_NN
    VK_NN_VI_SURFACE_EXTENSION_NAME
#endif
    VK_NV_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME
;

struct DeviceExtensions : public InstanceExtensions {
    bool vk_amd_buffer_marker{false};
    bool vk_amd_draw_indirect_count{false};
    bool vk_amd_gcn_shader{false};
    bool vk_amd_gpu_shader_half_float{false};
    bool vk_amd_gpu_shader_int16{false};
    bool vk_amd_memory_overallocation_behavior{false};
    bool vk_amd_mixed_attachment_samples{false};
    bool vk_amd_negative_viewport_height{false};
    bool vk_amd_rasterization_order{false};
    bool vk_amd_shader_ballot{false};
    bool vk_amd_shader_core_properties{false};
    bool vk_amd_shader_explicit_vertex_parameter{false};
    bool vk_amd_shader_fragment_mask{false};
    bool vk_amd_shader_image_load_store_lod{false};
    bool vk_amd_shader_info{false};
    bool vk_amd_shader_trinary_minmax{false};
    bool vk_amd_texture_gather_bias_lod{false};
    bool vk_android_external_memory_android_hardware_buffer{false};
    bool vk_ext_astc_decode_mode{false};
    bool vk_ext_blend_operation_advanced{false};
    bool vk_ext_buffer_device_address{false};
    bool vk_ext_calibrated_timestamps{false};
    bool vk_ext_conditional_rendering{false};
    bool vk_ext_conservative_rasterization{false};
    bool vk_ext_debug_marker{false};
    bool vk_ext_depth_clip_enable{false};
    bool vk_ext_depth_range_unrestricted{false};
    bool vk_ext_descriptor_indexing{false};
    bool vk_ext_discard_rectangles{false};
    bool vk_ext_display_control{false};
    bool vk_ext_external_memory_dma_buf{false};
    bool vk_ext_external_memory_host{false};
    bool vk_ext_filter_cubic{false};
    bool vk_ext_fragment_density_map{false};
    bool vk_ext_global_priority{false};
    bool vk_ext_hdr_metadata{false};
    bool vk_ext_image_drm_format_modifier{false};
    bool vk_ext_inline_uniform_block{false};
    bool vk_ext_memory_budget{false};
    bool vk_ext_memory_priority{false};
    bool vk_ext_pci_bus_info{false};
    bool vk_ext_post_depth_coverage{false};
    bool vk_ext_queue_family_foreign{false};
    bool vk_ext_sample_locations{false};
    bool vk_ext_sampler_filter_minmax{false};
    bool vk_ext_scalar_block_layout{false};
    bool vk_ext_separate_stencil_usage{false};
    bool vk_ext_shader_stencil_export{false};
    bool vk_ext_shader_subgroup_ballot{false};
    bool vk_ext_shader_subgroup_vote{false};
    bool vk_ext_shader_viewport_index_layer{false};
    bool vk_ext_transform_feedback{false};
    bool vk_ext_validation_cache{false};
    bool vk_ext_vertex_attribute_divisor{false};
    bool vk_ext_ycbcr_image_arrays{false};
    bool vk_google_decorate_string{false};
    bool vk_google_display_timing{false};
    bool vk_google_hlsl_functionality1{false};
    bool vk_img_filter_cubic{false};
    bool vk_img_format_pvrtc{false};
    bool vk_khr_16bit_storage{false};
    bool vk_khr_8bit_storage{false};
    bool vk_khr_bind_memory_2{false};
    bool vk_khr_create_renderpass_2{false};
    bool vk_khr_dedicated_allocation{false};
    bool vk_khr_depth_stencil_resolve{false};
    bool vk_khr_descriptor_update_template{false};
    bool vk_khr_device_group{false};
    bool vk_khr_display_swapchain{false};
    bool vk_khr_draw_indirect_count{false};
    bool vk_khr_driver_properties{false};
    bool vk_khr_external_fence{false};
    bool vk_khr_external_fence_fd{false};
    bool vk_khr_external_fence_win32{false};
    bool vk_khr_external_memory{false};
    bool vk_khr_external_memory_fd{false};
    bool vk_khr_external_memory_win32{false};
    bool vk_khr_external_semaphore{false};
    bool vk_khr_external_semaphore_fd{false};
    bool vk_khr_external_semaphore_win32{false};
    bool vk_khr_get_memory_requirements_2{false};
    bool vk_khr_image_format_list{false};
    bool vk_khr_incremental_present{false};
    bool vk_khr_maintenance1{false};
    bool vk_khr_maintenance2{false};
    bool vk_khr_maintenance3{false};
    bool vk_khr_multiview{false};
    bool vk_khr_push_descriptor{false};
    bool vk_khr_relaxed_block_layout{false};
    bool vk_khr_sampler_mirror_clamp_to_edge{false};
    bool vk_khr_sampler_ycbcr_conversion{false};
    bool vk_khr_shader_atomic_int64{false};
    bool vk_khr_shader_draw_parameters{false};
    bool vk_khr_shader_float16_int8{false};
    bool vk_khr_shader_float_controls{false};
    bool vk_khr_shared_presentable_image{false};
    bool vk_khr_storage_buffer_storage_class{false};
    bool vk_khr_swapchain{false};
    bool vk_khr_swapchain_mutable_format{false};
    bool vk_khr_variable_pointers{false};
    bool vk_khr_vulkan_memory_model{false};
    bool vk_khr_win32_keyed_mutex{false};
    bool vk_nvx_device_generated_commands{false};
    bool vk_nvx_image_view_handle{false};
    bool vk_nvx_multiview_per_view_attributes{false};
    bool vk_nv_clip_space_w_scaling{false};
    bool vk_nv_compute_shader_derivatives{false};
    bool vk_nv_cooperative_matrix{false};
    bool vk_nv_corner_sampled_image{false};
    bool vk_nv_dedicated_allocation{false};
    bool vk_nv_dedicated_allocation_image_aliasing{false};
    bool vk_nv_device_diagnostic_checkpoints{false};
    bool vk_nv_external_memory{false};
    bool vk_nv_external_memory_win32{false};
    bool vk_nv_fill_rectangle{false};
    bool vk_nv_fragment_coverage_to_color{false};
    bool vk_nv_fragment_shader_barycentric{false};
    bool vk_nv_framebuffer_mixed_samples{false};
    bool vk_nv_geometry_shader_passthrough{false};
    bool vk_nv_glsl_shader{false};
    bool vk_nv_mesh_shader{false};
    bool vk_nv_ray_tracing{false};
    bool vk_nv_representative_fragment_test{false};
    bool vk_nv_sample_mask_override_coverage{false};
    bool vk_nv_scissor_exclusive{false};
    bool vk_nv_shader_image_footprint{false};
    bool vk_nv_shader_subgroup_partitioned{false};
    bool vk_nv_shading_rate_image{false};
    bool vk_nv_viewport_array2{false};
    bool vk_nv_viewport_swizzle{false};
    bool vk_nv_win32_keyed_mutex{false};

    struct DeviceReq {
        const bool DeviceExtensions::* enabled;
        const char *name;
    };
    typedef std::vector<DeviceReq> DeviceReqVec;
    struct DeviceInfo {
       DeviceInfo(bool DeviceExtensions::* state_, const DeviceReqVec requires_): state(state_), requires(requires_) {}
       bool DeviceExtensions::* state;
       DeviceReqVec requires;
    };

    typedef std::unordered_map<std::string,DeviceInfo> DeviceInfoMap;
    static const DeviceInfo &get_info(const char *name) {
        static const DeviceInfoMap info_map = {
            std::make_pair(VK_AMD_BUFFER_MARKER_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_buffer_marker, {})),
            std::make_pair(VK_AMD_DRAW_INDIRECT_COUNT_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_draw_indirect_count, {})),
            std::make_pair(VK_AMD_GCN_SHADER_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_gcn_shader, {})),
            std::make_pair(VK_AMD_GPU_SHADER_HALF_FLOAT_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_gpu_shader_half_float, {})),
            std::make_pair(VK_AMD_GPU_SHADER_INT16_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_gpu_shader_int16, {})),
            std::make_pair(VK_AMD_MEMORY_OVERALLOCATION_BEHAVIOR_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_memory_overallocation_behavior, {})),
            std::make_pair(VK_AMD_MIXED_ATTACHMENT_SAMPLES_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_mixed_attachment_samples, {})),
            std::make_pair(VK_AMD_NEGATIVE_VIEWPORT_HEIGHT_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_negative_viewport_height, {})),
            std::make_pair(VK_AMD_RASTERIZATION_ORDER_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_rasterization_order, {})),
            std::make_pair(VK_AMD_SHADER_BALLOT_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_shader_ballot, {})),
            std::make_pair(VK_AMD_SHADER_CORE_PROPERTIES_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_shader_core_properties, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_AMD_SHADER_EXPLICIT_VERTEX_PARAMETER_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_shader_explicit_vertex_parameter, {})),
            std::make_pair(VK_AMD_SHADER_FRAGMENT_MASK_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_shader_fragment_mask, {})),
            std::make_pair(VK_AMD_SHADER_IMAGE_LOAD_STORE_LOD_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_shader_image_load_store_lod, {})),
            std::make_pair(VK_AMD_SHADER_INFO_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_shader_info, {})),
            std::make_pair(VK_AMD_SHADER_TRINARY_MINMAX_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_shader_trinary_minmax, {})),
            std::make_pair(VK_AMD_TEXTURE_GATHER_BIAS_LOD_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_amd_texture_gather_bias_lod, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
#ifdef VK_USE_PLATFORM_ANDROID_KHR
            std::make_pair(VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_android_external_memory_android_hardware_buffer, {{
                           {&DeviceExtensions::vk_khr_sampler_ycbcr_conversion, VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_external_memory, VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME},
                           {&DeviceExtensions::vk_ext_queue_family_foreign, VK_EXT_QUEUE_FAMILY_FOREIGN_EXTENSION_NAME}}})),
#endif
            std::make_pair(VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_astc_decode_mode, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_blend_operation_advanced, {})),
            std::make_pair(VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_buffer_device_address, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_CALIBRATED_TIMESTAMPS_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_calibrated_timestamps, {})),
            std::make_pair(VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_conditional_rendering, {})),
            std::make_pair(VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_conservative_rasterization, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_DEBUG_MARKER_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_debug_marker, {{
                           {&DeviceExtensions::vk_ext_debug_report, VK_EXT_DEBUG_REPORT_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_depth_clip_enable, {})),
            std::make_pair(VK_EXT_DEPTH_RANGE_UNRESTRICTED_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_depth_range_unrestricted, {})),
            std::make_pair(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_descriptor_indexing, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_maintenance3, VK_KHR_MAINTENANCE3_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_discard_rectangles, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_DISPLAY_CONTROL_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_display_control, {{
                           {&DeviceExtensions::vk_ext_display_surface_counter, VK_EXT_DISPLAY_SURFACE_COUNTER_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_swapchain, VK_KHR_SWAPCHAIN_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_EXTERNAL_MEMORY_DMA_BUF_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_external_memory_dma_buf, {{
                           {&DeviceExtensions::vk_khr_external_memory_fd, VK_KHR_EXTERNAL_MEMORY_FD_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_external_memory_host, {{
                           {&DeviceExtensions::vk_khr_external_memory, VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_FILTER_CUBIC_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_filter_cubic, {{
                           {&DeviceExtensions::vk_img_filter_cubic, VK_IMG_FILTER_CUBIC_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_fragment_density_map, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_global_priority, {})),
            std::make_pair(VK_EXT_HDR_METADATA_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_hdr_metadata, {{
                           {&DeviceExtensions::vk_khr_swapchain, VK_KHR_SWAPCHAIN_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_image_drm_format_modifier, {{
                           {&DeviceExtensions::vk_khr_bind_memory_2, VK_KHR_BIND_MEMORY_2_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_image_format_list, VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_sampler_ycbcr_conversion, VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_inline_uniform_block, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_maintenance1, VK_KHR_MAINTENANCE1_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_memory_budget, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_memory_priority, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_PCI_BUS_INFO_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_pci_bus_info, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_POST_DEPTH_COVERAGE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_post_depth_coverage, {})),
            std::make_pair(VK_EXT_QUEUE_FAMILY_FOREIGN_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_queue_family_foreign, {{
                           {&DeviceExtensions::vk_khr_external_memory, VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_sample_locations, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_sampler_filter_minmax, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_scalar_block_layout, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_SEPARATE_STENCIL_USAGE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_separate_stencil_usage, {})),
            std::make_pair(VK_EXT_SHADER_STENCIL_EXPORT_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_shader_stencil_export, {})),
            std::make_pair(VK_EXT_SHADER_SUBGROUP_BALLOT_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_shader_subgroup_ballot, {})),
            std::make_pair(VK_EXT_SHADER_SUBGROUP_VOTE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_shader_subgroup_vote, {})),
            std::make_pair(VK_EXT_SHADER_VIEWPORT_INDEX_LAYER_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_shader_viewport_index_layer, {})),
            std::make_pair(VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_transform_feedback, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_VALIDATION_CACHE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_validation_cache, {})),
            std::make_pair(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_vertex_attribute_divisor, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_EXT_YCBCR_IMAGE_ARRAYS_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_ext_ycbcr_image_arrays, {{
                           {&DeviceExtensions::vk_khr_sampler_ycbcr_conversion, VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME}}})),
            std::make_pair(VK_GOOGLE_DECORATE_STRING_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_google_decorate_string, {})),
            std::make_pair(VK_GOOGLE_DISPLAY_TIMING_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_google_display_timing, {{
                           {&DeviceExtensions::vk_khr_swapchain, VK_KHR_SWAPCHAIN_EXTENSION_NAME}}})),
            std::make_pair(VK_GOOGLE_HLSL_FUNCTIONALITY1_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_google_hlsl_functionality1, {})),
            std::make_pair(VK_IMG_FILTER_CUBIC_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_img_filter_cubic, {})),
            std::make_pair(VK_IMG_FORMAT_PVRTC_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_img_format_pvrtc, {})),
            std::make_pair(VK_KHR_16BIT_STORAGE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_16bit_storage, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_storage_buffer_storage_class, VK_KHR_STORAGE_BUFFER_STORAGE_CLASS_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_8BIT_STORAGE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_8bit_storage, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_storage_buffer_storage_class, VK_KHR_STORAGE_BUFFER_STORAGE_CLASS_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_BIND_MEMORY_2_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_bind_memory_2, {})),
            std::make_pair(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_create_renderpass_2, {{
                           {&DeviceExtensions::vk_khr_multiview, VK_KHR_MULTIVIEW_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_maintenance2, VK_KHR_MAINTENANCE2_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_dedicated_allocation, {{
                           {&DeviceExtensions::vk_khr_get_memory_requirements_2, VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_depth_stencil_resolve, {{
                           {&DeviceExtensions::vk_khr_create_renderpass_2, VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_descriptor_update_template, {})),
            std::make_pair(VK_KHR_DEVICE_GROUP_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_device_group, {{
                           {&DeviceExtensions::vk_khr_device_group_creation, VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_display_swapchain, {{
                           {&DeviceExtensions::vk_khr_swapchain, VK_KHR_SWAPCHAIN_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_display, VK_KHR_DISPLAY_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_DRAW_INDIRECT_COUNT_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_draw_indirect_count, {})),
            std::make_pair(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_driver_properties, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_external_fence, {{
                           {&DeviceExtensions::vk_khr_external_fence_capabilities, VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_EXTERNAL_FENCE_FD_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_external_fence_fd, {{
                           {&DeviceExtensions::vk_khr_external_fence, VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME}}})),
#ifdef VK_USE_PLATFORM_WIN32_KHR
            std::make_pair(VK_KHR_EXTERNAL_FENCE_WIN32_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_external_fence_win32, {{
                           {&DeviceExtensions::vk_khr_external_fence, VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME}}})),
#endif
            std::make_pair(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_external_memory, {{
                           {&DeviceExtensions::vk_khr_external_memory_capabilities, VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_EXTERNAL_MEMORY_FD_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_external_memory_fd, {{
                           {&DeviceExtensions::vk_khr_external_memory, VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME}}})),
#ifdef VK_USE_PLATFORM_WIN32_KHR
            std::make_pair(VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_external_memory_win32, {{
                           {&DeviceExtensions::vk_khr_external_memory, VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME}}})),
#endif
            std::make_pair(VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_external_semaphore, {{
                           {&DeviceExtensions::vk_khr_external_semaphore_capabilities, VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_external_semaphore_fd, {{
                           {&DeviceExtensions::vk_khr_external_semaphore, VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME}}})),
#ifdef VK_USE_PLATFORM_WIN32_KHR
            std::make_pair(VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_external_semaphore_win32, {{
                           {&DeviceExtensions::vk_khr_external_semaphore, VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME}}})),
#endif
            std::make_pair(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_get_memory_requirements_2, {})),
            std::make_pair(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_image_format_list, {})),
            std::make_pair(VK_KHR_INCREMENTAL_PRESENT_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_incremental_present, {{
                           {&DeviceExtensions::vk_khr_swapchain, VK_KHR_SWAPCHAIN_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_MAINTENANCE1_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_maintenance1, {})),
            std::make_pair(VK_KHR_MAINTENANCE2_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_maintenance2, {})),
            std::make_pair(VK_KHR_MAINTENANCE3_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_maintenance3, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_MULTIVIEW_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_multiview, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_push_descriptor, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_RELAXED_BLOCK_LAYOUT_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_relaxed_block_layout, {})),
            std::make_pair(VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_sampler_mirror_clamp_to_edge, {})),
            std::make_pair(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_sampler_ycbcr_conversion, {{
                           {&DeviceExtensions::vk_khr_maintenance1, VK_KHR_MAINTENANCE1_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_bind_memory_2, VK_KHR_BIND_MEMORY_2_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_get_memory_requirements_2, VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_SHADER_ATOMIC_INT64_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_shader_atomic_int64, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_shader_draw_parameters, {})),
            std::make_pair(VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_shader_float16_int8, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_shader_float_controls, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_SHARED_PRESENTABLE_IMAGE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_shared_presentable_image, {{
                           {&DeviceExtensions::vk_khr_swapchain, VK_KHR_SWAPCHAIN_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_get_surface_capabilities_2, VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_STORAGE_BUFFER_STORAGE_CLASS_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_storage_buffer_storage_class, {})),
            std::make_pair(VK_KHR_SWAPCHAIN_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_swapchain, {{
                           {&DeviceExtensions::vk_khr_surface, VK_KHR_SURFACE_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_swapchain_mutable_format, {{
                           {&DeviceExtensions::vk_khr_swapchain, VK_KHR_SWAPCHAIN_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_maintenance2, VK_KHR_MAINTENANCE2_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_image_format_list, VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_variable_pointers, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_storage_buffer_storage_class, VK_KHR_STORAGE_BUFFER_STORAGE_CLASS_EXTENSION_NAME}}})),
            std::make_pair(VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_vulkan_memory_model, {})),
#ifdef VK_USE_PLATFORM_WIN32_KHR
            std::make_pair(VK_KHR_WIN32_KEYED_MUTEX_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_khr_win32_keyed_mutex, {{
                           {&DeviceExtensions::vk_khr_external_memory_win32, VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME}}})),
#endif
            std::make_pair(VK_NVX_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nvx_device_generated_commands, {})),
            std::make_pair(VK_NVX_IMAGE_VIEW_HANDLE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nvx_image_view_handle, {})),
            std::make_pair(VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nvx_multiview_per_view_attributes, {{
                           {&DeviceExtensions::vk_khr_multiview, VK_KHR_MULTIVIEW_EXTENSION_NAME}}})),
            std::make_pair(VK_NV_CLIP_SPACE_W_SCALING_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_clip_space_w_scaling, {})),
            std::make_pair(VK_NV_COMPUTE_SHADER_DERIVATIVES_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_compute_shader_derivatives, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_cooperative_matrix, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_NV_CORNER_SAMPLED_IMAGE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_corner_sampled_image, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_dedicated_allocation, {})),
            std::make_pair(VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_dedicated_allocation_image_aliasing, {{
                           {&DeviceExtensions::vk_khr_dedicated_allocation, VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME}}})),
            std::make_pair(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_device_diagnostic_checkpoints, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_NV_EXTERNAL_MEMORY_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_external_memory, {{
                           {&DeviceExtensions::vk_nv_external_memory_capabilities, VK_NV_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME}}})),
#ifdef VK_USE_PLATFORM_WIN32_KHR
            std::make_pair(VK_NV_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_external_memory_win32, {{
                           {&DeviceExtensions::vk_nv_external_memory, VK_NV_EXTERNAL_MEMORY_EXTENSION_NAME}}})),
#endif
            std::make_pair(VK_NV_FILL_RECTANGLE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_fill_rectangle, {})),
            std::make_pair(VK_NV_FRAGMENT_COVERAGE_TO_COLOR_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_fragment_coverage_to_color, {})),
            std::make_pair(VK_NV_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_fragment_shader_barycentric, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_NV_FRAMEBUFFER_MIXED_SAMPLES_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_framebuffer_mixed_samples, {})),
            std::make_pair(VK_NV_GEOMETRY_SHADER_PASSTHROUGH_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_geometry_shader_passthrough, {})),
            std::make_pair(VK_NV_GLSL_SHADER_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_glsl_shader, {})),
            std::make_pair(VK_NV_MESH_SHADER_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_mesh_shader, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_NV_RAY_TRACING_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_ray_tracing, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME},
                           {&DeviceExtensions::vk_khr_get_memory_requirements_2, VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME}}})),
            std::make_pair(VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_representative_fragment_test, {})),
            std::make_pair(VK_NV_SAMPLE_MASK_OVERRIDE_COVERAGE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_sample_mask_override_coverage, {})),
            std::make_pair(VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_scissor_exclusive, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_NV_SHADER_IMAGE_FOOTPRINT_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_shader_image_footprint, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_NV_SHADER_SUBGROUP_PARTITIONED_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_shader_subgroup_partitioned, {})),
            std::make_pair(VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_shading_rate_image, {{
                           {&DeviceExtensions::vk_khr_get_physical_device_properties_2, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME}}})),
            std::make_pair(VK_NV_VIEWPORT_ARRAY2_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_viewport_array2, {})),
            std::make_pair(VK_NV_VIEWPORT_SWIZZLE_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_viewport_swizzle, {})),
#ifdef VK_USE_PLATFORM_WIN32_KHR
            std::make_pair(VK_NV_WIN32_KEYED_MUTEX_EXTENSION_NAME, DeviceInfo(&DeviceExtensions::vk_nv_win32_keyed_mutex, {{
                           {&DeviceExtensions::vk_nv_external_memory_win32, VK_NV_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME}}})),
#endif
        };

        static const DeviceInfo empty_info {nullptr, DeviceReqVec()};
        DeviceInfoMap::const_iterator info = info_map.find(name);
        if ( info != info_map.cend()) {
            return info->second;
        }
        return empty_info;
    }

    DeviceExtensions() = default;
    DeviceExtensions(const InstanceExtensions& instance_ext) : InstanceExtensions(instance_ext) {}

    uint32_t InitFromDeviceCreateInfo(const InstanceExtensions *instance_extensions, uint32_t requested_api_version,
                                      const VkDeviceCreateInfo *pCreateInfo) {
        // Initialize: this to defaults,  base class fields to input.
        assert(instance_extensions);
        *this = DeviceExtensions(*instance_extensions);

        static const std::vector<const char *> V_1_0_promoted_device_extensions = {
            VK_KHR_16BIT_STORAGE_EXTENSION_NAME,
            VK_KHR_BIND_MEMORY_2_EXTENSION_NAME,
            VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,
            VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME,
            VK_KHR_DEVICE_GROUP_EXTENSION_NAME,
            VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME,
            VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME,
            VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME,
            VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
            VK_KHR_MAINTENANCE1_EXTENSION_NAME,
            VK_KHR_MAINTENANCE2_EXTENSION_NAME,
            VK_KHR_MAINTENANCE3_EXTENSION_NAME,
            VK_KHR_MULTIVIEW_EXTENSION_NAME,
            VK_KHR_RELAXED_BLOCK_LAYOUT_EXTENSION_NAME,
            VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME,
            VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME,
            VK_KHR_STORAGE_BUFFER_STORAGE_CLASS_EXTENSION_NAME,
            VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME,
        };

        // Initialize struct data, robust to invalid pCreateInfo
        if (pCreateInfo->ppEnabledExtensionNames) {
            for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
                if (!pCreateInfo->ppEnabledExtensionNames[i]) continue;
                auto info = get_info(pCreateInfo->ppEnabledExtensionNames[i]);
                if(info.state) this->*(info.state) = true;
            }
        }
        uint32_t api_version = NormalizeApiVersion(requested_api_version);
        if (api_version >= VK_API_VERSION_1_1) {
            for (auto promoted_ext : V_1_0_promoted_device_extensions) {
                auto info = get_info(promoted_ext);
                assert(info.state);
                if (info.state) this->*(info.state) = true;
            }
        }
        return api_version;
    }
};

static const char * const kDeviceExtensionNames = 
    VK_AMD_BUFFER_MARKER_EXTENSION_NAME
    VK_AMD_DRAW_INDIRECT_COUNT_EXTENSION_NAME
    VK_AMD_GCN_SHADER_EXTENSION_NAME
    VK_AMD_GPU_SHADER_HALF_FLOAT_EXTENSION_NAME
    VK_AMD_GPU_SHADER_INT16_EXTENSION_NAME
    VK_AMD_MEMORY_OVERALLOCATION_BEHAVIOR_EXTENSION_NAME
    VK_AMD_MIXED_ATTACHMENT_SAMPLES_EXTENSION_NAME
    VK_AMD_NEGATIVE_VIEWPORT_HEIGHT_EXTENSION_NAME
    VK_AMD_RASTERIZATION_ORDER_EXTENSION_NAME
    VK_AMD_SHADER_BALLOT_EXTENSION_NAME
    VK_AMD_SHADER_CORE_PROPERTIES_EXTENSION_NAME
    VK_AMD_SHADER_EXPLICIT_VERTEX_PARAMETER_EXTENSION_NAME
    VK_AMD_SHADER_FRAGMENT_MASK_EXTENSION_NAME
    VK_AMD_SHADER_IMAGE_LOAD_STORE_LOD_EXTENSION_NAME
    VK_AMD_SHADER_INFO_EXTENSION_NAME
    VK_AMD_SHADER_TRINARY_MINMAX_EXTENSION_NAME
    VK_AMD_TEXTURE_GATHER_BIAS_LOD_EXTENSION_NAME
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME
#endif
    VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME
    VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
    VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
    VK_EXT_CALIBRATED_TIMESTAMPS_EXTENSION_NAME
    VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
    VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
    VK_EXT_DEBUG_MARKER_EXTENSION_NAME
    VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
    VK_EXT_DEPTH_RANGE_UNRESTRICTED_EXTENSION_NAME
    VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME
    VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
    VK_EXT_DISPLAY_CONTROL_EXTENSION_NAME
    VK_EXT_EXTERNAL_MEMORY_DMA_BUF_EXTENSION_NAME
    VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME
    VK_EXT_FILTER_CUBIC_EXTENSION_NAME
    VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
    VK_EXT_HDR_METADATA_EXTENSION_NAME
    VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
    VK_EXT_MEMORY_BUDGET_EXTENSION_NAME
    VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME
    VK_EXT_PCI_BUS_INFO_EXTENSION_NAME
    VK_EXT_POST_DEPTH_COVERAGE_EXTENSION_NAME
    VK_EXT_QUEUE_FAMILY_FOREIGN_EXTENSION_NAME
    VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
    VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME
    VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME
    VK_EXT_SEPARATE_STENCIL_USAGE_EXTENSION_NAME
    VK_EXT_SHADER_STENCIL_EXPORT_EXTENSION_NAME
    VK_EXT_SHADER_SUBGROUP_BALLOT_EXTENSION_NAME
    VK_EXT_SHADER_SUBGROUP_VOTE_EXTENSION_NAME
    VK_EXT_SHADER_VIEWPORT_INDEX_LAYER_EXTENSION_NAME
    VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    VK_EXT_YCBCR_IMAGE_ARRAYS_EXTENSION_NAME
    VK_GOOGLE_DECORATE_STRING_EXTENSION_NAME
    VK_GOOGLE_DISPLAY_TIMING_EXTENSION_NAME
    VK_GOOGLE_HLSL_FUNCTIONALITY1_EXTENSION_NAME
    VK_IMG_FILTER_CUBIC_EXTENSION_NAME
    VK_IMG_FORMAT_PVRTC_EXTENSION_NAME
    VK_KHR_16BIT_STORAGE_EXTENSION_NAME
    VK_KHR_8BIT_STORAGE_EXTENSION_NAME
    VK_KHR_BIND_MEMORY_2_EXTENSION_NAME
    VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME
    VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME
    VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME
    VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME
    VK_KHR_DEVICE_GROUP_EXTENSION_NAME
    VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME
    VK_KHR_DRAW_INDIRECT_COUNT_EXTENSION_NAME
    VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME
    VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME
    VK_KHR_EXTERNAL_FENCE_FD_EXTENSION_NAME
#ifdef VK_USE_PLATFORM_WIN32_KHR
    VK_KHR_EXTERNAL_FENCE_WIN32_EXTENSION_NAME
#endif
    VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME
    VK_KHR_EXTERNAL_MEMORY_FD_EXTENSION_NAME
#ifdef VK_USE_PLATFORM_WIN32_KHR
    VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME
#endif
    VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME
    VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME
#ifdef VK_USE_PLATFORM_WIN32_KHR
    VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME
#endif
    VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME
    VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME
    VK_KHR_INCREMENTAL_PRESENT_EXTENSION_NAME
    VK_KHR_MAINTENANCE1_EXTENSION_NAME
    VK_KHR_MAINTENANCE2_EXTENSION_NAME
    VK_KHR_MAINTENANCE3_EXTENSION_NAME
    VK_KHR_MULTIVIEW_EXTENSION_NAME
    VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
    VK_KHR_RELAXED_BLOCK_LAYOUT_EXTENSION_NAME
    VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME
    VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME
    VK_KHR_SHADER_ATOMIC_INT64_EXTENSION_NAME
    VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME
    VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME
    VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME
    VK_KHR_SHARED_PRESENTABLE_IMAGE_EXTENSION_NAME
    VK_KHR_STORAGE_BUFFER_STORAGE_CLASS_EXTENSION_NAME
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
    VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME
    VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME
    VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME
#ifdef VK_USE_PLATFORM_WIN32_KHR
    VK_KHR_WIN32_KEYED_MUTEX_EXTENSION_NAME
#endif
    VK_NVX_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    VK_NVX_IMAGE_VIEW_HANDLE_EXTENSION_NAME
    VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME
    VK_NV_CLIP_SPACE_W_SCALING_EXTENSION_NAME
    VK_NV_COMPUTE_SHADER_DERIVATIVES_EXTENSION_NAME
    VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
    VK_NV_CORNER_SAMPLED_IMAGE_EXTENSION_NAME
    VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
    VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING_EXTENSION_NAME
    VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME
    VK_NV_EXTERNAL_MEMORY_EXTENSION_NAME
#ifdef VK_USE_PLATFORM_WIN32_KHR
    VK_NV_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME
#endif
    VK_NV_FILL_RECTANGLE_EXTENSION_NAME
    VK_NV_FRAGMENT_COVERAGE_TO_COLOR_EXTENSION_NAME
    VK_NV_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME
    VK_NV_FRAMEBUFFER_MIXED_SAMPLES_EXTENSION_NAME
    VK_NV_GEOMETRY_SHADER_PASSTHROUGH_EXTENSION_NAME
    VK_NV_GLSL_SHADER_EXTENSION_NAME
    VK_NV_MESH_SHADER_EXTENSION_NAME
    VK_NV_RAY_TRACING_EXTENSION_NAME
    VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
    VK_NV_SAMPLE_MASK_OVERRIDE_COVERAGE_EXTENSION_NAME
    VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    VK_NV_SHADER_IMAGE_FOOTPRINT_EXTENSION_NAME
    VK_NV_SHADER_SUBGROUP_PARTITIONED_EXTENSION_NAME
    VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    VK_NV_VIEWPORT_ARRAY2_EXTENSION_NAME
    VK_NV_VIEWPORT_SWIZZLE_EXTENSION_NAME
#ifdef VK_USE_PLATFORM_WIN32_KHR
    VK_NV_WIN32_KEYED_MUTEX_EXTENSION_NAME
#endif
;


#endif // VK_EXTENSION_HELPER_H_
