#pragma once

#include "VulkanCommandPool.h"

#include <vulkan/vulkan.h>

namespace Vulkandemo {

    class VulkanCommandBuffer {
    private:
        static const VkAllocationCallbacks* ALLOCATOR;

    private:
        VulkanCommandPool* vulkanCommandPool;
        VulkanDevice* vulkanDevice;
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

    public:
        VulkanCommandBuffer(VulkanCommandPool* vulkanCommandPool, VulkanDevice* vulkanDevice);

        const VkCommandBuffer getCommandBuffer() const;

        bool initialize();

        bool begin() const;

        bool end() const;

        void reset() const;
    };

}
