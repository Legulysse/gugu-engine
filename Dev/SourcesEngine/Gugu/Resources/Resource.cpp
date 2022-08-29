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

const std::string& Resource::GetID() const
{
    return m_resourceInfos->resourceID;
}

const FileInfo& Resource::GetFileInfo() const
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
    pugi::xml_parse_result result = doc.load_file(GetFileInfo().GetPathName().c_str());
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

    // I could use spaces instead of tabs, but it's probably a waste in the context of stored data ? It could be an engine option.
    return doc.save_file(GetFileInfo().GetPathName().c_str(), PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_utf8);
}

bool Resource::SaveToXmlString(std::string& result) const
{
    pugi::xml_document doc;

    if (!SaveToXml(doc))
        return false;

    xml_string_writer buffer(result);
    doc.save(buffer, "", pugi::format_no_declaration | pugi::format_raw, pugi::encoding_utf8);
    return true;
}

void Resource::Unload()
{
    // TODO: If I want to activate those error messages, I need to ensure they wont spam the editor for resources like Textures.
    //GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "Unload is not supported for this type of resource");
}

bool Resource::LoadFromXml(const pugi::xml_document& document)
{
    // TODO: If I want to activate those error messages, I need to ensure they wont spam the editor for resources like Textures.
    //GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "LoadFromXml is not supported for this type of resource");
    return false;
}

bool Resource::SaveToXml(pugi::xml_document& document) const
{
    // TODO: If I want to activate those error messages, I need to ensure they wont spam the editor for resources like Textures.
    //GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "SaveToXml is not supported for this type of resource");
    return false;
}

void Resource::GetDependencies(std::vector<Resource*>& dependencies) const
{
    // No dependencies by default.
}

void Resource::OnDependencyRemoved(const Resource* removedDependency)
{
    // No dependencies by default.
}

}   // namespace gugu
