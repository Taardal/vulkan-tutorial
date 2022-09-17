#pragma once

#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"

#include <vulkan/vulkan.h>

namespace Vulkandemo {

    class VulkanFramebuffer {
    private:
        static const VkAllocationCallbacks* ALLOCATOR;

    private:
        VulkanDevice* vulkanDevice;
        VulkanSwapChain* vulkanSwapChain;
        VulkanRenderPass* vulkanRenderPass;
        VkFramebuffer framebuffer = VK_NULL_HANDLE;

    public:
        VulkanFramebuffer(VulkanDevice* vulkanDevice, VulkanSwapChain* vulkanSwapChain, VulkanRenderPass* vulkanRenderPass);

        const VkFramebuffer getFramebuffer() const;

        bool initialize(VkImageView colorImageView, VkImageView depthImageView);

        void terminate();
    };

}
