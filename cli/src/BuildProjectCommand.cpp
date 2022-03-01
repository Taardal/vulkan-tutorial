#include "BuildProjectCommand.h"
#include "Environment.h"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace VulkandemoCLI
{
    Command CreateBuildProjectCommand()
    {
        Flag buildTypeFlag;
        buildTypeFlag.Name = "buildType";
        buildTypeFlag.Aliases = {"b"};

        Command command;
        command.Name = "build";
        command.Usage = "Build project";
        command.Flags = {
                buildTypeFlag
        };
        command.Action = [](const Context& context) -> void
        {
            if (context.HasFlag("buildType"))
            {
                printf("%s\n", "NOTICE ME");
                printf("%s\n", "NOTICE ME");
                printf("%s\n", "NOTICE ME");
            }

            const std::string& buildTypeFlagValue = context.GetFlag("buildType").Value;
            const std::string& buildType = buildTypeFlagValue.size() > 0 ? buildTypeFlagValue : "Debug";

            std::string buildDir = buildType;
            std::transform(buildDir.begin(), buildDir.end(), buildDir.begin(), ::tolower);
            std::stringstream ss1;
            ss1 << "cmake-build-" << buildDir;
            buildDir = ss1.str();

            std::stringstream ss;
            ss << "cmake -DCMAKE_BUILD_TYPE=" << buildType << " -S . -B " << buildDir;
            ss << " && ";
            ss << "cmake --build " << buildDir << " --config " << buildType;

            std::string command = ss.str();
#ifdef VDC_DEBUG
            printf("%s\n", command.c_str());
#endif
            //std::system(command.c_str());
        };
        return command;
    }
}