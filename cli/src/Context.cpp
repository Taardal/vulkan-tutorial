#include "Context.h"
#include "Flag.h"

namespace VulkandemoCLI
{
    bool Context::HasFlag(const std::string& name) const
    {
        for (const Flag& flag : Flags)
        {
            if (flag.Name == name)
            {
                return true;
            }
        }
        return false;
    }

    Flag Context::GetFlag(const std::string& name) const
    {
        for (const Flag& flag : Flags)
        {
            if (flag.Name == name)
            {
                return flag;
            }
        }
        return {};
    }
}