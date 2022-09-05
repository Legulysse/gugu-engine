#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Modal/BaseModalDialog.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class OpenProjectDialog : public BaseModalDialog
{
public:

    OpenProjectDialog(const std::string& projectPath);
    virtual ~OpenProjectDialog();

protected:

    virtual void UpdateModalImpl(const DeltaTime& dt) override;

protected:

    std::string m_projectPath;
};

}   //namespace gugu
