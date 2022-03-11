#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <string>

namespace Vulkandemo {

    class Window {
    public:
        struct Config {
            std::string Title;
            uint32_t Width;
            uint32_t Height;
        };

    private:
        Config config;
        GLFWwindow* glfwWindow;

    public:
        explicit Window(const Config& config);

        bool initialize();

        void terminate();

        bool shouldClose() const;

        void pollEvents() const;
    };
}