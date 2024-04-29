#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/Document/DocumentPanel.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"
#include "Gugu/Editor/Resources/VirtualDatasheet.h"
#include "Gugu/Editor/Resources/VirtualDatasheetObject.h"

#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class VirtualDatasheet;
    class VirtualDatasheetObject;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class DatasheetPanel : public DocumentPanel
{
public:

    DatasheetPanel(VirtualDatasheet* resource);
    virtual ~DatasheetPanel();

protected:

    virtual void UpdatePanelImpl(const DeltaTime& dt) override;
    virtual bool SaveToFileImpl() override;

    void DisplayDatasheet();
    void DisplayParentReference();
    void DisplayDataClass(DatasheetParser::ClassDefinition* classDefinition, VirtualDatasheetObject* dataObject);
    void DisplayDataMember(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject);
    void DisplayInlineDataMemberValue(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject, VirtualDatasheetObject::DataValue*& dataValue, bool isParentData);
    void DisplayInlineDataMemberContent(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject::DataValue*& dataValue);
    void DisplayInstanceDataMemberValue(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject, VirtualDatasheetObject::DataValue*& dataValue, bool isParentData);
    void DisplayInstanceDataMemberContent(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject::DataValue*& dataValue, bool isParentData);

    void DisplayDepthColumn(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject, bool isParentObject, VirtualDatasheetObject::DataValue*& dataValue, bool& isParentData);
    void DisplayEmptyDepthColumn();

    void InstanciateDataObjectAndValueIfNeeded(VirtualDatasheetObject*& dataObject, VirtualDatasheetObject::DataValue*& dataValue, bool& isParentData, DatasheetParser::DataMemberDefinition* dataMemberDefinition);

    void OnResourceEvent(const Resource* resource, EResourceEvent event, const Resource* dependency);

protected:

    VirtualDatasheet* m_datasheet;
};

}   //namespace gugu
