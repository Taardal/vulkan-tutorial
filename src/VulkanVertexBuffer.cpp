#include "VulkanVertexBuffer.h"
#include "Log.h"

namespace Vulkandemo {

    VulkanVertexBuffer::VulkanVertexBuffer(VulkanPhysicalDevice* vulkanPhysicalDevice, VulkanDevice* vulkanDevice, VulkanCommandPool* vulkanCommandPool)
            : vulkanPhysicalDevice(vulkanPhysicalDevice), vulkanDevice(vulkanDevice), vulkanCommandPool(vulkanCommandPool), buffer(vulkanPhysicalDevice, vulkanDevice) {}

    const VulkanBuffer& VulkanVertexBuffer::getVulkanBuffer() const {
        return buffer;
    }

    bool VulkanVertexBuffer::initialize(const std::vector<Vertex>& vertices) {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        VulkanBuffer::Config stagingBufferConfig{};
        stagingBufferConfig.Size = bufferSize;
        stagingBufferConfig.Usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        stagingBufferConfig.MemoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        VulkanBuffer stagingBuffer(vulkanPhysicalDevice, vulkanDevice);
        if (!stagingBuffer.initialize(stagingBufferConfig)) {
            VD_LOG_ERROR("Could not initialize staging buffer for vertex buffer");
            return false;
        }

        VulkanBuffer::Config bufferConfig{};
        bufferConfig.Size = bufferSize;
        bufferConfig.Usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferConfig.MemoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        if (!buffer.initialize(bufferConfig)) {
            VD_LOG_ERROR("Could not initialize vertex buffer");
            return false;
        }

        stagingBuffer.setData((void*) vertices.data());
        VulkanBuffer::copy(stagingBuffer, buffer, *vulkanCommandPool, *vulkanDevice);
        VD_LOG_INFO("Copied vertices to vertex buffer");

        stagingBuffer.terminate();
        VD_LOG_INFO("Terminated staging buffer for vertex buffer");

        VD_LOG_INFO("Initialized Vulkan vertex buffer");
        return true;
    }

    void VulkanVertexBuffer::terminate() {
        buffer.terminate();
        VD_LOG_INFO("Terminated Vulkan vertex buffer");
    }
}