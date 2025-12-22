#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Modal/BaseModalDialog.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class FileInfo;
}

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

    std::string m_targetDirectory;
};

}   //namespace gugu
