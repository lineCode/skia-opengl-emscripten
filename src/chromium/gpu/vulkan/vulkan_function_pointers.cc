// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// This file is auto-generated from
// gpu/vulkan/generate_bindings.py
// It's formatted by clang-format using chromium coding style:
//    clang-format -i -style=chromium filename
// DO NOT EDIT!

#include "gpu/vulkan/vulkan_function_pointers.h"

#include "base/no_destructor.h"

namespace gpu {

VulkanFunctionPointers* GetVulkanFunctionPointers() {
  static base::NoDestructor<VulkanFunctionPointers> vulkan_function_pointers;
  return vulkan_function_pointers.get();
}

VulkanFunctionPointers::VulkanFunctionPointers() = default;
VulkanFunctionPointers::~VulkanFunctionPointers() = default;

bool VulkanFunctionPointers::BindUnassociatedFunctionPointers() {
  // vkGetInstanceProcAddr must be handled specially since it gets its function
  // pointer through base::GetFunctionPOinterFromNativeLibrary(). Other Vulkan
  // functions don't do this.
  vkGetInstanceProcAddrFn = reinterpret_cast<PFN_vkGetInstanceProcAddr>(
      base::GetFunctionPointerFromNativeLibrary(vulkan_loader_library_,
                                                "vkGetInstanceProcAddr"));
  if (!vkGetInstanceProcAddrFn)
    return false;

  vkEnumerateInstanceVersionFn =
      reinterpret_cast<PFN_vkEnumerateInstanceVersion>(
          vkGetInstanceProcAddrFn(nullptr, "vkEnumerateInstanceVersion"));
  // vkEnumerateInstanceVersion didn't exist in Vulkan 1.0, so we should
  // proceed even if we fail to get vkEnumerateInstanceVersion pointer.
  vkCreateInstanceFn = reinterpret_cast<PFN_vkCreateInstance>(
      vkGetInstanceProcAddrFn(nullptr, "vkCreateInstance"));
  if (!vkCreateInstanceFn)
    return false;

  vkEnumerateInstanceExtensionPropertiesFn =
      reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(
          vkGetInstanceProcAddrFn(nullptr,
                                  "vkEnumerateInstanceExtensionProperties"));
  if (!vkEnumerateInstanceExtensionPropertiesFn)
    return false;

  vkEnumerateInstanceLayerPropertiesFn =
      reinterpret_cast<PFN_vkEnumerateInstanceLayerProperties>(
          vkGetInstanceProcAddrFn(nullptr,
                                  "vkEnumerateInstanceLayerProperties"));
  if (!vkEnumerateInstanceLayerPropertiesFn)
    return false;

  return true;
}

bool VulkanFunctionPointers::BindInstanceFunctionPointers(
    VkInstance vk_instance) {
  vkDestroyInstanceFn = reinterpret_cast<PFN_vkDestroyInstance>(
      vkGetInstanceProcAddrFn(vk_instance, "vkDestroyInstance"));
  if (!vkDestroyInstanceFn)
    return false;

  vkEnumeratePhysicalDevicesFn =
      reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(
          vkGetInstanceProcAddrFn(vk_instance, "vkEnumeratePhysicalDevices"));
  if (!vkEnumeratePhysicalDevicesFn)
    return false;

  vkGetDeviceProcAddrFn = reinterpret_cast<PFN_vkGetDeviceProcAddr>(
      vkGetInstanceProcAddrFn(vk_instance, "vkGetDeviceProcAddr"));
  if (!vkGetDeviceProcAddrFn)
    return false;

  return true;
}

bool VulkanFunctionPointers::BindPhysicalDeviceFunctionPointers(
    VkInstance vk_instance) {
  vkCreateDeviceFn = reinterpret_cast<PFN_vkCreateDevice>(
      vkGetInstanceProcAddrFn(vk_instance, "vkCreateDevice"));
  if (!vkCreateDeviceFn)
    return false;

  vkEnumerateDeviceLayerPropertiesFn =
      reinterpret_cast<PFN_vkEnumerateDeviceLayerProperties>(
          vkGetInstanceProcAddrFn(vk_instance,
                                  "vkEnumerateDeviceLayerProperties"));
  if (!vkEnumerateDeviceLayerPropertiesFn)
    return false;

  vkGetPhysicalDeviceQueueFamilyPropertiesFn =
      reinterpret_cast<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(
          vkGetInstanceProcAddrFn(vk_instance,
                                  "vkGetPhysicalDeviceQueueFamilyProperties"));
  if (!vkGetPhysicalDeviceQueueFamilyPropertiesFn)
    return false;

  vkGetPhysicalDevicePropertiesFn =
      reinterpret_cast<PFN_vkGetPhysicalDeviceProperties>(
          vkGetInstanceProcAddrFn(vk_instance,
                                  "vkGetPhysicalDeviceProperties"));
  if (!vkGetPhysicalDevicePropertiesFn)
    return false;

  return true;
}

bool VulkanFunctionPointers::BindDeviceFunctionPointers(VkDevice vk_device) {
  // Device functions
  vkAllocateCommandBuffersFn = reinterpret_cast<PFN_vkAllocateCommandBuffers>(
      vkGetDeviceProcAddrFn(vk_device, "vkAllocateCommandBuffers"));
  if (!vkAllocateCommandBuffersFn)
    return false;

  vkAllocateDescriptorSetsFn = reinterpret_cast<PFN_vkAllocateDescriptorSets>(
      vkGetDeviceProcAddrFn(vk_device, "vkAllocateDescriptorSets"));
  if (!vkAllocateDescriptorSetsFn)
    return false;

  vkAllocateMemoryFn = reinterpret_cast<PFN_vkAllocateMemory>(
      vkGetDeviceProcAddrFn(vk_device, "vkAllocateMemory"));
  if (!vkAllocateMemoryFn)
    return false;

  vkBindImageMemoryFn = reinterpret_cast<PFN_vkBindImageMemory>(
      vkGetDeviceProcAddrFn(vk_device, "vkBindImageMemory"));
  if (!vkBindImageMemoryFn)
    return false;

  vkCreateCommandPoolFn = reinterpret_cast<PFN_vkCreateCommandPool>(
      vkGetDeviceProcAddrFn(vk_device, "vkCreateCommandPool"));
  if (!vkCreateCommandPoolFn)
    return false;

  vkCreateDescriptorPoolFn = reinterpret_cast<PFN_vkCreateDescriptorPool>(
      vkGetDeviceProcAddrFn(vk_device, "vkCreateDescriptorPool"));
  if (!vkCreateDescriptorPoolFn)
    return false;

  vkCreateDescriptorSetLayoutFn =
      reinterpret_cast<PFN_vkCreateDescriptorSetLayout>(
          vkGetDeviceProcAddrFn(vk_device, "vkCreateDescriptorSetLayout"));
  if (!vkCreateDescriptorSetLayoutFn)
    return false;

  vkCreateFenceFn = reinterpret_cast<PFN_vkCreateFence>(
      vkGetDeviceProcAddrFn(vk_device, "vkCreateFence"));
  if (!vkCreateFenceFn)
    return false;

  vkCreateFramebufferFn = reinterpret_cast<PFN_vkCreateFramebuffer>(
      vkGetDeviceProcAddrFn(vk_device, "vkCreateFramebuffer"));
  if (!vkCreateFramebufferFn)
    return false;

  vkCreateImageFn = reinterpret_cast<PFN_vkCreateImage>(
      vkGetDeviceProcAddrFn(vk_device, "vkCreateImage"));
  if (!vkCreateImageFn)
    return false;

  vkCreateImageViewFn = reinterpret_cast<PFN_vkCreateImageView>(
      vkGetDeviceProcAddrFn(vk_device, "vkCreateImageView"));
  if (!vkCreateImageViewFn)
    return false;

  vkCreateRenderPassFn = reinterpret_cast<PFN_vkCreateRenderPass>(
      vkGetDeviceProcAddrFn(vk_device, "vkCreateRenderPass"));
  if (!vkCreateRenderPassFn)
    return false;

  vkCreateSamplerFn = reinterpret_cast<PFN_vkCreateSampler>(
      vkGetDeviceProcAddrFn(vk_device, "vkCreateSampler"));
  if (!vkCreateSamplerFn)
    return false;

  vkCreateSemaphoreFn = reinterpret_cast<PFN_vkCreateSemaphore>(
      vkGetDeviceProcAddrFn(vk_device, "vkCreateSemaphore"));
  if (!vkCreateSemaphoreFn)
    return false;

  vkCreateShaderModuleFn = reinterpret_cast<PFN_vkCreateShaderModule>(
      vkGetDeviceProcAddrFn(vk_device, "vkCreateShaderModule"));
  if (!vkCreateShaderModuleFn)
    return false;

  vkDestroyCommandPoolFn = reinterpret_cast<PFN_vkDestroyCommandPool>(
      vkGetDeviceProcAddrFn(vk_device, "vkDestroyCommandPool"));
  if (!vkDestroyCommandPoolFn)
    return false;

  vkDestroyDescriptorPoolFn = reinterpret_cast<PFN_vkDestroyDescriptorPool>(
      vkGetDeviceProcAddrFn(vk_device, "vkDestroyDescriptorPool"));
  if (!vkDestroyDescriptorPoolFn)
    return false;

  vkDestroyDescriptorSetLayoutFn =
      reinterpret_cast<PFN_vkDestroyDescriptorSetLayout>(
          vkGetDeviceProcAddrFn(vk_device, "vkDestroyDescriptorSetLayout"));
  if (!vkDestroyDescriptorSetLayoutFn)
    return false;

  vkDestroyDeviceFn = reinterpret_cast<PFN_vkDestroyDevice>(
      vkGetDeviceProcAddrFn(vk_device, "vkDestroyDevice"));
  if (!vkDestroyDeviceFn)
    return false;

  vkDestroyFramebufferFn = reinterpret_cast<PFN_vkDestroyFramebuffer>(
      vkGetDeviceProcAddrFn(vk_device, "vkDestroyFramebuffer"));
  if (!vkDestroyFramebufferFn)
    return false;

  vkDestroyFenceFn = reinterpret_cast<PFN_vkDestroyFence>(
      vkGetDeviceProcAddrFn(vk_device, "vkDestroyFence"));
  if (!vkDestroyFenceFn)
    return false;

  vkDestroyImageFn = reinterpret_cast<PFN_vkDestroyImage>(
      vkGetDeviceProcAddrFn(vk_device, "vkDestroyImage"));
  if (!vkDestroyImageFn)
    return false;

  vkDestroyImageViewFn = reinterpret_cast<PFN_vkDestroyImageView>(
      vkGetDeviceProcAddrFn(vk_device, "vkDestroyImageView"));
  if (!vkDestroyImageViewFn)
    return false;

  vkDestroyRenderPassFn = reinterpret_cast<PFN_vkDestroyRenderPass>(
      vkGetDeviceProcAddrFn(vk_device, "vkDestroyRenderPass"));
  if (!vkDestroyRenderPassFn)
    return false;

  vkDestroySamplerFn = reinterpret_cast<PFN_vkDestroySampler>(
      vkGetDeviceProcAddrFn(vk_device, "vkDestroySampler"));
  if (!vkDestroySamplerFn)
    return false;

  vkDestroySemaphoreFn = reinterpret_cast<PFN_vkDestroySemaphore>(
      vkGetDeviceProcAddrFn(vk_device, "vkDestroySemaphore"));
  if (!vkDestroySemaphoreFn)
    return false;

  vkDestroyShaderModuleFn = reinterpret_cast<PFN_vkDestroyShaderModule>(
      vkGetDeviceProcAddrFn(vk_device, "vkDestroyShaderModule"));
  if (!vkDestroyShaderModuleFn)
    return false;

  vkDeviceWaitIdleFn = reinterpret_cast<PFN_vkDeviceWaitIdle>(
      vkGetDeviceProcAddrFn(vk_device, "vkDeviceWaitIdle"));
  if (!vkDeviceWaitIdleFn)
    return false;

  vkFreeCommandBuffersFn = reinterpret_cast<PFN_vkFreeCommandBuffers>(
      vkGetDeviceProcAddrFn(vk_device, "vkFreeCommandBuffers"));
  if (!vkFreeCommandBuffersFn)
    return false;

  vkFreeDescriptorSetsFn = reinterpret_cast<PFN_vkFreeDescriptorSets>(
      vkGetDeviceProcAddrFn(vk_device, "vkFreeDescriptorSets"));
  if (!vkFreeDescriptorSetsFn)
    return false;

  vkFreeMemoryFn = reinterpret_cast<PFN_vkFreeMemory>(
      vkGetDeviceProcAddrFn(vk_device, "vkFreeMemory"));
  if (!vkFreeMemoryFn)
    return false;

  vkGetDeviceQueueFn = reinterpret_cast<PFN_vkGetDeviceQueue>(
      vkGetDeviceProcAddrFn(vk_device, "vkGetDeviceQueue"));
  if (!vkGetDeviceQueueFn)
    return false;

  vkGetFenceStatusFn = reinterpret_cast<PFN_vkGetFenceStatus>(
      vkGetDeviceProcAddrFn(vk_device, "vkGetFenceStatus"));
  if (!vkGetFenceStatusFn)
    return false;

  vkGetImageMemoryRequirementsFn =
      reinterpret_cast<PFN_vkGetImageMemoryRequirements>(
          vkGetDeviceProcAddrFn(vk_device, "vkGetImageMemoryRequirements"));
  if (!vkGetImageMemoryRequirementsFn)
    return false;

  vkResetFencesFn = reinterpret_cast<PFN_vkResetFences>(
      vkGetDeviceProcAddrFn(vk_device, "vkResetFences"));
  if (!vkResetFencesFn)
    return false;

  vkUpdateDescriptorSetsFn = reinterpret_cast<PFN_vkUpdateDescriptorSets>(
      vkGetDeviceProcAddrFn(vk_device, "vkUpdateDescriptorSets"));
  if (!vkUpdateDescriptorSetsFn)
    return false;

  vkWaitForFencesFn = reinterpret_cast<PFN_vkWaitForFences>(
      vkGetDeviceProcAddrFn(vk_device, "vkWaitForFences"));
  if (!vkWaitForFencesFn)
    return false;

#if defined(OS_ANDROID)

  vkGetAndroidHardwareBufferPropertiesANDROIDFn =
      reinterpret_cast<PFN_vkGetAndroidHardwareBufferPropertiesANDROID>(
          vkGetDeviceProcAddrFn(vk_device,
                                "vkGetAndroidHardwareBufferPropertiesANDROID"));
  if (!vkGetAndroidHardwareBufferPropertiesANDROIDFn)
    return false;

#endif

#if defined(OS_LINUX) || defined(OS_ANDROID)

  vkGetSemaphoreFdKHRFn = reinterpret_cast<PFN_vkGetSemaphoreFdKHR>(
      vkGetDeviceProcAddrFn(vk_device, "vkGetSemaphoreFdKHR"));
  if (!vkGetSemaphoreFdKHRFn)
    return false;

  vkImportSemaphoreFdKHRFn = reinterpret_cast<PFN_vkImportSemaphoreFdKHR>(
      vkGetDeviceProcAddrFn(vk_device, "vkImportSemaphoreFdKHR"));
  if (!vkImportSemaphoreFdKHRFn)
    return false;

#endif

#if defined(OS_LINUX)

  vkGetMemoryFdKHRFn = reinterpret_cast<PFN_vkGetMemoryFdKHR>(
      vkGetDeviceProcAddrFn(vk_device, "vkGetMemoryFdKHR"));
  if (!vkGetMemoryFdKHRFn)
    return false;

#endif

#if defined(OS_FUCHSIA)

  vkImportSemaphoreZirconHandleFUCHSIAFn =
      reinterpret_cast<PFN_vkImportSemaphoreZirconHandleFUCHSIA>(
          vkGetDeviceProcAddrFn(vk_device,
                                "vkImportSemaphoreZirconHandleFUCHSIA"));
  if (!vkImportSemaphoreZirconHandleFUCHSIAFn)
    return false;

  vkGetSemaphoreZirconHandleFUCHSIAFn =
      reinterpret_cast<PFN_vkGetSemaphoreZirconHandleFUCHSIA>(
          vkGetDeviceProcAddrFn(vk_device,
                                "vkGetSemaphoreZirconHandleFUCHSIA"));
  if (!vkGetSemaphoreZirconHandleFUCHSIAFn)
    return false;

  vkCreateBufferCollectionFUCHSIAFn =
      reinterpret_cast<PFN_vkCreateBufferCollectionFUCHSIA>(
          vkGetDeviceProcAddrFn(vk_device, "vkCreateBufferCollectionFUCHSIA"));
  if (!vkCreateBufferCollectionFUCHSIAFn)
    return false;

  vkSetBufferCollectionConstraintsFUCHSIAFn =
      reinterpret_cast<PFN_vkSetBufferCollectionConstraintsFUCHSIA>(
          vkGetDeviceProcAddrFn(vk_device,
                                "vkSetBufferCollectionConstraintsFUCHSIA"));
  if (!vkSetBufferCollectionConstraintsFUCHSIAFn)
    return false;

  vkGetBufferCollectionPropertiesFUCHSIAFn =
      reinterpret_cast<PFN_vkGetBufferCollectionPropertiesFUCHSIA>(
          vkGetDeviceProcAddrFn(vk_device,
                                "vkGetBufferCollectionPropertiesFUCHSIA"));
  if (!vkGetBufferCollectionPropertiesFUCHSIAFn)
    return false;

  vkDestroyBufferCollectionFUCHSIAFn =
      reinterpret_cast<PFN_vkDestroyBufferCollectionFUCHSIA>(
          vkGetDeviceProcAddrFn(vk_device, "vkDestroyBufferCollectionFUCHSIA"));
  if (!vkDestroyBufferCollectionFUCHSIAFn)
    return false;

#endif

  // Queue functions
  vkQueueSubmitFn = reinterpret_cast<PFN_vkQueueSubmit>(
      vkGetDeviceProcAddrFn(vk_device, "vkQueueSubmit"));
  if (!vkQueueSubmitFn)
    return false;

  vkQueueWaitIdleFn = reinterpret_cast<PFN_vkQueueWaitIdle>(
      vkGetDeviceProcAddrFn(vk_device, "vkQueueWaitIdle"));
  if (!vkQueueWaitIdleFn)
    return false;

  // Command Buffer functions
  vkBeginCommandBufferFn = reinterpret_cast<PFN_vkBeginCommandBuffer>(
      vkGetDeviceProcAddrFn(vk_device, "vkBeginCommandBuffer"));
  if (!vkBeginCommandBufferFn)
    return false;

  vkCmdBeginRenderPassFn = reinterpret_cast<PFN_vkCmdBeginRenderPass>(
      vkGetDeviceProcAddrFn(vk_device, "vkCmdBeginRenderPass"));
  if (!vkCmdBeginRenderPassFn)
    return false;

  vkCmdEndRenderPassFn = reinterpret_cast<PFN_vkCmdEndRenderPass>(
      vkGetDeviceProcAddrFn(vk_device, "vkCmdEndRenderPass"));
  if (!vkCmdEndRenderPassFn)
    return false;

  vkCmdExecuteCommandsFn = reinterpret_cast<PFN_vkCmdExecuteCommands>(
      vkGetDeviceProcAddrFn(vk_device, "vkCmdExecuteCommands"));
  if (!vkCmdExecuteCommandsFn)
    return false;

  vkCmdNextSubpassFn = reinterpret_cast<PFN_vkCmdNextSubpass>(
      vkGetDeviceProcAddrFn(vk_device, "vkCmdNextSubpass"));
  if (!vkCmdNextSubpassFn)
    return false;

  vkCmdPipelineBarrierFn = reinterpret_cast<PFN_vkCmdPipelineBarrier>(
      vkGetDeviceProcAddrFn(vk_device, "vkCmdPipelineBarrier"));
  if (!vkCmdPipelineBarrierFn)
    return false;

  vkEndCommandBufferFn = reinterpret_cast<PFN_vkEndCommandBuffer>(
      vkGetDeviceProcAddrFn(vk_device, "vkEndCommandBuffer"));
  if (!vkEndCommandBufferFn)
    return false;

  vkResetCommandBufferFn = reinterpret_cast<PFN_vkResetCommandBuffer>(
      vkGetDeviceProcAddrFn(vk_device, "vkResetCommandBuffer"));
  if (!vkResetCommandBufferFn)
    return false;

  return true;
}

bool VulkanFunctionPointers::BindSwapchainFunctionPointers(VkDevice vk_device) {
  vkAcquireNextImageKHRFn = reinterpret_cast<PFN_vkAcquireNextImageKHR>(
      vkGetDeviceProcAddrFn(vk_device, "vkAcquireNextImageKHR"));
  if (!vkAcquireNextImageKHRFn)
    return false;

  vkCreateSwapchainKHRFn = reinterpret_cast<PFN_vkCreateSwapchainKHR>(
      vkGetDeviceProcAddrFn(vk_device, "vkCreateSwapchainKHR"));
  if (!vkCreateSwapchainKHRFn)
    return false;

  vkDestroySwapchainKHRFn = reinterpret_cast<PFN_vkDestroySwapchainKHR>(
      vkGetDeviceProcAddrFn(vk_device, "vkDestroySwapchainKHR"));
  if (!vkDestroySwapchainKHRFn)
    return false;

  vkGetSwapchainImagesKHRFn = reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(
      vkGetDeviceProcAddrFn(vk_device, "vkGetSwapchainImagesKHR"));
  if (!vkGetSwapchainImagesKHRFn)
    return false;

  vkQueuePresentKHRFn = reinterpret_cast<PFN_vkQueuePresentKHR>(
      vkGetDeviceProcAddrFn(vk_device, "vkQueuePresentKHR"));
  if (!vkQueuePresentKHRFn)
    return false;

  return true;
}

}  // namespace gpu
