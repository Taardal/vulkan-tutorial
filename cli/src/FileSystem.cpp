#include "FileSystem.h"
#include <fstream>

namespace VulkandemoCLI
{
    std::string FileSystem::ReadFile(const char* path) const
    {
        std::string result;
        std::ifstream inputStream(path, std::ios::in | std::ios::binary);
        if (inputStream)
        {
            inputStream.seekg(0, std::ios::end);
            size_t length = inputStream.tellg();
            if (length != -1)
            {
                result.resize(length);
                inputStream.seekg(0, std::ios::beg);
                inputStream.read(&result[0], length);
                inputStream.close();
            }
            else
            {
                printf("Could not read from file [%s]\n", path);
            }
        }
        else
        {
            printf("Could not open file [%s]\n", path);
        }
        return result;
    }
}
