////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Handle.h"
#include "Gugu/Math/MathUtility.h"

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

    GUGU_UTEST_FINALIZE();
}

}   // namespace tests
