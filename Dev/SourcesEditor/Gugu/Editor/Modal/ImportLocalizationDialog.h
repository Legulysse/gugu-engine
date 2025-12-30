#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Modal/BaseModalDialog.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ImportLocalizationDialog : public BaseModalDialog
{
public:

    ImportLocalizationDialog();
    virtual ~ImportLocalizationDialog();

protected:

    virtual void UpdateModalImpl(const DeltaTime& dt) override;

    void ImportLocalization();

protected:

    std::string m_exportDirectoryPath;
    std::string m_exportFileName;
    std::string m_targetTable;
};

}   //namespace gugu
