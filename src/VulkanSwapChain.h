#pragma once

#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "Vulkan.h"
#include "Window.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace Vulkandemo {

    class VulkanSwapChain {
    private:
        static const VkAllocationCallbacks* ALLOCATOR;

    private:
        VulkanDevice* vulkanDevice;
        VulkanPhysicalDevice* vulkanPhysicalDevice;
        Vulkan* vulkan;
        Window* window;
        VkSurfaceFormatKHR surfaceFormat;
        VkPresentModeKHR presentationMode;
        VkExtent2D extent;
        VkSwapchainKHR vkSwapChain = VK_NULL_HANDLE;
        std::vector<VkImage> vkImages;

    public:
        VulkanSwapChain(VulkanDevice* vulkanDevice, VulkanPhysicalDevice* vulkanPhysicalDevice, Vulkan* vulkan, Window* window);

        bool initialize();

        void terminate() const;

    private:
        VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;

        VkPresentModeKHR choosePresentationMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;

        VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities) const;

        std::string getPresentationModeAsString(VkPresentModeKHR presentMode) const;
    };

}


