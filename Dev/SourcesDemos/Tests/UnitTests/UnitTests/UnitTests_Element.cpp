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
#include "Gugu/Scene/ManagerScenes.h"
#include "Gugu/Scene/Scene.h"
#include "Gugu/System/Memory.h"
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

    //----------------------------------------------

    GUGU_UTEST_SECTION("Hierarchy");
    {
        Element* root = new Element;
        Element* elementA = root->AddChild<Element>();
        Element* elementB = elementA->AddChild<Element>();

        GUGU_UTEST_CHECK_EQUAL(elementB->GetTopAncestor(), root);
        GUGU_UTEST_CHECK_TRUE(elementB->HasAncestor(root));
        GUGU_UTEST_CHECK_TRUE(elementB->HasAncestor(elementA));
        GUGU_UTEST_CHECK_TRUE(elementB->HasAncestor(elementB));
        GUGU_UTEST_CHECK_EQUAL(elementA->GetTopAncestor(), root);
        GUGU_UTEST_CHECK_TRUE(elementA->HasAncestor(root));
        GUGU_UTEST_CHECK_TRUE(elementA->HasAncestor(elementA));
        GUGU_UTEST_CHECK_FALSE(elementA->HasAncestor(elementB));
        GUGU_UTEST_CHECK_EQUAL(root->GetTopAncestor(), root);
        GUGU_UTEST_CHECK_TRUE(root->HasAncestor(root));
        GUGU_UTEST_CHECK_FALSE(root->HasAncestor(elementA));
        GUGU_UTEST_CHECK_FALSE(root->HasAncestor(elementB));

        SafeDelete(root);
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("Unified Dimensions");
    {
        GUGU_UTEST_SUBSECTION("Origin");
        {
            //----------------------------------------------
            // Base tests.

            Element* root = new Element;
            root->SetSize(100.f, 100.f);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetOrigin(), Vector2f(0.f, 0.f), math::Epsilon3));

            root->SetUnifiedOrigin(UDim2::POSITION_CENTER);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetOrigin(), Vector2f(50.f, 50.f), math::Epsilon3));

            root->SetSize(50.f, 50.f);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetOrigin(), Vector2f(25.f, 25.f), math::Epsilon3));

            root->SetUnifiedOrigin(UDim2::POSITION_TOP_RIGHT + Vector2f(10.f, -10.f));
            GUGU_UTEST_CHECK(ApproxEqual(root->GetOrigin(), Vector2f(60.f, -10.f), math::Epsilon3));

            root->ResetUnifiedOrigin();
            root->SetSize(100.f, 50.f);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetOrigin(), Vector2f(60.f, -10.f), math::Epsilon3));

            //----------------------------------------------
            // Finalize.

            SafeDelete(root);
        }

        GUGU_UTEST_SUBSECTION("Position");
        {
            //----------------------------------------------
            // Base tests.

            Element* root = new Element;
            root->SetSize(100.f, 100.f);

            Element* elementA = root->AddChild<Element>();
            elementA->SetSize(10.f, 10.f);

            GUGU_UTEST_CHECK(ApproxEqual(root->GetPosition(), Vector2f(0.f, 0.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementA->GetPosition(), Vector2f(0.f, 0.f), math::Epsilon3));

            elementA->SetUnifiedPosition(UDim2::POSITION_CENTER);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetPosition(), Vector2f(0.f, 0.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementA->GetPosition(), Vector2f(50.f, 50.f), math::Epsilon3));

            root->SetSize(50.f, 25.f);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetPosition(), Vector2f(0.f, 0.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementA->GetPosition(), Vector2f(25.f, 12.f), math::Epsilon3));

            elementA->SetUnifiedPosition(UDim2::POSITION_BOTTOM_RIGHT + Vector2f(-10.f, -10.f));
            GUGU_UTEST_CHECK(ApproxEqual(root->GetPosition(), Vector2f(0.f, 0.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementA->GetPosition(), Vector2f(40.f, 15.f), math::Epsilon3));

            elementA->ResetUnifiedPosition();
            root->SetSize(100.f, 50.f);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetPosition(), Vector2f(0.f, 0.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementA->GetPosition(), Vector2f(40.f, 15.f), math::Epsilon3));

            //----------------------------------------------
            // Set UDim before attaching to parent.

            Element* elementB = new Element;
            elementB->SetSize(5.f, 5.f);
            elementB->SetUnifiedPosition(UDim2::POSITION_CENTER_LEFT);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetPosition(), Vector2f(0.f, 0.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementB->GetPosition(), Vector2f(0.f, 0.f), math::Epsilon3));

            root->AddChild(elementB);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetPosition(), Vector2f(0.f, 0.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementB->GetPosition(), Vector2f(0.f, 25.f), math::Epsilon3));

            //----------------------------------------------
            // Finalize.

            SafeDelete(root);
        }

        GUGU_UTEST_SUBSECTION("Size (parent ref)");
        {
            //----------------------------------------------
            // Base tests.

            Element* root = new Element;
            root->SetSize(100.f, 100.f);

            Element* elementA = root->AddChild<Element>();
            elementA->SetSize(10.f, 10.f);

            GUGU_UTEST_CHECK(ApproxEqual(root->GetSize(), Vector2f(100.f, 100.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementA->GetSize(), Vector2f(10.f, 10.f), math::Epsilon3));

            elementA->SetUnifiedSize(UDim2::SIZE_FULL);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetSize(), Vector2f(100.f, 100.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementA->GetSize(), Vector2f(100.f, 100.f), math::Epsilon3));

            root->SetSize(50.f, 25.f);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetSize(), Vector2f(50.f, 25.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementA->GetSize(), Vector2f(50.f, 25.f), math::Epsilon3));

            elementA->SetUnifiedSize(UDim2::SIZE_FULL + Vector2f(-10.f, -5.f));
            GUGU_UTEST_CHECK(ApproxEqual(root->GetSize(), Vector2f(50.f, 25.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementA->GetSize(), Vector2f(40.f, 20.f), math::Epsilon3));

            elementA->ResetUnifiedSize();
            root->SetSize(100.f, 50.f);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetSize(), Vector2f(100.f, 50.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementA->GetSize(), Vector2f(40.f, 20.f), math::Epsilon3));

            //----------------------------------------------
            // Set UDim before attaching to parent.

            Element* elementB = new Element;
            elementB->SetSize(5.f, 5.f);
            elementB->SetUnifiedSize(UDim2::SIZE_HORIZONTAL_HALF);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetSize(), Vector2f(100.f, 50.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementB->GetSize(), Vector2f(5.f, 5.f), math::Epsilon3));

            root->AddChild(elementB);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetSize(), Vector2f(100.f, 50.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementB->GetSize(), Vector2f(100.f, 25.f), math::Epsilon3));

            //----------------------------------------------
            // Finalize.

            SafeDelete(root);
        }

        GUGU_UTEST_SUBSECTION("Size (child ref)");
        {
            //----------------------------------------------
            // Base tests.

            Element* root = new Element;
            root->SetSize(100.f, 100.f);

            Element* elementA = root->AddChild<Element>();
            elementA->SetSize(10.f, 10.f);

            GUGU_UTEST_CHECK(ApproxEqual(root->GetSize(), Vector2f(100.f, 100.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementA->GetSize(), Vector2f(10.f, 10.f), math::Epsilon3));

            root->SetUnifiedSize(UDim2::SIZE_FULL, elementA);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetSize(), Vector2f(10.f, 10.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementA->GetSize(), Vector2f(10.f, 10.f), math::Epsilon3));

            elementA->SetSize(80.f, 80.f);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetSize(), Vector2f(80.f, 80.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementA->GetSize(), Vector2f(80.f, 80.f), math::Epsilon3));

            root->SetUnifiedSize(UDim2::SIZE_HORIZONTAL_HALF);
            elementA->SetSize(50.f, 50.f);
            GUGU_UTEST_CHECK(ApproxEqual(root->GetSize(), Vector2f(50.f, 25.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementA->GetSize(), Vector2f(50.f, 50.f), math::Epsilon3));

            root->ResetUnifiedSize();
            elementA->SetSize(100.f, 50.f);
            GUGU_UTEST_CHECK(!root->GetUseUnifiedSize());
            GUGU_UTEST_CHECK(ApproxEqual(root->GetSize(), Vector2f(50.f, 25.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementA->GetSize(), Vector2f(100.f, 50.f), math::Epsilon3));

            //----------------------------------------------
            // Delete reference after an initial attachment.

            root->SetUnifiedSize(UDim2::SIZE_FULL, elementA);
            GUGU_UTEST_CHECK(root->GetUseUnifiedSize());

            SafeDelete(elementA);
            GUGU_UTEST_CHECK(!root->GetUseUnifiedSize());

            //----------------------------------------------
            // Change reference after an initial attachment.

            root->SetSize(10.f, 10.f);
            Element* elementB = root->AddChild<Element>();
            elementB->SetSize(20.f, 20.f);
            Element* elementC = root->AddChild<Element>();
            elementC->SetSize(30.f, 30.f);
            GUGU_UTEST_CHECK(!root->GetUseUnifiedSize());
            GUGU_UTEST_CHECK(ApproxEqual(root->GetSize(), Vector2f(10.f, 10.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementB->GetSize(), Vector2f(20.f, 20.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementC->GetSize(), Vector2f(30.f, 30.f), math::Epsilon3));

            root->SetUnifiedSize(UDim2::SIZE_FULL, elementB);
            GUGU_UTEST_CHECK(root->GetUseUnifiedSize());
            GUGU_UTEST_CHECK(ApproxEqual(root->GetSize(), Vector2f(20.f, 20.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementB->GetSize(), Vector2f(20.f, 20.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementC->GetSize(), Vector2f(30.f, 30.f), math::Epsilon3));

            root->SetUnifiedSize(UDim2::SIZE_FULL, elementC);
            GUGU_UTEST_CHECK(root->GetUseUnifiedSize());
            GUGU_UTEST_CHECK(ApproxEqual(root->GetSize(), Vector2f(30.f, 30.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementB->GetSize(), Vector2f(20.f, 20.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(elementC->GetSize(), Vector2f(30.f, 30.f), math::Epsilon3));

            SafeDelete(elementB);
            GUGU_UTEST_CHECK(root->GetUseUnifiedSize());

            SafeDelete(elementC);
            GUGU_UTEST_CHECK(!root->GetUseUnifiedSize());

            //----------------------------------------------
            // Finalize.

            SafeDelete(root);
        }
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("Transform");
    {
        GUGU_UTEST_SUBSECTION("Local Global Transform");
        {
            Element* root = new Element;
            Element* elementA = root->AddChild<Element>();
            Element* elementB = elementA->AddChild<Element>();

            root->SetPosition(10.f, 10.f);
            root->SetOrigin(-10.f, -10.f);
            elementA->SetPosition(10.f, 10.f);
            elementA->SetOrigin(-10.f, -10.f);
            elementB->SetPosition(10.f, 10.f);
            elementB->SetOrigin(-10.f, -10.f);

            GUGU_UTEST_CHECK_APPROX_EQUAL(root->TransformToGlobal(Vector2f(5.f, 5.f)), Vector2f(25.f, 25.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementA->TransformToGlobal(Vector2f(5.f, 5.f)), Vector2f(45.f, 45.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementB->TransformToGlobal(Vector2f(5.f, 5.f)), Vector2f(65.f, 65.f), math::Epsilon3);

            GUGU_UTEST_CHECK_APPROX_EQUAL(root->TransformToLocal(Vector2f(25.f, 25.f)), Vector2f(5.f, 5.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementA->TransformToLocal(Vector2f(45.f, 45.f)), Vector2f(5.f, 5.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementB->TransformToLocal(Vector2f(65.f, 65.f)), Vector2f(5.f, 5.f), math::Epsilon3);

            GUGU_UTEST_CHECK_APPROX_EQUAL(Vector2f(5.f, 5.f), elementB->TransformToLocal(elementB->TransformToGlobal(Vector2f(5.f, 5.f))), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(Vector2f(65.f, 65.f), elementB->TransformToGlobal(elementB->TransformToLocal(Vector2f(65.f, 65.f))), math::Epsilon3);

            root->SetPosition(0.f, 0.f);
            root->SetOrigin(0.f, 0.f);
            elementA->SetOrigin(5.f, 5.f);
            elementA->SetPosition(10.f, 20.f);
            elementA->SetRotation(90.f);
            elementA->SetScale(2.f, 3.f);
            elementB->SetOrigin(10.f, 10.f);
            elementB->SetPosition(10.f, 0.f);
            elementB->SetRotation(90.f);
            elementB->SetScale(10.f, 10.f);

            GUGU_UTEST_CHECK_APPROX_EQUAL(root->TransformToGlobal(Vector2f(10.f, 5.f)), Vector2f(10.f, 5.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementA->TransformToGlobal(Vector2f(-2.5f, 5.f)), Vector2f(10.f, 5.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementB->TransformToGlobal(Vector2f(10.5f, 11.25f)), Vector2f(10.f, 5.f), math::Epsilon3);

            GUGU_UTEST_CHECK_APPROX_EQUAL(root->TransformToLocal(Vector2f(10.f, 5.f)), Vector2f(10.f, 5.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementA->TransformToLocal(Vector2f(10.f, 5.f)), Vector2f(-2.5f, 5.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementB->TransformToLocal(Vector2f(10.f, 5.f)), Vector2f(10.5f, 11.25f), math::Epsilon3);

            GUGU_UTEST_CHECK_APPROX_EQUAL(Vector2f(10.5f, 11.25f), elementB->TransformToLocal(elementB->TransformToGlobal(Vector2f(10.5f, 11.25f))), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(Vector2f(10.f, 5.f), elementB->TransformToGlobal(elementB->TransformToLocal(Vector2f(10.f, 5.f))), math::Epsilon3);

            SafeDelete(root);
        }

        GUGU_UTEST_SUBSECTION("Ancestor Transform");
        {
            Element* root = new Element;
            Element* elementA = root->AddChild<Element>();
            Element* elementB = elementA->AddChild<Element>();

            root->SetPosition(10.f, 10.f);
            root->SetOrigin(-10.f, -10.f);
            elementA->SetPosition(10.f, 10.f);
            elementA->SetOrigin(-10.f, -10.f);
            elementB->SetPosition(10.f, 10.f);
            elementB->SetOrigin(-10.f, -10.f);

            GUGU_UTEST_CHECK_APPROX_EQUAL(elementB->TransformToGlobal(Vector2f(5.f, 5.f)), Vector2f(65.f, 65.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementB->TransformToGlobal(Vector2f(5.f, 5.f), root), Vector2f(45.f, 45.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementB->TransformToGlobal(Vector2f(5.f, 5.f), elementA), Vector2f(25.f, 25.f), math::Epsilon3);

            GUGU_UTEST_CHECK_APPROX_EQUAL(elementB->TransformToLocal(Vector2f(65.f, 65.f)), Vector2f(5.f, 5.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementB->TransformToLocal(Vector2f(45.f, 45.f), root), Vector2f(5.f, 5.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementB->TransformToLocal(Vector2f(25.f, 25.f), elementA), Vector2f(5.f, 5.f), math::Epsilon3);

            root->SetPosition(0.f, 0.f);
            root->SetOrigin(0.f, 0.f);
            elementA->SetOrigin(5.f, 5.f);
            elementA->SetPosition(10.f, 20.f);
            elementA->SetRotation(90.f);
            elementA->SetScale(2.f, 3.f);
            elementB->SetOrigin(10.f, 10.f);
            elementB->SetPosition(10.f, 0.f);
            elementB->SetRotation(90.f);
            elementB->SetScale(10.f, 10.f);

            GUGU_UTEST_CHECK_APPROX_EQUAL(root->TransformToLocal(Vector2f(10.f, 5.f), root), Vector2f(10.f, 5.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementA->TransformToLocal(Vector2f(10.f, 5.f), elementA), Vector2f(10.f, 5.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementA->TransformToLocal(Vector2f(10.f, 5.f), root), Vector2f(-2.5f, 5.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementB->TransformToLocal(Vector2f(10.f, 5.f), elementB), Vector2f(10.f, 5.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementB->TransformToLocal(Vector2f(10.f, 5.f), elementA), Vector2f(10.5f, 10.f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementB->TransformToLocal(Vector2f(10.f, 5.f), root), Vector2f(10.5f, 11.25f), math::Epsilon3);
            GUGU_UTEST_CHECK_APPROX_EQUAL(elementB->TransformToLocal(elementA->TransformToLocal(Vector2f(10.f, 5.f), root), elementA), Vector2f(10.5f, 11.25f), math::Epsilon3);

            SafeDelete(root);
        }
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("Picking");
    {
        GUGU_UTEST_SUBSECTION("Local Picking");
        {
            Element* root = new Element;
            Element* elementA = root->AddChild<Element>();
            elementA->SetSize(10.f, 10.f);

            GUGU_UTEST_CHECK(elementA->IsPickedLocal(Vector2f(0.f, 0.f)));
            GUGU_UTEST_CHECK(elementA->IsPickedLocal(Vector2f(9.f, 9.f)));
            GUGU_UTEST_CHECK(elementA->IsPickedLocal(Vector2f(9.5f, 9.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPickedLocal(Vector2f(-.5f, -.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPickedLocal(Vector2f(10.f, 10.f)));

            elementA->SetPosition(10.f, 10.f);

            GUGU_UTEST_CHECK(elementA->IsPickedLocal(Vector2f(0.f, 0.f)));
            GUGU_UTEST_CHECK(elementA->IsPickedLocal(Vector2f(9.f, 9.f)));
            GUGU_UTEST_CHECK(elementA->IsPickedLocal(Vector2f(9.5f, 9.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPickedLocal(Vector2f(-.5f, -.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPickedLocal(Vector2f(10.f, 10.f)));

            elementA->SetOrigin(10.f, 10.f);

            GUGU_UTEST_CHECK(elementA->IsPickedLocal(Vector2f(0.f, 0.f)));
            GUGU_UTEST_CHECK(elementA->IsPickedLocal(Vector2f(9.f, 9.f)));
            GUGU_UTEST_CHECK(elementA->IsPickedLocal(Vector2f(9.5f, 9.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPickedLocal(Vector2f(-.5f, -.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPickedLocal(Vector2f(10.f, 10.f)));

            elementA->SetRotation(90.f);

            GUGU_UTEST_CHECK(elementA->IsPickedLocal(Vector2f(0.f, 0.f)));
            GUGU_UTEST_CHECK(elementA->IsPickedLocal(Vector2f(9.f, 9.f)));
            GUGU_UTEST_CHECK(elementA->IsPickedLocal(Vector2f(9.5f, 9.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPickedLocal(Vector2f(-.5f, -.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPickedLocal(Vector2f(10.f, 10.f)));

            elementA->SetScale(2.f, 3.f);

            GUGU_UTEST_CHECK(elementA->IsPickedLocal(Vector2f(0.f, 0.f)));
            GUGU_UTEST_CHECK(elementA->IsPickedLocal(Vector2f(9.f, 9.f)));
            GUGU_UTEST_CHECK(elementA->IsPickedLocal(Vector2f(9.5f, 9.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPickedLocal(Vector2f(-.5f, -.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPickedLocal(Vector2f(10.f, 10.f)));

            SafeDelete(root);
        }

        GUGU_UTEST_SUBSECTION("Global Picking");
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
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(0.f, 0.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(0.f, 0.f), math::Epsilon3));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(5.f, 5.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(5.f, 5.f), math::Epsilon3));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(10.f, 10.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(10.f, 10.f), math::Epsilon3));

            elementA->SetPosition(10.f, 10.f);

            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(10.f, 10.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(19.f, 19.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(19.5f, 19.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(9.5f, 9.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(20.f, 20.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(10.f, 10.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(0.f, 0.f), math::Epsilon3));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(15.f, 15.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(5.f, 5.f), math::Epsilon3));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(20.f, 20.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(10.f, 10.f), math::Epsilon3));

            elementA->SetOrigin(10.f, 10.f);

            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(0.f, 0.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(9.f, 9.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(9.5f, 9.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(-.5f, -.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(10.f, 10.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(0.f, 0.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(0.f, 0.f), math::Epsilon3));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(5.f, 5.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(5.f, 5.f), math::Epsilon3));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(10.f, 10.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(10.f, 10.f), math::Epsilon3));

            elementA->SetRotation(90.f);

            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(20.f, 0.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(11.f, 9.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(10.5f, 9.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(20.5f, -.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(10.f, 10.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(20.f, 0.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(0.f, 0.f), math::Epsilon3));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(15.f, 5.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(5.f, 5.f), math::Epsilon3));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(10.f, 10.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(10.f, 10.f), math::Epsilon3));

            elementA->SetScale(1.f, 2.f);

            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(30.f, 0.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(11.f, 9.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(10.5f, 9.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(30.5f, -.5f)));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(10.f, 10.f)));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(30.f, 0.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(0.f, 0.f), math::Epsilon3));
            GUGU_UTEST_CHECK(elementA->IsPicked(Vector2f(20.f, 5.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(5.f, 5.f), math::Epsilon3));
            GUGU_UTEST_CHECK(!elementA->IsPicked(Vector2f(10.f, 10.f), picked));
            GUGU_UTEST_CHECK(ApproxEqual(picked, Vector2f(10.f, 10.f), math::Epsilon3));

            SafeDelete(root);
        }

        GUGU_UTEST_SUBSECTION("Camera Picking");
        {
            // Check Window state.
            GUGU_UTEST_CHECK(GetGameWindow()->GetSize() == Vector2u(200, 200));

            Camera* camera = GetGameWindow()->CreateCamera();
            Element* root = GetGameWindow()->GetRootNode()->AddChild<Element>();

            // Check picking element not belonging to the camera (neither scene nor window).
            {
                Element* elementInvalid = new Element;
                elementInvalid->SetSize(10.f, 10.f);

                GUGU_UTEST_CHECK(elementInvalid->IsPicked(Vector2f(0.f, 0.f)));
                GUGU_UTEST_CHECK(!camera->IsMouseOverElement(Vector2i(0, 0), elementInvalid));

                SafeDelete(elementInvalid);
            }

            // Check picking elements through a scene with a secondary camera.
            {
                Scene* scene = GetScenes()->GetRootScene();
                Camera* cameraScene = GetGameWindow()->CreateCamera();
                GetGameWindow()->BindScene(scene, cameraScene);

                Element* elementWindowA = root->AddChild<Element>();
                elementWindowA->SetSize(10.f, 10.f);

                Element* elementWindowB = root->AddChild<Element>();
                elementWindowB->SetSize(10.f, 10.f);
                elementWindowB->SetPosition(100.f, 100.f);

                Element* elementSceneA = scene->GetRootNode()->AddChild<Element>();
                elementSceneA->SetSize(10.f, 10.f);

                Element* elementSceneB = scene->GetRootNode()->AddChild<Element>();
                elementSceneB->SetSize(10.f, 10.f);
                elementSceneB->SetPosition(100.f, 100.f);

                cameraScene->SetCenterOnTarget(false);
                cameraScene->SetTarget(0.f, 0.f);

                GUGU_UTEST_CHECK_FALSE(cameraScene->IsMouseOverElement(Vector2i(5, 5), elementWindowA));
                GUGU_UTEST_CHECK_FALSE(cameraScene->IsMouseOverElement(Vector2i(105, 105), elementWindowA));
                GUGU_UTEST_CHECK_FALSE(cameraScene->IsMouseOverElement(Vector2i(5, 5), elementWindowB));
                GUGU_UTEST_CHECK_FALSE(cameraScene->IsMouseOverElement(Vector2i(105, 105), elementWindowB));
                GUGU_UTEST_CHECK_TRUE(cameraScene->IsMouseOverElement(Vector2i(5, 5), elementSceneA));
                GUGU_UTEST_CHECK_FALSE(cameraScene->IsMouseOverElement(Vector2i(105, 105), elementSceneA));
                GUGU_UTEST_CHECK_FALSE(cameraScene->IsMouseOverElement(Vector2i(5, 5), elementSceneB));
                GUGU_UTEST_CHECK_TRUE(cameraScene->IsMouseOverElement(Vector2i(105, 105), elementSceneB));

                cameraScene->SetCenterOnTarget(true);
                cameraScene->SetTarget(0.f, 0.f);

                GUGU_UTEST_CHECK_FALSE(cameraScene->IsMouseOverElement(Vector2i(5, 5), elementWindowA));
                GUGU_UTEST_CHECK_FALSE(cameraScene->IsMouseOverElement(Vector2i(105, 105), elementWindowA));
                GUGU_UTEST_CHECK_FALSE(cameraScene->IsMouseOverElement(Vector2i(5, 5), elementWindowB));
                GUGU_UTEST_CHECK_FALSE(cameraScene->IsMouseOverElement(Vector2i(105, 105), elementWindowB));
                GUGU_UTEST_CHECK_FALSE(cameraScene->IsMouseOverElement(Vector2i(5, 5), elementSceneA));
                GUGU_UTEST_CHECK_TRUE(cameraScene->IsMouseOverElement(Vector2i(105, 105), elementSceneA));
                GUGU_UTEST_CHECK_FALSE(cameraScene->IsMouseOverElement(Vector2i(5, 5), elementSceneB));
                GUGU_UTEST_CHECK_FALSE(cameraScene->IsMouseOverElement(Vector2i(105, 105), elementSceneB));
            }

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
