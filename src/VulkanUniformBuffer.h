#pragma once

#include "VulkanBuffer.h"

#include <glm/glm.hpp>

namespace Vulkandemo {

    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    class VulkanUniformBuffer {
    private:
        VulkanPhysicalDevice* vulkanPhysicalDevice;
        VulkanDevice* vulkanDevice;
        VulkanBuffer buffer;

    public:
        VulkanUniformBuffer(VulkanPhysicalDevice* vulkanPhysicalDevice, VulkanDevice* vulkanDevice);

        const VulkanBuffer& getBuffer() const;

        bool initialize();

        void terminate();

        void setData(const UniformBufferObject& data) const;
    };

}
