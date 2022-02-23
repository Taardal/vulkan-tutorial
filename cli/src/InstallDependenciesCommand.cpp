#include "InstallDependenciesCommand.h"
#include <iostream>
#include <sstream>
#include <vector>

namespace VulkandemoCLI
{
    std::string InstallDependenciesCommand::Dependency::ToString()
    {
        std::stringstream ss;
        ss << Name << "=" << Version;
        return ss.str();
    }

    static InstallDependenciesCommand::Dependency FromString(const std::string& string)
    {
        int equalSignPosition = string.find("=");
        InstallDependenciesCommand::Dependency dependency;
        dependency.Name = string.substr(0, equalSignPosition);
        dependency.Version = string.substr(equalSignPosition + 1, string.length());
        return dependency;
    }
}

namespace VulkandemoCLI
{
    const char* InstallDependenciesCommand::NAME = "deps";
    const char* InstallDependenciesCommand::DESCRIPTION = "Install dependencies (git submodules)";
    const char* InstallDependenciesCommand::DEPENDENCIES_DIRECTORY_NAME = "lib";
    const char* InstallDependenciesCommand::DEPENDENCIES_FILENAME = ".dependencies";

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
        printf("Installing git submodules...\n");

        //printf("git submodule update --init\n");
        std::system("git submodule update --init");

        printf("Setting git submodule versions...\n");
        const std::vector<Dependency>& dependencies = ReadDependencies();
        for (Dependency dependency : dependencies)
        {
            std::stringstream ss;
            ss << "cd " << DEPENDENCIES_DIRECTORY << "/" << dependency.Name << " && git checkout " << dependency.Version;
            std::string command = ss.str();
            std::system(command.c_str());
        }
        /*
        FileSystem fileSystem;
        std::vector<Env> envs;
        std::istringstream iss(fileSystem.ReadFile("lib/.versions"));
        for (std::string line; std::getline(iss, line);)
        {
            Env env = Env::FromString(line);
            printf("-- Using [%s] version [%s]\n", env.Key.c_str(), env.Value.c_str());
            std::stringstream ss;
            ss << "cd " << DEPENDENCIES_DIRECTORY << "/" << env.Key.c_str() << " && git checkout " << env.Value.c_str();
            std::string command = ss.str();
            std::system(command.c_str());
        }
        */
    }

    std::vector<InstallDependenciesCommand::Dependency> InstallDependenciesCommand::ReadDependencies() const
    {
        std::vector<Dependency> dependencies;
        FileSystem fileSystem;
        std::istringstream iss(fileSystem.ReadFile(DEPENDENCIES_FILENAME));
        for (std::string line; std::getline(iss, line);)
        {
            //dependencies.push_back(Dependency::FromString(line));
            int equalSignPosition = line.find("=");
            Dependency dependency;
            dependency.Name = line.substr(0, equalSignPosition);
            dependency.Version = line.substr(equalSignPosition + 1, line.length());
            dependencies.push_back(dependency);
        }
        return dependencies;
    }

    void InstallDependenciesCommand::InstallGitSubmodules() const
    {
        printf("git submodule update --init\n");
        std::system("git submodule update --init");
    }

    void InstallDependenciesCommand::SetGitSubmoduleVersion(const char* directory, const char* version) const
    {
        printf("-- Using [%s] version [%s]\n", directory, version);
        std::stringstream ss;
        ss << "cd " << DEPENDENCIES_DIRECTORY << "/" << directory << " && git checkout " << version;
        std::string command = ss.str();
        std::system(command.c_str());
    }
}
