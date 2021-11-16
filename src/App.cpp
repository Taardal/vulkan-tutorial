#include "App.h"
#include "Log.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Vulkandemo
{
    App::App(const Config &config)
        : config(config),
          fileSystem(new FileSystem),
          window(new Window(config.Window)),
          vulkanContext(new VulkanContext(config.Vulkan))
    {
    }

    App::~App()
    {
        delete vulkanContext;
        delete window;
        delete fileSystem;
    }

    void App::Run()
    {
        if (!Initialize())
        {
            VD_LOG_CRITICAL("Could not initialize app");
            return;
        }
        VD_LOG_DEBUG("Running...");
        while (!window->ShouldClose())
        {
            window->PollEvents();
        }
        Terminate();
    }


    bool App::Initialize() const
    {
        Log::Initialize(config.Name);
        VD_LOG_DEBUG("Initializing...");
        if (!window->Initialize())
        {
            VD_LOG_ERROR("Could not initialize window");
            return false;
        }
        if (!vulkanContext->Initialize())
        {
            VD_LOG_ERROR("Could not initialize Vulkan");
            return false;
        }

        glm::mat4 matrix;
        glm::vec4 vec;
        auto test = matrix * vec;

        const char* path = "shaders/simple_shader.vert.spv";
        std::vector<char> vertexShaderByteCode = fileSystem->ReadBinaryFile(path);
        VD_LOG_INFO("Vertex Shader Code Size: [{0}]", vertexShaderByteCode.size());

        std::vector<char> fragmentShaderByteCode = fileSystem->ReadBinaryFile("shaders/simple_shader.frag.spv");
        VD_LOG_INFO("Fragment Shader Code Size: [{0}]", fragmentShaderByteCode.size());

        return true;
    }

    void App::Terminate()
    {
        VD_LOG_DEBUG("Terminating...");
        vulkanContext->Terminate();
        window->Terminate();
    }

}