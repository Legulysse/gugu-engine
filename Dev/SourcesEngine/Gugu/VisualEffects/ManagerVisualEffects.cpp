////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/VisualEffects/ManagerVisualEffects.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/VisualEffects/ParticleSystem.h"
#include "Gugu/Animation/SpriteAnimation.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Debug/Trace.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ManagerVisualEffects::ManagerVisualEffects()
{
}

ManagerVisualEffects::~ManagerVisualEffects()
{
}

void ManagerVisualEffects::Init(const EngineConfig& config)
{
}

void ManagerVisualEffects::Release()
{
    DeleteAllParticleSystems();
}

void ManagerVisualEffects::Update(const DeltaTime& dt)
{
    GUGU_SCOPE_TRACE_MAIN("Visual Effects");

    for (ParticleSystem* particleSystem : m_particleSystems)
    {
        GUGU_SCOPE_TRACE_MAIN_("Particles", Particles);

        particleSystem->Update(dt);
    }
}

void ManagerVisualEffects::AddParticleSystem(ParticleSystem* particleSystem)
{
    if (!particleSystem)
        return;

    m_particleSystems.push_back(particleSystem);
}

void ManagerVisualEffects::RemoveParticleSystem(ParticleSystem* particleSystem)
{
    StdVectorRemove(m_particleSystems, particleSystem);
}

void ManagerVisualEffects::DeleteAllParticleSystems()
{
    for (size_t i = 0; i < m_particleSystems.size(); ++i)
    {
        ParticleSystem* particleSystem = m_particleSystems[i];
        m_particleSystems[i] = nullptr;

        SafeDelete(particleSystem);
    }

    m_particleSystems.clear();
}

ManagerVisualEffects* GetVisualEffects()
{
    return GetEngine()->GetManagerVisualEffects();
}

}   // namespace gugu
