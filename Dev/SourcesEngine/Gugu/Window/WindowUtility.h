#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

enum class EWindowState : uint8
{
    Windowed,               // Resizable window.
    Fullscreen,             // Exclusive fullscreen.
    WindowedFullscreen,     // Borderless fullscreen window.
};

}   // namespace gugu
