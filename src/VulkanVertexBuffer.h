#pragma once

#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanCommandPool.h"
#include "VulkanBuffer.h"
#include "Vertex.h"

#include <vulkan/vulkan.h>

namespace Vulkandemo {

    class VulkanVertexBuffer {
    private:
        VulkanDevice* vulkanDevice;
        VulkanPhysicalDevice* vulkanPhysicalDevice;
        VulkanCommandPool* vulkanCommandPool;
        VulkanBuffer buffer;

    public:
        VulkanVertexBuffer(VulkanPhysicalDevice* vulkanPhysicalDevice, VulkanDevice* vulkanDevice, VulkanCommandPool* vulkanCommandPool);

        const VulkanBuffer& getVulkanBuffer() const;

        bool initialize(const std::vector<Vertex>& vertices);

        void terminate();

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
    };
}
