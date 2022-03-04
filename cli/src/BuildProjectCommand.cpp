#include "BuildProjectCommand.h"
#include "Environment.h"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace VulkandemoCLI
{
    Command CreateBuildProjectCommand()
    {
        Option buildTypeOption;
        buildTypeOption.Name = "buildType";
        buildTypeOption.Aliases = {"b"};

        Command command;
        command.Name = "build";
        command.Usage = "Build project";
        command.Options = {
                buildTypeOption
        };
        command.Action = [](const Context& context) -> void
        {
            if (context.HasOption("buildType"))
            {
                printf("%s\n", "NOTICE ME");
                printf("%s\n", "NOTICE ME");
                printf("%s\n", "NOTICE ME");
            }

            std::string_view buildTypeOptionValue = context.GetOption("buildType").Value;
            std::string_view buildType = buildTypeOptionValue.size() > 0 ? buildTypeOptionValue : "Debug";

            std::string buildDir(buildType);
            std::transform(buildDir.begin(), buildDir.end(), buildDir.begin(), ::tolower);
            std::stringstream ss1;
            ss1 << "cmake-build-" << buildDir;
            buildDir = ss1.str();

            std::stringstream ss;
            ss << "cmake -DCMAKE_BUILD_TYPE=" << buildType << " -S . -B " << buildDir;
            ss << " && ";
            ss << "cmake --build " << buildDir << " --config " << buildType;

            const std::string& command = ss.str();
#ifdef VDC_DEBUG
            printf("%s\n", command.c_str());
#endif
            //std::system(command.c_str());
        };
        return command;
    }
}