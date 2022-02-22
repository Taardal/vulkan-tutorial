#pragma once

#include "Command.h"

namespace VulkandemoCLI
{
    class InstallGLFWCommand : public Command
    {
    public:
        void Execute() const override;
    };
}
