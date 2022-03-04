#pragma once

#include "Context.h"
#include "Option.h"
#include <vector>

namespace VulkandemoCLI
{
    struct Command
    {
        std::string Name;
        std::string Usage;
        std::vector<std::string> Aliases;
        std::vector<Option> Options;
        std::function<void(const Context&)> Action;
    };
}