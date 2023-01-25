////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Data/DatasheetObject.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Data/DataBindingUtility.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Datasheet.h"
#include "Gugu/External/PugiXmlUtility.h"
#include "Gugu/Debug/Logger.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DatasheetObject::DatasheetObject()
    : m_datasheet(nullptr)
{
}

DatasheetObject::~DatasheetObject()
{
}

bool DatasheetObject::LoadFromXml(const std::string& _strPathName, Datasheet* ownerDatasheet, std::vector<Datasheet*>& ancestors)
{
    if (ownerDatasheet != nullptr)
    {
        m_datasheet = ownerDatasheet;
    }

    pugi::xml_document oDoc;
    pugi::xml_parse_result result = oDoc.load_file(_strPathName.c_str());
    if (!result)
        return false;

    pugi::xml_node oNodeRoot = oDoc.child("Datasheet");
    if (oNodeRoot.empty())
        return false;

    pugi::xml_attribute pAttributeParent = oNodeRoot.attribute("parent");
    if (pAttributeParent)
    {
        //TODO: check same type
        //TODO: error message if invalid base
        Datasheet* parentSheet = GetResources()->GetDatasheet(pAttributeParent.as_string());

        if (parentSheet)
        {
            if (StdVectorContains(ancestors, parentSheet))
            {
                std::string ancestorsLog;
                for (Datasheet* ancestor : ancestors)
                {
                    ancestorsLog += ancestor->GetFileInfo().GetFileName() + ", ";
                }

                ancestorsLog += parentSheet->GetFileInfo().GetFileName();
                GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("Datasheet ancestors create an infinite loop : {0}", ancestorsLog));
            }
            else
            {
                ancestors.push_back(parentSheet);
                LoadFromXml(parentSheet->GetFileInfo().GetFilePath(), nullptr, ancestors);
            }
        }
    }

    DatasheetParserContext kContext;
    kContext.currentNode = &oNodeRoot;
    ParseMembers(kContext);

    return true;
}

Datasheet* DatasheetObject::GetDatasheet() const
{
    return m_datasheet;
}

}   // namespace gugu
