#include "App.h"
#include "Context.h"
#include "Help.h"
#include <sstream>
#include <exception>

namespace VulkandemoCLI
{
    App::App()
        : helpCommand(CreateHelpCommand()), helpFlag(CreateHelpFlag())
    {
    }

    void App::Run(int argc, char* argv[])
    {
        Initialize();

        Context context;
        context.App = this;

        const std::function<void(const Context&)>* action;

        int firstSegmentIndex = 1;
        bool showHelp = argc - firstSegmentIndex == 0;
        bool previousSegmentWasFlag = false;
        for (int i = firstSegmentIndex; i < argc; i++)
        {
            const std::string& segment = argv[i];
            if (!context.Arguments.empty())
            {
                context.Arguments.push_back(segment);
                continue;
            }
            const Flag& flag = GetFlag(segment, context.Command);
            if (!flag.Name.empty())
            {
                previousSegmentWasFlag = true;
                context.Flags.push_back(flag);
                if (flag.Name == helpFlag.Name)
                {
                    action = &helpCommand.Action;
                }
            }
            else if (previousSegmentWasFlag)
            {
                previousSegmentWasFlag = false;
                context.Flags[context.Flags.size() - 1].Value = segment;
            }
            else
            {
                previousSegmentWasFlag = false;
                const Command* command = FindCommand(segment);
                if (command != nullptr)
                {
                    if (command->Name != helpCommand.Name)
                    {
                        context.Command = command;
                    }
                    action = &command->Action;
                }
                else
                {
                    context.Arguments.push_back(segment);
                }
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
        (*action)(context);
    }

    void App::Initialize()
    {
        Commands.push_back(helpCommand);
        Flags.push_back(helpFlag);
    }

    Flag App::GetFlag(const std::string &segment, const Command* command) const
    {
        constexpr int longFormDashCount = 2;
        constexpr int shortFormDashCount = 1;
        bool longForm = segment.length() > longFormDashCount && segment.substr(0, longFormDashCount) == "--";
        bool shortForm = segment.length() > shortFormDashCount && segment.substr(0, shortFormDashCount) == "-";
        bool isFlag = longForm || shortForm;
        if (!isFlag)
        {
            return {};
        }
        Flag flag;
        int dashCount = longForm ? longFormDashCount : shortFormDashCount;
        int equalSignIndex = segment.find("=");
        if (equalSignIndex != std::string::npos)
        {
            flag.Name = segment.substr(dashCount, equalSignIndex - dashCount);
            flag.Value = segment.substr(equalSignIndex + 1, segment.length());
        }
        else
        {
            flag.Name = segment.substr(dashCount, segment.length());
        }
        const Flag* definedFlag = FindFlag(flag.Name, Flags);
        if (definedFlag == nullptr && command != nullptr)
        {
            definedFlag = FindFlag(flag.Name, command->Flags);
        }
        if (definedFlag == nullptr)
        {
            std::stringstream ss;
            ss << "Flag provided but not defined: " << segment;
            throw std::runtime_error(ss.str());
        }
        flag.Name = definedFlag->Name;
        flag.Usage = definedFlag->Usage;
        flag.Aliases = definedFlag->Aliases;
        return flag;
    }

    const Flag* App::FindFlag(const std::string& name, const std::vector<Flag>& flags) const
    {
        for (const Flag& flag : flags)
        {
            if (name == flag.Name)
            {
                return &flag;
            }
            for (const std::string& alias : flag.Aliases)
            {
                if (name == alias)
                {
                    return &flag;
                }
            }
        }
        return nullptr;
    }

    const Command* App::FindCommand(const std::string &segment) const
    {
        for (const Command& command : Commands)
        {
            if (segment == command.Name)
            {
                return &command;
            }
            for (const std::string& commandAlias : command.Aliases)
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