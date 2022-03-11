#pragma once

#include "Vulkan.h"

#include <vulkan/vulkan.h>
#include <optional>
#include <vector>

namespace Vulkandemo {

    struct QueueFamilyIndices {
        std::optional<uint32_t> GraphicsFamily;
    };

}

namespace Vulkandemo {

    class VulkanPhysicalDevice {
    private:
        struct Info {
            VkPhysicalDevice Device = nullptr;
            VkPhysicalDeviceProperties DeviceProperties{};
            VkPhysicalDeviceFeatures DeviceFeatures{};
            QueueFamilyIndices QueueFamilyIndices;
        };

    private:
        Vulkan* vulkan;
        VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
        QueueFamilyIndices queueFamilies{};

    public:
        explicit VulkanPhysicalDevice(Vulkan* vulkan);

        VkPhysicalDevice getVkPhysicalDevice() const;

        const QueueFamilyIndices& getQueueFamilies() const;

        bool initialize();

    private:
        std::vector<Info> getAvailableDevices() const;

        std::string getDeviceTypeAsString(VkPhysicalDeviceType deviceType) const;

        Info getMostEligibleDevice(const std::vector<Info>& availableDevices) const;

        QueueFamilyIndices getQueueFamilyIndices(VkPhysicalDevice device) const;

        int getRating(const Info& deviceInfo) const;
    };

}

