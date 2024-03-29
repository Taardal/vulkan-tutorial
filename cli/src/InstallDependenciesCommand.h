#pragma once

#include "FileSystem.h"
#include <cli.h>

namespace VulkandemoCLI {
    CLI::Command createInstallLibrariesCommand(const FileSystem& fileSystem);
}
