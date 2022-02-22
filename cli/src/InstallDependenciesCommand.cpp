#include "InstallDependenciesCommand.h"
#include <iostream>
#include <sstream>

namespace VulkandemoCLI
{
    const char* InstallDependenciesCommand::NAME = "deps";
    const char* InstallDependenciesCommand::LIB_FOLDER = "lib";

    const char* InstallDependenciesCommand::GetName() const
    {
        return NAME;
    }

    const char* InstallDependenciesCommand::GetDescription() const
    {
        return "Install dependencies (git submodules)";
    }

    void InstallDependenciesCommand::Execute() const
    {
        printf("Installing git submodules...\n");
        InstallGitSubmodules();
        printf("Setting git submodule versions...\n");
        SetGitSubmoduleVersion("glfw", "3.3.4");
        SetGitSubmoduleVersion("glm", "0.9.9.8");
        SetGitSubmoduleVersion("spdlog", "v1.9.2");
    }

    void InstallDependenciesCommand::InstallGitSubmodules() const
    {
        printf("git submodule update --init\n");
        //std::system("git submodule update --init");
    }

    void InstallDependenciesCommand::SetGitSubmoduleVersion(const char* folder, const char* version) const
    {
        printf("-- Using [%s] version [%s]\n", folder, version);
        std::stringstream ss;
        ss << "cd " << LIB_FOLDER << "/" << folder << " && git checkout " << version;
        std::string command = ss.str();
        //std::system(command.c_str());
    }
}
