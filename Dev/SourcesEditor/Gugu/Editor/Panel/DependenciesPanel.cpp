////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/DependenciesPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"
#include "Gugu/Editor/Panel/Document/DocumentPanel.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Resource.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DependenciesPanel::DependenciesPanel()
{
    m_title = "Dependencies";
}

DependenciesPanel::~DependenciesPanel()
{
}

void DependenciesPanel::UpdatePanel(const DeltaTime& dt)
{
    if (ImGui::Begin(m_title.c_str(), false))
    {
        const std::map<const Resource*, ManagerResources::ResourceDependencies>& resourceDependencies = GetResources()->GetResourceDependencies();
        
        DocumentPanel* lastActiveDocument = GetEditor()->GetLastActiveDocument();
        if (lastActiveDocument)
        {
            auto it = resourceDependencies.find(lastActiveDocument->GetResource());
            if (it != resourceDependencies.end())
            {
                if (ImGui::TreeNodeEx(StringFormat("Active Document ({0}):###_ACTIVE_DOCUMENT", it->first->GetID()).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                {
                    DisplayResourceDependencies(it->second);

                    ImGui::TreePop();
                }

                ImGui::Spacing();
            }
        }

        if (ImGui::TreeNodeEx("Loaded Resources:", ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (const auto& kvp : resourceDependencies)
            {
                if (ImGui::TreeNodeEx(kvp.first->GetID().c_str(), ImGuiTreeNodeFlags_None))
                {
                    DisplayResourceDependencies(kvp.second);

                    ImGui::TreePop();
                }
            }

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void DependenciesPanel::DisplayResourceDependencies(const ManagerResources::ResourceDependencies& dependencies) const
{
    ImGui::Indent();

    ImGui::Text(StringFormat("Dependencies: {0}", dependencies.dependencies.size()));

    ImGui::Indent();
    for (const auto& dependency : dependencies.dependencies)
    {
        ImGui::Text(StringFormat("{0}", dependency->GetID()));
    }
    ImGui::Unindent();

    ImGui::Text(StringFormat("Referencers: {0}", dependencies.referencers.size()));

    ImGui::Indent();
    for (const auto& referencer : dependencies.referencers)
    {
        ImGui::Text(StringFormat("{0}", referencer->GetID()));
    }
    ImGui::Unindent();

    ImGui::Text(StringFormat("Listeners: {0}", dependencies.listeners.size()));

    ImGui::Unindent();
}

}   //namespace gugu
