#pragma once

#include "Command.h"
#include "FileSystem.h"
#include <string>
#include <sstream>

namespace VulkandemoCLI
{
    class InstallDependenciesCommand : public Command
    {
    public:
        struct Dependency
        {
            std::string Name;
            std::string Version;

            std::string ToString();

            static Dependency FromString(const std::string& string);
        };

    public:
        const char* GetName() const override;

        const char* GetDescription() const override;

        void Execute() const override;

    private:
        void InstallGitSubmodules() const;

        void SetGitSubmoduleVersion(const char* folder, const char* version) const;

        std::vector<Dependency> ReadDependencies() const;

    public:
        static const char* NAME;
        static const char* DESCRIPTION;

    private:
        static const char* DEPENDENCIES_DIRECTORY_NAME;
        static const char* DEPENDENCIES_FILENAME;
    };
}

namespace VulkandemoCLI
{
    struct Env
    {
        std::string Key;
        std::string Value;

        std::string ToString()
        {
            std::stringstream ss;
            ss << Key << "=" << Value;
            return ss.str();
        }

        static Env FromString(const std::string& envString)
        {
            int equalSignPosition = envString.find("=");
            Env env;
            env.Key = envString.substr(0, equalSignPosition);
            env.Value = envString.substr(equalSignPosition + 1, envString.length());
            return env;
        }
    };
}
