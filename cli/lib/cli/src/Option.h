#pragma once

#include <string>
#include <vector>

namespace CLI {

    struct Option {
        std::string_view Name;
        std::string_view Usage;
        std::string_view Value;
        std::string_view DefaultValue;
        std::vector<std::string_view> Aliases;

        std::string_view getValueOrDefault() const;
    };
}
