#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/VisualEffects/ParticleSystemSettings.h"

#include <SFML/Graphics/VertexArray.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DeltaTime;
    class Element;
    class ImageSet;
    struct RenderPass;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ParticleSystem
{
public:

    ParticleSystem();
    ~ParticleSystem();

    static void SanitizeSettings(ParticleSystemSettings& settings, bool limitsOnly);

    void Init(const ParticleSystemSettings& settings);
    void Release();

    void Start();
    void Stop();
    void Restart();

    void Update(const DeltaTime& dt);
    void Render(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf);

    void AttachToElement(Element* element);
    void SetEmitterPosition(const Vector2f& position);

    bool IsRunning() const;
    size_t GetMaxParticleCount() const;
    size_t GetActiveParticleCount() const;

private:

    void EmitParticle(size_t particleIndex);
    void KillParticle(size_t particleIndex);
    void ResetParticle(size_t particleIndex);

private:

    // Settings
    ParticleSystemSettings m_settings;
    size_t m_maxParticleCount;
    size_t m_verticesPerParticle;
    ImageSet* m_imageSet;
    sf::Texture* m_texture;
    Element* m_element;
    Vector2f m_emitterPosition;

    // Runtime
    bool m_running;
    int m_currentDuration;
    size_t m_activeParticleCount;
    size_t m_nextEmitIndex;
    int m_nextSpawnDelay;
    int m_currentSpawnDelay;

    // Particles data
    sf::VertexArray m_dataVertices;
    std::vector<int> m_dataLifetime;
    std::vector<int> m_dataRemainingTime;
    std::vector<Vector2f> m_dataPosition;
    std::vector<Vector2f> m_dataStartSize;
    std::vector<Vector2f> m_dataEndSize;
    std::vector<Vector2f> m_dataVelocity;
};

}   //namespace gugu
