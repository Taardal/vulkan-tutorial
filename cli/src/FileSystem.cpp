#include "FileSystem.h"
#include <fstream>

namespace VulkandemoCLI {

    std::string FileSystem::readFile(std::string_view path) const {
        std::ifstream inputStream(path, std::ios::in | std::ios::binary);
        if (!inputStream) {
            printf("Could not open file [%s]\n", path.data());
            return "";
        }
        inputStream.seekg(0, std::ios::end);
        std::streamsize length = inputStream.tellg();
        if (length == -1) {
            printf("Could not read from file [%s]\n", path.data());
            return "";
        }
        std::string result;
        result.resize(length);
        inputStream.seekg(0, std::ios::beg);
        inputStream.read(&result[0], length);
        inputStream.close();
        return result;
    }
}
