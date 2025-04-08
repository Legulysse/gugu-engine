#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/FileInfo.h"

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

    // TODO: seems unused, remove ?
    static bool CompareID(const ResourceInfo* pLeft, const ResourceInfo* pRight);

public:

    // TODO: proper accessors + ctor ?
    std::string resourceID;
    FileInfo fileInfo;
    Resource* resource;
};

}   // namespace gugu
