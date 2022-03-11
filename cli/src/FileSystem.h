#pragma once

#include <string>

namespace VulkandemoCLI {

    class FileSystem {
    public:
        std::string readFile(std::string_view path) const;
    };
}