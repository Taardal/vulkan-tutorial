#include "VulkanSwapChain.h"
#include "Log.h"

namespace Vulkandemo {

    const VkAllocationCallbacks* VulkanSwapChain::ALLOCATOR = VK_NULL_HANDLE;

    VulkanSwapChain::VulkanSwapChain(VulkanDevice* vulkanDevice, VulkanPhysicalDevice* vulkanPhysicalDevice, Vulkan* vulkan, Window* window)
            : vulkanDevice(vulkanDevice), vulkanPhysicalDevice(vulkanPhysicalDevice), vulkan(vulkan), window(window) {
    }

    bool VulkanSwapChain::initialize() {
        const SwapChainInfo& swapChainInfo = vulkanPhysicalDevice->getSwapChainInfo();

        surfaceFormat = chooseSurfaceFormat(swapChainInfo.VkSurfaceFormats);
        presentationMode = choosePresentationMode(swapChainInfo.VkPresentationModes);
        extent = chooseExtent(swapChainInfo.VkSurfaceCapabilities);

        uint32_t minImageCount = swapChainInfo.VkSurfaceCapabilities.minImageCount;
        uint32_t maxImageCount = swapChainInfo.VkSurfaceCapabilities.maxImageCount;

        uint32_t imageCount = minImageCount + 1;
        if (maxImageCount > 0 && imageCount > maxImageCount) {
            imageCount = maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = vulkan->getVkSurface();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        const QueueFamilyIndices& queueFamilyIndices = vulkanPhysicalDevice->getQueueFamilyIndices();
        if (queueFamilyIndices.GraphicsFamily != queueFamilyIndices.PresentationFamily) {
            uint32_t queueFamilyIndexValues[] = {
                    queueFamilyIndices.GraphicsFamily.value(),
                    queueFamilyIndices.PresentationFamily.value()
            };
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.pQueueFamilyIndices = queueFamilyIndexValues;
            createInfo.queueFamilyIndexCount = 2;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.pQueueFamilyIndices = nullptr;
            createInfo.queueFamilyIndexCount = 0;
        }

        createInfo.preTransform = swapChainInfo.VkSurfaceCapabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentationMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(vulkanDevice->getVkDevice(), &createInfo, ALLOCATOR, &vkSwapChain) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not create Vulkan swap chain");
            return false;
        }
        VD_LOG_INFO("Created Vulkan swap chain");

        vkGetSwapchainImagesKHR(vulkanDevice->getVkDevice(), vkSwapChain, &imageCount, nullptr);
        vkImages.resize(imageCount);
        vkGetSwapchainImagesKHR(vulkanDevice->getVkDevice(), vkSwapChain, &imageCount, vkImages.data());
        if (vkImages.size() != imageCount) {
            VD_LOG_ERROR("Could not initialize Vulkan swap chain image handles");
            return false;
        }
        VD_LOG_INFO("Initialized Vulkan swap chain image handles");

        return true;
    }

    void VulkanSwapChain::terminate() const {
        vkDestroySwapchainKHR(vulkanDevice->getVkDevice(), vkSwapChain, ALLOCATOR);
        VD_LOG_INFO("Destroyed Vulkan swap chain");
    }

    VkSurfaceFormatKHR VulkanSwapChain::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const {
        for (const auto& availableFormat: availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }
        VD_LOG_WARN("Could not find target format so defaulting to first available");
        return availableFormats[0];
    }

    VkPresentModeKHR VulkanSwapChain::choosePresentationMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const {
        VkPresentModeKHR targetPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
        for (const auto& availablePresentMode: availablePresentModes) {
            if (availablePresentMode == targetPresentMode) {
                return availablePresentMode;
            }
        }
        VkPresentModeKHR defaultPresentMode = VK_PRESENT_MODE_FIFO_KHR;
        VD_LOG_WARN("Could not find [{0}] presentation mode so defaulting to [{1}]", getPresentationModeAsString(targetPresentMode), getPresentationModeAsString(defaultPresentMode));
        return defaultPresentMode;
    }

    VkExtent2D VulkanSwapChain::chooseExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities) const {
        bool extentSizeCanDifferFromWindowResolution = surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max();
        if (!extentSizeCanDifferFromWindowResolution) {
            VD_LOG_DEBUG("Extent should match window resolution so using the surface capabilities extent");
            return surfaceCapabilities.currentExtent;
        }
        VD_LOG_DEBUG("Extent can differ from window resolution so picking the resolution that best matches the window within the minImageExtent and maxImageExtent bounds");
        Size windowSizeInPixels = window->getSizeInPixels();
        VkExtent2D extent = {
                (uint32_t) windowSizeInPixels.Width,
                (uint32_t) windowSizeInPixels.Height
        };
        extent.width = std::clamp(extent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
        extent.height = std::clamp(extent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
        return extent;
    }

    std::string VulkanSwapChain::getPresentationModeAsString(VkPresentModeKHR presentMode) const {
        switch (presentMode) {
            case VK_PRESENT_MODE_IMMEDIATE_KHR:
                return "VK_PRESENT_MODE_IMMEDIATE_KHR";
            case VK_PRESENT_MODE_MAILBOX_KHR:
                return "VK_PRESENT_MODE_MAILBOX_KHR";
            case VK_PRESENT_MODE_FIFO_KHR:
                return "VK_PRESENT_MODE_FIFO_KHR";
            case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
                return "VK_PRESENT_MODE_FIFO_RELAXED_KHR";
            case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
                return "VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR";
            case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
                return "VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR";
            case VK_PRESENT_MODE_MAX_ENUM_KHR:
                return "VK_PRESENT_MODE_MAX_ENUM_KHR";
            default:
                return "";
        }
    }

}
