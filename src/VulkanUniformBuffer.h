#pragma once

#include "VulkanBuffer.h"

#include <glm/glm.hpp>

namespace Vulkandemo {

    class VulkanUniformBuffer {
    private:
        VulkanBuffer buffer;

    public:
        VulkanUniformBuffer(VulkanPhysicalDevice* vulkanPhysicalDevice, VulkanDevice* vulkanDevice);

        const VulkanBuffer& getBuffer() const;

        bool initialize(uint32_t bufferSize);

        void terminate();

        void setData(void* data) const;
    };

}
