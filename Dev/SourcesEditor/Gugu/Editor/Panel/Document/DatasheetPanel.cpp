////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/DatasheetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"
#include "Gugu/Editor/Resources/VirtualDatasheet.h"
#include "Gugu/Editor/Resources/VirtualDatasheetObject.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DatasheetPanel::DatasheetPanel(VirtualDatasheet* resource)
    : DocumentPanel(resource)
    , m_datasheet(resource)
{
    // Dependencies
    GetResources()->RegisterResourceListener(m_datasheet, this, STD_BIND_3(&DatasheetPanel::OnResourceEvent, this));
}

DatasheetPanel::~DatasheetPanel()
{
    // Dependencies
    GetResources()->UnregisterResourceListeners(m_datasheet, this);
}

void DatasheetPanel::UpdatePanelImpl(const DeltaTime& dt)
{
    DisplayDatasheet();
}

bool DatasheetPanel::SaveToFileImpl()
{
    m_datasheet->SortDataValues();
    return DocumentPanel::SaveToFileImpl();
}

void DatasheetPanel::DisplayDatasheet()
{
    if (m_datasheet->m_rootObject == nullptr)
        return;

    DisplayParentReference();

    // Using those as a base value to create width/height that are factor of the size of our font
    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY;
    if (ImGui::BeginTable("DatasheetPropertiesTable", 3, tableFlags))
    {
        // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
        ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_WidthFixed;
        ImGui::TableSetupColumn("name", columnFlags, 350.f);
        ImGui::TableSetupColumn("value", columnFlags, 300.f);
        ImGui::TableSetupColumn("depth", columnFlags, 100.f);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        for (DatasheetParser::ClassDefinition* classDefinition : m_datasheet->m_rootObject->m_classDefinition->m_combinedInheritedClasses)
        {
            //ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;
            //if (ImGui::CollapsingHeader(classDefinition->m_name.c_str(), headerFlags))
            //{
            ImGui::PushID(classDefinition->m_name.c_str());

            DisplayDataClass(classDefinition, m_datasheet->m_rootObject);

            ImGui::PopID();
            //}
        }

        ImGui::EndTable();
    }
}

void DatasheetPanel::DisplayParentReference()
{
    bool invalidRecursiveParent = false;
    std::string dummyParentRefID = m_datasheet->m_parentDatasheetID;
    VirtualDatasheet* parentReference = m_datasheet->m_parentDatasheet;

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

        if (!m_datasheet->IsValidAsParent(parentReference, &invalidRecursiveParent))
        {
            parentReference = nullptr;
        }
        else if (dummyParentRefID.empty())
        {
            parentReference = nullptr;
        }

        m_datasheet->SetParentDatasheet(dummyParentRefID, parentReference);
        RaiseDirty();
    }
    else
    {
        if (!dummyParentRefID.empty() && !parentReference)
        {
            // If the current ID is defined but not valid, retry to find it.
            if (GetResources()->IsResourceLoaded(dummyParentRefID))
            {
                VirtualDatasheet* checkParentReference = dynamic_cast<VirtualDatasheet*>(GetResources()->GetResource(dummyParentRefID));

                if (m_datasheet->IsValidAsParent(checkParentReference, &invalidRecursiveParent))
                {
                    parentReference = checkParentReference;
                    m_datasheet->SetParentDatasheet(dummyParentRefID, parentReference);

                    // No need to dirty the file here, since we are not editing any value, just refreshing the view.
                    //m_dirty = true;
                }
            }
        }
    }

    std::string parentObjectDefinition = !m_datasheet->GetClassDefinition() ? "None" : m_datasheet->GetClassDefinition()->m_name;
    std::string description = parentReference ? "Valid Ref" : (dummyParentRefID.empty() ? "Empty Ref" : (invalidRecursiveParent ? "Invalid Recursive Ref" : "Invalid Ref"));

    ImGui::Indent();
    ImGui::Text(StringFormat("{1} (Def: {0})", parentObjectDefinition, description).c_str());
    ImGui::Unindent();
}

void DatasheetPanel::DisplayDataClass(DatasheetParser::ClassDefinition* classDefinition, VirtualDatasheetObject* dataObject)
{
    for (DatasheetParser::DataMemberDefinition* dataMemberDef : classDefinition->dataMembers)
    {
        ImGui::PushID(dataMemberDef->name.c_str());

        DisplayDataMember(dataMemberDef, dataObject);

        ImGui::PopID();
    }

    //if (classDefinition->dataMembers.empty())
    //{
    //    ImGui::Text("default");
    //    ImGui::SameLine();
    //    ImGui::Text("No Data Member");
    //}
}

void DatasheetPanel::DisplayDataMember(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject)
{
    ImGuiTreeNodeFlags nodeIndentFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
    ImGuiTreeNodeFlags nodeLeafFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

    ImGui::TableNextRow();

    // TODO: obtain this from caller ?
    bool isParentObject = dataObject->m_datasheet != m_datasheet;

    bool isParentData = false;
    VirtualDatasheetObject::DataValue* dataValue = dataObject->GetDataValue(dataMemberDefinition->name, isParentData);

    if (!dataMemberDefinition->isArray)
    {
        ImGui::TableNextColumn();
        ImGuiTreeNodeFlags nodeFlags = dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance && dataValue && dataValue->value_objectInstanceDefinition ? nodeIndentFlags : nodeLeafFlags;
        bool nodeOpen = ImGui::TreeNodeEx("##node", nodeFlags, dataMemberDefinition->name.c_str());

        ImGui::TableNextColumn();

        // The PushItemWidth will hide the label of the next widget.
        ImGui::PushItemWidth(-1);

        if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
        {
            DisplayInstanceDataMemberValue(dataMemberDefinition, dataObject, dataValue, isParentData);
        }
        else
        {
            DisplayInlineDataMemberValue(dataMemberDefinition, dataObject, dataValue, isParentData);
        }

        ImGui::PopItemWidth();

        if (dataMemberDefinition->type != DatasheetParser::DataMemberDefinition::ObjectInstance)
        {
            DisplayInlineDataMemberContent(dataMemberDefinition, dataValue);
        }

        DisplayDepthColumn(dataMemberDefinition, dataObject, isParentObject, dataValue, isParentData);

        if (nodeOpen)
        {
            if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
            {
                DisplayInstanceDataMemberContent(dataMemberDefinition, dataValue, isParentData);
            }

            ImGui::TreePop();
        }
    }
    else
    {
        ImGui::TableNextColumn();
        ImGuiTreeNodeFlags nodeFlags = dataValue && !dataValue->value_children.empty() ? nodeIndentFlags : nodeLeafFlags;
        bool nodeOpen = ImGui::TreeNodeEx("##node", nodeFlags, dataMemberDefinition->name.c_str());

        ImGui::TableNextColumn();

        ImGui::PushID(dataMemberDefinition->name.c_str());

        // TODO: I need to design the user flow for overriding default and inherited arrays.
        if (ImGui::Button("+"))
        {
            InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, isParentData, dataMemberDefinition);

            dataObject->InstanciateNewArrayMemberDataValue(dataValue);
            RaiseDirty();
        }

        if (dataValue && !dataValue->value_children.empty())
        {
            ImGui::SameLine();
            ImGui::Text("Array size : %d", dataValue->value_children.size());
        }
        else
        {
            ImGui::SameLine();
            ImGui::Text("Empty Array");
        }

        DisplayDepthColumn(dataMemberDefinition, dataObject, isParentObject, dataValue, isParentData);

        if (nodeOpen)
        {
            if (dataValue && !dataValue->value_children.empty())
            {
                size_t childIndex = 0;
                while (childIndex < dataValue->value_children.size())
                {
                    VirtualDatasheetObject::DataValue* childDataValue = dataValue->value_children[childIndex];

                    ImGui::PushID((int)childIndex);

                    ImGui::TableNextRow();

                    ImGui::BeginDisabled(isParentObject || isParentData);

                    ImGui::TableNextColumn();
                    ImGuiTreeNodeFlags nodeChildFlags = dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance && childDataValue && childDataValue->value_objectInstanceDefinition ? nodeIndentFlags : nodeLeafFlags;
                    bool nodeChildOpen = ImGui::TreeNodeEx("##arrayitem", nodeChildFlags, "%s [%d]", dataMemberDefinition->name.c_str(), childIndex);

                    ImGui::TableNextColumn();

                    // The double PushItemWidth will hide the label of the next widget then force its size.
                    const float button_size = ImGui::GetFrameHeight();
                    ImGui::PushItemWidth(-1);
                    ImGui::PushItemWidth(ImMax(1.0f, ImGui::CalcItemWidth() - (button_size + ImGui::GetStyle().ItemInnerSpacing.x)));

                    if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
                    {
                        DisplayInstanceDataMemberValue(dataMemberDefinition, dataObject, childDataValue, isParentData);
                    }
                    else
                    {
                        DisplayInlineDataMemberValue(dataMemberDefinition, dataObject, childDataValue, isParentData);
                    }

                    ImGui::PopItemWidth();
                    ImGui::PopItemWidth();

                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().ItemInnerSpacing.x);
                    
                    bool removeChild = ImGui::Button("X##arrayitemremove", ImVec2(button_size, button_size));

                    if (dataMemberDefinition->type != DatasheetParser::DataMemberDefinition::ObjectInstance)
                    {
                        DisplayInlineDataMemberContent(dataMemberDefinition, childDataValue);
                    }

                    DisplayEmptyDepthColumn();

                    ImGui::EndDisabled();

                    if (nodeChildOpen)
                    {
                        if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
                        {
                            DisplayInstanceDataMemberContent(dataMemberDefinition, childDataValue, isParentData);
                        }

                        ImGui::TreePop();
                    }

                    ImGui::PopID();

                    // Handle child removal and array iteration.
                    if (removeChild && dataValue->RemoveChildDataValue(childIndex))
                    {
                        m_datasheet->DeleteOrphanedInstanceObjects();
                        RaiseDirty();
                    }
                    else
                    {
                        ++childIndex;
                    }
                }
            }

            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}

void DatasheetPanel::DisplayDepthColumn(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject, bool isParentObject, VirtualDatasheetObject::DataValue*& dataValue, bool& isParentData)
{
    ImGui::TableNextColumn();

    ImGui::Text(dataValue ? (isParentObject || isParentData ? "parent " : "self   ") : "default");

    if (dataValue && !isParentObject && !isParentData)
    {
        ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            if (dataObject->RemoveDataValue(dataMemberDefinition->name))
            {
                // Retrieve the parent data now that we have removed the self data.
                dataValue = nullptr;
                isParentData = false;
                dataValue = dataObject->GetDataValue(dataMemberDefinition->name, isParentData);

                m_datasheet->DeleteOrphanedInstanceObjects();
                RaiseDirty();
            }
        }
    }
}

void DatasheetPanel::DisplayEmptyDepthColumn()
{
    ImGui::TableNextColumn();
}

void DatasheetPanel::DisplayInlineDataMemberValue(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject, VirtualDatasheetObject::DataValue*& dataValue, bool isParentData)
{
    //ImGui::PushItemWidth(-1);

    if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Bool)
    {
        bool dummy = dataValue ? dataValue->value_bool : dataMemberDefinition->defaultValue_bool;
        if (ImGui::Checkbox("##value", &dummy))
        {
            InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, isParentData, dataMemberDefinition);

            dataValue->value_bool = dummy;
            RaiseDirty();
        }
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Int)
    {
        int dummy = dataValue ? dataValue->value_int : dataMemberDefinition->defaultValue_int;
        if (ImGui::InputInt("##value", &dummy))
        {
            InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, isParentData, dataMemberDefinition);

            dataValue->value_int = dummy;
            RaiseDirty();
        }
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Float)
    {
        float dummy = dataValue ? dataValue->value_float : dataMemberDefinition->defaultValue_float;
        if (ImGui::InputFloat("##value", &dummy))
        {
            InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, isParentData, dataMemberDefinition);

            dataValue->value_float = dummy;
            RaiseDirty();
        }
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::String)
    {
        std::string dummy = dataValue ? dataValue->value_string : dataMemberDefinition->defaultValue_string;
        if (ImGui::InputText("##value", &dummy))
        {
            InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, isParentData, dataMemberDefinition);

            dataValue->value_string = dummy;
            RaiseDirty();
        }
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Enum)
    {
        std::string dummy = dataValue ? dataValue->value_string : dataMemberDefinition->defaultValue_string;

        ImGuiComboFlags flags = 0;
        if (ImGui::BeginCombo("##value", dummy.c_str(), flags))
        {
            const std::vector<std::string>& comboValues = dataMemberDefinition->enumDefinition->values;
            for (size_t i = 0; i < comboValues.size(); ++i)
            {
                bool selected = (dummy == comboValues[i]);
                if (ImGui::Selectable(comboValues[i].c_str(), selected))
                {
                    InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, isParentData, dataMemberDefinition);

                    dataValue->value_string = comboValues[i];
                    RaiseDirty();
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
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Vector2i)
    {
        Vector2i dummy = dataValue ? dataValue->value_vector2i : dataMemberDefinition->defaultValue_vector2i;
        if (ImGui::InputInt2("##value", &dummy))
        {
            InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, isParentData, dataMemberDefinition);

            dataValue->value_vector2i = dummy;
            RaiseDirty();
        }
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Vector2f)
    {
        Vector2f dummy = dataValue ? dataValue->value_vector2f : dataMemberDefinition->defaultValue_vector2f;
        if (ImGui::InputFloat2("##value", &dummy))
        {
            InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, isParentData, dataMemberDefinition);

            dataValue->value_vector2f = dummy;
            RaiseDirty();
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
            if (ImGui::InputText("##value", &dummyRefID))
            {
                InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, isParentData, dataMemberDefinition);

                // TODO: I should encapsulate this in some kind of GetOrLoad method.
                if (GetResources()->IsResourceLoaded(dummyRefID))
                {
                    objectReference = dynamic_cast<VirtualDatasheet*>(GetResources()->GetResource(dummyRefID));
                }
                else
                {
                    objectReference = GetEditor()->GetDatasheetParser()->InstanciateDatasheetResource(dummyRefID);
                }

                if (objectReference && !objectReference->GetClassDefinition()->IsDerivedFromClass(dataMemberDefinition->objectDefinition))
                {
                    objectReference = nullptr;
                }

                dataValue->value_string = dummyRefID;
                dataValue->value_objectReference = objectReference;
                RaiseDirty();
            }
            else
            {
                if (dataValue && !dummyRefID.empty() && !objectReference)
                {
                    // If the current ID is defined but not valid, retry to find it.
                    if (GetResources()->IsResourceLoaded(dummyRefID))
                    {
                        VirtualDatasheet* checkReference = dynamic_cast<VirtualDatasheet*>(GetResources()->GetResource(dummyRefID));

                        if (checkReference && checkReference->GetClassDefinition()->IsDerivedFromClass(dataMemberDefinition->objectDefinition))
                        {
                            dataValue->value_objectReference = checkReference;

                            // No need to dirty the file here, since we are not editing any value, just refreshing the view.
                            //m_dirty = true;
                        }
                    }
                }
            }
        }
    }

    //ImGui::PopItemWidth();
}

void DatasheetPanel::DisplayInlineDataMemberContent(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject::DataValue*& dataValue)
{
    // TODO: maybe I could inline those informations and avoid splitting the display method ?
    if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectReference)
    {
        if (dataMemberDefinition->objectDefinition)
        {
            VirtualDatasheet* objectReference = dataValue ? dataValue->value_objectReference : nullptr;
            std::string dummyRefID = dataValue ? dataValue->value_string : dataMemberDefinition->defaultValue_string;
            std::string objectDefinition = dataMemberDefinition->objectDefinition->m_name;
            std::string description = objectReference ? "Valid Ref" : (!objectReference && dummyRefID.empty() ? "Empty Ref" : "Invalid Ref");

            ImGui::Indent();
            ImGui::Text(StringFormat("{1} (Def: {0})", objectDefinition, description).c_str());
            ImGui::Unindent();
        }
    }
}

void DatasheetPanel::DisplayInstanceDataMemberValue(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject, VirtualDatasheetObject::DataValue*& dataValue, bool isParentData)
{
    DatasheetParser::ClassDefinition* instanceDefinition = dataValue ? dataValue->value_objectInstanceDefinition : nullptr;
    std::string dummyClassName = instanceDefinition ? instanceDefinition->m_name : "";

    //ImGui::PushItemWidth(-1);

    ImGuiComboFlags flags = 0;
    if (ImGui::BeginCombo("##value", dummyClassName.c_str(), flags))
    {
        // TODO: precompute this in definition.
        std::vector<std::string> comboValues;
        comboValues.reserve(dataMemberDefinition->objectDefinition->m_availableDerivedClasses.size() + 1);
        comboValues.push_back("##_NULL_CLASS");

        for (DatasheetParser::ClassDefinition* availableClass : dataMemberDefinition->objectDefinition->m_availableDerivedClasses)
        {
            comboValues.push_back(availableClass->m_name);
        }

        for (size_t i = 0; i < comboValues.size(); ++i)
        {
            bool selected = (dummyClassName == comboValues[i]);
            if (ImGui::Selectable(comboValues[i].c_str(), selected))
            {
                InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, isParentData, dataMemberDefinition);

                DatasheetParser::ClassDefinition* newInstanceDefinition = nullptr;

                if (i != 0)
                {
                    GetEditor()->GetDatasheetParser()->GetClassDefinition(comboValues[i], newInstanceDefinition);
                }

                if (newInstanceDefinition != instanceDefinition || isParentData)
                {
                    if (m_datasheet->DeleteInstanceObject(dataValue->value_objectInstance))
                    {
                        if (newInstanceDefinition)
                        {
                            VirtualDatasheetObject* newInstanceObject = m_datasheet->InstanciateNewObject(newInstanceDefinition);

                            dataValue->value_objectInstanceDefinition = newInstanceDefinition;
                            dataValue->value_objectInstance = newInstanceObject;
                            dataValue->value_string = newInstanceObject->m_uuid.ToString();
                        }
                        else
                        {
                            dataValue->value_objectInstanceDefinition = nullptr;
                            dataValue->value_objectInstance = nullptr;
                            dataValue->value_string = "";
                        }
                    }
                }

                m_datasheet->DeleteOrphanedInstanceObjects();
                RaiseDirty();
            }

            if (selected)
            {
                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    //ImGui::PopItemWidth();
}

void DatasheetPanel::DisplayInstanceDataMemberContent(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject::DataValue*& dataValue, bool isParentData)
{
    // If the definition is null, then the instance itself is null.
    if (dataValue && dataValue->value_objectInstanceDefinition)
    {
        auto objectDefinition = dataValue->value_objectInstanceDefinition;
        auto objectInstance = dataValue->value_objectInstance;

        // Check if we have an override for this object.
        if (isParentData)
        {
            VirtualDatasheetObject* overrideObject = m_datasheet->GetObjectOverrideFromUuid(dataValue->value_objectInstance->m_uuid);
            if (overrideObject)
            {
                objectInstance = overrideObject;
            }
        }

        ImGui::PushID(dataMemberDefinition->name.c_str());

        for (DatasheetParser::ClassDefinition* classDefinition : objectDefinition->m_combinedInheritedClasses)
        {
            ImGui::PushID(classDefinition->m_name.c_str());

            DisplayDataClass(classDefinition, objectInstance);

            ImGui::PopID();
        }

        ImGui::PopID();
    }
}

void DatasheetPanel::InstanciateDataObjectAndValueIfNeeded(VirtualDatasheetObject*& dataObject, VirtualDatasheetObject::DataValue*& dataValue, bool& isParentData, DatasheetParser::DataMemberDefinition* dataMemberDefinition)
{
    if (dataObject->m_datasheet != m_datasheet)
    {
        VirtualDatasheetObject* parentObject = dataObject;
        dataObject = m_datasheet->InstanciateNewObjectOverride(parentObject->m_classDefinition, parentObject->m_uuid);
        dataObject->RefreshParentObject(parentObject);

        dataValue = nullptr;
    }

    if (!dataValue || isParentData)
    {
        dataValue = dataObject->InstanciateNewClassMemberDataValue(dataMemberDefinition);
        isParentData = false;
    }
}

void DatasheetPanel::OnResourceEvent(const Resource* resource, EResourceEvent event, const Resource* dependency)
{
    //if (event == EResourceEvent::DependencyRemoved
    //    || event == EResourceEvent::DependencyUpdated)
    //{
    //}
}

}   //namespace gugu
