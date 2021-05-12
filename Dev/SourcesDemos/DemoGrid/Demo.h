#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Application.h"
#include "Gugu/Misc/EventListener.h"
#include "Gugu/Utility/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class ElementTileMap;
    class ElementSpriteGroup;
    class SquareGrid;
    class HexGrid;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class DemoGridData
{
public:

    DemoGridData();

    void GenerateCells(int width, int height);

    bool IsBlocked(const sf::Vector2i& coords) const;
    void SetBlocked(const sf::Vector2i& coords, bool blocked);

    bool IsWalkable(const sf::Vector2i& coordsFrom, const sf::Vector2i& coordsTo) const;
    bool IsWalkable(const sf::Vector2i& coordsFrom, const sf::Vector2i& coordsTo, float& cost) const;

private:

    struct DemoCellData
    {
        bool blocked = false;
    };

    int m_width;
    int m_height;
    std::vector<DemoCellData> m_cells;
};
    
class Demo : public gugu::Application, public gugu::EventListener
{
public:

    Demo();
    virtual ~Demo();

    virtual void AppStart() override;
    virtual void AppUpdate(const gugu::DeltaTime& dt) override;
    virtual bool OnSFEvent(const sf::Event& _oSFEvent) override;

    void RefreshGrids();

private:

    gugu::Element* m_root;
    gugu::ElementTileMap* m_pTileMapA;
    gugu::ElementTileMap* m_pTileMapB;
    gugu::ElementSpriteGroup* m_pTileMapC;

    gugu::SquareGrid* m_grid4;
    gugu::SquareGrid* m_grid8;
    gugu::HexGrid* m_grid6;

    DemoGridData* m_gridData4;
    DemoGridData* m_gridData8;
    DemoGridData* m_gridData6;

    sf::Vector2i m_referenceCoords4;
    sf::Vector2i m_referenceCoords8;
    sf::Vector2i m_referenceCoords6;
};

}   //namespace demoproject
