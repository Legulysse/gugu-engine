////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/MathUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

void RunUnitTests_Math(UnitTestResults* results)
{
    GUGU_UTEST_INIT("Math", "UnitTests_Math.log", results);

    //----------------------------------------------

    GUGU_UTEST_SECTION("Common");
    {
        GUGU_UTEST_SUBSECTION("Float Comparison");
        {
            GUGU_UTEST_CHECK(!ApproxEqual(1000000.f, 1000001.f, Math::Epsilon6));
            GUGU_UTEST_CHECK(!ApproxEqual(1000001.f, 1000000.f, Math::Epsilon6));

            GUGU_UTEST_CHECK(ApproxEqual(1.001f, 1.002f, Math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(1.002f, 1.001f, Math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxEqual(1.001f, 1.003f, Math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxEqual(1.003f, 1.001f, Math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxEqual(1.001f, 1.002f, Math::Epsilon6));
            GUGU_UTEST_CHECK(!ApproxEqual(1.002f, 1.001f, Math::Epsilon6));

            GUGU_UTEST_CHECK(ApproxEqual(1.0001f, 1.0002f, Math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(1.0002f, 1.0001f, Math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxEqual(1.0001f, 1.0002f, Math::Epsilon6));
            GUGU_UTEST_CHECK(!ApproxEqual(1.0002f, 1.0001f, Math::Epsilon6));

            GUGU_UTEST_CHECK(ApproxEqual(1.0000001f, 1.0000002f, Math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(1.0000002f, 1.0000001f, Math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(1.0000001f, 1.0000002f, Math::Epsilon6));
            GUGU_UTEST_CHECK(ApproxEqual(1.0000002f, 1.0000001f, Math::Epsilon6));

            GUGU_UTEST_CHECK(ApproxEqual(0.3f, 0.300003f, Math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(-0.3f, -0.300003f, Math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxEqual(0.3f, 0.300003f, Math::Epsilon6));
            GUGU_UTEST_CHECK(!ApproxEqual(-0.3f, -0.300003f, Math::Epsilon6));

            GUGU_UTEST_CHECK(ApproxEqual(200.04f, 100.02f + 100.02f, Math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(200.04f, 100.02f + 100.02f, Math::Epsilon6));

            float ratio = (100.f - 0.1f) / 100.f;
            GUGU_UTEST_CHECK(ApproxEqual(ratio, 1.f, Math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxEqual(ratio, 1.f, Math::Epsilon6));
            GUGU_UTEST_CHECK(!ApproxInferior(ratio, 1.f, Math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxInferior(ratio, 1.f, Math::Epsilon6));
            GUGU_UTEST_CHECK(ApproxInferiorOrEqual(ratio, 1.f, Math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxInferiorOrEqual(ratio, 1.f, Math::Epsilon6));
            GUGU_UTEST_CHECK(ApproxEqual(1.f, ratio, Math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxEqual(1.f, ratio, Math::Epsilon6));
            GUGU_UTEST_CHECK(!ApproxSuperior(1.f, ratio, Math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxSuperior(1.f, ratio, Math::Epsilon6));
            GUGU_UTEST_CHECK(ApproxSuperiorOrEqual(1.f, ratio, Math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxSuperiorOrEqual(1.f, ratio, Math::Epsilon6));
        }

        GUGU_UTEST_SUBSECTION("Angles");
        {
            GUGU_UTEST_CHECK(ApproxEqual((float)ToDegrees(Math::Pi), 180.f, Math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual((float)ToRadians(180.f), Math::Pi, Math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(ToDegreesf(Math::Pi), 180.f, Math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(ToRadiansf(180.f), Math::Pi, Math::Epsilon3));
        }

        GUGU_UTEST_SUBSECTION("Power");
        {
            GUGU_UTEST_CHECK(Power(5, 3) == 125);
            GUGU_UTEST_CHECK(Power(5, 1) == 5);
            GUGU_UTEST_CHECK(Power(5, 0) == 1);
            GUGU_UTEST_CHECK(ApproxEqual(Power(2.f, -2), 0.25f, Math::Epsilon3));
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
