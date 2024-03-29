#include "VulkanFramebuffer.h"
#include "Log.h"

namespace Vulkandemo {

    const VkAllocationCallbacks* VulkanFramebuffer::ALLOCATOR = VK_NULL_HANDLE;

    VulkanFramebuffer::VulkanFramebuffer(VulkanDevice* vulkanDevice, VulkanSwapChain* vulkanSwapChain, VulkanRenderPass* vulkanRenderPass)
            : vulkanDevice(vulkanDevice), vulkanSwapChain(vulkanSwapChain), vulkanRenderPass(vulkanRenderPass) {
    }

    const VkFramebuffer VulkanFramebuffer::getFramebuffer() const {
        return framebuffer;
    }

    bool VulkanFramebuffer::initialize(VkImageView colorImageView, VkImageView depthImageView, VkImageView swapChainImageView) {
        std::array<VkImageView, 3> attachments[] = {
                colorImageView,
                depthImageView,
                swapChainImageView
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = vulkanRenderPass->getRenderPass();
        framebufferInfo.attachmentCount = (uint32_t) attachments->size();
        framebufferInfo.pAttachments = attachments->data();
        framebufferInfo.width = vulkanSwapChain->getExtent().width;
        framebufferInfo.height = vulkanSwapChain->getExtent().height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(vulkanDevice->getDevice(), &framebufferInfo, ALLOCATOR, &framebuffer) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not create Vulkan framebuffer");
            return false;
        }
        return true;
    }

    void VulkanFramebuffer::terminate() {
        vkDestroyFramebuffer(vulkanDevice->getDevice(), framebuffer, ALLOCATOR);
    }

}
