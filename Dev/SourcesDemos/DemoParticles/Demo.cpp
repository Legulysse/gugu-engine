////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/2D/ElementParticles.h"
#include "Gugu/Element/2D/ElementSFDrawable.h"
#include "Gugu/VisualEffects/ParticleSystem.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/World/World.h"
#include "Gugu/World/Level.h"

#include <imgui.h>
#include <imgui_stdlib.h>

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

    // Level
    Camera* levelCamera = GetGameWindow()->CreateCamera();
    levelCamera->SetCenterOnTarget(true);
    GetGameWindow()->BindLevel(GetWorld()->GetMainLevel(), levelCamera);

    Element* levelRoot = GetWorld()->GetMainLevel()->GetRootNode();

    // Grid
    sf::VertexArray* gridVertices = new sf::VertexArray;
    gridVertices->setPrimitiveType(sf::PrimitiveType::Lines);

    gridVertices->append(sf::Vertex(Vector2f(0.f, -10000.f)));
    gridVertices->append(sf::Vertex(Vector2f(0.f, 10000.f)));
    gridVertices->append(sf::Vertex(Vector2f(-10000.f, 0.f)));
    gridVertices->append(sf::Vertex(Vector2f(10000.f, 0.f)));

    ElementSFDrawable* gridElement = levelRoot->AddChild<ElementSFDrawable>();
    gridElement->SetSFDrawable(gridVertices);

    // Level Particles
    for (size_t i = 0; i < 10; ++i)
    {
        ParticleSystemSettings settings;
        settings.maxParticleCount = 500;
        settings.minSpawnPerSecond = 500;
        settings.maxSpawnPerSecond = 500;
        settings.updateColorOverLifetime = true;

        ElementParticles* elementParticle = levelRoot->AddChild<ElementParticles>();
        elementParticle->SetPosition(Vector2f(-300.f + (i % 5) * 150.f, -100.f + (i / 5) * 200.f));
        ParticleSystem* particleSystem = elementParticle->CreateParticleSystem(settings, true);

        m_particleSystemSettings.push_back(settings);
        m_particleSystems.push_back(particleSystem);
        m_centerParticleElements.push_back(elementParticle);
    }
    
    // Arm Particle
    sf::VertexArray* armShape = new sf::VertexArray;
    armShape->setPrimitiveType(sf::PrimitiveType::Lines);
    armShape->append(sf::Vertex(Vector2f(0.f, 0.f)));
    armShape->append(sf::Vertex(Vector2f(400.f, 0.f)));
    armShape->append(sf::Vertex(Vector2f(400.f, 0.f)));
    armShape->append(sf::Vertex(Vector2f(400.f, -50.f)));

    ElementSFDrawable* arm = levelRoot->AddChild<ElementSFDrawable>();
    arm->SetPosition(0.f, 50.f);
    arm->SetOrigin(50.f, 0.f);
    arm->SetSFDrawable(armShape);
    m_moveArm = arm;

    ParticleSystemSettings armSettings;
    armSettings.maxParticleCount = 100;
    armSettings.minParticlesPerSpawn = 5;
    armSettings.maxParticlesPerSpawn = 5;
    armSettings.minLifetime = 1500;
    armSettings.maxLifetime = 1500;
    armSettings.startColor = sf::Color::Yellow;
    armSettings.endColor = sf::Color(255, 0, 0, 100);
    armSettings.updateColorOverLifetime = true;

    ElementParticles* armElementParticle = arm->AddChild<ElementParticles>();
    armElementParticle->SetPositionX(400.f);
    armElementParticle->SetOrigin(0.f, 50.f);
    ParticleSystem* armParticleSystem = armElementParticle->CreateParticleSystem(armSettings, true);

    m_particleSystemSettings.push_back(armSettings);
    m_particleSystems.push_back(armParticleSystem);

    // Cursor Particle
    ParticleSystemSettings cursorSettings;
    cursorSettings.updateColorOverLifetime = true;

    ElementParticles* cursorElementParticle = m_root->AddChild<ElementParticles>();
    ParticleSystem* cursorParticleSystem = cursorElementParticle->CreateParticleSystem(cursorSettings, true);
    m_mouseFollow = cursorElementParticle;

    m_particleSystemSettings.push_back(cursorSettings);
    m_particleSystems.push_back(cursorParticleSystem);
    
    // Animation
    m_startTime = GetTimestamp();
    m_rotateArm = true;
}

void Demo::AppStop()
{
    SafeDelete(m_root);
}

void Demo::AppUpdate(const DeltaTime& dt)
{
    for (size_t i = 0; i < m_centerParticleElements.size(); ++i)
    {
        int64 time = GetTimestamp() - m_startTime;
        float curve = std::sin((float)time / 1000.f) * 100.f;
        m_centerParticleElements[i]->SetPositionY(curve + -100.f + (i / 5) * 200.f);
    }

    if (m_rotateArm)
    {
        m_moveArm->Rotate(dt.s() * 90.f);
    }

    m_mouseFollow->SetPosition(GetGameWindow()->GetMousePosition());

    // Imgui debug menu.
    if (ImGui::Begin("Particles"))
    {
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

        for (size_t i = 0; i < m_particleSystemSettings.size(); ++i)
        {
            ImGui::PushID(i);

            ImGuiTreeNodeFlags treeNodeFlag = ImGuiTreeNodeFlags_SpanAvailWidth;
            if (ImGui::TreeNodeEx("node", treeNodeFlag, "%d", i))
            {
                ImGui::Text("Running : %s, Active Particles : %d / %d", (m_particleSystems[i]->IsRunning() ? "true" : "false"), m_particleSystems[i]->GetActiveParticleCount(), m_particleSystems[i]->GetMaxParticleCount());

                ImGui::Checkbox("loop", &m_particleSystemSettings[i].loop);

                ImGui::BeginDisabled(m_particleSystemSettings[i].loop);
                ImGui::InputInt("duration (ms)", &m_particleSystemSettings[i].duration);
                ImGui::EndDisabled();

                ImGui::InputInt("max particles", &m_particleSystemSettings[i].maxParticleCount);
                ImGui::InputInt("vertices per particle", &m_particleSystemSettings[i].verticesPerParticle);
                ImGui::Checkbox("local space", &m_particleSystemSettings[i].localSpace);

                ImGui::Spacing();

                float spawnPerSecond[2] = { m_particleSystemSettings[i].minSpawnPerSecond, m_particleSystemSettings[i].maxSpawnPerSecond };
                ImGui::InputFloat2("spawn per second", spawnPerSecond);
                m_particleSystemSettings[i].minSpawnPerSecond = spawnPerSecond[0];
                m_particleSystemSettings[i].maxSpawnPerSecond = spawnPerSecond[1];

                int particlesPerSpawn[2] = { m_particleSystemSettings[i].minParticlesPerSpawn, m_particleSystemSettings[i].maxParticlesPerSpawn };
                ImGui::InputInt2("particles per spawn", particlesPerSpawn);
                m_particleSystemSettings[i].minParticlesPerSpawn = particlesPerSpawn[0];
                m_particleSystemSettings[i].maxParticlesPerSpawn = particlesPerSpawn[1];

                ImGui::Spacing();

                int lifetime[2] = { m_particleSystemSettings[i].minLifetime, m_particleSystemSettings[i].maxLifetime };
                ImGui::InputInt2("lifetime (ms)", lifetime);
                m_particleSystemSettings[i].minLifetime = lifetime[0];
                m_particleSystemSettings[i].maxLifetime = lifetime[1];

                float velocity[2] = { m_particleSystemSettings[i].minVelocity, m_particleSystemSettings[i].maxVelocity };
                ImGui::InputFloat2("velocity", velocity);
                m_particleSystemSettings[i].minVelocity = velocity[0];
                m_particleSystemSettings[i].maxVelocity = velocity[1];

                ImGui::Spacing();

                ImGui::Checkbox("keep size ratio", &m_particleSystemSettings[i].keepSizeRatio);

                ImVec2 minStartSize = m_particleSystemSettings[i].minStartSize;
                ImVec2 maxStartSize = m_particleSystemSettings[i].maxStartSize;
                ImVec4 startSize = ImVec4(minStartSize.x, minStartSize.y, maxStartSize.x, maxStartSize.y);
                ImGui::InputFloat4("start size", (float*)&startSize);
                m_particleSystemSettings[i].minStartSize = Vector2f(startSize.x, startSize.y);
                m_particleSystemSettings[i].maxStartSize = Vector2f(startSize.z, startSize.w);

                ImGui::Checkbox("update size over lifetime", &m_particleSystemSettings[i].updateSizeOverLifetime);

                ImGui::BeginDisabled(!m_particleSystemSettings[i].updateSizeOverLifetime);
                ImVec2 minEndSize = m_particleSystemSettings[i].minEndSize;
                ImVec2 maxEndSize = m_particleSystemSettings[i].maxEndSize;
                ImVec4 endSize = ImVec4(minEndSize.x, minEndSize.y, maxEndSize.x, maxEndSize.y);
                ImGui::InputFloat4("end size", (float*)&endSize);
                m_particleSystemSettings[i].minEndSize = Vector2f(endSize.x, endSize.y);
                m_particleSystemSettings[i].maxEndSize = Vector2f(endSize.z, endSize.w);
                ImGui::EndDisabled();

                ImGui::Spacing();

                ImVec4 startColor = ColorConvertSfmlToFloat4(m_particleSystemSettings[i].startColor);
                ImGui::ColorEdit4("start color", (float*)&startColor);
                m_particleSystemSettings[i].startColor = sf::Color(ColorConvertFloat4ToSfml(startColor));

                ImGui::Checkbox("update color over lifetime", &m_particleSystemSettings[i].updateColorOverLifetime);

                ImGui::BeginDisabled(!m_particleSystemSettings[i].updateColorOverLifetime);
                ImVec4 endColor = ColorConvertSfmlToFloat4(m_particleSystemSettings[i].endColor);
                ImGui::ColorEdit4("end color", (float*)&endColor);
                m_particleSystemSettings[i].endColor = sf::Color(ColorConvertFloat4ToSfml(endColor));
                ImGui::EndDisabled();

                ImGui::Spacing();

                ImGui::InputText("imageSet", &m_particleSystemSettings[i].imageSetID);

                ImGui::TreePop();
            }

            ImGui::PopID();
        }

        ImGui::Separator();
        if (ImGui::Button("Restart All"))
        {
            for (size_t i = 0; i < m_particleSystems.size(); ++i)
            {
                m_particleSystems[i]->Init(m_particleSystemSettings[i]);
                m_particleSystems[i]->Restart();
            }
        }

        ImGui::Checkbox("rotate arm", &m_rotateArm);
    }
    ImGui::End();
}

}   //namespace demoproject
