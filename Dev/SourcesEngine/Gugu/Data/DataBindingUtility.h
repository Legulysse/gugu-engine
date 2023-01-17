#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace pugi
{
    class xml_node;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct DatasheetParserContext
{
    pugi::xml_node* currentNode;
};

struct DataSaveContext
{
    pugi::xml_node* currentNode;
};

struct DatasheetEnum
{
    std::vector<std::string> values;    //TODO: Store both string and enum values
};

namespace DataBinding {

pugi::xml_node FindNodeData(DatasheetParserContext& _kContext, const std::string& _strName);
pugi::xml_node AddNodeData(DataSaveContext& _kContext, const std::string& _strName);

void ReadString(DatasheetParserContext& _kContext, const std::string& _strName, std::string& _strMember);
void ReadInt(DatasheetParserContext& _kContext, const std::string& _strName, int& _iMember);
void ReadFloat(DatasheetParserContext& _kContext, const std::string& _strName, float& _fMember);
void ReadBool(DatasheetParserContext& _kContext, const std::string& _strName, bool& _bMember);

void ReadArrayString(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<std::string>& _vecMember);
void ReadArrayInt(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<int>& _vecMember);
void ReadArrayFloat(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<float>& _vecMember);
void ReadArrayBool(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<bool>& _vecMember);

void WriteInt(DataSaveContext& _kContext, const std::string& _strName, int _iMember);

}   // namespace DataBinding

}   // namespace gugu
