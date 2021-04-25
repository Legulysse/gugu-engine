////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Actors/Characters/CharacterEnemy.h"

////////////////////////////////////////////////////////////////
// Includes

#include "UI/ElementBar.h"
#include "Level/Grid.h"

#include "DatasheetBinding.h"

#include "Gugu/World/Level.h"
#include "Gugu/Element/2D/ElementSpriteAnimated.h"
#include "Gugu/Utility/Random.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

CharacterEnemy::CharacterEnemy()
{
}

CharacterEnemy::~CharacterEnemy()
{
}

void CharacterEnemy::InitEnemy(DS_Enemy* sheetEnemy, float _fSpeed, Grid* grid)
{
    m_grid = grid;

    // Init stats from datasheet
    m_maxLife = sheetEnemy->health;
    m_walkSpeed = sheetEnemy->speed;

    m_currentLife = m_maxLife;

    m_sprite = m_level->GetRootNode()->AddChild<ElementSpriteAnimated>();

    //if (GetRandom(0, 1) == 0)
    //    m_sprite->ChangeAnimSet("Orc.animset.xml");
    //else
    //    m_sprite->ChangeAnimSet("Lady.animset.xml");

    m_sprite->ChangeAnimSet(sheetEnemy->sprite->animSet);
    m_sprite->StartAnimation("IdleDown");
    m_sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);

    m_lifeBar = m_sprite->AddChild<ElementBar>();
    m_lifeBar->InitBar(ElementBar::BarColor::Red, 2.f);
    m_lifeBar->SetUnifiedOrigin(UDim2::POSITION_TOP_CENTER);
    m_lifeBar->SetUnifiedPosition(UDim2(0.5f, 0.f, 0.f, -5.f));
    m_lifeBar->SetSize(32.f, 4.f);

    m_lifeBar->SetVisible(false);
}

}   //namespace demoproject
