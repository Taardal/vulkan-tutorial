#pragma once

#include "FileSystem.h"
#include "Window.h"
#include "GraphicsDevice.h"

namespace vulkandemo
{
    class App
    {
    public:
        struct Config
        {
            std::string Name;
            Window::Config Window;
            GraphicsDevice::Config Graphics;
        };

        App(const Config& config);

        ~App();

        void Run();

    private:
        void Initialize() const;

        void Terminate();

        Config config;
        FileSystem* fileSystem;
        Window* window;
        GraphicsDevice* graphicsDevice;
    };
}