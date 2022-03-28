#include "App.h"
#include "Log.h"

namespace Vulkandemo {

    App::App(Config config)
            : config(std::move(config)),
              fileSystem(new FileSystem),
              window(new Window(config.Window)),
              vulkan(new Vulkan(config.Vulkan, window)),
              vulkanPhysicalDevice(new VulkanPhysicalDevice(vulkan)),
              vulkanDevice(new VulkanDevice(vulkan, vulkanPhysicalDevice)),
              vulkanSwapChain(new VulkanSwapChain(vulkanDevice, vulkanPhysicalDevice, vulkan, window)),
              vertexShader(new VulkanShader(vulkanDevice)),
              fragmentShader(new VulkanShader(vulkanDevice)),
              vulkanRenderPass(new VulkanRenderPass(vulkanSwapChain, vulkanDevice)),
              vulkanGraphicsPipeline(new VulkanGraphicsPipeline(vulkanRenderPass, vulkanSwapChain, vulkanDevice)) {
    }

    App::~App() {
        delete vulkanGraphicsPipeline;
        delete vulkanRenderPass;
        delete fragmentShader;
        delete vertexShader;
        delete vulkanSwapChain;
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
        Log::initialize(config.Name, config.LogLevel);
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
        if (!vulkanSwapChain->initialize()) {
            VD_LOG_ERROR("Could not initialize Vulkan swap chain");
            return false;
        }

        std::vector<char> vertexShaderBytes = fileSystem->readBytes("shaders/simple_shader.vert.spv");
        VD_LOG_INFO("Vertex shader byte size: [{0}]", vertexShaderBytes.size());
        if (!vertexShader->initialize(vertexShaderBytes)) {
            VD_LOG_ERROR("Could not initialize vertex shader");
            return false;
        }
        std::vector<char> fragmentShaderBytes = fileSystem->readBytes("shaders/simple_shader.frag.spv");
        VD_LOG_INFO("Fragment shader byte size: [{0}]", fragmentShaderBytes.size());
        if (!fragmentShader->initialize(fragmentShaderBytes)) {
            VD_LOG_ERROR("Could not initialize fragment shader");
            return false;
        }

        if (!vulkanRenderPass->initialize()) {
            VD_LOG_ERROR("Could not initialize Vulkan render pass");
            return false;
        }
        if (!vulkanGraphicsPipeline->initialize(*vertexShader, *fragmentShader)) {
            VD_LOG_ERROR("Could not initialize Vulkan graphics pipeline");
            return false;
        }

        return true;
    }

    void App::terminate() {
        VD_LOG_DEBUG("Terminating...");
        vulkanGraphicsPipeline->terminate();
        vulkanRenderPass->terminate();
        fragmentShader->terminate();
        vertexShader->terminate();
        vulkanSwapChain->terminate();
        vulkanDevice->terminate();
        vulkan->terminate();
        window->terminate();
    }

}