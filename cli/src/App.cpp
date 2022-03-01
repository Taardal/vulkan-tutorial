#include "App.h"
#include "Context.h"
#include "HelpCommand.h"
#include <map>

namespace VulkandemoCLI
{
    const char* App::DEFAULT_EXE_NAME = "vd";

    App::App()
        : helpCommand(CreateHelpCommand())
    {
    }

    void App::Run(int argc, char* argv[]) const
    {
#ifdef VDC_DEBUG
        PrintInput(argc, argv);
#endif
        const Command& command = GetCommand(argc, argv);
        if (command.Name == "")
        {
            printf("%s\n", "Error: Unknown command");
            return;
        }
        Context context;
        context.App = this;
        command.Action(context);
    }

    Command App::GetCommand(int argc, char* argv[]) const
    {
        constexpr int commandIndex = 1;
        bool noCommand = argc < commandIndex + 1;
        if (noCommand)
        {
            return helpCommand;
        }
        std::string commandArgument(argv[commandIndex]);
        if (commandArgument == helpCommand.Name)
        {
            return helpCommand;
        }
        else
        {
            for (Command command : Commands)
            {
                if (command.Name == commandArgument)
                {
                    return command;
                }
            }
        }
        return {};
    }

    std::vector<Option> App::GetOptions(int argc, char* argv[]) const
    {
        int firstArgumentIndex = 2;

        bool noOptions = argc < firstArgumentIndex + 1;
        if (noOptions)
        {
            return {};
        }

        std::vector<Option> options;
        for (int i = firstArgumentIndex; i < argc; i++)
        {
            std::string argument(argv[i]);
            std::string nextArgument = argc - 1 >= i + 1 ? std::string(argv[i + 1]) : "";

            constexpr int longFormDashCount = 2;
            constexpr int shortFormDashCount = 1;

            bool longForm = argument.length() > 2 && argument.substr(0, longFormDashCount) == "--";
            bool shortForm = argument.length() > 1 && argument.substr(0, shortFormDashCount) == "-";

            if (!longForm && !shortForm)
            {
                continue;
            }

            int dashCount = longForm ? longFormDashCount : shortFormDashCount;
            options.push_back(GetOption(argument, nextArgument, dashCount));
        }
        return options;
    }

    Option App::GetOption(const std::string& argument, const std::string& nextArgument, int dashCount) const
    {
        int equalSignIndex = argument.find("=");
        bool hasValueWithEquals = equalSignIndex != std::string::npos;
        bool hasValueWithSpace = nextArgument.length() > 0 && nextArgument.substr(0, 1) != "-";
        Option option;
        if (hasValueWithEquals)
        {
            option.Name = argument.substr(dashCount, equalSignIndex - dashCount);
            option.NameWithDashes = argument.substr(0, equalSignIndex);
            option.Value = argument.substr(equalSignIndex + 1, argument.length());
        }
        else if (hasValueWithSpace)
        {
            option.Name = argument.substr(dashCount, argument.length());
            option.NameWithDashes = argument;
            option.Value = nextArgument;
        }
        else
        {
            option.Name = argument.substr(dashCount, argument.length());
            option.NameWithDashes = argument;
        }
        return option;
    }

    void App::PrintHelp() const
    {
        /*
#ifdef VDC_EXE_NAME
        const char* appName = VDC_EXE_NAME;
#else
        const char* appName = DEFAULT_EXE_NAME;
#endif

        constexpr int keyLeftPadding = 5;
        constexpr int valueLeftPadding = 15;

        printf("Usage:\n");
        int usageWidth = keyLeftPadding + strlen(appName);
        printf("%*s %s\n", usageWidth, appName, "[options] [command]");
        printf("\n");

        printf("Options:\n");
        const char* optionsKey = "-h, --help";
        const char* optionsDescription = "Output usage information";
        int optionsKeyWidth = keyLeftPadding + strlen(optionsKey);
        printf("%*s", optionsKeyWidth, optionsKey);
        int optionsDescriptionWidth = valueLeftPadding + strlen(optionsDescription);
        printf("%*s\n", optionsDescriptionWidth, optionsDescription);
        printf("\n");

        int longestKeyLength = strlen(optionsKey);
        for (const auto& iterator : commands)
        {
            int commandNameLength = iterator.first.length();
            if (commandNameLength > longestKeyLength)
            {
                longestKeyLength = commandNameLength;
            }
        }

        printf("Commands:\n");
        for (const auto& iterator : commands)
        {
            Command* command = iterator.second;
            int nameWidth = keyLeftPadding + strlen(command->GetName());
            printf("%*s", nameWidth, command->GetName());

            int keyRightPadding = longestKeyLength - strlen(command->GetName());
            int descriptionWidth = keyRightPadding + valueLeftPadding + strlen(command->GetDescription());
            printf("%*s\n", descriptionWidth, command->GetDescription());
        }
        printf("\n");
        */
    }

    void App::PrintInput(int argc, char* argv[]) const
    {
        for (int i = 0; i < argc; i++)
        {
            printf("argc [%i/%i], argv [%s]\n", (i + 1), argc, argv[i]);
        }
        printf("\n");
    }
}
