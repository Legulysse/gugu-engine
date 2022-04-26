////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"
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

    RunUnitTests_Math();
    RunUnitTests_System();

    //----------------------------------------------

    return 0;
}
