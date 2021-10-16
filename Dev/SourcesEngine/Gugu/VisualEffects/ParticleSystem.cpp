////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/VisualEffects/ParticleSystem.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/Random.h"
#include "Gugu/Math/MathUtility.h"

#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ParticleSystem::ParticleSystem()
    : m_running(false)
    , m_activeParticleCount(0)
    , m_currentDuration(0)
    , m_nextEmitIndex(0)
    , m_nextSpawnDelay(0)
    , m_currentSpawnDelay(0)
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Init(const ParticleSystemSettings& settings)
{
    Release();

    m_settings = settings;

    m_maxParticleCount = settings.maxParticleCount;
    m_verticesPerParticle = settings.verticesPerParticle;

    m_dataVertices.setPrimitiveType(m_verticesPerParticle == 6 ? sf::PrimitiveType::Triangles : sf::PrimitiveType::Points);
    m_dataVertices.resize(m_maxParticleCount * m_verticesPerParticle);

    m_dataLifetime.resize(m_maxParticleCount, 0);
    m_dataRemainingTime.resize(m_maxParticleCount, 0);
    m_dataVelocity.resize(m_maxParticleCount);

    if (m_settings.imageSetID != "")
    {
        ImageSet* imageSet = GetResources()->GetImageSet(m_settings.imageSetID);
        if (imageSet && imageSet->GetSubImageCount() > 0 && imageSet->GetTexture())
        {
            m_imageSet = imageSet;
            m_texture = m_imageSet->GetTexture()->GetSFTexture();
        }
    }
}

void ParticleSystem::Release()
{
    Stop();

    m_imageSet = nullptr;
    m_texture = nullptr;
}

void ParticleSystem::Start()
{
    if (m_running || m_dataVertices.getVertexCount() == 0)
        return;

    if (m_element && !m_settings.localSpace)
    {
        SetEmitterPosition(m_element->TransformToGlobalFull(Vector2f()));
    }
    else
    {
        SetEmitterPosition(Vector2f());
    }

    m_running = true;
    m_currentDuration = 0;

    // TODO: I should refactor this to avoid duplicated code from the Update.
    // TODO: Delay parameter ?
    size_t i = 0;
    size_t emitCount = GetRandom(m_settings.minParticlesPerSpawn, m_settings.maxParticlesPerSpawn);
    while (i < Min(emitCount, m_maxParticleCount))
    {
        EmitParticle(i);
        ++i;
    }

    m_nextEmitIndex = i % m_maxParticleCount;
    m_currentSpawnDelay = 0;

    float randValue = GetRandomf(m_settings.minSpawnPerSecond, m_settings.maxSpawnPerSecond);
    m_nextSpawnDelay = (int)(1000.f / Max(Math::Epsilon, randValue));
}

void ParticleSystem::Stop()
{
    if (!m_running)
        return;

    m_running = false;
    m_activeParticleCount = 0;
    m_currentDuration = 0;
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

void ParticleSystem::AttachToElement(Element* element)
{
    m_element = element;
}

void ParticleSystem::SetEmitterPosition(const Vector2f& position)
{
    m_emitterPosition = position;
}

bool ParticleSystem::IsRunning() const
{
    return m_running;
}

size_t ParticleSystem::GetMaxParticleCount() const
{
    return m_maxParticleCount;
}

size_t ParticleSystem::GetActiveParticleCount() const
{
    return m_activeParticleCount;
}

void ParticleSystem::EmitParticle(size_t particleIndex)
{
    ++m_activeParticleCount;

    // Reset lifetime.
    int lifetime = GetRandom(m_settings.minLifetime, m_settings.maxLifetime);
    m_dataLifetime[particleIndex] = lifetime;
    m_dataRemainingTime[particleIndex] = lifetime;

    // Reset position.
    if (m_verticesPerParticle == 6)
    {
        float x = 0.f;
        float y = 0.f;
        if (m_settings.keepSizeRatio)
        {
            float ratio = GetRandomf(1.f);
            x = Lerp(m_settings.minStartSize.x, m_settings.maxStartSize.x, ratio);
            y = Lerp(m_settings.minStartSize.y, m_settings.maxStartSize.y, ratio);
        }
        else
        {
            x = GetRandomf(m_settings.minStartSize.x, m_settings.maxStartSize.x);
            y = GetRandomf(m_settings.minStartSize.y, m_settings.maxStartSize.y);
        }

        x *= 0.5f;
        y *= 0.5f;

        m_dataVertices[particleIndex * 6 + 0].position = m_emitterPosition + Vector2f(-x, -y);
        m_dataVertices[particleIndex * 6 + 1].position = m_emitterPosition + Vector2f(x, -y);
        m_dataVertices[particleIndex * 6 + 2].position = m_emitterPosition + Vector2f(-x, y);
        m_dataVertices[particleIndex * 6 + 3].position = m_emitterPosition + Vector2f(x, -y);
        m_dataVertices[particleIndex * 6 + 4].position = m_emitterPosition + Vector2f(-x, y);
        m_dataVertices[particleIndex * 6 + 5].position = m_emitterPosition + Vector2f(x, y);
    }
    else
    {
        m_dataVertices[particleIndex].position = m_emitterPosition + Vector2f(0.f, 0.f);
    }

    if (m_imageSet)
    {
        SubImage* subImage = m_imageSet->GetSubImage(GetRandom(m_imageSet->GetSubImageCount()));

        sf::IntRect subRect = subImage->GetRect();
        float fLeft = (float)subRect.left;
        float fTop = (float)subRect.top;
        float fRight = fLeft + subRect.width;
        float fBottom = fTop + subRect.height;

        m_dataVertices[particleIndex * 6 + 0].texCoords = Vector2f(fLeft, fTop);
        m_dataVertices[particleIndex * 6 + 1].texCoords = Vector2f(fRight, fTop);
        m_dataVertices[particleIndex * 6 + 2].texCoords = Vector2f(fLeft, fBottom);
        m_dataVertices[particleIndex * 6 + 3].texCoords = Vector2f(fRight, fTop);
        m_dataVertices[particleIndex * 6 + 4].texCoords = Vector2f(fLeft, fBottom);
        m_dataVertices[particleIndex * 6 + 5].texCoords = Vector2f(fRight, fBottom);
    }

    // Reset color.
    for (size_t ii = 0; ii < m_verticesPerParticle; ++ii)
    {
        m_dataVertices[particleIndex * m_verticesPerParticle + ii].color = m_settings.startColor;
    }

    // Reset velocity.
    Vector2f velocity = Vector2f(GetRandomf(m_settings.minVelocity, m_settings.maxVelocity), 0.f);
    velocity = Rotate(velocity, GetRandomf(Math::Pi * 2.f));
    m_dataVelocity[particleIndex] = velocity;
}

void ParticleSystem::KillParticle(size_t particleIndex)
{
    --m_activeParticleCount;

    ResetParticle(particleIndex);
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

    if (m_element && !m_settings.localSpace)
    {
        SetEmitterPosition(m_element->TransformToGlobalFull(Vector2f()));
    }
    else
    {
        SetEmitterPosition(Vector2f());
    }

    // Update live particles.
    for (size_t i = 0; i < m_maxParticleCount; ++i)
    {
        // Ignore disabled particles.
        if (m_dataLifetime[i] <= 0)
            continue;

        m_dataRemainingTime[i] -= dt.ms();

        if (m_dataRemainingTime[i] <= 0)
        {
            KillParticle(i);
        }
        else
        {
            float vx = m_dataVelocity[i].x * dt.s();
            float vy = m_dataVelocity[i].y * dt.s();

            float lerpValue = (float)m_dataRemainingTime[i] / (float)m_dataLifetime[i];
            sf::Color color = sf::Color(
                Lerp<sf::Uint8>(m_settings.endColor.r, m_settings.startColor.r, lerpValue),
                Lerp<sf::Uint8>(m_settings.endColor.g, m_settings.startColor.g, lerpValue),
                Lerp<sf::Uint8>(m_settings.endColor.b, m_settings.startColor.b, lerpValue),
                Lerp<sf::Uint8>(m_settings.endColor.a, m_settings.startColor.a, lerpValue));

            for (size_t ii = 0; ii < m_verticesPerParticle; ++ii)
            {
                m_dataVertices[i * m_verticesPerParticle + ii].position.x += vx;
                m_dataVertices[i * m_verticesPerParticle + ii].position.y += vy;
                m_dataVertices[i * m_verticesPerParticle + ii].color = color;
            }
        }
    }

    // Check duration end.
    bool canEmit = true;

    if (!m_settings.loop)
    {
        if (m_currentDuration >= m_settings.duration)
        {
            canEmit = false;
            m_running = m_activeParticleCount > 0;
        }
        else
        {
            m_currentDuration += dt.ms();
        }
    }

    // Emit new particles.
    if (canEmit)
    {
        int nbSpawns = 0;

        if (dt.ms() >= m_nextSpawnDelay)
        {
            // This case triggers if delay <= dt (high spawn rate).
            nbSpawns = Max(1, (int)(dt.s() * GetRandomf(m_settings.minSpawnPerSecond, m_settings.maxSpawnPerSecond)));
            m_currentSpawnDelay += dt.ms() - m_nextSpawnDelay;

            float randValue = GetRandomf(m_settings.minSpawnPerSecond, m_settings.maxSpawnPerSecond);
            m_nextSpawnDelay = (int)(1000.f / Max(Math::Epsilon, randValue));
        }
        else
        {
            // This case triggers if delay > dt (low spawn rate).
            m_currentSpawnDelay += dt.ms();
            if (m_currentSpawnDelay >= m_nextSpawnDelay)
            {
                nbSpawns = 1;
                m_currentSpawnDelay -= m_nextSpawnDelay;

                float randValue = GetRandomf(m_settings.minSpawnPerSecond, m_settings.maxSpawnPerSecond);
                m_nextSpawnDelay = (int)(1000.f / Max(Math::Epsilon, randValue));
            }
        }

        if (nbSpawns > 0)
        {
            for (int n = 0; n < nbSpawns; ++n)
            {
                size_t i = m_nextEmitIndex;
                size_t findCount = 0;
                size_t emitCount = 0;
                size_t emitLimit = GetRandom(m_settings.minParticlesPerSpawn, m_settings.maxParticlesPerSpawn);
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
                    findNextAvailableParticle = (findCount < m_maxParticleCount) && (emitCount < emitLimit);
                }

                m_nextEmitIndex = i;
            }
        }
    }
}

void ParticleSystem::Render(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    if (!m_running)
        return;

    sf::RenderStates states;
    states.texture = m_texture;

    if (m_settings.localSpace)
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
