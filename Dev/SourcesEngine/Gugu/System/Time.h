#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

namespace timeformat
{
    // Available formats : https://fr.cppreference.net/cpp/chrono/c/strftime.html
    inline constexpr const char* LogEntry = "%Y-%m-%d %H:%M:%S";
    inline constexpr const char* Filename = "%Y-%m-%d_%H-%M-%S";
}

// Get elapsed seconds since an arbitrary epoch (probably system boot).
float GetElapsedSeconds();

// Get UTC timestamp as raw milliseconds since epoch.
int64 GetUtcTimestampAsMilliseconds();

// Get UTC timestamp in a yyyymmddhhmmss000 numerical format (currently floored on seconds).
int64 GetUtcTimestampAsNumeric();

// Get UTC timestamp as a date in a formatted string (limited to seconds as the smallest unit).
std::string GetUtcTimestampAsString(const char* format);
std::string GetLocalTimestampAsString(const char* format);

}   // namespace gugu
