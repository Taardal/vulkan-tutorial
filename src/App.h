#pragma once

#include "FileSystem.h"
#include "Window.h"
#include "VulkanContext.h"

namespace Vulkandemo
{
    class App
    {
    public:
        struct Config
        {
            std::string Name;
            Window::Config Window;
            VulkanContext::Config Vulkan;
        };

        explicit App(const Config& config);

        ~App();

        void Run();

    private:
        bool Initialize() const;

        void Terminate();

        Config config;
        FileSystem* fileSystem;
        Window* window;
        VulkanContext* vulkanContext;
    };
}