////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Window.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSpriteAnimated.h"
#include "Gugu/Element/UI/ElementButton.h"
#include "Gugu/Network/ManagerNetwork.h"
#include "Gugu/System/SystemUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
Demo::Demo()
: m_root(nullptr)
{
}

Demo::~Demo()
{
}

void Demo::AppStart()
{
    RegisterHandlerEvents(GetGameWindow());

    //Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

    m_buttonListenA = m_root->AddChild<ElementButton>();
    m_buttonListenA->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    m_buttonListenA->SetText("Listen A");
    m_buttonListenA->Move(10.0, 10.0);
    m_buttonListenA->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnButtonClick, 1));

    m_buttonConnectA = m_root->AddChild<ElementButton>();
    m_buttonConnectA->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    m_buttonConnectA->SetText("Connect A");
    m_buttonConnectA->Move(10.0, 60.0);
    m_buttonConnectA->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnButtonClick, 2));

    m_buttonPingA = m_root->AddChild<ElementButton>();
    m_buttonPingA->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    m_buttonPingA->SetText("Ping All");
    m_buttonPingA->Move(10.0, 160.0);
    m_buttonPingA->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnButtonClick, 3));

    m_buttonListenB = m_root->AddChild<ElementButton>();
    m_buttonListenB->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    m_buttonListenB->SetText("Listen B");
    m_buttonListenB->Move(300.0, 10.0);
    m_buttonListenB->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnButtonClick, 4));

    m_buttonConnectB = m_root->AddChild<ElementButton>();
    m_buttonConnectB->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    m_buttonConnectB->SetText("Connect B");
    m_buttonConnectB->Move(300.0, 60.0);
    m_buttonConnectB->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnButtonClick, 5));

    m_buttonDisconnect = m_root->AddChild<ElementButton>();
    m_buttonDisconnect->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    m_buttonDisconnect->SetText("Disconnect");
    m_buttonDisconnect->Move(590.0, 110.0);
    m_buttonDisconnect->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnButtonClick, 7));
}

void Demo::AppStop()
{
}

void Demo::OnButtonClick(int _iButtonID)
{
    if (_iButtonID == 1)
    {
        GetNetwork()->StartListening(GUGU_PORT_DEFAULT);
    }
    else if (_iButtonID == 2)
    {
        GetNetwork()->ConnectToClient(sf::IpAddress::LocalHost.toString(), GUGU_PORT_DEFAULT + 1);
    }
    else if (_iButtonID == 3)
    {
        GetNetwork()->SendNetPacketToAll(new NetPacket(ENetPacket::Ping), false);
    }
    else if (_iButtonID == 4)
    {
        GetNetwork()->StartListening(GUGU_PORT_DEFAULT + 1);
    }
    else if (_iButtonID == 5)
    {
        GetNetwork()->ConnectToClient(sf::IpAddress::LocalHost.toString(), GUGU_PORT_DEFAULT);
    }
    else if (_iButtonID == 7)
    {
        GetNetwork()->DisconnectAll();
    }
}

}   //namespace demoproject
