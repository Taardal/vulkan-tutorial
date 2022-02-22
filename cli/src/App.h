#pragma once

#include "Command.h"
#include <unordered_map>

namespace VulkandemoCLI
{
    void PrintHelp();

    void PrintInput(int argc, char* argv[]);

    Command* ParseCommand(int argc, char* argv[]);

    void ExecuteCommand(Command* command);

    class App
    {
    public:
        Command* GetCommand(int argc, char* argv[]) const;

    private:

    };
}