#include "App.h"
#include "Environment.h"
#include "BuildProjectCommand.h"
#include "FileSystem.h"
#include "InstallDependenciesCommand.h"
#include "InstallGLFWCommand.h"

using namespace VulkandemoCLI;

int main(int argc, char* argv[]) {
#ifdef VDC_EXE_NAME
    const char* appName = VDC_EXE_NAME;
#else
    const char* appName = "vd";
#endif

    FileSystem fileSystem;

    App app;
    app.Name = appName;
    app.Usage = "Vulkandemo CLI";
    app.Commands = {
        CreateBuildProjectCommand(),
        CreateInstallDependenciesCommand(fileSystem),
        CreateInstallGLFWCommand()
    };
    app.Run(argc, argv);

    return 0;
}