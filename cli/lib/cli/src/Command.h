#pragma once

#include "Context.h"
#include "Option.h"
#include <string>
#include <vector>

namespace CLI
{
    struct Command
    {
        std::string_view Name;
        std::string_view Usage;
        std::vector<std::string_view> Aliases;
        std::vector<Option> Options;
        std::function<void(const Context&)> Action;

        bool HasOption(std::string_view name) const;

        const Option* GetOption(std::string_view name) const;
    };
}