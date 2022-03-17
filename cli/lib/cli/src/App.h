#pragma once

#include "Command.h"
#include "Option.h"
#include <string>
#include <vector>
#include <functional>

namespace CLI {

    struct App {
    public:
        std::string_view Name;
        std::string_view Usage;
        std::vector<Command> Commands;
        std::vector<Option> Options;
        std::function<void(const Context& context)> Action;

    private:
        Command helpCommand;
        Option helpOption;

    public:
        App();

        void Run(int argc, char* argv[]);

    private:
        void initialize();

        Option getOption(std::string_view segment, const Command* command) const;

        const Option* findOption(std::string_view name, const std::vector<Option>& flags) const;

        const Command* findCommand(std::string_view segment) const;
    };
}