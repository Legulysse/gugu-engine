////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/UDim.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/External/PugiXmlUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace tests {

void RunUnitTests_Xml(UnitTestResults* results)
{
    GUGU_UTEST_INIT("Xml", "UnitTests_Xml.log", results);

    //----------------------------------------------

    GUGU_UTEST_SECTION("String Writer");
    {
        pugi::xml_document document;
        pugi::xml_node nodeRoot = document.append_child("Root");
        nodeRoot.append_attribute("version").set_value(10);
        pugi::xml_node nodeData = nodeRoot.append_child("Data");
        nodeData.append_attribute("name").set_value("age");
        nodeData.append_attribute("value").set_value("30");

        GUGU_UTEST_CHECK(xml::SaveDocumentToString(document) == "<Root version=\"10\"><Data name=\"age\" value=\"30\"/></Root>");
    }

    GUGU_UTEST_SECTION("Read Attribute");
    {
        GUGU_UTEST_SUBSECTION("Vector2i");
        {
            pugi::xml_document document = xml::ParseDocumentFromString("<Data x=\"10\" y=\"20\"/><IllFormedData x=\"10\"/>");
            pugi::xml_node nodeData = document.child("Data");
            pugi::xml_node nodeNoData = document.child("NoData");
            pugi::xml_node nodeIllData = document.child("IllFormedData");

            const Vector2i defaultData(30, 40);

            // Read (no default)
            GUGU_UTEST_CHECK(xml::ReadVector2i(nodeData) == Vector2i(10, 20));
            GUGU_UTEST_CHECK(xml::ReadVector2i(nodeNoData) == Vector2::Zero_i);
            GUGU_UTEST_CHECK(xml::ReadVector2i(nodeIllData) == Vector2i(10, 0));

            // Read (default)
            GUGU_UTEST_CHECK(xml::ReadVector2i(nodeData, defaultData) == Vector2i(10, 20));
            GUGU_UTEST_CHECK(xml::ReadVector2i(nodeNoData, defaultData) == Vector2i(30, 40));
            GUGU_UTEST_CHECK(xml::ReadVector2i(nodeIllData, defaultData) == Vector2i(10, 40));

            // Parse (no default)
            Vector2i resultParseData(5, 5);
            xml::ParseVector2i(nodeData, resultParseData);
            GUGU_UTEST_CHECK(resultParseData == Vector2i(10, 20));

            Vector2i resultParseNoData(5, 5);
            xml::ParseVector2i(nodeNoData, resultParseNoData);
            GUGU_UTEST_CHECK(resultParseNoData == Vector2::Zero_i);

            Vector2i resultParseIllData(5, 5);
            xml::ParseVector2i(nodeIllData, resultParseIllData);
            GUGU_UTEST_CHECK(resultParseIllData == Vector2i(10, 0));

            // Parse (default)
            Vector2i resultParseDataWithDefault(5, 5);
            xml::ParseVector2i(nodeData, resultParseDataWithDefault, defaultData);
            GUGU_UTEST_CHECK(resultParseDataWithDefault == Vector2i(10, 20));

            Vector2i resultParseNoDataWithDefault(5, 5);
            xml::ParseVector2i(nodeNoData, resultParseNoDataWithDefault, defaultData);
            GUGU_UTEST_CHECK(resultParseNoDataWithDefault == Vector2i(30, 40));

            Vector2i resultParseIllDataWithDefault(5, 5);
            xml::ParseVector2i(nodeIllData, resultParseIllDataWithDefault, defaultData);
            GUGU_UTEST_CHECK(resultParseIllDataWithDefault == Vector2i(10, 40));

            // TryParse
            Vector2i resultTryParseData(5, 5);
            if (GUGU_UTEST_CHECK(xml::TryParseVector2i(nodeData, resultTryParseData)))
            {
                GUGU_UTEST_CHECK(resultTryParseData == Vector2i(10, 20));
            }

            Vector2i resultTryParseNoData(5, 5);
            GUGU_UTEST_CHECK(!xml::TryParseVector2i(nodeNoData, resultTryParseNoData));
            GUGU_UTEST_CHECK(resultTryParseNoData == Vector2i(5, 5));

            Vector2i resultTryParseIllData(5, 5);
            if (GUGU_UTEST_CHECK(xml::TryParseVector2i(nodeIllData, resultTryParseIllData)))
            {
                GUGU_UTEST_CHECK(resultTryParseIllData == Vector2i(10, 5));
            }
        }

        GUGU_UTEST_SUBSECTION("Vector2f");
        {
            pugi::xml_document document = xml::ParseDocumentFromString("<Data x=\"10\" y=\"20\"/><IllFormedData x=\"10\"/>");
            pugi::xml_node nodeData = document.child("Data");
            pugi::xml_node nodeNoData = document.child("NoData");
            pugi::xml_node nodeIllData = document.child("IllFormedData");

            const Vector2f defaultData(30.f, 40.f);

            // Read (no default)
            GUGU_UTEST_CHECK(ApproxEqual(xml::ReadVector2f(nodeData), Vector2f(10.f, 20.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(xml::ReadVector2f(nodeNoData), Vector2::Zero_f, math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(xml::ReadVector2f(nodeIllData), Vector2f(10.f, 0.f), math::Epsilon3));

            // Read (default)
            GUGU_UTEST_CHECK(ApproxEqual(xml::ReadVector2f(nodeData, defaultData), Vector2f(10.f, 20.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(xml::ReadVector2f(nodeNoData, defaultData), Vector2f(30.f, 40.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(xml::ReadVector2f(nodeIllData, defaultData), Vector2f(10.f, 40.f), math::Epsilon3));

            // Parse (no default)
            Vector2f resultParseData(5.f, 5.f);
            xml::ParseVector2f(nodeData, resultParseData);
            GUGU_UTEST_CHECK(ApproxEqual(resultParseData, Vector2f(10.f, 20.f), math::Epsilon3));

            Vector2f resultParseNoData(5.f, 5.f);
            xml::ParseVector2f(nodeNoData, resultParseNoData);
            GUGU_UTEST_CHECK(ApproxEqual(resultParseNoData, Vector2::Zero_f, math::Epsilon3));

            Vector2f resultParseIllData(5.f, 5.f);
            xml::ParseVector2f(nodeIllData, resultParseIllData);
            GUGU_UTEST_CHECK(ApproxEqual(resultParseIllData, Vector2f(10.f, 0.f), math::Epsilon3));

            // Parse (default)
            Vector2f resultParseDataWithDefault(5.f, 5.f);
            xml::ParseVector2f(nodeData, resultParseDataWithDefault, defaultData);
            GUGU_UTEST_CHECK(ApproxEqual(resultParseDataWithDefault, Vector2f(10.f, 20.f), math::Epsilon3));

            Vector2f resultParseNoDataWithDefault(5.f, 5.f);
            xml::ParseVector2f(nodeNoData, resultParseNoDataWithDefault, defaultData);
            GUGU_UTEST_CHECK(ApproxEqual(resultParseNoDataWithDefault, Vector2f(30.f, 40.f), math::Epsilon3));

            Vector2f resultParseIllDataWithDefault(5.f, 5.f);
            xml::ParseVector2f(nodeIllData, resultParseIllDataWithDefault, defaultData);
            GUGU_UTEST_CHECK(ApproxEqual(resultParseIllDataWithDefault, Vector2f(10.f, 40.f), math::Epsilon3));

            // TryParse
            Vector2f resultTryParseData(5.f, 5.f);
            if (GUGU_UTEST_CHECK(xml::TryParseVector2f(nodeData, resultTryParseData)))
            {
                GUGU_UTEST_CHECK(ApproxEqual(resultTryParseData, Vector2f(10.f, 20.f), math::Epsilon3));
            }

            Vector2f resultTryParseNoData(5.f, 5.f);
            GUGU_UTEST_CHECK(!xml::TryParseVector2f(nodeNoData, resultTryParseNoData));
            GUGU_UTEST_CHECK(ApproxEqual(resultTryParseNoData, Vector2f(5.f, 5.f), math::Epsilon3));

            Vector2f resultTryParseIllData(5.f, 5.f);
            if (GUGU_UTEST_CHECK(xml::TryParseVector2f(nodeIllData, resultTryParseIllData)))
            {
                GUGU_UTEST_CHECK(ApproxEqual(resultTryParseIllData, Vector2f(10.f, 5.f), math::Epsilon3));
            }
        }

        GUGU_UTEST_SUBSECTION("UDim2");
        {
            pugi::xml_document document = xml::ParseDocumentFromString("<Data xRel=\"10\" yRel=\"20\" xAbs=\"100\" yAbs=\"200\"/>");
            pugi::xml_node nodeData = document.child("Data");
            pugi::xml_node nodeNoData = document.child("NoData");

            const UDim2 defaultData(Vector2f(30, 40), Vector2f(300, 400));

            // Parse (no default)
            UDim2 resultParseData(Vector2f(5, 5), Vector2f(5, 5));
            xml::ParseUDim2(nodeData, resultParseData);
            GUGU_UTEST_CHECK(resultParseData == UDim2(Vector2f(10, 20), Vector2f(100, 200)));

            UDim2 resultParseNoData(Vector2f(5, 5), Vector2f(5, 5));
            xml::ParseUDim2(nodeNoData, resultParseNoData);
            GUGU_UTEST_CHECK(resultParseNoData == UDim2::ZERO);

            // Parse (default)
            UDim2 resultParseDataWithDefault(Vector2f(5, 5), Vector2f(5, 5));
            xml::ParseUDim2(nodeData, resultParseDataWithDefault, defaultData);
            GUGU_UTEST_CHECK(resultParseDataWithDefault == UDim2(Vector2f(10, 20), Vector2f(100, 200)));

            UDim2 resultParseNoDataWithDefault(Vector2f(5, 5), Vector2f(5, 5));
            xml::ParseUDim2(nodeNoData, resultParseNoDataWithDefault, defaultData);
            GUGU_UTEST_CHECK(resultParseNoDataWithDefault == UDim2(Vector2f(30, 40), Vector2f(300, 400)));

            // TryParse
            UDim2 resultTryParseData(Vector2f(5, 5), Vector2f(5, 5));
            if (GUGU_UTEST_CHECK(xml::TryParseUDim2(nodeData, resultTryParseData)))
            {
                GUGU_UTEST_CHECK(resultTryParseData == UDim2(Vector2f(10, 20), Vector2f(100, 200)));
            }

            UDim2 resultTryParseNoData(Vector2f(5, 5), Vector2f(5, 5));
            GUGU_UTEST_CHECK(!xml::TryParseUDim2(nodeNoData, resultTryParseNoData));
            GUGU_UTEST_CHECK(resultTryParseNoData == UDim2(Vector2f(5, 5), Vector2f(5, 5)));
        }

        GUGU_UTEST_SUBSECTION("Rect");
        {
            pugi::xml_document document = xml::ParseDocumentFromString("<Data x=\"10\" y=\"20\" w=\"100\" h=\"200\"/>");
            pugi::xml_node nodeData = document.child("Data");
            pugi::xml_node nodeNoData = document.child("NoData");

            const sf::IntRect defaultData(Vector2i(30, 40), Vector2i(300, 400));

            // Read (no default)
            GUGU_UTEST_CHECK(xml::ReadRect(nodeData) == sf::IntRect(Vector2i(10, 20), Vector2i(100, 200)));
            GUGU_UTEST_CHECK(xml::ReadRect(nodeNoData) == sf::IntRect());

            // Read (default)
            GUGU_UTEST_CHECK(xml::ReadRect(nodeData, defaultData) == sf::IntRect(Vector2i(10, 20), Vector2i(100, 200)));
            GUGU_UTEST_CHECK(xml::ReadRect(nodeNoData, defaultData) == sf::IntRect(Vector2i(30, 40), Vector2i(300, 400)));

            // Parse (no default)
            sf::IntRect resultParseData(Vector2i(5, 5), Vector2i(5, 5));
            xml::ParseRect(nodeData, resultParseData);
            GUGU_UTEST_CHECK(resultParseData == sf::IntRect(Vector2i(10, 20), Vector2i(100, 200)));

            sf::IntRect resultParseNoData(Vector2i(5, 5), Vector2i(5, 5));
            xml::ParseRect(nodeNoData, resultParseNoData);
            GUGU_UTEST_CHECK(resultParseNoData == sf::IntRect());

            // Parse (default)
            sf::IntRect resultParseDataWithDefault(Vector2i(5, 5), Vector2i(5, 5));
            xml::ParseRect(nodeData, resultParseDataWithDefault, defaultData);
            GUGU_UTEST_CHECK(resultParseDataWithDefault == sf::IntRect(Vector2i(10, 20), Vector2i(100, 200)));

            sf::IntRect resultParseNoDataWithDefault(Vector2i(5, 5), Vector2i(5, 5));
            xml::ParseRect(nodeNoData, resultParseNoDataWithDefault, defaultData);
            GUGU_UTEST_CHECK(resultParseNoDataWithDefault == sf::IntRect(Vector2i(30, 40), Vector2i(300, 400)));

            // TryParse
            sf::IntRect resultTryParseData(Vector2i(5, 5), Vector2i(5, 5));
            if (GUGU_UTEST_CHECK(xml::TryParseRect(nodeData, resultTryParseData)))
            {
                GUGU_UTEST_CHECK(resultTryParseData == sf::IntRect(Vector2i(10, 20), Vector2i(100, 200)));
            }

            sf::IntRect resultTryParseNoData(Vector2i(5, 5), Vector2i(5, 5));
            GUGU_UTEST_CHECK(!xml::TryParseRect(nodeNoData, resultTryParseNoData));
            GUGU_UTEST_CHECK(resultTryParseNoData == sf::IntRect(Vector2i(5, 5), Vector2i(5, 5)));
        }

        GUGU_UTEST_SUBSECTION("Color");
        {
            GUGU_UTEST_CHECK(sf::Color(0, 0, 0, 255) == sf::Color::Black);

            pugi::xml_document document = xml::ParseDocumentFromString("<Data r=\"10\" g=\"20\" b=\"30\" a=\"128\"/>");
            pugi::xml_node nodeData = document.child("Data");
            pugi::xml_node nodeNoData = document.child("NoData");

            const sf::Color defaultData(16, 32, 64, 200);

            // Parse (no default)
            sf::Color resultParseData(5, 5, 5, 5);
            xml::ParseColor(nodeData, resultParseData);
            GUGU_UTEST_CHECK(resultParseData == sf::Color(10, 20, 30, 128));

            sf::Color resultParseNoData(5, 5, 5, 5);
            xml::ParseColor(nodeNoData, resultParseNoData);
            GUGU_UTEST_CHECK(resultParseNoData == sf::Color::Black);

            // Parse (default)
            sf::Color resultParseDataWithDefault(5, 5, 5, 5);
            xml::ParseColor(nodeData, resultParseDataWithDefault, defaultData);
            GUGU_UTEST_CHECK(resultParseDataWithDefault == sf::Color(10, 20, 30, 128));

            sf::Color resultParseNoDataWithDefault(5, 5, 5, 5);
            xml::ParseColor(nodeNoData, resultParseNoDataWithDefault, defaultData);
            GUGU_UTEST_CHECK(resultParseNoDataWithDefault == sf::Color(16, 32, 64, 200));

            // TryParse
            sf::Color resultTryParseData(5, 5, 5, 5);
            if (GUGU_UTEST_CHECK(xml::TryParseColor(nodeData, resultTryParseData)))
            {
                GUGU_UTEST_CHECK(resultTryParseData == sf::Color(10, 20, 30, 128));
            }

            sf::Color resultTryParseNoData(5, 5, 5, 5);
            GUGU_UTEST_CHECK(!xml::TryParseColor(nodeNoData, resultTryParseNoData));
            GUGU_UTEST_CHECK(resultTryParseNoData == sf::Color(5, 5, 5, 5));
        }
    }

    GUGU_UTEST_SECTION("Write Attribute");
    {
        GUGU_UTEST_SUBSECTION("Vector2i");
        {
            pugi::xml_document document;
            pugi::xml_node node = document.append_child("Data");
            xml::WriteVector2i(node, Vector2i(10, 20));

            GUGU_UTEST_CHECK(xml::SaveDocumentToString(document) == "<Data x=\"10\" y=\"20\"/>");
        }

        GUGU_UTEST_SUBSECTION("Vector2f");
        {
            pugi::xml_document document;
            pugi::xml_node node = document.append_child("Data");
            xml::WriteVector2f(node, Vector2f(10.f, 20.f));

            GUGU_UTEST_CHECK(xml::SaveDocumentToString(document) == "<Data x=\"10\" y=\"20\"/>");
        }

        GUGU_UTEST_SUBSECTION("UDim2");
        {
            pugi::xml_document document;
            pugi::xml_node node = document.append_child("Data");
            xml::WriteUDim2(node, UDim2(Vector2f(10, 20), Vector2f(100, 200)));

            GUGU_UTEST_CHECK(xml::SaveDocumentToString(document) == "<Data xRel=\"10\" yRel=\"20\" xAbs=\"100\" yAbs=\"200\"/>");
        }

        GUGU_UTEST_SUBSECTION("Rect");
        {
            pugi::xml_document document;
            pugi::xml_node node = document.append_child("Data");
            xml::WriteRect(node, sf::IntRect(Vector2i(10, 20), Vector2i(100, 200)));

            GUGU_UTEST_CHECK(xml::SaveDocumentToString(document) == "<Data x=\"10\" y=\"20\" w=\"100\" h=\"200\"/>");
        }

        GUGU_UTEST_SUBSECTION("Color");
        {
            pugi::xml_document document;
            pugi::xml_node node = document.append_child("Data");
            xml::WriteColor(node, sf::Color(10, 20, 30, 128));

            GUGU_UTEST_CHECK(xml::SaveDocumentToString(document) == "<Data r=\"10\" g=\"20\" b=\"30\" a=\"128\"/>");
        }
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}

}   // namespace tests
