#include "App.h"
#include "Context.h"
#include "Environment.h"
#include "HelpCommand.h"
#include <sstream>
#include <exception>

namespace VulkandemoCLI
{
    App::App()
        : helpCommand(CreateHelpCommand())
    {
    }

    void App::Run(int argc, char* argv[])
    {
        Initialize();

        Command command;
        std::vector<Flag> flags;
        std::vector<std::string> args;

        Context context;
        context.App = this;
        context.Command = &command;
        context.Flags = &flags;
        context.Args = &args;

        const std::vector<std::string>& arguments = GetArguments(argc, argv);

        bool previousArgumentWasFlag = false;

        for (int i = 0; i < arguments.size(); i++)
        {
            const std::string& argument = arguments[i];

            bool everyArgumentIsArg = !args.empty();
            if (everyArgumentIsArg)
            {
                args.push_back(argument);
                continue;
            }

            const Flag& flag = GetFlag(argument, context);

            bool argumentIsFlag = !flag.Name.empty();
            if (argumentIsFlag)
            {
                previousArgumentWasFlag = true;
                flags.push_back(flag);
            }
            else if (previousArgumentWasFlag)
            {
                flags[flags.size() - 1].Value = argument;
                previousArgumentWasFlag = false;
            }
            else
            {
                previousArgumentWasFlag = false;

                const Command& parsedCommand = GetCommand(argument);
                if (parsedCommand.Name.size() > 0)
                {
                    command = parsedCommand;
                }
                else
                {
                    args.push_back(argument);
                }
            }
        }

        if (command.Name.size() > 0)
        {
            command.Action(context);
            return;
        }

        Action(context);
    }

    void App::Initialize()
    {
        Commands.push_back(helpCommand);
        helpFlag.Name = "help";
        helpFlag.Aliases = {"h"};
        helpFlag.Usage = "Show help";
        Flags.push_back(helpFlag);
    }

    std::vector<std::string> App::GetArguments(int argc, char* argv[]) const
    {
        std::vector<std::string> arguments;
        constexpr int firstArgumentIndex = 1;
        if (argc > firstArgumentIndex)
        {
            for (int i = firstArgumentIndex; i < argc; i++)
            {
                arguments.push_back(std::string(argv[i]));
            }
        }
        return arguments;
    }

    Flag App::GetFlag(const std::string &argument, const Context& context) const
    {
        constexpr int longFormDashCount = 2;
        constexpr int shortFormDashCount = 1;
        bool longForm = argument.length() > longFormDashCount && argument.substr(0, longFormDashCount) == "--";
        bool shortForm = argument.length() > shortFormDashCount && argument.substr(0, shortFormDashCount) == "-";
        bool isFlag = longForm || shortForm;
        if (!isFlag)
        {
            return {};
        }
        Flag parsedFlag;
        int dashCount = longForm ? longFormDashCount : shortFormDashCount;
        int equalSignIndex = argument.find("=");
        bool hasValueWithEquals = equalSignIndex != std::string::npos;
        if (hasValueWithEquals)
        {
            parsedFlag.Name = argument.substr(dashCount, equalSignIndex - dashCount);
            parsedFlag.Value = argument.substr(equalSignIndex + 1, argument.length());
        }
        else
        {
            parsedFlag.Name = argument.substr(dashCount, argument.length());
        }
        bool parsedFlagIsDefined = false;
        for (const Flag& definedFlag : Flags)
        {
            if (parsedFlag.Name == definedFlag.Name)
            {
                parsedFlag.Aliases = definedFlag.Aliases;
                parsedFlag.Usage = definedFlag.Usage;
                parsedFlagIsDefined = true;
                break;
            }
        }
        if (!parsedFlagIsDefined && context.Command != nullptr && !context.Command->Name.empty())
        {
            for (const Flag& commandFlag : context.Command->Flags)
            {
                if (parsedFlag.Name == commandFlag.Name)
                {
                    parsedFlag.Aliases = commandFlag.Aliases;
                    parsedFlag.Usage = commandFlag.Usage;
                    parsedFlagIsDefined = true;
                    break;
                }
            }
        }
        if (!parsedFlagIsDefined)
        {
            std::stringstream ss;
            ss << "Flag provided but not defined: " << argument;
            throw std::runtime_error(ss.str());
        }
        return parsedFlag;
    }

    Command App::GetCommand(const std::string &argument) const
    {
        for (const Command& command : Commands)
        {
            if (argument == command.Name)
            {
                return command;
            }
            for (const std::string& commandAlias : command.Aliases)
            {
                if (argument == commandAlias)
                {
                    return command;
                }
            }
        }
        return {};
    }
}