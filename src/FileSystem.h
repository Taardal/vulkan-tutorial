#pragma once

#include <vector>

namespace vulkandemo
{
    class FileSystem
    {
    public:
        std::vector<char> ReadBinaryFile(const char* path) const;
    };
}
