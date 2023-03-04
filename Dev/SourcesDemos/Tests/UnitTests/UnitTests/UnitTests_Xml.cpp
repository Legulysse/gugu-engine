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

pugi::xml_document ParseStringDocument(const std::string& xmlSource)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_string(xmlSource.c_str());
    return document;
}

std::string ConvertDocumentToString(const pugi::xml_document& document)
{
    std::string result;
    xml_string_writer buffer(result);
    document.save(buffer, "", pugi::format_no_declaration | pugi::format_raw, pugi::encoding_utf8);
    return result;
}

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

        std::string result;
        xml_string_writer buffer(result);
        document.save(buffer, "", pugi::format_raw, pugi::encoding_utf8);

        GUGU_UTEST_CHECK(result == "<?xml version=\"1.0\"?><Root version=\"10\"><Data name=\"age\" value=\"30\"/></Root>");
    }

    GUGU_UTEST_SECTION("Read Attribute");
    {
        GUGU_UTEST_SUBSECTION("Vector2i");
        {
            pugi::xml_document document = ParseStringDocument("<Data x=\"10\" y=\"20\"/>");
            pugi::xml_node nodeData = document.child("Data");
            pugi::xml_node nodeNoData = document.child("NoData");

            const Vector2i defaultData(30, 40);
            const Vector2i defaultNoData(30, 40);

            // Read (no default)
            GUGU_UTEST_CHECK(XmlReadVector2i(nodeData) == Vector2i(10, 20));
            GUGU_UTEST_CHECK(XmlReadVector2i(nodeNoData) == Vector2i(0, 0));

            // Read (default)
            GUGU_UTEST_CHECK(XmlReadVector2i(nodeData, defaultData) == Vector2i(10, 20));
            GUGU_UTEST_CHECK(XmlReadVector2i(nodeNoData, defaultNoData) == Vector2i(30, 40));

            // Parse (no default)
            Vector2i resultDataSelf(10, 10);
            XmlParseVector2i(nodeData, resultDataSelf);
            GUGU_UTEST_CHECK(resultDataSelf == Vector2i(10, 20));

            Vector2i resultNoDataSelf(10, 10);
            XmlParseVector2i(nodeNoData, resultNoDataSelf);
            GUGU_UTEST_CHECK(resultNoDataSelf == Vector2i(0, 0));

            // Parse (default)
            Vector2i resultDataRefWithDefault(10, 10);
            XmlParseVector2i(nodeData, resultDataRefWithDefault, defaultData);
            GUGU_UTEST_CHECK(resultDataRefWithDefault == Vector2i(10, 20));

            Vector2i resultNoDataRefWithDefault(10, 10);
            XmlParseVector2i(nodeNoData, resultNoDataRefWithDefault, defaultNoData);
            GUGU_UTEST_CHECK(resultNoDataRefWithDefault == Vector2i(30, 40));

            // TryParse
            Vector2i resultData(10, 10);
            if (GUGU_UTEST_CHECK(XmlTryParseVector2i(nodeData, resultData)))
            {
                GUGU_UTEST_CHECK(resultData == Vector2i(10, 20));
            }

            Vector2i resultNoData(10, 10);
            GUGU_UTEST_CHECK(!XmlTryParseVector2i(nodeNoData, resultNoData));
            GUGU_UTEST_CHECK(resultNoData == Vector2i(10, 10));
        }

        GUGU_UTEST_SUBSECTION("Vector2f");
        {
            pugi::xml_document document = ParseStringDocument("<Data x=\"10\" y=\"20\"/><IllFormedData x=\"10\"/>");
            pugi::xml_node nodeData = document.child("Data");
            pugi::xml_node nodeNoData = document.child("NoData");
            pugi::xml_node nodeIllFormedData = document.child("IllFormedData");

            const Vector2f defaultData(30.f, 40.f);

            // Read (no default)
            GUGU_UTEST_CHECK(ApproxEqual(XmlReadVector2f(nodeData), Vector2f(10.f, 20.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(XmlReadVector2f(nodeNoData), Vector2f(0.f, 0.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(XmlReadVector2f(nodeIllFormedData), Vector2f(10.f, 0.f), math::Epsilon3));

            // Read (default)
            GUGU_UTEST_CHECK(ApproxEqual(XmlReadVector2f(nodeData, defaultData), Vector2f(10.f, 20.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(XmlReadVector2f(nodeNoData, defaultData), Vector2f(30.f, 40.f), math::Epsilon3));
            GUGU_UTEST_CHECK(ApproxEqual(XmlReadVector2f(nodeIllFormedData, defaultData), Vector2f(10.f, 40.f), math::Epsilon3));

            // Parse (no default)
            Vector2f resultParseData(10.f, 10.f);
            XmlParseVector2f(nodeData, resultParseData);
            GUGU_UTEST_CHECK(ApproxEqual(resultParseData, Vector2f(10.f, 20.f), math::Epsilon3));

            Vector2f resultParseNoData(10.f, 10.f);
            XmlParseVector2f(nodeNoData, resultParseNoData);
            GUGU_UTEST_CHECK(ApproxEqual(resultParseNoData, Vector2f(0.f, 0.f), math::Epsilon3));

            // Parse (default)
            Vector2f resultParseDataWithDefault(10.f, 10.f);
            XmlParseVector2f(nodeData, resultParseDataWithDefault, defaultData);
            GUGU_UTEST_CHECK(ApproxEqual(resultParseDataWithDefault, Vector2f(10.f, 20.f), math::Epsilon3));

            Vector2f resultParseNoDataWithDefault(10.f, 10.f);
            XmlParseVector2f(nodeNoData, resultParseNoDataWithDefault, defaultData);
            GUGU_UTEST_CHECK(ApproxEqual(resultParseNoDataWithDefault, Vector2f(30.f, 40.f), math::Epsilon3));

            // TryParse
            Vector2f resultTryParseData(10.f, 10.f);
            if (GUGU_UTEST_CHECK(XmlTryParseVector2f(nodeData, resultTryParseData)))
            {
                GUGU_UTEST_CHECK(ApproxEqual(resultTryParseData, Vector2f(10.f, 20.f), math::Epsilon3));
            }

            Vector2f resultTryParseNoData(10.f, 10.f);
            GUGU_UTEST_CHECK(!XmlTryParseVector2f(nodeNoData, resultTryParseNoData));
            GUGU_UTEST_CHECK(ApproxEqual(resultTryParseNoData, Vector2f(10.f, 10.f), math::Epsilon3));
        }

        GUGU_UTEST_SUBSECTION("UDim2");
        {
            pugi::xml_document document = ParseStringDocument("<Data xRel=\"10\" yRel=\"20\" xAbs=\"100\" yAbs=\"200\"/>");

            UDim2 result;
            if (GUGU_UTEST_CHECK(XmlTryParseUDim2(document.child("Data"), result)))
            {
                GUGU_UTEST_CHECK(result.x.relative == 10
                                && result.y.relative == 20
                                && result.x.absolute == 100
                                && result.y.absolute == 200);
            }
        }

        GUGU_UTEST_SUBSECTION("Rect");
        {
            pugi::xml_document document = ParseStringDocument("<Data x=\"10\" y=\"20\" w=\"100\" h=\"200\"/>");

            sf::IntRect result;
            if (GUGU_UTEST_CHECK(XmlTryParseRect(document.child("Data"), result)))
            {
                GUGU_UTEST_CHECK(result == sf::IntRect(10, 20, 100, 200));
            }
        }
    }

    GUGU_UTEST_SECTION("Write Attribute");
    {
        GUGU_UTEST_SUBSECTION("Vector2i");
        {
            pugi::xml_document document;
            pugi::xml_node node = document.append_child("Data");
            XmlWriteVector2i(node, Vector2i(10, 20));

            GUGU_UTEST_CHECK(ConvertDocumentToString(document) == "<Data x=\"10\" y=\"20\"/>");
        }

        GUGU_UTEST_SUBSECTION("Vector2f");
        {
            pugi::xml_document document;
            pugi::xml_node node = document.append_child("Data");
            XmlWriteVector2f(node, Vector2f(10.f, 20.f));

            GUGU_UTEST_CHECK(ConvertDocumentToString(document) == "<Data x=\"10\" y=\"20\"/>");
        }

        GUGU_UTEST_SUBSECTION("UDim2");
        {
            pugi::xml_document document;
            pugi::xml_node node = document.append_child("Data");
            XmlWriteUDim2(node, UDim2(10, 100, 20, 200));

            GUGU_UTEST_CHECK(ConvertDocumentToString(document) == "<Data xRel=\"10\" yRel=\"20\" xAbs=\"100\" yAbs=\"200\"/>");
        }

        GUGU_UTEST_SUBSECTION("Rect");
        {
            pugi::xml_document document;
            pugi::xml_node node = document.append_child("Data");
            XmlWriteRect(node, sf::IntRect(10, 20, 100, 200));

            GUGU_UTEST_CHECK(ConvertDocumentToString(document) == "<Data x=\"10\" y=\"20\" w=\"100\" h=\"200\"/>");
        }
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}

}   // namespace tests
