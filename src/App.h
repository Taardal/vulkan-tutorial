#pragma once

#include "FileSystem.h"
#include "Window.h"
#include "Vulkan.h"

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
        Vulkan* vulkanContext;

    public:
        explicit App(const Config& config);

        ~App();

        void run();

    private:
        bool initialize() const;

        void terminate();
    };

}