#pragma once

#include "VulkanShader.h"
#include "VulkanSwapChain.h"
#include "VulkanDevice.h"

#include <vulkan/vulkan.h>

namespace Vulkandemo {

    class VulkanRenderPass {
    private:
        static const VkAllocationCallbacks* ALLOCATOR;

    private:
        VulkanSwapChain* vulkanSwapChain;
        VulkanDevice* vulkanDevice;
        VkRenderPass renderPass = VK_NULL_HANDLE;

    public:
        VulkanRenderPass(VulkanSwapChain* vulkanSwapChain, VulkanDevice* vulkanDevice);

        const VkRenderPass getRenderPass() const;

        bool initialize();

        void terminate();
    };

}



