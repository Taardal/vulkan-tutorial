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

    Flag cmdFlag;
    cmdFlag.Name = "bar";
    cmdFlag.Usage = "bar flag";

    Command command;
    command.Name = "foo";
    command.Usage = "The Foo, The Bar and the WTF";
    command.Flags = {
            cmdFlag
    };
    command.Action = [](const Context& context) -> void
    {
        printf("%s\n", "The Foo, The Bar and the WTF");
        printf("%s\n", context.ToString().c_str());
    };

    Flag flag;
    flag.Name = "lang";
    flag.Usage = "Language for the greeting";
    flag.Aliases = {"a", "b", "c", "d"};

    App app;
    app.Name = appName;
    app.Usage = "fight the loneliness!";
    app.Commands = {
        command
    };
    app.Flags = {
        flag
    };

    app.Action = [](const Context& context) -> void
    {
        std::string name = "Nefertiti";
        if (context.Arguments.size() > 0)
        {
            name = context.Arguments[0];
        }
        if (context.GetFlag("lang").Value == "spanish")
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