#pragma once

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
        std::vector<std::string> Arguments;

        bool HasOption(const std::string& name) const;

        Option GetOption(const std::string& name) const;

        std::string ToString() const;
    };
}