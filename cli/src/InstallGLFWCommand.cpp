#include "InstallGLFWCommand.h"
#include <cli.h>
#include <iostream>
#include <sstream>

namespace VulkandemoCLI
{
    CLI::Command CreateInstallGLFWCommand()
    {
        CLI::Option buildTypeOption;
        buildTypeOption.Name = "buildType";
        buildTypeOption.Usage = "Which build type to use. Must be Debug or Release";
        buildTypeOption.DefaultValue = "Release";
        buildTypeOption.Aliases = {"b"};

        CLI::Option buildDirectoryOption;
        buildDirectoryOption.Name = "buildDir";
        buildDirectoryOption.Usage = "Where to store build files generated with CMake";
        buildDirectoryOption.DefaultValue = "build";
        buildDirectoryOption.Aliases = {"d"};

        CLI::Option cmakeSourceDirectoryOption;
        cmakeSourceDirectoryOption.Name = "cmakeDir";
        cmakeSourceDirectoryOption.Usage = "Where the CMake source files are located (CMakeLists.txt)";
        cmakeSourceDirectoryOption.DefaultValue = ".";
        cmakeSourceDirectoryOption.Aliases = {"c"};

        CLI::Command command;
        command.Name = "glfw";
        command.Usage = "Install GLFW on local machine";
        command.Options = {
                buildTypeOption,
                buildDirectoryOption,
                cmakeSourceDirectoryOption
        };
        command.Action = [](const CLI::Context& context) -> void
        {
            const char* also = " && ";

            const CLI::Option* buildTypeOption = context.GetOption("buildType");
            std::string_view buildType;
            if (buildTypeOption != nullptr)
            {
                buildType = buildTypeOption->GetValue();
            } else
            {
                buildType = context.Command->GetOption("buildType")->DefaultValue;
            }

            const CLI::Option* buildDirectoryOption = context.GetOption("buildDir");
            std::string buildDirectory;
            if (buildDirectoryOption != nullptr)
            {
                buildDirectory = buildDirectoryOption->Value;
            }
            else
            {
                buildDirectory = context.Command->GetOption("buildDir")->DefaultValue;
            }

            const CLI::Option* cmakeSourceDirectoryOption = context.GetOption("cmakeDir");
            std::string_view cmakeSourceDirectory;
            if (cmakeSourceDirectoryOption != nullptr)
            {
                cmakeSourceDirectory = cmakeSourceDirectoryOption->GetValue();
            } else
            {
                cmakeSourceDirectory = context.Command->GetOption("cmakeDir")->DefaultValue;
            }

            std::stringstream ss;
            ss << "cd lib/glfw";
            ss << also;
            ss << "cmake -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=" << buildType << " -B " << buildDirectory << " -S " << cmakeSourceDirectory;
#ifdef VDC_PLATFORM_WINDOWS
            ss << " -A x64";
#endif
            ss << also;
            ss << "cmake --build " << buildDirectory << " --config " << buildType;
            ss << also;
            ss << "cmake --install " << buildDirectory << " --config " << buildType;

            std::string command = ss.str();
#ifdef VDC_DEBUG
            printf("%s\n", command.c_str());
#endif
            std::system(command.c_str());
        };
        return command;
    }
}
