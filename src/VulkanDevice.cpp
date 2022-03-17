#include "VulkanDevice.h"
#include "Log.h"

namespace Vulkandemo {

    const VkAllocationCallbacks* VulkanDevice::ALLOCATOR = VK_NULL_HANDLE;

    VulkanDevice::VulkanDevice(Vulkan* vulkan, VulkanPhysicalDevice* vulkanPhysicalDevice) : vulkan(vulkan), vulkanPhysicalDevice(vulkanPhysicalDevice) {
    }

    bool VulkanDevice::initialize() const {
        uint32_t graphicsQueueFamilyIndex = vulkanPhysicalDevice->getQueueFamilies().GraphicsFamily.value();
        constexpr float graphicsQueuePriority = 1.0f;

        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &graphicsQueuePriority;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &vulkanPhysicalDevice->getVkDeviceFeatures();
        createInfo.enabledExtensionCount = 0;

        if (vulkan->isValidationLayersEnabled()) {
            createInfo.enabledLayerCount = vulkan->getValidationLayers().size();
            createInfo.ppEnabledLayerNames = vulkan->getValidationLayers().data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(vulkanPhysicalDevice->getVkPhysicalDevice(), &createInfo, ALLOCATOR, (VkDevice*) &vkDevice) != VK_SUCCESS) {
            VD_LOG_ERROR("Could not create Vulkan device");
            return false;
        }

        constexpr int graphicsQueueIndex = 0;
        vkGetDeviceQueue(vkDevice, graphicsQueueFamilyIndex, graphicsQueueIndex, (VkQueue*) &graphicsQueue);
        if (graphicsQueue == VK_NULL_HANDLE) {
            VD_LOG_ERROR("Could not get graphics queue handle");
            return false;
        }

        return true;
    }

    void VulkanDevice::terminate() const {
        vkDestroyDevice(vkDevice, ALLOCATOR);
        VD_LOG_INFO("Destroyed Vulkan device");
    }

}