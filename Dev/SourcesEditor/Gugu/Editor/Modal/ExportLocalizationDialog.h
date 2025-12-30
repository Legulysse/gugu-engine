#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Modal/BaseModalDialog.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ExportLocalizationDialog : public BaseModalDialog
{
public:

    ExportLocalizationDialog();
    virtual ~ExportLocalizationDialog();

protected:

    virtual void UpdateModalImpl(const DeltaTime& dt) override;

    void ExportLocalization();

protected:

    std::string m_exportDirectoryPath;
    std::string m_exportFileName;
    std::string m_targetTable;
};

}   //namespace gugu
