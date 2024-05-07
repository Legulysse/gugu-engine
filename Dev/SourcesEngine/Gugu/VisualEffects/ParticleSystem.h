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
    class ParticleEffect;
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

    void Init(ParticleEffect* particleEffect);
    void Init(const ParticleSystemSettings& settings);

    void Start();
    void Stop();
    void Restart();

    void Update(const DeltaTime& dt);
    void Render(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf);

    void AttachToElement(Element* element);
    void SetEmitterPosition(const Vector2f& position);

    bool IsRunning() const;
    void StopEmitting();

    void SetPaused(bool paused);
    bool IsPaused() const;

    size_t GetMaxParticleCount() const;
    size_t GetActiveParticleCount() const;
    size_t GetParticleDataSize() const;

    const ParticleSystemSettings& GetSettings() const;

private:

    void EmitParticle(size_t particleIndex);
    void KillParticle(size_t particleIndex);
    void ResetParticle(size_t particleIndex);

    void UpdateEmitterPosition();

private:

    // Settings
    ParticleSystemSettings m_settings;
    size_t m_maxParticleCount;
    size_t m_verticesPerParticle;
    sf::PrimitiveType m_primitiveType;
    ImageSet* m_imageSet;
    sf::Texture* m_texture;
    Element* m_element;
    Vector2f m_emitterPosition;

    // Runtime
    bool m_running;
    bool m_stopEmitting;
    bool m_paused;
    float m_currentDuration;
    size_t m_activeParticleCount;
    size_t m_nextEmitIndex;
    float m_nextSpawnDelay;
    float m_currentSpawnDelay;

    // Particles data
    std::vector<sf::Vertex> m_dataVertices;
    std::vector<sf::Vertex> m_sortBuffer;
    std::vector<float> m_dataLifetime;
    std::vector<float> m_dataRemainingTime;
    std::vector<Vector2f> m_dataPosition;
    std::vector<Vector2f> m_dataStartSize;
    std::vector<Vector2f> m_dataEndSize;
    std::vector<Vector2f> m_dataVelocity;
};

}   //namespace gugu
