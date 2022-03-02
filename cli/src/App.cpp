#include "App.h"
#include "Context.h"
#include "Environment.h"
#include "HelpCommand.h"

namespace VulkandemoCLI
{

}

/*

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
                if (flag1.Name == input.Name)
                {
                    copy = flag1;
                    copy.Value = input.Value;
                    break;
                }
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

*/
