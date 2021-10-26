#include "VulkanContext.h"
#include "Log.h"

#include <GLFW/glfw3.h>
#include <unordered_set>

namespace vulkandemo
{
    VulkanContext::VulkanContext(const Config& config)
        : config(config)
    {
    }

    bool VulkanContext::Initialize()
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = config.Name.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(config.MajorVersion, config.MinorVersion, config.PatchVersion);
        appInfo.pEngineName = config.Name.c_str();
        appInfo.engineVersion = VK_MAKE_VERSION(config.MajorVersion, config.MinorVersion, config.PatchVersion);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        const std::vector<const char*>& extensions = GetExtensions();
        if (extensions.empty())
        {
            VD_LOG_ERROR("Could not get extensions");
            return false;
        }
        createInfo.enabledExtensionCount = extensions.size();
        createInfo.ppEnabledExtensionNames = extensions.data();

        const std::vector<const char*>& validationLayers = GetValidationLayers();
        if (config.ValidationLayersEnabled)
        {
            if (validationLayers.empty())
            {
                VD_LOG_ERROR("Could not get validation layers");
                return false;
            }
            createInfo.enabledLayerCount = validationLayers.size();
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        VkAllocationCallbacks* allocator = nullptr;
        if (vkCreateInstance(&createInfo, allocator, &instance) != VK_SUCCESS)
        {
            VD_LOG_CRITICAL("Could not create Vulkan instance");
            return false;
        }
        VD_LOG_INFO("Created Vulkan instance");
        return true;
    }

    void VulkanContext::Terminate()
    {
        VkAllocationCallbacks* allocator = nullptr;
        vkDestroyInstance(instance, allocator);
        VD_LOG_INFO("Destroyed Vulkan instance");
    }

    std::vector<const char*> VulkanContext::GetExtensions() const
    {
        const std::vector<const char*>& requiredExtensions = GetRequiredExtensions();
        VD_LOG_DEBUG("Required extensions [{0}]", requiredExtensions.size());
        for (const char* extension : requiredExtensions) {
            VD_LOG_DEBUG(extension);
        }
        VD_LOG_DEBUG("");
        const std::vector<VkExtensionProperties>& availableExtensions = GetAvailableExtensions();
        VD_LOG_DEBUG("Available extensions [{0}]", availableExtensions.size());
        for (const VkExtensionProperties& extensionProperties : availableExtensions) {
            VD_LOG_DEBUG(extensionProperties.extensionName);
        }
        VD_LOG_DEBUG("");
        if (!HasExtensions(requiredExtensions, availableExtensions))
        {
            VD_LOG_ERROR("Could not find required extensions");
            return {};
        }
        return requiredExtensions;
    }

    std::vector<const char*> VulkanContext::GetRequiredExtensions() const
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        const std::vector<const char*>& extensions = std::vector(glfwExtensions, glfwExtensions + glfwExtensionCount);
        return extensions;
    }

    std::vector<VkExtensionProperties> VulkanContext::GetAvailableExtensions() const
    {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        return extensions;
    }

    bool VulkanContext::HasExtensions(const std::vector<const char*>& extensions, const std::vector<VkExtensionProperties>& availableExtensions) const
    {
        for (const char* extension : extensions)
        {
            bool extensionFound = false;
            for (const VkExtensionProperties& availableExtension : availableExtensions) {
                if (strcmp(extension, availableExtension.extensionName) == 0) {
                    extensionFound = true;
                    break;
                }
            }
            if (!extensionFound) {
                VD_LOG_WARN("Could not find extension [{0}]", extension);
                return false;
            }
        }
        return true;
    }

    std::vector<VkLayerProperties> VulkanContext::GetAvailableValidationLayers() const
    {
        uint32_t validationLayerCount;
        vkEnumerateInstanceLayerProperties(&validationLayerCount, nullptr);
        std::vector<VkLayerProperties> validationLayers(validationLayerCount);
        vkEnumerateInstanceLayerProperties(&validationLayerCount, validationLayers.data());
        return validationLayers;
    }

    bool VulkanContext::HasValidationLayers(const std::vector<const char*>& validationLayers, const std::vector<VkLayerProperties>& availableValidationLayers) const
    {
        for (const char* layerName: validationLayers)
        {
            bool layerFound = false;
            for (const auto& availableLayer: availableValidationLayers)
            {
                if (strcmp(layerName, availableLayer.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound)
            {
                VD_LOG_WARN("Could not find validation layer [{0}]", layerName);
                return false;
            }
        }
        return true;
    }

    std::vector<const char*> VulkanContext::GetValidationLayers() const
    {
        const std::vector<const char*>& requestedValidationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };
        VD_LOG_DEBUG("Requested validation layers [{0}]", requestedValidationLayers.size());
        for (const char* validationLayer : requestedValidationLayers) {
            VD_LOG_DEBUG(validationLayer);
        }
        VD_LOG_DEBUG("");
        const std::vector<VkLayerProperties>& availableValidationLayers = GetAvailableValidationLayers();
        VD_LOG_DEBUG("Available validation layers [{0}]", availableValidationLayers.size());
        for (const VkLayerProperties& layerProperties: availableValidationLayers)
        {
            VD_LOG_DEBUG(layerProperties.layerName);
        }
        VD_LOG_DEBUG("");
        if (!HasValidationLayers(requestedValidationLayers, availableValidationLayers))
        {
            VD_LOG_ERROR("Could not find requested validation layers");
            return {};
        }
        return requestedValidationLayers;
    }
}