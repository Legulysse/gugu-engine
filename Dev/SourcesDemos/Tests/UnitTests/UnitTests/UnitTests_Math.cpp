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

namespace tests {

void RunUnitTests_Math(UnitTestResults* results)
{
    GUGU_UTEST_INIT("Math", "UnitTests_Math.log", results);

    //----------------------------------------------

    GUGU_UTEST_SECTION("Common");
    {
        GUGU_UTEST_SUBSECTION("Float Comparison");
        {
            GUGU_UTEST_CHECK(!ApproxEqual(1000000.f, 1000001.f, math::Epsilon6));
            GUGU_UTEST_CHECK(!ApproxEqual(1000001.f, 1000000.f, math::Epsilon6));

            GUGU_UTEST_CHECK(ApproxEqual(1.001f, 1.002f, math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(1.002f, 1.001f, math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxEqual(1.001f, 1.003f, math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxEqual(1.003f, 1.001f, math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxEqual(1.001f, 1.002f, math::Epsilon6));
            GUGU_UTEST_CHECK(!ApproxEqual(1.002f, 1.001f, math::Epsilon6));

            GUGU_UTEST_CHECK(ApproxEqual(1.0001f, 1.0002f, math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(1.0002f, 1.0001f, math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxEqual(1.0001f, 1.0002f, math::Epsilon6));
            GUGU_UTEST_CHECK(!ApproxEqual(1.0002f, 1.0001f, math::Epsilon6));

            GUGU_UTEST_CHECK(ApproxEqual(1.0000001f, 1.0000002f, math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(1.0000002f, 1.0000001f, math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(1.0000001f, 1.0000002f, math::Epsilon6));
            GUGU_UTEST_CHECK(ApproxEqual(1.0000002f, 1.0000001f, math::Epsilon6));

            GUGU_UTEST_CHECK(ApproxEqual(0.3f, 0.300003f, math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(-0.3f, -0.300003f, math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxEqual(0.3f, 0.300003f, math::Epsilon6));
            GUGU_UTEST_CHECK(!ApproxEqual(-0.3f, -0.300003f, math::Epsilon6));

            GUGU_UTEST_CHECK(ApproxEqual(200.04f, 100.02f + 100.02f, math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(200.04f, 100.02f + 100.02f, math::Epsilon6));

            const float ratio = (100.f - 0.1f) / 100.f;
            GUGU_UTEST_CHECK(ApproxEqual(ratio, 1.f, math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxEqual(ratio, 1.f, math::Epsilon6));
            GUGU_UTEST_CHECK(!ApproxInferior(ratio, 1.f, math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxInferior(ratio, 1.f, math::Epsilon6));
            GUGU_UTEST_CHECK(!ApproxSuperior(ratio, 1.f, math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxSuperior(ratio, 1.f, math::Epsilon6));
            GUGU_UTEST_CHECK(ApproxInferiorOrEqual(ratio, 1.f, math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxInferiorOrEqual(ratio, 1.f, math::Epsilon6));
            GUGU_UTEST_CHECK(ApproxSuperiorOrEqual(ratio, 1.f, math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxSuperiorOrEqual(ratio, 1.f, math::Epsilon6));

            GUGU_UTEST_CHECK(ApproxEqual(1.f, ratio, math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxEqual(1.f, ratio, math::Epsilon6));
            GUGU_UTEST_CHECK(!ApproxInferior(1.f, ratio, math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxInferior(1.f, ratio, math::Epsilon6));
            GUGU_UTEST_CHECK(!ApproxSuperior(1.f, ratio, math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxSuperior(1.f, ratio, math::Epsilon6));
            GUGU_UTEST_CHECK(ApproxInferiorOrEqual(1.f, ratio, math::Epsilon3));
            GUGU_UTEST_CHECK(!ApproxInferiorOrEqual(1.f, ratio, math::Epsilon6));
            GUGU_UTEST_CHECK(ApproxSuperiorOrEqual(1.f, ratio, math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxSuperiorOrEqual(1.f, ratio, math::Epsilon6));
        }

        GUGU_UTEST_SUBSECTION("Absolute");
        {
            GUGU_UTEST_CHECK(Absolute(0) == 0);
            GUGU_UTEST_CHECK(Absolute(5) == 5);
            GUGU_UTEST_CHECK(Absolute(-5) == 5);
            GUGU_UTEST_CHECK(ApproxEqual(Absolute(0.f), 0.f, math::Epsilon6));
            GUGU_UTEST_CHECK(ApproxEqual(Absolute(5.5f), 5.5f, math::Epsilon6));
            GUGU_UTEST_CHECK(ApproxEqual(Absolute(-5.5f), 5.5f, math::Epsilon6));
        }

        GUGU_UTEST_SUBSECTION("Angles");
        {
            GUGU_UTEST_CHECK(ApproxEqual((float)ToDegrees(math::Pi), 180.f, math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual((float)ToRadians(180.f), math::Pi, math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(ToDegreesf(math::Pi), 180.f, math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(ToRadiansf(180.f), math::Pi, math::Epsilon3));
        }

        GUGU_UTEST_SUBSECTION("Power");
        {
            GUGU_UTEST_CHECK(Power(5, 3) == 125);
            GUGU_UTEST_CHECK(Power(5, 1) == 5);
            GUGU_UTEST_CHECK(Power(5, 0) == 1);
            GUGU_UTEST_CHECK(ApproxEqual(Power(2.f, -2), 0.25f, math::Epsilon3));
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

}   // namespace tests
