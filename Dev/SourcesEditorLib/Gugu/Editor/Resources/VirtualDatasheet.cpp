////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Resources/VirtualDatasheet.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/PugiXmlWrap.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

VirtualDatasheetObject::VirtualDatasheetObject()
    : classDefinition(nullptr)
{
}

VirtualDatasheetObject::~VirtualDatasheetObject()
{
    classDefinition = nullptr;
    ClearStdVector(dataValues);
}

bool VirtualDatasheetObject::LoadFromXml(const pugi::xml_node& nodeDatasheetObject, DatasheetParser::ClassDefinition* definition)
{
    classDefinition = definition;

    for (pugi::xml_node nodeData = nodeDatasheetObject.child("Data"); nodeData; nodeData = nodeData.next_sibling("Data"))
    {
        VirtualDatasheetObject::DataValue* dataValue = new VirtualDatasheetObject::DataValue;
        dataValue->name = nodeData.attribute("name").value();
        dataValue->value = nodeData.attribute("value").value();

        dataValues.push_back(dataValue);
    }

    return true;
}

VirtualDatasheetObject::DataValue::~DataValue()
{
    ClearStdVector(children);
}

VirtualDatasheet::VirtualDatasheet()
    : rootObject(nullptr)
{
}

VirtualDatasheet::~VirtualDatasheet()
{
    SafeDelete(rootObject);
}

EResourceType::Type VirtualDatasheet::GetResourceType() const
{
    return EResourceType::Datasheet;    // TODO: Should I use Custom ? Or a dedicated VirtualDatasheet enum value ?
}

bool VirtualDatasheet::LoadFromFile()
{
    pugi::xml_document xmlDocument;
    pugi::xml_parse_result result = xmlDocument.load_file(GetFileInfoRef().GetPathName().c_str());
    if (!result)
        return false;

    pugi::xml_node nodeDatasheetObject = xmlDocument.child("Datasheet");
    if (!nodeDatasheetObject)
        return false;

    rootObject = new VirtualDatasheetObject;

    pugi::xml_attribute attributeParent = nodeDatasheetObject.attribute("parent");
    if (attributeParent)
    {
        // TODO: read parent data.
    }

    return rootObject->LoadFromXml(nodeDatasheetObject, classDefinition);
}

}   // namespace gugu
