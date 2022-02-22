#include "App.h"
#include "Command.h"
#include "Environment.h"

using namespace VulkandemoCLI;

int main(int argc, char* argv[])
{
    auto* app = new App();

#ifdef VD_DEBUG
    app->PrintInput(argc, argv);
#endif

    Command* command = app->GetCommand(argc, argv);
    if (command != nullptr)
    {
        command->Execute();
    }
    else
    {
        app->PrintHelp();
    }

    delete app;
    return 0;
}