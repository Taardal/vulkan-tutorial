#include "Context.h"
#include "Option.h"
#include "Command.h"
#include <sstream>

namespace VulkandemoCLI
{
    bool Context::HasOption(const std::string& name) const
    {
        for (const Option& option : Options)
        {
            if (option.Name == name)
            {
                return true;
            }
        }
        return false;
    }

    Option Context::GetOption(const std::string& name) const
    {
        for (const Option& option : Options)
        {
            if (option.Name == name)
            {
                return option;
            }
        }
        return {};
    }

    std::string Context::ToString() const
    {
        std::stringstream ss;
        ss << "CONTEXT" << std::endl;
        ss << "  COMMAND:" << std::endl;
        ss << "    " << (Command != nullptr ? Command->Name : "[NULLPTR]") << std::endl;
        ss << "  FLAGS: " << Options.size() << std::endl;
        for (const Option& option : Options)
        {
            ss << "    " << option.Name.c_str();
            if (!option.Value.empty())
            {
                ss << " --> " << option.Value.c_str();
            }
            ss << std::endl;
        }
        ss << "  ARGUMENTS: " << Arguments.size() << std::endl;
        for (const std::string& argument : Arguments)
        {
            ss << "    " << argument.c_str() << std::endl;
        }
        return ss.str();
    }
}