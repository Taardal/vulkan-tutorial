#pragma once

#include "Command.h"
#include <unordered_map>

namespace VulkandemoCLI
{
    class App
    {
    public:
        Command* GetCommand(int argc, char* argv[]) const;

        void PrintHelp() const;

        void PrintInput(int argc, char* argv[]) const;

    private:
        static const std::unordered_map<std::string, Command*> COMMANDS;
        static const char* HELP;
    };
}