#include "InstallGLFWCommand.h"
#include <iostream>
#include <sstream>

namespace VulkandemoCLI
{
    const char* InstallGLFWCommand::NAME = "glfw";
    const char* InstallGLFWCommand::DESCRIPTION = "Install GLFW";

    const char* InstallGLFWCommand::GetName() const
    {
        return NAME;
    }

    const char* InstallGLFWCommand::GetDescription() const
    {
        return DESCRIPTION;
    }

    void InstallGLFWCommand::Execute(const std::vector<Option>& options) const
    {
        printf("Building and installing GLFW...");

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
        printf("%s\n", command.c_str());
        std::system(command.c_str());
    }
}
