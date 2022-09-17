#include "InstallDependenciesCommand.h"
#include "Environment.h"
#include <sstream>

namespace VulkandemoCLI {

    CLI::Command createInstallLibrariesCommand(const FileSystem& fileSystem) {
        CLI::Command command;
        command.Name = "lib:install";
        command.Usage = "Install libraries (git submodules)";
        command.Action = [&](const CLI::Context& context) -> void {
            std::system("git submodule update --init");
            printf("\n");

            std::istringstream iss(fileSystem.readFile(".dependencies"));
            for (std::string line; std::getline(iss, line);) {
                if (line.empty() || line[0] == '#') {
                    continue;
                }
                int equalSignPosition = (int) line.find('=');
                const std::string& name = line.substr(0, equalSignPosition);
                const std::string& version = line.substr(equalSignPosition + 1, line.length());
                if (version != "latest") {
                    printf("%s%s%s%s%s\n", "-- Using [", name.c_str(), "] version [", version.c_str(), "]");
                    std::stringstream ss;
                    ss << "cd lib/" << name << " && git checkout " << version;
                    std::string command = ss.str();
#ifdef VDC_DEBUG
                    printf("%s\n", command.c_str());
#endif
                    std::system(command.c_str());
                }
                printf("\n");
            }
            printf("\n");

        };
        return command;
    }
}
