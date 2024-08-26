////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/VisualEffects/ParticleSystem.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ParticleEffect.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/Math/Random.h"
#include "Gugu/Math/MathUtility.h"

#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ParticleSystem::ParticleSystem()
    : m_maxParticleCount(0)
    , m_verticesPerParticle(0)
    , m_primitiveType(sf::PrimitiveType::Points)
    , m_imageSet(nullptr)
    , m_texture(nullptr)
    , m_element(nullptr)
    , m_emitterRotation(0.f)
    , m_running(false)
    , m_stopEmitting(false)
    , m_paused(false)
    , m_currentDuration(0.f)
    , m_activeParticleCount(0)
    , m_nextEmitIndex(0)
    , m_nextSpawnDelay(0.f)
    , m_currentSpawnDelay(0.f)
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::SanitizeSettings(ParticleSystemSettings& settings, bool limitsOnly)
{
    settings.duration = Max(settings.duration, 0);
    settings.maxParticleCount = Clamp(settings.maxParticleCount, 1, 100000);

    settings.minSpawnPerSecond = Max(settings.minSpawnPerSecond, math::Epsilon6);
    settings.minParticlesPerSpawn = Max(settings.minParticlesPerSpawn, 0);

    settings.emissionAngle = Clamp(settings.emissionAngle, 0.f, 360.f);
    settings.minLifetime = Max(settings.minLifetime, 1);

    // TODO: split in two methods.
    if (!limitsOnly)
    {
        settings.maxSpawnPerSecond = settings.useRandomSpawnPerSecond ? Max(settings.minSpawnPerSecond, settings.maxSpawnPerSecond) : settings.minSpawnPerSecond;
        settings.maxParticlesPerSpawn = settings.useRandomParticlesPerSpawn ? Max(settings.minParticlesPerSpawn, settings.maxParticlesPerSpawn) : settings.minParticlesPerSpawn;
        
        settings.emissionDirection = (settings.emissionDirection != Vector2::Zero_f) ? Normalize(settings.emissionDirection) : Vector2f(0.f, -1.f);
        settings.maxLifetime = settings.useRandomLifetime ? Max(settings.minLifetime, settings.maxLifetime) : settings.minLifetime;

        float minVelocity = settings.minVelocity;
        float maxVelocity = settings.maxVelocity;
        settings.minVelocity = settings.useRandomVelocity ? Min(minVelocity, maxVelocity) : minVelocity;
        settings.maxVelocity = settings.useRandomVelocity ? Max(minVelocity, maxVelocity) : minVelocity;

        settings.maxStartSize.x = settings.useRandomStartSize ? Max(settings.minStartSize.x, settings.maxStartSize.x) : settings.minStartSize.x;
        settings.maxStartSize.y = settings.useRandomStartSize ? Max(settings.minStartSize.y, settings.maxStartSize.y) : settings.minStartSize.y;
        settings.maxEndSize.x = settings.useRandomEndSize ? Max(settings.minEndSize.x, settings.maxEndSize.x) : settings.minEndSize.x;
        settings.maxEndSize.y = settings.useRandomEndSize ? Max(settings.minEndSize.y, settings.maxEndSize.y) : settings.minEndSize.y;
    }
    else
    {
        settings.maxSpawnPerSecond = Max(settings.maxSpawnPerSecond, 0.f);
        settings.maxParticlesPerSpawn = Max(settings.maxParticlesPerSpawn, 0);

        settings.maxLifetime = Max(settings.maxLifetime, 0);
    }
}

void ParticleSystem::Init(ParticleEffect* particleEffect)
{
    if (!particleEffect)
        return;

    Init(*particleEffect->GetParticleSettings());
}

void ParticleSystem::Init(const ParticleSystemSettings& settings)
{
    Stop();

    // Copy and sanitize settings.
    m_settings = settings;
    SanitizeSettings(m_settings, false);

    m_maxParticleCount = m_settings.maxParticleCount;

    if (m_settings.particleShape == ParticleSystemSettings::EParticleShape::Point)
    {
        m_verticesPerParticle = 1;
        m_primitiveType = sf::PrimitiveType::Points;
    }
    else if (m_settings.particleShape == ParticleSystemSettings::EParticleShape::Quad)
    {
        m_verticesPerParticle = 6;
        m_primitiveType = sf::PrimitiveType::Triangles;
    }

    m_dataVertices.resize(m_maxParticleCount* m_verticesPerParticle);
    m_sortBuffer.resize(m_settings.useSortBuffer ? m_maxParticleCount * m_verticesPerParticle : 0);
    m_dataLifetime.resize(m_maxParticleCount, 0.f);
    m_dataRemainingTime.resize(m_maxParticleCount, 0.f);
    m_dataPosition.resize(m_maxParticleCount);
    m_dataStartSize.resize(m_maxParticleCount);
    m_dataEndSize.resize(m_maxParticleCount);
    m_dataVelocity.resize(m_maxParticleCount);

    m_imageSet = nullptr;
    m_texture = nullptr;

    if (m_settings.imageSet && m_settings.imageSet->GetSubImageCount() > 0 && m_settings.imageSet->GetTexture())
    {
        m_imageSet = m_settings.imageSet;
        m_texture = m_imageSet->GetTexture()->GetSFTexture();
    }
}

void ParticleSystem::Start()
{
    if (m_running || m_dataVertices.empty())
        return;

    UpdateEmitterPosition();

    // Reset all particles.
    std::fill(m_dataLifetime.begin(), m_dataLifetime.end(), 0.f);
    std::fill(m_dataVertices.begin(), m_dataVertices.end(), sf::Vertex(sf::Vector2f(), sf::Color::Transparent, sf::Vector2f()));

    m_running = true;
    m_paused = false;
    m_currentDuration = 0.f;
    
    // TODO: Delay parameter ?
    // TODO: Wait for the update to trigger a pending StartImpl ? to ensure we are not polluting Step.
    // TODO: I should refactor this to avoid duplicated code from the Update.
    // - The current implementation will slightly mess with particles lifetime, since they will get updated the same frame they are spawned, and may disappear before the next render.
    // - I need to be careful with firing rate computations in emission handling in update if I provide zero delays.

    size_t emitCount = Min(m_maxParticleCount, (size_t)GetRandom(m_settings.minParticlesPerSpawn, m_settings.maxParticlesPerSpawn));
    for (size_t i = 0; i < emitCount; ++i)
    {
        EmitParticle(i);
    }
    
    m_nextEmitIndex = emitCount % m_maxParticleCount;
    m_currentSpawnDelay = 0.f;

    float randValue = GetRandomf(m_settings.minSpawnPerSecond, m_settings.maxSpawnPerSecond);
    m_nextSpawnDelay = 1000.f / Max(math::Epsilon6, randValue);
}

void ParticleSystem::Stop()
{
    if (!m_running)
        return;

    // When m_running is false, the render is ignored, so we dont need to reset particles data here.
    m_running = false;
    m_paused = false;
    m_activeParticleCount = 0;
    m_currentDuration = 0.f;
    m_nextEmitIndex = 0;
}

void ParticleSystem::Restart()
{
    Stop();
    Start();
}

void ParticleSystem::AttachToElement(Element* element)
{
    m_element = element;
    UpdateEmitterPosition();
}

void ParticleSystem::SetEmitterPosition(const Vector2f& position)
{
    m_emitterPosition = position;
}

bool ParticleSystem::IsRunning() const
{
    return m_running;
}

void ParticleSystem::StopEmitting()
{
    m_stopEmitting = true;
}

void ParticleSystem::SetPaused(bool paused)
{
    m_paused = paused;
}

bool ParticleSystem::IsPaused() const
{
    return m_paused;
}

size_t ParticleSystem::GetMaxParticleCount() const
{
    return m_maxParticleCount;
}

size_t ParticleSystem::GetActiveParticleCount() const
{
    return m_activeParticleCount;
}

size_t ParticleSystem::GetParticleDataSize() const
{
    size_t total = 0;
    total += m_dataVertices.size() * sizeof(sf::Vertex);
    total += m_sortBuffer.size() * sizeof(sf::Vertex);
    total += m_dataLifetime.size() * sizeof(float);
    total += m_dataRemainingTime.size() * sizeof(float);
    total += m_dataPosition.size() * sizeof(Vector2f);
    total += m_dataStartSize.size() * sizeof(Vector2f);
    total += m_dataEndSize.size() * sizeof(Vector2f);
    total += m_dataVelocity.size() * sizeof(Vector2f);
    return total;
}

const ParticleSystemSettings& ParticleSystem::GetSettings() const
{
    return m_settings;
}

void ParticleSystem::EmitParticle(size_t particleIndex)
{
    ++m_activeParticleCount;

    // Reset lifetime.
    float lifetime = static_cast<float>(GetRandom(m_settings.minLifetime, m_settings.maxLifetime));
    m_dataLifetime[particleIndex] = lifetime;
    m_dataRemainingTime[particleIndex] = lifetime;

    // Reset position.
    Vector2f position = m_emitterPosition;
    if (m_settings.emitterShape == ParticleSystemSettings::EEmitterShape::Point)
    {
        m_dataPosition[particleIndex] = position;
    }
    else if (m_settings.emitterShape == ParticleSystemSettings::EEmitterShape::Circle)
    {
        position = GetRandomPointInCircle(position, m_settings.emitterRadius);
        m_dataPosition[particleIndex] = position;
    }
    else if (m_settings.emitterShape == ParticleSystemSettings::EEmitterShape::Annulus)
    {
        position = GetRandomPointInAnnulus(position, m_settings.emitterInnerRadius, m_settings.emitterRadius);
        m_dataPosition[particleIndex] = position;
    }

    // Reset vertices position.
    if (m_verticesPerParticle == 6)
    {
        Vector2f startSize, endSize;
        if (m_settings.keepSizeRatio)
        {
            float ratio = GetRandomf(1.f);
            startSize = Lerp(m_settings.minStartSize, m_settings.maxStartSize, ratio);
            endSize = Lerp(m_settings.minEndSize, m_settings.maxEndSize, ratio);
        }
        else
        {
            startSize.x = GetRandomf(m_settings.minStartSize.x, m_settings.maxStartSize.x);
            startSize.y = GetRandomf(m_settings.minStartSize.y, m_settings.maxStartSize.y);
            endSize.x = GetRandomf(m_settings.minEndSize.x, m_settings.maxEndSize.x);
            endSize.y = GetRandomf(m_settings.minEndSize.y, m_settings.maxEndSize.y);
        }

        m_dataStartSize[particleIndex] = startSize;
        m_dataEndSize[particleIndex] = endSize;

        float x = startSize.x * 0.5f;
        float y = startSize.y * 0.5f;

        sf::Vertex* vertices = &m_dataVertices[particleIndex * 6];
        vertices->position = position + Vector2f(-x, -y); ++vertices;
        vertices->position = position + Vector2f(x, -y); ++vertices;
        vertices->position = position + Vector2f(-x, y); ++vertices;
        vertices->position = position + Vector2f(x, -y); ++vertices;
        vertices->position = position + Vector2f(-x, y); ++vertices;
        vertices->position = position + Vector2f(x, y);
    }
    else
    {
        m_dataVertices[particleIndex].position = position + Vector2f(0.f, 0.f);
    }

    // Reset texture.
    if (m_imageSet)
    {
        SubImage* subImage = m_imageSet->GetSubImage(GetRandom(m_imageSet->GetSubImageCount()));

        sf::IntRect subRect = subImage->GetRect();
        float fLeft = (float)subRect.left;
        float fTop = (float)subRect.top;
        float fRight = fLeft + subRect.width;
        float fBottom = fTop + subRect.height;

        sf::Vertex* vertices = &m_dataVertices[particleIndex * 6];
        vertices->texCoords = Vector2f(fLeft, fTop); ++vertices;
        vertices->texCoords = Vector2f(fRight, fTop); ++vertices;
        vertices->texCoords = Vector2f(fLeft, fBottom); ++vertices;
        vertices->texCoords = Vector2f(fRight, fTop); ++vertices;
        vertices->texCoords = Vector2f(fLeft, fBottom); ++vertices;
        vertices->texCoords = Vector2f(fRight, fBottom);
    }

    // Reset color.
    sf::Vertex* vertices = &m_dataVertices[particleIndex * m_verticesPerParticle];
    for (size_t i = 0; i < m_verticesPerParticle; ++i)
    {
        vertices->color = m_settings.startColor;
        ++vertices;
    }

    // Reset velocity.
    if (m_settings.emissionBehaviour == ParticleSystemSettings::EEmissionBehaviour::RandomDirection)
    {
        Vector2f velocityVector = Vector2f(GetRandomf(m_settings.minVelocity, m_settings.maxVelocity), 0.f);
        velocityVector = Rotate(velocityVector, GetRandomf(math::Pi * 2.f));
        m_dataVelocity[particleIndex] = velocityVector;
    }
    else if (m_settings.emissionBehaviour == ParticleSystemSettings::EEmissionBehaviour::AngleDirection)
    {
        float velocityValue = GetRandomf(m_settings.minVelocity, m_settings.maxVelocity);
        Vector2f velocityVector = m_settings.emissionDirection * velocityValue;
        float emissionAngle = m_emitterRotation + GetRandomf(m_settings.emissionAngle) - (m_settings.emissionAngle * 0.5f);
        velocityVector = Rotate(velocityVector, ToRadiansf(emissionAngle));
        m_dataVelocity[particleIndex] = velocityVector;
    }
    else if (m_settings.emissionBehaviour == ParticleSystemSettings::EEmissionBehaviour::AwayFromCenter)
    {
        float velocityValue = GetRandomf(m_settings.minVelocity, m_settings.maxVelocity);
        Vector2f velocityVector = position - m_emitterPosition;
        velocityVector = velocityVector != Vector2::Zero_f ? Normalize(velocityVector) * velocityValue : Vector2f(0.f, -1.f) * velocityValue;
        m_dataVelocity[particleIndex] = velocityVector;
    }
}

void ParticleSystem::KillParticle(size_t particleIndex)
{
    --m_activeParticleCount;

    ResetParticle(particleIndex);
}

void ParticleSystem::ResetParticle(size_t particleIndex)
{
    // Disable particle.
    m_dataLifetime[particleIndex] = 0.f;

    sf::Vertex* vertices = &m_dataVertices[particleIndex * m_verticesPerParticle];
    for (size_t i = 0; i < m_verticesPerParticle; ++i)
    {
        vertices->position = m_emitterPosition;
        vertices->color = sf::Color::Transparent;
        ++vertices;
    }
}

void ParticleSystem::UpdateEmitterPosition()
{
    if (m_element && !m_settings.localSpace)
    {
        SetEmitterPosition(m_element->TransformToGlobal(Vector2::Zero_f));
        m_emitterRotation = 0.f;

        // TODO: GetGlobalRotation ?
        Element* ancestor = m_element;
        while (ancestor != nullptr)
        {
            m_emitterRotation += ancestor->GetRotation();
            ancestor = ancestor->GetParent();
        }
    }
    else
    {
        SetEmitterPosition(Vector2::Zero_f);
        m_emitterRotation = 0.f;
    }
}

void ParticleSystem::Update(const DeltaTime& updateDt)
{
    if (!m_running)
        return;

    UpdateEmitterPosition();

    if (m_paused)
        return;

    const DeltaTime dt = !m_settings.useUnscaledTime ? updateDt : DeltaTime(updateDt.GetUnscaledTime(), updateDt.GetUnscaledTime(), 1.f);

    // Update live particles.
    for (size_t i = 0; i < m_maxParticleCount; ++i)
    {
        float lifetime = m_dataLifetime[i];

        // Ignore disabled particles.
        if (ApproxInferiorOrEqualToZero(lifetime, math::Epsilon6))
            continue;

        float remainingLifetime = m_dataRemainingTime[i];
        remainingLifetime -= dt.ms();
        m_dataRemainingTime[i] = remainingLifetime;

        if (ApproxInferiorOrEqualToZero(remainingLifetime, math::Epsilon6))
        {
            KillParticle(i);
        }
        else
        {
            // Lifetime lerp (1 to 0).
            float lerpValue = remainingLifetime / lifetime;

            if (m_verticesPerParticle == 6 && m_settings.updateSizeOverLifetime)
            {
                Vector2f position = m_dataPosition[i] + m_dataVelocity[i] * dt.s();
                m_dataPosition[i] = position;

                const Vector2f& startSize = m_dataStartSize[i];
                const Vector2f& endSize = m_dataEndSize[i];
                float sx = Lerp(endSize.x, startSize.x, lerpValue) * 0.5f;
                float sy = Lerp(endSize.y, startSize.y, lerpValue) * 0.5f;

                sf::Vertex* vertices = &m_dataVertices[i * 6];
                vertices->position = position + Vector2f(-sx, -sy); ++vertices;
                vertices->position = position + Vector2f(sx, -sy); ++vertices;
                vertices->position = position + Vector2f(-sx, sy); ++vertices;
                vertices->position = position + Vector2f(sx, -sy); ++vertices;
                vertices->position = position + Vector2f(-sx, sy); ++vertices;
                vertices->position = position + Vector2f(sx, sy);
            }
            else if (m_verticesPerParticle == 6 && !m_settings.updateSizeOverLifetime)
            {
                Vector2f position = m_dataPosition[i] + m_dataVelocity[i] * dt.s();
                m_dataPosition[i] = position;

                const Vector2f& startSize = m_dataStartSize[i];
                float sx = startSize.x * 0.5f;
                float sy = startSize.y * 0.5f;

                sf::Vertex* vertices = &m_dataVertices[i * 6];
                vertices->position = position + Vector2f(-sx, -sy); ++vertices;
                vertices->position = position + Vector2f(sx, -sy); ++vertices;
                vertices->position = position + Vector2f(-sx, sy); ++vertices;
                vertices->position = position + Vector2f(sx, -sy); ++vertices;
                vertices->position = position + Vector2f(-sx, sy); ++vertices;
                vertices->position = position + Vector2f(sx, sy);
            }
            else
            {
                Vector2f position = m_dataPosition[i] + m_dataVelocity[i] * dt.s();
                m_dataPosition[i] = position;

                m_dataVertices[i].position = position;
            }

            if (m_settings.updateColorOverLifetime)
            {
                sf::Color color = sf::Color(
                    Lerp<sf::Uint8>(m_settings.endColor.r, m_settings.startColor.r, lerpValue),
                    Lerp<sf::Uint8>(m_settings.endColor.g, m_settings.startColor.g, lerpValue),
                    Lerp<sf::Uint8>(m_settings.endColor.b, m_settings.startColor.b, lerpValue),
                    Lerp<sf::Uint8>(m_settings.endColor.a, m_settings.startColor.a, lerpValue));

                sf::Vertex* vertices = &m_dataVertices[i * m_verticesPerParticle];
                for (size_t ii = 0; ii < m_verticesPerParticle; ++ii)
                {
                    vertices->color = color;
                    ++vertices;
                }
            }
        }
    }

    // Check duration end.
    bool canEmit = true;

    if (m_stopEmitting)
    {
        canEmit = false;
        m_running = m_activeParticleCount > 0;
    }
    else if (!m_settings.loop)
    {
        if (ApproxSuperiorOrEqual(m_currentDuration, static_cast<float>(m_settings.duration), math::Epsilon6))
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

        if (ApproxSuperiorOrEqual(dt.ms(), m_nextSpawnDelay, math::Epsilon6))
        {
            // This case triggers if delay <= dt (high spawn rate).
            nbSpawns = Max(1, (int)(dt.s() * GetRandomf(m_settings.minSpawnPerSecond, m_settings.maxSpawnPerSecond)));
            m_currentSpawnDelay = 0.f;

            float randValue = GetRandomf(m_settings.minSpawnPerSecond, m_settings.maxSpawnPerSecond);
            m_nextSpawnDelay = 1000.f / Max(math::Epsilon6, randValue);
        }
        else
        {
            // This case triggers if delay > dt (low spawn rate).
            m_currentSpawnDelay += dt.ms();
            if (ApproxSuperiorOrEqual(m_currentSpawnDelay, m_nextSpawnDelay, math::Epsilon6))
            {
                nbSpawns = 1;
                m_currentSpawnDelay -= m_nextSpawnDelay;

                float randValue = GetRandomf(m_settings.minSpawnPerSecond, m_settings.maxSpawnPerSecond);
                m_nextSpawnDelay = 1000.f / Max(math::Epsilon6, randValue);
            }
        }

        if (nbSpawns > 0)
        {
            for (int n = 0; n < nbSpawns; ++n)
            {
                size_t i = m_nextEmitIndex;
                size_t emitCount = 0;
                size_t emitLimit = Min(m_maxParticleCount, (size_t)GetRandom(m_settings.minParticlesPerSpawn, m_settings.maxParticlesPerSpawn));
                while (ApproxInferiorOrEqualToZero(m_dataLifetime[i], math::Epsilon6) && emitCount < emitLimit)
                {
                    EmitParticle(i);

                    i = (i + 1) % m_maxParticleCount;
                    ++emitCount;
                }

                m_nextEmitIndex = i;
            }
        }
    }
}

void ParticleSystem::Render(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    if (!m_running || m_dataVertices.empty())
        return;

    sf::RenderStates states;
    states.texture = m_texture;

    if (m_settings.localSpace)
    {
        states.transform = _kTransformSelf;
    }

    if (m_settings.useSortBuffer)
    {
        if (m_nextEmitIndex == 0)
        {
            std::copy(m_dataVertices.begin(), m_dataVertices.end(), m_sortBuffer.begin());
        }
        else
        {
            size_t sliceIndex = m_nextEmitIndex * m_verticesPerParticle;
            size_t sliceSize = m_dataVertices.size() - sliceIndex;

            std::copy(m_dataVertices.begin() + sliceIndex, m_dataVertices.end(), m_sortBuffer.begin());
            std::copy(m_dataVertices.begin(), m_dataVertices.begin() + sliceIndex, m_sortBuffer.begin() + sliceSize);
        }

        _kRenderPass.target->draw(&m_sortBuffer[0], m_sortBuffer.size(), m_primitiveType, states);
    }
    else
    {
        _kRenderPass.target->draw(&m_dataVertices[0], m_dataVertices.size(), m_primitiveType, states);
    }

    // Stats
    if (_kRenderPass.frameInfos)
    {
        if (m_verticesPerParticle == 6)
        {
            _kRenderPass.frameInfos->statDrawCalls += 1;
            _kRenderPass.frameInfos->statTriangles += (int)m_dataVertices.size() / 3;
        }
        else
        {
            // TODO: vertex count instead of triangle count ?
            _kRenderPass.frameInfos->statDrawCalls += 1;
            _kRenderPass.frameInfos->statTriangles += (int)m_dataVertices.size();
        }
    }
}

}   //namespace gugu
