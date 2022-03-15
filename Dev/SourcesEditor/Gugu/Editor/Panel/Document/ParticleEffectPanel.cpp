////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/ParticleEffectPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Widget/RenderViewport.h"

#include "Gugu/Engine.h"
#include "Gugu/Core/DeltaTime.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ParticleEffect.h"
#include "Gugu/VisualEffects/ParticleSystem.h"
#include "Gugu/VisualEffects/ManagerVisualEffects.h"
#include "Gugu/Element/2D/ElementParticles.h"
#include "Gugu/Window/Window.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"

#include <imgui.h>
#include <imgui_stdlib.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ParticleEffectPanel::ParticleEffectPanel(const std::string& resourceID)
    : m_renderViewport(nullptr)
    , m_particleEffect(nullptr)
    , m_particleSystem(nullptr)
    , m_elementParticle(nullptr)
    , m_followCursor(false)
    , m_rotate(false)
    , m_restartDelay(1.f)
    , m_pendingRestart(false)
    , m_currentDelay(0.f)
    , m_maxParticleCount(0)
{
    m_resourceID = resourceID;
    m_particleEffect = GetResources()->GetParticleEffect(resourceID);

    m_title = m_resourceID;

    // Setup RenderViewport.
    m_renderViewport = new RenderViewport(true);
    m_renderViewport->SetSize(Vector2u(500, 500));

    // Test Particle
    m_elementParticle = m_renderViewport->GetRoot()->AddChild<ElementParticles>();
    m_elementParticle->SetUnifiedPosition(UDim2::POSITION_CENTER);
    m_particleSystem = m_elementParticle->CreateParticleSystem(*m_particleEffect->GetParticleSettings(), true);
}

ParticleEffectPanel::~ParticleEffectPanel()
{
    m_particleSystem = nullptr;
    SafeDelete(m_renderViewport);
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
    static float zoomFactor = 1.f;
    if (ImGui::SliderFloat("Zoom Factor", &zoomFactor, 1.f, 16.f))
    {
        m_renderViewport->SetZoom(zoomFactor);
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
        // Handle picking (should be used inside a viewport begin/end block).
        ImGuiIO& io = ImGui::GetIO();
        const Vector2f canvas_p0 = ImGui::GetCursorScreenPos();
        const Vector2f mouse_pos_in_canvas(io.MousePos.x - canvas_p0.x, io.MousePos.y - canvas_p0.y);
        Vector2f pickedGlobalPosition = m_renderViewport->GetPickedPosition(Vector2i(mouse_pos_in_canvas));

        m_elementParticle->SetPosition(pickedGlobalPosition);
    }

    if (m_rotate)
    {
        m_elementParticle->Rotate(dt.s() * 90.f);
    }

    m_renderViewport->ImGuiEnd();
}

void ParticleEffectPanel::UpdateProperties(const gugu::DeltaTime& dt)
{
    int id = 0;
    bool updated = false;

    ParticleSystemSettings* particleSettings = m_particleEffect->GetParticleSettings();

    // Helpers
    auto ColorConvertSfmlToFloat4 = [](const sf::Color& color) -> ImVec4
    {
        return ImGui::ColorConvertU32ToFloat4((color.a << 24) | (color.b << 16) | (color.g << 8) | color.r);
    };

    auto ColorConvertFloat4ToSfml = [](const ImVec4& color) -> sf::Color
    {
        sf::Uint32 colorU32 = ImGui::ColorConvertFloat4ToU32(color);
        return sf::Color(
            (sf::Uint8)((colorU32 & 0x000000ff) >> 0),
            (sf::Uint8)((colorU32 & 0x0000ff00) >> 8),
            (sf::Uint8)((colorU32 & 0x00ff0000) >> 16),
            (sf::Uint8)((colorU32 & 0xff000000) >> 24)
        );
    };

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
        float spawnPerSecond[2] = { particleSettings->minSpawnPerSecond, particleSettings->maxSpawnPerSecond };
        updated |= ImGui::InputFloat2("spawn per second", spawnPerSecond);
        particleSettings->minSpawnPerSecond = spawnPerSecond[0];
        particleSettings->maxSpawnPerSecond = spawnPerSecond[1];
    }
    else
    {
        float spawnPerSecond = particleSettings->minSpawnPerSecond;
        updated |= ImGui::InputFloat("spawn per second", &spawnPerSecond);
        particleSettings->minSpawnPerSecond = spawnPerSecond;
    }

    updated |= checkRandomSetting(id, particleSettings->useRandomParticlesPerSpawn);
    if (particleSettings->useRandomParticlesPerSpawn)
    {
        int particlesPerSpawn[2] = { particleSettings->minParticlesPerSpawn, particleSettings->maxParticlesPerSpawn };
        updated |= ImGui::InputInt2("particles per spawn", particlesPerSpawn);
        particleSettings->minParticlesPerSpawn = particlesPerSpawn[0];
        particleSettings->maxParticlesPerSpawn = particlesPerSpawn[1];
    }
    else
    {
        int particlesPerSpawn = particleSettings->minParticlesPerSpawn;
        updated |= ImGui::InputInt("particles per spawn", &particlesPerSpawn, 0);
        particleSettings->minParticlesPerSpawn = particlesPerSpawn;
    }

    ImGui::Spacing();

    // Particle behaviour
    const char* emissionBehaviourValues[] = { "Random Direction", "Angle Direction", "Away From Center" };
    int emissionBehaviourIndex = (int)particleSettings->emissionBehaviour;
    updated |= ImGui::Combo("emission behaviour", &emissionBehaviourIndex, emissionBehaviourValues, IM_ARRAYSIZE(emissionBehaviourValues));
    particleSettings->emissionBehaviour = (ParticleSystemSettings::EEmissionBehaviour)emissionBehaviourIndex;

    if (particleSettings->emissionBehaviour == ParticleSystemSettings::EEmissionBehaviour::AngleDirection)
    {
        ImVec2 emissionDirection = particleSettings->emissionDirection;
        updated |= ImGui::InputFloat2("emission direction", (float*)&emissionDirection);
        particleSettings->emissionDirection = emissionDirection;

        updated |= ImGui::InputFloat("emission angle", &particleSettings->emissionAngle);
    }

    updated |= checkRandomSetting(id, particleSettings->useRandomLifetime);
    if (particleSettings->useRandomLifetime)
    {
        int lifetime[2] = { particleSettings->minLifetime, particleSettings->maxLifetime };
        updated |= ImGui::InputInt2("lifetime (ms)", lifetime);
        particleSettings->minLifetime = lifetime[0];
        particleSettings->maxLifetime = lifetime[1];
    }
    else
    {
        int lifetime = particleSettings->minLifetime;
        updated |= ImGui::InputInt("lifetime (ms)", &lifetime, 0);
        particleSettings->minLifetime = lifetime;
    }

    updated |= checkRandomSetting(id, particleSettings->useRandomVelocity);
    if (particleSettings->useRandomVelocity)
    {
        float velocity[2] = { particleSettings->minVelocity, particleSettings->maxVelocity };
        updated |= ImGui::InputFloat2("velocity", velocity);
        particleSettings->minVelocity = velocity[0];
        particleSettings->maxVelocity = velocity[1];
    }
    else
    {
        float velocity = particleSettings->minVelocity;
        updated |= ImGui::InputFloat("velocity", &velocity);
        particleSettings->minVelocity = velocity;
    }

    ImGui::Spacing();

    // Render
    updated |= ImGui::Checkbox("keep size ratio", &particleSettings->keepSizeRatio);

    updated |= checkRandomSetting(id, particleSettings->useRandomStartSize);
    if (particleSettings->useRandomStartSize)
    {
        ImVec2 minStartSize = particleSettings->minStartSize;
        ImVec2 maxStartSize = particleSettings->maxStartSize;
        ImVec4 startSize = ImVec4(minStartSize.x, minStartSize.y, maxStartSize.x, maxStartSize.y);
        updated |= ImGui::InputFloat4("start size", (float*)&startSize);
        particleSettings->minStartSize = Vector2f(startSize.x, startSize.y);
        particleSettings->maxStartSize = Vector2f(startSize.z, startSize.w);
    }
    else
    {
        ImVec2 startSize = particleSettings->minStartSize;
        updated |= ImGui::InputFloat2("start size", (float*)&startSize);
        particleSettings->minStartSize = startSize;
    }

    updated |= ImGui::Checkbox("update size over lifetime", &particleSettings->updateSizeOverLifetime);
    ImGui::BeginDisabled(!particleSettings->updateSizeOverLifetime);

    updated |= checkRandomSetting(id, particleSettings->useRandomEndSize);
    if (particleSettings->useRandomEndSize)
    {
        ImVec2 minEndSize = particleSettings->minEndSize;
        ImVec2 maxEndSize = particleSettings->maxEndSize;
        ImVec4 endSize = ImVec4(minEndSize.x, minEndSize.y, maxEndSize.x, maxEndSize.y);
        updated |= ImGui::InputFloat4("end size", (float*)&endSize);
        particleSettings->minEndSize = Vector2f(endSize.x, endSize.y);
        particleSettings->maxEndSize = Vector2f(endSize.z, endSize.w);
    }
    else
    {
        ImVec2 endSize = particleSettings->minEndSize;
        updated |= ImGui::InputFloat2("end size", (float*)&endSize);
        particleSettings->minEndSize = endSize;
    }

    ImGui::EndDisabled();

    ImGui::Spacing();

    ImVec4 startColor = ColorConvertSfmlToFloat4(particleSettings->startColor);
    updated |= ImGui::ColorEdit4("start color", (float*)&startColor);
    particleSettings->startColor = sf::Color(ColorConvertFloat4ToSfml(startColor));

    updated |= ImGui::Checkbox("update color over lifetime", &particleSettings->updateColorOverLifetime);
    ImGui::BeginDisabled(!particleSettings->updateColorOverLifetime);

    ImVec4 endColor = ColorConvertSfmlToFloat4(particleSettings->endColor);
    updated |= ImGui::ColorEdit4("end color", (float*)&endColor);
    particleSettings->endColor = sf::Color(ColorConvertFloat4ToSfml(endColor));

    ImGui::EndDisabled();

    ImGui::Spacing();

    updated |= ImGui::InputText("imageSet", &particleSettings->imageSetID);

    // Finalize
    if (updated)
    {
        ParticleSystem::SanitizeSettings(*particleSettings, true);

        m_particleSystem->Init(*particleSettings);
        m_particleSystem->Restart();
        m_maxParticleCount = 0;

        m_dirty = true;
    }
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
