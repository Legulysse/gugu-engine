////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Modal/GenerateAnimationFramesDialog.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ImageSet.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

GenerateAnimationFramesDialog::GenerateAnimationFramesDialog(ImageSet* imageSet, const DelegateGenerateFromDirectory& delegateGenerateFromDirectory, const DelegateGenerateFromImageSet& delegateGenerateFromImageSet)
    : BaseModalDialog("Generate Animation Frames")
    , m_imageSet(imageSet)
    , m_delegateGenerateFromDirectory(delegateGenerateFromDirectory)
    , m_delegateGenerateFromImageSet(delegateGenerateFromImageSet)
    , m_generatorIndex(EGenerator::ImageSet)
    , m_from(0)
    , m_to(0)
{
}

GenerateAnimationFramesDialog::~GenerateAnimationFramesDialog()
{
}

void GenerateAnimationFramesDialog::UpdateModalImpl(const DeltaTime& dt)
{
    static const std::vector<std::string> generators = { "ImageSet", "Directory" };
    size_t dummyIndex = static_cast<size_t>(m_generatorIndex);
    if (ImGui::Combo("Generator", generators, &dummyIndex))
    {
        m_generatorIndex = static_cast<EGenerator>(dummyIndex);
    }

    if (m_generatorIndex == EGenerator::Directory)
    {
        ImGui::InputText("Directory", &m_directoryPath);
    }
    else if (m_generatorIndex == EGenerator::ImageSet)
    {
        if (!m_imageSet)
        {
            ImGui::Text("Please provide an ImageSet on this AnimSet to use this generator.");
        }
        else if (m_imageSet->GetSubImageCount() == 0)
        {
            ImGui::Text("Please provide at least 1 SubImage in the provided ImageSet to use this generator.");
        }
        else
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
        if (m_generatorIndex == EGenerator::Directory)
        {
            m_delegateGenerateFromDirectory(m_directoryPath);
        }
        else if (m_generatorIndex == EGenerator::ImageSet)
        {
            if (m_imageSet)
            {
                m_delegateGenerateFromImageSet(m_from, m_to);
            }
        }

        CloseModalImpl();
    }
}

}   //namespace gugu
