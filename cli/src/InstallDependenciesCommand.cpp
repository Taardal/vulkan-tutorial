#include "InstallDependenciesCommand.h"
#include <iostream>
#include <sstream>
#include <vector>

namespace VulkandemoCLI
{
    const char* InstallDependenciesCommand::NAME = "deps";
    const char* InstallDependenciesCommand::DESCRIPTION = "Install dependencies (git submodules)";
    const char* InstallDependenciesCommand::DEPENDENCIES_FILENAME = ".dependencies";
    const char* InstallDependenciesCommand::DEPENDENCIES_DIRECTORY_NAME = "lib";

    const char* InstallDependenciesCommand::GetName() const
    {
        return NAME;
    }

    const char* InstallDependenciesCommand::GetDescription() const
    {
        return DESCRIPTION;
    }

    void InstallDependenciesCommand::Execute() const
    {
        printf("%s\n", "#######################################");
        printf("%s\n", "#  Installing git submodules...       #");
        printf("%s\n", "#######################################");
        printf("\n");

        printf("%s\n", "git submodule update --init");
        //std::system("git submodule update --init");

        printf("%s\n", "#######################################");
        printf("%s\n", "#  Setting git submodule versions...  #");
        printf("%s\n", "#######################################");
        printf("\n");

        FileSystem fileSystem;
        std::istringstream iss(fileSystem.ReadFile(DEPENDENCIES_FILENAME));
        for (std::string line; std::getline(iss, line);)
        {
            if (line.empty() || line[0] == '#')
            {
                continue;
            }
            int equalSignPosition = line.find("=");
            std::string name = line.substr(0, equalSignPosition);
            std::string version = line.substr(equalSignPosition + 1, line.length());
            printf("%s%s%s%s%s\n", "-- Using [", name.c_str(), "] version [", version.c_str(), "]");
            std::stringstream ss;
            ss << "cd " << DEPENDENCIES_DIRECTORY_NAME << "/" << name << " && git checkout " << version;
            std::string command = ss.str();
            //std::system(command.c_str());
        }
    }
}
