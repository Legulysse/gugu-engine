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

    size_t GetMaxParticleCount() const;
    size_t GetActiveParticleCount() const;

private:

    void EmitParticle(size_t particleIndex);
    void ResetParticle(size_t particleIndex);

private:

    Element* m_element;

    bool m_running;
    size_t m_nextEmitIndex;
    int m_nextCycleDelay;
    Vector2f m_emitterPosition;

    ParticleSystemSettings m_settings;
    bool m_loop;
    size_t m_maxParticleCount;
    size_t m_verticesPerParticle;
    ImageSet* m_imageSet;
    sf::Texture* m_texture;

    sf::VertexArray m_dataVertices;
    std::vector<int> m_dataLifetime;
    std::vector<int> m_dataRemainingTime;
    std::vector<Vector2f> m_dataVelocity;
};

}   //namespace gugu
