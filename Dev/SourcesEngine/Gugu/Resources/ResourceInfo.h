#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/FileInfo.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Resource;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ResourceInfo
{
public:

    ResourceInfo();
    ~ResourceInfo();

    static bool CompareID(const ResourceInfo* pLeft, const ResourceInfo* pRight);

public:

    std::string resourceID;
    FileInfo fileInfo;
    Resource* resource;
};

}   // namespace gugu
