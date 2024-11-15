#pragma once

////////////////////////////////////////////////////////////////
// Macros

//----------------------------------------------
// Helpers for string conversion in macros

#define GUGU_STRINGIZE_IMPL(A) #A
#define GUGU_STRINGIZE(A) GUGU_STRINGIZE_IMPL(A)

//----------------------------------------------
// Operating System

#if defined(_WIN32)

    #define GUGU_OS_WINDOWS

    #if !defined(WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif

    #if !defined(NOMINMAX)
        #define NOMINMAX
    #endif

#elif defined(__unix__) && defined(__linux__)

    #define GUGU_OS_LINUX

#elif defined(__APPLE__) && defined(__MACH__)

    // Apple platform, see which one it is
    #include "TargetConditionals.h"

    #if TARGET_OS_MAC
        #define GUGU_OS_MAC
    #endif

#endif

//----------------------------------------------
// Development Environment

#if defined(_MSC_VER)
    #define GUGU_ENV_VISUAL
#endif

//----------------------------------------------
// Debug / Release

#if defined(NDEBUG)
    #define GUGU_RELEASE
#else
    #define GUGU_DEBUG
#endif
