////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/DatasheetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/EditorCore.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"
#include "Gugu/Editor/Resources/VirtualDatasheet.h"

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

    // Parse definition.
    ClassDefinitionEntry classEntry;
    classEntry.classDefinition = m_datasheet->m_classDefinition;
    m_classEntries.push_back(classEntry);

    DatasheetParser::ClassDefinition* baseDefinition = m_datasheet->m_classDefinition->baseDefinition;
    while (baseDefinition != nullptr)
    {
        ClassDefinitionEntry baseClassEntry;
        baseClassEntry.classDefinition = baseDefinition;
        m_classEntries.insert(m_classEntries.begin(), baseClassEntry);

        baseDefinition = baseDefinition->baseDefinition;
    }
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
    std::string dummyParent = m_datasheet->m_parentDatasheet ? "Ref: " + m_datasheet->m_parentDatasheet->GetID() : "";
    ImGui::InputText("parent##root", &dummyParent);

    for (ClassDefinitionEntry classEntry : m_classEntries)
    {
        ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;
        if (ImGui::CollapsingHeader(classEntry.classDefinition->m_name.c_str(), headerFlags))
        {
            //ImGui::Indent();
            ImGui::PushID(classEntry.classDefinition->m_name.c_str());

            DisplayDataClass(classEntry.classDefinition, m_datasheet->m_rootObject);

            ImGui::PopID();
            //ImGui::Unindent();
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
        ImGui::Text(dataMemberDefinition->name.c_str());
        ImGui::Indent();

        if (dataValue && !dataValue->value_children.empty())
        {
            // TODO: force PushDisabled for instanced data if the data comes from the parent.
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
        ImGui::InputText(dataMemberDefinition->name.c_str(), &dummy);
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectReference)
    {
        std::string objectDefinition = dataMemberDefinition->objectDefinition ? dataMemberDefinition->objectDefinition->m_name : "Invalid Definition";
        std::string dummy = dataValue && dataValue->value_objectReference ? "Ref: " + dataValue->value_objectReference->GetID() : StringFormat("Empty Ref (Def: {0})", objectDefinition);
        ImGui::InputText(dataMemberDefinition->name.c_str(), &dummy);
    }
}

void DatasheetPanel::DisplayInstanceDataMemberValue(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject, VirtualDatasheetObject::DataValue* dataValue, bool isParentData)
{
    if (dataValue)
    {
        // TODO: force PushDisabled for instanced data if the data comes from the parent.
        std::string objectDefinition = dataValue->value_objectInstanceDefinition ? dataValue->value_objectInstanceDefinition->m_name : "Invalid Definition";
        std::string dummy = StringFormat("Instance (Def: {0})", objectDefinition);
        ImGui::InputText(dataMemberDefinition->name.c_str(), &dummy);

        ImGui::Indent();
        ImGui::PushID(dataMemberDefinition->name.c_str());

        DisplayDataClass(dataValue->value_objectInstanceDefinition, dataValue->value_objectInstance);

        ImGui::PopID();
        ImGui::Unindent();
    }
    else
    {
        std::string objectDefinition = dataMemberDefinition->objectDefinition ? dataMemberDefinition->objectDefinition->m_name : "Invalid Definition";
        std::string dummy = StringFormat("Empty Instance (Def: {0})", objectDefinition);
        ImGui::InputText(dataMemberDefinition->name.c_str(), &dummy);
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
