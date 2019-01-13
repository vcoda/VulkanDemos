#pragma once

#define VK_USE_PLATFORM_MACOS_MVK                   1
#define VULKAN_HAS_PHYSICAL_DEVICE_PROPERTIES2      1
#define VULKAN_SUPPORTS_DEDICATED_ALLOCATION        0
#define VULKAN_SUPPORTS_AMD_BUFFER_MARKER           1
#define VULKAN_SUPPORTS_NV_DIAGNOSTIC_CHECKPOINT    1

#include <vector>
#include <vulkan/vulkan.h>

#include "Common/Common.h"
#include "Vulkan/VulkanGenericPlatform.h"

class VulkanMacPlatform : public VulkanGenericPlatform
{
public:
    static bool LoadVulkanLibrary();
    
    static bool LoadVulkanInstanceFunctions(VkInstance inInstance);
    
    static void FreeVulkanLibrary();
    
    static void GetInstanceExtensions(std::vector<const char*>& outExtensions);
    
    static void GetDeviceExtensions(std::vector<const char*>& outExtensions);
    
    static void CreateSurface(void* windowHandle, VkInstance instance, VkSurfaceKHR* outSurface);
    
    static bool SupportsDeviceLocalHostVisibleWithNoPenalty();
    
    static void WriteCrashMarker(const OptionalVulkanDeviceExtensions& optionalExtensions, VkCommandBuffer cmdBuffer, VkBuffer destBuffer, const std::vector<uint32>& entries, bool adding);
    
    static bool UseRealUBsOptimization(bool codeHeaderUseRealUBs)
    {
        return true;
    }
};

typedef VulkanMacPlatform VulkanPlatform;
