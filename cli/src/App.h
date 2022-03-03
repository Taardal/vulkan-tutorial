#pragma once

#include "Command.h"
#include "FileSystem.h"
#include "Flag.h"
#include <map>

namespace VulkandemoCLI
{
    struct App
    {
    public:
        App();

        void Run(int argc, char* argv[]);

    private:
        void Initialize();

        Flag GetFlag(const std::string& segment, const Command* command) const;

        const Flag* FindFlag(const std::string& name, const std::vector<Flag>& flags) const;

        const Command* FindCommand(const std::string& segment) const;

    public:
        std::string Name;
        std::string Usage;
        std::function<void(const Context& context)> Action;
        std::vector<Command> Commands;
        std::vector<Flag> Flags;

    private:
        Command helpCommand;
        Flag helpFlag;
    };
}