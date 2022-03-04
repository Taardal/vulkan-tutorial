#include "Help.h"
#include "App.h"
#include <sstream>

namespace VulkandemoCLI
{
    Flag CreateHelpFlag()
    {
        Flag flag;
        flag.Name = "help";
        flag.Aliases = {"h"};
        flag.Usage = "Show help";
        return flag;
    }

    Command CreateHelpCommand()
    {
        Command command;
        command.Name = "help";
        command.Usage = "Shows a list of commands or help for one command";
        command.Aliases = {"h"};
        command.Action = [](const Context& context) -> void
        {
            const App& app = *context.App;

            constexpr int keyLeftPadding = 4;
            constexpr int valueLeftPadding = 4;

            if (context.Command != nullptr)
            {
                const Command& command = *context.Command;
                {
                    printf("%s\n", "NAME:");
                    int keyWidth = keyLeftPadding + command.Name.length();
                    printf("%*s - %s\n", keyWidth, command.Name.c_str(), command.Usage.c_str());
                    printf("\n");
                }
                {
                    printf("%s\n", "USAGE:");
                    int keyWidth = keyLeftPadding + command.Name.length();
                    printf("%*s %s\n", keyWidth, command.Name.c_str(), "[command options] [arguments...]");
                    printf("\n");
                }
                {
                    printf("%s\n", "OPTIONS:");
                    const char* longFormPrefix = "--";
                    const char* shortFormPrefix = "-";
                    int longestKeyLength = 0;
                    for (const Flag& flag : command.Flags)
                    {
                        std::stringstream ss;
                        ss << longFormPrefix << flag.Name;
                        for (const std::string& alias : flag.Aliases)
                        {
                            ss << ", " << shortFormPrefix << alias;
                        }
                        int flagKeyLength = ss.str().length();
                        if (flagKeyLength > longestKeyLength)
                        {
                            longestKeyLength = flagKeyLength;
                        }
                    }
                    for (const Flag& flag : command.Flags)
                    {
                        std::stringstream ss;
                        ss << longFormPrefix << flag.Name;
                        for (const std::string& alias : flag.Aliases)
                        {
                            ss << ", " << shortFormPrefix << alias;
                        }
                        const std::string& key = ss.str();
                        int keyWidth = keyLeftPadding + key.length();
                        printf("%*s", keyWidth, key.c_str());

                        const std::string& value = flag.Usage;
                        int keyRightPadding = longestKeyLength - key.length();
                        int valueWidth = keyRightPadding + valueLeftPadding + value.length();
                        printf("%*s\n", valueWidth, value.c_str());
                    }
                    printf("\n");
                }
            }
            else
            {
                {
                    printf("%s\n", "NAME:");
                    int keyWidth = keyLeftPadding + app.Name.length();
                    printf("%*s - %s\n", keyWidth, app.Name.c_str(), app.Usage.c_str());
                    printf("\n");
                }
                {
                    printf("%s\n", "USAGE:");
                    int keyWidth = keyLeftPadding + app.Name.length();
                    printf("%*s %s\n", keyWidth, app.Name.c_str(), "[global options] command [command options] [arguments...]");
                    printf("\n");
                }
                {
                    printf("%s\n", "COMMANDS:");
                    int longestKeyLength = 0;
                    for (const Command& command : app.Commands)
                    {
                        std::stringstream ss;
                        ss << command.Name;
                        for (const std::string& alias : command.Aliases)
                        {
                            ss << ", " << alias;
                        }
                        int keyLength = ss.str().length();
                        if (keyLength > longestKeyLength)
                        {
                            longestKeyLength = keyLength;
                        }
                    }
                    for (const Command& command : app.Commands)
                    {
                        std::stringstream ss;
                        ss << command.Name;
                        for (const std::string& alias : command.Aliases)
                        {
                            ss << ", " << alias;
                        }
                        const std::string& key = ss.str();
                        int keyWidth = keyLeftPadding + key.length();
                        printf("%*s", keyWidth, key.c_str());

                        const std::string& value = command.Usage;
                        int keyRightPadding = longestKeyLength - key.length();
                        int valueWidth = keyRightPadding + valueLeftPadding + value.length();
                        printf("%*s\n", valueWidth, value.c_str());
                    }
                    printf("\n");
                }
                {
                    printf("%s\n", "GLOBAL OPTIONS:");
                    const char* longFormPrefix = "--";
                    const char* shortFormPrefix = "-";
                    int longestKeyLength = 0;
                    for (const Flag& flag : app.Flags)
                    {
                        std::stringstream ss;
                        ss << longFormPrefix << flag.Name;
                        for (const std::string& alias : flag.Aliases)
                        {
                            ss << ", " << shortFormPrefix << alias;
                        }
                        int flagKeyLength = ss.str().length();
                        if (flagKeyLength > longestKeyLength)
                        {
                            longestKeyLength = flagKeyLength;
                        }
                    }
                    for (const Flag& flag : app.Flags)
                    {
                        std::stringstream ss;
                        ss << longFormPrefix << flag.Name;
                        for (const std::string& alias : flag.Aliases)
                        {
                            ss << ", " << shortFormPrefix << alias;
                        }
                        const std::string& key = ss.str();
                        int keyWidth = keyLeftPadding + key.length();
                        printf("%*s", keyWidth, key.c_str());

                        const std::string& value = flag.Usage;
                        int keyRightPadding = longestKeyLength - key.length();
                        int valueWidth = keyRightPadding + valueLeftPadding + value.length();
                        printf("%*s\n", valueWidth, value.c_str());
                    }
                    printf("\n");
                }
            }
        };
        return command;
    }
}
