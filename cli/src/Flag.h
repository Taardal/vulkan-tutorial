#pragma once

#include <string>
#include <vector>

namespace VulkandemoCLI
{
    struct Flag
    {
        std::string Name;
        std::vector<std::string> Aliases;
        std::string Value;
    };
}