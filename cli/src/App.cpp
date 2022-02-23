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
            std::string input(argv[i]);

            bool longForm = input.length() > 2 && input.substr(0, 2) == "--";
            bool shortForm = input.length() > 1 && input.substr(0, 1) == "-";

            if (longForm)
            {
                bool hasValueWithEquals = input.find("=") != std::string::npos;

                std::string nextInput = argc - 1 >= i + 1 ? std::string(argv[i + 1]) : "";
                bool nextInputIsValueForThisOption = nextInput.length() > 0 && nextInput.substr(0, 1) != "-";
                bool hasValueWithSpace = nextInputIsValueForThisOption;

                Option option;
                if (hasValueWithEquals)
                {
                    printf("NOTICE ME: %s\n", input.c_str());
                    printf("NOTICE ME: %lu\n", input.find("="));
                    printf("NOTICE ME: %s\n", input.substr(2, input.find("=") - 2).c_str());
                    option.Name = input.substr(2, input.find("=") - 2);
                    option.NameWithDashes = input.substr(0, input.find("="));
                    option.Value = input.substr(input.find("=") + 1, input.length());
                }
                else if (hasValueWithSpace)
                {
                    option.Name = input.substr(2, input.length());
                    option.NameWithDashes = input;
                    option.Value = nextInput;
                }
                else
                {
                    option.Name = input.substr(2, input.length());
                    option.NameWithDashes = input;
                }
                options.push_back(option);
            }
            else if (shortForm)
            {
                bool hasValueWithEquals = input.find("=") != std::string::npos;

                std::string nextInput = argc - 1 >= i + 1 ? std::string(argv[i + 1]) : "";
                bool nextInputIsValueForThisOption = nextInput.length() > 0 && nextInput.substr(0, 1) != "-";
                bool hasValueWithSpace = nextInputIsValueForThisOption;

                Option option;
                if (hasValueWithEquals)
                {
                    option.Name = input.substr(1, input.find("=") - 1);
                    option.NameWithDashes = input.substr(0, input.find("="));
                    option.Value = input.substr(input.find("=") + 1, input.length());
                }
                else if (hasValueWithSpace)
                {
                    option.Name = input.substr(1, input.length());
                    option.NameWithDashes = input;
                    option.Value = nextInput;
                }
                else
                {
                    option.Name = input.substr(1, input.length());
                    option.NameWithDashes = input;
                }
                options.push_back(option);
            }
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
