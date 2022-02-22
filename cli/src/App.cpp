#include "App.h"
#include "BuildProjectCommand.h"
#include "InstallDependenciesCommand.h"
#include "InstallGLFWCommand.h"
#include <iostream>
#include <unordered_map>

namespace VulkandemoCLI
{
    void PrintHelp()
    {
#ifdef VD_EXE_NAME
        const char* exeName = VD_EXE_NAME;
#else
        const char* exeName = "vd";
#endif
        std::cout << "Usage: " << exeName << " [options] [command]" << std::endl;
        std::cout << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "  -h, --help          Output usage information" << std::endl;
        std::cout << std::endl;
        std::cout << "Commands:" << std::endl;
        std::cout << "  build               Build project" << std::endl;
        std::cout << "  deps                Install dependencies" << std::endl;
        std::cout << "  glfw                Build and install glfw" << std::endl;
        std::cout << std::endl;
    }

    void PrintInput(int argc, char* argv[])
    {
        for (int i = 0; i < argc; i++)
        {
            printf("argc [%i/%i], argv [%s]\n", (i + 1), argc, argv[i]);
        }
    }

    Command* ParseCommand(int argc, char* argv[])
    {
        if (argc < 2)
        {
            return nullptr;
        }
        char* command = argv[1];
        if (strlen(command) == 0)
        {
            return nullptr;
        }
        std::unordered_map<std::string, Command*> commands = {
                {"build", new BuildProjectCommand()},
                {"deps", new InstallDependenciesCommand()},
                {"glfw", new InstallGLFWCommand()}
        };
        const auto& iterator = commands.find(command);
        if (iterator == commands.end())
        {
            return nullptr;
        }
        return iterator->second;
    }

    void ExecuteCommand(Command* command)
    {

    }

    Command* App::GetCommand(int argc, char* argv[]) const
    {
        if (argc < 2)
        {
            return nullptr;
        }
        char* command = argv[1];
        if (strlen(command) == 0)
        {
            return nullptr;
        }
        std::unordered_map<std::string, Command*> commands = {
                {"build", new BuildProjectCommand()},
                {"deps", new InstallDependenciesCommand()},
                {"glfw", new InstallGLFWCommand()}
        };
        const auto& iterator = commands.find(command);
        if (iterator == commands.end())
        {
            return nullptr;
        }
        return iterator->second;
    }
}
