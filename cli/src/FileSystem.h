#pragma once

#include <string>

namespace VulkandemoCLI
{
    class FileSystem
    {
    public:
        std::string ReadFile(std::string_view) const;
    };
}