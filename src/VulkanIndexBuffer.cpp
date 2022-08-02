#include "VulkanIndexBuffer.h"
#include "Log.h"

namespace Vulkandemo {

    VulkanIndexBuffer::VulkanIndexBuffer(VulkanPhysicalDevice* vulkanPhysicalDevice, VulkanDevice* vulkanDevice, VulkanCommandPool* vulkanCommandPool)
            : vulkanPhysicalDevice(vulkanPhysicalDevice), vulkanDevice(vulkanDevice), vulkanCommandPool(vulkanCommandPool), buffer(vulkanPhysicalDevice, vulkanDevice) {}

    const VulkanBuffer& VulkanIndexBuffer::getVulkanBuffer() const {
        return buffer;
    }

    bool VulkanIndexBuffer::initialize(const std::vector<uint16_t>& indices) {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VulkanBuffer::Config stagingBufferConfig{};
        stagingBufferConfig.Size = bufferSize;
        stagingBufferConfig.Usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        stagingBufferConfig.MemoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        VulkanBuffer stagingBuffer(vulkanPhysicalDevice, vulkanDevice);
        if (!stagingBuffer.initialize(stagingBufferConfig)) {
            VD_LOG_ERROR("Could not initialize staging buffer for index buffer");
            return false;
        }

        void* memory;
        constexpr VkDeviceSize stagingBufferMemoryOffset = 0;
        constexpr VkMemoryMapFlags stagingBufferMemoryMapFlags = 0;
        vkMapMemory(vulkanDevice->getDevice(), stagingBuffer.getVkDeviceMemory(), stagingBufferMemoryOffset, bufferSize, stagingBufferMemoryMapFlags, &memory);
        memcpy(memory, indices.data(), (size_t) bufferSize);
        vkUnmapMemory(vulkanDevice->getDevice(), stagingBuffer.getVkDeviceMemory());

        VulkanBuffer::Config bufferConfig{};
        bufferConfig.Size = bufferSize;
        bufferConfig.Usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        bufferConfig.MemoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        if (!buffer.initialize(bufferConfig)) {
            VD_LOG_ERROR("Could not initialize index buffer");
            return false;
        }

        copyBuffer(stagingBuffer.getVkBuffer(), buffer.getVkBuffer(), bufferSize);

        stagingBuffer.terminate();
        VD_LOG_INFO("Terminated staging buffer for index buffer");

        VD_LOG_INFO("Initialized Vulkan index buffer");
        return true;
    }

    void VulkanIndexBuffer::terminate() {
        buffer.terminate();
        VD_LOG_INFO("Terminated Vulkan index buffer");
    }

    void VulkanIndexBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const {
        VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandPool = vulkanCommandPool->getCommandPool();
        commandBufferAllocateInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(vulkanDevice->getDevice(), &commandBufferAllocateInfo, &commandBuffer);

        VkCommandBufferBeginInfo commandBufferBeginInfo{};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        constexpr uint32_t regionCount = 1;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        constexpr uint32_t submitCount = 1;
        VkFence fence = VK_NULL_HANDLE;
        vkQueueSubmit(vulkanDevice->getGraphicsQueue(), submitCount, &submitInfo, fence);
        vkQueueWaitIdle(vulkanDevice->getGraphicsQueue());

        constexpr uint32_t commandBufferCount = 1;
        vkFreeCommandBuffers(vulkanDevice->getDevice(), vulkanCommandPool->getCommandPool(), commandBufferCount, &commandBuffer);
    }
}