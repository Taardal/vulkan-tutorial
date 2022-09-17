#include "VulkanBuffer.h"
#include "Log.h"
#include "Assert.h"

namespace Vulkandemo {

    Vulkandemo::VulkanBuffer::VulkanBuffer(Vulkandemo::VulkanPhysicalDevice* vulkanPhysicalDevice, Vulkandemo::VulkanDevice* vulkanDevice)
            : vulkanPhysicalDevice(vulkanPhysicalDevice), vulkanDevice(vulkanDevice) {}

    const VulkanBuffer::Config& VulkanBuffer::getConfig() const {
        return config;
    }

    const VkBuffer VulkanBuffer::getVkBuffer() const {
        return vkBuffer;
    }

    const VkDeviceMemory VulkanBuffer::getVkDeviceMemory() const {
        return vkDeviceMemory;
    }

    bool VulkanBuffer::initialize(const Config& config) {
        this->config = config;

        VkAllocationCallbacks* allocator = VK_NULL_HANDLE;

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = config.Size;
        bufferInfo.usage = config.Usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(vulkanDevice->getDevice(), &bufferInfo, allocator, &vkBuffer) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not create Vulkan buffer");
            return false;
        }

        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(vulkanDevice->getDevice(), vkBuffer, &memoryRequirements);

        VkMemoryAllocateInfo memoryAllocateInfo{};
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = vulkanPhysicalDevice->findMemoryType(memoryRequirements.memoryTypeBits, config.MemoryProperties);

        if (vkAllocateMemory(vulkanDevice->getDevice(), &memoryAllocateInfo, allocator, &vkDeviceMemory) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not allocate Vulkan vertex vkBuffer memory");
            return false;
        }

        constexpr VkDeviceSize memoryOffset = 0;
        vkBindBufferMemory(vulkanDevice->getDevice(), vkBuffer, vkDeviceMemory, memoryOffset);

        VD_LOG_INFO("Initialized Vulkan buffer");
        return true;
    }

    void VulkanBuffer::terminate() {
        VkAllocationCallbacks* allocator = VK_NULL_HANDLE;
        vkDestroyBuffer(vulkanDevice->getDevice(), vkBuffer, allocator);
        VD_LOG_INFO("Destroyed Vulkan buffer");
        vkFreeMemory(vulkanDevice->getDevice(), vkDeviceMemory, allocator);
        VD_LOG_INFO("Freed Vulkan buffer memory");
        VD_LOG_INFO("Terminated Vulkan buffer");
    }

    void VulkanBuffer::setData(void* data) const {
        void* memory;
        constexpr VkDeviceSize memoryOffset = 0;
        constexpr VkMemoryMapFlags memoryMapFlags = 0;
        vkMapMemory(vulkanDevice->getDevice(), vkDeviceMemory, memoryOffset, config.Size, memoryMapFlags, &memory);
        memcpy(memory, data, config.Size);
        vkUnmapMemory(vulkanDevice->getDevice(), vkDeviceMemory);
    }

    void VulkanBuffer::copy(const VulkanBuffer& sourceBuffer, const VulkanBuffer& destinationBuffer, const VulkanCommandPool& commandPool, const VulkanDevice& vulkanDevice) {
        VD_ASSERT(sourceBuffer.config.Size == destinationBuffer.config.Size);

        constexpr uint32_t commandBufferCount = 1;
        const std::vector<VulkanCommandBuffer>& commandBuffers = commandPool.allocateCommandBuffers(commandBufferCount);
        VD_ASSERT(commandBuffers.size() == commandBufferCount)

        const VulkanCommandBuffer& commandBuffer = commandBuffers[0];
        commandBuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        VkCommandBuffer vkCommandBuffer = commandBuffer.getVkCommandBuffer();

        VkBufferCopy copyRegion{};
        copyRegion.size = sourceBuffer.config.Size;
        constexpr uint32_t regionCount = 1;
        vkCmdCopyBuffer(vkCommandBuffer, sourceBuffer.vkBuffer, destinationBuffer.vkBuffer, regionCount, &copyRegion);

        commandBuffer.end();

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &vkCommandBuffer;

        constexpr uint32_t submitCount = 1;
        VkFence fence = VK_NULL_HANDLE;
        vkQueueSubmit(vulkanDevice.getGraphicsQueue(), submitCount, &submitInfo, fence);
        vkQueueWaitIdle(vulkanDevice.getGraphicsQueue());

        commandPool.free(commandBuffer);
    }

}
