#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Modal/BaseModalDialog.h"
#include "Gugu/Resources/EnumsResources.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class NewResourceDialog : public BaseModalDialog
{
public:

    NewResourceDialog(const std::string& resourcePath, EResourceType::Type resourceType);
    virtual ~NewResourceDialog();

protected:

    virtual void UpdateModalImpl(const DeltaTime& dt) override;

protected:

    EResourceType::Type m_resourceType;
    std::string m_resourcePath;
    std::string m_resourceName;
    std::string m_resourceExtension;
    std::string m_datasheetClassName;   // TODO: use pointer instead (need to move the definition outside of DatasheetParser class).
};

}   //namespace gugu
