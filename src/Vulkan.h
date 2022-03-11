#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace Vulkandemo {

    class Vulkan {
    public:
        struct Config {
            std::string Name;
            uint32_t MajorVersion = 1;
            uint32_t MinorVersion = 0;
            uint32_t PatchVersion = 0;
            bool ValidationLayersEnabled = false;
        };

    private:
        static const VkAllocationCallbacks* ALLOCATOR;

    private:
        Config config;
        VkInstance vkInstance;
        VkDebugUtilsMessengerEXT vkDebugMessenger;

    public:
        explicit Vulkan(const Config& config);

        bool initialize();

        void terminate();

    private:
        bool createVkInstance();

        void destroyVkInstance();

        bool createDebugMessenger();

        void destroyDebugMessenger();

        std::vector<const char*> getExtensions() const;

        std::vector<const char*> getRequiredExtensions() const;

        std::vector<VkExtensionProperties> getAvailableExtensions() const;

        bool hasExtensions(const std::vector<const char*>& extensions, const std::vector<VkExtensionProperties>& availableExtensions) const;

        std::vector<const char*> getValidationLayers() const;

        std::vector<VkLayerProperties> getAvailableValidationLayers() const;

        bool hasValidationLayers(const std::vector<const char*>& validationLayers, const std::vector<VkLayerProperties>& availableValidationLayers) const;

        VkDebugUtilsMessengerCreateInfoEXT getDebugMessengerCreateInfo() const;
    };
}