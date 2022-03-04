#include "App.h"
#include "Environment.h"
#include "BuildProjectCommand.h"
#include "FileSystem.h"
#include "InstallDependenciesCommand.h"
#include "InstallGLFWCommand.h"

using namespace VulkandemoCLI;

int main(int argc, char* argv[]) {
    FileSystem fileSystem;

#ifdef VDC_EXE_NAME
    const char* appName = VDC_EXE_NAME;
#else
    const char* appName = "vd";
#endif

    Option cmdOption;
    cmdOption.Name = "bar";
    cmdOption.Usage = "bar option";

    Command command;
    command.Name = "foo";
    command.Usage = "The Foo, The Bar and the WTF";
    command.Options = {
            cmdOption
    };
    command.Action = [](const Context& context) -> void
    {
        printf("%s\n", "The Foo, The Bar and the WTF");
        printf("%s\n", context.ToString().c_str());
    };

    Option option;
    option.Name = "lang";
    option.Usage = "Language for the greeting";
    option.Aliases = {"a", "b", "c", "d"};

    App app;
    app.Name = appName;
    app.Usage = "fight the loneliness!";
    app.Commands = {
        command
    };
    app.Options = {
        option
    };

    app.Action = [](const Context& context) -> void
    {
        std::string name = "Nefertiti";
        if (context.Arguments.size() > 0)
        {
            name = context.Arguments[0];
        }
        if (context.GetOption("lang").Value == "spanish")
        {
            printf("%s %s\n", "HOLA", name.c_str());
        }
        else
        {
            printf("%s %s\n", "Hello", name.c_str());
        }
        printf("%s\n", context.ToString().c_str());
    };

    app.Run(argc, argv);
    return 0;
}