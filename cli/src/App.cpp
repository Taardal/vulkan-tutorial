#include "App.h"
#include "Context.h"
#include "Help.h"
#include <sstream>

namespace VulkandemoCLI
{
    App::App()
        : helpCommand(CreateHelpCommand()), helpOption(CreateHelpOption())
    {
    }

    void App::Run(int argc, char* argv[])
    {
        Initialize();

        Context context;
        context.App = this;

        bool showHelp = false;

        int firstSegmentIndex = 1;
        bool noSegments = argc - firstSegmentIndex == 0;
        if (noSegments)
        {
            showHelp = true;
        }

        bool previousSegmentWasOption = false;
        for (int i = firstSegmentIndex; i < argc; i++)
        {
            std::string_view segment(argv[i]);
            if (!context.Arguments.empty())
            {
                context.Arguments.push_back(segment);
                continue;
            }
            Option option = GetOption(segment, context.Command);
            if (!option.Name.empty())
            {
                previousSegmentWasOption = true;
                context.Options.push_back(option);
                if (option.Name == helpOption.Name)
                {
                    showHelp = true;
                }
            }
            else if (previousSegmentWasOption)
            {
                previousSegmentWasOption = false;
                context.Options[context.Options.size() - 1].Value = segment;
            }
            else
            {
                previousSegmentWasOption = false;
                const Command* command = FindCommand(segment);
                if (command == nullptr)
                {
                    context.Arguments.push_back(segment);
                    continue;
                }
                if (command->Name == helpCommand.Name)
                {
                    showHelp = true;
                    continue;
                }
                context.Command = command;
            }
        }
        if (showHelp)
        {
            helpCommand.Action(context);
        }
        else if (context.Command != nullptr)
        {
            context.Command->Action(context);
        }
        else
        {
            Action(context);
        }
    }

    void App::Initialize()
    {
        Commands.push_back(helpCommand);
        Options.push_back(helpOption);
    }

    Option App::GetOption(std::string_view segment, const Command* command) const
    {
        constexpr int longFormDashCount = 2;
        constexpr int shortFormDashCount = 1;

        bool longForm = segment.length() > longFormDashCount && segment.substr(0, longFormDashCount) == "--";
        bool shortForm = segment.length() > shortFormDashCount && segment.substr(0, shortFormDashCount) == "-";

        bool isOption = longForm || shortForm;
        if (!isOption)
        {
            return {};
        }

        Option option;
        int dashCount = longForm ? longFormDashCount : shortFormDashCount;
        int equalSignIndex = segment.find("=");
        if (equalSignIndex != std::string::npos)
        {
            option.Name = segment.substr(dashCount, equalSignIndex - dashCount);
            option.Value = segment.substr(equalSignIndex + 1, segment.length());
        }
        else
        {
            option.Name = segment.substr(dashCount, segment.length());
        }
        const Option* definedOption = FindOption(option.Name, Options);
        if (definedOption == nullptr && command != nullptr)
        {
            definedOption = FindOption(option.Name, command->Options);
        }
        if (definedOption == nullptr)
        {
            std::stringstream ss;
            ss << "Option provided but not defined: " << segment;
            throw std::runtime_error(ss.str());
        }
        option.Name = definedOption->Name;
        option.Usage = definedOption->Usage;
        option.Aliases = definedOption->Aliases;
        return option;
    }

    const Option* App::FindOption(std::string_view name, const std::vector<Option>& options) const
    {
        for (const Option& option : options)
        {
            if (name == option.Name)
            {
                return &option;
            }
            for (std::string_view alias : option.Aliases)
            {
                if (name == alias)
                {
                    return &option;
                }
            }
        }
        return nullptr;
    }

    const Command* App::FindCommand(std::string_view segment) const
    {
        for (const Command& command : Commands)
        {
            if (segment == command.Name)
            {
                return &command;
            }
            for (std::string_view commandAlias : command.Aliases)
            {
                if (segment == commandAlias)
                {
                    return &command;
                }
            }
        }
        return nullptr;
    }
}