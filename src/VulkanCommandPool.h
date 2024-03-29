#pragma once

#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace Vulkandemo {

    class VulkanCommandPool {
    private:
        static const VkAllocationCallbacks* ALLOCATOR;

    private:
        VulkanPhysicalDevice* vulkanPhysicalDevice;
        VulkanDevice* vulkanDevice;
        VkCommandPool vkCommandPool = VK_NULL_HANDLE;

    public:
        VulkanCommandPool(VulkanPhysicalDevice* vulkanPhysicalDevice, VulkanDevice* vulkanDevice);

        const VkCommandPool getVkCommandPool() const;

        bool initialize();

        void terminate();

        std::vector<VulkanCommandBuffer> allocateCommandBuffers(uint32_t count) const;

        void free(const VulkanCommandBuffer& commandBuffer) const;
    };

}
