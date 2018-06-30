#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Application.h"
#include "Gugu/Misc/EventListener.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class ElementButton;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {
    
class Demo : public gugu::Application, public gugu::EventListener
{
public:

    Demo();
    virtual ~Demo();

	virtual	void AppStart() override;
    virtual	void AppStop() override;

protected:

    void OnButtonClick(int _iButtonID);

private:

    gugu::Element* m_root;

    gugu::ElementButton* m_buttonListenA;
    gugu::ElementButton* m_buttonConnectA;
    gugu::ElementButton* m_buttonPingA;

    gugu::ElementButton* m_buttonListenB;
    gugu::ElementButton* m_buttonConnectB;
    gugu::ElementButton* m_buttonPingB;

    gugu::ElementButton* m_buttonDisconnect;

    gugu::ElementButton* m_buttonExit;
};

}	//namespace demoproject
