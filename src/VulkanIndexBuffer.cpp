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

        VulkanBuffer::Config bufferConfig{};
        bufferConfig.Size = bufferSize;
        bufferConfig.Usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        bufferConfig.MemoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        if (!buffer.initialize(bufferConfig)) {
            VD_LOG_ERROR("Could not initialize index buffer");
            return false;
        }

        stagingBuffer.setData((void*) indices.data());
        VulkanBuffer::copy(stagingBuffer, buffer, *vulkanCommandPool, *vulkanDevice);
        VD_LOG_INFO("Copied indices to index buffer");

        stagingBuffer.terminate();
        VD_LOG_INFO("Terminated staging buffer for index buffer");

        VD_LOG_INFO("Initialized Vulkan index buffer");
        return true;
    }

    void VulkanIndexBuffer::terminate() {
        buffer.terminate();
        VD_LOG_INFO("Terminated Vulkan index buffer");
    }
}