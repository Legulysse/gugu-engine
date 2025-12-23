////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/LocalizationTable.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

LocalizationTable::LocalizationTable()
{
}

LocalizationTable::~LocalizationTable()
{
    Unload();
}

EResourceType::Type LocalizationTable::GetResourceType() const
{
    return EResourceType::LocalizationTable;
}

void LocalizationTable::Unload()
{
}

bool LocalizationTable::LoadFromXml(const pugi::xml_document& document)
{
    Unload();

    pugi::xml_node rootNode = document.child("LocalizationTable");
    if (!rootNode)
        return false;

    return true;
}

bool LocalizationTable::SaveToXml(pugi::xml_document& document) const
{
    pugi::xml_node rootNode = document.append_child("LocalizationTable");
    rootNode.append_attribute("serializationVersion") = 1;

    return true;
}

}   // namespace gugu
