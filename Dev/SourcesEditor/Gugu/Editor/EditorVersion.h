#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

////////////////////////////////////////////////////////////////
// Macros

#define GUGU_EDITOR_VERSION_MAJOR 0
#define GUGU_EDITOR_VERSION_MINOR 4
#define GUGU_EDITOR_VERSION_PATCH 0
#define GUGU_EDITOR_VERSION_IS_DEV true

#define GUGU_EDITOR_VERSION_NUMBER GUGU_STRINGIZE(GUGU_EDITOR_VERSION_MAJOR) "." GUGU_STRINGIZE(GUGU_EDITOR_VERSION_MINOR) "." GUGU_STRINGIZE(GUGU_EDITOR_VERSION_PATCH)

#if GUGU_EDITOR_VERSION_IS_DEV
#define GUGU_EDITOR_VERSION GUGU_EDITOR_VERSION_NUMBER "-dev"
#else
#define GUGU_EDITOR_VERSION GUGU_EDITOR_VERSION_NUMBER
#endif
