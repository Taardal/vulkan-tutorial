#pragma once

#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanCommandPool.h"

#include <vulkan/vulkan.h>

namespace Vulkandemo {

    class VulkanIndexBuffer {
    private:
        VulkanDevice* vulkanDevice;
        VulkanPhysicalDevice* vulkanPhysicalDevice;
        VulkanCommandPool* vulkanCommandPool;
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory deviceMemory = VK_NULL_HANDLE;

    public:
        VulkanIndexBuffer(VulkanPhysicalDevice* vulkanPhysicalDevice, VulkanDevice* vulkanDevice, VulkanCommandPool* vulkanCommandPool);

        const VkBuffer getVkBuffer() const;

        bool initialize(const std::vector<uint16_t>& indices);

        void terminate();

        bool createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties, VkBuffer& buffer, VkDeviceMemory& bufferDeviceMemory) const;

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;

        uint32_t findMemoryType(uint32_t suitableMemoryTypeBits, VkMemoryPropertyFlags propertyFlags) const;
    };
}
