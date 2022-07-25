////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Actors/Controllers/ControllerPlayer.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Actors/Characters/CharacterHero.h"

#include "Gugu/Inputs/ManagerInputs.h"
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

void ControllerPlayer::InitControllerPlayer(CharacterHero* _pCharacter)
{
    m_character = _pCharacter;

    RegisterEventHandler(GetGameWindow());
}

void ControllerPlayer::Step(const DeltaTime& dt)
{
    SceneActor::Step(dt);

    m_character->Move(m_moveDirection, dt);
}

void ControllerPlayer::Update(const DeltaTime& dt)
{
    SceneActor::Update(dt);

    ManagerInputs* inputs = GetInputs();

    //Attack
    if (inputs->IsButtonDown(sf::Mouse::Left))
    {
        Vector2i kMouseCoords = GetGameWindow()->GetMousePixelCoords();
        Camera* pCamera = GetGameWindow()->GetCamera(0);
        Vector2f kPickedPosition = pCamera->GetPickedPosition(kMouseCoords);    //TODO: shortcuts (Camera->GetPickedPosition, Window->GetPickedPosition
        m_character->UseSkillByIndex(0, kPickedPosition, dt);       //TODO: test with varrying world transform
    }

    if (inputs->IsInputDown("Player_1_Skill_1"))
    {
        Vector2i kMouseCoords = GetGameWindow()->GetMousePixelCoords();
        Camera* pCamera = GetGameWindow()->GetCamera(0);
        Vector2f kPickedPosition = pCamera->GetPickedPosition(kMouseCoords);
        m_character->UseSkillByIndex(1, kPickedPosition, dt);
    }

    //Movement
    Vector2f kDirection;

    if (inputs->IsInputDown("Player_1_Up"))
    {
        kDirection.y += -1.f;
    }
    if (inputs->IsInputDown("Player_1_Down"))
    {
        kDirection.y += 1.f;
    }
    if (inputs->IsInputDown("Player_1_Left"))
    {
        kDirection.x += -1.f;
    }
    if (inputs->IsInputDown("Player_1_Right"))
    {
        kDirection.x += 1.f;
    }

    m_moveDirection = kDirection;

    //Camera
    Camera* pCamera = GetGameWindow()->GetCamera(0);
    if (pCamera)
    {
        pCamera->SetTarget(m_character->GetPosition());
    }

    //Orientation
    //Vector2f kMousePosition = GetGameWindow()->GetMousePosition();
}

bool ControllerPlayer::OnSFEvent(const sf::Event& _oSFEvent)
{
    if (!EventListener::OnSFEvent(_oSFEvent))
        return false;

    ManagerInputs* inputs = GetInputs();

    //Picking
    if (_oSFEvent.type == sf::Event::KeyPressed)
    {
        if (_oSFEvent.mouseButton.button == sf::Mouse::Left)
        {
            //Vector2i kMouseCoords = GetGameWindow()->GetMousePixelCoords();
            //Camera* pCamera = GetGameWindow()->GetCamera(0);
            //Vector2f kPickedPosition = pCamera->GetPickedPosition(kMouseCoords);    //TODO: shortcuts (Camera->GetPickedPosition, Window->GetPickedPosition
            //m_pCharacter->Attack(kPickedPosition);      //TODO: test with varrying world transform
        }
    }

    return true;
}

}   //namespace demoproject
