#include "Command.h"

namespace VulkandemoCLI
{
    bool Command::HasOption(std::string_view name) const
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

    const Option* Command::GetOption(std::string_view name) const
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
}