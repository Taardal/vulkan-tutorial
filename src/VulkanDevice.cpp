#include "VulkanDevice.h"
#include "Log.h"

#include <set>

namespace Vulkandemo {

    const VkAllocationCallbacks* VulkanDevice::ALLOCATOR = VK_NULL_HANDLE;

    VulkanDevice::VulkanDevice(Vulkan* vulkan, VulkanPhysicalDevice* vulkanPhysicalDevice) : vulkan(vulkan), vulkanPhysicalDevice(vulkanPhysicalDevice) {
    }

    bool VulkanDevice::initialize() const {
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
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = queueCreateInfos.size();
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &vulkanPhysicalDevice->getVkDeviceFeatures();
        createInfo.enabledExtensionCount = 0;

        if (vulkan->areValidationLayersEnabled()) {
            createInfo.enabledLayerCount = vulkan->getValidationLayers().size();
            createInfo.ppEnabledLayerNames = vulkan->getValidationLayers().data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(vulkanPhysicalDevice->getVkPhysicalDevice(), &createInfo, ALLOCATOR, (VkDevice*) &vkDevice) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not create Vulkan device");
            return false;
        }

        constexpr int queueIndex = 0;
        vkGetDeviceQueue(vkDevice, indices.GraphicsFamily.value(), queueIndex, (VkQueue*) &graphicsVkQueue);
        if (graphicsVkQueue == VK_NULL_HANDLE) {
            VD_LOG_ERROR("Could not get graphics Vulkan queue handle");
            return false;
        }
        vkGetDeviceQueue(vkDevice, indices.PresentationFamily.value(), queueIndex, (VkQueue*) &presentationVkQueue);
        if (presentationVkQueue == VK_NULL_HANDLE) {
            VD_LOG_ERROR("Could not get presentation Vulkan queue handle");
            return false;
        }

        return true;
    }

    void VulkanDevice::terminate() const {
        vkDestroyDevice(vkDevice, ALLOCATOR);
        VD_LOG_INFO("Destroyed Vulkan device");
    }

}