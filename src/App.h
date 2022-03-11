#pragma once

#include "FileSystem.h"
#include "Window.h"
#include "Vulkan.h"
#include "VulkanPhysicalDevice.h"

namespace Vulkandemo {

    class App {
    public:
        struct Config {
            std::string Name;
            Window::Config Window;
            Vulkan::Config Vulkan;
        };

    private:
        Config config;
        FileSystem* fileSystem;
        Window* window;
        Vulkan* vulkan;
        VulkanPhysicalDevice* vulkanPhysicalDevice;

    public:
        explicit App(Config config);

        ~App();

        void run();

    private:
        bool initialize() const;

        void terminate();
    };

}