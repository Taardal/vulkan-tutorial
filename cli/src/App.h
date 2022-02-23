#pragma once

#include "Command.h"
#include <map>

namespace VulkandemoCLI
{
    class App
    {
    public:
        App();

        ~App();

        Command* GetCommand(int argc, char* argv[]) const;

        std::vector<Option> GetOptions(int argc, char* argv[]) const;

    private:
        Option GetOption(const std::string& argument, const std::string& nextArgument, int dashCount) const;

        void PrintHelp() const;

        void PrintInput(int argc, char* argv[]) const;

        static const char* DEFAULT_EXE_NAME;
        std::map<std::string, Command*> commands;
    };
}