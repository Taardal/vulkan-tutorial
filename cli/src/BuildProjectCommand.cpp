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
        buildTypeOption.Usage = "Which build type to use. Must be Debug or Release";
        buildTypeOption.DefaultValue = "Debug";
        buildTypeOption.Aliases = {"b"};

        Option buildDirectoryOption;
        buildDirectoryOption.Name = "buildDir";
        buildDirectoryOption.Usage = "Where to store build files generated with CMake";
        buildDirectoryOption.DefaultValue = "build";
        buildDirectoryOption.Aliases = {"d"};

        Option cmakeSourceDirectoryOption;
        cmakeSourceDirectoryOption.Name = "cmakeDir";
        cmakeSourceDirectoryOption.Usage = "Where the CMake source files are located";
        cmakeSourceDirectoryOption.DefaultValue = ".";
        cmakeSourceDirectoryOption.Aliases = {"c"};

        Command command;
        command.Name = "build";
        command.Usage = "Build project";
        command.Options = {
                buildTypeOption,
                buildDirectoryOption,
                cmakeSourceDirectoryOption
        };
        command.Action = [](const Context& context) -> void
        {
            const Option* buildTypeOption = context.GetOption("buildType");
            std::string_view buildType;
            if (buildTypeOption != nullptr)
            {
                buildType = buildTypeOption->GetValue();
            } else
            {
                buildType = context.Command->GetOption("buildType")->DefaultValue;
            }

            const Option* buildDirectoryOption = context.GetOption("buildDir");
            std::string buildDirectory;
            if (buildDirectoryOption != nullptr && buildDirectoryOption->Value.length() > 0)
            {
                buildDirectory = buildDirectoryOption->Value;
            }
            else
            {
                std::string buildTypeCopy(buildType);
                std::transform(buildTypeCopy.begin(), buildTypeCopy.end(), buildTypeCopy.begin(), ::tolower);
                buildDirectory =  "cmake-build-" + buildTypeCopy;
            }

            const Option* cmakeSourceDirectoryOption = context.GetOption("cmakeDir");
            std::string_view cmakeSourceDirectory;
            if (cmakeSourceDirectoryOption != nullptr)
            {
                cmakeSourceDirectory = cmakeSourceDirectoryOption->GetValue();
            } else
            {
                cmakeSourceDirectory = context.Command->GetOption("cmakeDir")->DefaultValue;
            }

            std::stringstream ss;
            ss << "cmake -DCMAKE_BUILD_TYPE=" << buildType << " -B " << buildDirectory << " -S " << cmakeSourceDirectory;
#ifdef VDC_PLATFORM_WINDOWS
            ss << " -A x64";
#endif
            ss << " && ";
            ss << "cmake --build " << buildDirectory << " --config " << buildType;

            std::string command = ss.str();
#ifdef VDC_DEBUG
            printf("%s\n", command.c_str());
#endif
            std::system(command.c_str());
        };
        return command;
    }
}