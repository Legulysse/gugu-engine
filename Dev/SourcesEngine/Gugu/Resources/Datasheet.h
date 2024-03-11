#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"
#include "Gugu/System/UUID.h"

#include <map>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DataObject;
    class DatasheetObject;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class Datasheet : public Resource
{
    friend class DatasheetObject;

public:

    Datasheet();
    virtual ~Datasheet();

    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;

    //TODO: Store reference to parent, at least a Getter
    const DatasheetObject* GetRootObject() const;

protected:

    virtual void Unload() override;

private:

    DatasheetObject* m_rootObject;
    std::map<UUID, DataObject*> m_instanceObjects;
};

}   // namespace gugu
