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

        void Run(int argc, char* argv[]) const;

    private:
        std::vector<std::string> GetArguments(int argc, char* argv[]) const;

        Flag GetFlag(const std::string& argument, const std::string& nextArgument) const;



        std::vector<Flag> GetFlags(const std::vector<std::string>& arguments) const;

        Command GetCommand(const std::vector<std::string>& arguments) const;

        std::vector<Option> GetOptions(const std::vector<std::string>& arguments) const;

        Option GetOption(const std::string& argument, const std::string& nextArgument, int dashCount) const;

        void PrintHelp() const;

        void PrintInput(int argc, char* argv[]) const;

    public:
        std::string Name;
        std::string Usage;
        std::vector<Command> Commands;
        std::vector<Option> Options;
        std::vector<Flag> Flags;

    private:
        static const char* DEFAULT_EXE_NAME;
        Command helpCommand;
        Flag helpFlag;
    };
}