#include "BuildProjectCommand.h"
#include "FileSystem.h"
#include "InstallDependenciesCommand.h"
#include "InstallGLFWCommand.h"
#include <cli.h>

int main(int argc, char* argv[]) {
#ifdef VDC_EXE_NAME
    const char* appName = VDC_EXE_NAME;
#else
    const char* appName = "vd";
#endif

    VulkandemoCLI::FileSystem fileSystem;

    CLI::App app;
    app.Name = appName;
    app.Usage = "Vulkandemo CLI";
    app.Commands = {
        VulkandemoCLI::CreateBuildProjectCommand(),
        VulkandemoCLI::CreateInstallDependenciesCommand(fileSystem),
        VulkandemoCLI::CreateInstallGLFWCommand()
    };

    app.Run(argc, argv);
    return 0;
}