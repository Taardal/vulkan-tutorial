#include "Environment.h"
#include "BuildProjectCommand.h"
#include "InstallDependenciesCommand.h"
#include "InstallGLFWCommand.h"
#include <iostream>
#include <unordered_map>

using namespace VulkandemoCLI;

std::unordered_map<std::string, Command*> commands = {
        {"build", new BuildProjectCommand()},
        {"deps", new InstallDependenciesCommand()},
        {"glfw", new InstallGLFWCommand()}
};

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

int main(int argc, char* argv[])
{
#ifdef VD_DEBUG
    PrintInput(argc, argv);
#endif
    if (argc < 2)
    {
        PrintHelp();
        return 0;
    }
    char* command = argv[1];
    if (strlen(command) == 0)
    {
        PrintHelp();
        return 0;
    }
    const auto& iterator = commands.find(command);
    if (iterator == commands.end())
    {
        PrintHelp();
        return 0;
    }
    Command* value = iterator->second;
    value->Execute();
}