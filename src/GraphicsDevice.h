#pragma once

#include <vulkan/vulkan.h>
#include <string>

namespace vulkandemo
{
    class GraphicsDevice
    {
    public:
        struct Config
        {
            std::string Name;
            uint32_t MajorVersion = 1;
            uint32_t MinorVersion = 0;
            uint32_t PatchVersion = 0;
        };

        GraphicsDevice(const Config& config);

        void Initialize();

        void Terminate();

    private:
        Config config;
        VkInstance instance;
    };
}