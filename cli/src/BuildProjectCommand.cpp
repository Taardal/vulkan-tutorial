#include "BuildProjectCommand.h"
#include "Environment.h"
#include <iostream>
#include <sstream>

namespace VulkandemoCLI
{
    Command CreateBuildProjectCommand()
    {
        Command command;
        command.Name = "build";
        command.Usage = "Build project";
        command.Action = [](const Context& context) -> void
        {
            const char* also = " && ";
            const char* buildType = "Debug";
            const char* buildDir = "cmake-build-debug";

            std::stringstream ss;
            ss << "cmake -DCMAKE_BUILD_TYPE=" << buildType << " -S . -B " << buildDir;
            ss << also;
            ss << "cmake --build " << buildDir << " --config " << buildType;

            std::string command = ss.str();
#ifdef VDC_DEBUG
            printf("%s\n", command.c_str());
#endif
            std::system(command.c_str());
        };
        return command;
    }
}