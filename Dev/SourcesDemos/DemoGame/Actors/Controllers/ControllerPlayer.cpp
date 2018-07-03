////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Actors/Controllers/ControllerPlayer.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Actors/Characters/Character.h"

#include "Gugu/Manager/ManagerConfig.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/Window/Window.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

ControllerPlayer::ControllerPlayer()
{
    m_character = nullptr;
}

ControllerPlayer::~ControllerPlayer()
{
}

void ControllerPlayer::InitControllerPlayer(Character* _pCharacter)
{
    m_character = _pCharacter;

    RegisterHandlerEvents(GetGameWindow());
}

void ControllerPlayer::Step(const DeltaTime& dt)
{
    Actor::Step(dt);

    if (GetGameWindow()->IsConsoleVisible())
        return;

    ManagerConfig* pConfig = GetConfig();

    //Movement
    sf::Vector2f kDirection;

    if (pConfig->IsInputDown("Player_1_Up"))
    {
        kDirection.y += -1.f;
    }
    if (pConfig->IsInputDown("Player_1_Down"))
    {
        kDirection.y += 1.f;
    }
    if (pConfig->IsInputDown("Player_1_Left"))
    {
        kDirection.x += -1.f;
    }
    if (pConfig->IsInputDown("Player_1_Right"))
    {
        kDirection.x += 1.f;
    }

    m_character->Move(kDirection, dt);

    //Attack
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))    //TODO: RegisterInput handling for mouse buttons (need little upgrade on the ConfigManager)
    {
        sf::Vector2i kMouseCoords = GetGameWindow()->GetMousePixelCoords();
        Camera* pCamera = GetGameWindow()->GetCamera(0);
        sf::Vector2f kPickedPosition = pCamera->GetPickedPosition(kMouseCoords);    //TODO: shortcuts (Camera->GetPickedPosition, Window->GetPickedPosition
        m_character->Attack(kPickedPosition);       //TODO: test with varrying world transform
    }

    //Camera
    Camera* pCamera = GetGameWindow()->GetCamera(0);
    if (pCamera)
    {
        pCamera->SetTarget(m_character->GetPosition());
    }

    //Orientation
    //sf::Vector2f kMousePosition = GetGameWindow()->GetMousePosition();
}

bool ControllerPlayer::OnSFEvent(const sf::Event& _oSFEvent)
{
    if (!EventListener::OnSFEvent(_oSFEvent))
        return false;

    ManagerConfig* pConfig = GetConfig();

    //Picking
    /*if (_oSFEvent.type == sf::Event::MouseButtonPressed)
    {
        if (_oSFEvent.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2i kMouseCoords = GetGameWindow()->GetMousePixelCoords();
            Camera* pCamera = GetGameWindow()->GetCamera(0);
            sf::Vector2f kPickedPosition = pCamera->GetPickedPosition(kMouseCoords);    //TODO: shortcuts (Camera->GetPickedPosition, Window->GetPickedPosition
            m_pCharacter->Attack(kPickedPosition);      //TODO: test with varrying world transform
        }
    }*/

    return true;
}

}   //namespace demoproject
