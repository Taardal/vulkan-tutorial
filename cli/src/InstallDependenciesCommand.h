#pragma once

#include "Command.h"

namespace VulkandemoCLI
{
    class InstallDependenciesCommand : public Command
    {
    public:
        void Execute() const override;

    private:
        void InstallGitSubmodules() const;

        void SetGitSubmoduleVersion(const char* folder, const char* version) const;

        static const char* LIB_FOLDER;
    };
}
