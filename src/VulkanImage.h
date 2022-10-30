#pragma once

#include "VulkanDevice.h"

#include <vulkan/vulkan.h>

namespace Vulkandemo {

    class VulkanImage {
    public:
        struct Config {
            uint32_t Width;
            uint32_t Height;
            uint32_t MipLevels;
            VkFormat Format;
            VkImageTiling Tiling;
            VkImageUsageFlags Usage;
            VkMemoryPropertyFlags MemoryProperties;
            VkImageLayout Layout;
            VkSampleCountFlagBits SampleCount;
        };

    private:
        VulkanPhysicalDevice* vulkanPhysicalDevice;
        VulkanDevice* vulkanDevice;
        VkImage vkImage = VK_NULL_HANDLE;
        VkDeviceMemory vkDeviceMemory = VK_NULL_HANDLE;

    public:
        VulkanImage(VulkanPhysicalDevice* vulkanPhysicalDevice, VulkanDevice* vulkanDevice);

        VkImage getVkImage() const;

        bool initialize(const Config& config);

        void terminate();
    };

}
