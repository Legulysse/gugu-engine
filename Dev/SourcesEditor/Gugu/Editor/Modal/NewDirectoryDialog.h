#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Modal/BaseModalDialog.h"
#include "Gugu/System/Callback.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class NewDirectoryDialog : public BaseModalDialog
{
public:

    using DelegateValidation = std::function<void(const std::string& validatedName)>;

public:

    NewDirectoryDialog(const std::string& path, const std::string& name, const DelegateValidation& callback);
    virtual ~NewDirectoryDialog();

protected:

    virtual void UpdateModalImpl(const DeltaTime& dt) override;

protected:

    std::string m_path;
    std::string m_name;
    DelegateValidation m_callback;
};

}   //namespace gugu
