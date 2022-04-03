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
    class ParticleEffect;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementParticles : public Element
{
public:

    ElementParticles();
    virtual ~ElementParticles();

    ParticleSystem* CreateParticleSystem(ParticleEffect* particleEffect, bool startNow);
    ParticleSystem* CreateParticleSystem(const ParticleSystemSettings& settings, bool startNow);

    ParticleSystem* GetParticleSystem() const;

protected:

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;

private:

    ParticleSystem* m_particleSystem;
};

}   // namespace gugu
