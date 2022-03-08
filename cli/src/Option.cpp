#include "Option.h"

namespace VulkandemoCLI
{
    std::string_view Option::GetValue() const
    {
        return Value.length() > 0 ? Value : DefaultValue.length() > 0 ? DefaultValue : "";
    }
}