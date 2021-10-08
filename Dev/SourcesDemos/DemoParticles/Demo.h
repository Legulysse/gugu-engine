#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Application.h"
#include "Gugu/Events/EventListener.h"

#include "Gugu/Element/Element.h"

#include <SFML/Graphics/VertexArray.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class ElementSFDrawable;
    class ImageSet;
    struct RenderPass;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ParticleSystem;

class ElementParticleSystem : public Element
{
public:
    ElementParticleSystem();
    virtual ~ElementParticleSystem();
    void SetParticleSystem(ParticleSystem* particleSystem);
    virtual void Update(const DeltaTime& dt) override;
protected:
    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
private:
    ParticleSystem* m_particleSystem;
};

struct ParticleSystemSettings
{
    // Setup
    bool loop = true;
    int maxParticleCount = 50;
    int verticesPerParticle = 6;
    bool localSpace = false;

    // Behaviour
    int minLifetime = 500;
    int maxLifetime = 500;
    int minEmitCountPerCycle = 1;
    int maxEmitCountPerCycle = 1;
    int minCycleDelay = 20;
    int maxCycleDelay = 20;
    float minVelocity = 50.f;
    float maxVelocity = 100.f;

    // Render
    bool keepSizeRatio = true;
    Vector2f minStartSize = Vector2f(5.f, 5.f);
    Vector2f maxStartSize = Vector2f(10.f, 10.f);
    std::string imageSetID = "";
    //Vector2f minEndSize = Vector2f(5.f, 5.f); // TODO: lerp particle size over lifetime, we need to store a reference position to recompute the vertices.
    //Vector2f maxEndSize = Vector2f(10.f, 10.f);
    sf::Color startColor = sf::Color::Red;
    sf::Color endColor = sf::Color::Blue;
};

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

    void AttachEmitter(Element* element);
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

namespace demoproject {

class Demo : public gugu::Application, public gugu::EventListener
{
public:

    Demo();
    virtual ~Demo();

    virtual void AppStart() override;
    virtual void AppStop() override;

    virtual void AppUpdate(const gugu::DeltaTime& dt) override;

protected:

    gugu::Element* m_root;
    gugu::Element* m_mouseFollow;
    gugu::Element* m_moveArm;

    gugu::int64 m_startTime;
    bool m_rotateArm;

    std::vector<gugu::ParticleSystemSettings> m_particleSystemSettings;
    std::vector<gugu::ParticleSystem*> m_particleSystems;
    std::vector<gugu::ElementParticleSystem*> m_particleRenderers;
};

}   //namespace demoproject
