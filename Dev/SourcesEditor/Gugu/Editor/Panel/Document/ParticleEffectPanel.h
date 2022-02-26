#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/Document/DocumentPanel.h"

#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DeltaTime;
    class RenderViewport;
    class ParticleEffect;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ParticleEffectPanel : public DocumentPanel
{
public:

    ParticleEffectPanel(const std::string& resourceID);
    virtual ~ParticleEffectPanel();

    virtual void UpdatePanel(const gugu::DeltaTime& dt) override;
    virtual void UpdateProperties(const gugu::DeltaTime& dt) override;
    virtual bool Save() override;

private:

    RenderViewport* m_renderViewport;

    ParticleEffect* m_particleEffect;
};

}   //namespace gugu
