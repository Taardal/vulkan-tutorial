#include "App.h"
#include "Command.h"
#include "Environment.h"

using namespace VulkandemoCLI;

int main(int argc, char* argv[])
{
#ifdef VD_DEBUG
    PrintInput(argc, argv);
#endif
    Command* command = ParseCommand(argc, argv);
    if (command != nullptr)
    {
        command->Execute();
    }
    else
    {
        PrintHelp();
    }
}