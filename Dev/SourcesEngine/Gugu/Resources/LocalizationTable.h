#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class LocalizationTable : public Resource
{
public:

    LocalizationTable();
    virtual ~LocalizationTable();

    virtual EResourceType::Type GetResourceType() const override;

protected:

    virtual void Unload() override;
    virtual bool LoadFromXml(const pugi::xml_document& document) override;
    virtual bool SaveToXml(pugi::xml_document& document) const override;

protected:

};

}   // namespace gugu
