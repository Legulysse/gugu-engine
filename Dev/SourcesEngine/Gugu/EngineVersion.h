#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

////////////////////////////////////////////////////////////////
// Macros

#define GUGU_ENGINE_VERSION_MAJOR 0
#define GUGU_ENGINE_VERSION_MINOR 7
#define GUGU_ENGINE_VERSION_PATCH 0
#define GUGU_ENGINE_VERSION_IS_DEV false

#define GUGU_ENGINE_VERSION_NUMBER GUGU_STRINGIZE(GUGU_ENGINE_VERSION_MAJOR) "." GUGU_STRINGIZE(GUGU_ENGINE_VERSION_MINOR) "." GUGU_STRINGIZE(GUGU_ENGINE_VERSION_PATCH)

#if GUGU_ENGINE_VERSION_IS_DEV
#define GUGU_ENGINE_VERSION GUGU_ENGINE_VERSION_NUMBER "-dev"
#else
#define GUGU_ENGINE_VERSION GUGU_ENGINE_VERSION_NUMBER
#endif
