#pragma once

#include "Environment.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef VD_PLATFORM_MACOS
    #define VD_FILENAME __FILE_NAME__
#else
    #define VD_FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

#define VD_FUNCTION_NAME __func__

#define VD_LINE_NUMBER __LINE__

#define VD_FORMAT_LOG_MESSAGE(message) ::vulkandemo::Log::FormatMessage(VD_FILENAME, VD_FUNCTION_NAME, VD_LINE_NUMBER, message)

#define VD_LOG_TRACE(message, ...) ::vulkandemo::Log::GetLogger()->trace(VD_FORMAT_LOG_MESSAGE(message), ##__VA_ARGS__)
#define VD_LOG_DEBUG(message, ...) ::vulkandemo::Log::GetLogger()->debug(VD_FORMAT_LOG_MESSAGE(message), ##__VA_ARGS__)
#define VD_LOG_INFO(message, ...) ::vulkandemo::Log::GetLogger()->info(VD_FORMAT_LOG_MESSAGE(message), ##__VA_ARGS__)
#define VD_LOG_WARN(message, ...) ::vulkandemo::Log::GetLogger()->warn(VD_FORMAT_LOG_MESSAGE(message), ##__VA_ARGS__)
#define VD_LOG_ERROR(message, ...) ::vulkandemo::Log::GetLogger()->error(VD_FORMAT_LOG_MESSAGE(message), ##__VA_ARGS__)
#define VD_LOG_CRITICAL(message, ...) ::vulkandemo::Log::GetLogger()->critical(VD_FORMAT_LOG_MESSAGE(message), ##__VA_ARGS__)

namespace vulkandemo
{
    class Log
    {
    public:
        static void Initialize(const std::string& name);

        static const std::shared_ptr<spdlog::logger>& GetLogger();

        static std::string FormatMessage(const char* filename, const char* functionName, uint32_t lineNumber, std::string_view message);

    private:
        static std::shared_ptr<spdlog::logger> logger;
    };
}