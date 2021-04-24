////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

#include "Game/Game.h"
#include "Game/StateMenuMain.h"
#include "Game/StateScenario.h"

#include "Gugu/Engine.h"

#if defined(GUGU_ENV_VISUAL )

    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>

#endif

using namespace demoproject;
using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

int main(int argc, char* argv[])
{
    #if defined(GUGU_ENV_VISUAL )

        _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

    #endif

    //Init engine
    EngineConfig config;
    config.applicationName = "GuguEngine Demo Game";
    config.pathAssets = "Assets";
    config.defaultFont = "Roboto-Regular.ttf";
    config.debugFont = "Roboto-Regular.ttf";
    config.gameWindow = EGameWindow::Sfml;
    config.maximizeWindow = true;
    //config.showStats = true;
    
    GetEngine()->Init(config);

    //--------
    
    Game* pGame = new Game;
    GetEngine()->SetApplication(pGame);
    pGame->PushStateNow(new StateScenario);

    //--------

    GetEngine()->StartLooping();
    GetEngine()->Release();

    return 0;
}
