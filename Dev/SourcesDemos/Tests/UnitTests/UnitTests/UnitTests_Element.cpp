////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"
#include "Gugu/Core/EngineConfig.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/Math/MathUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace tests {

// TODO: move this elsewhere (need a Vector2.cpp ?).
bool ApproxEqual(const Vector2f& left, const Vector2f& right, float epsilon)
{
    return gugu::ApproxEqual(left.x, right.x, epsilon) && gugu::ApproxEqual(left.y, right.y, epsilon);
}

void RunUnitTests_Element(UnitTestResults* results)
{
    GUGU_UTEST_INIT("Element", "UnitTests_Element.log", results);

    //----------------------------------------------

    GUGU_UTEST_SECTION("Picking");
    {
        GUGU_UTEST_SUBSECTION("Local Transform");
        {
            Element* root = new Element;
            Element* elementA = root->AddChild<Element>();
            elementA->SetSize(10.f, 10.f);

            Vector2f picked;
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(0.f, 0.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(9.f, 9.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(9.5f, 9.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(-.5f, -.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(10.f, 10.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(5.f, 5.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(5.f, 5.f), math::Epsilon3));

            elementA->SetPosition(10.f, 10.f);

            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(10.f, 10.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(19.f, 19.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(19.5f, 19.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(9.5f, 9.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(20.f, 20.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(15.f, 15.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(5.f, 5.f), math::Epsilon3));

            // TODO: Rotate.
            // TODO: Scale.
            // TODO: Origin.

            SafeDelete(root);
        }

        // TODO: Global Transform (test local/global conversions through all kinds of intermediate transforms).

        GUGU_UTEST_SUBSECTION("Camera Transform");
        {
            // Check Window state.
            GUGU_UTEST_CHECK(GetGameWindow()->GetSize() == Vector2u(200, 200));

            Camera* camera = GetGameWindow()->CreateCamera();
            Element* root = GetGameWindow()->GetRootNode()->AddChild<Element>();

            // Check picking element not belonging to the camera (neither scene nor window).
            Element* elementInvalid = new Element;
            elementInvalid->SetSize(10.f, 10.f);

            GUGU_UTEST_CHECK(elementInvalid->IsPicked(Vector2f(0.f, 0.f)));
            GUGU_UTEST_CHECK(!camera->IsMouseOverElement(Vector2i(0, 0), elementInvalid));

            SafeDelete(elementInvalid);

            Element* elementA = root->AddChild<Element>();
            elementA->SetSize(10.f, 10.f);

            camera->SetCenterOnTarget(false);
            camera->SetTarget(0.f, 0.f);
            camera->SetZoom(1.f);

            Vector2f picked;
            GUGU_UTEST_CHECK(camera->IsMouseOverElement(Vector2i(0, 0), elementA));
            GUGU_UTEST_CHECK(camera->IsMouseOverElement(Vector2i(9, 9), elementA));
            GUGU_UTEST_CHECK(!camera->IsMouseOverElement(Vector2i(-1, -1), elementA));
            GUGU_UTEST_CHECK(!camera->IsMouseOverElement(Vector2i(10, 10), elementA));
            GUGU_UTEST_CHECK(camera->IsMouseOverElement(Vector2i(5, 5), elementA, picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(5.f, 5.f), math::Epsilon3));

            camera->SetCenterOnTarget(false);
            camera->SetTarget(0.f, 0.f);
            camera->SetZoom(0.5f);

            GUGU_UTEST_CHECK(camera->IsMouseOverElement(Vector2i(0, 0), elementA));
            GUGU_UTEST_CHECK(camera->IsMouseOverElement(Vector2i(4, 4), elementA));
            GUGU_UTEST_CHECK(!camera->IsMouseOverElement(Vector2i(-1, -1), elementA));
            GUGU_UTEST_CHECK(!camera->IsMouseOverElement(Vector2i(5, 5), elementA));
            GUGU_UTEST_CHECK(camera->IsMouseOverElement(Vector2i(2, 2), elementA, picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(4.f, 4.f), math::Epsilon3));

            camera->SetCenterOnTarget(false);
            camera->SetTarget(-100.f, 0.f);
            camera->SetZoom(1.f);

            GUGU_UTEST_CHECK(camera->IsMouseOverElement(Vector2i(100, 0), elementA));
            GUGU_UTEST_CHECK(camera->IsMouseOverElement(Vector2i(109, 9), elementA));
            GUGU_UTEST_CHECK(!camera->IsMouseOverElement(Vector2i(99, -1), elementA));
            GUGU_UTEST_CHECK(!camera->IsMouseOverElement(Vector2i(110, 10), elementA));
            GUGU_UTEST_CHECK(camera->IsMouseOverElement(Vector2i(105, 5), elementA, picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(5.f, 5.f), math::Epsilon3));

            camera->SetCenterOnTarget(false);
            camera->SetTarget(-100.f, 0.f);
            camera->SetZoom(0.5f);

            GUGU_UTEST_CHECK(camera->IsMouseOverElement(Vector2i(50, 0), elementA));
            GUGU_UTEST_CHECK(camera->IsMouseOverElement(Vector2i(54, 4), elementA));
            GUGU_UTEST_CHECK(!camera->IsMouseOverElement(Vector2i(49, -1), elementA));
            GUGU_UTEST_CHECK(!camera->IsMouseOverElement(Vector2i(55, 5), elementA));
            GUGU_UTEST_CHECK(camera->IsMouseOverElement(Vector2i(52, 2), elementA, picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(4.f, 4.f), math::Epsilon3));

            // TODO: Here I only check the last case for centerOnTarget=true (modified target+zoom), I could check intermediate cases too.
            camera->SetCenterOnTarget(true);
            camera->SetTarget(100.f, 0.f);
            camera->SetZoom(0.5f);

            GUGU_UTEST_CHECK(camera->IsMouseOverElement(Vector2i(50, 100), elementA));
            GUGU_UTEST_CHECK(camera->IsMouseOverElement(Vector2i(54, 104), elementA));
            GUGU_UTEST_CHECK(!camera->IsMouseOverElement(Vector2i(49, 99), elementA));
            GUGU_UTEST_CHECK(!camera->IsMouseOverElement(Vector2i(55, 105), elementA));
            GUGU_UTEST_CHECK(camera->IsMouseOverElement(Vector2i(52, 102), elementA, picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(4.f, 4.f), math::Epsilon3));

            SafeDelete(root);
            GetGameWindow()->DeleteCamera(camera);
        }
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}

}   // namespace tests
