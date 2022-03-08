#pragma once

#include "FileSystem.h"
#include <cli.h>

namespace VulkandemoCLI
{
    CLI::Command CreateInstallDependenciesCommand(const FileSystem& fileSystem);
}
