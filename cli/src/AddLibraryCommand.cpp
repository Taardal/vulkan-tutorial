#include "AddLibraryCommand.h"
#include "Environment.h"
#include <sstream>

namespace VulkandemoCLI {

    CLI::Command createAddLibraryCommand(const FileSystem& fileSystem) {
        CLI::Option urlOption;
        urlOption.Name = "url";
        urlOption.Usage = "Git submodule URL --> https://github.com/foo/bar.git";
        urlOption.Aliases = {"u"};

        CLI::Option nameOption;
        nameOption.Name = "name";
        nameOption.Usage = "Git submodule name --> ./lib/<name>";
        nameOption.Aliases = {"n"};

        CLI::Option versionOption;
        versionOption.Name = "version";
        versionOption.Usage = "Git tag or commit to checkout --> 1.2.3";
        versionOption.Aliases = {"v"};
        versionOption.DefaultValue = "latest";

        CLI::Command command;
        command.Name = "lib:add";
        command.Usage = "Add library (git submodule)";
        command.Options = {
                urlOption,
                nameOption,
                versionOption
        };
        command.Action = [&](const CLI::Context& context) -> void {
            const CLI::Option* urlOption = context.getOption("url");
            assert(urlOption != nullptr);

            const std::string_view& url = urlOption->Value;
            assert(!url.empty());
            
            const CLI::Option* nameOption = context.getOption("name");
            assert(nameOption != nullptr);

            const std::string_view& name = nameOption->Value;
            assert(!name.empty());

            const CLI::Option* versionOption = context.getOption("version");
            const std::string_view& version = versionOption != nullptr ? versionOption->getValueOrDefault() : "latest";

            printf("%s%s%s%s%s\n", "-- Adding submodule [", name.data(), "] by URL [", url.data(), "]");
            
            std::stringstream ss;
            ss << "git submodule add " << url << " ./lib/" << name;
            std::string command = ss.str();

#ifdef VDC_DEBUG
            printf("%s\n", command.c_str());
#endif
            std::system(command.c_str());

            std::stringstream ss1;
            ss1 << "\n" << name << "=" << version;
            std::string libraryFileEntry = ss1.str();
            fileSystem.appendTextToFile(libraryFileEntry, ".dependencies");

            if (version != "latest") {
                printf("%s%s%s%s%s\n", "-- Checking out submodule [", name.data(), "] to version [", version.data(), "]");
                std::stringstream ss2;
                ss2 << "cd lib/" << name << " && git checkout " << version;
                std::string checkoutVersionCommand = ss2.str();
#ifdef VDC_DEBUG
                printf("%s\n", checkoutVersionCommand.c_str());
#endif
                std::system(checkoutVersionCommand.c_str());
            }

            printf("\n");
        };
        return command;
    }
}
