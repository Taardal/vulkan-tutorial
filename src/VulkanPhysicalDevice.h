#pragma once

#include "Vulkan.h"

#include <vulkan/vulkan.h>
#include <optional>
#include <vector>

namespace Vulkandemo {

    struct QueueFamilyIndices {
        std::optional<uint32_t> GraphicsFamily;
        std::optional<uint32_t> PresentationFamily;
    };

    struct SwapChainInfo {
        VkSurfaceCapabilitiesKHR VkSurfaceCapabilities{};
        std::vector<VkSurfaceFormatKHR> VkSurfaceFormats;
        std::vector<VkPresentModeKHR> VkPresentationModes;
    };

}

namespace Vulkandemo {

    class VulkanPhysicalDevice {
    private:
        struct DeviceInfo {
            VkPhysicalDevice VkPhysicalDevice = nullptr;
            VkPhysicalDeviceProperties VkPhysicalDeviceProperties{};
            VkPhysicalDeviceFeatures VkPhysicalDeviceFeatures{};
            std::vector<VkExtensionProperties> VkExtensionPropertiesList{};
            QueueFamilyIndices QueueFamilyIndices{};
            SwapChainInfo SwapChainInfo{};
        };

    private:
        Vulkan* vulkan;
        DeviceInfo deviceInfo{};

    public:
        explicit VulkanPhysicalDevice(Vulkan* vulkan);

        VkPhysicalDevice getVkPhysicalDevice() const;

        const VkPhysicalDeviceFeatures& getVkPhysicalDeviceFeatures() const;

        const QueueFamilyIndices& getQueueFamilyIndices() const;

        const SwapChainInfo& getSwapChainInfo() const;

        const std::vector<const char*>& getExtensions() const;

        bool initialize();

    private:
        std::vector<DeviceInfo> findAvailableDevices() const;

        std::string getDeviceTypeAsString(VkPhysicalDeviceType deviceType) const;

        DeviceInfo findMostSuitableDevice(const std::vector<DeviceInfo>& availableDevices) const;

        std::vector<VkExtensionProperties> findDeviceExtensions(VkPhysicalDevice device) const;

        QueueFamilyIndices findQueueFamilyIndices(VkPhysicalDevice device) const;

        SwapChainInfo findSwapChainInfo(VkPhysicalDevice device) const;

        bool hasRequiredDeviceExtensions(const std::vector<VkExtensionProperties>& availableDeviceExtensions) const;

        bool hasRequiredSwapChainSupport(const SwapChainInfo& swapChainInfo) const;

        bool hasRequiredQueueFamilyIndices(const QueueFamilyIndices& queueFamilyIndices) const;

        int getSuitabilityRating(const DeviceInfo& deviceInfo) const;
    };

}

