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

    Element* m_element;

    ParticleSystemSettings m_settings;
    size_t m_maxParticleCount;
    size_t m_verticesPerParticle;
    ImageSet* m_imageSet;
    sf::Texture* m_texture;

    bool m_running;
    size_t m_activeParticleCount;
    int m_currentDuration;
    Vector2f m_emitterPosition;
    size_t m_nextEmitIndex;
    int m_nextSpawnDelay;
    int m_currentSpawnDelay;

    sf::VertexArray m_dataVertices;
    std::vector<int> m_dataLifetime;
    std::vector<int> m_dataRemainingTime;
    std::vector<Vector2f> m_dataVelocity;
};

}   //namespace gugu
