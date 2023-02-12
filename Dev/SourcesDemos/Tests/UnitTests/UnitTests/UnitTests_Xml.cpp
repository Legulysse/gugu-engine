////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/UDim.h"
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
        GUGU_UTEST_SUBSECTION("Vector2");
        {
            pugi::xml_document document = ParseStringDocument("<Data x=\"10\" y=\"20\"/>");

            Vector2f result;
            if (GUGU_UTEST_CHECK(XmlReadVector2(document.child("Data"), result)))
            {
                GUGU_UTEST_CHECK(result == Vector2f(10.f, 20.f));
            }
        }

        GUGU_UTEST_SUBSECTION("UDim2");
        {
            pugi::xml_document document = ParseStringDocument("<Data xRel=\"10\" yRel=\"20\" xAbs=\"100\" yAbs=\"200\"/>");

            UDim2 result;
            if (GUGU_UTEST_CHECK(XmlReadUDim2(document.child("Data"), result)))
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
            if (GUGU_UTEST_CHECK(XmlReadRect(document.child("Data"), result)))
            {
                GUGU_UTEST_CHECK(result == sf::IntRect(10, 20, 100, 200));
            }
        }
    }

    GUGU_UTEST_SECTION("Write Attribute");
    {
        GUGU_UTEST_SUBSECTION("Vector2");
        {
            pugi::xml_document document;
            pugi::xml_node node = document.append_child("Data");

            if (GUGU_UTEST_CHECK(XmlWriteVector2(node, Vector2f(10.f, 20.f))))
            {
                GUGU_UTEST_CHECK(ConvertDocumentToString(document) == "<Data x=\"10\" y=\"20\"/>");
            }
        }

        GUGU_UTEST_SUBSECTION("UDim2");
        {
            pugi::xml_document document;
            pugi::xml_node node = document.append_child("Data");

            if (GUGU_UTEST_CHECK(XmlWriteUDim2(node, UDim2(10, 100, 20, 200))))
            {
                GUGU_UTEST_CHECK(ConvertDocumentToString(document) == "<Data xRel=\"10\" yRel=\"20\" xAbs=\"100\" yAbs=\"200\"/>");
            }
        }

        GUGU_UTEST_SUBSECTION("Rect");
        {
            pugi::xml_document document;
            pugi::xml_node node = document.append_child("Data");

            if (GUGU_UTEST_CHECK(XmlWriteRect(node, sf::IntRect(10, 20, 100, 200))))
            {
                GUGU_UTEST_CHECK(ConvertDocumentToString(document) == "<Data x=\"10\" y=\"20\" w=\"100\" h=\"200\"/>");
            }
        }
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}

}   // namespace tests
