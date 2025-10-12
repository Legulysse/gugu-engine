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
    gugu::WriteInIDEConsoleEndline(gugu::ToString(value));
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
        gugu::WriteInIDEConsoleEndline(value);
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

    WriteInIDEConsoleEndline("--------------------------------");

    auto functor1 = []() {
        WriteInIDEConsoleEndline(ToString(22));
    };

    std::function<void()> functor2 = []() {
        WriteInIDEConsoleEndline(ToString(33));
    };

    std::function<void(int)> functor3 = [](int value) {
        WriteInIDEConsoleEndline(ToString(value));
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
    WriteInIDEConsoleEndline(ToString(functor4(55)));
    object->m_Action();
    WriteInIDEConsoleEndline(ToString(object->m_Delegate(77, false)));
    object->m_ActionType();
    WriteInIDEConsoleEndline(ToString(object->m_DelegateType(99)));
    object->m_ActionType2();
    WriteInIDEConsoleEndline(ToString(object->m_Factory(42, false)));
    WriteInIDEConsoleEndline(ToString(object->m_Factory2(42, true)));
    WriteInIDEConsoleEndline(object->m_actionType3() ? "true" : "false");

    SafeDelete(object);

    WriteInIDEConsoleEndline("--------------------------------");

    Signal<> signal0;
    signal0.Register([]() { WriteInIDEConsoleEndline("Signal 0"); });
    signal0.Fire();

    Signal<int> signal1;
    signal1.Register([](int a) { WriteInIDEConsoleEndline(StringFormat("Signal 1 : {0}", a)); });
    signal1.Fire(16);

    Signal<bool, float> signal2;
    signal2.Register([](bool a, float b) { WriteInIDEConsoleEndline(StringFormat("Signal 2 : {0} {1}", a, b)); });
    signal2.Fire(false, 32.f);

    WriteInIDEConsoleEndline("--------------------------------");

#endif
}

}   //namespace test
