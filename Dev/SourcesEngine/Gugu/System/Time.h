#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

// Get the current timestamp as milliseconds
int64 GetTimestamp();
std::string GetTimestampAsString();

}   // namespace gugu
