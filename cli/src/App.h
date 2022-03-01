#pragma once

#include "Command.h"
#include "FileSystem.h"
#include <map>

namespace VulkandemoCLI
{
    struct App
    {
    public:
        App();

        void Run(int argc, char* argv[]) const;

    private:
        Command GetCommand(int argc, char* argv[]) const;

        std::vector<Option> GetOptions(int argc, char* argv[]) const;

        Option GetOption(const std::string& argument, const std::string& nextArgument, int dashCount) const;

        void PrintHelp() const;

        void PrintInput(int argc, char* argv[]) const;

    public:
        std::string Name;
        std::string Usage;
        std::vector<Command> Commands;

    private:
        static const char* DEFAULT_EXE_NAME;
        Command helpCommand;
    };
}