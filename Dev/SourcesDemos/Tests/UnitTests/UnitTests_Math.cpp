////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"
#include "Gugu/Debug/UnitTest.h"
#include "Gugu/Math/MathUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

void RunUnitTests_Math()
{
    GUGU_UTEST_INIT("Math", "UnitTests_Math.log");

    //----------------------------------------------

    GUGU_UTEST_SECTION("Common");
    {
        GUGU_UTEST_SUBSECTION("Float Comparison");
        {
            GUGU_UTEST_CHECK(NearlyEquals(200.04f, 100.02f + 100.02f, Math::Epsilon3));
            GUGU_UTEST_CHECK(NearlyEquals(200.04f, 100.02f + 100.02f, Math::Epsilon6));
        }

        GUGU_UTEST_SUBSECTION("Angles");
        {
            GUGU_UTEST_CHECK(NearlyEquals<double>(ToDegrees(Math::Pi), 180.0, Math::Epsilon3));
            GUGU_UTEST_CHECK(NearlyEquals<double>(ToRadians(180.f), Math::Pi, Math::Epsilon3));
            GUGU_UTEST_CHECK(NearlyEquals(ToDegreesf(Math::Pi), 180.f, Math::Epsilon3));
            GUGU_UTEST_CHECK(NearlyEquals(ToRadiansf(180.f), Math::Pi, Math::Epsilon3));
        }

        GUGU_UTEST_SUBSECTION("Power");
        {
            GUGU_UTEST_CHECK(Power(5, 3) == 125);
            GUGU_UTEST_CHECK(Power(5, 1) == 5);
            GUGU_UTEST_CHECK(Power(5, 0) == 1);
            GUGU_UTEST_CHECK(NearlyEquals(Power(2.f, -2), 0.25f, Math::Epsilon3));
        }

        GUGU_UTEST_SUBSECTION("Clamp");
        {
            GUGU_UTEST_CHECK(Clamp(15, 10, 20) == 15);
            GUGU_UTEST_CHECK(Clamp(5, 10, 20) == 10);
            GUGU_UTEST_CHECK(Clamp(25, 10, 20) == 20);
            GUGU_UTEST_CHECK(ClampUnordered(15, 20, 10) == 15);
            GUGU_UTEST_CHECK(ClampUnordered(5, 20, 10) == 10);
            GUGU_UTEST_CHECK(ClampUnordered(25, 20, 10) == 20);
        }
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}
