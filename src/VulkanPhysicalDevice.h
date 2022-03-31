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
        VkSurfaceCapabilitiesKHR SurfaceCapabilities{};
        std::vector<VkSurfaceFormatKHR> SurfaceFormats;
        std::vector<VkPresentModeKHR> PresentModes;
    };

}

namespace Vulkandemo {

    class VulkanPhysicalDevice {
    private:
        struct DeviceInfo {
            VkPhysicalDevice PhysicalDevice = nullptr;
            VkPhysicalDeviceProperties Properties{};
            VkPhysicalDeviceFeatures Features{};
            std::vector<VkExtensionProperties> Extensions{};
            QueueFamilyIndices QueueFamilyIndices{};
            SwapChainInfo SwapChainInfo{};
        };

    private:
        Vulkan* vulkan;
        DeviceInfo deviceInfo{};

    public:
        explicit VulkanPhysicalDevice(Vulkan* vulkan);

        VkPhysicalDevice getPhysicalDevice() const;

        const VkPhysicalDeviceFeatures& getFeatures() const;

        const QueueFamilyIndices& getQueueFamilyIndices() const;

        const SwapChainInfo& getSwapChainInfo() const;

        const std::vector<const char*>& getExtensions() const;

        bool initialize();

    private:
        std::vector<DeviceInfo> findAvailableDevices() const;

        std::string getDeviceTypeAsString(VkPhysicalDeviceType deviceType) const;

        DeviceInfo findMostSuitableDevice(const std::vector<DeviceInfo>& availableDevices) const;

        std::vector<const char*>& getRequiredExtensions() const;

        const std::vector<const char*>& getOptionalExtensions() const;

        std::vector<VkExtensionProperties> findExtensions(VkPhysicalDevice device) const;

        QueueFamilyIndices findQueueFamilyIndices(VkPhysicalDevice device) const;

        SwapChainInfo findSwapChainInfo(VkPhysicalDevice device) const;

        bool hasRequiredExtensions(const std::vector<VkExtensionProperties>& availableDeviceExtensions) const;

        bool hasRequiredSwapChainSupport(const SwapChainInfo& swapChainInfo) const;

        bool hasRequiredQueueFamilyIndices(const QueueFamilyIndices& queueFamilyIndices) const;

        int getSuitabilityRating(const DeviceInfo& deviceInfo) const;
    };

}

