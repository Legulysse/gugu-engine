#pragma once

////////////////////////////////////////////////////////////////
// Macros

//--------------------------------
// Helpers for string conversion in macros

#define GUGU_STRINGIZE_IMPL(A) #A
#define GUGU_STRINGIZE(A) GUGU_STRINGIZE_IMPL(A)

//--------------------------------
// Operating System

#if defined(_WIN32) || defined(__WIN32__)
    #define GUGU_OS_WIN32

    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #ifndef NOMINMAX
        #define NOMINMAX
    #endif

#elif defined(linux) || defined(__linux)
    #define GUGU_OS_LINUX

#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)
    #define GUGU_OS_MAC

#endif

//--------------------------------
// Development Environment

#if defined(_MSC_VER)
    #define GUGU_ENV_VISUAL
#endif

//--------------------------------
// Debug / Release

#if defined(_DEBUG)
    #define GUGU_DEBUG
#else
    #define GUGU_RELEASE
#endif
