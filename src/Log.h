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

#define VD_TAG ::vulkandemo::Log::Tag(VD_FILENAME, VD_FUNCTION_NAME, VD_LINE_NUMBER)

#define VD_TRACE(message, ...) ::vulkandemo::Log::GetLogger()->trace(::vulkandemo::Log::Format(VD_TAG, message), ##__VA_ARGS__)
#define VD_DEBUG(message, ...) ::vulkandemo::Log::GetLogger()->debug(::vulkandemo::Log::Format(VD_TAG, message), ##__VA_ARGS__)
#define VD_INFO(message, ...) ::vulkandemo::Log::GetLogger()->info(::vulkandemo::Log::Format(VD_TAG, message), ##__VA_ARGS__)
#define VD_WARN(message, ...) ::vulkandemo::Log::GetLogger()->warn(::vulkandemo::Log::Format(VD_TAG, message), ##__VA_ARGS__)
#define VD_ERROR(message, ...) ::vulkandemo::Log::GetLogger()->error(::vulkandemo::Log::Format(VD_TAG, message), ##__VA_ARGS__)
#define VD_CRITICAL(message, ...) ::vulkandemo::Log::GetLogger()->critical(::vulkandemo::Log::Format(VD_TAG, message), ##__VA_ARGS__)

namespace vulkandemo
{
    class Log
    {
    public:
        static void Initialize(const std::string& name);

        static std::shared_ptr<spdlog::logger>& GetLogger() { return logger; };

        static std::string Tag(const char* filename, const char* functionName, uint32_t lineNumber);

        static std::string Format(const char* tag, const char* message);

    private:
        static std::shared_ptr<spdlog::logger> logger;
    };
}