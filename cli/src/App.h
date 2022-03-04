#pragma once

#include "Command.h"
#include "Option.h"
#include <string>
#include <vector>
#include <functional>

namespace VulkandemoCLI
{
    struct App
    {
    public:
        App();

        void Run(int argc, char* argv[]);

    private:
        void Initialize();

        Option GetOption(const std::string& segment, const Command* command) const;

        const Option* FindOption(const std::string& name, const std::vector<Option>& flags) const;

        const Command* FindCommand(const std::string& segment) const;

    public:
        std::string Name;
        std::string Usage;
        std::vector<Command> Commands;
        std::vector<Option> Options;
        std::function<void(const Context& context)> Action;

    private:
        Command helpCommand;
        Option helpOption;
    };
}