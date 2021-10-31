#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct ParticleSystemSettings;
    class ParticleSystem;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementParticles : public Element
{
public:

    ElementParticles();
    virtual ~ElementParticles();

    ParticleSystem* CreateParticleSystem(const ParticleSystemSettings& settings, bool startNow);

protected:

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;

private:

    ParticleSystem* m_particleSystem;
};

}   // namespace gugu