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

        std::vector<std::string> GetArguments(int argc, char* argv[]) const;

        Flag GetFlag(const std::string& argument, const Context& context) const;

        Command GetCommand(const std::string& argument) const;

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