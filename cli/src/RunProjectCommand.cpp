#include "RunProjectCommand.h"
#include "Environment.h"
#include <sstream>

namespace VulkandemoCLI {

    CLI::Command createRunProjectCommand() {
        CLI::Option buildOption;
        buildOption.Name = "build";
        buildOption.Usage = "Build project before running";
        buildOption.Aliases = {"b"};

        CLI::Option glfwOption;
        glfwOption.Name = "glfw";
        glfwOption.Usage = "Build GLFW as part of this project (Requires build option)";
        glfwOption.Aliases = {"g"};

        CLI::Option releaseOption;
        releaseOption.Name = "release";
        releaseOption.Usage = "Use Release mode";
        releaseOption.Aliases = {"r"};

        CLI::Command command;
        command.Name = "run";
        command.Usage = "Run project";
        command.Options = {
                buildOption,
                glfwOption,
                releaseOption,
        };
        command.Action = [](const CLI::Context& context) -> void {
            const char* binDirName = "bin";
            const char* buildTypeDirName = context.hasOption("release") ? "release" : "debug";

            std::stringstream ss;
            if (context.hasOption("build")) {
                ss << "./" << context.App->Name << " build";
                if (context.hasOption("release")) {
                    ss << " --release";
                }
                if (context.hasOption("glfw")) {
                    ss << " --glfw";
                }
                ss << " && ";
            }
            ss << "cd " << binDirName << "/" << buildTypeDirName;
            ss << " && ";
            ss << "./vulkandemo";
            std::string command = ss.str();

#ifdef VDC_DEBUG
            printf("%s\n", command.c_str());
#endif
            std::system(command.c_str());
        };
        return command;
    }
}