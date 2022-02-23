#include "BuildProjectCommand.h"
#include "Environment.h"
#include <iostream>
#include <sstream>

namespace VulkandemoCLI
{
    const char* BuildProjectCommand::NAME = "build";
    const char* BuildProjectCommand::DESCRIPTION = "Build project";

    const char* BuildProjectCommand::GetName() const
    {
        return NAME;
    }

    const char* BuildProjectCommand::GetDescription() const
    {
        return DESCRIPTION;
    }

    void BuildProjectCommand::Execute(const std::vector<Option>& options) const
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
    }
}