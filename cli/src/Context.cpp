#include "Context.h"
#include "Option.h"
#include "Command.h"
#include <sstream>

namespace VulkandemoCLI
{
    bool Context::HasOption(std::string_view name) const
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

    const Option* Context::GetOption(std::string_view name) const
    {
        for (const Option& option : Options)
        {
            if (option.Name == name)
            {
                return &option;
            }
        }
        return nullptr;
    }

    const Option* Context::GetOptionOrDefault(std::string_view name) const
    {
        const Option* option = GetOption(name);
        if (option == nullptr && Command != nullptr)
        {
            for (const Option& commandOption : Command->Options)
            {
                if (commandOption.Name == name)
                {
                    return &commandOption;
                }
            }
        }
        return nullptr;
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
            ss << "    " << option.Name.data();
            if (!option.Value.empty())
            {
                ss << " --> " << option.Value.data();
            }
            ss << std::endl;
        }
        ss << "  ARGUMENTS: " << Arguments.size() << std::endl;
        for (std::string_view argument : Arguments)
        {
            ss << "    " << argument << std::endl;
        }
        return ss.str();
    }
}