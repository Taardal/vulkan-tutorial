#pragma once

#include "VulkanShader.h"
#include "VulkanRenderPass.h"
#include "VulkanSwapChain.h"
#include "VulkanDevice.h"

#include <vulkan/vulkan.h>

namespace Vulkandemo {

    class VulkanGraphicsPipeline {
    private:
        static const VkAllocationCallbacks* ALLOCATOR;

    private:
        VulkanRenderPass* vulkanRenderPass;
        VulkanSwapChain* vulkanSwapChain;
        VulkanDevice* vulkanDevice;
        VulkanPhysicalDevice* vulkanPhysicalDevice;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        VkPipeline pipeline = VK_NULL_HANDLE;

    public:
        VulkanGraphicsPipeline(VulkanRenderPass* vulkanRenderPass, VulkanSwapChain* vulkanSwapChain, VulkanDevice* vulkanDevice, VulkanPhysicalDevice* vulkanPhysicalDevice);

        const VkPipelineLayout getPipelineLayout() const;

        const VkPipeline getPipeline() const;

        bool initialize(const VulkanShader& vertexShader, const VulkanShader& fragmentShader, VkDescriptorSetLayout descriptorSetLayout);

        void terminate();

        void bind(const VulkanCommandBuffer& vulkanCommandBuffer) const;
    };

}


