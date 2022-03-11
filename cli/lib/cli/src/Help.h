#pragma once

#include "Command.h"
#include "Option.h"

namespace CLI
{
    Option createHelpOption();

    Command createHelpCommand();
}