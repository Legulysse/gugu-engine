////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/Resource.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ResourceInfo.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Debug/Logger.h"
#include "Gugu/External/PugiXmlWrap.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
Resource::Resource()
{
    m_resourceInfos = nullptr;
}

Resource::~Resource()
{
}

void Resource::Init(ResourceInfo* _pResourceInfos)
{
    m_resourceInfos = _pResourceInfos;
}

std::string Resource::GetID() const
{
    return m_resourceInfos->resourceID;
}

FileInfo Resource::GetFileInfo() const
{
    return m_resourceInfos->fileInfo;
}

const FileInfo& Resource::GetFileInfoRef() const
{
    return m_resourceInfos->fileInfo;
}

EResourceType::Type Resource::GetResourceType() const
{
    return EResourceType::Unknown;
}

bool Resource::LoadFromFile()
{
    return LoadFromXmlFile();
}

bool Resource::LoadFromString(const std::string& source)
{
    return LoadFromXmlString(source);
}

bool Resource::SaveToFile() const
{
    return SaveToXmlFile();
}

bool Resource::SaveToString(std::string& result) const
{
    return SaveToXmlString(result);
}

bool Resource::LoadFromXmlFile()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(GetFileInfoRef().GetPathName().c_str());
    if (!result)
        return false;

    return LoadFromXml(doc);
}

bool Resource::LoadFromXmlString(const std::string& source)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(source.c_str());
    if (!result)
        return false;

    return LoadFromXml(doc);
}

bool Resource::SaveToXmlFile() const
{
    pugi::xml_document doc;

    if (!SaveToXml(doc))
        return false;

    return doc.save_file(GetFileInfoRef().GetPathName().c_str());
}

bool Resource::SaveToXmlString(std::string& result) const
{
    pugi::xml_document doc;

    if (!SaveToXml(doc))
        return false;

    xml_string_writer buffer(result);
    doc.save(buffer);
    return true;
}

bool Resource::ReloadFromFile()
{
    GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "ReloadFromFile is not supported for this type of resource");
    return false;
}

void Resource::Unload()
{
    GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "Unload is not supported for this type of resource");
}

bool Resource::LoadFromXml(const pugi::xml_document& document)
{
    GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "LoadFromXml is not supported for this type of resource");
    return false;
}

bool Resource::SaveToXml(pugi::xml_document& document) const
{
    GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "SaveToXml is not supported for this type of resource");
    return false;
}

}   // namespace gugu
