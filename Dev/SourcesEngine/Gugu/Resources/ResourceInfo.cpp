////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/ResourceInfo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"
#include "Gugu/Utility/System.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ResourceInfo::ResourceInfo()
{
    resource = nullptr;
}

ResourceInfo::~ResourceInfo()
{
    SafeDelete(resource);
}

bool ResourceInfo::CompareID(const ResourceInfo* pLeft, const ResourceInfo* pRight)
{
    return pLeft->resourceID < pRight->resourceID;
}

}   // namespace gugu
