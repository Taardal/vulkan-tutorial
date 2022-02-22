#pragma once

namespace VulkandemoCLI
{
    class Command
    {
    public:
        virtual void Execute() const = 0;
    };
}