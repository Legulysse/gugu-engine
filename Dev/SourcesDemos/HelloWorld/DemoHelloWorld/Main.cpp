////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

#include "Gugu/Engine.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/System/SystemUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

int main(int argc, char* argv[])
{
#if defined(GUGU_ENV_VISUAL )

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

    //----------------------------------------------

    //Init engine
    EngineConfig config;
    config.applicationName = "GuguEngine Demo HelloWorld";
    config.pathAssets = "Assets";
    config.defaultFont = "Spaceranger.ttf";
    config.debugFont = "Roboto-Regular.ttf";
    config.gameWindow = EGameWindow::Sfml;
    config.windowWidth = 400;
    config.windowHeight = 300;
    config.backgroundColor = sf::Color(128, 128, 128, 255);
    
    GetEngine()->Init(config);

    //----------------------------------------------
    
    ElementText* pText = GetGameWindow()->GetUINode()->AddChild<ElementText>();
    pText->SetFont("mplus-1p-medium.ttf");
    pText->SetText("Hello World !");
    //pText->SetText(u8"יטאח");  // "れぐーさん"    "יטאח"
    pText->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    pText->SetUnifiedPosition(UDim2::POSITION_CENTER);
    
    //----------------------------------------------

    GetEngine()->RunMainLoop();
    GetEngine()->Release();

    return 0;
}
