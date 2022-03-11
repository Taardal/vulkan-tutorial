#pragma once

#include "FileSystem.h"
#include <cli.h>

namespace VulkandemoCLI
{
    CLI::Command createInstallDependenciesCommand(const FileSystem& fileSystem);
}
