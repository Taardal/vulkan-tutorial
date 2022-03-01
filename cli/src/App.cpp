#include "App.h"
#include "Context.h"
#include "Environment.h"
#include "HelpCommand.h"
#include <map>

namespace VulkandemoCLI
{
    const char* App::DEFAULT_EXE_NAME = "vd";

    App::App()
        : helpCommand(CreateHelpCommand())
    {
        Flag flag;
        flag.Name = "help";
        helpFlag = flag;
    }

    void App::Run(int argc, char* argv[]) const
    {
#ifdef VDC_DEBUG
        PrintInput(argc, argv);
#endif
        const std::vector<std::string>& arguments = GetArguments(argc, argv);
        //const std::vector<Option>& options = GetOptions(arguments);

        const Command& command = GetCommand(arguments);
        if (command.Name == "")
        {
            printf("%s\n", "Error: Unknown command");
            return;
        }

        const std::vector<Flag>& commandFlags = GetFlags(arguments);

        printf("%s\n", "ZEH FLAGZ:");
        for (const Flag& flag : commandFlags)
        {
            printf("%s", flag.Name.c_str());
            if (!flag.Value.empty())
            {
                printf(" -- %s", flag.Value.c_str());
            }
            printf("\n");
        }
        printf("\n");

        std::vector<Flag> copies;
        for (const Flag& input : commandFlags)
        {
            printf("INPUT -- %s\n", input.Name.c_str());

            Flag copy;
            for (const Flag& flag1 : command.Flags)
            {
                printf("CANDIDATE -- %s\n", flag1.Name.c_str());
                /*
                if (flag1.Name == input.Name)
                {
                    copy = flag1;
                    copy.Value = input.Value;
                    break;
                }
                */
                for (const std::string& alias : flag1.Aliases)
                {
                    printf("CANDIDATE ALIAS -- %s\n", alias.c_str());
                    if (alias == input.Name)
                    {
                        copy = flag1;
                        copy.Value = input.Value;
                        break;
                    }
                }
            }
            if (copy.Name.empty())
            {
                copy = input;
            }
            printf("FINAL -- %s\n", copy.Name.c_str());
            copies.push_back(copy);
        }


        Context context;
        context.App = this;
        context.Flags = copies;
        command.Action(context);
    }

    std::vector<Flag> App::GetFlags(const std::vector<std::string>& arguments) const
    {
        std::vector<Flag> flags;
        int firstFlagIndex = 1;
        if (arguments.size() > firstFlagIndex)
        {
            for (int i = firstFlagIndex; i < arguments.size(); i++)
            {
                const std::string& argument = arguments[i];

                constexpr int longFormDashCount = 2;
                constexpr int shortFormDashCount = 1;

                bool longForm = argument.length() > longFormDashCount && argument.substr(0, longFormDashCount) == "--";
                bool shortForm = argument.length() > shortFormDashCount && argument.substr(0, shortFormDashCount) == "-";

                if (longForm || shortForm)
                {
                    int dashCount = longForm ? longFormDashCount : shortFormDashCount;
                    const std::string& nextArgument = arguments.size() - 1 >= i + 1 ? arguments[i + 1] : "";

                    int equalSignIndex = argument.find("=");
                    bool hasValueWithEquals = equalSignIndex != std::string::npos;
                    bool hasValueWithSpace = nextArgument.length() > 0 && nextArgument.substr(0, 1) != "-";
                    Flag flag;
                    if (hasValueWithEquals)
                    {
                        flag.Name = argument.substr(dashCount, equalSignIndex - dashCount);
                        flag.Value = argument.substr(equalSignIndex + 1, argument.length());
                    }
                    else if (hasValueWithSpace)
                    {
                        flag.Name = argument.substr(dashCount, argument.length());
                        flag.Value = nextArgument;
                    }
                    else
                    {
                        flag.Name = argument.substr(dashCount, argument.length());
                    }
                    flags.push_back(flag);
                }
            }
        }
        return flags;
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

    Command App::GetCommand(const std::vector<std::string>& arguments) const
    {
        if (arguments.size() == 0)
        {
            return helpCommand;
        }
        const std::string& commandArgument = arguments[0];
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

    std::vector<Option> App::GetOptions(const std::vector<std::string>& arguments) const
    {
        std::vector<Option> options;
        int firstOptionIndex = 1;
        if (arguments.size() > firstOptionIndex)
        {
            for (int i = firstOptionIndex; i < arguments.size(); i++)
            {
                const std::string& argument = arguments[i];

                constexpr int longFormDashCount = 2;
                constexpr int shortFormDashCount = 1;

                bool longForm = argument.length() > longFormDashCount && argument.substr(0, longFormDashCount) == "--";
                bool shortForm = argument.length() > shortFormDashCount && argument.substr(0, shortFormDashCount) == "-";

                if (longForm || shortForm)
                {
                    int dashCount = longForm ? longFormDashCount : shortFormDashCount;
                    const std::string& nextArgument = arguments.size() - 1 >= i + 1 ? arguments[i + 1] : "";
                    options.push_back(GetOption(argument, nextArgument, dashCount));
                }
            }
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
