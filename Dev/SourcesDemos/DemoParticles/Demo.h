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

class ParticleSystem
{
public:

    ParticleSystem();
    ~ParticleSystem();

    void Init();

    void Start();
    void Stop();
    void Restart();

    void Update(const DeltaTime& dt);
    void Render(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf);

    void SetEmitterPosition(const Vector2f& position);

    size_t GetMaxParticleCount() const;
    size_t GetActiveParticleCount() const;

private:

    void EmitParticle(size_t particleIndex);
    void ResetParticle(size_t particleIndex);

private:

    bool m_running;
    Vector2f m_emitterPosition;

    bool m_loop;
    size_t m_maxParticleCount;
    size_t m_verticesPerParticle;
    size_t m_emitCountPerCycle;
    size_t m_nextEmitIndex;
    bool m_applyRenderLocalTransform;

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

    std::vector<gugu::ParticleSystem*> m_particleSystems;
    gugu::ParticleSystem* m_cursorParticleSystem;
};

}   //namespace demoproject
