#pragma once

#include <string>
#include <vector>

namespace VulkandemoCLI
{
    class App;
    class Command;
    class Option;

    struct Context
    {
        const App* App;
        const Command* Command = nullptr;
        std::vector<Option> Options;
        std::vector<std::string_view> Arguments;

        bool HasOption(std::string_view name) const;

        Option GetOption(std::string_view name) const;

        std::string ToString() const;
    };
}