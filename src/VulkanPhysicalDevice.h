#pragma once

#include "Vulkan.h"
#include <vulkan/vulkan.h>

namespace Vulkandemo {

    class VulkanPhysicalDevice {
    private:
        Vulkan* vulkan = nullptr;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    public:
        explicit VulkanPhysicalDevice(Vulkan* vulkan);

        bool initialize();

        void terminate();

    private:
        int rateDeviceSuitability(VkPhysicalDevice device) const;

        bool isDeviceSuitable(VkPhysicalDevice device) const;
    };

}

