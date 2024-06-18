////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"
#include "Demo.h"

#include "Gugu/Engine.h"

using namespace demoproject;
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
    config.applicationName = "GuguEngine Demo Audio";
    config.pathAssets = "Assets";
    config.defaultFont = "Roboto-Regular.ttf";
    config.debugFont = "Roboto-Regular.ttf";
    config.windowWidth = 800;
    config.windowHeight = 600;
    
    GetEngine()->Init(config);

    //----------------------------------------------

    GetEngine()->SetApplication(new Demo);
    GetEngine()->RunMainLoop();
    GetEngine()->Release();

    return 0;
}
