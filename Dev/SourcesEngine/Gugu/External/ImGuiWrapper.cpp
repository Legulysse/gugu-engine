////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/External/ImGuiWrapper.h"

////////////////////////////////////////////////////////////////
// Includes

////////////////////////////////////////////////////////////////
// File Implementation

namespace ImGui {

bool InputVector2(const char* label, gugu::Vector2f* v, const char* format, ImGuiInputTextFlags flags)
{
    float temp[2] = { v->x, v->y };

    bool updated = ImGui::InputFloat2(label, temp, format, flags);
    if (updated)
    {
        v->x = temp[0];
        v->y = temp[1];
    }

    return updated;
}

bool InputVector4(const char* label, gugu::Vector2f* v1, gugu::Vector2f* v2, const char* format, ImGuiInputTextFlags flags)
{
    float temp[4] = { v1->x, v1->y, v2->x, v2->y };

    bool updated = ImGui::InputFloat4(label, temp, format, flags);
    if (updated)
    {
        v1->x = temp[0];
        v1->y = temp[1];
        v2->x = temp[2];
        v2->y = temp[3];
    }

    return updated;
}

}   // namespace ImGui
