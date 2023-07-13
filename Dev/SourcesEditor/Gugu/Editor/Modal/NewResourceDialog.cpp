////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Modal/NewResourceDialog.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"
#include "Gugu/Editor/Resources/VirtualDatasheet.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/AnimSet.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Resources/ParticleEffect.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

NewResourceDialog::NewResourceDialog(const std::string& resourcePath, EResourceType::Type resourceType)
    : BaseModalDialog("New Resource")
    , m_resourceType(resourceType)
    , m_resourcePath(resourcePath)
    , m_resourceName("")
    , m_resourceExtension("")
    , m_datasheetClassName("")
{
    //TODO: centralize resource type extensions somewhere.
    static const std::map<EResourceType::Type, std::string> resourceExtensions
    {
        { EResourceType::AnimSet, "animset.xml" },
        { EResourceType::ImageSet, "imageset.xml" },
        { EResourceType::ParticleEffect, "particle.xml" },
        { EResourceType::ElementWidget, "widget.xml" },
    };

    auto itExtension = resourceExtensions.find(m_resourceType);
    if (itExtension != resourceExtensions.end())
    {
        m_resourceExtension = itExtension->second;
    }
}

NewResourceDialog::~NewResourceDialog()
{
}

void NewResourceDialog::UpdateModalImpl(const DeltaTime& dt)
{
    if (m_resourceType == EResourceType::Datasheet)
    {
        std::string dummy = m_datasheetClassName;

        ImGuiComboFlags flags = 0;
        if (ImGui::BeginCombo("Datasheet Class", dummy.c_str(), flags))
        {
            const std::vector<DatasheetParser::ClassDefinition*>& classDefinitions = GetEditor()->GetDatasheetParser()->GetAllClassDefinitions();

            for (size_t i = 0; i < classDefinitions.size(); ++i)
            {
                if (classDefinitions[i]->isAbstract)
                    continue;

                bool selected = (dummy == classDefinitions[i]->m_name);
                if (ImGui::Selectable(classDefinitions[i]->m_name.c_str(), selected))
                {
                    m_datasheetClassName = classDefinitions[i]->m_name;
                    m_resourceExtension = m_datasheetClassName;
                }

                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }
    }

    ImGui::LabelText("Path", m_resourcePath.c_str());

    ImGui::Spacing();
    if (ImGui::InputText("Name", &m_resourceName, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        //TODO: Check ID.
    }

    ImGui::BeginDisabled();
    if (ImGui::InputText("Extension", &m_resourceExtension, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        //TODO: Check Extension.
    }
    ImGui::EndDisabled();

    ImGui::Spacing();
    if (ImGui::Button("Cancel"))
    {
        CloseModalImpl();
    }

    ImGui::SameLine();
    ImGui::BeginDisabled(m_resourceName.empty() || m_resourceExtension.empty());
    if (ImGui::Button("Validate"))
    {
        FileInfo fileInfo(m_resourcePath, m_resourceName + system::ExtensionSeparator + m_resourceExtension);

        //TODO: centralize resource type instanciation somewhere.
        Resource* newResource = nullptr;

        if (m_resourceType == EResourceType::ImageSet)
        {
            newResource = new ImageSet;
        }
        else if (m_resourceType == EResourceType::AnimSet)
        {
            newResource = new AnimSet;
        }
        else if (m_resourceType == EResourceType::ParticleEffect)
        {
            newResource = new ParticleEffect;
        }
        else if (m_resourceType == EResourceType::ElementWidget)
        {
            newResource = new ElementWidget;
        }
        else if (m_resourceType == EResourceType::Datasheet)
        {
            DatasheetParser::ClassDefinition* classDefinition;
            if (GetEditor()->GetDatasheetParser()->GetClassDefinition(m_datasheetClassName, classDefinition))
            {
                newResource = new VirtualDatasheet(classDefinition);
            }
        }

        if (newResource)
        {
            bool bSuccess = GetResources()->AddResource(newResource, fileInfo);
            if (bSuccess)
            {
                newResource->SaveToFile();

                GetEditor()->RefreshAssets();
                GetEditor()->OpenDocument(newResource->GetID());
            }
            else
            {
                SafeDelete(newResource);
            }
        }

        CloseModalImpl();
    }
    ImGui::EndDisabled();
}

}   //namespace gugu
