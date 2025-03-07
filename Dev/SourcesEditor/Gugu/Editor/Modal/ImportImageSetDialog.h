#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Modal/BaseModalDialog.h"
#include "Gugu/System/Callback.h"
#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ImportImageSetDialog : public BaseModalDialog
{
public:

    ImportImageSetDialog();
    virtual ~ImportImageSetDialog();

protected:

    virtual void UpdateModalImpl(const DeltaTime& dt) override;

protected:

    std::string m_sourceDirectory;
    std::string m_targetDirectory;
    std::string m_targetTextureName;
    float m_resizeScale;
    size_t m_filterIndex;
};

}   //namespace gugu
