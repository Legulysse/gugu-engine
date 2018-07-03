////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

#include "Demo.h"
#include "States/StateMenuMain.h"

#include "Gugu/Engine.h"

using namespace demoproject;
using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

int main(int argc, char* argv[])
{
    //Init engine
    EngineConfig config;
    config.applicationName = "GuguEngine Demo UI";
    config.pathAssets = "Assets";
    config.defaultFont = "Spaceranger.ttf";
    config.windowWidth = 1024;
    config.windowHeight = 768;
    
    GetEngine()->Init(config);

    //--------

    Demo* demo = new Demo;
    demo->PushStateNow(new StateMenuMain);
    GetEngine()->SetApplication(demo);

    //--------

    GetEngine()->Loop();
    GetEngine()->Release();

    return 0;
}
