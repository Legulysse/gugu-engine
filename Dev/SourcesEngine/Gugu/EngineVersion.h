#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

////////////////////////////////////////////////////////////////
// Macros

#define GUGU_ENGINE_VERSION_MAJOR 0
#define GUGU_ENGINE_VERSION_MINOR 8
#define GUGU_ENGINE_VERSION_PATCH 1
#define GUGU_ENGINE_VERSION_IS_DEV true
#define GUGU_ENGINE_VERSION_IS_PRE_RELEASE false
#define GUGU_ENGINE_VERSION_IS_POST_RELEASE false

#define GUGU_ENGINE_VERSION_NUMBER GUGU_STRINGIZE(GUGU_ENGINE_VERSION_MAJOR) "." GUGU_STRINGIZE(GUGU_ENGINE_VERSION_MINOR) "." GUGU_STRINGIZE(GUGU_ENGINE_VERSION_PATCH)

#if GUGU_ENGINE_VERSION_IS_DEV
#define GUGU_ENGINE_VERSION GUGU_ENGINE_VERSION_NUMBER "-dev"
#elif GUGU_ENGINE_VERSION_IS_PRE_RELEASE
#define GUGU_ENGINE_VERSION GUGU_ENGINE_VERSION_NUMBER "-rc"
#elif GUGU_ENGINE_VERSION_IS_POST_RELEASE
#define GUGU_ENGINE_VERSION GUGU_ENGINE_VERSION_NUMBER "-post"
#else
#define GUGU_ENGINE_VERSION GUGU_ENGINE_VERSION_NUMBER
#endif
