////////////////////////////////////////////////////////////////
// Header

#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Manager/ManagerConfig.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementTileMap.h"
#include "Gugu/Element/2D/ElementSpriteGroup.h"
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

    Element* gridsLayer = m_root->AddChild<Element>();
    gridsLayer->SetInteractionFlags(EInteraction::Absorb | EInteraction::Disabled);  //TODO: default on Level nodes ?

    // Square-4 grid.
    SquareGrid* m_grid4 = new SquareGrid();
    m_grid4->InitSquareGrid(10, 10, 32.f, 32.f, false);

    std::vector<sf::Vector2i> neighboursRangeA;
    m_grid4->GetNeighboursInRange(sf::Vector2i(4, 3), 4, neighboursRangeA);

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
    
    // Square-8 grid.
    SquareGrid* m_grid8 = new SquareGrid();
    m_grid8->InitSquareGrid(10, 10, 32.f, 32.f, true);

    std::vector<sf::Vector2i> neighboursRangeB;
    m_grid8->GetNeighboursInRange(sf::Vector2i(4, 3), 4, neighboursRangeB);

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

    // Hex grid.
    HexGrid* m_grid6 = new HexGrid();
    m_grid6->InitHexGrid(10, 10, 32.f);

    std::vector<sf::Vector2i> neighboursRangeC;
    m_grid6->GetNeighboursInRange(sf::Vector2i(4, 3), 4, neighboursRangeC);

    ElementSpriteGroup* m_pTileMapC = gridsLayer->AddChild<ElementSpriteGroup>();
    m_pTileMapC->SetTexture("HexGrid.png");
    m_pTileMapC->SetPosition(690, 10);
    m_pTileMapC->SetSize(32.f * m_grid6->GetWidth(), 32.f * m_grid6->GetHeight());

    for (int y = 0; y < m_grid6->GetHeight(); ++y)
    {
        for (int x = 0; x < m_grid6->GetWidth(); ++x)
        {
            ElementSpriteGroupItem* item = new ElementSpriteGroupItem;
            item->SetSubRect(sf::IntRect(0, 0, 32, 37));
            item->SetPosition(m_grid6->GetCellPosition(sf::Vector2i(x, y)));
            item->SetSize(m_grid6->GetCellWidth(), m_grid6->GetCellHeight());

            m_pTileMapC->AddItem(item);
        }
    }

    for (sf::Vector2i coords : neighboursRangeC)
    {
        int tileIndex = coords.x + coords.y * m_grid6->GetWidth();
        m_pTileMapC->GetItem(tileIndex)->SetSubRect(sf::IntRect(32, 0, 32, 37));
        m_pTileMapC->RecomputeItemVertices(tileIndex);
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

bool Demo::OnSFEvent(const sf::Event& _oSFEvent)
{
    if (!EventListener::OnSFEvent(_oSFEvent))
        return false;

    ManagerConfig* pConfig = GetConfig();

    if (pConfig->IsInputReleased("CloseGame", _oSFEvent))
    {
        GetEngine()->StopLooping();
        return false;
    }

    return true;
}

}   //namespace demoproject
