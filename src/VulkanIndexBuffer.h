#pragma once

#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanCommandPool.h"
#include "VulkanBuffer.h"

#include <vulkan/vulkan.h>

namespace Vulkandemo {

    class VulkanIndexBuffer {
    private:
        VulkanPhysicalDevice* vulkanPhysicalDevice;
        VulkanDevice* vulkanDevice;
        VulkanCommandPool* vulkanCommandPool;
        VulkanBuffer buffer;

    public:
        VulkanIndexBuffer(VulkanPhysicalDevice* vulkanPhysicalDevice, VulkanDevice* vulkanDevice, VulkanCommandPool* vulkanCommandPool);

        const VulkanBuffer& getVulkanBuffer() const;

        bool initialize(const std::vector<uint16_t>& indices);

        void terminate();

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
    };
}