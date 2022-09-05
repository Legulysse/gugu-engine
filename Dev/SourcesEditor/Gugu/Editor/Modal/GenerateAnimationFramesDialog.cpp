////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Modal/GenerateAnimationFramesDialog.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ImageSet.h"
#include "Gugu/External/ImGuiWrapper.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

GenerateAnimationFramesDialog::GenerateAnimationFramesDialog(ImageSet* imageSet, const DelegateGenerateFromDirectory& delegateGenerateFromDirectory, const DelegateGenerateFromImageSet& delegateGenerateFromImageSet)
    : BaseModalDialog("Generate Animation Frames")
    , m_imageSet(imageSet)
    , m_delegateGenerateFromDirectory(delegateGenerateFromDirectory)
    , m_delegateGenerateFromImageSet(delegateGenerateFromImageSet)
    , m_generatorIndex(0)
    , m_from(0)
    , m_to(0)
{
}

GenerateAnimationFramesDialog::~GenerateAnimationFramesDialog()
{
}

void GenerateAnimationFramesDialog::UpdateModalImpl(const DeltaTime& dt)
{
    static const std::vector<std::string> generators = { "Directory", "ImageSet" };
    ImGui::Combo("Generator", generators, &m_generatorIndex);

    if (m_generatorIndex == 0)
    {
        // Generator : directory.
        ImGui::InputText("Directory", &m_directoryPath);
    }
    else
    {
        // Generator : ImageSet.
        if (m_imageSet)
        {
            const std::vector<SubImage*>& subImages = m_imageSet->GetSubImages();

            if (ImGui::BeginCombo("From", subImages[m_from]->GetName().c_str()))
            {
                for (size_t i = 0; i < subImages.size(); ++i)
                {
                    bool selected = (m_from == i);
                    if (ImGui::Selectable(subImages[i]->GetName().c_str(), selected))
                    {
                        m_from = i;
                    }

                    if (selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }

            if (ImGui::BeginCombo("To", subImages[m_to]->GetName().c_str()))
            {
                for (size_t i = 0; i < subImages.size(); ++i)
                {
                    bool selected = (m_to == i);
                    if (ImGui::Selectable(subImages[i]->GetName().c_str(), selected))
                    {
                        m_to = i;
                    }

                    if (selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }
        }
    }

    ImGui::Spacing();
    if (ImGui::Button("Cancel"))
    {
        CloseModalImpl();
    }

    ImGui::SameLine();
    if (ImGui::Button("Validate"))
    {
        if (m_generatorIndex == 0)
        {
            m_delegateGenerateFromDirectory(m_directoryPath);
        }
        else
        {
            m_delegateGenerateFromImageSet(m_from, m_to);
        }

        CloseModalImpl();
    }
}

}   //namespace gugu
