#pragma once

#include <vector>

namespace VulkandemoCLI
{
    class App;
    class Command;

    struct Context
    {
        const App* App;
        const Command* Command;
    };
}