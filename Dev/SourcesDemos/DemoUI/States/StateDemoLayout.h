#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Patterns/State.h"
#include "Gugu/Events/EventListener.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {
    
class StateDemoLayout: public gugu::State, public gugu::EventListener
{
public:

    StateDemoLayout();
    virtual ~StateDemoLayout();

    virtual void Init() override;
    virtual void Release() override;

    virtual bool OnSFEvent(const sf::Event& _oSFEvent) override;

private:

    gugu::Element* m_root;
};

}   //namespace demoproject
