#pragma once

#include "VulkanDevice.h"

#include <vulkan/vulkan.h>

#include <vector>

namespace Vulkandemo {

    class VulkanShader {
    private:
        static const VkAllocationCallbacks* ALLOCATOR;

    private:
        VulkanDevice* vulkanDevice;
        VkShaderModule shaderModule = VK_NULL_HANDLE;

    public:
        explicit VulkanShader(VulkanDevice* vulkanDevice);

        const VkShaderModule getShaderModule() const;

        bool initialize(const std::vector<char>& bytes);

        void terminate();
    };

}


