#include "App.h"
#include "Log.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <filesystem>

namespace vulkandemo
{
    App::App(const Config &config)
        : config(config),
          fileSystem(new FileSystem),
          window(new Window(config.Window)),
          graphicsDevice(new GraphicsDevice(config.Graphics))
    {
    }

    App::~App()
    {
        delete graphicsDevice;
        delete window;
        delete fileSystem;
    }

    void App::Run()
    {
        Initialize();
        while (!window->ShouldClose())
        {
            window->PollEvents();
        }
        Terminate();
    }


    void App::Initialize() const
    {
        Log::Initialize(config.Name);
        window->Initialize();
        graphicsDevice->Initialize();

        glm::mat4 matrix;
        glm::vec4 vec;
        auto test = matrix * vec;

        std::vector<char> vertexShaderByteCode = fileSystem->ReadBinaryFile("shaders/simple_shader.vert.spv");
        VD_INFO("Vertex Shader Code Size: [{0}]", vertexShaderByteCode.size());

        std::vector<char> fragmentShaderByteCode = fileSystem->ReadBinaryFile("shaders/simple_shader.frag.spv");
        VD_INFO("Fragment Shader Code Size: [{0}]", fragmentShaderByteCode.size());
    }

    void App::Terminate()
    {
        graphicsDevice->Terminate();
        window->Terminate();
    }

}