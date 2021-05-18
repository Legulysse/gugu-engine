////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Level/Grid.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementTileMap.h"
#include "Gugu/World/Level.h"
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

void Grid::InitGrid(Level* _pLevel, int _iWidth, int _iHeight, float _fCellWidth, float _fCellHeight)
{
    SquareGrid::InitSquareGrid(_iWidth, _iHeight, _fCellWidth, _fCellHeight, true);
    
    Element* pRoot = _pLevel->GetRootNode()->AddChild<Element>();
    pRoot->SetInteractionFlags(EInteraction::Absorb | EInteraction::Disabled);  //TODO: default on Level nodes ?
    //pRoot->Rotate(45);

    m_pTileMap = pRoot->AddChild<ElementTileMap>();
    m_pTileMap->SetTexture("LevelFloor.png");
    m_pTileMap->SetTileCount(m_width, m_height);
    m_pTileMap->SetTileSize(sf::Vector2f(m_cellWidth, m_cellHeight));
    m_pTileMap->SetPosition(m_pTileMap->GetSize() / -2.f);

    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            m_pTileMap->SetTile(x, y, GetRandom(100) > 5 ? sf::IntRect(0, 0, 16, 16) : sf::IntRect(16, 0, 16, 16));
        }
    }
}

void Grid::ClampPositionInsideBounds(sf::Vector2f& position) const
{
    position.x = Clamp(position.x, m_pTileMap->GetPosition().x, m_pTileMap->GetPosition().x + m_pTileMap->GetSize().x);
    position.y = Clamp(position.y, m_pTileMap->GetPosition().y, m_pTileMap->GetPosition().y + m_pTileMap->GetSize().y);
}

}   //namespace demoproject
