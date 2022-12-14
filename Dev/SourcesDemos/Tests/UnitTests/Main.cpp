////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"
#include "Gugu/Engine.h"
#include "AllUnitTests.h"

#if defined(GUGU_ENV_VISUAL )

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#endif

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
    config.applicationName = "GuguEngine Unit Tests";
    config.pathAssets = "Assets";
    config.defaultFont = "Roboto-Regular.ttf";
    config.debugFont = "Roboto-Regular.ttf";
    config.gameWindow = EGameWindow::Sfml;
    config.windowWidth = 200;
    config.windowHeight = 200;

    GetEngine()->Init(config);

    //----------------------------------------------

    // TODO: Maybe I could separate standalone tests before engine initialization (and add a special test for engine init).
    UnitTestResults results("UnitTests.log");

    RunUnitTests_Math(&results);
    RunUnitTests_System(&results);
    RunUnitTests_Xml(&results);
    RunUnitTests_Element(&results);

    results.PrintResults();

    //----------------------------------------------

    GetEngine()->Release();

    return 0;
}
