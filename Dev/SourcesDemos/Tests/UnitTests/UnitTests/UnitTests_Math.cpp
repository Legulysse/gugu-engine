////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/MathUtility.h"
#include "Gugu/Math/UDim.h"
#include "Gugu/Math/Random.h"

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
            GUGU_UTEST_CHECK(Clamp01(0.5f) == 0.5f);
            GUGU_UTEST_CHECK(Clamp01(-0.5f) == 0.f);
            GUGU_UTEST_CHECK(Clamp01(1.5f) == 1.f);
        }

        GUGU_UTEST_SUBSECTION("Rounding");
        {
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundNearest(10.5f), 11.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundNearest(-10.5f), -11.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundNearest(10.2f), 10.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundNearest(-10.2f), -10.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundNearest(10.8f), 11.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundNearest(-10.8f), -11.f, math::Epsilon6);

            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundFloor(10.2f), 10.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundFloor(-10.2f), -11.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundFloor(10.8f), 10.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundFloor(-10.8f), -11.f, math::Epsilon6);

            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundCeil(10.2f), 11.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundCeil(-10.2f), -10.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundCeil(10.8f), 11.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundCeil(-10.8f), -10.f, math::Epsilon6);

            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundTowardZero(10.2f), 10.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundTowardZero(-10.2f), -10.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundTowardZero(10.8f), 10.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundTowardZero(-10.8f), -10.f, math::Epsilon6);

            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundAwayFromZero(10.2f), 11.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundAwayFromZero(-10.2f), -11.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundAwayFromZero(10.8f), 11.f, math::Epsilon6);
            GUGU_UTEST_CHECK_APPROX_EQUAL(RoundAwayFromZero(-10.8f), -11.f, math::Epsilon6);
        }

        GUGU_UTEST_SUBSECTION("Distance");
        {
            GUGU_UTEST_CHECK_EQUAL(Distance(25, 35), 10);
            GUGU_UTEST_CHECK_EQUAL(Distance(35, 25), 10);
            GUGU_UTEST_CHECK_EQUAL(Distance(-5, 5), 10);
            GUGU_UTEST_CHECK_EQUAL(Distance(5, -5), 10);
            GUGU_UTEST_CHECK_EQUAL(Distance(-35, -25), 10);

            GUGU_UTEST_CHECK_TRUE(IsInRange(15, 10, 20));
            GUGU_UTEST_CHECK_FALSE(IsInRange(5, 10, 20));
            GUGU_UTEST_CHECK_FALSE(IsInRange(25, 10, 20));
            GUGU_UTEST_CHECK_FALSE(IsInRange(15, 20, 10));
            GUGU_UTEST_CHECK_FALSE(IsInRange(5, 20, 10));
            GUGU_UTEST_CHECK_FALSE(IsInRange(25, 20, 10));

            GUGU_UTEST_CHECK_TRUE(IsInRangeUnordered(15, 10, 20));
            GUGU_UTEST_CHECK_FALSE(IsInRangeUnordered(5, 10, 20));
            GUGU_UTEST_CHECK_FALSE(IsInRangeUnordered(25, 10, 20));
            GUGU_UTEST_CHECK_TRUE(IsInRangeUnordered(15, 20, 10));
            GUGU_UTEST_CHECK_FALSE(IsInRangeUnordered(5, 20, 10));
            GUGU_UTEST_CHECK_FALSE(IsInRangeUnordered(25, 20, 10));
        }
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("Interpolations");
    {
        GUGU_UTEST_CHECK_EQUAL(Lerp(20, 30, 0.f), 20);
        GUGU_UTEST_CHECK_EQUAL(Lerp(20, 30, 0.5f), 25);
        GUGU_UTEST_CHECK_EQUAL(Lerp(20, 30, 1.f), 30);
        GUGU_UTEST_CHECK_EQUAL(Lerp(20, 30, 2.f), 40);
        GUGU_UTEST_CHECK_EQUAL(Lerp(20, 30, -1.f), 10);
        GUGU_UTEST_CHECK_EQUAL(Lerp(20, 30, -2.f), 0);

        GUGU_UTEST_CHECK_EQUAL(Lerp(30, 20, 0.f), 30);
        GUGU_UTEST_CHECK_EQUAL(Lerp(30, 20, 0.5f), 25);
        GUGU_UTEST_CHECK_EQUAL(Lerp(30, 20, 1.f), 20);

        GUGU_UTEST_CHECK_EQUAL(Lerp(-20, 30, 0.f), -20);
        GUGU_UTEST_CHECK_EQUAL(Lerp(-20, 30, 0.5f), 5);
        GUGU_UTEST_CHECK_EQUAL(Lerp(-20, 30, 1.f), 30);

        GUGU_UTEST_CHECK_EQUAL(Lerp(-30, 20, 0.f), -30);
        GUGU_UTEST_CHECK_EQUAL(Lerp(-30, 20, 0.5f), -5);
        GUGU_UTEST_CHECK_EQUAL(Lerp(-30, 20, 1.f), 20);

        GUGU_UTEST_CHECK_APPROX_EQUAL(InverseLerp(20, 30, 25), 0.5f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_EQUAL(InverseLerp(20, 30, 40), 2.f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_EQUAL(InverseLerp(20, 30, 0), -2.f, math::Epsilon6);

        GUGU_UTEST_CHECK_APPROX_EQUAL(InverseLerp(30, 20, 25), 0.5f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_EQUAL(InverseLerp(30, 20, 10), 2.f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_EQUAL(InverseLerp(30, 20, 50), -2.f, math::Epsilon6);

        GUGU_UTEST_CHECK_APPROX_EQUAL(InverseLerp(-20, 30, 5), 0.5f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_EQUAL(InverseLerp(-20, 30, 80), 2.f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_EQUAL(InverseLerp(-20, 30, -120), -2.f, math::Epsilon6);

        GUGU_UTEST_CHECK_APPROX_EQUAL(InverseLerp(-30, 20, -5), 0.5f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_EQUAL(InverseLerp(-30, 20, 70), 2.f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_EQUAL(InverseLerp(-30, 20, -130), -2.f, math::Epsilon6);

        GUGU_UTEST_CHECK_EQUAL(RemapLerp(20, 30, 1, 5, 25), 3);
        GUGU_UTEST_CHECK_EQUAL(RemapLerp(20, 30, 1, 5, 40), 9);
        GUGU_UTEST_CHECK_EQUAL(RemapLerp(20, 30, 1, 5, 0), -7);

        GUGU_UTEST_CHECK_APPROX_EQUAL(RemapLerp(20, 30, 1.f, 5.f, 25), 3.f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_EQUAL(RemapLerp(20, 30, 1.f, 5.f, 40), 9.f, math::Epsilon6);
        GUGU_UTEST_CHECK_APPROX_EQUAL(RemapLerp(20, 30, 1.f, 5.f, 0), -7.f, math::Epsilon6);
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("Vector2");
    {
        GUGU_UTEST_CHECK_EQUAL(ToString(Vector2f(10.f, 20.f)), "(10, 20)");
        GUGU_UTEST_CHECK_EQUAL(ToString(Vector2f(10.5f, 20.5f)), "(10.5, 20.5)");

        Vector2f direction = Vector2f(10.f, 20.f);
        Vector2f normalized = Normalize(direction);
        Vector2f rotated = Rotate(direction, math::PiDivTwo);

        GUGU_UTEST_CHECK_APPROX_EQUAL(normalized, Vector2f(0.447214f, 0.894427f), math::Epsilon3);
        GUGU_UTEST_CHECK_APPROX_EQUAL(rotated, Vector2f(-20.f, 10.f), math::Epsilon3);
        GUGU_UTEST_CHECK_APPROX_EQUAL(Length(direction), 22.3607f, math::Epsilon3);
        GUGU_UTEST_CHECK_APPROX_EQUAL(Length(rotated), 22.3607f, math::Epsilon3);
        GUGU_UTEST_CHECK_APPROX_EQUAL(Length(normalized), 1.f, math::Epsilon3);
        GUGU_UTEST_CHECK_APPROX_EQUAL(LengthSquare(direction), 500.f, math::Epsilon3);
        GUGU_UTEST_CHECK_APPROX_EQUAL(LengthSquare(rotated), 500.f, math::Epsilon3);
        GUGU_UTEST_CHECK_APPROX_EQUAL(LengthSquare(normalized), 1.f, math::Epsilon3);
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("UDim");
    {
        GUGU_UTEST_CHECK_EQUAL(ToString(UDim2::POSITION_CENTER + Vector2f(10.f, 20.f)), "(rel=(0.5, 0.5), abs=(10, 20))");

        GUGU_UTEST_CHECK(UDim2::POSITION_CENTER + Vector2f(10.f, 20.f) == UDim2(Vector2f(0.5f, 0.5f), Vector2f(10.f, 20.f)));

        GUGU_UTEST_CHECK(UDim2::POSITION_TOP_LEFT.GetComputedDimension(100.f, 50.f) == Vector2f(0.f, 0.f));
        GUGU_UTEST_CHECK(UDim2::POSITION_CENTER.GetComputedDimension(100.f, 50.f) == Vector2f(50.f, 25.f));
        GUGU_UTEST_CHECK(UDim2::POSITION_BOTTOM_RIGHT.GetComputedDimension(100.f, 50.f) == Vector2f(100.f, 50.f));

        GUGU_UTEST_CHECK(UDim2::POSITION_TOP_LEFT.GetPixelAlignedComputedDimension(125.f, 55.f) == Vector2f(0.f, 0.f));
        GUGU_UTEST_CHECK(UDim2::POSITION_CENTER.GetPixelAlignedComputedDimension(125.f, 55.f) == Vector2f(62.f, 27.f));
        GUGU_UTEST_CHECK(UDim2::POSITION_BOTTOM_RIGHT.GetPixelAlignedComputedDimension(125.f, 55.f) == Vector2f(125.f, 55.f));
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("Random");
    {
        // Setup
        ResetRandSeed();

        GUGU_UTEST_SECTION("Default");
        {
            std::map<size_t, int> resultsA;
            std::map<int, int> resultsB;
            std::set<float> resultsC;
            std::set<float> resultsD;

            for (int i = 0; i < 100000; ++i)
            {
                resultsA[GetRandom(10)] += 1;
            }

            for (int i = 0; i < 100000; ++i)
            {
                resultsB[GetRandom(-5, 5)] += 1;
            }

            for (int i = 0; i < 100000; ++i)
            {
                resultsC.insert(GetRandomf(10.f));
            }

            for (int i = 0; i < 100000; ++i)
            {
                resultsD.insert(GetRandomf(-5.f, 5.f));
            }

            GUGU_UTEST_CHECK_EQUAL(resultsA.size(), 10);
            GUGU_UTEST_CHECK_EQUAL(resultsB.size(), 11);
            GUGU_UTEST_CHECK(ApproxEqual(*resultsC.begin(), 0.f, math::Epsilon6));
            GUGU_UTEST_CHECK(ApproxInferior(*resultsC.rbegin(), 10.f, math::Epsilon6));
            GUGU_UTEST_CHECK(ApproxEqual(*resultsD.begin(), -5.f, math::Epsilon6));
            GUGU_UTEST_CHECK(ApproxInferior(*resultsD.rbegin(), 5.f, math::Epsilon6));
        }

        GUGU_UTEST_SECTION("Weighted");
        {
            std::vector<int> weightsA { 1, 0, 9 };

            std::map<int, int> resultsA;
            for (int i = 0; i < 100000; ++i)
            {
                resultsA[GetWeightedRandomIndex(weightsA)] += 1;
            }

            GUGU_UTEST_CHECK_EQUAL(resultsA.size(), 2);

            struct ItemType
            {
                int weight;
            };
            std::vector<ItemType> itemsA { {1}, { 0 }, { -1 }, { 8 } };

            auto predicateA = [](const ItemType& item) -> int { return item.weight; };

            std::map<int, int> resultsB;
            for (int i = 0; i < 10000; ++i)
            {
                resultsB[GetWeightedRandomIndex(itemsA, predicateA)] += 1;
            }

            GUGU_UTEST_CHECK_EQUAL(resultsB.size(), 2);
        }
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}

}   // namespace tests
