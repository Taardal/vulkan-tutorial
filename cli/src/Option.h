#pragma once

#include <string>
#include <vector>

namespace VulkandemoCLI
{
    struct Option
    {
        std::string Name;
        std::string Usage;
        std::string Value;
        std::vector<std::string> Aliases;
    };
}
