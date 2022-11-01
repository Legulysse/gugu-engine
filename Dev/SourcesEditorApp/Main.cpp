////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"
#include "EditorApp.h"
#include "Gugu/Editor/Editor.h"
#include "Gugu/Engine.h"

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

        _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

    #endif

    //----------------------------------------------
    // Init engine

    EngineConfig config;
    config.applicationName = "Gugu::Editor";
    config.applicationIcon = "Icon.png";
    config.pathAssets = "Assets";
    config.defaultFont = "Roboto-Regular.ttf";
    config.debugFont = "Roboto-Regular.ttf";
    config.windowWidth = 1440;
    config.windowHeight = 900;
    config.maximizeWindow = true;
    config.handleResourceDependencies = true;

    GetEngine()->Init(config);

    //----------------------------------------------
    // Init editor

    EditorConfig editorConfig;
    editorConfig.LoadFromFile("EditorConfig.xml");

#if 0
#if 0
    editorConfig.defaultProjectFilePath = "../../Version/DemoSystems/Editor/ProjectSettings.xml";
#else
    editorConfig.defaultProjectFilePath = "../../Version/DemoGame/Editor/ProjectSettings.xml";
#endif
#endif

    GetEditor()->Init(editorConfig);

    //----------------------------------------------

    GetEngine()->SetApplication(new EditorApp);
    GetEngine()->RunMainLoop();
    GetEngine()->Release();

    return 0;
}
