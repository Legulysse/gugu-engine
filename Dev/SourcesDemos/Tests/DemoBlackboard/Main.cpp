////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

#include "Test/Callback.h"
#include "Test/Condition/Condition_App.h"
#include "Test/Task.h"

#if defined(GUGU_ENV_VISUAL )

    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>

#endif

using namespace test;
using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

int main(int argc, char* argv[])
{
#if defined(GUGU_ENV_VISUAL )

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

    //----------------------------------------------

    RunTestCallback();
    RunTestCondition();
    RunTestTask();

    //----------------------------------------------

    return 0;
}
