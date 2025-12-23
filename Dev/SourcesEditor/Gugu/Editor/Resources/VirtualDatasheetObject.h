#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Parser/DatasheetParser.h"

#include "Gugu/System/UUID.h"
#include "Gugu/Math/Vector2.h"

#include <string>
#include <vector>
#include <map>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Resource;
    class VirtualDatasheet;
}

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
        VirtualDatasheetObject* owner = nullptr;

        std::string name;
        DatasheetParser::DataMemberDefinition* dataMemberDefinition = nullptr;
        int64 localizationTimestamp = 0;

        // TODO: I should probably see if I can move this union-like block into a separate struct.
        // - This would make it way cleaner when dealing with array properties.
        // - Step 2 could be that non-array properties are actually an array of size 1, to remove all places with a if(isarray) split.
        bool value_bool = false;
        int value_int = 0;
        float value_float = 0.f;
        std::string value_string;
        Vector2i value_vector2i;
        Vector2f value_vector2f;
        DatasheetParser::ClassDefinition* value_objectInstanceDefinition = nullptr;
        VirtualDatasheetObject* value_objectInstance = nullptr;
        VirtualDatasheet* value_objectReference = nullptr;  // TODO: this is more a helper than an actual serialized value data, the naming should reflect that.
        std::vector<VirtualDatasheetObject::DataValue*> value_children;

        ~DataValue();

        bool MoveChildDataValue(size_t indexFrom, size_t indexTo);
        bool RemoveChildDataValue(size_t index);
    };

public:

    VirtualDatasheetObject();
    ~VirtualDatasheetObject();

    void SetParentObject(VirtualDatasheetObject* parentObject);

    void GetDependencies(std::set<Resource*>& dependencies) const;
    void OnDependencyRemoved(const Resource* removedDependency);

    bool LoadFromXml(const pugi::xml_node& nodeDatasheetObject, VirtualDatasheet* datasheet);
    bool SaveToXml(pugi::xml_node& nodeDatasheet, const std::string& xmlObjectType) const;

    void ResolveInstances(const std::map<UUID, VirtualDatasheetObject*>& dataObjects, std::set<UUID>& orphanObjectUuids);
    void RegenerateInstanceUuidsRecursively();
    void GatherInstanceUuidsRecursively(std::set<UUID>& instanceUuids) const;

    VirtualDatasheetObject::DataValue* InstanciateNewClassMemberDataValue(DatasheetParser::DataMemberDefinition* dataMemberDef);
    VirtualDatasheetObject::DataValue* InstanciateNewArrayMemberDataValue(VirtualDatasheetObject::DataValue* arrayDataMember);
    bool RemoveDataValue(const std::string& name);
    VirtualDatasheetObject::DataValue* GetDataValue(const std::string& name) const;

    void SortDataValues();

protected:

    void GetDependencies(const std::vector<VirtualDatasheetObject::DataValue*>& dataValues, std::set<Resource*>& dependencies) const;
    void OnDependencyRemoved(const Resource* removedDependency, std::vector<VirtualDatasheetObject::DataValue*>& dataValues);

    void ParseDataValue(const pugi::xml_node& nodeData, DatasheetParser::DataMemberDefinition* dataMemberDef, VirtualDatasheetObject::DataValue* dataValue);
    void SaveDataValue(pugi::xml_node& nodeData, DatasheetParser::DataMemberDefinition* dataMemberDef, const VirtualDatasheetObject::DataValue* dataValue) const;

public:

    UUID m_uuid;
    DatasheetParser::ClassDefinition* m_classDefinition;
    std::vector<VirtualDatasheetObject::DataValue*> m_dataValues;

    VirtualDatasheet* m_datasheet;
    VirtualDatasheetObject* m_parentObject;
};

}   // namespace gugu
