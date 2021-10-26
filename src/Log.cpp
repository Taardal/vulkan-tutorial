#include "Log.h"
#include <sstream>

namespace vulkandemo
{
    std::shared_ptr<spdlog::logger> Log::logger;

    void Log::Initialize(const std::string& name)
    {
        logger = spdlog::stdout_color_mt(name);
        logger->set_pattern("%^[%Y-%m-%d] [%T] [%n] [%l] %v%$");
        logger->set_level(spdlog::level::trace);
    }

    const std::shared_ptr<spdlog::logger>& Log::GetLogger()
    {
        return logger;
    }

    std::string Log::FormatMessage(const char* filename, const char* functionName, uint32_t lineNumber, std::string_view message)
    {
        std::stringstream ss;
        ss << "[" << filename << ":" << functionName << ":" << lineNumber << "] - " << message;
        return ss.str();
    }
}