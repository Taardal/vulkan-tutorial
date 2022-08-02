#pragma once

#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanCommandPool.h"
#include "Vertex.h"

#include <vulkan/vulkan.h>

namespace Vulkandemo {

    class VulkanVertexBuffer {
    private:
        VulkanDevice* vulkanDevice;
        VulkanPhysicalDevice* vulkanPhysicalDevice;
        VulkanCommandPool* vulkanCommandPool;
        VkBuffer vertexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory vertexBufferDeviceMemory = VK_NULL_HANDLE;

    public:
        VulkanVertexBuffer(VulkanPhysicalDevice* vulkanPhysicalDevice, VulkanDevice* vulkanDevice, VulkanCommandPool* vulkanCommandPool);

        const VkBuffer getVkBuffer() const;

        bool initialize(const std::vector<Vertex>& vertices);

        void terminate();

        bool createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties, VkBuffer& buffer, VkDeviceMemory& bufferDeviceMemory) const;

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;

        uint32_t findMemoryType(uint32_t suitableMemoryTypeBits, VkMemoryPropertyFlags propertyFlags) const;
    };
}
