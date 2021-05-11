#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Application.h"
#include "Gugu/Misc/EventListener.h"

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

private:

    gugu::Element* m_root;
    gugu::ElementTileMap* m_pTileMapA;
    gugu::ElementTileMap* m_pTileMapB;
    gugu::ElementSpriteGroup* m_pTileMapC;

    gugu::SquareGrid* m_grid4;
    gugu::SquareGrid* m_grid8;
    gugu::HexGrid* m_grid6;
};

}   //namespace demoproject
