#pragma once

#include "Command.h"
#include "FileSystem.h"

namespace VulkandemoCLI
{
    Command CreateInstallDependenciesCommand(const FileSystem& fileSystem);
}
