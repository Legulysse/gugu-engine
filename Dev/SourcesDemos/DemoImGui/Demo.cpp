////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Window.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Utility/System.h"
#include "Gugu/Utility/Math.h"

#include <imgui.h>

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
Demo::Demo()
: m_root(nullptr)
{
}

Demo::~Demo()
{
}

void Demo::AppStart()
{
    RegisterHandlerEvents(GetGameWindow());

    // Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));
}

void Demo::AppStop()
{
    SafeDelete(m_root);
}

void Demo::AppUpdate(const DeltaTime& dt)
{
    ImGui::ShowDemoWindow();
}

}   //namespace demoproject
