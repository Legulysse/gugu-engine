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
#include "Gugu/World/Grid/GridUtility.h"

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
    m_grid4 = new SquareGrid();
    m_grid4->InitSquareGrid(10, 12, 32.f, 32.f, false);

    m_gridData4 = new DemoGridData;
    m_gridData4->GenerateCells(m_grid4->GetWidth(), m_grid4->GetHeight());

    m_referenceCoords4 = sf::Vector2i(4, 3);

    m_pTileMapA = gridsLayer->AddChild<ElementTileMap>();
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

    // Square-8 grid.
    m_grid8 = new SquareGrid;
    m_grid8->InitSquareGrid(10, 12, 32.f, 32.f, true);

    m_gridData8 = new DemoGridData;
    m_gridData8->GenerateCells(m_grid8->GetWidth(), m_grid8->GetHeight());

    m_referenceCoords8 = sf::Vector2i(4, 3);

    m_pTileMapB = gridsLayer->AddChild<ElementTileMap>();
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

    // Hex grid.
    m_grid6 = new HexGrid();
    m_grid6->InitHexGrid(10, 12, 32.f);

    m_gridData6 = new DemoGridData;
    m_gridData6->GenerateCells(m_grid6->GetWidth(), m_grid6->GetHeight());

    m_referenceCoords6 = sf::Vector2i(4, 3);

    m_pTileMapC = gridsLayer->AddChild<ElementSpriteGroup>();
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

    // Finalize.
    RefreshGrids();
}

void Demo::RefreshGrids()
{
    // Square-4 grid.
    for (int y = 0; y < m_grid4->GetHeight(); ++y)
    {
        for (int x = 0; x < m_grid4->GetWidth(); ++x)
        {
            m_pTileMapA->SetTile(x, y, m_gridData4->IsBlocked(sf::Vector2i(x, y)) ? sf::IntRect(0, 32, 32, 32) : sf::IntRect(0, 0, 32, 32));
        }
    }

    m_pTileMapA->SetTile(m_referenceCoords4.x, m_referenceCoords4.y, sf::IntRect(32, 32, 32, 32));

    std::vector<sf::Vector2i> neighboursRangeA;
    BreadthFirstSearchNeighboursByWalkableRange(*m_grid4, *m_gridData4, m_referenceCoords4, 4, neighboursRangeA);

    for (sf::Vector2i coords : neighboursRangeA)
    {
        m_pTileMapA->SetTile(coords.x, coords.y, sf::IntRect(32, 0, 32, 32));
    }

    // Square-8 grid.
    for (int y = 0; y < m_grid8->GetHeight(); ++y)
    {
        for (int x = 0; x < m_grid8->GetWidth(); ++x)
        {
            m_pTileMapB->SetTile(x, y, m_gridData8->IsBlocked(sf::Vector2i(x, y)) ? sf::IntRect(0, 32, 32, 32) : sf::IntRect(0, 0, 32, 32));
        }
    }

    m_pTileMapB->SetTile(m_referenceCoords8.x, m_referenceCoords8.y, sf::IntRect(32, 32, 32, 32));

    std::vector<sf::Vector2i> neighboursRangeB;
    BreadthFirstSearchNeighboursByWalkableRange(*m_grid8, *m_gridData8, m_referenceCoords8, 4, neighboursRangeB);

    for (sf::Vector2i coords : neighboursRangeB)
    {
        m_pTileMapB->SetTile(coords.x, coords.y, sf::IntRect(32, 0, 32, 32));
    }

    // Hex grid.
    for (int y = 0; y < m_grid6->GetHeight(); ++y)
    {
        for (int x = 0; x < m_grid6->GetWidth(); ++x)
        {
            int tileIndex = x + y * m_grid6->GetWidth();
            m_pTileMapC->GetItem(tileIndex)->SetSubRect(m_gridData6->IsBlocked(sf::Vector2i(x, y)) ? sf::IntRect(0, 37, 32, 37) : sf::IntRect(0, 0, 32, 37));
            m_pTileMapC->RecomputeItemVertices(tileIndex);
        }
    }

    int referenceTileIndex = m_referenceCoords6.x + m_referenceCoords6.y * m_grid6->GetWidth();
    m_pTileMapC->GetItem(referenceTileIndex)->SetSubRect(sf::IntRect(32, 37, 32, 37));
    m_pTileMapC->RecomputeItemVertices(referenceTileIndex);

    std::vector<sf::Vector2i> neighboursRangeC;
    BreadthFirstSearchNeighboursByWalkableRange(*m_grid6, *m_gridData6, m_referenceCoords6, 4, neighboursRangeC);

    for (sf::Vector2i coords : neighboursRangeC)
    {
        int tileIndex = coords.x + coords.y * m_grid6->GetWidth();
        m_pTileMapC->GetItem(tileIndex)->SetSubRect(sf::IntRect(32, 0, 32, 37));
        m_pTileMapC->RecomputeItemVertices(tileIndex);
    }
}

void Demo::AppUpdate(const DeltaTime& dt)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))    //TODO: RegisterInput handling for mouse buttons (need little upgrade on the ConfigManager)
    {
        sf::Vector2f localPickedPositionA = m_pTileMapA->TransformToLocalFull(GetGameWindow()->GetMousePosition());
        sf::Vector2f localPickedPositionB = m_pTileMapB->TransformToLocalFull(GetGameWindow()->GetMousePosition());
        sf::Vector2f localPickedPositionC = m_pTileMapC->TransformToLocalFull(GetGameWindow()->GetMousePosition());

        sf::Vector2i pickedCoords;
        if (m_grid4->PickCoords(localPickedPositionA, pickedCoords))
        {
            m_gridData4->SetBlocked(pickedCoords, true);
            RefreshGrids();
        }
        else if (m_grid8->PickCoords(localPickedPositionB, pickedCoords))
        {
            m_gridData8->SetBlocked(pickedCoords, true);
            RefreshGrids();
        }
        else if (m_grid6->PickCoords(localPickedPositionC, pickedCoords))
        {
            m_gridData6->SetBlocked(pickedCoords, true);
            RefreshGrids();
        }
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))    //TODO: RegisterInput handling for mouse buttons (need little upgrade on the ConfigManager)
    {
        sf::Vector2f localPickedPositionA = m_pTileMapA->TransformToLocalFull(GetGameWindow()->GetMousePosition());
        sf::Vector2f localPickedPositionB = m_pTileMapB->TransformToLocalFull(GetGameWindow()->GetMousePosition());
        sf::Vector2f localPickedPositionC = m_pTileMapC->TransformToLocalFull(GetGameWindow()->GetMousePosition());

        sf::Vector2i pickedCoords;
        if (m_grid4->PickCoords(localPickedPositionA, pickedCoords))
        {
            m_referenceCoords4 = pickedCoords;
            RefreshGrids();
        }
        else if (m_grid8->PickCoords(localPickedPositionB, pickedCoords))
        {
            m_referenceCoords8 = pickedCoords;
            RefreshGrids();
        }
        else if (m_grid6->PickCoords(localPickedPositionC, pickedCoords))
        {
            m_referenceCoords6 = pickedCoords;
            RefreshGrids();
        }
    }

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


DemoGridData::DemoGridData()
    : m_width(0)
    , m_height(0)
{
}

void DemoGridData::GenerateCells(int width, int height)
{
    m_width = width;
    m_height = height;

    m_cells.clear();
    m_cells.resize(m_width * m_height);
}

bool DemoGridData::IsBlocked(const sf::Vector2i& coords) const
{
    return m_cells[coords.x + m_width * coords.y].blocked;
}

void DemoGridData::SetBlocked(const sf::Vector2i& coords, bool blocked)
{
    m_cells[coords.x + m_width * coords.y].blocked = blocked;
}

bool DemoGridData::IsWalkable(const sf::Vector2i& coordsFrom, const sf::Vector2i& coordsTo) const
{
    return !m_cells[coordsTo.x + m_width * coordsTo.y].blocked;
}

bool DemoGridData::IsWalkable(const sf::Vector2i& coordsFrom, const sf::Vector2i& coordsTo, float& cost) const
{
    cost = 1.f;
    return !m_cells[coordsTo.x + m_width * coordsTo.y].blocked;
}

}   //namespace demoproject
