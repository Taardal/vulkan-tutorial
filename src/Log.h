#pragma once

#include "Environment.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifndef __FILE_NAME__
    #define __FILE_NAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

#define VD_FILENAME __FILE_NAME__
#define VD_FUNCTION_NAME __func__
#define VD_LINE_NUMBER __LINE__

#define VD_FORMAT_LOG_MESSAGE(message) ::Vulkandemo::Log::formatMessage(VD_FILENAME, VD_FUNCTION_NAME, VD_LINE_NUMBER, message)

#define VD_LOG_TRACE(message, ...) ::Vulkandemo::Log::getLogger()->trace(VD_FORMAT_LOG_MESSAGE(message), ##__VA_ARGS__)
#define VD_LOG_DEBUG(message, ...) ::Vulkandemo::Log::getLogger()->debug(VD_FORMAT_LOG_MESSAGE(message), ##__VA_ARGS__)
#define VD_LOG_INFO(message, ...) ::Vulkandemo::Log::getLogger()->info(VD_FORMAT_LOG_MESSAGE(message), ##__VA_ARGS__)
#define VD_LOG_WARN(message, ...) ::Vulkandemo::Log::getLogger()->warn(VD_FORMAT_LOG_MESSAGE(message), ##__VA_ARGS__)
#define VD_LOG_ERROR(message, ...) ::Vulkandemo::Log::getLogger()->error(VD_FORMAT_LOG_MESSAGE(message), ##__VA_ARGS__)
#define VD_LOG_CRITICAL(message, ...) ::Vulkandemo::Log::getLogger()->critical(VD_FORMAT_LOG_MESSAGE(message), ##__VA_ARGS__)

namespace Vulkandemo {

    class Log {
    public:
        static void initialize(const std::string& name);

        static const std::shared_ptr<spdlog::logger>& getLogger();

        static std::string formatMessage(const char* filename, const char* functionName, uint32_t lineNumber, std::string_view message);

    private:
        static std::shared_ptr<spdlog::logger> logger;
    };
}