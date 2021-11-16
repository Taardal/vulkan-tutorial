#include "App.h"
#include "Environment.h"

int main()
{
    Vulkandemo::App::Config config{};
    config.Name = "Vulkan Demo";
    config.Window.Title = config.Name;
    config.Window.Width = 800;
    config.Window.Height = 600;
    config.Vulkan.Name = config.Name;
#ifdef VD_DEBUG
    config.Vulkan.ValidationLayersEnabled = true;
#endif
    
    auto* app = new Vulkandemo::App(config);
    app->Run();
    delete app;

    return 0;
}