#include "Command.h"
#include "Context.h"
#include "Option.h"

namespace CLI {

    bool Context::hasOption(std::string_view name) const {
        for (const Option& option: Options) {
            if (option.Name == name) {
                return true;
            }
        }
        return false;
    }

    const Option* Context::getOption(std::string_view name) const {
        for (const Option& option: Options) {
            if (option.Name == name) {
                return &option;
            }
        }
        return nullptr;
    }
}