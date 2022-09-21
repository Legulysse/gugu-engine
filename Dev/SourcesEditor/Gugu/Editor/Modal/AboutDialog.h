#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Modal/BaseModalDialog.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class AboutDialog : public BaseModalDialog
{
public:

    AboutDialog();
    virtual ~AboutDialog();

protected:

    virtual void UpdateModalImpl(const DeltaTime& dt) override;
};

}   //namespace gugu
