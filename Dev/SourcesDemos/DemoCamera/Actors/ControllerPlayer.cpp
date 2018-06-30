////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Actors/ControllerPlayer.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Actors/Character.h"

#include "Gugu/Manager/ManagerConfig.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/Window/Window.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

ControllerPlayer::ControllerPlayer()
{
	m_playerIndex = -1;
    m_character = nullptr;
}

ControllerPlayer::~ControllerPlayer()
{
}

void ControllerPlayer::InitController(Character* _pCharacter, int _playerIndex)
{
    m_playerIndex = _playerIndex;
    m_character = _pCharacter;
}

void ControllerPlayer::Step(const DeltaTime& dt)
{
	Actor::Step(dt);

	ManagerConfig* pConfig = GetConfig();

    sf::Vector2f kDirection;

	if (m_playerIndex == 0)
	{
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
	}
	else if (m_playerIndex == 1)
	{
		if (pConfig->IsInputDown("Player_2_Up"))
		{
			kDirection.y += -1.f;
		}
		if (pConfig->IsInputDown("Player_2_Down"))
		{
			kDirection.y += 1.f;
		}
		if (pConfig->IsInputDown("Player_2_Left"))
		{
			kDirection.x += -1.f;
		}
		if (pConfig->IsInputDown("Player_2_Right"))
		{
			kDirection.x += 1.f;
		}
	}

    m_character->Move(kDirection, dt);

    Camera* pCamera = GetGameWindow()->GetCamera(m_playerIndex);
    if (pCamera)
    {
        pCamera->SetTarget(m_character->GetPosition());
    }
}

}   //namespace demoproject
