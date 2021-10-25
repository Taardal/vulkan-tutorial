#include "FileSystem.h"
#include <fstream>

namespace vulkandemo
{
    std::vector<char> FileSystem::ReadBinaryFile(const char* path) const
    {
        std::ifstream file{path, std::ios::ate | std::ios::binary};
        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }
}
