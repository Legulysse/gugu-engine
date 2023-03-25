#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

#include "Gugu/Math/Vector2.h"

#include <SFML/Graphics/Rect.hpp>

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace ImGui {

// Text variation.
void Text(const std::string& text);

// Scalar Inputs variations.
bool InputInt2(const char* label, int* v1, int* v2, ImGuiInputTextFlags flags = 0);
bool InputInt2(const char* label, gugu::Vector2i* v, ImGuiInputTextFlags flags = 0);
bool InputInt4(const char* label, sf::IntRect* v, ImGuiInputTextFlags flags = 0);
bool InputFloat2(const char* label, float* v1, float* v2, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
bool InputFloat2(const char* label, gugu::Vector2f* v, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
bool InputFloat4(const char* label, gugu::Vector2f* v1, gugu::Vector2f* v2, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);

// Color Inputs variations.
bool ColorEdit4(const char* label, sf::Color* color, ImGuiColorEditFlags flags = 0);

// Color conversions.
ImVec4 ColorConvertSfmlToFloat4(const sf::Color& color);
sf::Color ColorConvertFloat4ToSfml(const ImVec4& color);

// Combo variation.
bool Combo(const char* label, const std::vector<std::string>& comboValues, size_t* selectedIndex, ImGuiComboFlags flags = 0);

}   // namespace ImGui
