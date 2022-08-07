#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/Document/DocumentPanel.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DeltaTime;
    class RenderViewport;
    class ParticleEffect;
    class ParticleSystem;
    class ElementParticles;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ParticleEffectPanel : public DocumentPanel
{
public:

    ParticleEffectPanel(ParticleEffect* resource);
    virtual ~ParticleEffectPanel();

protected:

    void ResetParticleSystem();

    virtual void OnUndoRedo() override;
    virtual void UpdatePanelImpl(const DeltaTime& dt) override;
    virtual void UpdatePropertiesImpl(const DeltaTime& dt) override;

private:

    RenderViewport* m_renderViewport;
    float m_zoomFactor;

    ParticleEffect* m_particleEffect;
    ParticleSystem* m_particleSystem;
    ElementParticles* m_elementParticle;

    bool m_followCursor;
    bool m_rotate;
    float m_restartDelay;

    bool m_pendingRestart;
    float m_currentDelay;
    size_t m_maxParticleCount;
};

}   //namespace gugu
