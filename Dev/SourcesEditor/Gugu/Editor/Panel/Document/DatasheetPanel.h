#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/Document/DocumentPanel.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"
#include "Gugu/Editor/Resources/VirtualDatasheet.h"

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

    DatasheetPanel(VirtualDatasheet* datasheet);
    virtual ~DatasheetPanel();

    virtual void UpdatePanelImpl(const gugu::DeltaTime& dt) override;
    virtual void UpdateProperties(const gugu::DeltaTime& dt) override;
    virtual bool Save() override;

protected:

    void DisplayDatasheet();
    void DisplayParentReference();
    void DisplayDataClass(DatasheetParser::ClassDefinition* classDefinition, VirtualDatasheetObject* dataObject);
    void DisplayDataMember(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject);
    void DisplayInlineDataMemberValue(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject, VirtualDatasheetObject::DataValue*& dataValue, bool isParentData);
    void DisplayInlineDataMemberContent(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject::DataValue*& dataValue);
    void DisplayInstanceDataMemberValue(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject, VirtualDatasheetObject::DataValue*& dataValue, bool isParentData);
    void DisplayInstanceDataMemberContent(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject::DataValue*& dataValue, bool isParentData);

protected:

    VirtualDatasheet* m_datasheet;
};

}   //namespace gugu
