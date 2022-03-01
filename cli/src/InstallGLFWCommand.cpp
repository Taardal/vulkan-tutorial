#include "InstallGLFWCommand.h"
#include <iostream>
#include <sstream>

namespace VulkandemoCLI
{
    Command CreateInstallGLFWCommand()
    {
        Command command;
        command.Name = "glfw";
        command.Usage = "Build and install GLFW";
        command.Action = [](const Context& context) -> void
        {
            const char* also = " && ";
            const char* buildType = "Debug";
            const char* buildDir = "build";

            std::stringstream ss;
            ss << "cd lib/glfw";
            ss << also;
            ss << "cmake -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=" << buildType << " -S . -B ./" << buildDir;
            ss << also;
            ss << "cmake --build ./" << buildDir << " --config " << buildType;
            ss << also;
            ss << "cmake --install ./" << buildDir << " --config " << buildType;

            std::string command = ss.str();
#ifdef VDC_DEBUG
            printf("%s\n", command.c_str());
#endif
            std::system(command.c_str());
        };
        return command;
    }
}
