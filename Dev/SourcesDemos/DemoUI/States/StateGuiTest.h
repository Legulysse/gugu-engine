#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/State.h"
#include "Gugu/Misc/EventListener.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
	class Element;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {
    
class StateGuiTest: public gugu::State, public gugu::EventListener
{
public:

    StateGuiTest();
    virtual ~StateGuiTest();

    virtual void Init() override;
    virtual void Release() override;

    virtual bool OnSFEvent(const sf::Event& _oSFEvent) override;

private:

    gugu::Element* m_root;
};

}   //namespace demoproject
