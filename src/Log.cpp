#include "Log.h"
#include <sstream>

namespace vulkandemo
{
    std::shared_ptr<spdlog::logger> Log::logger;

    void Log::Initialize(const std::string& name)
    {
        logger = spdlog::stdout_color_mt(name);
        logger->set_pattern("%^[%T] [%n] [thread %t] [%l] %v%$");
        logger->set_level(spdlog::level::trace);
    }

    std::string Tag(const char* filename, const char* functionName, uint32_t lineNumber)
    {
        std::stringstream ss;
        ss << filename << ":" << functionName << ":" << lineNumber;
        return ss.str();
    }

    std::string Log::Format(const char* tag, const char* message)
    {
        std::stringstream ss;
        ss << "[" << tag << "] - " << message;
        return ss.str();
    }
}