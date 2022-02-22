#include "App.h"
#include "BuildProjectCommand.h"
#include "InstallDependenciesCommand.h"
#include "InstallGLFWCommand.h"
#include <iostream>
#include <unordered_map>

namespace VulkandemoCLI
{
    const std::unordered_map<std::string, Command*> App::COMMANDS = {
        {"build", new BuildProjectCommand()},
        {"deps", new InstallDependenciesCommand()},
        {"glfw", new InstallGLFWCommand()}
    };

    const char* App::HELP = R"(
Usage: %s [options] [command]

Options:
    -h, --help          Output usage information

Commands:
    build               Build project
    deps                Install dependencies
    glfw                Build and install GLFW
)";

    Command* App::GetCommand(int argc, char* argv[]) const
    {
        bool noCommand = argc < 2;
        if (noCommand)
        {
            return nullptr;
        }
        const char* command = argv[1];
        const auto& iterator = COMMANDS.find(command);
        if (iterator == COMMANDS.end())
        {
            return nullptr;
        }
        return iterator->second;
    }

    void App::PrintHelp() const
    {
#ifdef VD_EXE_NAME
        const char* exeName = VD_EXE_NAME;
#else
        const char* exeName = "vd";
#endif
        printf(HELP, exeName);
        printf("\n");
    }

    void App::PrintInput(int argc, char* argv[]) const
    {
        for (int i = 0; i < argc; i++)
        {
            printf("argc [%i/%i], argv [%s]\n", (i + 1), argc, argv[i]);
        }
    }
}
