////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/Datasheet.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ResourceInfo.h"
#include "Gugu/Data/DatasheetObject.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Debug/Logger.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

Datasheet::Datasheet()
    : m_rootObject(nullptr)
{
}

Datasheet::~Datasheet()
{
    Unload();
}

EResourceType::Type Datasheet::GetResourceType() const
{
    return EResourceType::Datasheet;
}

void Datasheet::Unload()
{
    SafeDelete(m_rootObject);
}

bool Datasheet::LoadFromFile()
{
    Unload();

    // TODO: handle extensions with several dots (like name.type.xml instead of name.type).
    m_rootObject = dynamic_cast<DatasheetObject*>(GetResources()->InstanciateDataObject(m_resourceInfos->fileInfo.GetExtension()));
    if (!m_rootObject)
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("Could not instantiate Datasheet Root Object : {0}", GetID()));
        return false;
    }

    std::vector<Datasheet*> ancestors;
    ancestors.push_back(this);
    return m_rootObject->LoadFromFile(GetFileInfo().GetFilePath(), this, ancestors);
}

const DatasheetObject* Datasheet::GetRootObject() const
{
    return m_rootObject;
}

}   // namespace gugu
