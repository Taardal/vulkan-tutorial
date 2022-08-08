#include "Option.h"

namespace CLI {

    std::string_view Option::getValueOrDefault() const {
        return Value.length() > 0 ? Value : DefaultValue.length() > 0 ? DefaultValue : "";
    }
}