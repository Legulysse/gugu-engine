////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/ParticleEffectPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Widget/RenderViewport.h"

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ParticleEffect.h"
#include "Gugu/VisualEffects/ParticleSystem.h"
#include "Gugu/VisualEffects/ManagerVisualEffects.h"
#include "Gugu/Element/2D/ElementParticles.h"
#include "Gugu/System/SystemUtility.h"

#include <imgui.h>
#include <imgui_stdlib.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ParticleEffectPanel::ParticleEffectPanel(const std::string& resourceID)
    : m_renderViewport(nullptr)
    , m_particleEffect(nullptr)
{
    m_resourceID = resourceID;
    m_particleEffect = GetResources()->GetParticleEffect(resourceID);

    m_title = m_resourceID;

    // Setup RenderViewport.
    m_renderViewport = new RenderViewport(true);
    m_renderViewport->SetSize(Vector2u(500, 500));

    // Test Particle
    ParticleSystemSettings settings;
    settings.maxParticleCount = 500;
    settings.minSpawnPerSecond = 500;
    settings.updateColorOverLifetime = true;

    ElementParticles* elementParticle = m_renderViewport->GetRoot()->AddChild<ElementParticles>();
    elementParticle->SetPosition(Vector2f(100.f, 100.f));
    ParticleSystem* particleSystem = elementParticle->CreateParticleSystem(settings, true);
}

ParticleEffectPanel::~ParticleEffectPanel()
{
    SafeDelete(m_renderViewport);
}

void ParticleEffectPanel::UpdatePanel(const DeltaTime& dt)
{
    m_focused = false;

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_HorizontalScrollbar;
    if (m_dirty)
    {
        flags |= ImGuiWindowFlags_UnsavedDocument;
    }

    if (ImGui::Begin(m_title.c_str(), false, flags))
    {
        if (ImGui::IsWindowFocused())
        {
            m_focused = true;
        }

        // Toolbar.
        static float zoomFactor = 1.f;
        if (ImGui::SliderFloat("Zoom Factor", &zoomFactor, 1.f, 16.f))
        {
            m_renderViewport->SetZoom(zoomFactor);
        }

        // Viewport.
        m_renderViewport->ImGuiBegin();
        m_renderViewport->ImGuiEnd();
    }
    ImGui::End();
}

void ParticleEffectPanel::UpdateProperties(const gugu::DeltaTime& dt)
{
}

bool ParticleEffectPanel::Save()
{
    if (m_particleEffect->SaveToFile())
    {
        m_dirty = false;
    }

    return !m_dirty;
}

}   //namespace gugu
