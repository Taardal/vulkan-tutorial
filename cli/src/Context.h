#pragma once

#include <vector>

namespace VulkandemoCLI
{
    class App;
    class Command;
    class Flag;

    struct Context
    {
        const App* App;
        const Command* Command = nullptr;
        std::vector<Flag> Flags;
        std::vector<std::string> Arguments;

        std::string ToString() const;

        bool HasFlag(const std::string& name) const;

        Flag GetFlag(const std::string& name) const;
    };
}