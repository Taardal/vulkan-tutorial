#include "VulkanPhysicalDevice.h"
#include "Log.h"

#include <map>
#include <vector>

namespace Vulkandemo {

    VulkanPhysicalDevice::VulkanPhysicalDevice(Vulkan* vulkan) : vulkan(vulkan) {
    }

    VkPhysicalDevice VulkanPhysicalDevice::getVkPhysicalDevice() const {
        return vkPhysicalDevice;
    }

    const QueueFamilyIndices& VulkanPhysicalDevice::getQueueFamilies() const {
        return queueFamilies;
    }

    const VkPhysicalDeviceFeatures& VulkanPhysicalDevice::getVkDeviceFeatures() const {
        return deviceInfo.VkDeviceFeatures;
    }

    bool VulkanPhysicalDevice::initialize() {
        std::vector<VulkanPhysicalDevice::DeviceInfo> availableDevices = getAvailableDevices();
        if (availableDevices.empty()) {
            VD_LOG_ERROR("Could not get any available physical devices");
            return false;
        }
        VulkanPhysicalDevice::DeviceInfo mostEligibleDevice = getMostEligibleDevice(availableDevices);
        if (mostEligibleDevice.VkDevice == nullptr) {
            VD_LOG_ERROR("Could not get any eligible physical device");
            return false;
        }
        deviceInfo = mostEligibleDevice;
        vkPhysicalDevice = mostEligibleDevice.VkDevice;
        queueFamilies = mostEligibleDevice.QueueFamilyIndices;
        return true;
    }

    std::vector<VulkanPhysicalDevice::DeviceInfo> VulkanPhysicalDevice::getAvailableDevices() const {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(vulkan->getVkInstance(), &deviceCount, nullptr);

        std::vector<VkPhysicalDevice> vkPhysicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(vulkan->getVkInstance(), &deviceCount, vkPhysicalDevices.data());

        std::vector<DeviceInfo> devices;
        for (VkPhysicalDevice vkPhysicalDevice : vkPhysicalDevices) {

            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(vkPhysicalDevice, &deviceProperties);

            VkPhysicalDeviceFeatures deviceFeatures;
            vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &deviceFeatures);

            DeviceInfo device{};
            device.VkDevice = vkPhysicalDevice;
            device.VkDeviceProperties = deviceProperties;
            device.VkDeviceFeatures = deviceFeatures;
            device.QueueFamilyIndices = getQueueFamilyIndices(vkPhysicalDevice);

            devices.push_back(device);
        }
        VD_LOG_DEBUG("Available physical devices [{0}]", deviceCount);
        for (const DeviceInfo& device : devices) {
            VD_LOG_DEBUG("{0} --> {1}", device.VkDeviceProperties.deviceName, getDeviceTypeAsString(device.VkDeviceProperties.deviceType));
        }
        return devices;
    }

    QueueFamilyIndices VulkanPhysicalDevice::getQueueFamilyIndices(VkPhysicalDevice device) const {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        QueueFamilyIndices indices;
        for (int i = 0; i < queueFamilies.size(); i++) {
            const VkQueueFamilyProperties& queueFamily = queueFamilies[i];
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.GraphicsFamily = i;
                break;
            }
        }
        return indices;
    }

    VulkanPhysicalDevice::DeviceInfo VulkanPhysicalDevice::getMostEligibleDevice(const std::vector<VulkanPhysicalDevice::DeviceInfo>& availableDevices) const {
        std::multimap<int, VulkanPhysicalDevice::DeviceInfo> devicesByRating;
        VD_LOG_DEBUG("Device ratings");
        for (const VulkanPhysicalDevice::DeviceInfo& device : availableDevices) {
            int rating = getRating(device);
            VD_LOG_DEBUG("{0} --> {1}", device.VkDeviceProperties.deviceName, rating);
            devicesByRating.insert(std::make_pair(rating, device));
        }
        int highestRating = devicesByRating.rbegin()->first;
        if (highestRating == 0) {
            return {};
        }
        VD_LOG_DEBUG("Most eligible device");
        const DeviceInfo& device = devicesByRating.rbegin()->second;
        VD_LOG_DEBUG("{0}", device.VkDeviceProperties.deviceName);
        return device;
    }

    int VulkanPhysicalDevice::getRating(const VulkanPhysicalDevice::DeviceInfo& deviceInfo) const {
        if (!deviceInfo.QueueFamilyIndices.GraphicsFamily.has_value()) {
            return 0;
        }
        int rating = (int) deviceInfo.VkDeviceProperties.limits.maxImageDimension2D;
        if (deviceInfo.VkDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            rating += 1000;
        }
        return rating;
    }

    std::string VulkanPhysicalDevice::getDeviceTypeAsString(VkPhysicalDeviceType deviceType) const {
        if (deviceType == VK_PHYSICAL_DEVICE_TYPE_OTHER) {
            return "VK_PHYSICAL_DEVICE_TYPE_OTHER";
        } else if (deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
            return "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU";
        } else if (deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            return "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU";
        } else if (deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU) {
            return "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU";
        } else if (deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU) {
            return "VK_PHYSICAL_DEVICE_TYPE_CPU";
        } else {
            return "UNKNOWN";
        }
    }

}