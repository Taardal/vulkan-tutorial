#include "VulkanDevice.h"
#include "Log.h"

#include <set>

namespace Vulkandemo {

    const VkAllocationCallbacks* VulkanDevice::ALLOCATOR = VK_NULL_HANDLE;

    VulkanDevice::VulkanDevice(Vulkan* vulkan, VulkanPhysicalDevice* vulkanPhysicalDevice) : vulkan(vulkan), vulkanPhysicalDevice(vulkanPhysicalDevice) {
    }

    VkDevice VulkanDevice::getVkDevice() const {
        return vkDevice;
    }

    bool VulkanDevice::initialize() {
        constexpr float queuePriority = 1.0f;
        const QueueFamilyIndices& indices = vulkanPhysicalDevice->getQueueFamilyIndices();

        std::set<uint32_t> queueFamilies = {
                indices.GraphicsFamily.value(),
                indices.PresentationFamily.value()
        };

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        for (uint32_t queueFamily : queueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfo.queueCount = 1;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pEnabledFeatures = &vulkanPhysicalDevice->getVkPhysicalDeviceFeatures();
        createInfo.enabledExtensionCount = vulkanPhysicalDevice->getExtensions().size();
        createInfo.ppEnabledExtensionNames = vulkanPhysicalDevice->getExtensions().data();
        createInfo.queueCreateInfoCount = queueCreateInfos.size();
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        if (vulkan->isValidationLayersEnabled()) {
            createInfo.enabledLayerCount = vulkan->getValidationLayers().size();
            createInfo.ppEnabledLayerNames = vulkan->getValidationLayers().data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(vulkanPhysicalDevice->getVkPhysicalDevice(), &createInfo, ALLOCATOR, &vkDevice) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not create Vulkan device");
            return false;
        }
        VD_LOG_INFO("Created Vulkan device");

        constexpr int queueIndex = 0;
        vkGetDeviceQueue(vkDevice, indices.GraphicsFamily.value(), queueIndex, &graphicsVkQueue);
        if (graphicsVkQueue == VK_NULL_HANDLE) {
            VD_LOG_ERROR("Could not get graphics Vulkan queue handle");
            return false;
        }
        vkGetDeviceQueue(vkDevice, indices.PresentationFamily.value(), queueIndex, &presentationVkQueue);
        if (presentationVkQueue == VK_NULL_HANDLE) {
            VD_LOG_ERROR("Could not get presentation Vulkan queue handle");
            return false;
        }
        VD_LOG_INFO("Initialized Vulkan queue handles");
        return true;
    }

    void VulkanDevice::terminate() const {
        vkDestroyDevice(vkDevice, ALLOCATOR);
        VD_LOG_INFO("Destroyed Vulkan device");
    }

}