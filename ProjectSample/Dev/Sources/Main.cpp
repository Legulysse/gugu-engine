////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

#include "Gugu/Engine.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementText.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

int main(int argc, char* argv[])
{
    //Init engine
    EngineConfig config;
    config.applicationName = "GuguEngine Sample Application";
    config.pathAssets = "Assets";
    config.defaultFont = "Spaceranger.ttf";
    config.debugFont = "Roboto-Regular.ttf";
    config.gameWindow = EGameWindow::Sfml;
    config.windowWidth = 800;
    config.windowHeight = 600;
    config.backgroundColor = sf::Color(128, 128, 128, 255);
    
    GetEngine()->Init(config);

    //--------
    
    ElementSprite* sprite = GetGameWindow()->GetUINode()->AddChild<ElementSprite>();
    sprite->SetTexture("sfml-logo-small.png");
    sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    sprite->SetUnifiedPosition(UDim2(0.5f, 0.75f));

    ElementText* text = GetGameWindow()->GetUINode()->AddChild<ElementText>();
    text->SetFont("Spaceranger.ttf");
    text->SetText("Hello World !");
    text->SetFontSize(48);
    text->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    text->SetUnifiedPosition(UDim2(0.5f, 0.25f));
    
    //--------

    GetEngine()->RunMainLoop();
    GetEngine()->Release();

    return 0;
}
