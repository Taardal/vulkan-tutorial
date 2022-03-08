#include "RunProjectCommand.h"
#include <sstream>

namespace VulkandemoCLI
{
    CLI::Command CreateRunProjectCommand()
    {
        CLI::Option buildOption;
        buildOption.Name = "build";
        buildOption.Usage = "Build project before running";
        buildOption.Aliases = {"b"};

        CLI::Option releaseOption;
        releaseOption.Name = "release";
        releaseOption.Usage = "Use Release mode";
        releaseOption.Aliases = {"r"};

        CLI::Option glfwOption;
        glfwOption.Name = "glfw";
        glfwOption.Usage = "Build GLFW as part of this project (Requires build option)";
        glfwOption.Aliases = {"g"};

        CLI::Command command;
        command.Name = "run";
        command.Usage = "Run project";
        command.Options = {
                buildOption,
                releaseOption,
                glfwOption
        };
        command.Action = [](const CLI::Context& context) -> void
        {
            const char* binDirName = "bin";
            const char* buildType = context.HasOption("release") ? "Release" : "Debug";
            const char* buildTypeDirName = context.HasOption("release") ? "release" : "debug";

            if (context.HasOption("build"))
            {
                std::stringstream ss;
                ss << "./" << context.App->Name << " build --buildType=" << buildType;
                if (context.HasOption("glfw"))
                {
                    ss << " --glfw";
                }
                std::string buildCommand = ss.str();
                std::system(buildCommand.c_str());
            }

            std::stringstream ss;
            ss << binDirName << "/" << buildTypeDirName;
            std::string path = ss.str();

            std::stringstream ss1;
            ss1 << "cd " << path << " && " << "./vulkandemo";
            std::string command = ss1.str();
            printf("%s\n", command.c_str());
            std::system(command.c_str());
        };
        return command;
    }
}