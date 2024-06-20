////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/String.h"
#include "Gugu/System/Platform.h"
#include "Gugu/System/Memory.h"
#include "Gugu/System/Callback.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace test {

using Callable = std::function<void()>;
using CallableReturn = std::function<bool()>;
using DelegateFactory = std::function<int(int, bool)>;

template <typename TReturn>
using DelegateType = std::function<TReturn()>;
template <typename TReturn, typename TParam1>
using DelegateType1P = std::function<TReturn(TParam1)>;

template<typename ...T>
class Signal
{
public:
    void Register(std::function<void(T...)> callback)
    {
        m_callbacks.push_back(callback);
    }

    void Fire(T... args)
    {
        for (size_t i = 0; i < m_callbacks.size(); ++i)
        {
            m_callbacks[i](args...);
        }
    }

private:
    std::vector<std::function<void(T...)>> m_callbacks;
};


void TestFunction1(int value)
{
    gugu::WriteInConsoleEndline(gugu::ToString(value), true);
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
    CallableReturn m_actionType3;

public:

    void ComputeTest(const std::string& value)
    {
        gugu::WriteInConsoleEndline(value, true);
    }

    bool ReturnTest(int a, int b)
    {
        return a > b;
    }

    int TestObjectFactoryFunction(int a, bool b)
    {
        return b ? a : a * 2;
    }
};

//----------------------------------------------

void RunTestCallback()
{
    using namespace gugu;

    //----------------------------------------------

#if 1

    WriteInConsoleEndline("--------------------------------", true);

    auto functor1 = []() {
        WriteInConsoleEndline(ToString(22), true);
    };

    std::function<void()> functor2 = []() {
        WriteInConsoleEndline(ToString(33), true);
    };

    std::function<void(int)> functor3 = [](int value) {
        WriteInConsoleEndline(ToString(value), true);
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
    object->m_actionType3 = [object]() { return object->ReturnTest(5, 10); };

    functor1();
    functor2();
    functor3(44);
    WriteInConsoleEndline(ToString(functor4(55)), true);
    object->m_Action();
    WriteInConsoleEndline(ToString(object->m_Delegate(77, false)), true);
    object->m_ActionType();
    WriteInConsoleEndline(ToString(object->m_DelegateType(99)), true);
    object->m_ActionType2();
    WriteInConsoleEndline(ToString(object->m_Factory(42, false)), true);
    WriteInConsoleEndline(ToString(object->m_Factory2(42, true)), true);
    WriteInConsoleEndline(object->m_actionType3() ? "true" : "false", true);

    SafeDelete(object);

    WriteInConsoleEndline("--------------------------------", true);

    Signal<> signal0;
    signal0.Register([]() { WriteInConsoleEndline("Signal 0", true); });
    signal0.Fire();

    Signal<int> signal1;
    signal1.Register([](int a) { WriteInConsoleEndline(StringFormat("Signal 1 : {0}", a), true); });
    signal1.Fire(16);

    Signal<bool, float> signal2;
    signal2.Register([](bool a, float b) { WriteInConsoleEndline(StringFormat("Signal 2 : {0} {1}", a, b), true); });
    signal2.Fire(false, 32.f);

    WriteInConsoleEndline("--------------------------------", true);

#endif
}

}   //namespace test
