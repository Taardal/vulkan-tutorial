#include "Context.h"
#include "Flag.h"
#include "Command.h"
#include <sstream>

namespace VulkandemoCLI
{
    std::string Context::ToString() const
    {
        std::stringstream ss;

        ss << "CONTEXT" << std::endl;
        ss << "  COMMAND:" << std::endl;
        ss << "    " << (Command != nullptr ? Command->Name : "[EMPTY]") << std::endl;
        ss << "  FLAGS: " << Flags.size() << std::endl;
        for (const Flag& flag : Flags)
        {
            ss << "    " << flag.Name.c_str();
            if (!flag.Value.empty())
            {
                ss << " --> " << flag.Value.c_str();
            }
            ss << std::endl;
        }
        ss << "  ARGS: " << Arguments.size() << std::endl;
        for (const std::string& argument : Arguments)
        {
            ss << "    " << argument.c_str() << std::endl;
        }

        return ss.str();
    }

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