#pragma once

#include <string>

namespace VulkandemoCLI
{
    class FileSystem
    {
    public:
        std::string ReadFile(const char* path) const;
    };
}