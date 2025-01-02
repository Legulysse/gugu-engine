////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "wren.hpp"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace tests {

namespace wren {

void WriteFn(WrenVM* vm, const char* text)
{
    // The System.print() methods in wren_core.wren will always make a finalizing call with a single endline char.
    if (StringEquals(text, "\n"))
        return;

    GetLogEngine()->Print(ELog::Info, ELogEngine::Engine, StringFormat("[Wren] {0}", text));
}

void ErrorFn(WrenVM* vm, WrenErrorType errorType,
    const char* module, const int line,
    const char* msg)
{
    switch (errorType)
    {
    case WREN_ERROR_COMPILE:
        GetLogEngine()->Print(ELog::Error, ELogEngine::Engine, StringFormat("[Wren] [{0} line {1}] [Error] {2}", module, line, msg));
        break;
    case WREN_ERROR_STACK_TRACE:
        GetLogEngine()->Print(ELog::Error, ELogEngine::Engine, StringFormat("[Wren] [{0} line {1}] in {2}", module, line, msg));
        break;
    case WREN_ERROR_RUNTIME:
        GetLogEngine()->Print(ELog::Error, ELogEngine::Engine, StringFormat("[Wren] [Runtime Error] {0}", msg));
        break;
    }
}

}   // namespace wren

void RunUnitTests_Wren(UnitTestResults* results)
{
    GUGU_UTEST_INIT("Wren", "UnitTests_Wren.log", results);

    //----------------------------------------------

    GUGU_UTEST_SECTION("Basics");
    {
        WrenConfiguration wrenConfig;
        wrenInitConfiguration(&wrenConfig);
        wrenConfig.writeFn = &wren::WriteFn;
        wrenConfig.errorFn = &wren::ErrorFn;
        WrenVM* vm = wrenNewVM(&wrenConfig);

        const char* module = "main";
        const char* script = "System.print(\"I am running in a VM!\")";

        WrenInterpretResult result = wrenInterpret(vm, module, script);

        switch (result)
        {
        case WREN_RESULT_COMPILE_ERROR:
            GetLogEngine()->Print(ELog::Error, ELogEngine::Engine, "[Wren] Compile Error");
            break;
        case WREN_RESULT_RUNTIME_ERROR:
            GetLogEngine()->Print(ELog::Error, ELogEngine::Engine, "[Wren] Runtime Error");
            break;
        case WREN_RESULT_SUCCESS:
            GetLogEngine()->Print(ELog::Info, ELogEngine::Engine, "[Wren] Interpret Success");
            break;
        }

        wrenFreeVM(vm);

        GUGU_UTEST_CHECK_EQUAL(result, WREN_RESULT_SUCCESS);
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}

}   // namespace tests
