#include "Flag.h"
#include <sstream>

namespace VulkandemoCLI
{
    std::string Flag::NameWithPrefix() const
    {
        std::stringstream ss;
        ss << "--" << Name;
        return ss.str();
    }
}