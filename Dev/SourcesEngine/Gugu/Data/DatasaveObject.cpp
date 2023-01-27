////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Data/DatasaveObject.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Data/DataBindingUtility.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DatasaveObject::DatasaveObject()
{
}

DatasaveObject::~DatasaveObject()
{
}

bool DatasaveObject::LoadFromXml(const std::string& _strPathName)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(_strPathName.c_str());
    if (!result)
        return false;

    pugi::xml_node rootNode = document.child("Datasave");
    if (rootNode.empty())
        return false;

    DataParseContext context;
    context.currentNode = &rootNode;
    ParseMembers(context);

    return true;
}

bool DatasaveObject::SaveToXml(const std::string& _strPathName)
{
    pugi::xml_document document;

    pugi::xml_node rootNode = document.append_child("Datasave");
    rootNode.append_attribute("serializationVersion").set_value(1);
    rootNode.append_attribute("bindingVersion").set_value(1);

    DataSaveContext context;
    context.currentNode = &rootNode;
    SerializeMembers(context);

    return document.save_file(_strPathName.c_str(), PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_utf8);
}

}   // namespace gugu
