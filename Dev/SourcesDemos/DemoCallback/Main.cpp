////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

#include "Gugu/System/SystemUtility.h"
#include "Gugu/Misc/Action.h"

#include <functional>

#if defined(GUGU_ENV_VISUAL )

    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>

#endif

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

//--------

using Callable = std::function<void()>;
using DelegateFactory = std::function<int(int, bool)>;

template <typename TReturn>
using DelegateType = std::function<TReturn()>;
template <typename TReturn, typename TParam1>
using DelegateType1P = std::function<TReturn(TParam1)>;

void TestFunction1(int value)
{
    WriteInConsole(ToString(value));
}

int TestFactoryFunction(int a, bool b)
{
    return b ? a : a * 2;
}

class HolderClass
{
public:

    std::function<void()> m_Action;
    std::function<int(int, bool)> m_Delegate;

    Callable m_ActionType;
    DelegateType1P<int, int> m_DelegateType;
    Callable m_ActionType2;
    DelegateFactory m_Factory;
    DelegateFactory m_Factory2;

public:

    void ComputeTest(std::string value)
    {
        WriteInConsole(value);
    }

    int TestObjectFactoryFunction(int a, bool b)
    {
        return b ? a : a * 2;
    }
};

//--------

int main(int argc, char* argv[])
{
#if defined(GUGU_ENV_VISUAL )

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

    //--------

    Action* action1 = new ActionStatic1P<int>(TestFunction1, 11);

    auto functor1 = [](){
        WriteInConsole(ToString(22));
    };

    std::function<void()> functor2 = []() {
        WriteInConsole(ToString(33));
    };

    std::function<void(int)> functor3 = [](int value) {
        WriteInConsole(ToString(value));
    };

    std::function<int(int)> functor4 = [](int value) {
        return value;
    };

    HolderClass* object = new HolderClass();
    object->m_Action = []() { TestFunction1(66); };
    object->m_Delegate = [](int value, bool test) { return value; };
    object->m_ActionType = []() { TestFunction1(88); };
    object->m_DelegateType = [](int value) { return value; };
    object->m_ActionType2 = [object]() { object->ComputeTest("plop"); };
    object->m_Factory = TestFactoryFunction;
    object->m_Factory2 = std::bind(&HolderClass::TestObjectFactoryFunction, object, std::placeholders::_1, std::placeholders::_2);

    action1->Call();
    functor1();
    functor2();
    functor3(44);
    WriteInConsole(ToString(functor4(55)));
    object->m_Action();
    WriteInConsole(ToString(object->m_Delegate(77, false)));
    object->m_ActionType();
    WriteInConsole(ToString(object->m_DelegateType(99)));
    object->m_ActionType2();
    WriteInConsole(ToString(object->m_Factory(42, false)));
    WriteInConsole(ToString(object->m_Factory2(42, true)));

    SafeDelete(action1);
    SafeDelete(object);

    //--------

    return 0;
}
