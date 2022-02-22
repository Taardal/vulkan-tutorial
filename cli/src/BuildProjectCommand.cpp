#include "BuildProjectCommand.h"
#include <iostream>
#include <sstream>

namespace VulkandemoCLI
{
    void BuildProjectCommand::Execute() const
    {
        printf("Building application...");

        const char* also = " && ";
        const char* buildType = "Debug";
        const char* buildDir = "cmake-build-debug";

        std::stringstream ss;
        ss << "cmake -DCMAKE_BUILD_TYPE=" << buildType << " -S . -B " << buildDir;
        ss << also;
        ss << "cmake --build " << buildDir << " --config " << buildType;

        std::string cmd = ss.str();
        printf("%s\n", cmd.c_str());
        //std::system(cmd.c_str());
    }
}