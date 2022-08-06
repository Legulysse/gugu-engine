#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

#include "Gugu/Math/Vector2.h"

#include <imgui.h>
#include <imgui_stdlib.h>

////////////////////////////////////////////////////////////////
// Forward Declarations

////////////////////////////////////////////////////////////////
// File Declarations

namespace ImGui {

bool InputVector2(const char* label, gugu::Vector2f* v, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
bool InputVector4(const char* label, gugu::Vector2f* v1, gugu::Vector2f* v2, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);

}   // namespace ImGui
