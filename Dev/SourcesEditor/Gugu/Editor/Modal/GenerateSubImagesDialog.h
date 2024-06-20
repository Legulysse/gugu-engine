#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Modal/BaseModalDialog.h"
#include "Gugu/System/Callback.h"
#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class GenerateSubImagesDialog : public BaseModalDialog
{
public:

    using DelegateGenerateFromCount = std::function<void(int columnCount, int rowCount)>;
    using DelegateGenerateFromSize = std::function<void(const Vector2i& itemSize, const Vector2i& itemOffset)>;

public:

    GenerateSubImagesDialog(const DelegateGenerateFromCount& delegateGenerateFromCount, const DelegateGenerateFromSize& delegateGenerateFromSize);
    virtual ~GenerateSubImagesDialog();

protected:

    virtual void UpdateModalImpl(const DeltaTime& dt) override;

protected:

    DelegateGenerateFromCount m_delegateGenerateFromCount;
    DelegateGenerateFromSize m_delegateGenerateFromSize;

    size_t m_generatorIndex;
    int m_columnCount;
    int m_rowCount;
    Vector2i m_itemSize;
    Vector2i m_itemOffset;
};

}   //namespace gugu
