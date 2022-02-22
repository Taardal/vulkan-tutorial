#pragma once

namespace VulkandemoCLI
{
    class Command
    {
    public:
        virtual const char* GetName() const = 0;

        virtual const char* GetDescription() const = 0;

        virtual void Execute() const = 0;
    };
}