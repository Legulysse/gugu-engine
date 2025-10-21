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
using namespace tests;

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

    size_t errorLogCount = 0;
    size_t warningLogCount = 0;
    const auto logDelegate = [&](const std::string& timestamp, ELog::Type level, ELogEngine::Type category, const std::string& text)
    {
        if (level == ELog::Error)
        {
            errorLogCount += 1;
        }
        else if (level == ELog::Warning)
        {
            warningLogCount += 1;
        }
    };

    // TODO: Move log setup before Engine setup (need a way to provide the engine with an existing logger).
    // TODO: Setup the logger to ignore Debug/Log levels, only keep warnings and errors (Or handle the output directly in the delegate).
    GetLogEngine()->SetIDEConsoleOutput(true);
    GetLogEngine()->RegisterDelegate(Handle(nullptr), logDelegate);

    //----------------------------------------------

    // TODO: Maybe I could separate standalone tests before engine initialization (and add a special test for engine init).
    UnitTestResults results("UnitTests.log");

    // Standard Tests.
    RunUnitTests_Math(&results);
    RunUnitTests_System(&results);
    RunUnitTests_Xml(&results);
    RunUnitTests_Core(&results);
    RunUnitTests_Element(&results);
    RunUnitTests_DataBinding(&results);
    RunUnitTests_Resources(&results);

    // Finalize Tests.
    GUGU_UTEST_INIT("Finalize", "UnitTests_Finalize.log", &results);
    GUGU_UTEST_SECTION("Check Log Errors");
    GUGU_UTEST_CHECK_EQUAL(errorLogCount, results.GetExpectedErrorCount());
    GUGU_UTEST_CHECK_EQUAL(warningLogCount, results.GetExpectedWarningCount());
    GUGU_UTEST_FINALIZE();

    results.PrintResults();

    //----------------------------------------------

    GetEngine()->Release();

    return 0;
}
