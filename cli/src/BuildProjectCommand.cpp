#include "BuildProjectCommand.h"
#include "Environment.h"
#include <cli.h>
#include <iostream>
#include <sstream>
#include <algorithm>

namespace VulkandemoCLI {

    CLI::Command createBuildProjectCommand() {
        CLI::Option buildDirectoryOption;
        buildDirectoryOption.Name = "buildDir";
        buildDirectoryOption.Usage = "Where to store build files generated with CMake";
        buildDirectoryOption.DefaultValue = "build";
        buildDirectoryOption.Aliases = {"d"};

        CLI::Option cmakeSourceDirectoryOption;
        cmakeSourceDirectoryOption.Name = "cmakeDir";
        cmakeSourceDirectoryOption.Usage = "Where the CMake source files are located";
        cmakeSourceDirectoryOption.DefaultValue = ".";
        cmakeSourceDirectoryOption.Aliases = {"c"};

        CLI::Option glfwOption;
        glfwOption.Name = "glfw";
        glfwOption.Usage = "Build GLFW as part of this project instead of using binaries installed on local machine";
        glfwOption.Aliases = {"g"};

        CLI::Option releaseOption;
        releaseOption.Name = "release";
        releaseOption.Usage = "Use Release mode";
        releaseOption.Aliases = {"r"};

        CLI::Command command;
        command.Name = "build";
        command.Usage = "Build project";
        command.Options = {
                buildDirectoryOption,
                cmakeSourceDirectoryOption,
                glfwOption,
                releaseOption
        };
        command.Action = [](const CLI::Context& context) -> void {
            const char* buildType = context.HasOption("release") ? "Release" : "Debug";

            const CLI::Option* buildDirectoryOption = context.GetOption("buildDir");
            std::string buildDirectory;
            if (buildDirectoryOption != nullptr && buildDirectoryOption->Value.length() > 0) {
                buildDirectory = buildDirectoryOption->Value;
            } else {
                std::string buildTypeCopy(buildType);
                std::transform(buildTypeCopy.begin(), buildTypeCopy.end(), buildTypeCopy.begin(), ::tolower);
                buildDirectory = "cmake-build-" + buildTypeCopy;
            }

            const CLI::Option* cmakeSourceDirectoryOption = context.GetOption("cmakeDir");
            std::string_view cmakeSourceDirectory;
            if (cmakeSourceDirectoryOption != nullptr) {
                cmakeSourceDirectory = cmakeSourceDirectoryOption->GetValue();
            } else {
                cmakeSourceDirectory = context.Command->GetOption("cmakeDir")->DefaultValue;
            }

            std::stringstream ss;
            ss << "cmake -DCMAKE_BUILD_TYPE=" << buildType;
            if (context.HasOption("glfw")) {
                ss << " -DBUILD_GLFW_SRC=ON";
            }
            ss << " -B " << buildDirectory;
            ss << " -S " << cmakeSourceDirectory;
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