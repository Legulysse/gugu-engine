////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Modal/NewResourceDialog.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/AnimSet.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Resources/ParticleEffect.h"
#include "Gugu/System/SystemUtility.h"

#include <imgui.h>
#include <imgui_stdlib.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

NewResourceDialog::NewResourceDialog(const std::string& resourcePath, EResourceType::Type resourceType)
    : BaseModalDialog("New Resource")
    , m_resourceType(resourceType)
    , m_resourcePath(resourcePath)
    , m_resourceName("")
    , m_resourceExtension("")
{
    //TODO: centralize resource type extensions somewhere.
    static const std::map<EResourceType::Type, std::string> resourceExtensions
    {
        { EResourceType::AnimSet, "animset.xml" },
        { EResourceType::ImageSet, "imageset.xml" },
        { EResourceType::ParticleEffect, "particle.xml" },
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
    ImGui::LabelText("Path", m_resourcePath.c_str());

    ImGui::Spacing();
    if (ImGui::InputText("Name", &m_resourceName, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        //TODO: Check ID.
    }

    if (ImGui::InputText("Extension", &m_resourceExtension, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        //TODO: Check Extension.
    }

    ImGui::Spacing();
    if (ImGui::Button("Cancel"))
    {
        CloseModalImpl();
    }

    ImGui::SameLine();
    if (ImGui::Button("Validate"))
    {
        gugu::FileInfo fileInfo(m_resourcePath, m_resourceName + "." + m_resourceExtension);

        //TODO: centralize resource type instanciation somewhere.
        gugu::Resource* newResource = nullptr;

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

        if (newResource)
        {
            bool bSuccess = gugu::GetResources()->AddResource(newResource, fileInfo);
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
}

}   //namespace gugu
