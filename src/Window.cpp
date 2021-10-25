#include "Window.h"
#include "Log.h"

namespace vulkandemo
{
    Window::Window(const Config& config)
        : config(config)
    {
    }

    void Window::Initialize()
    {
        bool glfwInitialized = glfwInit();
        if (!glfwInitialized)
        {
            VD_CRITICAL("Could not initialize GLFW");
            return;
        }
        VD_INFO("Initialized GLFW");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        GLFWmonitor* fullscreenMonitor = nullptr;
        GLFWwindow* sharedWindow = nullptr;
        glfwWindow = glfwCreateWindow(config.Width, config.Height, config.Title.c_str(), fullscreenMonitor, sharedWindow);
        if (!glfwWindow)
        {
            VD_CRITICAL("Could not create GLFW window");
            return;
        }
        VD_INFO("Created GLFW window");
    }

    void Window::Terminate()
    {
        glfwDestroyWindow(glfwWindow);
        VD_INFO("Destroyed GLFW window");
        glfwTerminate();
        VD_INFO("Terminated GLFW");
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(glfwWindow);
    }

    void Window::PollEvents() const
    {
        glfwPollEvents();
    }
}