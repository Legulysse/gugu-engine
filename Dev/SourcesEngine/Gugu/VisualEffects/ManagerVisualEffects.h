#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct EngineConfig;
    struct EngineStats;
    struct ParticleSystemSettings;
    class DeltaTime;
    class ParticleSystem;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ManagerVisualEffects
{
public:

    ManagerVisualEffects();
    ~ManagerVisualEffects();

    void Init(const EngineConfig& config);
    void Release();

    void Update(const DeltaTime& dt, EngineStats& stats);

    void AddParticleSystem(ParticleSystem* particleSystem);
    void RemoveParticleSystem(ParticleSystem* particleSystem);

private:

    void DeleteAllParticleSystems();

protected:

    std::vector<ParticleSystem*> m_particleSystems;
};

ManagerVisualEffects* GetVisualEffects();

}   // namespace gugu
