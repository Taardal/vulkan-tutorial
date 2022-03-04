#pragma once

#include "Command.h"
#include "Option.h"

namespace VulkandemoCLI
{
    Option CreateHelpOption();

    Command CreateHelpCommand();
}