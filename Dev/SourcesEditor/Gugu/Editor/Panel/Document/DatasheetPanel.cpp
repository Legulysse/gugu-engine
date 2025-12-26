////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/DatasheetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"
#include "Gugu/Editor/Core/EditorClipboard.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"
#include "Gugu/Editor/Resources/VirtualDatasheet.h"
#include "Gugu/Editor/Resources/VirtualDatasheetObject.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/LocalizationTable.h"
#include "Gugu/System/String.h"
#include "Gugu/System/Container.h"
#include "Gugu/System/Time.h"
#include "Gugu/Debug/Logger.h"
#include "Gugu/External/ImGuiUtility.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// Constants

namespace gugu
{
    namespace impl
    {
        const std::string ClipboardContentType_DatasheetObjectData = "DatasheetObjectData";
    }
}

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DatasheetPanel::DatasheetPanel(VirtualDatasheet* resource)
    : DocumentPanel(resource)
    , m_datasheet(resource)
{
    // Dependencies
    GetResources()->RegisterResourceListener(m_datasheet, Handle(this), STD_BIND_3(&DatasheetPanel::OnResourceEvent, this));
}

DatasheetPanel::~DatasheetPanel()
{
    // Dependencies
    GetResources()->UnregisterResourceListeners(m_datasheet, Handle(this));
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

    // Note: NoSavedSettings is already applied on the whole document panel, but I keep it here to match property tables.
    ImGuiTableFlags tableFlags = ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY;
    if (ImGui::BeginTable("_DATASHEET_PROPERTIES_TABLE", 3, tableFlags))
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
    const float button_size = ImGui::GetFrameHeight();

    ImGui::TableNextRow();

    VirtualDatasheetObject::DataValue* dataValue = dataObject->GetDataValue(dataMemberDefinition->name);

    if (!dataMemberDefinition->isArray)
    {
        // Name column.
        ImGui::TableNextColumn();
        ImGuiTreeNodeFlags nodeFlags = dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance && dataValue && dataValue->value_objectInstanceDefinition ? nodeIndentFlags : nodeLeafFlags;
        bool nodeOpen = ImGui::TreeNodeEx("##_NODE", nodeFlags, dataMemberDefinition->name.c_str());

        // Context menu.
        HandleContextMenu(dataMemberDefinition, dataObject, dataValue);

        // Value column.
        ImGui::TableNextColumn();

        // The PushItemWidth will hide the label of the next widget.
        ImGui::PushItemWidth(-1);

        if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
        {
            DisplayInstanceDataMemberValue(dataMemberDefinition, dataObject, dataValue);
        }
        else
        {
            DisplayInlineDataMemberValue(dataMemberDefinition, dataObject, dataValue);
        }

        ImGui::PopItemWidth();

        // Depth column.
        DisplayDepthColumn(dataMemberDefinition, dataObject, dataValue);

        if (nodeOpen)
        {
            if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
            {
                DisplayInstanceDataMemberContent(dataMemberDefinition, dataValue);
            }

            ImGui::TreePop();
        }
    }
    else
    {
        ImGui::TableNextColumn();
        ImGuiTreeNodeFlags nodeFlags = dataValue && !dataValue->value_children.empty() ? nodeIndentFlags : nodeLeafFlags;
        bool nodeOpen = ImGui::TreeNodeEx("##_NODE", nodeFlags, dataMemberDefinition->name.c_str());

        ImGui::TableNextColumn();

        ImGui::PushID(dataMemberDefinition->name.c_str());

        // TODO: I need to design the user flow for overriding default and inherited arrays.
        bool addChild = ImGui::Button("+", ImVec2(button_size, button_size));

        if (addChild)
        {
            InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, dataMemberDefinition);

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

        DisplayDepthColumn(dataMemberDefinition, dataObject, dataValue);

        if (nodeOpen)
        {
            if (dataValue && !dataValue->value_children.empty())
            {
                size_t moveDownChildIndex = system::InvalidIndex;
                size_t moveUpChildIndex = system::InvalidIndex;
                size_t removeChildIndex = system::InvalidIndex;

                for (size_t childIndex = 0; childIndex < dataValue->value_children.size(); ++childIndex)
                {
                    VirtualDatasheetObject::DataValue* childDataValue = dataValue->value_children[childIndex];

                    ImGui::PushID((int)childIndex);

                    ImGui::TableNextRow();

                    ImGui::BeginDisabled(dataValue->owner->m_datasheet != m_datasheet);

                    // Name column.
                    ImGui::TableNextColumn();
                    ImGuiTreeNodeFlags nodeChildFlags = dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance && childDataValue && childDataValue->value_objectInstanceDefinition ? nodeIndentFlags : nodeLeafFlags;
                    bool nodeChildOpen = ImGui::TreeNodeEx("##_ARRAY_ITEM", nodeChildFlags, "%s [%d]", dataMemberDefinition->name.c_str(), childIndex);

                    // Context menu.
                    HandleContextMenu(dataMemberDefinition, dataObject, childDataValue);

                    // Value column.
                    ImGui::TableNextColumn();

                    // The double PushItemWidth will hide the label of the next widget then force its size.
                    const float sizeOffset = 3 * (button_size + ImGui::GetStyle().ItemInnerSpacing.x);
                    ImGui::PushItemWidth(-1);
                    ImGui::PushItemWidth(ImMax(1.0f, ImGui::CalcItemWidth() - sizeOffset));

                    if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
                    {
                        DisplayInstanceDataMemberValue(dataMemberDefinition, dataObject, childDataValue);
                    }
                    else
                    {
                        DisplayInlineDataMemberValue(dataMemberDefinition, dataObject, childDataValue);
                    }

                    ImGui::PopItemWidth();
                    ImGui::PopItemWidth();

                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().ItemInnerSpacing.x);
                    ImGui::BeginDisabled(childIndex == dataValue->value_children.size() - 1);
                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.13f, 0.65f, 0.65f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.13f, 0.9f, 0.9f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.13f, 1.0f, 1.0f));
                    if (ImGui::ArrowButton("##_ARRAY_ITEM_MOVE_DOWN", ImGuiDir_Down))
                    {
                        moveDownChildIndex = childIndex;
                    }
                    ImGui::PopStyleColor(3);
                    ImGui::EndDisabled();

                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().ItemInnerSpacing.x);
                    ImGui::BeginDisabled(childIndex == 0);
                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.13f, 0.65f, 0.65f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.13f, 0.9f, 0.9f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.13f, 1.0f, 1.0f));
                    if (ImGui::ArrowButton("##_ARRAY_ITEM_MOVE_UP", ImGuiDir_Up))
                    {
                        moveUpChildIndex = childIndex;
                    }
                    ImGui::PopStyleColor(3);
                    ImGui::EndDisabled();

                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().ItemInnerSpacing.x);
                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.65f, 0.65f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.9f, 0.9f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 1.0f, 1.0f));
                    if (ImGui::Button("X##_ARRAY_ITEM_REMOVE", ImVec2(button_size, button_size)))
                    {
                        removeChildIndex = childIndex;
                    }
                    ImGui::PopStyleColor(3);

                    // Depth column.
                    DisplayEmptyDepthColumn();

                    ImGui::EndDisabled();

                    if (nodeChildOpen)
                    {
                        if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
                        {
                            DisplayInstanceDataMemberContent(dataMemberDefinition, childDataValue);
                        }

                        ImGui::TreePop();
                    }

                    ImGui::PopID();
                }

                // Handle child removal and array iteration.
                if (moveDownChildIndex != system::InvalidIndex && dataValue->MoveChildDataValue(moveDownChildIndex, moveDownChildIndex + 1))
                {
                    RaiseDirty();
                }
                else if (moveUpChildIndex != system::InvalidIndex && dataValue->MoveChildDataValue(moveUpChildIndex, moveUpChildIndex - 1))
                {
                    RaiseDirty();
                }
                else if (removeChildIndex != system::InvalidIndex && dataValue->RemoveChildDataValue(removeChildIndex))
                {
                    m_datasheet->DeleteOrphanedInstanceObjects();
                    RaiseDirty();
                }
            }

            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}

void DatasheetPanel::DisplayDepthColumn(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject, VirtualDatasheetObject::DataValue*& dataValue)
{
    ImGui::TableNextColumn();

    ImGui::Text(dataValue ? (dataValue->owner->m_datasheet != m_datasheet ? "parent " : "self   ") : "default");

    if (dataValue && dataValue->owner->m_datasheet == m_datasheet)
    {
        ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            if (dataObject->RemoveDataValue(dataMemberDefinition->name))
            {
                // Retrieve the parent data now that we have removed the self data.
                dataValue = nullptr;
                dataValue = dataObject->GetDataValue(dataMemberDefinition->name);

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

void DatasheetPanel::DisplayInlineDataMemberValue(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject, VirtualDatasheetObject::DataValue*& dataValue)
{
    const float button_size = ImGui::GetFrameHeight();

    //ImGui::PushItemWidth(-1);

    if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Bool)
    {
        bool dummy = dataValue ? dataValue->value_bool : dataMemberDefinition->defaultValue_bool;
        if (ImGui::Checkbox("##_VALUE", &dummy))
        {
            InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, dataMemberDefinition);

            dataValue->value_bool = dummy;
            RaiseDirty();
        }
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Int)
    {
        int dummy = dataValue ? dataValue->value_int : dataMemberDefinition->defaultValue_int;
        if (ImGui::InputInt("##_VALUE", &dummy))
        {
            InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, dataMemberDefinition);

            dataValue->value_int = dummy;
            RaiseDirty();
        }
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Float)
    {
        float dummy = dataValue ? dataValue->value_float : dataMemberDefinition->defaultValue_float;
        if (ImGui::InputFloat("##_VALUE", &dummy))
        {
            InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, dataMemberDefinition);

            dataValue->value_float = dummy;
            RaiseDirty();
        }
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::String)
    {
        std::string dummy = dataValue ? dataValue->value_string : dataMemberDefinition->defaultValue_string;
        if (ImGui::InputText("##_VALUE", &dummy))
        {
            InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, dataMemberDefinition);

            if (dataMemberDefinition->isLocalized)
            {
                // Generate localization key if necessary.
                if (dataValue->localizationKey.empty())
                {
                    dataValue->localizationKey = GenerateLocalizationKeyForDatasheetMember(m_datasheet->GetID(), dataObject->m_uuid.ToString(), dataMemberDefinition->name);
                }

                // Update last edit timestamp.
                dataValue->localizationTimestamp = GetUtcTimestampAsNumeric();
            }

            dataValue->value_string = dummy;
            RaiseDirty();
        }
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Enum)
    {
        std::string dummy = dataValue ? dataValue->value_string : dataMemberDefinition->defaultValue_string;

        ImGuiComboFlags flags = 0;
        if (ImGui::BeginCombo("##_VALUE", dummy.c_str(), flags))
        {
            const std::vector<std::string>& comboValues = dataMemberDefinition->enumDefinition->values;
            for (size_t i = 0; i < comboValues.size(); ++i)
            {
                bool selected = (dummy == comboValues[i]);
                if (ImGui::Selectable(comboValues[i].c_str(), selected))
                {
                    InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, dataMemberDefinition);

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
        if (ImGui::InputInt2("##_VALUE", &dummy))
        {
            InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, dataMemberDefinition);

            dataValue->value_vector2i = dummy;
            RaiseDirty();
        }
    }
    else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Vector2f)
    {
        Vector2f dummy = dataValue ? dataValue->value_vector2f : dataMemberDefinition->defaultValue_vector2f;
        if (ImGui::InputFloat2("##_VALUE", &dummy))
        {
            InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, dataMemberDefinition);

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

            float allocatedWidth = ImGui::CalcItemWidth();

            // Feedback reference validation.
            if (objectReference)
            {
                // Valid reference.
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.32f, 0.65f, 0.65f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.32f, 0.9f, 0.9f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.32f, 1.0f, 1.0f));
                ImGui::Button("##_REFERENCE_BUTTON", ImVec2(button_size, button_size));
                ImGui::PopStyleColor(3);
            }
            else if (!objectReference && dummyRefID.empty())
            {
                // Null reference.
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.f, 0.65f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.f, 0.9f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.f, 1.0f));
                ImGui::Button("##_REFERENCE_BUTTON", ImVec2(button_size, button_size));
                ImGui::PopStyleColor(3);
            }
            else
            {
                // Invalid reference.
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.65f, 0.65f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.9f, 0.9f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 1.0f, 1.0f));
                ImGui::Button("##_REFERENCE_BUTTON", ImVec2(button_size, button_size));
                ImGui::PopStyleColor(3);
            }

            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().ItemInnerSpacing.x);

            // Edit reference.
            float sizeOffset = button_size + ImGui::GetStyle().ItemInnerSpacing.x;
            ImGui::PushItemWidth(ImMax(1.0f, allocatedWidth - sizeOffset));
            bool editedValue = ImGui::InputText("##_VALUE", &dummyRefID);
            ImGui::PopItemWidth();

            if (editedValue)
            {
                InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, dataMemberDefinition);

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

void DatasheetPanel::DisplayInstanceDataMemberValue(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject, VirtualDatasheetObject::DataValue*& dataValue)
{
    DatasheetParser::ClassDefinition* instanceDefinition = dataValue ? dataValue->value_objectInstanceDefinition : nullptr;
    std::string dummyClassName = instanceDefinition ? instanceDefinition->m_name : "";

    //ImGui::PushItemWidth(-1);

    ImGuiComboFlags flags = 0;
    if (ImGui::BeginCombo("##_VALUE", dummyClassName.c_str(), flags))
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
                bool isNewData = InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, dataMemberDefinition);

                DatasheetParser::ClassDefinition* newInstanceDefinition = nullptr;

                if (i != 0)
                {
                    GetEditor()->GetDatasheetParser()->GetClassDefinition(comboValues[i], newInstanceDefinition);
                }

                if (newInstanceDefinition != instanceDefinition || isNewData)
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

void DatasheetPanel::DisplayInstanceDataMemberContent(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject::DataValue*& dataValue)
{
    // If the definition is null, then the instance itself is null.
    if (dataValue && dataValue->value_objectInstanceDefinition)
    {
        auto objectDefinition = dataValue->value_objectInstanceDefinition;
        auto objectInstance = dataValue->value_objectInstance;

        // Check if we have an override for this object.
        if (objectInstance->m_datasheet != m_datasheet)
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

bool DatasheetPanel::InstanciateDataObjectAndValueIfNeeded(VirtualDatasheetObject*& dataObject, VirtualDatasheetObject::DataValue*& dataValue, DatasheetParser::DataMemberDefinition* dataMemberDefinition)
{
    bool isNewData = false;

    // TODO: Should I check that the dataObject actually comes from a parent datasheet ?
    if (dataObject->m_datasheet != m_datasheet)
    {
        VirtualDatasheetObject* parentObject = dataObject;
        dataObject = m_datasheet->InstanciateNewObjectOverride(parentObject->m_classDefinition, parentObject->m_uuid);
        dataObject->SetParentObject(parentObject);
        isNewData = true;
    }

    if (isNewData || !dataValue || dataValue->owner->m_datasheet != m_datasheet)
    {
        dataValue = dataObject->InstanciateNewClassMemberDataValue(dataMemberDefinition);
        isNewData = true;
    }

    return isNewData;
}

void DatasheetPanel::HandleContextMenu(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject, VirtualDatasheetObject::DataValue* dataValue)
{
    if (dataMemberDefinition->type != DatasheetParser::DataMemberDefinition::ObjectInstance)
        return;

    // TODO: Handle copy of object overrides.
    // TODO: Ensure the copy will retrieve ALL override and instance data from its parent object.

    bool canCopy = dataValue && dataValue->value_objectInstance && dataValue->value_objectInstance->m_datasheet == m_datasheet;  // Disable for object overrides.
    bool canPaste = GetEditorClipboard()->contentType == impl::ClipboardContentType_DatasheetObjectData;   // Disable if clipboard type does not match.

    // TODO: To handle disabled items, I should use my own variation of BeginPopupContextItem.
    if (ImGui::BeginPopupContextItem())
    {
        ImGui::BeginDisabled(!canCopy);
        if (ImGui::MenuItem("Copy"))
        {
            pugi::xml_document xmlDocument;

            bool savedRoot = dataValue->value_objectInstance->SaveToXml(xmlDocument, "ClipboardRootObject");

            std::set<UUID> instanceUuids;
            dataValue->value_objectInstance->GatherInstanceUuidsRecursively(instanceUuids);

            for (const auto& uuid : instanceUuids)
            {
                VirtualDatasheetObject* referencedObject = m_datasheet->GetInstanceObjectFromUuid(uuid);
                if (!referencedObject)
                    referencedObject = m_datasheet->GetObjectOverrideFromUuid(uuid);
                if (referencedObject)
                {
                    bool savedObject = referencedObject->SaveToXml(xmlDocument, "ClipboardObject");
                }
            }

            std::string stringContent = xml::SaveDocumentToString(xmlDocument);

            GetEditorClipboard()->SetStringContent(impl::ClipboardContentType_DatasheetObjectData, stringContent);
        }
        ImGui::EndDisabled();

        ImGui::BeginDisabled(!canPaste);
        if (ImGui::MenuItem("Paste"))
        {
            if (StringEquals(GetEditorClipboard()->contentType, impl::ClipboardContentType_DatasheetObjectData))
            {
                pugi::xml_document xmlDocument;
                bool parseResult = xml::ParseDocumentFromString(GetEditorClipboard()->stringContent, xmlDocument);
                pugi::xml_node clipboardRootNode = xmlDocument.child("ClipboardRootObject");
                
                bool isNewData = InstanciateDataObjectAndValueIfNeeded(dataObject, dataValue, dataMemberDefinition);
                if (m_datasheet->DeleteInstanceObject(dataValue->value_objectInstance))
                {
                    VirtualDatasheetObject* newRootObject = nullptr;
                    std::map<UUID, VirtualDatasheetObject*> newInstanceObjects;
                    std::set<UUID> orphanObjectUuids;

                    {
                        VirtualDatasheetObject* newObject = new VirtualDatasheetObject;
                        newObject->LoadFromXml(clipboardRootNode, m_datasheet);

                        newRootObject = newObject;
                    }

                    for (pugi::xml_node clipboardNode = xmlDocument.child("ClipboardObject"); clipboardNode; clipboardNode = clipboardNode.next_sibling("ClipboardObject"))
                    {
                        VirtualDatasheetObject* newObject = new VirtualDatasheetObject;
                        newObject->LoadFromXml(clipboardNode, m_datasheet);

                        newInstanceObjects.insert(std::make_pair(newObject->m_uuid, newObject));
                        orphanObjectUuids.insert(newObject->m_uuid);
                    }

                    // Override the target dataValue with this new object.
                    dataValue->value_objectInstanceDefinition = newRootObject->m_classDefinition;
                    dataValue->value_objectInstance = newRootObject;
                    dataValue->value_string = newRootObject->m_uuid.ToString();

                    // Rebuild references between data objects.
                    newRootObject->ResolveInstances(newInstanceObjects, orphanObjectUuids);

                    for (const auto& kvp : newInstanceObjects)
                    {
                        kvp.second->ResolveInstances(newInstanceObjects, orphanObjectUuids);
                    }

                    if (!orphanObjectUuids.empty())
                    {
                        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("Datasheet contains {0} orphan instanced objects : {1}", orphanObjectUuids.size(), m_datasheet->GetFileInfo().GetFilePath_utf8()));
                    }

                    // Generate new uuids for every new node.
                    UUID uuid = UUID::Generate();
                    dataValue->value_string = uuid.ToString();
                    dataValue->value_objectInstance->m_uuid = uuid;

                    newRootObject->RegenerateInstanceUuidsRecursively();

                    // Register new instance objects.
                    m_datasheet->m_instanceObjects.insert(std::make_pair(newRootObject->m_uuid, newRootObject));

                    for (const auto& kvp : newInstanceObjects)
                    {
                        VirtualDatasheetObject* newObject = kvp.second;
                        m_datasheet->m_instanceObjects.insert(std::make_pair(newObject->m_uuid, newObject));
                    }
                }

                m_datasheet->DeleteOrphanedInstanceObjects();
                RaiseDirty();
            }
        }
        ImGui::EndDisabled();

        ImGui::EndPopup();
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
