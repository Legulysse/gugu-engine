#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Pattern/State.h"
#include "Gugu/Events/EventListener.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class ElementSpriteAnimated;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class StateMenuMain: public gugu::State, public gugu::EventListener
{
public:

    StateMenuMain();
    virtual ~StateMenuMain();

    virtual void Init() override;
    virtual void Release() override;

    virtual bool OnSFEvent(const sf::Event& _oSFEvent) override;

    void OnButtonClick(int _iButtonID);

private:

    gugu::Element* m_root;
    gugu::Element* m_menu;
};

}   //namespace demoproject
