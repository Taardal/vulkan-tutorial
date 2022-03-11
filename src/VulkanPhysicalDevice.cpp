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
            int score = rateDeviceSuitability(device);
            candidates.insert(std::make_pair(score, device));
        }
        if (candidates.rbegin()->first > 0) {
            physicalDevice = candidates.rbegin()->second;
        } else {
            VD_LOG_ERROR("Could not get physical device");
            return false;
        }

        return true;
    }

    void VulkanPhysicalDevice::terminate() {

    }

#define TO_STRING(value) #value

    int VulkanPhysicalDevice::rateDeviceSuitability(VkPhysicalDevice device) const {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        //VD_LOG_DEBUG("{0}, {1}", deviceProperties.deviceName, deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU" : deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU" : "UNKNOWN");

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        int score = 0;

        // Discrete GPUs have a significant performance advantage
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }

        // Maximum possible size of textures affects graphics quality
        score += (int) deviceProperties.limits.maxImageDimension2D;

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
        VD_LOG_DEBUG("{0} {1}: {2}", deviceProperties.deviceName, deviceType, score);
        return score;
    }

    bool VulkanPhysicalDevice::isDeviceSuitable(VkPhysicalDevice device) const {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        VD_LOG_DEBUG("{0}, {1}", deviceProperties.deviceName, deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU" : deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU" : "UNKNOWN");

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        //return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
        return true;
    }

}