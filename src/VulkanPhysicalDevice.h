#pragma once

#include "Vulkan.h"

#include <vulkan/vulkan.h>
#include <optional>

namespace Vulkandemo {

    class VulkanPhysicalDevice {
    private:
        struct Info {
            VkPhysicalDevice Device;
            VkPhysicalDeviceProperties DeviceProperties;
            VkPhysicalDeviceFeatures DeviceFeatures;
        };

        struct QueueFamilyIndices {
            std::optional<uint32_t> GraphicsFamily;
        };

    private:
        Vulkan* vulkan = nullptr;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    public:
        explicit VulkanPhysicalDevice(Vulkan* vulkan);

        bool initialize();

    private:
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;

        int getRating(VkPhysicalDevice device) const;
    };

}

