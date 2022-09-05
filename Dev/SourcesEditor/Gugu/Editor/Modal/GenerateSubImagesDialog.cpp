////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Modal/GenerateSubImagesDialog.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/External/ImGuiWrapper.h"

#include <array>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

GenerateSubImagesDialog::GenerateSubImagesDialog(const DelegateGenerateFromCount& delegateGenerateFromCount, const DelegateGenerateFromSize& delegateGenerateFromSize)
    : BaseModalDialog("Generate SubImages")
    , m_delegateGenerateFromCount(delegateGenerateFromCount)
    , m_delegateGenerateFromSize(delegateGenerateFromSize)
    , m_generatorIndex(0)
    , m_columnCount(0)
    , m_rowCount(0)
{
}

GenerateSubImagesDialog::~GenerateSubImagesDialog()
{
}

void GenerateSubImagesDialog::UpdateModalImpl(const DeltaTime& dt)
{
    static const std::vector<std::string> generators = { "Item Count", "Item Size" };
    ImGui::Combo("Generator", generators, &m_generatorIndex);

    if (m_generatorIndex == 0)
    {
        // Generator : item count.
        ImGui::InputInt("Items per Row", &m_columnCount);
        ImGui::InputInt("Row Count", &m_rowCount);
    }
    else
    {
        // Generator : item size.
        ImGui::InputInt2("Item Size", &m_itemSize);
        ImGui::InputInt2("Offset", &m_itemOffset);
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
            m_delegateGenerateFromCount(m_columnCount, m_rowCount);
        }
        else
        {
            m_delegateGenerateFromSize(m_itemSize, m_itemOffset);
        }

        CloseModalImpl();
    }
}

}   //namespace gugu
