#include "GraphicsDevice.h"
#include "Log.h"

#include <GLFW/glfw3.h>
#include <unordered_set>

namespace vulkandemo
{
    GraphicsDevice::GraphicsDevice(const Config& config)
        : config(config)
    {
    }

    void GraphicsDevice::Initialize()
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = config.Name.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(config.MajorVersion, config.MinorVersion, config.PatchVersion);
        appInfo.pEngineName = config.Name.c_str();
        appInfo.engineVersion = VK_MAKE_VERSION(config.MajorVersion, config.MinorVersion, config.PatchVersion);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        std::unordered_set<const char*> extensionNames;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        
        VD_DEBUG("Supported extensions [{0}]", extensionCount);
        for (const auto& extension : extensions) 
        {
            VD_DEBUG(extension.extensionName);
            extensionNames.insert(extension.extensionName);
        }

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        
        VD_DEBUG("Required GLFW extensions");
        for (uint32_t i = 0; i < glfwExtensionCount; i++)
        {
            const char* glfwExtension = glfwExtensions[i];
            VD_DEBUG(glfwExtension);
            if (extensionNames.find(glfwExtension) == extensionNames.end())
            {
                VD_WARN("Could not find required GLFW extension [{0}]", glfwExtension);
            }
        }

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledLayerCount = 0;
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        VkAllocationCallbacks* allocator = nullptr;
        VkResult result = vkCreateInstance(&createInfo, allocator, &instance);
        if (result != VK_SUCCESS)
        {
            VD_CRITICAL("Could not create Vulkan instance");
            return;
        }
        VD_INFO("Created Vulkan instance");
    }

    void GraphicsDevice::Terminate()
    {
        VkAllocationCallbacks* allocator = nullptr;
        vkDestroyInstance(instance, allocator);
        VD_INFO("Destroyed Vulkan instance");
    }
}