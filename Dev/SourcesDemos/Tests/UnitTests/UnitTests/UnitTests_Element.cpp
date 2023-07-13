////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"
#include "Gugu/Element/ElementData.h"
#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Core/EngineConfig.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/External/PugiXmlUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace tests {

void RunUnitTests_Element(UnitTestResults* results)
{
    GUGU_UTEST_INIT("Element", "UnitTests_Element.log", results);

    //----------------------------------------------

    GUGU_UTEST_SECTION("ElementWidget");
    {
        static const std::string dummyElementWidgetString = {
            "<ElementWidget serializationVersion=\"1\">"
                "<Element type=\"Element\" name=\"root\">"
                    "<Children>"
                        "<Element type=\"ElementSprite\" name=\"sprite_1\">"
                            "<Size x=\"128\" y=\"128\"/>"
                        "</Element>"
                        "<Element type=\"Element\">"
                            "<Children>"
                                "<Element type=\"ElementSprite\" name=\"sprite_2\">"
                                    "<Size x=\"128\" y=\"128\"/>"
                                "</Element>"
                            "</Children>"
                        "</Element>"
                    "</Children>"
                "</Element>"
            "</ElementWidget>"
        };

        static const std::string dummyElementDataString = {
            "<Element type=\"Element\" name=\"root\">"
                "<Children>"
                    "<Element type=\"ElementSprite\" name=\"sprite_1\">"
                        "<Size x=\"128\" y=\"128\"/>"
                    "</Element>"
                    "<Element type=\"Element\">"
                        "<Children>"
                            "<Element type=\"ElementSprite\" name=\"sprite_2\">"
                                "<Size x=\"128\" y=\"128\"/>"
                            "</Element>"
                        "</Children>"
                    "</Element>"
                "</Children>"
            "</Element>"
        };

        ElementWidget* dummyWidget = new ElementWidget;

        GUGU_UTEST_SUBSECTION("Serialization");
        {
            // Test ElementWidget Serialization
            if (GUGU_UTEST_CHECK(dummyWidget->LoadFromString(dummyElementWidgetString)))
            {
                if (GUGU_UTEST_CHECK(dummyWidget->GetRootData()))
                {
                    GUGU_UTEST_CHECK(dummyWidget->GetRootData()->GetSerializedType() == "Element");

                    if (GUGU_UTEST_CHECK(dummyWidget->GetRootData()->children.size() == 2))
                    {
                        GUGU_UTEST_CHECK(dummyWidget->GetRootData()->children[0]->GetSerializedType() == "ElementSprite");
                        GUGU_UTEST_CHECK(dummyWidget->GetRootData()->children[0]->children.size() == 0);

                        ElementSpriteData* spriteData = dynamic_cast<ElementSpriteData*>(dummyWidget->GetRootData()->children[0]);
                        if (GUGU_UTEST_CHECK(spriteData != nullptr))
                        {
                            GUGU_UTEST_CHECK(ApproxEqual(spriteData->size, Vector2f(128, 128), math::Epsilon3));
                        }

                        GUGU_UTEST_CHECK(dummyWidget->GetRootData()->children[1]->GetSerializedType() == "Element");
                        GUGU_UTEST_CHECK(dummyWidget->GetRootData()->children[1]->children.size() == 1);
                    }
                }

                std::string saveResultString;
                dummyWidget->SaveToString(saveResultString);
                GUGU_UTEST_CHECK(saveResultString == dummyElementWidgetString);
            }

            // Test single ElementData Serialization
            pugi::xml_document xmlLoadDocument = xml::ParseDocumentFromString(dummyElementDataString);
            pugi::xml_node xmlRootNode = xmlLoadDocument.first_child();

            BaseElementData* dummyElementData = InstanciateElementData(xmlRootNode);
            ElementParseContext parseContext;
            parseContext.node = xmlRootNode;
            if (GUGU_UTEST_CHECK(dummyElementData->LoadFromXml(parseContext)))
            {
                pugi::xml_document xmlSaveDocument;
                ElementSaveContext saveContext;
                saveContext.node = xmlSaveDocument;
                if (GUGU_UTEST_CHECK(dummyElementData->SaveToXml(saveContext)))
                {
                    std::string xmlSaveString = xml::SaveDocumentToString(xmlSaveDocument);
                    GUGU_UTEST_CHECK(xmlSaveString == dummyElementDataString);
                }
            }

            SafeDelete(dummyElementData);
        }

        GUGU_UTEST_SUBSECTION("Path Bindings");
        {
            ElementPathBindings pathBindings;
            Element* dummyWidgetInstance = dummyWidget->InstanciateWidget(pathBindings);

            if (GUGU_UTEST_CHECK(dummyWidgetInstance != nullptr))
            {
                GUGU_UTEST_CHECK(pathBindings.elementFromPath.size() == 3);
                GUGU_UTEST_CHECK(pathBindings.GetElement("root") != nullptr);
                GUGU_UTEST_CHECK(pathBindings.GetElement("sprite_1") == nullptr);
                GUGU_UTEST_CHECK(pathBindings.GetElement("sprite_2") == nullptr);
                GUGU_UTEST_CHECK(pathBindings.GetElement("root/sprite_1") != nullptr);
                GUGU_UTEST_CHECK(pathBindings.GetElement("root/sprite_2") != nullptr);
            }

            SafeDelete(dummyWidgetInstance);
        }

        SafeDelete(dummyWidget);
    }

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
