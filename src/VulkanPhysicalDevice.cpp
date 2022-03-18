#include "VulkanPhysicalDevice.h"
#include "Log.h"

#include <map>
#include <vector>

namespace Vulkandemo {

    VulkanPhysicalDevice::VulkanPhysicalDevice(Vulkan* vulkan) : vulkan(vulkan) {
    }

    VkPhysicalDevice VulkanPhysicalDevice::getVkPhysicalDevice() const {
        return deviceInfo.VkPhysicalDevice;
    }

    const VkPhysicalDeviceFeatures& VulkanPhysicalDevice::getVkPhysicalDeviceFeatures() const {
        return deviceInfo.VkPhysicalDeviceFeatures;
    }

    const QueueFamilyIndices& VulkanPhysicalDevice::getQueueFamilyIndices() const {
        return deviceInfo.QueueFamilyIndices;
    }

    const SwapChainInfo& VulkanPhysicalDevice::getSwapChainInfo() const {
        return deviceInfo.SwapChainInfo;
    }

    const std::vector<const char*>& VulkanPhysicalDevice::getExtensions() const {
        static const std::vector<const char*> extensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        return extensions;
    }

    bool VulkanPhysicalDevice::initialize() {
        std::vector<VulkanPhysicalDevice::DeviceInfo> availableDevices = findAvailableDevices();
        if (availableDevices.empty()) {
            VD_LOG_ERROR("Could not get any available devices");
            return false;
        }
        this->deviceInfo = findMostSuitableDevice(availableDevices);
        if (this->deviceInfo.VkPhysicalDevice == nullptr) {
            VD_LOG_ERROR("Could not get any suitable device");
            return false;
        }
        VD_LOG_INFO("Initialized Vulkan physical device");
        return true;
    }

    std::vector<VulkanPhysicalDevice::DeviceInfo> VulkanPhysicalDevice::findAvailableDevices() const {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(vulkan->getVkInstance(), &deviceCount, nullptr);

        std::vector<VkPhysicalDevice> vkPhysicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(vulkan->getVkInstance(), &deviceCount, vkPhysicalDevices.data());

        std::vector<DeviceInfo> devices;
        for (VkPhysicalDevice vkPhysicalDevice : vkPhysicalDevices) {

            VkPhysicalDeviceProperties vkPhysicalDeviceProperties;
            vkGetPhysicalDeviceProperties(vkPhysicalDevice, &vkPhysicalDeviceProperties);

            VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures;
            vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &vkPhysicalDeviceFeatures);

            DeviceInfo device{};
            device.VkPhysicalDevice = vkPhysicalDevice;
            device.VkPhysicalDeviceProperties = vkPhysicalDeviceProperties;
            device.VkPhysicalDeviceFeatures = vkPhysicalDeviceFeatures;
            device.VkExtensionPropertiesList = findDeviceExtensions(vkPhysicalDevice);
            device.QueueFamilyIndices = findQueueFamilyIndices(vkPhysicalDevice);
            device.SwapChainInfo = findSwapChainInfo(vkPhysicalDevice);

            devices.push_back(device);
        }
        VD_LOG_DEBUG("Available physical devices [{0}]", deviceCount);
        for (const DeviceInfo& device : devices) {
            VD_LOG_DEBUG("{0} --> {1}", device.VkPhysicalDeviceProperties.deviceName, getDeviceTypeAsString(device.VkPhysicalDeviceProperties.deviceType));
        }
        return devices;
    }

    std::vector<VkExtensionProperties> VulkanPhysicalDevice::findDeviceExtensions(VkPhysicalDevice device) const {
        const char* layerName = nullptr;

        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, layerName, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, layerName, &extensionCount, extensions.data());

        VD_LOG_DEBUG("Available device extensions [{0}]", extensions.size());
        for (const VkExtensionProperties& extensionProperties: extensions) {
            VD_LOG_DEBUG(extensionProperties.extensionName);
        }
        return extensions;
    }

    QueueFamilyIndices VulkanPhysicalDevice::findQueueFamilyIndices(VkPhysicalDevice device) const {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        QueueFamilyIndices indices;
        for (int i = 0; i < queueFamilies.size(); i++) {
            const VkQueueFamilyProperties& queueFamily = queueFamilies[i];
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.GraphicsFamily = i;
            }
            VkBool32 presentationSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vulkan->getVkSurface(), &presentationSupport);
            if (presentationSupport) {
                indices.PresentationFamily = i;
            }
            if (indices.GraphicsFamily.has_value() && indices.PresentationFamily.has_value()) {
                break;
            }
        }
        return indices;
    }

    SwapChainInfo VulkanPhysicalDevice::findSwapChainInfo(VkPhysicalDevice device) const {
        SwapChainInfo swapChainInfo;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vulkan->getVkSurface(), &swapChainInfo.VkSurfaceCapabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, vulkan->getVkSurface(), &formatCount, nullptr);
        swapChainInfo.VkSurfaceFormats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, vulkan->getVkSurface(), &formatCount, swapChainInfo.VkSurfaceFormats.data());

        uint32_t presentationModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, vulkan->getVkSurface(), &presentationModeCount, nullptr);
        swapChainInfo.VkPresentationModes.resize(presentationModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, vulkan->getVkSurface(), &presentationModeCount, swapChainInfo.VkPresentationModes.data());

        return swapChainInfo;
    }

    VulkanPhysicalDevice::DeviceInfo VulkanPhysicalDevice::findMostSuitableDevice(const std::vector<VulkanPhysicalDevice::DeviceInfo>& availableDevices) const {
        std::multimap<int, VulkanPhysicalDevice::DeviceInfo> devicesByRating;
        VD_LOG_DEBUG("Device suitability ratings");
        for (const VulkanPhysicalDevice::DeviceInfo& device : availableDevices) {
            int suitabilityRating = getSuitabilityRating(device);
            VD_LOG_DEBUG("{0} --> {1}", device.VkPhysicalDeviceProperties.deviceName, suitabilityRating);
            devicesByRating.insert(std::make_pair(suitabilityRating, device));
        }
        int highestRating = devicesByRating.rbegin()->first;
        if (highestRating == 0) {
            return {};
        }
        VD_LOG_DEBUG("Most suitable device");
        const DeviceInfo& device = devicesByRating.rbegin()->second;
        VD_LOG_DEBUG("{0}", device.VkPhysicalDeviceProperties.deviceName);
        return device;
    }

    int VulkanPhysicalDevice::getSuitabilityRating(const VulkanPhysicalDevice::DeviceInfo& deviceInfo) const {
        if (!hasRequiredDeviceExtensions(deviceInfo.VkExtensionPropertiesList)) {
            VD_LOG_DEBUG("{0} does not have required device extensions", deviceInfo.VkPhysicalDeviceProperties.deviceName);
            return 0;
        }
        if (!hasRequiredSwapChainSupport(deviceInfo.SwapChainInfo)) {
            VD_LOG_DEBUG("{0} does not have required swap chain info", deviceInfo.VkPhysicalDeviceProperties.deviceName);
            return 0;
        }
        if (!hasRequiredQueueFamilyIndices(deviceInfo.QueueFamilyIndices)) {
            VD_LOG_DEBUG("{0} does not have required queue family indices", deviceInfo.VkPhysicalDeviceProperties.deviceName);
            return 0;
        }
        int rating = (int) deviceInfo.VkPhysicalDeviceProperties.limits.maxImageDimension2D;
        if (deviceInfo.VkPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            rating += 1000;
        }
        return rating;
    }

    bool VulkanPhysicalDevice::hasRequiredDeviceExtensions(const std::vector<VkExtensionProperties>& availableDeviceExtensions) const {
        for (const char* requiredExtension : this->getExtensions()) {
            bool requiredExtensionFound = false;
            for (const VkExtensionProperties& availableExtension : availableDeviceExtensions) {
                if (strcmp(requiredExtension, availableExtension.extensionName) == 0) {
                    requiredExtensionFound = true;
                    break;
                }
            }
            if (!requiredExtensionFound) {
                VD_LOG_WARN("Could not find required extension [{0}]", requiredExtensionFound);
                return false;
            }
        }
        return true;
    }

    bool VulkanPhysicalDevice::hasRequiredSwapChainSupport(const SwapChainInfo& swapChainInfo) const {
        return !swapChainInfo.VkSurfaceFormats.empty() && !swapChainInfo.VkPresentationModes.empty();
    }

    bool VulkanPhysicalDevice::hasRequiredQueueFamilyIndices(const QueueFamilyIndices& queueFamilyIndices) const {
        return queueFamilyIndices.GraphicsFamily.has_value() && queueFamilyIndices.PresentationFamily.has_value();
    }

    std::string VulkanPhysicalDevice::getDeviceTypeAsString(VkPhysicalDeviceType deviceType) const {
        switch (deviceType) {
            case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                return "VK_PHYSICAL_DEVICE_TYPE_OTHER";
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                return "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU";
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                return "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU";
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                return "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU";
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                return "VK_PHYSICAL_DEVICE_TYPE_CPU";
            default:
                return "";
        }
    }

}