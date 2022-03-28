#include "Log.h"
#include <sstream>

namespace Vulkandemo {

    std::shared_ptr<spdlog::logger> Log::logger;

    void Log::initialize(const std::string& name, Level level) {
        logger = spdlog::stdout_color_mt(name);
        logger->set_pattern("%^[%Y-%m-%d] [%T] [%n] [%l] %v%$");
        logger->set_level(Log::getSpdLogLevel(level));
    }

    const std::shared_ptr<spdlog::logger>& Log::getLogger() {
        return logger;
    }

    std::string Log::formatMessage(const char* filename, const char* functionName, uint32_t lineNumber, std::string_view message) {
        std::stringstream ss;
        ss << "[" << filename << ":" << functionName << ":" << lineNumber << "] - " << message;
        return ss.str();
    }

    spdlog::level::level_enum Log::getSpdLogLevel(Level level) {
        switch (level) {
            case Level::Critical:
                return spdlog::level::critical;
            case Level::Error:
                return spdlog::level::err;
            case Level::Warn:
                return spdlog::level::warn;
            case Level::Info:
                return spdlog::level::info;
            case Level::Debug:
                return spdlog::level::debug;
            default:
                return spdlog::level::trace;
        }
    }
}