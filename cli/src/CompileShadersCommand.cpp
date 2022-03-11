#include "CompileShadersCommand.h"
#include <sstream>

namespace VulkandemoCLI {

    CLI::Command createCompileShadersCommand() {
        CLI::Option releaseOption;
        releaseOption.Name = "release";
        releaseOption.Usage = "Use Release mode";
        releaseOption.Aliases = {"r"};

        CLI::Command command;
        command.Name = "shaders";
        command.Usage = "Compile shaders";
        command.Options = {
                releaseOption
        };
        command.Action = [](const CLI::Context& context) -> void {
            const char* assetsDirectoryName = "assets";
            const char* shaderDirectoryName = "shaders";

            const char* binDirectoryName = "bin";
            const char* buildTypeDirName = context.HasOption("release") ? "release" : "debug";

            const char* cmakeScriptsDirectoryName = "cmake";
            const char* cmakeScriptName = "compile_shaders.cmake";

            std::stringstream ss;
            ss << "cmake";
            ss << " -DSHADER_ASSETS_DIR=" << assetsDirectoryName << "/" << shaderDirectoryName;
            ss << " -DSHADER_OUTPUT_DIR=" << binDirectoryName << "/" << buildTypeDirName << "/" << shaderDirectoryName;
            ss << " -P " << cmakeScriptsDirectoryName << "/" << cmakeScriptName;
            std::string command = ss.str();

#ifdef VDC_DEBUG
            printf("%s\n", command.c_str());
#endif
            std::system(command.c_str());
        };
        return command;
    }
}
