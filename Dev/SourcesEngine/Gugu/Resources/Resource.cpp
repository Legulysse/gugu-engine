////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/Resource.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ResourceInfo.h"
#include "Gugu/System/SystemUtility.h"

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

/*void Resource::SetFileInfo(const FileInfo& _oFileInfo)
{
    m_resourceInfos->fileInfo = _oFileInfo;
}*/

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
    return false;
}

bool Resource::ReloadFromFile()
{
    return false;
}

bool Resource::SaveToFile()
{
    return false;
}

}   // namespace gugu
