////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/ParticleEffectPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Widget/RenderViewport.h"

#include "Gugu/Core/DeltaTime.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ParticleEffect.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/VisualEffects/ParticleSystem.h"
#include "Gugu/VisualEffects/ManagerVisualEffects.h"
#include "Gugu/Element/2D/ElementParticles.h"
#include "Gugu/System/Memory.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ParticleEffectPanel::ParticleEffectPanel(ParticleEffect* resource)
    : DocumentPanel(resource)
    , m_particleEffect(resource)
    , m_renderViewport(nullptr)
    , m_zoomFactor(1.f)
    , m_particleSystem(nullptr)
    , m_elementParticle(nullptr)
    , m_followCursor(false)
    , m_rotate(false)
    , m_restartDelay(1.f)
    , m_pendingRestart(false)
    , m_currentDelay(0.f)
    , m_maxParticleCount(0)
{
    // Setup RenderViewport.
    m_renderViewport = new RenderViewport(true);
    m_renderViewport->SetSize(Vector2u(500, 500));

    // Test Particle
    m_elementParticle = m_renderViewport->GetRoot()->AddChild<ElementParticles>();
    m_elementParticle->SetUnifiedPosition(UDim2::POSITION_CENTER);
    m_particleSystem = m_elementParticle->CreateParticleSystem(m_particleEffect, true);

    // Dependencies
    GetResources()->RegisterResourceListener(m_particleEffect, this, STD_BIND_3(&ParticleEffectPanel::OnResourceEvent, this));
}

ParticleEffectPanel::~ParticleEffectPanel()
{
    // Dependencies
    GetResources()->UnregisterResourceListeners(m_particleEffect, this);

    m_particleSystem = nullptr;
    SafeDelete(m_renderViewport);
}

void ParticleEffectPanel::ResetParticleSystem()
{
    m_particleSystem->Init(m_particleEffect);
    m_particleSystem->Restart();
    m_maxParticleCount = 0;
}

void ParticleEffectPanel::OnUndoRedo()
{
    ResetParticleSystem();
}

void ParticleEffectPanel::OnVisibilityChanged(bool visible)
{
    m_particleSystem->SetPaused(!visible);
}

void ParticleEffectPanel::UpdatePanelImpl(const DeltaTime& dt)
{
    //TODO: If this panel is not focused, I should be able to pause the particle system.

    if (!m_particleSystem->IsRunning())
    {
        if (!m_pendingRestart)
        {
            m_pendingRestart = true;
            m_currentDelay = m_restartDelay;
        }
        else
        {
            m_currentDelay -= dt.s();
            if (m_currentDelay <= 0.f)
            {
                m_pendingRestart = false;
                m_currentDelay = 0.f;

                m_particleSystem->Restart();
                m_maxParticleCount = 0;
            }
        }
    }
    else
    {
        m_maxParticleCount = Max(m_maxParticleCount, m_particleSystem->GetActiveParticleCount());
    }

    // Toolbar.
    if (ImGui::SliderFloat("Zoom Factor", &m_zoomFactor, 0.f, 16.f))
    {
        m_renderViewport->SetZoom(m_zoomFactor);
    }

    if (ImGui::Checkbox("Follow Cursor", &m_followCursor))
    {
        if (m_followCursor)
        {
            m_elementParticle->ResetUnifiedPosition();
        }
        else
        {
            m_elementParticle->SetUnifiedPosition(UDim2::POSITION_CENTER);
        }
    }

    ImGui::SameLine();
    if (ImGui::Checkbox("Rotate", &m_rotate))
    {
        if (m_rotate)
        {
            m_elementParticle->SetOriginY(150.f);
        }
        else
        {
            m_elementParticle->SetOriginY(0.f);
            m_elementParticle->SetRotation(0.f);
        }
    }

    ImGui::SameLine();
    ImGui::SetNextItemWidth(100.f);
    ImGui::InputFloat("Restart Delay", &m_restartDelay);
    m_restartDelay = Clamp(m_restartDelay, 0.f, 10.f);

    // Viewport.
    m_renderViewport->ImGuiBegin();

    if (m_followCursor)
    {
        Vector2f pickedGlobalPosition = m_renderViewport->GetMousePickedPosition();
        m_elementParticle->SetPosition(pickedGlobalPosition);
    }

    if (m_rotate)
    {
        m_elementParticle->Rotate(dt.s() * 90.f);
    }

    m_renderViewport->ImGuiEnd();
}

void ParticleEffectPanel::UpdatePropertiesImpl(const DeltaTime& dt)
{
    int id = 0;
    bool updated = false;

    ParticleSystemSettings* particleSettings = m_particleEffect->GetParticleSettings();

    // Helpers
    auto checkRandomSetting = [](int& id, bool& useRandom) -> bool
    {
        ImGui::PushID(id);
        float posX_before = ImGui::GetCursorPosX();
        bool updated = ImGui::Checkbox("##Random", &useRandom);
        ImGui::PopID();

        ImGui::SameLine();

        float posX_after = ImGui::GetCursorPosX();
        ImGui::SetCursorPosX(posX_after - ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::SetNextItemWidth(ImGui::CalcItemWidth() + ImGui::GetStyle().ItemInnerSpacing.x - (posX_after - posX_before));

        ++id;
        return updated;
    };

    // Infos
    ImGui::Text("Active Particles : %d / %d, Max Reached : %d",
        m_particleSystem->GetActiveParticleCount(),
        m_particleSystem->GetMaxParticleCount(),
        m_maxParticleCount);

    ImGui::Text("Data Size : %d bytes", m_particleSystem->GetParticleDataSize());

    ImGui::Spacing();

    // Setup
    updated |= ImGui::Checkbox("loop", &particleSettings->loop);
    ImGui::BeginDisabled(particleSettings->loop);
    updated |= ImGui::InputInt("duration (ms)", &particleSettings->duration, 0);
    ImGui::EndDisabled();

    updated |= ImGui::InputInt("max particles", &particleSettings->maxParticleCount, 0);

    const char* particleShapeValues[] = { "Point", "Quad" };
    int particleShapeIndex = (int)particleSettings->particleShape;
    updated |= ImGui::Combo("particle shape", &particleShapeIndex, particleShapeValues, IM_ARRAYSIZE(particleShapeValues));
    particleSettings->particleShape = (ParticleSystemSettings::EParticleShape)particleShapeIndex;

    updated |= ImGui::Checkbox("sort buffer", &particleSettings->useSortBuffer);
    updated |= ImGui::Checkbox("local space", &particleSettings->localSpace);

    ImGui::Spacing();

    // Emitter Shape
    const char* emitterShapeValues[] = { "Point", "Circle", "Annulus" };
    int emitterShapeIndex = (int)particleSettings->emitterShape;
    updated |= ImGui::Combo("emitter shape", &emitterShapeIndex, emitterShapeValues, IM_ARRAYSIZE(emitterShapeValues));
    particleSettings->emitterShape = (ParticleSystemSettings::EEmitterShape)emitterShapeIndex;

    if (particleSettings->emitterShape == ParticleSystemSettings::EEmitterShape::Circle
        || particleSettings->emitterShape == ParticleSystemSettings::EEmitterShape::Annulus)
    {
        updated |= ImGui::InputFloat("emitter radius", &particleSettings->emitterRadius);

        if (particleSettings->emitterShape == ParticleSystemSettings::EEmitterShape::Annulus)
        {
            updated |= ImGui::InputFloat("emitter inner radius", &particleSettings->emitterInnerRadius);
        }
    }

    ImGui::Spacing();

    // Particle Spawn
    updated |= checkRandomSetting(id, particleSettings->useRandomSpawnPerSecond);
    if (particleSettings->useRandomSpawnPerSecond)
    {
        updated |= ImGui::InputFloat2("spawn per second", &particleSettings->minSpawnPerSecond, &particleSettings->maxSpawnPerSecond);
    }
    else
    {
        updated |= ImGui::InputFloat("spawn per second", &particleSettings->minSpawnPerSecond);
    }

    updated |= checkRandomSetting(id, particleSettings->useRandomParticlesPerSpawn);
    if (particleSettings->useRandomParticlesPerSpawn)
    {
        updated |= ImGui::InputInt2("particles per spawn", &particleSettings->minParticlesPerSpawn, &particleSettings->maxParticlesPerSpawn);
    }
    else
    {
        updated |= ImGui::InputInt("particles per spawn", &particleSettings->minParticlesPerSpawn, 0);
    }

    ImGui::Spacing();

    // Particle behaviour
    const char* emissionBehaviourValues[] = { "Random Direction", "Angle Direction", "Away From Center" };
    int emissionBehaviourIndex = (int)particleSettings->emissionBehaviour;
    updated |= ImGui::Combo("emission behaviour", &emissionBehaviourIndex, emissionBehaviourValues, IM_ARRAYSIZE(emissionBehaviourValues));
    particleSettings->emissionBehaviour = (ParticleSystemSettings::EEmissionBehaviour)emissionBehaviourIndex;

    if (particleSettings->emissionBehaviour == ParticleSystemSettings::EEmissionBehaviour::AngleDirection)
    {
        updated |= ImGui::InputFloat2("emission direction", &particleSettings->emissionDirection);
        updated |= ImGui::InputFloat("emission angle", &particleSettings->emissionAngle);
    }

    updated |= checkRandomSetting(id, particleSettings->useRandomLifetime);
    if (particleSettings->useRandomLifetime)
    {
        updated |= ImGui::InputInt2("lifetime (ms)", &particleSettings->minLifetime, &particleSettings->maxLifetime);
    }
    else
    {
        updated |= ImGui::InputInt("lifetime (ms)", &particleSettings->minLifetime, 0);
    }

    updated |= checkRandomSetting(id, particleSettings->useRandomVelocity);
    if (particleSettings->useRandomVelocity)
    {
        updated |= ImGui::InputFloat2("velocity", &particleSettings->minVelocity, &particleSettings->maxVelocity);
    }
    else
    {
        updated |= ImGui::InputFloat("velocity", &particleSettings->minVelocity);
    }

    ImGui::Spacing();

    // Render
    updated |= ImGui::Checkbox("keep size ratio", &particleSettings->keepSizeRatio);

    updated |= checkRandomSetting(id, particleSettings->useRandomStartSize);
    if (particleSettings->useRandomStartSize)
    {
        updated |= ImGui::InputFloat4("start size", &particleSettings->minStartSize, &particleSettings->maxStartSize);
    }
    else
    {
        updated |= ImGui::InputFloat2("start size", &particleSettings->minStartSize);
    }

    updated |= ImGui::Checkbox("update size over lifetime", &particleSettings->updateSizeOverLifetime);
    ImGui::BeginDisabled(!particleSettings->updateSizeOverLifetime);

    updated |= checkRandomSetting(id, particleSettings->useRandomEndSize);
    if (particleSettings->useRandomEndSize)
    {
        updated |= ImGui::InputFloat4("end size", &particleSettings->minEndSize, &particleSettings->maxEndSize);
    }
    else
    {
        updated |= ImGui::InputFloat2("end size", &particleSettings->minEndSize);
    }

    ImGui::EndDisabled();

    ImGui::Spacing();

    updated |= ImGui::ColorEdit4("start color", &particleSettings->startColor);
    updated |= ImGui::Checkbox("update color over lifetime", &particleSettings->updateColorOverLifetime);
    ImGui::BeginDisabled(!particleSettings->updateColorOverLifetime);
    updated |= ImGui::ColorEdit4("end color", &particleSettings->endColor);
    ImGui::EndDisabled();

    ImGui::Spacing();

    std::string imageSetID = particleSettings->imageSet == nullptr ? "" : particleSettings->imageSet->GetID();
    if (ImGui::InputText("imageSet", &imageSetID, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        particleSettings->imageSet = (imageSetID == "") ? nullptr : GetResources()->GetImageSet(imageSetID);
        updated |= true;
    }

    // Finalize
    if (updated)
    {
        ParticleSystem::SanitizeSettings(*particleSettings, true);

        m_particleSystem->Init(*particleSettings);
        m_particleSystem->Restart();
        m_maxParticleCount = 0;

        RaiseDirty();
    }
}

void ParticleEffectPanel::OnResourceEvent(const Resource* resource, EResourceEvent event, const Resource* dependency)
{
    if (event == EResourceEvent::DependencyRemoved
        || event == EResourceEvent::DependencyUpdated)
    {
        m_particleSystem->Init(m_particleEffect);
        m_particleSystem->Restart();
        m_maxParticleCount = 0;
    }
}

}   //namespace gugu
