#pragma once

#include <vector>

namespace Vulkandemo {

    class FileSystem {
    public:
        std::vector<char> readBinaryFile(const char* path) const;
    };
}
