#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/FileInfo.h"
#include "Gugu/Resources/EnumsResources.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class ResourceInfo;
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

    std::string	GetID() const;

    //void			SetFileInfo(const FileInfo& _oFileInfo);	//TODO: Remove ? needed by Editor ?
	FileInfo		GetFileInfo() const;
	const FileInfo& GetFileInfoRef() const;

	virtual EResourceType::Type	GetResourceType() const;

    virtual bool        LoadFromFile    ();
    virtual bool        ReloadFromFile  ();
    virtual bool        SaveToFile      ();

protected:

    ResourceInfo* m_resourceInfos;
};

}	// namespace gugu
