#pragma once

#include "Option.h"
#include <vector>

namespace VulkandemoCLI
{
    class Command
    {
    public:
        virtual const char* GetName() const = 0;

        virtual const char* GetDescription() const = 0;

        virtual void Execute(const std::vector<Option>& options) const = 0;
    };
}