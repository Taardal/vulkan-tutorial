#include "App.h"
#include "BuildProjectCommand.h"
#include "FileSystem.h"
#include "InstallDependenciesCommand.h"
#include "InstallGLFWCommand.h"

using namespace VulkandemoCLI;

int main(int argc, char* argv[]) {
    FileSystem fileSystem;

    App app;
    app.Name = "Vulkandemo CLI";
    app.Usage = "Support multiple platforms";
    app.Commands = {
            CreateBuildProjectCommand(),
            CreateInstallDependenciesCommand(fileSystem),
            CreateInstallGLFWCommand()
    };
    app.Run(argc, argv);

    return 0;
}