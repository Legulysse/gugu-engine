#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Parser/DatasheetParser.h"

#include "Gugu/Resources/Resource.h"

#include <vector>
#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace pugi
{
    class xml_node;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class VirtualDatasheetObject
{
public:

    struct DataValue
    {
        std::string name;
        DatasheetParser::DataMemberDefinition* dataMemberDefinition = nullptr;
        std::string backupValue;

        bool value_bool = false;
        int value_int = 0;
        float value_float = 0.f;
        std::string value_string;
        DatasheetParser::ClassDefinition* value_objectInstanceDefinition = nullptr;
        VirtualDatasheetObject* value_objectInstance = nullptr;
        VirtualDatasheet* value_objectReference = nullptr;
        std::vector<VirtualDatasheetObject::DataValue*> value_children; // TODO: should I store objects, or dataValues holding the objects ?

        ~DataValue();
    };

public:

    VirtualDatasheetObject();
    ~VirtualDatasheetObject();

    bool LoadFromXml(const pugi::xml_node& nodeDatasheetObject, DatasheetParser::ClassDefinition* classDefinition, VirtualDatasheetObject* parentObject);
    bool SaveToXml(pugi::xml_node& nodeDatasheetObject) const;

    VirtualDatasheetObject::DataValue* RegisterDataValue(DatasheetParser::DataMemberDefinition* dataMemberDef);
    VirtualDatasheetObject::DataValue* GetDataValue(const std::string& name, bool& isParentData) const;

protected:

    void ParseInlineDataValue(const pugi::xml_node& nodeData, DatasheetParser::DataMemberDefinition* dataMemberDef, VirtualDatasheetObject::DataValue* dataValue);
    void ParseInstanceDataValue(const pugi::xml_node& nodeData, DatasheetParser::DataMemberDefinition* dataMemberDef, VirtualDatasheetObject::DataValue* dataValue);

    void SaveInlineDataValue(pugi::xml_node& nodeData, const VirtualDatasheetObject::DataValue* dataValue, DatasheetParser::DataMemberDefinition::Type memberType) const;
    void SaveInstanceDataValue(pugi::xml_node& nodeData, const VirtualDatasheetObject::DataValue* dataValue, DatasheetParser::ClassDefinition* classDefinition) const;

public:

    VirtualDatasheetObject* m_parentObject;
    DatasheetParser::ClassDefinition* m_classDefinition = nullptr;
    std::vector<VirtualDatasheetObject::DataValue*> m_dataValues;
};

class VirtualDatasheet : public Resource
{
public:

    VirtualDatasheet();
    virtual ~VirtualDatasheet();

    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;
    virtual bool SaveToFile() override;

public:

    DatasheetParser::ClassDefinition* m_classDefinition;
    VirtualDatasheetObject* m_rootObject;
    VirtualDatasheet* m_parentDatasheet;
};

}   // namespace gugu
