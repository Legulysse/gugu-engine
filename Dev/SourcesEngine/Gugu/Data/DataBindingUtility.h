#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DataObject;
    class DatasheetObject;
    class DatasaveObject;
}

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

//----------------------------------------------
// Read base types

void ReadString(DatasheetParserContext& _kContext, const std::string& _strName, std::string& _strMember);
void ReadInt(DatasheetParserContext& _kContext, const std::string& _strName, int& _iMember);
void ReadFloat(DatasheetParserContext& _kContext, const std::string& _strName, float& _fMember);
void ReadBool(DatasheetParserContext& _kContext, const std::string& _strName, bool& _bMember);

void ReadStringArray(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<std::string>& _vecMember);
void ReadIntArray(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<int>& _vecMember);
void ReadFloatArray(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<float>& _vecMember);
void ReadBoolArray(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<bool>& _vecMember);

//----------------------------------------------
// Write base types

void WriteString(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strMember);
void WriteInt(DataSaveContext& _kContext, const std::string& _strName, int _iMember);
void WriteFloat(DataSaveContext& _kContext, const std::string& _strName, float _fMember);
void WriteBool(DataSaveContext& _kContext, const std::string& _strName, bool _bMember);

void WriteStringArray(DataSaveContext& _kContext, const std::string& _strName, const std::vector<std::string>& _vecMember);
void WriteIntArray(DataSaveContext& _kContext, const std::string& _strName, const std::vector<int>& _vecMember);
void WriteFloatArray(DataSaveContext& _kContext, const std::string& _strName, const std::vector<float>& _vecMember);
void WriteBoolArray(DataSaveContext& _kContext, const std::string& _strName, const std::vector<bool>& _vecMember);

//----------------------------------------------
// Read enums

template<typename T>
void ReadEnum(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, T& _eMember);

template<typename T>
void ReadEnumArray(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<T>& _vecMember);

//----------------------------------------------
// Write enums

template<typename T>
void WriteEnum(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, T _eMember);

template<typename T>
void WriteEnumArray(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<T>& _vecMember);

//----------------------------------------------
// Read datasheet references

template<typename T>
void ReadDatasheetReference(DatasheetParserContext& _kContext, const std::string& _strName, const T*& _pMember);

template<typename T>
void ReadDatasheetReferenceArray(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<const T*>& _vecMember);

//----------------------------------------------
// Write datasheet references

void WriteDatasheetReference(DataSaveContext& _kContext, const std::string& _strName, const DatasheetObject* _pMember);

template<typename T>
void WriteDatasheetReferenceArray(DataSaveContext& _kContext, const std::string& _strName, const std::vector<const T*>& _pMember);

//----------------------------------------------
// Read datasheet instances

template<typename T>
void ReadDatasheetInstance(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, const T*& _pMember);

template<typename T>
void ReadDatasheetInstanceArray(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<const T*>& _vecMember);

//----------------------------------------------
// Read datasave instances

template<typename T>
void ReadDatasaveInstance(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, T*& _pMember);

template<typename T>
void ReadDatasaveInstanceArray(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<T*>& _vecMember);

//----------------------------------------------
// Write datasave instances

void WriteDatasaveInstance(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const DatasaveObject* _pMember);

template<typename T>
void WriteDatasaveInstanceArray(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<T*>& _pMember);

//----------------------------------------------
// Impl methods

namespace Impl {

pugi::xml_node FindNodeData(DatasheetParserContext& _kContext, const std::string& _strName);
pugi::xml_node AddNodeData(DataSaveContext& _kContext, const std::string& _strName);

bool ReadEnumValue(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, int& _iValue);
bool ReadEnumValues(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<int>& _vecValues);

void WriteEnumValue(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, int _iValue);
void WriteEnumValues(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<int>& _vecValues);

const DatasheetObject* ResolveDatasheetLink(const std::string& _strName);
bool ResolveDatasheetLink(DatasheetParserContext& _kContext, const std::string& _strName, const DatasheetObject*& _pDatasheet);
bool ResolveDatasheetLinks(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<const DatasheetObject*>& _vecDatasheets);

DataObject* InstanciateDataObject(DatasheetParserContext& _kContext, const std::string& _strType);
bool InstanciateDataObject(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, DataObject*& _pInstance);
bool InstanciateDataObjects(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, std::vector<DataObject*>& _vecInstances);

void WriteDatasheetReferences(DataSaveContext& _kContext, const std::string& _strName, const std::vector<const DatasheetObject*>& _pMember);
void WriteDatasaveInstances(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<DatasaveObject*>& _pMember);

}   // namespace Impl

}   // namespace DataBinding

}   // namespace gugu

////////////////////////////////////////////////////////////////
// Template Implementation

#include "Gugu/Data/DataBindingUtility.tpp"
