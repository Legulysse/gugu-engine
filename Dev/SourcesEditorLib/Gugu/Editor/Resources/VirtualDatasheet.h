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

    VirtualDatasheetObject::DataValue* GetDataValue(const std::string& name, bool& isParentData) const;

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

public:

    DatasheetParser::ClassDefinition* m_classDefinition;
    VirtualDatasheetObject* m_rootObject;
    VirtualDatasheet* m_parentDatasheet;
};

}   // namespace gugu
