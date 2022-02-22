#pragma once

#include "Command.h"

namespace VulkandemoCLI
{
    class InstallDependenciesCommand : public Command
    {
    public:
        const char* GetName() const override;

        const char* GetDescription() const override;

        void Execute() const override;

    private:
        void InstallGitSubmodules() const;

        void SetGitSubmoduleVersion(const char* folder, const char* version) const;

    public:
        static const char* NAME;

    private:
        static const char* LIB_FOLDER;
    };
}
