#pragma once

#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanCommandPool.h"

#include <vulkan/vulkan.h>

namespace Vulkandemo {

    class VulkanBuffer {
    public:
        struct Config {
            VkDeviceSize Size = 0;
            VkBufferUsageFlags Usage = 0;
            VkMemoryPropertyFlags MemoryProperties = 0;
        };

    private:
        VulkanPhysicalDevice* vulkanPhysicalDevice;
        VulkanDevice* vulkanDevice;
        Config config;
        VkBuffer vkBuffer = VK_NULL_HANDLE;
        VkDeviceMemory vkDeviceMemory = VK_NULL_HANDLE;

    public:
        VulkanBuffer(VulkanPhysicalDevice* vulkanPhysicalDevice, VulkanDevice* vulkanDevice);

        const Config& getConfig() const;

        const VkBuffer getVkBuffer() const;

        const VkDeviceMemory getVkDeviceMemory() const;

        bool initialize(const Config& config);

        void terminate();

        void setData(void* data) const;

        static void copy(const VulkanBuffer& sourceBuffer, const VulkanBuffer& destinationBuffer, const VulkanCommandPool& commandPool, const VulkanDevice& vulkanDevice);

    private:
        uint32_t findMemoryType(uint32_t memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags) const;
    };
}
