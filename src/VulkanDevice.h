#pragma once

#include "VulkanPhysicalDevice.h"

#include <vulkan/vulkan.h>

namespace Vulkandemo {

    class VulkanDevice {
    private:
        static const VkAllocationCallbacks* ALLOCATOR;

    private:
        Vulkan* vulkan;
        VulkanPhysicalDevice* vulkanPhysicalDevice;
        VkDevice vkDevice = VK_NULL_HANDLE;
        VkQueue graphicsVkQueue = VK_NULL_HANDLE;
        VkQueue presentationVkQueue = VK_NULL_HANDLE;

    public:
        VulkanDevice(Vulkan* vulkan, VulkanPhysicalDevice* vulkanPhysicalDevice);

        bool initialize() const;

        void terminate() const;
    };

}