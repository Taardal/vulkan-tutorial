#include "VulkanPhysicalDevice.h"
#include "Log.h"

#include <map>
#include <vector>

namespace Vulkandemo {

    VulkanPhysicalDevice::VulkanPhysicalDevice(Vulkan* vulkan) : vulkan(vulkan) {
    }

    VkPhysicalDevice VulkanPhysicalDevice::getPhysicalDevice() const {
        return deviceInfo.PhysicalDevice;
    }

    const VkPhysicalDeviceProperties& VulkanPhysicalDevice::getProperties() const {
        return deviceInfo.Properties;
    }

    const VkPhysicalDeviceFeatures& VulkanPhysicalDevice::getFeatures() const {
        return deviceInfo.Features;
    }

    const QueueFamilyIndices& VulkanPhysicalDevice::getQueueFamilyIndices() const {
        return deviceInfo.QueueFamilyIndices;
    }

    const SwapChainInfo& VulkanPhysicalDevice::getSwapChainInfo() const {
        return deviceInfo.SwapChainInfo;
    }

    const std::vector<const char*>& VulkanPhysicalDevice::getExtensions() const {
        return getRequiredExtensions();
    }

    VkSampleCountFlagBits VulkanPhysicalDevice::getSampleCount() const {
        return deviceInfo.SampleCount;
    }

    bool VulkanPhysicalDevice::initialize() {
        std::vector<VulkanPhysicalDevice::DeviceInfo> availableDevices = findAvailableDevices();
        if (availableDevices.empty()) {
            VD_LOG_ERROR("Could not get any available devices");
            return false;
        }
        this->deviceInfo = findMostSuitableDevice(availableDevices);
        if (this->deviceInfo.PhysicalDevice == nullptr) {
            VD_LOG_ERROR("Could not get any suitable device");
            return false;
        }
        VD_LOG_INFO("Initialized Vulkan physical device");
        return true;
    }

    void VulkanPhysicalDevice::updateSwapChainInfo() {
        deviceInfo.SwapChainInfo = findSwapChainInfo(deviceInfo.PhysicalDevice);
    }

    uint32_t VulkanPhysicalDevice::findMemoryType(uint32_t memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags) const {
        VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
        vkGetPhysicalDeviceMemoryProperties(deviceInfo.PhysicalDevice, &physicalDeviceMemoryProperties);
        for (uint32_t memoryTypeIndex = 0; memoryTypeIndex < physicalDeviceMemoryProperties.memoryTypeCount; memoryTypeIndex++) {
            /*
             * The memoryTypeBits parameter will be used to specify the bit field of memory types that are suitable.
             * That means that we can find the index of a suitable memory type by simply iterating over them and checking if the corresponding bit is set to 1.
             */
            bool isSuitableType = (memoryTypeBits & (1 << memoryTypeIndex)) > 0;

            /*
             * However, we're not just interested in a memory type that is suitable for the buffer. We also need to ensure that it has the necessary properties
             * The memoryTypes array consists of VkMemoryType structs that specify the heap and properties of each type of memory.
             */
            VkMemoryType& memoryType = physicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex];
            bool hasNecessaryProperties = (memoryType.propertyFlags & memoryPropertyFlags) == memoryPropertyFlags;

            if (isSuitableType && hasNecessaryProperties) {
                return memoryTypeIndex;
            }
        }
        VD_LOG_WARN("Could not find memory type [{}]", memoryTypeBits);
        return -1;
    }

    VkFormat VulkanPhysicalDevice::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(deviceInfo.PhysicalDevice, format, &props);
            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }
        VD_LOG_WARN("Could not find supported format");
        return VK_FORMAT_UNDEFINED;
    }

    std::vector<VulkanPhysicalDevice::DeviceInfo> VulkanPhysicalDevice::findAvailableDevices() const {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(vulkan->getVulkanInstance(), &deviceCount, nullptr);

        std::vector<VkPhysicalDevice> vkPhysicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(vulkan->getVulkanInstance(), &deviceCount, vkPhysicalDevices.data());

        std::vector<DeviceInfo> devices;
        for (VkPhysicalDevice vkPhysicalDevice : vkPhysicalDevices) {

            VkPhysicalDeviceProperties vkPhysicalDeviceProperties;
            vkGetPhysicalDeviceProperties(vkPhysicalDevice, &vkPhysicalDeviceProperties);

            VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures;
            vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &vkPhysicalDeviceFeatures);

            /*
             * There are certain limitations of our current MSAA implementation which may impact the quality of the output image in more detailed scenes.
             * For example, we're currently not solving potential problems caused by shader aliasing, i.e. MSAA only smoothens out the edges of geometry but not the interior filling.
             * This may lead to a situation when you get a smooth polygon rendered on screen but the applied texture will still look aliased if it contains high contrasting colors.
             * One way to approach this problem is to enable Sample Shading which will improve the image quality even further, though at an additional performance cost:
             */
            //vkPhysicalDeviceFeatures.sampleRateShading = VK_TRUE; // enable sample shading feature for the device

            DeviceInfo device{};
            device.PhysicalDevice = vkPhysicalDevice;
            device.Properties = vkPhysicalDeviceProperties;
            device.Features = vkPhysicalDeviceFeatures;
            device.Extensions = findExtensions(vkPhysicalDevice);
            device.QueueFamilyIndices = findQueueFamilyIndices(vkPhysicalDevice);
            device.SwapChainInfo = findSwapChainInfo(vkPhysicalDevice);
            device.SampleCount = getSampleCount(vkPhysicalDeviceProperties);

            devices.push_back(device);
        }
        VD_LOG_DEBUG("Available physical devices [{0}]", deviceCount);
        for (const DeviceInfo& device : devices) {
            VD_LOG_DEBUG("{0} --> {1}", device.Properties.deviceName, getDeviceTypeAsString(device.Properties.deviceType));
        }
        return devices;
    }

    std::vector<VkExtensionProperties> VulkanPhysicalDevice::findExtensions(VkPhysicalDevice device) const {
        const char* layerName = nullptr;

        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, layerName, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, layerName, &extensionCount, extensions.data());

        VD_LOG_DEBUG("Available device extensions [{0}]", extensions.size());
        for (const VkExtensionProperties& extensionProperties : extensions) {
            VD_LOG_DEBUG(extensionProperties.extensionName);
            for (const char* optionalExtension : getOptionalExtensions()) {
                if (strcmp(extensionProperties.extensionName, optionalExtension) == 0) {
                    getRequiredExtensions().push_back(optionalExtension);
                }
            }
        }
        return extensions;
    }

    std::vector<const char*>& VulkanPhysicalDevice::getRequiredExtensions() const {
        static std::vector<const char*> extensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        return extensions;
    }

    const std::vector<const char*>& VulkanPhysicalDevice::getOptionalExtensions() const {
        static std::vector<const char*> extensions = {
                "VK_KHR_portability_subset"
        };
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
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vulkan->getSurface(), &presentationSupport);
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

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vulkan->getSurface(), &swapChainInfo.SurfaceCapabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, vulkan->getSurface(), &formatCount, nullptr);
        swapChainInfo.SurfaceFormats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, vulkan->getSurface(), &formatCount, swapChainInfo.SurfaceFormats.data());

        uint32_t presentationModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, vulkan->getSurface(), &presentationModeCount, nullptr);
        swapChainInfo.PresentModes.resize(presentationModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, vulkan->getSurface(), &presentationModeCount, swapChainInfo.PresentModes.data());

        return swapChainInfo;
    }

    VkSampleCountFlagBits VulkanPhysicalDevice::getSampleCount(const VkPhysicalDeviceProperties& deviceProperties) const {
        VkSampleCountFlags counts = deviceProperties.limits.framebufferColorSampleCounts & deviceProperties.limits.framebufferDepthSampleCounts;
        if (counts & VK_SAMPLE_COUNT_64_BIT) {
            return VK_SAMPLE_COUNT_64_BIT;
        }
        if (counts & VK_SAMPLE_COUNT_32_BIT) {
            return VK_SAMPLE_COUNT_32_BIT;
        }
        if (counts & VK_SAMPLE_COUNT_16_BIT) {
            return VK_SAMPLE_COUNT_16_BIT;
        }
        if (counts & VK_SAMPLE_COUNT_8_BIT) {
            return VK_SAMPLE_COUNT_8_BIT;
        }
        if (counts & VK_SAMPLE_COUNT_4_BIT) {
            return VK_SAMPLE_COUNT_4_BIT;
        }
        if (counts & VK_SAMPLE_COUNT_2_BIT) {
            return VK_SAMPLE_COUNT_2_BIT;
        }
        return VK_SAMPLE_COUNT_1_BIT;
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

    VulkanPhysicalDevice::DeviceInfo VulkanPhysicalDevice::findMostSuitableDevice(const std::vector<VulkanPhysicalDevice::DeviceInfo>& availableDevices) const {
        std::multimap<uint32_t, VulkanPhysicalDevice::DeviceInfo> devicesByRating;
        VD_LOG_DEBUG("Device suitability ratings");
        for (const VulkanPhysicalDevice::DeviceInfo& device : availableDevices) {
            uint32_t suitabilityRating = getSuitabilityRating(device);
            VD_LOG_DEBUG("{0} --> {1}", device.Properties.deviceName, suitabilityRating);
            devicesByRating.insert(std::make_pair(suitabilityRating, device));
        }
        uint32_t highestRating = devicesByRating.rbegin()->first;
        if (highestRating == 0) {
            return {};
        }
        VD_LOG_DEBUG("Most suitable device");
        const DeviceInfo& device = devicesByRating.rbegin()->second;
        VD_LOG_DEBUG("{0}", device.Properties.deviceName);
        return device;
    }

    uint32_t VulkanPhysicalDevice::getSuitabilityRating(const VulkanPhysicalDevice::DeviceInfo& deviceInfo) const {
        if (!hasRequiredFeatures(deviceInfo.Features)) {
            VD_LOG_DEBUG("{0} does not have required device features", deviceInfo.Properties.deviceName);
            return 0;
        }
        if (!hasRequiredExtensions(deviceInfo.Extensions)) {
            VD_LOG_DEBUG("{0} does not have required device extensions", deviceInfo.Properties.deviceName);
            return 0;
        }
        if (!hasRequiredSwapChainSupport(deviceInfo.SwapChainInfo)) {
            VD_LOG_DEBUG("{0} does not have required swap chain info", deviceInfo.Properties.deviceName);
            return 0;
        }
        if (!hasRequiredQueueFamilyIndices(deviceInfo.QueueFamilyIndices)) {
            VD_LOG_DEBUG("{0} does not have required queue family indices", deviceInfo.Properties.deviceName);
            return 0;
        }
        uint32_t rating = 0;
        rating += (uint32_t) deviceInfo.Properties.limits.maxImageDimension2D;
        rating += (uint32_t) deviceInfo.Properties.limits.framebufferColorSampleCounts;
        rating += (uint32_t) deviceInfo.Properties.limits.framebufferDepthSampleCounts;
        if (deviceInfo.Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            rating += 1000;
        }
        return rating;
    }

    bool VulkanPhysicalDevice::hasRequiredFeatures(const VkPhysicalDeviceFeatures& availableDeviceFeatures) const {
        return availableDeviceFeatures.samplerAnisotropy;
    }

    bool VulkanPhysicalDevice::hasRequiredExtensions(const std::vector<VkExtensionProperties>& availableDeviceExtensions) const {
        for (const char* requiredExtension : this->getRequiredExtensions()) {
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
        return !swapChainInfo.SurfaceFormats.empty() && !swapChainInfo.PresentModes.empty();
    }

    bool VulkanPhysicalDevice::hasRequiredQueueFamilyIndices(const QueueFamilyIndices& queueFamilyIndices) const {
        return queueFamilyIndices.GraphicsFamily.has_value() && queueFamilyIndices.PresentationFamily.has_value();
    }

}