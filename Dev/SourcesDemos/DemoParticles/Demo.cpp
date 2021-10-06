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
    m_particleSystem->SetEmitterPosition(GetPosition());
    m_particleSystem->Update(dt);
}

void ElementParticleSystem::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    m_particleSystem->Render(_kRenderPass, _kTransformSelf);
}

void ParticleSystem::Init()
{
    m_running = false;

    m_loop = true;
    m_particleCount = 1000;
    m_verticesPerParticle = 6;

    m_dataVertices.setPrimitiveType(m_verticesPerParticle == 6 ? sf::PrimitiveType::Triangles : sf::PrimitiveType::Points);
    m_dataVertices.resize(m_particleCount * m_verticesPerParticle);

    m_dataLifetime.resize(m_particleCount);
    m_dataRemainingTime.resize(m_particleCount);
    m_dataVelocity.resize(m_particleCount);
}

void ParticleSystem::Start()
{
    m_running = true;

    for (size_t i = 0; i < m_particleCount; ++i)
    {
        if (m_verticesPerParticle == 6)
        {
            m_dataVertices[i * 6 + 0].position = m_emitterPosition + Vector2f(0.f, 0.f);
            m_dataVertices[i * 6 + 1].position = m_emitterPosition + Vector2f(10.f, 0.f);
            m_dataVertices[i * 6 + 2].position = m_emitterPosition + Vector2f(0.f, 10.f);
            m_dataVertices[i * 6 + 3].position = m_emitterPosition + Vector2f(10.f, 0.f);
            m_dataVertices[i * 6 + 4].position = m_emitterPosition + Vector2f(0.f, 10.f);
            m_dataVertices[i * 6 + 5].position = m_emitterPosition + Vector2f(10.f, 10.f);
        }
        else
        {
            m_dataVertices[i].position = m_emitterPosition + Vector2f(0.f, 0.f);
        }

        for (size_t ii = 0; ii < m_verticesPerParticle; ++ii)
        {
            m_dataVertices[i * m_verticesPerParticle + ii].color = sf::Color(0, 0, 0, 255);
        }

        m_dataLifetime[i] = GetRandom(2000, 3000);
        m_dataRemainingTime[i] = m_dataLifetime[i];
        m_dataVelocity[i] = Vector2f(GetRandomf(-70.f, 70.f), GetRandomf(-70.f, 70.f));
    }
}

void ParticleSystem::SetEmitterPosition(const Vector2f& position)
{
    m_emitterPosition = position;
}

void ParticleSystem::Update(const DeltaTime& dt)
{
    if (!m_running)
        return;

    for (size_t i = 0; i < m_particleCount; ++i)
    {
        // Ignore destroyed particles
        if (m_dataLifetime[i] <= 0)
            continue;

        m_dataRemainingTime[i] -= dt.ms();

        if (m_dataRemainingTime[i] <= 0)
        {
            if (m_loop)
            {
                // Reset position
                if (m_verticesPerParticle == 6)
                {
                    m_dataVertices[i * 6 + 0].position = m_emitterPosition + Vector2f(0.f, 0.f);
                    m_dataVertices[i * 6 + 1].position = m_emitterPosition + Vector2f(10.f, 0.f);
                    m_dataVertices[i * 6 + 2].position = m_emitterPosition + Vector2f(0.f, 10.f);
                    m_dataVertices[i * 6 + 3].position = m_emitterPosition + Vector2f(10.f, 0.f);
                    m_dataVertices[i * 6 + 4].position = m_emitterPosition + Vector2f(0.f, 10.f);
                    m_dataVertices[i * 6 + 5].position = m_emitterPosition + Vector2f(10.f, 10.f);
                }
                else
                {
                    m_dataVertices[i].position = m_emitterPosition + Vector2f(0.f, 0.f);
                }

                // Reset color
                for (size_t ii = 0; ii < m_verticesPerParticle; ++ii)
                {
                    m_dataVertices[i * m_verticesPerParticle + ii].color = sf::Color(0, 0, 0, 255);
                }

                // Reset lifetime and velocity
                m_dataLifetime[i] = GetRandom(2000, 3000);
                m_dataRemainingTime[i] = m_dataLifetime[i];
                m_dataVelocity[i] = Vector2f(GetRandomf(-70.f, 70.f), GetRandomf(-70.f, 70.f));
            }
            else
            {
                // Destroy
                m_dataLifetime[i] = 0;

                // Reset position
                if (m_verticesPerParticle == 6)
                {
                    m_dataVertices[i * 6 + 0].position = m_emitterPosition + Vector2f(0.f, 0.f);
                    m_dataVertices[i * 6 + 1].position = m_emitterPosition + Vector2f(10.f, 0.f);
                    m_dataVertices[i * 6 + 2].position = m_emitterPosition + Vector2f(0.f, 10.f);
                    m_dataVertices[i * 6 + 3].position = m_emitterPosition + Vector2f(10.f, 0.f);
                    m_dataVertices[i * 6 + 4].position = m_emitterPosition + Vector2f(0.f, 10.f);
                    m_dataVertices[i * 6 + 5].position = m_emitterPosition + Vector2f(10.f, 10.f);
                }
                else
                {
                    m_dataVertices[i].position = m_emitterPosition + Vector2f(0.f, 0.f);
                }

                // Hide
                for (size_t ii = 0; ii < m_verticesPerParticle; ++ii)
                {
                    m_dataVertices[i * m_verticesPerParticle + ii].color = sf::Color::Transparent;
                }
            }
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
}

void ParticleSystem::Render(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    if (!m_running)
        return;

    sf::RenderStates states;
    //states.transform = _kTransformSelf;
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
    }

    ParticleSystem* particleSystem = new ParticleSystem;
    particleSystem->Init();

    ElementParticleSystem* particleRenderer = m_root->AddChild<ElementParticleSystem>();
    particleRenderer->SetParticleSystem(particleSystem);
    m_mouseFollow = particleRenderer;

    particleSystem->Start();
}

void Demo::AppStop()
{
    SafeDelete(m_root);
}

void Demo::AppUpdate(const DeltaTime& dt)
{
    m_mouseFollow->SetPosition(GetGameWindow()->GetMousePosition());
}

}   //namespace demoproject
