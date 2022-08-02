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
        memoryAllocateInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, config.MemoryProperties);

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

    uint32_t VulkanBuffer::findMemoryType(uint32_t memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags) const {
        VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
        vkGetPhysicalDeviceMemoryProperties(vulkanPhysicalDevice->getPhysicalDevice(), &physicalDeviceMemoryProperties);
        for (uint32_t memoryTypeIndex = 0; memoryTypeIndex < physicalDeviceMemoryProperties.memoryTypeCount; memoryTypeIndex++) {
            /*
             * The memoryTypeBits parameter will be used to specify the bit field of memory types that are suitable.
             * That means that we can find the index of a suitable memory type by simply iterating over them and checking if the corresponding bit is set to 1.
             */
            bool isSuitableType = (memoryTypeBits & (1 << memoryTypeIndex)) > 0;

            /*
             * However, we're not just interested in a memory type that is suitable for the buffer. We also need to ensure that it has the necessary properties
             * The memoryTypes array consists of VkMemoryType structs that specify the heap and properties of each type of memory.
             */
            VkMemoryType& memoryType = physicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex];
            bool hasNecessaryProperties = (memoryType.propertyFlags & memoryPropertyFlags) == memoryPropertyFlags;

            if (isSuitableType && hasNecessaryProperties) {
                return memoryTypeIndex;
            }
        }
        VD_LOG_ERROR("Could not find memory type [{}]", memoryTypeBits);
        return -1;
    }

    void VulkanBuffer::copy(const VulkanBuffer& sourceBuffer, const VulkanBuffer& destinationBuffer, const VulkanCommandPool& commandPool, const VulkanDevice& vulkanDevice) {
        VD_ASSERT(sourceBuffer.config.Size == destinationBuffer.config.Size);
        
        constexpr uint32_t commandBufferCount = 1;
        const std::vector<VulkanCommandBuffer>& commandBuffers = commandPool.allocateCommandBuffers(commandBufferCount);
        VD_ASSERT(commandBuffers.size() == commandBufferCount);

        const VulkanCommandBuffer& commandBuffer = commandBuffers[0];
        VkCommandBuffer vkCommandBuffer = commandBuffer.getCommandBuffer();
        
        VkCommandBufferBeginInfo commandBufferBeginInfo{};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(vkCommandBuffer, &commandBufferBeginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.size = sourceBuffer.config.Size;
        constexpr uint32_t regionCount = 1;
        vkCmdCopyBuffer(vkCommandBuffer, sourceBuffer.vkBuffer, destinationBuffer.vkBuffer, regionCount, &copyRegion);

        vkEndCommandBuffer(vkCommandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &vkCommandBuffer;

        constexpr uint32_t submitCount = 1;
        VkFence fence = VK_NULL_HANDLE;
        vkQueueSubmit(vulkanDevice.getGraphicsQueue(), submitCount, &submitInfo, fence);
        vkQueueWaitIdle(vulkanDevice.getGraphicsQueue());

        vkFreeCommandBuffers(vulkanDevice.getDevice(), commandPool.getCommandPool(), commandBufferCount, &vkCommandBuffer);
    }

}
