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
        std::string value;  // TODO: union ?
        std::vector<VirtualDatasheetObject::DataValue*> children;

        ~DataValue();
    };

public:

    VirtualDatasheetObject();
    ~VirtualDatasheetObject();

    bool LoadFromXml(const pugi::xml_node& nodeDatasheetObject, DatasheetParser::ClassDefinition* definition);

public:

    DatasheetParser::ClassDefinition* classDefinition;
    std::vector<VirtualDatasheetObject::DataValue*> dataValues;
};

class VirtualDatasheet : public Resource
{
public:

    VirtualDatasheet();
    virtual ~VirtualDatasheet();

    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;

public:

    DatasheetParser::ClassDefinition* classDefinition;
    VirtualDatasheetObject* rootObject;
};

}   // namespace gugu
