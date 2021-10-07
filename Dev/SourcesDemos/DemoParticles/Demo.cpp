////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Window.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Element/2D/ElementSFDrawable.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/Random.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Window/Renderer.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include <imgui.h>

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementParticleSystem::ElementParticleSystem()
    : m_particleSystem(nullptr)
{
}

ElementParticleSystem::~ElementParticleSystem()
{
    SafeDelete(m_particleSystem);
}

void ElementParticleSystem::SetParticleSystem(ParticleSystem* particleSystem)
{
    SafeDelete(m_particleSystem);

    m_particleSystem = particleSystem;
    m_particleSystem->SetEmitterPosition(GetPosition());
}

void ElementParticleSystem::Update(const DeltaTime& dt)
{
    // TODO: Maybe this should be inside the particle system.
    m_particleSystem->SetEmitterPosition(GetPosition());
    m_particleSystem->Update(dt);
}

void ElementParticleSystem::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    m_particleSystem->Render(_kRenderPass, _kTransformSelf);
}

ParticleSystem::ParticleSystem()
    : m_running(false)
    , m_nextEmitIndex(0)
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Init()
{
    Stop();

    m_loop = true;
    m_maxParticleCount = 1000;
    m_verticesPerParticle = 6;
    m_emitCountPerCycle = 5;
    m_applyRenderLocalTransform = false;

    m_dataVertices.setPrimitiveType(m_verticesPerParticle == 6 ? sf::PrimitiveType::Triangles : sf::PrimitiveType::Points);
    m_dataVertices.resize(m_maxParticleCount * m_verticesPerParticle);

    m_dataLifetime.resize(m_maxParticleCount, 0);
    m_dataRemainingTime.resize(m_maxParticleCount, 0);
    m_dataVelocity.resize(m_maxParticleCount);
}

void ParticleSystem::Start()
{
    if (m_running || m_dataVertices.getVertexCount() == 0)
        return;

    m_running = true;

    size_t i = 0;
    while (i < Min(m_emitCountPerCycle, m_maxParticleCount))
    {
        EmitParticle(i);
        ++i;
    }

    m_nextEmitIndex = i % m_maxParticleCount;
}

void ParticleSystem::Stop()
{
    if (!m_running)
        return;

    m_running = false;
    m_nextEmitIndex = 0;

    for (size_t i = 0; i < m_maxParticleCount; ++i)
    {
        ResetParticle(i);
    }
}

void ParticleSystem::Restart()
{
    Stop();
    Start();
}

void ParticleSystem::SetEmitterPosition(const Vector2f& position)
{
    m_emitterPosition = position;
}

size_t ParticleSystem::GetMaxParticleCount() const
{
    return m_maxParticleCount;
}

size_t ParticleSystem::GetActiveParticleCount() const
{
    if (!m_running)
        return 0;

    size_t count = 0;
    for (size_t i = 0; i < m_maxParticleCount; ++i)
    {
        if (m_dataLifetime[i] > 0)
        {
            ++count;
        }
    }

    return count;
}

void ParticleSystem::EmitParticle(size_t particleIndex)
{
    // Reset lifetime.
    m_dataLifetime[particleIndex] = GetRandom(2000, 3000);
    m_dataRemainingTime[particleIndex] = m_dataLifetime[particleIndex];

    // Reset position.
    if (m_verticesPerParticle == 6)
    {
        m_dataVertices[particleIndex * 6 + 0].position = m_emitterPosition + Vector2f(-5.f, -5.f);
        m_dataVertices[particleIndex * 6 + 1].position = m_emitterPosition + Vector2f(5, -5.f);
        m_dataVertices[particleIndex * 6 + 2].position = m_emitterPosition + Vector2f(-5.f, 5);
        m_dataVertices[particleIndex * 6 + 3].position = m_emitterPosition + Vector2f(5, -5.f);
        m_dataVertices[particleIndex * 6 + 4].position = m_emitterPosition + Vector2f(-5.f, 5);
        m_dataVertices[particleIndex * 6 + 5].position = m_emitterPosition + Vector2f(5, 5);
    }
    else
    {
        m_dataVertices[particleIndex].position = m_emitterPosition + Vector2f(0.f, 0.f);
    }

    // Reset color.
    for (size_t ii = 0; ii < m_verticesPerParticle; ++ii)
    {
        m_dataVertices[particleIndex * m_verticesPerParticle + ii].color = sf::Color(0, 0, 0, 255);
    }

    // Reset velocity.
    Vector2f velocity = Vector2f(GetRandomf(10.f, 70.f), 0.f);
    velocity = Rotate(velocity, GetRandomf(Math::Pi * 2.f));
    m_dataVelocity[particleIndex] = velocity;
}

void ParticleSystem::ResetParticle(size_t particleIndex)
{
    // Disable particle.
    m_dataLifetime[particleIndex] = 0;

    // Reset position and hide vertices.
    for (size_t ii = 0; ii < m_verticesPerParticle; ++ii)
    {
        m_dataVertices[particleIndex * m_verticesPerParticle + ii].position = m_emitterPosition + Vector2f(0.f, 0.f);
        m_dataVertices[particleIndex * m_verticesPerParticle + ii].color = sf::Color::Transparent;
    }
}

void ParticleSystem::Update(const DeltaTime& dt)
{
    if (!m_running)
        return;

    for (size_t i = 0; i < m_maxParticleCount; ++i)
    {
        // Ignore disabled particles
        if (m_dataLifetime[i] <= 0)
            continue;

        m_dataRemainingTime[i] -= dt.ms();

        if (m_dataRemainingTime[i] <= 0)
        {
            //if (m_loop)
            //{
            //    EmitParticle(i);
            //}
            //else
            //{
                ResetParticle(i);
            //}
        }
        else
        {
            float vx = m_dataVelocity[i].x * dt.s();
            float vy = m_dataVelocity[i].y * dt.s();
            sf::Color color = sf::Color(0, 0, 0, Lerp<sf::Uint8>(0, 255, (float)m_dataRemainingTime[i] / (float)m_dataLifetime[i]));

            for (size_t ii = 0; ii < m_verticesPerParticle; ++ii)
            {
                m_dataVertices[i * m_verticesPerParticle + ii].position.x += vx;
                m_dataVertices[i * m_verticesPerParticle + ii].position.y += vy;
                m_dataVertices[i * m_verticesPerParticle + ii].color = color;
            }
        }
    }

    // TODO: Stop running once all particles are disabled.
    if (m_loop || m_nextEmitIndex > 0)
    {
        size_t i = m_nextEmitIndex;
        size_t findCount = 0;
        size_t emitCount = 0;
        bool findNextAvailableParticle = true;
        while (findNextAvailableParticle)
        {
            if (m_dataLifetime[i] <= 0)
            {
                EmitParticle(i);
                ++emitCount;
            }

            i = (i + 1) % m_maxParticleCount;

            ++findCount;
            findNextAvailableParticle = (findCount < m_maxParticleCount) && (emitCount < m_emitCountPerCycle) && (m_loop || i != 0);
        }

        m_nextEmitIndex = i;
    }
}

void ParticleSystem::Render(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    if (!m_running)
        return;

    sf::RenderStates states;
    if (m_applyRenderLocalTransform)
    {
        states.transform = _kTransformSelf;
    }

    _kRenderPass.target->draw(m_dataVertices, states);

    if (m_verticesPerParticle == 6)
    {
        _kRenderPass.frameInfos->statDrawCalls += 1;
        _kRenderPass.frameInfos->statTriangles += m_dataVertices.getVertexCount() / 3;
    }
    else
    {
        // TODO: vertex count instead of triangle count ?
        _kRenderPass.frameInfos->statDrawCalls += 1;
        _kRenderPass.frameInfos->statTriangles += m_dataVertices.getVertexCount();
    }
}

}   //namespace gugu


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

    for (size_t i = 0; i < 10; ++i)
    {
        ParticleSystem* particleSystem = new ParticleSystem;
        particleSystem->Init();

        ElementParticleSystem* particleRenderer = m_root->AddChild<ElementParticleSystem>();
        particleRenderer->SetUnifiedPosition(UDim2::POSITION_CENTER + Vector2f(-300 + (i % 5) * 150.f, -100 + (i / 5) * 200.f));
        particleRenderer->SetParticleSystem(particleSystem);

        particleSystem->Start();

        m_particleSystems.push_back(particleSystem);
    }

    m_cursorParticleSystem = new ParticleSystem;
    m_cursorParticleSystem->Init();

    ElementParticleSystem* particleRenderer = m_root->AddChild<ElementParticleSystem>();
    particleRenderer->SetParticleSystem(m_cursorParticleSystem);
    m_mouseFollow = particleRenderer;

    m_cursorParticleSystem->Start();

    m_particleSystems.push_back(m_cursorParticleSystem);
}

void Demo::AppStop()
{
    SafeDelete(m_root);
}

void Demo::AppUpdate(const DeltaTime& dt)
{
    m_mouseFollow->SetPosition(GetGameWindow()->GetMousePosition());

    if (ImGui::Begin("Particles"))
    {
        ImGui::Text("Cursor ParticleSystem");
        ImGui::Text("- Active Particles : %d/%d", m_cursorParticleSystem->GetActiveParticleCount(), m_cursorParticleSystem->GetMaxParticleCount());

        ImGui::Separator();
        if (ImGui::Button("Restart All"))
        {
            for (ParticleSystem* particleSystem : m_particleSystems)
            {
                particleSystem->Restart();
            }
        }
    }
    ImGui::End();
}

}   //namespace demoproject
