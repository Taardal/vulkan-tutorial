#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace Vulkandemo
{
    class Window
    {
    public:
        struct Config
        {
            std::string Title;
            uint32_t Width;
            uint32_t Height;
        };

        Window(const Config& config);

        bool Initialize();

        void Terminate();

        bool ShouldClose() const;

        void PollEvents() const;

    private:
        Config config;
        GLFWwindow* glfwWindow;
    };
}