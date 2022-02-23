#include "App.h"
#include "BuildProjectCommand.h"
#include "InstallDependenciesCommand.h"
#include "InstallGLFWCommand.h"
#include <map>

namespace VulkandemoCLI
{
    const char* App::DEFAULT_EXE_NAME = "vd";

    App::App()
        : commands({
            {BuildProjectCommand::NAME, new BuildProjectCommand()},
            {InstallDependenciesCommand::NAME, new InstallDependenciesCommand()},
            {InstallGLFWCommand::NAME, new InstallGLFWCommand()}
        })
    {
    }

    App::~App()
    {
        commands.clear();
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

            int longFormDashCount = 2;
            int shortFormDashCount = 2;

            bool longForm = argument.length() > 2 && argument.substr(0, longFormDashCount) == "--";
            bool shortForm = argument.length() > 1 && argument.substr(0, shortFormDashCount) == "-";

            if (!longForm && !shortForm)
            {
                continue;
            }

            int dashCount = longForm ? longFormDashCount : shortFormDashCount;
            int argumentEqualSignIndex = argument.find("=");
            bool hasValueWithEquals = argumentEqualSignIndex != std::string::npos;

            int nextArgumentIndex = i + 1;
            std::string nextArgument = argc - 1 >= nextArgumentIndex ? std::string(argv[nextArgumentIndex]) : "";
            bool hasValueWithSpace = nextArgument.length() > 0 && nextArgument.substr(0, 1) != "-";

            Option option;
            if (hasValueWithEquals)
            {
                option.Name = argument.substr(dashCount, argumentEqualSignIndex - dashCount);
                option.NameWithDashes = argument.substr(0, argumentEqualSignIndex);
                option.Value = argument.substr(argumentEqualSignIndex + 1, argument.length());
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
            options.push_back(option);
        }

        printf("\n");
        for (Option option : options)
        {
            printf("%s\n", option.NameWithDashes.c_str());
            printf("%s\n", option.Name.c_str());
            if (option.Value.length() > 0)
            {
                printf("%s --> %s\n", option.Name.c_str(), option.Value.c_str());
            }
            printf("\n");
        }
        printf("\n");

        return options;
    }

    Command* App::GetCommand(int argc, char* argv[]) const
    {
#ifdef VD_DEBUG
        PrintInput(argc, argv);
#endif
        bool noCommand = argc < 2;
        if (noCommand)
        {
            PrintHelp();
            return nullptr;
        }
        const char* command = argv[1];
        bool printHelp = strcmp("-h", command) == 0 || strcmp("--help", command) == 0;
        if (printHelp)
        {
            PrintHelp();
            return nullptr;
        }
        const auto& iterator = commands.find(command);
        if (iterator == commands.end())
        {
            printf("Unknown command: %s\n", command);
            return nullptr;
        }
        return iterator->second;
    }

    void App::PrintHelp() const
    {
#ifdef VD_EXE_NAME
        const char* appName = VD_EXE_NAME;
#else
        const char* appName = DEFAULT_EXE_NAME;
#endif

        int keyLeftPadding = 5;
        int valueLeftPadding = 15;

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
    }

    void App::PrintInput(int argc, char* argv[]) const
    {
        for (int i = 0; i < argc; i++)
        {
            printf("argc [%i/%i], argv [%s]\n", (i + 1), argc, argv[i]);
        }
    }
}
