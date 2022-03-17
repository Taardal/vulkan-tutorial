#include "App.h"
#include "Log.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Vulkandemo {

    App::App(Config config)
            : config(std::move(config)),
              fileSystem(new FileSystem),
              window(new Window(config.Window)),
              vulkan(new Vulkan(config.Vulkan)),
              vulkanPhysicalDevice(new VulkanPhysicalDevice(vulkan)),
              vulkanDevice(new VulkanDevice(vulkan, vulkanPhysicalDevice)){
    }

    App::~App() {
        delete vulkanDevice;
        delete vulkanPhysicalDevice;
        delete vulkan;
        delete window;
        delete fileSystem;
    }

    void App::run() {
        if (!initialize()) {
            VD_LOG_CRITICAL("Could not initialize app");
            return;
        }
        VD_LOG_DEBUG("Running...");
        while (!window->shouldClose()) {
            window->pollEvents();
        }
        terminate();
    }


    bool App::initialize() const {
        Log::initialize(config.Name);
        VD_LOG_DEBUG("Initializing...");
        if (!window->initialize()) {
            VD_LOG_ERROR("Could not initialize window");
            return false;
        }
        if (!vulkan->initialize()) {
            VD_LOG_ERROR("Could not initialize Vulkan");
            return false;
        }
        if (!vulkanPhysicalDevice->initialize()) {
            VD_LOG_ERROR("Could not initialize Vulkan physical device");
            return false;
        }
        if (!vulkanDevice->initialize()) {
            VD_LOG_ERROR("Could not initialize Vulkan device");
            return false;
        }

        glm::mat4 matrix;
        glm::vec4 vec;
        auto test = matrix * vec;

        const char* path = "shaders/simple_shader.vert.spv";
        std::vector<char> vertexShaderByteCode = fileSystem->readBinaryFile(path);
        VD_LOG_INFO("Vertex Shader Code Size: [{0}]", vertexShaderByteCode.size());

        std::vector<char> fragmentShaderByteCode = fileSystem->readBinaryFile("shaders/simple_shader.frag.spv");
        VD_LOG_INFO("Fragment Shader Code Size: [{0}]", fragmentShaderByteCode.size());

        return true;
    }

    void App::terminate() {
        VD_LOG_DEBUG("Terminating...");
        vulkanDevice->terminate();
        vulkan->terminate();
        window->terminate();
    }

}