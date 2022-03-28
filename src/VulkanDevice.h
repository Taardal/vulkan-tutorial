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
        VkDevice device = VK_NULL_HANDLE;
        VkQueue graphicsQueue = VK_NULL_HANDLE;
        VkQueue presentQueue = VK_NULL_HANDLE;

    public:
        VulkanDevice(Vulkan* vulkan, VulkanPhysicalDevice* vulkanPhysicalDevice);

        VkDevice getDevice() const;

        bool initialize();

        void terminate() const;

    private:
        std::vector<VkDeviceQueueCreateInfo> getDeviceQueueCreateInfos(const QueueFamilyIndices& queueFamilyIndices) const;

        bool createDevice(const std::vector<VkDeviceQueueCreateInfo>& deviceQueueCreateInfos);

        bool findDeviceQueues(const QueueFamilyIndices& queueFamilyIndices);

        VkQueue findDeviceQueue(uint32_t queueFamilyIndex) const;
    };

}