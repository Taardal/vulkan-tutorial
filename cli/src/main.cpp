#include "App.h"
#include "Command.h"
#include "Option.h"

using namespace VulkandemoCLI;

int main(int argc, char* argv[])
{
    auto* app = new App();
    Command* command = app->GetCommand(argc, argv);
    if (command != nullptr)
    {
        command->Execute(app->GetOptions(argc, argv));
    }
    delete app;
    return 0;
}