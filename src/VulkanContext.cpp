#include "VulkanContext.h"
#include "Log.h"

#include <GLFW/glfw3.h>
#include <unordered_set>
#include <iostream>

namespace Vulkandemo
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) 
    {
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            VD_LOG_ERROR(pCallbackData->pMessage);
        }
        else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) 
        {
            VD_LOG_WARN(pCallbackData->pMessage);
        }
        else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        {
            VD_LOG_INFO(pCallbackData->pMessage);
        }
        else
        {
            VD_LOG_TRACE(pCallbackData->pMessage);
        }
        return VK_FALSE;
    }
}

namespace Vulkandemo
{
    const VkAllocationCallbacks* VulkanContext::ALLOCATOR = VK_NULL_HANDLE;

    VulkanContext::VulkanContext(const Config& config)
        : config(config), 
          instance(VK_NULL_HANDLE), 
          debugMessenger(VK_NULL_HANDLE)
    {
    }

    bool VulkanContext::Initialize()
    {
        bool vkInstanceCreated = CreateInstance();
        if (!vkInstanceCreated)
        {
            VD_LOG_ERROR("Could not create Vulkan instance");
            return false;
        }
        VD_LOG_INFO("Created Vulkan instance");
        if (config.ValidationLayersEnabled)
        {
            bool debugMessengerCreated = CreateDebugMessenger();
            if (!debugMessengerCreated)
            {
                VD_LOG_ERROR("Could not create debug messenger");
                return false;
            }
            VD_LOG_INFO("Created Vulkan debug messenger");
        }
        return true;
    }

    void VulkanContext::Terminate()
    {
        if (config.ValidationLayersEnabled)
        {
            DestoryDebugMessenger();
        }
        DestroyInstance();
    }

    bool VulkanContext::CreateInstance()
    {
        const std::vector<const char*>& extensions = GetExtensions();
        if (extensions.empty())
        {
            VD_LOG_ERROR("Could not get extensions");
            return false;
        }

        std::vector<const char*> validationLayers;
        if (config.ValidationLayersEnabled)
        {
            validationLayers = GetValidationLayers();
            if (validationLayers.empty())
            {
                VD_LOG_ERROR("Could not get validation layers");
                return false;
            }
        }

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
        createInfo.enabledExtensionCount = extensions.size();
        createInfo.ppEnabledExtensionNames = extensions.data();
        if (config.ValidationLayersEnabled)
        {
            createInfo.enabledLayerCount = validationLayers.size();
            createInfo.ppEnabledLayerNames = validationLayers.data();
            const VkDebugUtilsMessengerCreateInfoEXT& debugMessengerCreateInfo = GetDebugMessengerCreateInfo();
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugMessengerCreateInfo;
        }
        else
        {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        return vkCreateInstance(&createInfo, ALLOCATOR, &instance) == VK_SUCCESS;
    }

    void VulkanContext::DestroyInstance()
    {
        vkDestroyInstance(instance, ALLOCATOR);
        VD_LOG_INFO("Destroyed Vulkan instance");
    }

    bool VulkanContext::CreateDebugMessenger()
    {
        const VkDebugUtilsMessengerCreateInfoEXT& createInfo = GetDebugMessengerCreateInfo();
        const char* functionName = "vkCreateDebugUtilsMessengerEXT";
        auto function = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, functionName);
        if (function == nullptr) 
        {
            VD_LOG_ERROR("Could not look up address of extension function [{0}]", functionName);
            return false;
        }
        return function(instance, &createInfo, ALLOCATOR, &debugMessenger) == VK_SUCCESS;
    }

    void VulkanContext::DestoryDebugMessenger()
    {
        const char* functionName = "vkDestroyDebugUtilsMessengerEXT";
        auto function = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, functionName);
        if (function == nullptr) 
        {
            VD_LOG_WARN("Could not look up address of extension function [{0}]", functionName);
            return;
        }
        function(instance, debugMessenger, ALLOCATOR);
        VD_LOG_INFO("Destroyed Vulkan debug messenger");
    }

    std::vector<const char*> VulkanContext::GetExtensions() const
    {
        const std::vector<const char*>& requiredExtensions = GetRequiredExtensions();
        VD_LOG_DEBUG("Required extensions [{0}]", requiredExtensions.size());
        for (const char* extension : requiredExtensions) 
        {
            VD_LOG_DEBUG(extension);
        }
        const std::vector<VkExtensionProperties>& availableExtensions = GetAvailableExtensions();
        VD_LOG_DEBUG("Available extensions [{0}]", availableExtensions.size());
        for (const VkExtensionProperties& extensionProperties : availableExtensions) 
        {
            VD_LOG_DEBUG(extensionProperties.extensionName);
        }
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
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        if (config.ValidationLayersEnabled) 
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
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

    std::vector<const char*> VulkanContext::GetValidationLayers() const
    {
        const std::vector<const char*>& validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };
        VD_LOG_DEBUG("Requested validation layers [{0}]", validationLayers.size());
        for (const char* validationLayer : validationLayers) 
        {
            VD_LOG_DEBUG(validationLayer);
        }
        const std::vector<VkLayerProperties>& availableValidationLayers = GetAvailableValidationLayers();
        VD_LOG_DEBUG("Available validation layers [{0}]", availableValidationLayers.size());
        for (const VkLayerProperties& layerProperties: availableValidationLayers)
        {
            VD_LOG_DEBUG(layerProperties.layerName);
        }
        if (!HasValidationLayers(validationLayers, availableValidationLayers))
        {
            VD_LOG_ERROR("Could not find requested validation layers");
            return {};
        }
        return validationLayers;
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

    VkDebugUtilsMessengerCreateInfoEXT VulkanContext::GetDebugMessengerCreateInfo() const
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr;
        return createInfo;
    }
}