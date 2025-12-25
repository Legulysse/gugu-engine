////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"
#include "Gugu/System/Callback.h"
#include "Gugu/System/Handle.h"
#include "Gugu/System/Signal.h"

#include <SFML/System/Sleep.hpp>

#include <set>

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace tests {

void RunUnitTests_Core(UnitTestResults* results)
{
    GUGU_UTEST_INIT("Core", "UnitTests_Core.log", results);

    //----------------------------------------------

    GUGU_UTEST_SECTION("UnitTest Handler");
    {
        int* dummyPtr = new int;

        GUGU_UTEST_CHECK(true);
        GUGU_UTEST_SILENT_CHECK(true);
        GUGU_UTEST_CHECK_TRUE(true);
        GUGU_UTEST_CHECK_FALSE(false);
        GUGU_UTEST_CHECK_NULL(nullptr);
        GUGU_UTEST_CHECK_NOT_NULL(dummyPtr);
        GUGU_UTEST_CHECK_EQUAL(10, 10);
        GUGU_UTEST_CHECK_NOT_EQUAL(10, 11);
        GUGU_UTEST_CHECK_APPROX_EQUAL(10.f, 10.f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_NOT_EQUAL(10.f, 11.f, math::Epsilon6);

#if 0
        // Test that failure cases actually fail.
        GUGU_UTEST_CHECK(false);
        GUGU_UTEST_SILENT_CHECK(false);
        GUGU_UTEST_CHECK_TRUE(false);
        GUGU_UTEST_CHECK_FALSE(true);
        GUGU_UTEST_CHECK_NULL(dummyPtr);
        GUGU_UTEST_CHECK_NOT_NULL(nullptr);
        GUGU_UTEST_CHECK_EQUAL(10, 11);
        GUGU_UTEST_CHECK_NOT_EQUAL(10, 10);
        GUGU_UTEST_CHECK_APPROX_EQUAL(10.f, 11.f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_NOT_EQUAL(10.f, 10.f, math::Epsilon6);
#endif

        delete dummyPtr;

        GUGU_UTEST_PERFORMANCE(100, []()
        {
        });

        GUGU_UTEST_PERFORMANCE_WITH_WARMUP(1, 100, []()
        {
        });

        GUGU_UTEST_PERFORMANCE(10, []()
        {
            sf::sleep(sf::milliseconds(10));
        });

        GUGU_UTEST_PERFORMANCE_WITH_WARMUP(1, 10, []()
        {
            sf::sleep(sf::milliseconds(10));
        });
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("Common Macros");
    {
        int buildConfigurationCount = 0;
        int buildTypeCount = 0;
        int operatingSystemCount = 0;

#if defined(GUGU_DEBUG)
        ++buildConfigurationCount;
#endif
#if defined(GUGU_RELEASE)
        ++buildConfigurationCount;
#endif
#if defined(GUGU_MASTER)
        ++buildConfigurationCount;
#endif

#if defined(GUGU_DEVELOPMENT_BUILD)
        ++buildTypeCount;
#endif
#if defined(GUGU_PRODUCTION_BUILD)
        ++buildTypeCount;
#endif

#if defined(GUGU_OS_WINDOWS)
        ++operatingSystemCount;
#endif
#if defined(GUGU_OS_LINUX)
        ++operatingSystemCount;
#endif
#if defined(GUGU_OS_MAC)
        ++operatingSystemCount;
#endif

        GUGU_UTEST_CHECK_EQUAL(buildConfigurationCount, 1);
        GUGU_UTEST_CHECK_EQUAL(buildTypeCount, 1);
        GUGU_UTEST_CHECK_EQUAL(operatingSystemCount, 1);
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("Handle");
    {
        int* dummyPtr = new int;

        Handle handleInvalid;
        Handle handleNullA(nullptr);
        Handle handleNullB((size_t)0);
        Handle handleNullC(nullptr, (size_t)0);
        Handle handleA(dummyPtr);
        Handle handleAA(dummyPtr, 0);
        Handle handleB(1);
        Handle handleBB(nullptr, 1);
        Handle handleC(dummyPtr, 1);

        GUGU_UTEST_CHECK_FALSE(handleInvalid.IsValid());
        GUGU_UTEST_CHECK_TRUE(handleNullA.IsValid());
        GUGU_UTEST_CHECK_TRUE(handleNullB.IsValid());
        GUGU_UTEST_CHECK_TRUE(handleNullC.IsValid());
        GUGU_UTEST_CHECK_TRUE(handleA.IsValid());
        GUGU_UTEST_CHECK_TRUE(handleAA.IsValid());
        GUGU_UTEST_CHECK_TRUE(handleB.IsValid());
        GUGU_UTEST_CHECK_TRUE(handleBB.IsValid());
        GUGU_UTEST_CHECK_TRUE(handleC.IsValid());

        GUGU_UTEST_CHECK_TRUE(handleInvalid == Handle());
        GUGU_UTEST_CHECK_FALSE(handleInvalid == Handle(nullptr));
        GUGU_UTEST_CHECK_FALSE(handleInvalid == Handle((size_t)0));
        GUGU_UTEST_CHECK_FALSE(handleInvalid == Handle(nullptr, 0));
        GUGU_UTEST_CHECK_FALSE(handleInvalid == handleNullA);
        GUGU_UTEST_CHECK_FALSE(handleInvalid == handleNullB);
        GUGU_UTEST_CHECK_FALSE(handleInvalid == handleNullC);

        GUGU_UTEST_CHECK_FALSE(handleA == handleInvalid);
        GUGU_UTEST_CHECK_FALSE(handleAA == handleInvalid);
        GUGU_UTEST_CHECK_FALSE(handleB == handleInvalid);
        GUGU_UTEST_CHECK_FALSE(handleBB == handleInvalid);
        GUGU_UTEST_CHECK_FALSE(handleC == handleInvalid);
        GUGU_UTEST_CHECK_FALSE(handleA == handleAA);
        GUGU_UTEST_CHECK_FALSE(handleB == handleBB);
        GUGU_UTEST_CHECK_TRUE(handleC == Handle(dummyPtr, 1));

        GUGU_UTEST_CHECK_TRUE(handleA != handleInvalid);
        GUGU_UTEST_CHECK_TRUE(handleAA != handleInvalid);
        GUGU_UTEST_CHECK_TRUE(handleB != handleInvalid);
        GUGU_UTEST_CHECK_TRUE(handleBB != handleInvalid);
        GUGU_UTEST_CHECK_TRUE(handleC != handleInvalid);
        GUGU_UTEST_CHECK_TRUE(handleA != handleAA);
        GUGU_UTEST_CHECK_TRUE(handleB != handleBB);
        GUGU_UTEST_CHECK_FALSE(handleC != Handle(dummyPtr, 1));

        GUGU_UTEST_CHECK_TRUE(handleInvalid == Handle());
        GUGU_UTEST_CHECK_TRUE(handleNullA == Handle(nullptr));
        GUGU_UTEST_CHECK_TRUE(handleNullB == Handle((size_t)0));
        GUGU_UTEST_CHECK_TRUE(handleNullC == Handle(nullptr, (size_t)0));
        GUGU_UTEST_CHECK_TRUE(handleA == Handle(dummyPtr));
        GUGU_UTEST_CHECK_TRUE(handleAA == Handle(dummyPtr, 0));
        GUGU_UTEST_CHECK_TRUE(handleB == Handle(1));
        GUGU_UTEST_CHECK_TRUE(handleBB == Handle(nullptr, 1));
        GUGU_UTEST_CHECK_TRUE(handleC == Handle(dummyPtr, 1));

        delete dummyPtr;

        std::vector<Handle> vector_handle;
        std::set<Handle> set_handle;
        std::map<Handle, std::string> map_handle;

        vector_handle.push_back(Handle());
        set_handle.insert(Handle());
        map_handle.insert(std::make_pair(Handle(), ""));
        map_handle[Handle()] = "";
        std::sort(vector_handle.begin(), vector_handle.end());
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("DeltaTime");
    {
        GUGU_UTEST_CHECK_TRUE(DeltaTime::Zero.IsZero());
        GUGU_UTEST_CHECK_EQUAL(DeltaTime::Zero.s(), 0.f);
        GUGU_UTEST_CHECK_EQUAL(DeltaTime::Zero.ms(), 0.f);
        GUGU_UTEST_CHECK_EQUAL(DeltaTime::Zero.micro(), 0);
        GUGU_UTEST_CHECK_EQUAL(DeltaTime::Zero.unscaled_s(), 0.f);
        GUGU_UTEST_CHECK_EQUAL(DeltaTime::Zero.unscaled_ms(), 0.f);
        GUGU_UTEST_CHECK_EQUAL(DeltaTime::Zero.unscaled_micro(), 0);
        GUGU_UTEST_CHECK_EQUAL(DeltaTime::Zero.GetScale(), 1.f);

        DeltaTime deltaTimeA(sf::Time(), sf::Time(), 1.f);
        GUGU_UTEST_CHECK_TRUE(deltaTimeA.IsZero());
        GUGU_UTEST_CHECK_EQUAL(deltaTimeA.s(), 0.f);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeA.ms(), 0.f);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeA.micro(), 0);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeA.unscaled_s(), 0.f);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeA.unscaled_ms(), 0.f);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeA.unscaled_micro(), 0);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeA.GetScale(), 1.f);

        DeltaTime deltaTimeB(sf::Time(sf::microseconds(int64(16000 * 0.1f))), sf::Time(sf::microseconds(16000)), 0.1f);
        GUGU_UTEST_CHECK_FALSE(deltaTimeB.IsZero());
        GUGU_UTEST_CHECK_APPROX_EQUAL(deltaTimeB.s(), 0.0016f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_EQUAL(deltaTimeB.ms(), 1.6f, math::Epsilon6);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeB.micro(), 1600);
        GUGU_UTEST_CHECK_APPROX_EQUAL(deltaTimeB.unscaled_s(), 0.016f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_EQUAL(deltaTimeB.unscaled_ms(), 16.f, math::Epsilon6);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeB.unscaled_micro(), 16000);
        GUGU_UTEST_CHECK_APPROX_EQUAL(deltaTimeB.GetScale(), 0.1f, math::Epsilon6);
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("Callback");
    {
        int counter = 0;
        Callback callback = [&counter]() { counter += 1; };

        callback();
        GUGU_UTEST_CHECK_EQUAL(counter, 1);

        callback();
        GUGU_UTEST_CHECK_EQUAL(counter, 2);
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("Signal");
    {
        // Tests with lambdas.
        int counter = 0;

        Signal<> signal0;
        signal0.Subscribe(Handle(nullptr), [&counter]() { counter += 1; });
        signal0.Notify();
        GUGU_UTEST_CHECK_EQUAL(counter, 1);

        Signal<int> signal1;
        signal1.Subscribe(Handle(nullptr), [&counter](int a) { counter += a; });
        signal1.Notify(2);
        GUGU_UTEST_CHECK_EQUAL(counter, 3);

        Signal<bool, int> signal2;
        signal2.Subscribe(Handle(nullptr), [&counter](bool a, int b) { counter += b; });
        signal2.Notify(false, 3);
        GUGU_UTEST_CHECK_EQUAL(counter, 6);

        std::string result;
        Signal<> signalA;

        signalA.Notify();
        GUGU_UTEST_CHECK_EQUAL(result, "");

        signalA.Subscribe(Handle(42), [&result]() { result += "1"; });
        signalA.Notify();
        GUGU_UTEST_CHECK_EQUAL(result, "1");

        signalA.Subscribe(Handle(42), [&result]() { result += "2"; });
        signalA.Notify();
        GUGU_UTEST_CHECK_EQUAL(result, "112");

        signalA.Notify();
        GUGU_UTEST_CHECK_EQUAL(result, "11212");

        signalA.Unsubscribe(Handle(42));
        signalA.Notify();
        GUGU_UTEST_CHECK_EQUAL(result, "11212");

        signalA.Notify();
        GUGU_UTEST_CHECK_EQUAL(result, "11212");

        // Tests with std::bind.
        struct TestCounter
        {
            int counter = 0;

            void Add(int value) { counter += value; }
        };

        TestCounter test;
        Signal<int> signalB;
        signalB.Subscribe(Handle(77), STD_BIND_1(&TestCounter::Add, &test));
        GUGU_UTEST_CHECK_EQUAL(test.counter, 0);

        signalB.Notify(10);
        GUGU_UTEST_CHECK_EQUAL(test.counter, 10);

        signalB.Notify(20);
        signalB.Notify(4);
        signalB.Notify(2);
        GUGU_UTEST_CHECK_EQUAL(test.counter, 36);
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}

}   // namespace tests
