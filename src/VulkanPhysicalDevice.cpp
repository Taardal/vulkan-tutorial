#include "VulkanPhysicalDevice.h"
#include "Log.h"

#include <map>
#include <vector>

namespace Vulkandemo {

    VulkanPhysicalDevice::VulkanPhysicalDevice(Vulkan* vulkan) : vulkan(vulkan) {
    }

    bool VulkanPhysicalDevice::initialize() {

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(vulkan->getVkInstance(), &deviceCount, nullptr);
        if (deviceCount == 0) {
            VD_LOG_ERROR("Could not get physical device count");
            return false;
        }
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(vulkan->getVkInstance(), &deviceCount, devices.data());
        VD_LOG_DEBUG("Physical devices [{0}]", devices.size());

        std::multimap<int, VkPhysicalDevice> candidates;
        for (VkPhysicalDevice device : devices) {
            int rating = getRating(device);
            candidates.insert(std::make_pair(rating, device));
        }
        int highestRating = candidates.rbegin()->first;
        if (highestRating > 0) {
            physicalDevice = candidates.rbegin()->second;
        } else {
            VD_LOG_ERROR("Could not get physical device");
            return false;
        }
        return true;
    }

    VulkanPhysicalDevice::QueueFamilyIndices VulkanPhysicalDevice::findQueueFamilies(VkPhysicalDevice device) const {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for (int i = 0; i < queueFamilies.size(); i++) {
            const VkQueueFamilyProperties& queueFamily = queueFamilies[i];
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.GraphicsFamily = i;
                break;
            }
        }
        return indices;
    }

    int VulkanPhysicalDevice::getRating(VkPhysicalDevice device) const {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        int rating = 0;

        // Discrete GPUs have a significant performance advantage
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            rating += 1000;
        }

        // Maximum possible size of textures affects graphics quality
        rating += (int) deviceProperties.limits.maxImageDimension2D;

        QueueFamilyIndices indices = findQueueFamilies(device);
        if (!indices.GraphicsFamily.has_value()) {
            rating = 0;
        }

        const char* deviceType;
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_OTHER) {
            deviceType = "OTHER";
        } else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
            deviceType = "INTEGRATED";
        } else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            deviceType = "DISCRETE";
        } else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU) {
            deviceType = "VIRTUAL";
        } else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU) {
            deviceType = "CPU";
        } else {
            deviceType = "UNKNOWN";
        }

        VD_LOG_DEBUG("{0} [{1}]: {2}", deviceProperties.deviceName, deviceType, rating);
        return rating;
    }

}