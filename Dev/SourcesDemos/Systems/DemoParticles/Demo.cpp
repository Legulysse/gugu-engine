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
#include "Gugu/Scene/ManagerScenes.h"
#include "Gugu/Scene/Scene.h"

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
    RegisterEventHandler(GetGameWindow());

    // Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

    // Scene
    Camera* sceneCamera = GetGameWindow()->CreateCamera();
    sceneCamera->SetCenterOnTarget(true);
    GetGameWindow()->BindScene(GetScenes()->GetRootScene(), sceneCamera);

    Element* sceneRoot = GetScenes()->GetRootScene()->GetRootNode();

    // Grid
    sf::VertexArray* gridVertices = new sf::VertexArray;
    gridVertices->setPrimitiveType(sf::PrimitiveType::Lines);

    gridVertices->append(sf::Vertex(Vector2f(0.f, -10000.f)));
    gridVertices->append(sf::Vertex(Vector2f(0.f, 10000.f)));
    gridVertices->append(sf::Vertex(Vector2f(-10000.f, 0.f)));
    gridVertices->append(sf::Vertex(Vector2f(10000.f, 0.f)));

    ElementSFDrawable* gridElement = sceneRoot->AddChild<ElementSFDrawable>();
    gridElement->SetSFDrawable(gridVertices);

    // Prepare Scene Particles
    for (size_t i = 0; i < 10; ++i)
    {
        ParticleSystemSettings settings;
        settings.maxParticleCount = 500;
        settings.minSpawnPerSecond = 500;
        settings.updateColorOverLifetime = true;

        m_particleSystemSettings.push_back(settings);
    }

    // Test ring effect
    m_particleSystemSettings[0].maxParticleCount = 2000;
    m_particleSystemSettings[0].emitterShape = ParticleSystemSettings::EEmitterShape::Annulus;
    m_particleSystemSettings[0].emitterRadius = 200.f;
    m_particleSystemSettings[0].emitterInnerRadius = 180.f;
    m_particleSystemSettings[0].minSpawnPerSecond = 1200.f;

    // Test swarm effect
    m_particleSystemSettings[9].maxParticleCount = 1000;
    m_particleSystemSettings[9].emitterShape = ParticleSystemSettings::EEmitterShape::Circle;
    m_particleSystemSettings[9].emitterRadius = 100.f;
    m_particleSystemSettings[9].minSpawnPerSecond = 1000.f;
    m_particleSystemSettings[9].minVelocity = 200.f;
    m_particleSystemSettings[9].endColor = sf::Color(255, 255, 255, 0);

    // Spawn Scene Particles
    for (size_t i = 0; i < 10; ++i)
    {
        ElementParticles* elementParticle = sceneRoot->AddChild<ElementParticles>();
        elementParticle->SetPosition(Vector2f(-300.f + (i % 5) * 150.f, -100.f + (i / 5) * 200.f));
        ParticleSystem* particleSystem = elementParticle->CreateParticleSystem(m_particleSystemSettings[i], true);

        m_particleSystems.push_back(particleSystem);
        m_centerParticleElements.push_back(elementParticle);
    }

    // Arm
    sf::VertexArray* armShape = new sf::VertexArray;
    armShape->setPrimitiveType(sf::PrimitiveType::Lines);
    armShape->append(sf::Vertex(Vector2f(0.f, 0.f)));
    armShape->append(sf::Vertex(Vector2f(300.f, 0.f)));
    armShape->append(sf::Vertex(Vector2f(300.f, 0.f)));
    armShape->append(sf::Vertex(Vector2f(300.f, -50.f)));
    armShape->append(sf::Vertex(Vector2f(0.f, 0.f)));
    armShape->append(sf::Vertex(Vector2f(-400.f, 0.f)));
    armShape->append(sf::Vertex(Vector2f(-400.f, 0.f)));
    armShape->append(sf::Vertex(Vector2f(-400.f, 50.f)));

    ElementSFDrawable* arm = sceneRoot->AddChild<ElementSFDrawable>();
    arm->SetPosition(0.f, 50.f);
    arm->SetOrigin(50.f, 0.f);
    arm->SetSFDrawable(armShape);
    m_moveArm = arm;

    // Arm Particle
    ParticleSystemSettings armSettings;
    armSettings.maxParticleCount = 100;
    armSettings.minParticlesPerSpawn = 5;
    armSettings.minLifetime = 1500;
    armSettings.useRandomStartSize = true;
    armSettings.minStartSize = Vector2f(5.f, 5.f);
    armSettings.maxStartSize = Vector2f(10.f, 10.f);
    armSettings.updateSizeOverLifetime = true;
    armSettings.startColor = sf::Color::Yellow;
    armSettings.endColor = sf::Color::Red;
    armSettings.updateColorOverLifetime = true;

    ElementParticles* armElementParticle = arm->AddChild<ElementParticles>();
    armElementParticle->SetPositionX(300.f);
    armElementParticle->SetOrigin(0.f, 50.f);
    ParticleSystem* armParticleSystem = armElementParticle->CreateParticleSystem(armSettings, true);

    m_particleSystemSettings.push_back(armSettings);
    m_particleSystems.push_back(armParticleSystem);

    // Arm Particle 2
    ParticleSystemSettings armSettings2;
    armSettings2.maxParticleCount = 5000;
    armSettings2.emitterShape = ParticleSystemSettings::EEmitterShape::Annulus;
    armSettings2.emitterRadius = 80.f;
    armSettings2.emitterInnerRadius = 70.f;
    armSettings2.minSpawnPerSecond = 500.f;
    armSettings2.minParticlesPerSpawn = 5;
    armSettings2.minLifetime = 1500;
    armSettings2.useRandomStartSize = true;
    armSettings2.minStartSize = Vector2f(5.f, 5.f);
    armSettings2.maxStartSize = Vector2f(10.f, 10.f);
    armSettings2.updateSizeOverLifetime = true;
    armSettings2.startColor = sf::Color::Yellow;
    armSettings2.endColor = sf::Color::Red;
    armSettings2.updateColorOverLifetime = true;

    ElementParticles* armElementParticle2 = arm->AddChild<ElementParticles>();
    armElementParticle2->SetPositionX(-400.f);
    armElementParticle2->SetOrigin(0.f, -50.f);
    ParticleSystem* armParticleSystem2 = armElementParticle2->CreateParticleSystem(armSettings2, true);

    m_particleSystemSettings.push_back(armSettings2);
    m_particleSystems.push_back(armParticleSystem2);

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
    m_animateEmitters = true;
    m_rotateArm = true;
}

void Demo::AppStop()
{
    SafeDelete(m_root);
}

void Demo::AppUpdate(const DeltaTime& dt)
{
    if (m_animateEmitters)
    {
        for (size_t i = 0; i < m_centerParticleElements.size(); ++i)
        {
            int64 time = GetTimestamp() - m_startTime;
            float curve = std::sin((float)time / 1000.f) * 100.f;
            m_centerParticleElements[i]->SetPositionY(curve + -100.f + (i / 5) * 200.f);
        }
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

        for (size_t i = 0; i < m_particleSystemSettings.size(); ++i)
        {
            ImGui::PushID(i);

            ImGuiTreeNodeFlags treeNodeFlag = ImGuiTreeNodeFlags_SpanAvailWidth;
            if (ImGui::TreeNodeEx("node", treeNodeFlag, "%d", i))
            {
                int id = 0;
                bool updated = false;

                ImGui::Text("Running : %s, Active Particles : %d / %d", (m_particleSystems[i]->IsRunning() ? "true" : "false"), m_particleSystems[i]->GetActiveParticleCount(), m_particleSystems[i]->GetMaxParticleCount());

                // Setup
                updated |= ImGui::Checkbox("loop", &m_particleSystemSettings[i].loop);
                ImGui::BeginDisabled(m_particleSystemSettings[i].loop);
                updated |= ImGui::InputInt("duration (ms)", &m_particleSystemSettings[i].duration, 0);
                ImGui::EndDisabled();

                updated |= ImGui::InputInt("max particles", &m_particleSystemSettings[i].maxParticleCount, 0);

                const char* particleShapeValues[] = { "Point", "Quad" };
                int particleShapeIndex = (int)m_particleSystemSettings[i].particleShape;
                updated |= ImGui::Combo("particle shape", &particleShapeIndex, particleShapeValues, IM_ARRAYSIZE(particleShapeValues));
                m_particleSystemSettings[i].particleShape = (ParticleSystemSettings::EParticleShape)particleShapeIndex;

                updated |= ImGui::Checkbox("sort buffer", &m_particleSystemSettings[i].useSortBuffer);
                updated |= ImGui::Checkbox("local space", &m_particleSystemSettings[i].localSpace);

                ImGui::Spacing();

                // Emitter Shape
                const char* emitterShapeValues[] = { "Point", "Circle", "Annulus" };
                int emitterShapeIndex = (int)m_particleSystemSettings[i].emitterShape;
                updated |= ImGui::Combo("emitter shape", &emitterShapeIndex, emitterShapeValues, IM_ARRAYSIZE(emitterShapeValues));
                m_particleSystemSettings[i].emitterShape = (ParticleSystemSettings::EEmitterShape)emitterShapeIndex;

                if (m_particleSystemSettings[i].emitterShape == ParticleSystemSettings::EEmitterShape::Circle
                    || m_particleSystemSettings[i].emitterShape == ParticleSystemSettings::EEmitterShape::Annulus)
                {
                    updated |= ImGui::InputFloat("emitter radius", &m_particleSystemSettings[i].emitterRadius);

                    if (m_particleSystemSettings[i].emitterShape == ParticleSystemSettings::EEmitterShape::Annulus)
                    {
                        updated |= ImGui::InputFloat("emitter inner radius", &m_particleSystemSettings[i].emitterInnerRadius);
                    }
                }

                ImGui::Spacing();

                // Particle Spawn
                updated |= checkRandomSetting(id, m_particleSystemSettings[i].useRandomSpawnPerSecond);
                if (m_particleSystemSettings[i].useRandomSpawnPerSecond)
                {
                    float spawnPerSecond[2] = { m_particleSystemSettings[i].minSpawnPerSecond, m_particleSystemSettings[i].maxSpawnPerSecond };
                    updated |= ImGui::InputFloat2("spawn per second", spawnPerSecond);
                    m_particleSystemSettings[i].minSpawnPerSecond = spawnPerSecond[0];
                    m_particleSystemSettings[i].maxSpawnPerSecond = spawnPerSecond[1];
                }
                else
                {
                    float spawnPerSecond = m_particleSystemSettings[i].minSpawnPerSecond;
                    updated |= ImGui::InputFloat("spawn per second", &spawnPerSecond);
                    m_particleSystemSettings[i].minSpawnPerSecond = spawnPerSecond;
                }

                updated |= checkRandomSetting(id, m_particleSystemSettings[i].useRandomParticlesPerSpawn);
                if (m_particleSystemSettings[i].useRandomParticlesPerSpawn)
                {
                    int particlesPerSpawn[2] = { m_particleSystemSettings[i].minParticlesPerSpawn, m_particleSystemSettings[i].maxParticlesPerSpawn };
                    updated |= ImGui::InputInt2("particles per spawn", particlesPerSpawn);
                    m_particleSystemSettings[i].minParticlesPerSpawn = particlesPerSpawn[0];
                    m_particleSystemSettings[i].maxParticlesPerSpawn = particlesPerSpawn[1];
                }
                else
                {
                    int particlesPerSpawn = m_particleSystemSettings[i].minParticlesPerSpawn;
                    updated |= ImGui::InputInt("particles per spawn", &particlesPerSpawn , 0);
                    m_particleSystemSettings[i].minParticlesPerSpawn = particlesPerSpawn;
                }

                ImGui::Spacing();

                // Particle behaviour
                const char* emissionBehaviourValues[] = { "Random Direction", "Angle Direction", "Away From Center" };
                int emissionBehaviourIndex = (int)m_particleSystemSettings[i].emissionBehaviour;
                updated |= ImGui::Combo("emission behaviour", &emissionBehaviourIndex, emissionBehaviourValues, IM_ARRAYSIZE(emissionBehaviourValues));
                m_particleSystemSettings[i].emissionBehaviour = (ParticleSystemSettings::EEmissionBehaviour)emissionBehaviourIndex;

                if (m_particleSystemSettings[i].emissionBehaviour == ParticleSystemSettings::EEmissionBehaviour::AngleDirection)
                {
                    ImVec2 emissionDirection = m_particleSystemSettings[i].emissionDirection;
                    updated |= ImGui::InputFloat2("emission direction", (float*)&emissionDirection);
                    m_particleSystemSettings[i].emissionDirection = emissionDirection;

                    updated |= ImGui::InputFloat("emission angle", &m_particleSystemSettings[i].emissionAngle);
                }

                updated |= checkRandomSetting(id, m_particleSystemSettings[i].useRandomLifetime);
                if (m_particleSystemSettings[i].useRandomLifetime)
                {
                    int lifetime[2] = { m_particleSystemSettings[i].minLifetime, m_particleSystemSettings[i].maxLifetime };
                    updated |= ImGui::InputInt2("lifetime (ms)", lifetime);
                    m_particleSystemSettings[i].minLifetime = lifetime[0];
                    m_particleSystemSettings[i].maxLifetime = lifetime[1];
                }
                else
                {
                    int lifetime = m_particleSystemSettings[i].minLifetime;
                    updated |= ImGui::InputInt("lifetime (ms)", &lifetime, 0);
                    m_particleSystemSettings[i].minLifetime = lifetime;
                }

                updated |= checkRandomSetting(id, m_particleSystemSettings[i].useRandomVelocity);
                if (m_particleSystemSettings[i].useRandomVelocity)
                {
                    float velocity[2] = { m_particleSystemSettings[i].minVelocity, m_particleSystemSettings[i].maxVelocity };
                    updated |= ImGui::InputFloat2("velocity", velocity);
                    m_particleSystemSettings[i].minVelocity = velocity[0];
                    m_particleSystemSettings[i].maxVelocity = velocity[1];
                }
                else
                {
                    float velocity = m_particleSystemSettings[i].minVelocity;
                    updated |= ImGui::InputFloat("velocity", &velocity);
                    m_particleSystemSettings[i].minVelocity = velocity;
                }

                ImGui::Spacing();

                // Render
                updated |= ImGui::Checkbox("keep size ratio", &m_particleSystemSettings[i].keepSizeRatio);

                updated |= checkRandomSetting(id, m_particleSystemSettings[i].useRandomStartSize);
                if (m_particleSystemSettings[i].useRandomStartSize)
                {
                    ImVec2 minStartSize = m_particleSystemSettings[i].minStartSize;
                    ImVec2 maxStartSize = m_particleSystemSettings[i].maxStartSize;
                    ImVec4 startSize = ImVec4(minStartSize.x, minStartSize.y, maxStartSize.x, maxStartSize.y);
                    updated |= ImGui::InputFloat4("start size", (float*)&startSize);
                    m_particleSystemSettings[i].minStartSize = Vector2f(startSize.x, startSize.y);
                    m_particleSystemSettings[i].maxStartSize = Vector2f(startSize.z, startSize.w);
                }
                else
                {
                    ImVec2 startSize = m_particleSystemSettings[i].minStartSize;
                    updated |= ImGui::InputFloat2("start size", (float*)&startSize);
                    m_particleSystemSettings[i].minStartSize = startSize;
                }

                updated |= ImGui::Checkbox("update size over lifetime", &m_particleSystemSettings[i].updateSizeOverLifetime);
                ImGui::BeginDisabled(!m_particleSystemSettings[i].updateSizeOverLifetime);

                updated |= checkRandomSetting(id, m_particleSystemSettings[i].useRandomEndSize);
                if (m_particleSystemSettings[i].useRandomEndSize)
                {
                    ImVec2 minEndSize = m_particleSystemSettings[i].minEndSize;
                    ImVec2 maxEndSize = m_particleSystemSettings[i].maxEndSize;
                    ImVec4 endSize = ImVec4(minEndSize.x, minEndSize.y, maxEndSize.x, maxEndSize.y);
                    updated |= ImGui::InputFloat4("end size", (float*)&endSize);
                    m_particleSystemSettings[i].minEndSize = Vector2f(endSize.x, endSize.y);
                    m_particleSystemSettings[i].maxEndSize = Vector2f(endSize.z, endSize.w);
                }
                else
                {
                    ImVec2 endSize = m_particleSystemSettings[i].minEndSize;
                    updated |= ImGui::InputFloat2("end size", (float*)&endSize);
                    m_particleSystemSettings[i].minEndSize = endSize;
                }

                ImGui::EndDisabled();

                ImGui::Spacing();

                ImVec4 startColor = ColorConvertSfmlToFloat4(m_particleSystemSettings[i].startColor);
                updated |= ImGui::ColorEdit4("start color", (float*)&startColor);
                m_particleSystemSettings[i].startColor = sf::Color(ColorConvertFloat4ToSfml(startColor));

                updated |= ImGui::Checkbox("update color over lifetime", &m_particleSystemSettings[i].updateColorOverLifetime);
                ImGui::BeginDisabled(!m_particleSystemSettings[i].updateColorOverLifetime);

                ImVec4 endColor = ColorConvertSfmlToFloat4(m_particleSystemSettings[i].endColor);
                updated |= ImGui::ColorEdit4("end color", (float*)&endColor);
                m_particleSystemSettings[i].endColor = sf::Color(ColorConvertFloat4ToSfml(endColor));

                ImGui::EndDisabled();

                ImGui::Spacing();

                updated |= ImGui::InputText("imageSet", &m_particleSystemSettings[i].imageSetID);

                ImGui::TreePop();

                if (updated)
                {
                    ParticleSystem::SanitizeSettings(m_particleSystemSettings[i], true);
                }
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

        ImGui::Checkbox("animate emitters", &m_animateEmitters);
        ImGui::Checkbox("rotate arm", &m_rotateArm);
    }
    ImGui::End();
}

}   //namespace demoproject
