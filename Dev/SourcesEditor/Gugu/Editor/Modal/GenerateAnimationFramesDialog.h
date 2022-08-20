#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Modal/BaseModalDialog.h"
#include "Gugu/Core/Callback.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class ImageSet;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class GenerateAnimationFramesDialog : public BaseModalDialog
{
public:

    using DelegateGenerateFromDirectory = std::function<void(const std::string& path)>;
    using DelegateGenerateFromImageSet = std::function<void(size_t from, size_t to)>;

public:

    GenerateAnimationFramesDialog(ImageSet* imageSet, const DelegateGenerateFromDirectory& delegateGenerateFromDirectory, const DelegateGenerateFromImageSet& delegateGenerateFromImageSet);
    virtual ~GenerateAnimationFramesDialog();

protected:

    virtual void UpdateModalImpl(const DeltaTime& dt) override;

protected:

    ImageSet* m_imageSet;
    DelegateGenerateFromDirectory m_delegateGenerateFromDirectory;
    DelegateGenerateFromImageSet m_delegateGenerateFromImageSet;

    size_t m_generatorIndex;
    std::string m_directoryPath;
    size_t m_from;
    size_t m_to;
};

}   //namespace gugu
