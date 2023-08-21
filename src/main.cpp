#include "App.h"
#include "Environment.h"
#include "Log.h"

int main()
{
    Vulkandemo::App::Config config{};
    config.Name = "Vulkandemo";
    config.LogLevel = Vulkandemo::Log::Level::Debug;
    config.Window.Title = config.Name;
    config.Window.Width = 800;
    config.Window.Height = 600;
    config.Vulkan.Name = config.Name;
#ifdef VD_BUILD_DEBUG
    config.Vulkan.ValidationLayersEnabled = true;
#endif

    auto* app = new Vulkandemo::App(config);
    app->run();
    delete app;
}