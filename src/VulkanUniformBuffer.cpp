#include "VulkanUniformBuffer.h"
#include "Log.h"

namespace Vulkandemo {

    VulkanUniformBuffer::VulkanUniformBuffer(VulkanPhysicalDevice* vulkanPhysicalDevice, VulkanDevice* vulkanDevice)
            : vulkanPhysicalDevice(vulkanPhysicalDevice), vulkanDevice(vulkanDevice), buffer(vulkanPhysicalDevice, vulkanDevice) {}

    const VulkanBuffer& VulkanUniformBuffer::getBuffer() const {
        return buffer;
    }

    bool VulkanUniformBuffer::initialize() {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        VulkanBuffer::Config bufferConfig{};
        bufferConfig.Size = bufferSize;
        bufferConfig.Usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferConfig.MemoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        if (!buffer.initialize(bufferConfig)) {
            VD_LOG_ERROR("Could not initialize uniform buffer");
            return false;
        }

        VD_LOG_INFO("Initialized uniform buffer");
        return true;
    }

    void VulkanUniformBuffer::terminate() {
        buffer.terminate();
    }

    void VulkanUniformBuffer::setData(const UniformBufferObject& data) const {
        buffer.setData((void*) &data);
    }

}
