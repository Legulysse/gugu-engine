////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Scene/Grid.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementTileMap.h"
#include "Gugu/Scene/Scene.h"
#include "Gugu/Math/Random.h"
#include "Gugu/Math/MathUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

Grid::Grid()
{
}

Grid::~Grid()
{
}

void Grid::InitGrid(Scene* scene, int _iWidth, int _iHeight, float _fCellWidth, float _fCellHeight)
{
    SquareGrid::InitSquareGrid(_iWidth, _iHeight, _fCellWidth, _fCellHeight, true);
    
    Element* pRoot = scene->GetRootNode()->AddChild<Element>();
    pRoot->GetInteractions()->SetInteractionFlags(EElementInteractionEvent::Absorb | EElementInteractionEvent::Disabled);  //TODO: default on Scene nodes ?

    m_pTileMap = pRoot->AddChild<ElementTileMap>();
    m_pTileMap->BuildFromSquareGrid(this);
    m_pTileMap->SetTexture("LevelFloor.png");
    m_pTileMap->SetPosition(m_pTileMap->GetSize() / -2.f);

    for (int i = 0; i < m_pTileMap->GetTileCount(); ++i)
    {
        m_pTileMap->UpdateTileTextureCoords(i, GetRandom(100) > 5 ? sf::IntRect(0, 0, 16, 16) : sf::IntRect(16, 0, 16, 16));
    }
}

void Grid::ClampPositionInsideBounds(Vector2f& position) const
{
    position.x = Clamp(position.x, m_pTileMap->GetPosition().x, m_pTileMap->GetPosition().x + m_pTileMap->GetSize().x);
    position.y = Clamp(position.y, m_pTileMap->GetPosition().y, m_pTileMap->GetPosition().y + m_pTileMap->GetSize().y);
}

}   //namespace demoproject
