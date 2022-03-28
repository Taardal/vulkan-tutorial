#include "Window.h"

#include <utility>
#include "Log.h"

namespace Vulkandemo {

    Window::Window(Config config) : config(std::move(config)), glfwWindow(nullptr) {
    }

    GLFWwindow* Window::getGlfwWindow() const {
        return glfwWindow;
    }

    Size Window::getSizeInPixels() const {
        int width;
        int height;
        glfwGetFramebufferSize(glfwWindow, &width, &height);
        return { width, height };
    }

    Size Window::getSizeInScreenCoordinates() const {
        int width;
        int height;
        glfwGetWindowSize(glfwWindow, &width, &height);
        return { width, height };
    }

    bool Window::initialize() {
        bool glfwInitialized = glfwInit();
        if (!glfwInitialized) {
            VD_LOG_CRITICAL("Could not initialize GLFW");
            return false;
        }
        VD_LOG_INFO("Initialized GLFW");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        GLFWmonitor* fullscreenMonitor = nullptr;
        GLFWwindow* sharedWindow = nullptr;
        glfwWindow = glfwCreateWindow((int32_t) config.Width, (int32_t) config.Height, config.Title.c_str(), fullscreenMonitor, sharedWindow);
        if (!glfwWindow) {
            VD_LOG_CRITICAL("Could not create GLFW window");
            return false;
        }
        VD_LOG_INFO("Created GLFW window");
        return true;
    }

    void Window::terminate() {
        glfwDestroyWindow(glfwWindow);
        VD_LOG_INFO("Destroyed GLFW window");
        glfwTerminate();
        VD_LOG_INFO("Terminated GLFW");
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(glfwWindow);
    }

    void Window::pollEvents() const {
        glfwPollEvents();
    }
}