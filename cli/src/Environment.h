#pragma once

#if defined(_WIN64)
    #define VDC_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
    #define VDC_PLATFORM_MACOS
#else
    #error "Unsupported platform"
#endif

#if defined(_MSC_VER)
    #define VDC_COMPILER_MSVC
#elif defined(__clang__)
    #define VDC_COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
    #define VDC_COMPILER_GCC
#else
    #error "Unknown compiler"
#endif

#if defined(NDEBUG)
    #define VDC_RELEASE
#else
    #define VDC_DEBUG
#endif