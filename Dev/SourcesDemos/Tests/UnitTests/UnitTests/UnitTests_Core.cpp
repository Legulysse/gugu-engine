////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"
#include "Gugu/Core/Handle.h"

#include <SFML/System/Sleep.hpp>

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
        GUGU_UTEST_CHECK(true);
        GUGU_UTEST_CHECK_TRUE(true);
        GUGU_UTEST_CHECK_FALSE(false);
        GUGU_UTEST_CHECK_EQUAL(10, 10);
        GUGU_UTEST_CHECK_APPROX_EQUAL(10.f, 10.f, math::Epsilon6);
        GUGU_UTEST_CHECK_NOT_EQUAL(10, 11);
        GUGU_UTEST_CHECK_NOT_APPROX_EQUAL(10.f, 11.f, math::Epsilon6);

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

    GUGU_UTEST_SECTION("Handle");
    {
        int* dummyPtr = new int;

        Handle handleNull;
        Handle handleNullA(nullptr);
        Handle handleNullB((size_t)0);
        Handle handleNullC(nullptr, (size_t)0);

        Handle handleA(dummyPtr);
        Handle handleAA(dummyPtr, 0);
        Handle handleB(1);
        Handle handleBB(nullptr, 1);
        Handle handleC(dummyPtr, 1);

        GUGU_UTEST_CHECK_TRUE(handleNull.IsNull());
        GUGU_UTEST_CHECK_TRUE(handleNullA.IsNull());
        GUGU_UTEST_CHECK_TRUE(handleNullB.IsNull());
        GUGU_UTEST_CHECK_TRUE(handleNullC.IsNull());

        GUGU_UTEST_CHECK_FALSE(handleA.IsNull());
        GUGU_UTEST_CHECK_FALSE(handleAA.IsNull());
        GUGU_UTEST_CHECK_FALSE(handleB.IsNull());
        GUGU_UTEST_CHECK_FALSE(handleBB.IsNull());
        GUGU_UTEST_CHECK_FALSE(handleC.IsNull());

        GUGU_UTEST_CHECK_TRUE(handleNull == Handle());
        GUGU_UTEST_CHECK_TRUE(handleNull == Handle(nullptr));
        GUGU_UTEST_CHECK_TRUE(handleNull == Handle((size_t)0));
        GUGU_UTEST_CHECK_TRUE(handleNull == Handle(nullptr, 0));
        GUGU_UTEST_CHECK_TRUE(handleNull == handleNullA);
        GUGU_UTEST_CHECK_TRUE(handleNull == handleNullB);
        GUGU_UTEST_CHECK_TRUE(handleNull == handleNullC);

        GUGU_UTEST_CHECK_FALSE(handleA == handleNull);
        GUGU_UTEST_CHECK_FALSE(handleAA == handleNull);
        GUGU_UTEST_CHECK_FALSE(handleB == handleNull);
        GUGU_UTEST_CHECK_FALSE(handleBB == handleNull);
        GUGU_UTEST_CHECK_FALSE(handleC == handleNull);
        GUGU_UTEST_CHECK_TRUE(handleA == handleAA);
        GUGU_UTEST_CHECK_TRUE(handleB == handleBB);
        GUGU_UTEST_CHECK_TRUE(handleC == Handle(dummyPtr, 1));

        GUGU_UTEST_CHECK_TRUE(handleA != handleNull);
        GUGU_UTEST_CHECK_TRUE(handleAA != handleNull);
        GUGU_UTEST_CHECK_TRUE(handleB != handleNull);
        GUGU_UTEST_CHECK_TRUE(handleBB != handleNull);
        GUGU_UTEST_CHECK_TRUE(handleC != handleNull);
        GUGU_UTEST_CHECK_FALSE(handleA != handleAA);
        GUGU_UTEST_CHECK_FALSE(handleB != handleBB);
        GUGU_UTEST_CHECK_FALSE(handleC != Handle(dummyPtr, 1));

        delete dummyPtr;
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("DeltaTime");
    {
        DeltaTime deltaTimeA(sf::Time(), sf::Time(), 1.f);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeA.s(), 0);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeA.ms(), 0);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeA.micro(), 0);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeA.unscaled_s(), 0);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeA.unscaled_ms(), 0);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeA.unscaled_micro(), 0);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeA.GetScale(), 1.f);

        DeltaTime deltaTimeB(sf::Time(sf::microseconds(sf::Int64(16000 * 0.1f))), sf::Time(sf::microseconds(16000)), 0.1f);
        GUGU_UTEST_CHECK_APPROX_EQUAL(deltaTimeB.s(), 0.0016f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_EQUAL(deltaTimeB.ms(), 1.6f, math::Epsilon6);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeB.micro(), 1600);
        GUGU_UTEST_CHECK_APPROX_EQUAL(deltaTimeB.unscaled_s(), 0.016f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_EQUAL(deltaTimeB.unscaled_ms(), 16.f, math::Epsilon6);
        GUGU_UTEST_CHECK_EQUAL(deltaTimeB.unscaled_micro(), 16000);
        GUGU_UTEST_CHECK_APPROX_EQUAL(deltaTimeB.GetScale(), 0.1f, math::Epsilon6);
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}

}   // namespace tests
