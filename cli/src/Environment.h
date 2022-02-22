#pragma once

#if defined(_WIN64)
    #define VD_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
    #define VD_PLATFORM_MACOS
#else
    #error "Unsupported platform"
#endif

#if defined(_MSC_VER)
    #define VD_COMPILER_MSVC
#elif defined(__clang__)
    #define VD_COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
    #define VD_COMPILER_GCC
#else
    #error "Unknown compiler"
#endif

#if defined(NDEBUG)
    #define VD_RELEASE
#else
    #define VD_DEBUG
#endif