#include "AddLibraryCommand.h"
#include "BuildProjectCommand.h"
#include "CompileShadersCommand.h"
#include "FileSystem.h"
#include "InstallDependenciesCommand.h"
#include "InstallGLFWCommand.h"
#include "RunProjectCommand.h"
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
            VulkandemoCLI::createBuildProjectCommand(),
            VulkandemoCLI::createInstallGLFWCommand(),
            VulkandemoCLI::createAddLibraryCommand(fileSystem),
            VulkandemoCLI::createInstallLibrariesCommand(fileSystem),
            VulkandemoCLI::createRunProjectCommand(),
            VulkandemoCLI::createCompileShadersCommand(),
    };

    app.Run(argc, argv);
    return 0;
}