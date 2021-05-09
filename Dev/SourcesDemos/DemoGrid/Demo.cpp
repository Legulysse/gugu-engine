////////////////////////////////////////////////////////////////
// Header

#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Manager/ManagerConfig.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementTileMap.h"
#include "Gugu/World/Grid/SquareGrid.h"
#include "Gugu/World/Grid/HexGrid.h"

#include <imgui.h>

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
Demo::Demo()
{
}

Demo::~Demo()
{
}

void Demo::AppStart()
{
    RegisterHandlerEvents(GetGameWindow());

    // Set Mouse aspect
    GetGameWindow()->SetSystemMouseVisible(false);
    GetGameWindow()->SetMouseVisible(true);
    GetGameWindow()->SetMouseTexture("Mouse.png");
    
    // Register Inputs
    ManagerConfig* pConfig = GetConfig();
    pConfig->RegisterInput("CloseGame", pConfig->BuildKeyboardEvent(sf::Keyboard::Escape));

    //Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

    SquareGrid* m_grid4 = new SquareGrid();
    m_grid4->InitSquareGrid(10, 10, 32.f, 32.f, false);

    SquareGrid* m_grid8 = new SquareGrid();
    m_grid8->InitSquareGrid(10, 10, 32.f, 32.f, true);

    std::vector<sf::Vector2i> neighboursA;
    m_grid4->GetNeighbours(sf::Vector2i(4, 3), neighboursA);

    std::vector<sf::Vector2i> neighboursB;
    m_grid8->GetNeighbours(sf::Vector2i(4, 3), neighboursB);

    std::vector<sf::Vector2i> neighboursRangeA;
    m_grid4->GetNeighboursInRange(sf::Vector2i(4, 3), 4, neighboursRangeA);

    std::vector<sf::Vector2i> neighboursRangeB;
    m_grid8->GetNeighboursInRange(sf::Vector2i(4, 3), 4, neighboursRangeB);

    Element* gridsLayer = m_root->AddChild<Element>();
    gridsLayer->SetInteractionFlags(EInteraction::Absorb | EInteraction::Disabled);  //TODO: default on Level nodes ?

    ElementTileMap* m_pTileMapA = gridsLayer->AddChild<ElementTileMap>();
    m_pTileMapA->SetTexture("SquareGrid.png");
    m_pTileMapA->SetTileCount(m_grid4->GetWidth(), m_grid4->GetHeight());
    m_pTileMapA->SetTileSize(sf::Vector2f(m_grid4->GetCellWidth(), m_grid4->GetCellHeight()));
    m_pTileMapA->SetPosition(10, 10);

    for (int y = 0; y < m_grid4->GetHeight(); ++y)
    {
        for (int x = 0; x < m_grid4->GetWidth(); ++x)
        {
            m_pTileMapA->SetTile(x, y, sf::IntRect(0, 0, 32, 32));
        }
    }

    for (sf::Vector2i coords : neighboursRangeA)
    {
        m_pTileMapA->SetTile(coords.x, coords.y, sf::IntRect(32, 0, 32, 32));
    }
    
    ElementTileMap* m_pTileMapB = gridsLayer->AddChild<ElementTileMap>();
    m_pTileMapB->SetTexture("SquareGrid.png");
    m_pTileMapB->SetTileCount(m_grid8->GetWidth(), m_grid8->GetHeight());
    m_pTileMapB->SetTileSize(sf::Vector2f(m_grid8->GetCellWidth(), m_grid8->GetCellHeight()));
    m_pTileMapB->SetPosition(350, 10);

    for (int y = 0; y < m_grid8->GetHeight(); ++y)
    {
        for (int x = 0; x < m_grid8->GetWidth(); ++x)
        {
            m_pTileMapB->SetTile(x, y, sf::IntRect(0, 0, 32, 32));
        }
    }

    for (sf::Vector2i coords : neighboursRangeB)
    {
        m_pTileMapB->SetTile(coords.x, coords.y, sf::IntRect(32, 0, 32, 32));
    }
}

void Demo::AppStop()
{
}

void Demo::AppUpdate(const DeltaTime& dt)
{
    if (ImGui::Begin("Grid Setup"))
    {
        const char* items[] = { "Square 4", "Square 8", "Hexagon" };
        static int item_current = 0;
        ImGui::Combo("Grid Type", &item_current, items, IM_ARRAYSIZE(items));

        static int rangeNeighbours = 1;
        ImGui::SliderInt("Neighbours Range", &rangeNeighbours, 1, 10);

        if (ImGui::Button("Refresh Neighbours"))
        {

        }
    }

    ImGui::End();
}

}   //namespace demoproject
