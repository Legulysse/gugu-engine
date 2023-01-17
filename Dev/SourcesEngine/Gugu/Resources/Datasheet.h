#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DatasheetObject;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class Datasheet : public Resource
{
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
};

}   // namespace gugu
