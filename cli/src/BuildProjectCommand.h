#pragma once

#include "Command.h"

namespace VulkandemoCLI
{
    class BuildProjectCommand : public Command
    {
    public:
        void Execute() const override;
    };
}