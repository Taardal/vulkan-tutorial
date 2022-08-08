#pragma once

#include "FileSystem.h"

#include <cli.h>

namespace VulkandemoCLI {
    CLI::Command createAddLibraryCommand(const FileSystem& fileSystem);
}
