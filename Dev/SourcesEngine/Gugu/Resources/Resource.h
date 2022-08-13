#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/FileInfo.h"
#include "Gugu/Resources/EnumsResources.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class ResourceInfo;
}

namespace pugi
{
    class xml_document;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class Resource
{
public:

    Resource();
    virtual ~Resource();

    void Init(ResourceInfo* _pResourceInfos);

    const std::string& GetID() const;
    const FileInfo& GetFileInfo() const;

    virtual EResourceType::Type GetResourceType() const;

    virtual bool LoadFromFile();
    virtual bool LoadFromString(const std::string& source);

    virtual bool SaveToFile() const;
    virtual bool SaveToString(std::string& result) const;

protected:

    bool LoadFromXmlFile();
    bool LoadFromXmlString(const std::string& source);
    bool SaveToXmlFile() const;
    bool SaveToXmlString(std::string& result) const;

    virtual void Unload();
    virtual bool LoadFromXml(const pugi::xml_document& document);
    virtual bool SaveToXml(pugi::xml_document& document) const;

protected:

    ResourceInfo* m_resourceInfos;
};

}   // namespace gugu
