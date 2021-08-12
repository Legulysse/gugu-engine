////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/DatasheetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/EditorCore.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"
#include "Gugu/Editor/Resources/VirtualDatasheet.h"

#include "Gugu/Resources/ManagerResources.h"

#include <imgui.h>
#include <imgui_stdlib.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DatasheetPanel::DatasheetPanel(VirtualDatasheet* datasheet)
{
    m_resourceID = datasheet->GetID();
    m_datasheet = datasheet;

    m_title = m_resourceID;
}

DatasheetPanel::~DatasheetPanel()
{
}

void DatasheetPanel::UpdatePanel(const DeltaTime& dt)
{
    m_focused = false;

    ImGuiWindowFlags flags = 0;
    if (m_dirty)
    {
        flags |= ImGuiWindowFlags_UnsavedDocument;
    }

    if (ImGui::Begin(m_title.c_str(), false, flags))
    {
        if (ImGui::IsWindowFocused())
        {
            m_focused = true;
        }

        // TODO: I should reverse-parse the bases to go top-bottom in the hierarchy.
        DatasheetParser::ClassDefinition* baseDefinition = m_datasheet->m_classDefinition->baseDefinition;
        while (baseDefinition != nullptr)
        {
            ImGui::Text("---- base: %s ----", baseDefinition->m_name.c_str());

            for (DatasheetParser::DataMemberDefinition* dataDefinition : baseDefinition->dataMembers)
            {
                ImGui::Text(dataDefinition->name.c_str());
                //ImGui::Text(dataDefinition->type.c_str());
                //ImGui::Text(dataDefinition->defaultValue.c_str());
            }

            baseDefinition = baseDefinition->baseDefinition;
        }

        ImGui::Text("---- class: %s ----", m_datasheet->m_rootObject->m_classDefinition->m_name.c_str());

        for (DatasheetParser::DataMemberDefinition* dataDefinition : m_datasheet->m_rootObject->m_classDefinition->dataMembers)
        {
            ImGui::Text(dataDefinition->name.c_str());
            //ImGui::Text(dataDefinition->type.c_str());
            //ImGui::Text(dataDefinition->defaultValue.c_str());
        }

        ImGui::Text("---- values ----");

        for (VirtualDatasheetObject::DataValue* dataValue : m_datasheet->m_rootObject->m_dataValues)
        {
            ImGui::Text(dataValue->name.c_str());
            //ImGui::Text(dataValue->value.c_str());
        }
    }
    ImGui::End();
}

void DatasheetPanel::UpdateProperties(const gugu::DeltaTime& dt)
{
    VirtualDatasheet* parentReference = m_datasheet->m_parentDatasheet;
    std::string dummyParentRefID = m_datasheet->m_parentDatasheet ? m_datasheet->m_parentDatasheet->GetID() : "";
    if (ImGui::InputText("parent datasheet", &dummyParentRefID))
    {
        // TODO: I should encapsulate this in some kind of GetOrLoad method.
        if (GetResources()->IsResourceLoaded(dummyParentRefID))
        {
            parentReference = dynamic_cast<VirtualDatasheet*>(GetResources()->GetResource(dummyParentRefID));
        }
        else
        {
            parentReference = GetEditor()->GetDatasheetParser()->InstanciateDatasheetResource(dummyParentRefID);
        }

        if (parentReference && parentReference->m_classDefinition != m_datasheet->m_classDefinition)
        {
            parentReference = nullptr;
        }
        else if (dummyParentRefID.empty())
        {
            parentReference = nullptr;
        }

        // TODO: refresh all m_parentObject references.
        m_datasheet->m_parentDatasheet = parentReference;
        m_dirty = true;
    }

    std::string parentObjectDefinition = m_datasheet->m_classDefinition->m_name;
    std::string description = parentReference ? "Valid Ref" : (!parentReference && dummyParentRefID.empty() ? "Empty Ref" : "Invalid Ref");

    ImGui::Indent();
    ImGui::Text(StringFormat("{1} (Def: {0})", parentObjectDefinition, description).c_str());
    ImGui::Unindent();

    for (DatasheetParser::ClassDefinition* classDefinition : m_datasheet->m_classDefinition->m_combinedInheritedClasses)
    {
        ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;
        if (ImGui::CollapsingHeader(classDefinition->m_name.c_str(), headerFlags))
        {
            ImGui::PushID(classDefinition->m_name.c_str());

            DisplayDataClass(classDefinition, m_datasheet->m_rootObject);

            ImGui::PopID();
        }
    }
}

void DatasheetPanel::DisplayDataClass(DatasheetParser::ClassDefinition* classDefinition, VirtualDatasheetObject* dataObject)
{
    for (DatasheetParser::DataMemberDefinition* dataMemberDef : classDefinition->dataMembers)
    {
        DisplayDataMember(dataMemberDef, dataObject);
    }

    if (classDefinition->dataMembers.empty())
    {
        ImGui::Text("default");
        ImGui::SameLine();
        ImGui::Text("No Data Member");
    }
}

void DatasheetPanel::DisplayDataMember(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject)
{
    bool isParentData = false;
    VirtualDatasheetObject::DataValue* dataValue = dataObject->GetDataValue(dataMemberDefinition->name, isParentData);

    if (dataValue && !isParentData)
    {
        // TODO: I will probably end up using a push ID for the whole data member edition.
        if (ImGui::Button(StringFormat("Reset##{0}", dataMemberDefinition->name).c_str()))
        {
            if (dataObject->RemoveDataValue(dataMemberDefinition->name))
            {
                // Retrieve the parent data now that we have removed the self data.
                dataValue = nullptr;
                isParentData = false;
                dataValue = dataObject->GetDataValue(dataMemberDefinition->name, isParentData);

                m_dirty = true;
            }
        }

        ImGui::SameLine();
    }

    ImGui::Text(dataValue ? (isParentData ? "parent " : "self   ") : "default");
    ImGui::SameLine();

    if (!dataMemberDefinition->isArray)
    {
        if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
        {
            DisplayInstanceDataMemberValue(dataMemberDefinition, dataObject, dataValue, isParentData);
        }
        else
        {
            DisplayInlineDataMemberValue(dataMemberDefinition, dataObject, dataValue, isParentData);
        }
    }
    else
    {
        ImGui::PushID(dataMemberDefinition->name.c_str());

        // TODO: force PushDisabled for instanced data if the data comes from the parent.
        ImGui::Text(dataMemberDefinition->name.c_str());

        // TODO: I need to design the user flow for overriding default and inherited arrays.
        ImGui::SameLine();
        if (ImGui::Button("+"))
        {
            if (!dataValue || isParentData)
            {
                dataValue = dataObject->RegisterDataValue(dataMemberDefinition);
            }

            // TODO: I probably need to move this in a dedicated function (I also need to inject the default value).
            VirtualDatasheetObject::DataValue* newChildDataValue = new VirtualDatasheetObject::DataValue;
            dataValue->value_children.push_back(newChildDataValue);
            m_dirty = true;
        }

        if (dataValue && !isParentData && !dataValue->value_children.empty())
        {
            // TODO: This should be replaced by a "remove" button on each line.
            ImGui::SameLine();
            if (ImGui::Button("-"))
            {
                // TODO: I probably need to move this in a dedicated function (I also need to inject the default value).
                SafeDelete(dataValue->value_children.back());
                dataValue->value_children.pop_back();
                m_dirty = true;
            }
        }

        ImGui::Indent();

        if (dataValue && !dataValue->value_children.empty())
        {
            int childIndex = 0;
            for (VirtualDatasheetObject::DataValue* childDataValue : dataValue->value_children)
            {
                ImGui::PushID(childIndex++);

                if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
                {
                    DisplayInstanceDataMemberValue(dataMemberDefinition, dataObject, childDataValue, isParentData);
                }
                else
                {
                    DisplayInlineDataMemberValue(dataMemberDefinition, dataObject, childDataValue, isParentData);
                }

                ImGui::PopID();
            }
        }
        else
        {
            ImGui::Text("Empty Array");
        }

        ImGui::Unindent();

        ImGui::PopID();
    }
}

void DatasheetPanel::DisplayInlineDataMemberValue(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject, VirtualDatasheetObject::DataValue* dataValue, bool isParentData)
{
    if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Bool)
    {
        bool dummy = dataValue ? dataValue->value_bool : dataMemberDefinition->defaultValue_bool;
        if (ImGui::Checkbox(dataMemberDefinition->name.c_str(), &dummy))
        {
            if (!dataValue || isParentData)
            {
                dataValue = dataObject->RegisterDataValue(dataMemberDefinition);
            }

            dataValue->value_bool = dummy;
            m_dirty = true;
        }
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Int)
    {
        int dummy = dataValue ? dataValue->value_int : dataMemberDefinition->defaultValue_int;
        if (ImGui::InputInt(dataMemberDefinition->name.c_str(), &dummy))
        {
            if (!dataValue || isParentData)
            {
                dataValue = dataObject->RegisterDataValue(dataMemberDefinition);
            }

            dataValue->value_int = dummy;
            m_dirty = true;
        }
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Float)
    {
        float dummy = dataValue ? dataValue->value_float : dataMemberDefinition->defaultValue_float;
        if (ImGui::InputFloat(dataMemberDefinition->name.c_str(), &dummy))
        {
            if (!dataValue || isParentData)
            {
                dataValue = dataObject->RegisterDataValue(dataMemberDefinition);
            }

            dataValue->value_float = dummy;
            m_dirty = true;
        }
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::String)
    {
        std::string dummy = dataValue ? dataValue->value_string : dataMemberDefinition->defaultValue_string;
        if (ImGui::InputText(dataMemberDefinition->name.c_str(), &dummy))
        {
            if (!dataValue || isParentData)
            {
                dataValue = dataObject->RegisterDataValue(dataMemberDefinition);
            }

            dataValue->value_string = dummy;
            m_dirty = true;
        }
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Enum)
    {
        std::string dummy = dataValue ? dataValue->value_string : dataMemberDefinition->defaultValue_string;

        ImGuiComboFlags flags = 0;
        if (ImGui::BeginCombo(dataMemberDefinition->name.c_str(), dummy.c_str(), flags))
        {
            const std::vector<std::string>& comboValues = dataMemberDefinition->enumDefinition->values;
            for (size_t i = 0; i < comboValues.size(); ++i)
            {
                bool selected = (dummy == comboValues[i]);
                if (ImGui::Selectable(comboValues[i].c_str(), selected))
                {
                    if (!dataValue || isParentData)
                    {
                        dataValue = dataObject->RegisterDataValue(dataMemberDefinition);
                    }

                    dataValue->value_string = comboValues[i];
                    m_dirty = true;
                }

                if (selected)
                {
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectReference)
    {
        if (!dataMemberDefinition->objectDefinition)
        {
            ImGui::Text("Invalid Definition");
        }
        else
        {
            VirtualDatasheet* objectReference = dataValue ? dataValue->value_objectReference : nullptr;
            std::string dummyRefID = dataValue ? dataValue->value_string : dataMemberDefinition->defaultValue_string;
            if (ImGui::InputText(dataMemberDefinition->name.c_str(), &dummyRefID))
            {
                if (!dataValue || isParentData)
                {
                    dataValue = dataObject->RegisterDataValue(dataMemberDefinition);
                }

                // TODO: I should encapsulate this in some kind of GetOrLoad method.
                if (GetResources()->IsResourceLoaded(dummyRefID))
                {
                    objectReference = dynamic_cast<VirtualDatasheet*>(GetResources()->GetResource(dummyRefID));
                }
                else
                {
                    objectReference = GetEditor()->GetDatasheetParser()->InstanciateDatasheetResource(dummyRefID);
                }

                // TODO: This may need to be refreshed more often, to handle created/deleted assets.
                if (objectReference && !objectReference->m_classDefinition->IsDerivedFromClass(dataMemberDefinition->objectDefinition))
                {
                    objectReference = nullptr;
                }

                dataValue->value_string = dummyRefID;
                dataValue->value_objectReference = objectReference;
                m_dirty = true;
            }

            std::string objectDefinition = dataMemberDefinition->objectDefinition->m_name;
            std::string description = objectReference ? "Valid Ref" : (!objectReference && dummyRefID.empty() ? "Empty Ref" : "Invalid Ref");

            ImGui::Indent();
            ImGui::Text(StringFormat("{1} (Def: {0})", objectDefinition, description).c_str());
            ImGui::Unindent();
        }
    }
}

void DatasheetPanel::DisplayInstanceDataMemberValue(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject, VirtualDatasheetObject::DataValue* dataValue, bool isParentData)
{
    DatasheetParser::ClassDefinition* instanceDefinition = dataValue ? dataValue->value_objectInstanceDefinition : nullptr;
    std::string dummyClassName = instanceDefinition ? instanceDefinition->m_name : "";

    ImGuiComboFlags flags = 0;
    if (ImGui::BeginCombo(dataMemberDefinition->name.c_str(), dummyClassName.c_str(), flags))
    {
        // TODO: precompute this in definition.
        std::vector<std::string> comboValues;
        comboValues.reserve(dataMemberDefinition->objectDefinition->m_derivedClasses.size() + 1);
        comboValues.push_back("");
        for (DatasheetParser::ClassDefinition* availableClass : dataMemberDefinition->objectDefinition->m_derivedClasses)
        {
            comboValues.push_back(availableClass->m_name);
        }

        for (size_t i = 0; i < comboValues.size(); ++i)
        {
            bool selected = (dummyClassName == comboValues[i]);
            if (ImGui::Selectable(comboValues[i].c_str(), selected))
            {
                if (!dataValue || isParentData)
                {
                    dataValue = dataObject->RegisterDataValue(dataMemberDefinition);
                }

                DatasheetParser::ClassDefinition* newInstanceDefinition = nullptr;
                GetEditor()->GetDatasheetParser()->GetClassDefinition(comboValues[i], newInstanceDefinition);

                if (newInstanceDefinition != instanceDefinition)
                {
                    SafeDelete(dataValue->value_objectInstance);

                    VirtualDatasheetObject* newInstanceObject = new VirtualDatasheetObject;
                    dataValue->value_objectInstanceDefinition = newInstanceDefinition;
                    dataValue->value_objectInstance = newInstanceObject;
                }

                m_dirty = true;
            }

            if (selected)
            {
                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    // If the definition is null, then the instance itself is null.
    if (dataValue && dataValue->value_objectInstanceDefinition)
    {
        ImGui::Indent();
        ImGui::PushID(dataMemberDefinition->name.c_str());

        for (DatasheetParser::ClassDefinition* classDefinition : dataValue->value_objectInstanceDefinition->m_combinedInheritedClasses)
        {
            ImGui::PushID(classDefinition->m_name.c_str());

            // TODO: force PushDisabled for instanced data if the data comes from the parent.
            DisplayDataClass(classDefinition, dataValue->value_objectInstance);

            ImGui::PopID();
        }

        ImGui::PopID();
        ImGui::Unindent();
    }
}

bool DatasheetPanel::Save()
{
    if (m_datasheet->SaveToFile())
    {
        m_dirty = false;
    }

    return !m_dirty;
}

}   //namespace gugu
