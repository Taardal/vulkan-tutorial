#pragma once

#include "Environment.h"
#include "Log.h"

#ifdef VD_DEBUG
    #ifdef VD_PLATFORM_WINDOWS
        #define VD_BREAK() __debugbreak()
    #elif __has_builtin(__builtin_debugtrap)
        #define VD_BREAK() __builtin_debugtrap()
    #else
        #include <csignal>
        #if defined(SIGTRAP)
            #define VD_BREAK() std::raise(SIGTRAP)
        #else
            #define VD_BREAK() std::raise(SIGABRT)
        #endif
    #endif
    #define VD_ASSERT(tag, expression) \
        if (expression) \
        {} \
        else \
        { \
            VD_LOG_ERROR(tag, "Could not assert [{0}]", VD_TO_STRING(expression)); \
            VD_BREAK(); \
        }
#else
    #define VD_BREAK()
    #define VD_ASSERT(tag, expression)
#endif