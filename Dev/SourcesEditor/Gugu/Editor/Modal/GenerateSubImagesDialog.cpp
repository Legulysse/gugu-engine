////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Modal/GenerateSubImagesDialog.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/External/ImGuiUtility.h"

#include <array>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

GenerateSubImagesDialog::GenerateSubImagesDialog(const DelegateGenerateFromCount& delegateGenerateFromCount, const DelegateGenerateFromSize& delegateGenerateFromSize)
    : BaseModalDialog("Generate SubImages")
    , m_delegateGenerateFromCount(delegateGenerateFromCount)
    , m_delegateGenerateFromSize(delegateGenerateFromSize)
    , m_generatorIndex(EGenerator::ItemCount)
    , m_columnCount(1)
    , m_rowCount(1)
    , m_itemSize(Vector2i(16, 16))
    , m_itemOffset(Vector2i(0, 0))
{
}

GenerateSubImagesDialog::~GenerateSubImagesDialog()
{
}

void GenerateSubImagesDialog::UpdateModalImpl(const DeltaTime& dt)
{
    static const std::vector<std::string> generators = { "Item Count", "Item Size" };
    size_t dummyIndex = static_cast<size_t>(m_generatorIndex);
    if (ImGui::Combo("Generator", generators, &dummyIndex))
    {
        m_generatorIndex = static_cast<EGenerator>(dummyIndex);
    }

    if (m_generatorIndex == EGenerator::ItemCount)
    {
        ImGui::InputInt("Items per Row", &m_columnCount);
        ImGui::InputInt("Row Count", &m_rowCount);
    }
    else if (m_generatorIndex == EGenerator::ItemSize)
    {
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
        if (m_generatorIndex == EGenerator::ItemCount)
        {
            m_delegateGenerateFromCount(m_columnCount, m_rowCount);
        }
        else if (m_generatorIndex == EGenerator::ItemSize)
        {
            m_delegateGenerateFromSize(m_itemSize, m_itemOffset);
        }

        CloseModalImpl();
    }
}

}   //namespace gugu
