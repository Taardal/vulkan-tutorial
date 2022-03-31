#include "VulkanCommandBuffer.h"
#include "Log.h"

namespace Vulkandemo {

    const VkAllocationCallbacks* VulkanCommandBuffer::ALLOCATOR = VK_NULL_HANDLE;

    VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandPool* vulkanCommandPool, VulkanDevice* vulkanDevice) : vulkanCommandPool(vulkanCommandPool), vulkanDevice(vulkanDevice) {
    }

    const VkCommandBuffer VulkanCommandBuffer::getCommandBuffer() const {
        return commandBuffer;
    }

    bool VulkanCommandBuffer::initialize() {
        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = 1;
        allocateInfo.commandPool = vulkanCommandPool->getCommandPool();

        if (vkAllocateCommandBuffers(vulkanDevice->getDevice(), &allocateInfo, &commandBuffer) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not allocate Vulkan command buffer");
            return false;
        }
        VD_LOG_INFO("Allocated Vulkan command buffer");
        return true;
    }

    bool VulkanCommandBuffer::begin() const {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not begin Vulkan command buffer");
            return false;
        }
        return true;
    }

    bool VulkanCommandBuffer::end() const {
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not end Vulkan command buffer");
            return false;
        }
        return true;
    }

    void VulkanCommandBuffer::reset() const {
        VkCommandBufferResetFlags flags = 0;
        vkResetCommandBuffer(commandBuffer, flags);
    }

}