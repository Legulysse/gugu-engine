////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Grid/SquareGrid.h"
#include "Gugu/Misc/Grid/HexGrid.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace tests {

void RunUnitTests_Grid(UnitTestResults* results)
{
    GUGU_UTEST_INIT("Grid", "UnitTests_Grid.log", results);

    //----------------------------------------------

    GUGU_UTEST_SECTION("PickCoords");
    {
        SquareGrid squareGrid;
        squareGrid.InitSquareGrid(10, 10, 5.f, 5.f, false);

        HexGrid hexGrid;
        hexGrid.InitHexGrid(10, 10, 5.f);

        Vector2i pickedCoords;

        GUGU_UTEST_CHECK_TRUE(squareGrid.PickCoords(Vector2f(0.f, 0.f), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(0, 0));
        GUGU_UTEST_CHECK_TRUE(squareGrid.PickCoords(Vector2f(49.9f, 49.9f), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(9, 9));
        GUGU_UTEST_CHECK_FALSE(squareGrid.PickCoords(Vector2f(-0.1f, -0.1f), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(-1, -1));
        GUGU_UTEST_CHECK_FALSE(squareGrid.PickCoords(Vector2f(50.f, 50.f), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(10, 10));

        GUGU_UTEST_CHECK_TRUE(squareGrid.PickCoordsClamped(Vector2f(0.f, 0.f), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(0, 0));
        GUGU_UTEST_CHECK_TRUE(squareGrid.PickCoordsClamped(Vector2f(49.9f, 49.9f), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(9, 9));
        GUGU_UTEST_CHECK_FALSE(squareGrid.PickCoordsClamped(Vector2f(-0.1f, -0.1f), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(0, 0));
        GUGU_UTEST_CHECK_FALSE(squareGrid.PickCoordsClamped(Vector2f(50.f, 50.f), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(9, 9));

        GUGU_UTEST_CHECK_TRUE(hexGrid.PickCoords(Vector2f(2.5f, 0.f), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(0, 0));
        GUGU_UTEST_CHECK_TRUE(hexGrid.PickCoords(hexGrid.GetCellCenter(Vector2i(9, 9)), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(9, 9));
        GUGU_UTEST_CHECK_FALSE(hexGrid.PickCoords(Vector2f(0.f, 0.f), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(-1, -1));
        GUGU_UTEST_CHECK_FALSE(hexGrid.PickCoords(hexGrid.GetCellCenter(Vector2i(10, 10)), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(10, 10));

        GUGU_UTEST_CHECK_TRUE(hexGrid.PickCoordsClamped(Vector2f(2.5f, 0.f), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(0, 0));
        GUGU_UTEST_CHECK_TRUE(hexGrid.PickCoordsClamped(hexGrid.GetCellCenter(Vector2i(9, 9)), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(9, 9));
        GUGU_UTEST_CHECK_FALSE(hexGrid.PickCoordsClamped(Vector2f(0.f, 0.f), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(0, 0));
        GUGU_UTEST_CHECK_FALSE(hexGrid.PickCoordsClamped(hexGrid.GetCellCenter(Vector2i(10, 10)), pickedCoords));
        GUGU_UTEST_CHECK_EQUAL(pickedCoords, Vector2i(9, 9));
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}

}   // namespace tests
