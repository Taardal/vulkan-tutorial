#include "VulkanRenderPass.h"
#include "Log.h"

namespace Vulkandemo {

    const VkAllocationCallbacks* VulkanRenderPass::ALLOCATOR = VK_NULL_HANDLE;

    VulkanRenderPass::VulkanRenderPass(VulkanSwapChain* vulkanSwapChain, VulkanDevice* vulkanDevice) : vulkanSwapChain(vulkanSwapChain), vulkanDevice(vulkanDevice) {
    }

    const VkRenderPass VulkanRenderPass::getRenderPass() const {
        return renderPass;
    }

    bool VulkanRenderPass::initialize() {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = vulkanSwapChain->getSurfaceFormat().format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        if (vkCreateRenderPass(vulkanDevice->getDevice(), &renderPassInfo, ALLOCATOR, &renderPass) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not create Vulkan render pass");
            return false;
        }
        VD_LOG_INFO("Created Vulkan render pass");
        return true;
    }

    void VulkanRenderPass::terminate() {
        vkDestroyRenderPass(vulkanDevice->getDevice(), renderPass, ALLOCATOR);
        VD_LOG_INFO("Destroyed Vulkan render pass");
    }

}
