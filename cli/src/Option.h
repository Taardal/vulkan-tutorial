#pragma once

#include <string>
#include <vector>

namespace VulkandemoCLI
{
    struct Option
    {
        std::string_view Name;
        std::string_view Usage;
        std::string_view Value;
        std::vector<std::string_view> Aliases;
    };
}
