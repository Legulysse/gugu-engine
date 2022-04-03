////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/2D/ElementParticles.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ParticleEffect.h"
#include "Gugu/VisualEffects/ManagerVisualEffects.h"
#include "Gugu/VisualEffects/ParticleSystem.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementParticles::ElementParticles()
    : m_particleSystem(nullptr)
{
}

ElementParticles::~ElementParticles()
{
    GetVisualEffects()->RemoveParticleSystem(m_particleSystem);
    SafeDelete(m_particleSystem);
}

ParticleSystem* ElementParticles::CreateParticleSystem(ParticleEffect* particleEffect, bool startNow)
{
    return CreateParticleSystem(*particleEffect->GetParticleSettings(), startNow);
}

ParticleSystem* ElementParticles::CreateParticleSystem(const ParticleSystemSettings& settings, bool startNow)
{
    if (m_particleSystem)
    {
        GetVisualEffects()->RemoveParticleSystem(m_particleSystem);
        SafeDelete(m_particleSystem);
    }

    m_particleSystem = new ParticleSystem;
    m_particleSystem->Init(settings);
    m_particleSystem->AttachToElement(this);
    GetVisualEffects()->AddParticleSystem(m_particleSystem);

    if (startNow)
    {
        m_particleSystem->Start();
    }

    return m_particleSystem;
}

ParticleSystem* ElementParticles::GetParticleSystem() const
{
    return m_particleSystem;
}

void ElementParticles::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    m_particleSystem->Render(_kRenderPass, _kTransformSelf);
}

}   // namespace gugu
