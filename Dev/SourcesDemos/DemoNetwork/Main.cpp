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
	//Init engine
	EngineConfig config;
	config.applicationName = "GuguEngine Demo Network";
	config.pathAssets = "Assets";
    config.defaultFont = "Spaceranger.ttf";
    config.windowWidth = 1024;
    config.windowHeight = 768;
    
    GetEngine()->Init(config);

    //--------

    GetEngine()->SetApplication(new Demo);

    //--------

    GetEngine()->Loop();
    GetEngine()->Release();

    return 0;
}
