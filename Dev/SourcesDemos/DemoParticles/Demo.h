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

    void Init();
    void Start();
    void Update(const DeltaTime& dt);
    void Render(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf);

    void SetEmitterPosition(const Vector2f& position);

private:

    bool m_running;
    bool m_loop;
    size_t m_particleCount;
    size_t m_verticesPerParticle;
    Vector2f m_emitterPosition;
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
};

}   //namespace demoproject
