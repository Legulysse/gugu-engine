#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Application.h"
#include "Gugu/Events/EventListener.h"
#include "Gugu/Math/Vector2.h"

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

class Demo : public gugu::Application, public gugu::EventListener
{
public:

    Demo();
    virtual ~Demo();

    virtual void AppStart() override;
    virtual void AppStop() override;
    virtual void AppUpdate(const gugu::DeltaTime& dt) override;
    virtual bool OnSFEvent(const sf::Event& _oSFEvent) override;

    void RefreshGrids();

private:

    gugu::Element* m_root;
    gugu::ElementTileMap* m_pTileMapA;
    gugu::ElementTileMap* m_pTileMapB;
    gugu::ElementTileMap* m_pTileMapC;

    gugu::SquareGrid* m_grid4;
    gugu::SquareGrid* m_grid8;
    gugu::HexGrid* m_grid6;

    class DemoGridData* m_gridData4;
    class DemoGridData* m_gridData8;
    class DemoGridData* m_gridData6;

    gugu::Vector2i m_referenceCoords4;
    gugu::Vector2i m_referenceCoords8;
    gugu::Vector2i m_referenceCoords6;

    int m_neighboursRange;
};

}   //namespace demoproject
