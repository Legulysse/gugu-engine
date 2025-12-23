////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/System/Time.h"

////////////////////////////////////////////////////////////////
// Includes

#include <chrono>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

float GetElapsedSeconds()
{
    // Notes :
    // - This method is intended as a helper for simple looping animations needing a duration reference.
    // - steady_clock::now() will probably return elapsed time since the system boot.
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());
    return ms.count() * 0.001f;
}

int64 GetUtcTimestampAsMilliseconds()
{
    // Note : system_clock::now() should return utc time.
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    return ms.count();
}

int64 GetUtcTimestampAsNumeric()
{
    // Notes :
    // - system_clock::now() should return utc time.
    // - The intended formatting looks like yyyymmddhhmmss000 in order to obtain an integer that is both easy to read and easy to sort/compare.
    // - Directly using the tm struct from gmtime is the simplest way to break down a date into its components, without milliseconds.
    // - I should not need milliseconds for my usage of this method (mostly edition timestamps that stay human-readable for debug purposes).
    // - Even so, just in case, I will keep trailing zeroes to keep this formatting compatible with any future format including milliseconds.
    // - 2^63=9223372036854775808 which implies that the year 922337 is the upper limit of this approach.

    // Note : time_t is limited to seconds, and intended for use as a date.
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    tm* date = std::gmtime(&now);

    int64 formatted = 0;
    formatted += int64(date->tm_year + 1900) * int64(10000000000000);
    formatted += int64(date->tm_mon + 1) * int64(100000000000);
    formatted += int64(date->tm_mday) * int64(1000000000);
    formatted += int64(date->tm_hour) * int64(10000000);
    formatted += int64(date->tm_min) * int64(100000);
    formatted += int64(date->tm_sec) * int64(1000);
    return formatted;
}

std::string GetUtcTimestampAsString(const char* format)
{
    // Note : time_t is limited to seconds, and intended for use as a date.
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    char buffer[32] = { 0 };
    std::strftime(buffer, sizeof(buffer), format, std::gmtime(&now));
    return std::string(buffer);
}

std::string GetLocalTimestampAsString(const char* format)
{
    // Note : time_t is limited to seconds, and intended for use as a date.
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    char buffer[32] = { 0 };
    std::strftime(buffer, sizeof(buffer), format, std::localtime(&now));
    return std::string(buffer);
}

}   // namespace gugu
