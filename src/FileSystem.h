#pragma once

#include <vector>

namespace Vulkandemo {

    class FileSystem {
    public:
        std::vector<char> readBytes(const char* path) const;
    };
}
