#pragma once

#include <string>
#include <vector>

namespace VulkandemoCLI
{
    struct Flag
    {
        std::string Name;
        std::string Usage;
        std::string Value;
        std::vector<std::string> Aliases;

        std::string NameWithPrefix() const;
    };
}