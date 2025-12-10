////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/2D/ElementParticles.h"
#include "Gugu/Element/2D/ElementSFDrawable.h"
#include "Gugu/VisualEffects/ParticleSystem.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/System/Memory.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/Window/Vertex2.h"
#include "Gugu/Scene/ManagerScenes.h"
#include "Gugu/Scene/Scene.h"
#include "Gugu/External/ImGuiUtility.h"

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
    m_root->SetUnifiedSize(UDim2::SIZE_FULL);

    // Scene
    Camera* sceneCamera = GetGameWindow()->CreateCamera();
    sceneCamera->SetCenterOnTarget(true);
    GetGameWindow()->BindScene(GetScenes()->GetRootScene(), sceneCamera);

    Element* sceneRoot = GetScenes()->GetRootScene()->GetRootNode();

    // Grid
    sf::VertexArray* gridVertices = new sf::VertexArray;
    gridVertices->setPrimitiveType(sf::PrimitiveType::Lines);

    AppendLineVertices(*gridVertices
        , Vector2f(0.f, -10000.f)
        , Vector2f(0.f, 10000.f));
    AppendLineVertices(*gridVertices
        , Vector2f(-10000.f, 0.f)
        , Vector2f(10000.f, 0.f));

    ElementSFDrawable* gridElement = sceneRoot->AddChild<ElementSFDrawable>();
    gridElement->SetSFDrawable(gridVertices, gridVertices->getBounds());

    // Prepare Scene Particles
    for (size_t i = 0; i < 10; ++i)
    {
        ParticleSystemSettings settings;
        settings.maxParticleCount = 500;
        settings.minSpawnPerSecond = 500;
        settings.updateColorOverLifetime = true;
        settings.useSortBuffer = true;

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
    AppendLineVertices(*armShape
        , Vector2f(0.f, 0.f)
        , Vector2f(300.f, 0.f));
    AppendLineVertices(*armShape
        , Vector2f(300.f, 0.f)
        , Vector2f(300.f, -50.f));
    AppendLineVertices(*armShape
        , Vector2f(0.f, 0.f)
        , Vector2f(-400.f, 0.f));
    AppendLineVertices(*armShape
        , Vector2f(-400.f, 0.f)
        , Vector2f(-400.f, 50.f));

    ElementSFDrawable* arm = sceneRoot->AddChild<ElementSFDrawable>();
    arm->SetPosition(0.f, 50.f);
    arm->SetOrigin(50.f, 0.f);
    arm->SetSFDrawable(armShape, armShape->getBounds());
    m_moveArm = arm;

    // Arm Particle
    ParticleSystemSettings armSettings;
    armSettings.localSpace = false;
    armSettings.maxParticleCount = 1000;
    armSettings.emissionBehaviour = ParticleSystemSettings::EEmissionBehaviour::AngleDirection;
    armSettings.emissionDirection = Vector2f(0.f, -1.f);
    armSettings.emissionAngle = 45.f;
    armSettings.minSpawnPerSecond = 100.f;
    armSettings.minParticlesPerSpawn = 3;
    armSettings.minLifetime = 1500;
    armSettings.minVelocity = 500;
    armSettings.useRandomStartSize = true;
    armSettings.minStartSize = Vector2f(5.f, 5.f);
    armSettings.maxStartSize = Vector2f(10.f, 10.f);
    armSettings.updateSizeOverLifetime = true;
    armSettings.startColor = sf::Color::Yellow;
    armSettings.endColor = sf::Color::Red;
    armSettings.updateColorOverLifetime = true;
    armSettings.useSortBuffer = true;

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
    armSettings2.useSortBuffer = true;

    ElementParticles* armElementParticle2 = arm->AddChild<ElementParticles>();
    armElementParticle2->SetPositionX(-400.f);
    armElementParticle2->SetOrigin(0.f, -50.f);
    ParticleSystem* armParticleSystem2 = armElementParticle2->CreateParticleSystem(armSettings2, true);

    m_particleSystemSettings.push_back(armSettings2);
    m_particleSystems.push_back(armParticleSystem2);

    // Cursor Particle
    ParticleSystemSettings cursorSettings;
    cursorSettings.updateColorOverLifetime = true;
    cursorSettings.useSortBuffer = true;

    ElementParticles* cursorElementParticle = m_root->AddChild<ElementParticles>();
    ParticleSystem* cursorParticleSystem = cursorElementParticle->CreateParticleSystem(cursorSettings, true);
    m_mouseFollow = cursorElementParticle;

    m_particleSystemSettings.push_back(cursorSettings);
    m_particleSystems.push_back(cursorParticleSystem);
    
    // Animation
    m_animationTime = 0.f;
    m_animateEmitters = true;
    m_rotateArm = true;
}

void Demo::AppStop()
{
    SafeDelete(m_root);
}

void Demo::AppUpdateImGui(const DeltaTime& dt)
{
    if (m_animateEmitters)
    {
        m_animationTime += dt.s();

        for (size_t i = 0; i < m_centerParticleElements.size(); ++i)
        {
            float curve = std::sin(m_animationTime) * 100.f;
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
