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
        struct DeviceInfo {
            VkPhysicalDevice VkDevice = nullptr;
            VkPhysicalDeviceProperties VkDeviceProperties{};
            VkPhysicalDeviceFeatures VkDeviceFeatures{};
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
        std::vector<DeviceInfo> getAvailableDevices() const;

        std::string getDeviceTypeAsString(VkPhysicalDeviceType deviceType) const;

        DeviceInfo getMostEligibleDevice(const std::vector<DeviceInfo>& availableDevices) const;

        QueueFamilyIndices getQueueFamilyIndices(VkPhysicalDevice device) const;

        int getRating(const DeviceInfo& deviceInfo) const;
    };

}

