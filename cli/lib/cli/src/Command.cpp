#include "Command.h"

namespace CLI {

    bool Command::hasOption(std::string_view name) const {
        for (const Option& option: Options) {
            if (option.Name == name) {
                return true;
            }
        }
        return false;
    }

    const Option* Command::getOption(std::string_view name) const {
        for (const Option& option: Options) {
            if (option.Name == name) {
                return &option;
            }
        }
        return nullptr;
    }
}