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
        VirtualDatasheet* value_objectReference = nullptr;  // TODO: this is more a helper than an actual serialized value data, the naming should reflect that.
        std::vector<VirtualDatasheetObject::DataValue*> value_children;

        ~DataValue();
    };

public:

    VirtualDatasheetObject();
    ~VirtualDatasheetObject();

    void GetDependencies(std::vector<Resource*>& dependencies) const;
    void OnDependencyRemoved(const Resource* removedDependency);

    bool LoadFromXml(const pugi::xml_node& nodeDatasheetObject, DatasheetParser::ClassDefinition* classDefinition);
    bool SaveToXml(pugi::xml_node& nodeDatasheetObject) const;

    void RefreshParentObject(VirtualDatasheetObject* parentObject);

    VirtualDatasheetObject::DataValue* RegisterDataValue(DatasheetParser::DataMemberDefinition* dataMemberDef);
    bool RemoveDataValue(const std::string& name);
    VirtualDatasheetObject::DataValue* GetDataValue(const std::string& name, bool& isParentData) const;

protected:

    void GetDependencies(const std::vector<VirtualDatasheetObject::DataValue*>& dataValues, std::vector<Resource*>& dependencies) const;
    void OnDependencyRemoved(const Resource* removedDependency, std::vector<VirtualDatasheetObject::DataValue*>& dataValues);

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

    VirtualDatasheet(DatasheetParser::ClassDefinition* classDefinition);
    virtual ~VirtualDatasheet();

    virtual EResourceType::Type GetResourceType() const override;

    virtual void GetDependencies(std::vector<Resource*>& dependencies) const override;
    virtual void OnDependencyRemoved(const Resource* removedDependency) override;

    bool IsValidAsParent(VirtualDatasheet* parentDatasheet, bool* invalidRecursiveParent) const;    // TODO: I could use an enum for error returns, and reuse them in other cases of references error feedbacks.
    void SetParentDatasheet(const std::string& parentDatasheetID, VirtualDatasheet* parentDatasheet);

protected:

    virtual void Unload() override;
    virtual bool LoadFromXml(const pugi::xml_document& document) override;
    virtual bool SaveToXml(pugi::xml_document& document) const override;

public:

    DatasheetParser::ClassDefinition* m_classDefinition;
    VirtualDatasheetObject* m_rootObject;
    std::string m_parentDatasheetID;
    VirtualDatasheet* m_parentDatasheet;
};

}   // namespace gugu
