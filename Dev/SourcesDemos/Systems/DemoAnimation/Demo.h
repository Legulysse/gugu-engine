#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Application.h"
#include "Gugu/Events/EventListener.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class ElementSprite;
    class ElementText;
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

    virtual void AppStep(const gugu::DeltaTime& dt) override;

private:

    void OnWalkStep();

private:

    gugu::Element* m_root;
    gugu::ElementSprite* m_movingSprite1;
    gugu::ElementSprite* m_movingSprite2;
    gugu::ElementText* m_walkStepText;
    bool m_walkStepToggle;
};

}   //namespace demoproject
