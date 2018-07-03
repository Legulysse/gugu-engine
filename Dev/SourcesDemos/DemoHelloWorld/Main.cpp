////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

#include "Gugu/Engine.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Element/2D/ElementText.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

int main(int argc, char* argv[])
{
    //Init engine
    EngineConfig config;
    config.applicationName = "GuguEngine Demo HelloWorld";
    config.pathAssets = "Assets";
    config.defaultFont = "Spaceranger.ttf";
    config.gameWindow = EGameWindow::Sfml;
    config.windowWidth = 400;
    config.windowHeight = 300;
    config.backgroundColor = sf::Color(128, 128, 128, 255);
    
    GetEngine()->Init(config);

    //--------
    
    ElementText* pText = GetGameWindow()->GetUINode()->AddChild<ElementText>();
    pText->SetFont("Spaceranger.ttf");
    pText->SetText("Hello World !");
    pText->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    pText->SetUnifiedPosition(UDim2::POSITION_CENTER);
    
    //--------

    GetEngine()->Loop();
    GetEngine()->Release();

    return 0;
}
