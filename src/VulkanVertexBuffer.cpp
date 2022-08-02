#include "VulkanVertexBuffer.h"
#include "Log.h"

namespace Vulkandemo {

    VulkanVertexBuffer::VulkanVertexBuffer(VulkanPhysicalDevice* vulkanPhysicalDevice, VulkanDevice* vulkanDevice, VulkanCommandPool* vulkanCommandPool)
            : vulkanPhysicalDevice(vulkanPhysicalDevice), vulkanDevice(vulkanDevice), vulkanCommandPool(vulkanCommandPool) {}

    const VkBuffer VulkanVertexBuffer::getVkBuffer() const {
        return vertexBuffer;
    }

    bool VulkanVertexBuffer::initialize(const std::vector<Vertex>& vertices) {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferDeviceMemory;
        constexpr VkBufferUsageFlags stagingBufferUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        constexpr VkMemoryPropertyFlags stagingBufferMemoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        if (!createBuffer(bufferSize, stagingBufferUsage, stagingBufferMemoryProperties, stagingBuffer, stagingBufferDeviceMemory)) {
            VD_LOG_ERROR("Could not create staging vertex buffer");
            return false;
        }

        void* memory;
        constexpr VkDeviceSize stagingBufferMemoryOffset = 0;
        constexpr VkMemoryMapFlags stagingBufferMemoryMapFlags = 0;
        vkMapMemory(vulkanDevice->getDevice(), stagingBufferDeviceMemory, stagingBufferMemoryOffset, bufferSize, stagingBufferMemoryMapFlags, &memory);
        memcpy(memory, vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(vulkanDevice->getDevice(), stagingBufferDeviceMemory);

        constexpr VkBufferUsageFlags bufferUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        constexpr VkMemoryPropertyFlags bufferMemoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        if (!createBuffer(bufferSize, bufferUsage, bufferMemoryProperties, vertexBuffer, vertexBufferDeviceMemory)) {
            VD_LOG_ERROR("Could not create vertex buffer");
            return false;
        }

        copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        VkAllocationCallbacks* allocator = VK_NULL_HANDLE;
        vkDestroyBuffer(vulkanDevice->getDevice(), stagingBuffer, allocator);
        vkFreeMemory(vulkanDevice->getDevice(), stagingBufferDeviceMemory, allocator);

        return true;
    }

    bool VulkanVertexBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties, VkBuffer& buffer, VkDeviceMemory& bufferDeviceMemory) const {
        VkAllocationCallbacks* allocator = VK_NULL_HANDLE;

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(vulkanDevice->getDevice(), &bufferInfo, allocator, &buffer) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not create Vulkan vertex buffer");
            return false;
        }

        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(vulkanDevice->getDevice(), buffer, &memoryRequirements);

        VkMemoryAllocateInfo memoryAllocateInfo{};
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, memoryProperties);

        if (vkAllocateMemory(vulkanDevice->getDevice(), &memoryAllocateInfo, allocator, &bufferDeviceMemory) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not allocate Vulkan vertex buffer memory");
            return false;
        }

        constexpr VkDeviceSize memoryOffset = 0;
        vkBindBufferMemory(vulkanDevice->getDevice(), buffer, bufferDeviceMemory, memoryOffset);

        return true;
    }

    void VulkanVertexBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const {
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

    uint32_t VulkanVertexBuffer::findMemoryType(uint32_t suitableMemoryTypeBits, VkMemoryPropertyFlags propertyFlags) const {
        VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
        vkGetPhysicalDeviceMemoryProperties(vulkanPhysicalDevice->getPhysicalDevice(), &physicalDeviceMemoryProperties);
        for (uint32_t memoryTypeIndex = 0; memoryTypeIndex < physicalDeviceMemoryProperties.memoryTypeCount; memoryTypeIndex++) {
            /*
             * The suitableMemoryTypeBits parameter will be used to specify the bit field of memory types that are suitable.
             * That means that we can find the index of a suitable memory type by simply iterating over them and checking if the corresponding bit is set to 1.
             */
            bool isSuitableType = (suitableMemoryTypeBits & (1 << memoryTypeIndex)) > 0;

            /*
             * However, we're not just interested in a memory type that is suitable for the buffer. We also need to ensure that it has the necessary properties
             * The memoryTypes array consists of VkMemoryType structs that specify the heap and properties of each type of memory.
             */
            VkMemoryType& memoryType = physicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex];
            bool hasNecessaryProperties = (memoryType.propertyFlags & propertyFlags) == propertyFlags;

            if (isSuitableType && hasNecessaryProperties) {
                return memoryTypeIndex;
            }
        }
        VD_LOG_ERROR("Could not find memory type [{}]", suitableMemoryTypeBits);
        return -1;
    }

    void VulkanVertexBuffer::terminate() {
        VkAllocationCallbacks* allocator = VK_NULL_HANDLE;
        vkDestroyBuffer(vulkanDevice->getDevice(), vertexBuffer, allocator);
        vkFreeMemory(vulkanDevice->getDevice(), vertexBufferDeviceMemory, allocator);
    }
}