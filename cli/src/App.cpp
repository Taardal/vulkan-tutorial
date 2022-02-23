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
        int firstOptionArgumentIndex = 2;
        bool noOptions = argc < firstOptionArgumentIndex + 1;
        if (noOptions)
        {
            return {};
        }
        std::vector<Option> options;
        for (int i = firstOptionArgumentIndex; i < argc; i++)
        {
            std::string optionString(argv[i]);
            if (optionString.length() == 0 || optionString[0] != '-')
            {
                continue;
            }
            int equalSignPosition = optionString.find("=");
            if (equalSignPosition == std::string::npos)
            {
                Option option;
                option.Name = optionString;
                options.push_back(option);
                continue;
            }
            Option option;
            option.Name = optionString.substr(0, equalSignPosition);
            option.Value = optionString.substr(equalSignPosition + 1, optionString.length());
            options.push_back(option);
        }

        for (Option option : options)
        {
            printf("%s", option.Name.c_str());
            if (option.Value.length() > 0)
            {
                printf("--> %s", option.Value.c_str());
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
