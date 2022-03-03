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
        // SETUP
        Commands.push_back(helpCommand);
        helpFlag.Name = "help";
        helpFlag.Aliases = {"h"};
        helpFlag.Usage = "Show help";
        Flags.push_back(helpFlag);

        // RUN
        Context context;
        context.App = this;

        Command command;
        std::vector<Flag> flags;
        std::vector<std::string> args;

        const std::vector<std::string>& arguments = GetArguments(argc, argv);

        bool previousArgumentWasFlag = false;
        bool commandExists = false;
        bool argumentsStarted = false;

        for (int i = 0; i < arguments.size(); i++)
        {
            const std::string& argument = arguments[i];

            if (argumentsStarted)
            {
                args.push_back(argument);
                continue;
            }

            constexpr int longFormDashCount = 2;
            constexpr int shortFormDashCount = 1;

            bool longForm = argument.length() > longFormDashCount && argument.substr(0, longFormDashCount) == "--";
            bool shortForm = argument.length() > shortFormDashCount && argument.substr(0, shortFormDashCount) == "-";

            int dashCount = longForm ? longFormDashCount : shortFormDashCount;

            bool isFlag = longForm || shortForm;
            if (isFlag)
            {
                Flag flag;
                int equalSignIndex = argument.find("=");
                bool hasValueWithEquals = equalSignIndex != std::string::npos;
                if (hasValueWithEquals)
                {
                    flag.Name = argument.substr(dashCount, equalSignIndex - dashCount);
                    flag.Value = argument.substr(equalSignIndex + 1, argument.length());
                }
                else
                {
                    flag.Name = argument.substr(dashCount, argument.length());
                }
                bool flagExists = false;
                for (const Flag& f : Flags)
                {
                    if (f.Name == flag.Name)
                    {
                        flagExists = true;
                        break;
                    }
                }
                if (!flagExists)
                {
                    for (const Flag& f : command.Flags)
                    {
                        if (f.Name == flag.Name)
                        {
                            flagExists = true;
                            break;
                        }
                    }
                }
                if (flagExists)
                {
                    previousArgumentWasFlag = true;
                    flags.push_back(flag);
                }
                else
                {
                    std::stringstream ss;
                    ss << "Flag provided but not defined: " << argument;
                    throw std::runtime_error(ss.str());
                }
            }
            else if (previousArgumentWasFlag)
            {
                flags[flags.size() - 1].Value = argument;
                previousArgumentWasFlag = false;
            }
            else
            {
                previousArgumentWasFlag = false;

                bool commandFound = false;
                if (!commandExists)
                {
                    for (const Command& appCommand : Commands)
                    {
                        if (argument == appCommand.Name)
                        {
                            command = appCommand;
                            commandExists = true;
                            commandFound = true;
                            break;
                        }
                        for (const std::string& appCommandAlias : appCommand.Aliases)
                        {
                            if (argument == appCommandAlias)
                            {
                                command = appCommand;
                                commandExists = true;
                                commandFound = true;
                                break;
                            }
                        }
                    }
                }

                if (!commandFound)
                {
                    args.push_back(argument);
                    argumentsStarted = true;
                }
            }
        }

        context.Command = &command;
        context.Flags = flags;
        context.Args = args;

        if (command.Name.size() > 0)
        {
            command.Action(context);
            return;
        }

        Action(context);
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
}