#include "App.h"

int main()
{
    vulkandemo::App::Config config{};
    config.Name = "Vulkan Demo";
    config.Window.Title = config.Name;
    config.Window.Width = 800;
    config.Window.Height = 600;
    config.Graphics.Name = config.Name;
    
    vulkandemo::App* app = new vulkandemo::App(config);
    app->Run();
    delete app;

    return 0;
}