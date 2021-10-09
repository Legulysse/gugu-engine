#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Application.h"
#include "Gugu/Events/EventListener.h"
#include "Gugu/VisualEffects/ParticleSystemSettings.h"
#include "Gugu/System/Types.h"

#include <SFML/Graphics/VertexArray.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class ElementParticles;
    class ParticleSystem;
}

////////////////////////////////////////////////////////////////
// File Declarations

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
    std::vector<gugu::ElementParticles*> m_centerParticleElements;
};

}   //namespace demoproject
