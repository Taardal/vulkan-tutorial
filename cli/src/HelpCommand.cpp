#include "HelpCommand.h"
#include "App.h"

namespace VulkandemoCLI
{
    Command CreateHelpCommand()
    {
        Command command;
        command.Name = "help";
        command.Usage = "Shows a list of commands or help for one command";
        command.Action = [](const Context& context) -> void
        {
#ifdef VDC_EXE_NAME
            const char* exeName = VDC_EXE_NAME;
#else
            const char* exeName = "vd";
#endif

            /*

NAME:
   aa - Useful commands for animators working @ Apparat

USAGE:
   animation-cli [global options] command [command options] [arguments...]

COMMANDS:
   character, c    Download all versions of character(s) from remote (Z:) to local (W:) drive
   environment, e  Download all versions of one-or-more environments from remote (Z:) to local (W:) drive
   prop, p         Download all versions of one-or-more props from remote (Z:) to local (W:) drive
   sequence, s     Download sequence(s), cache(s) and animation-folder from remote (Z:) to local (W:) drive
   help, h         Shows a list of commands or help for one command

GLOBAL OPTIONS:
   --help, -h  show help (default: false)

            */


            const App& app = *context.App;

            constexpr int keyLeftPadding = 4;
            constexpr int valueLeftPadding = 4;

            int longestKeyLength = 0;
            for (const Command& command : app.Commands)
            {
                int commandNameLength = command.Name.length();
                if (commandNameLength > longestKeyLength)
                {
                    longestKeyLength = commandNameLength;
                }
            }

            printf("%s\n", "NAME:");
            int nameWidth = keyLeftPadding + strlen(app.Name.c_str());
            printf("%*s - %s\n", nameWidth, app.Name.c_str(), app.Usage.c_str());
            printf("\n");

            printf("%s\n", "USAGE:");
            int usageWidth = keyLeftPadding + strlen(exeName);
            printf("%*s %s\n", usageWidth, exeName, "[command] [command options]");
            printf("\n");

            printf("%s\n", "COMMANDS:");
            for (const Command& command : app.Commands)
            {
                int commandNameWidth = keyLeftPadding + strlen(command.Name.c_str());
                printf("%*s", commandNameWidth, command.Name.c_str());

                int keyRightPadding = longestKeyLength - strlen(command.Name.c_str());
                int descriptionWidth = keyRightPadding + valueLeftPadding + strlen(command.Usage.c_str());
                printf("%*s\n", descriptionWidth, command.Usage.c_str());
            }
            printf("\n");
        };
        return command;
    }
}
