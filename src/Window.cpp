#include "Window.h"
#include "Log.h"

namespace Vulkandemo
{
    Window::Window(const Config& config)
        : config(config)
    {
    }

    bool Window::Initialize()
    {
        bool glfwInitialized = glfwInit();
        if (!glfwInitialized)
        {
            VD_LOG_CRITICAL("Could not initialize GLFW");
            return false;
        }
        VD_LOG_INFO("Initialized GLFW");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        GLFWmonitor* fullscreenMonitor = nullptr;
        GLFWwindow* sharedWindow = nullptr;
        glfwWindow = glfwCreateWindow((int32_t) config.Width, (int32_t) config.Height, config.Title.c_str(), fullscreenMonitor, sharedWindow);
        if (!glfwWindow)
        {
            VD_LOG_CRITICAL("Could not create GLFW window");
            return false;
        }
        VD_LOG_INFO("Created GLFW window");
        return true;
    }

    void Window::Terminate()
    {
        glfwDestroyWindow(glfwWindow);
        VD_LOG_INFO("Destroyed GLFW window");
        glfwTerminate();
        VD_LOG_INFO("Terminated GLFW");
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