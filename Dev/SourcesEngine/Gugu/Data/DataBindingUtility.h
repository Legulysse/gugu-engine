#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"
#include "Gugu/System/UUID.h"

#include <string>
#include <vector>
#include <map>

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

struct DataParseContext
{
    pugi::xml_node* currentNode;
    const std::map<UUID, DataObject*>* objectByUUID;
};

struct DataSaveContext
{
    pugi::xml_node* currentNode;
};

struct DataEnumInfos
{
    std::vector<std::string> values;    //TODO: Store both string and enum values
};

namespace binding {

//----------------------------------------------
// Read base types

void ReadString(DataParseContext& _kContext, const std::string& _strName, std::string& _strMember);
void ReadInt(DataParseContext& _kContext, const std::string& _strName, int& _iMember);
void ReadFloat(DataParseContext& _kContext, const std::string& _strName, float& _fMember);
void ReadBool(DataParseContext& _kContext, const std::string& _strName, bool& _bMember);

void ReadStringArray(DataParseContext& _kContext, const std::string& _strName, std::vector<std::string>& _vecMember);
void ReadIntArray(DataParseContext& _kContext, const std::string& _strName, std::vector<int>& _vecMember);
void ReadFloatArray(DataParseContext& _kContext, const std::string& _strName, std::vector<float>& _vecMember);
void ReadBoolArray(DataParseContext& _kContext, const std::string& _strName, std::vector<bool>& _vecMember);

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
void ReadEnum(DataParseContext& _kContext, const std::string& _strName, const std::string& _strType, T& _eMember);

template<typename T>
void ReadEnumArray(DataParseContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<T>& _vecMember);

//----------------------------------------------
// Write enums

template<typename T>
void WriteEnum(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, T _eMember);

template<typename T>
void WriteEnumArray(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<T>& _vecMember);

//----------------------------------------------
// Read Vector2

void ReadVector2(DataParseContext& context, const std::string& name, Vector2i& value);
void ReadVector2(DataParseContext& context, const std::string& name, Vector2f& value);

void ReadVector2Array(DataParseContext& context, const std::string& name, std::vector<Vector2i>& values);
void ReadVector2Array(DataParseContext& context, const std::string& name, std::vector<Vector2f>& values);

//----------------------------------------------
// Write Vector2

void WriteVector2(DataSaveContext& context, const std::string& name, const Vector2i& value);
void WriteVector2(DataSaveContext& context, const std::string& name, const Vector2f& value);

void WriteVector2Array(DataSaveContext& context, const std::string& name, const std::vector<Vector2i>& values);
void WriteVector2Array(DataSaveContext& context, const std::string& name, const std::vector<Vector2f>& values);

//----------------------------------------------
// Read datasheet references

template<typename T>
void ReadDatasheetReference(DataParseContext& _kContext, const std::string& _strName, const T*& _pMember);

template<typename T>
void ReadDatasheetReferenceArray(DataParseContext& _kContext, const std::string& _strName, std::vector<const T*>& _vecMember);

//----------------------------------------------
// Write datasheet references

void WriteDatasheetReference(DataSaveContext& _kContext, const std::string& _strName, const DatasheetObject* _pMember);

template<typename T>
void WriteDatasheetReferenceArray(DataSaveContext& _kContext, const std::string& _strName, const std::vector<const T*>& _pMember);

//----------------------------------------------
// Read datasheet instances

template<typename T>
void ReadDatasheetInstance(DataParseContext& _kContext, const std::string& _strName, const std::string& _strType, const T*& _pMember);

template<typename T>
void ReadDatasheetInstanceArray(DataParseContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<const T*>& _vecMember);

//----------------------------------------------
// Read datasave instances

template<typename T>
void ReadDatasaveInstance(DataParseContext& _kContext, const std::string& _strName, const std::string& _strType, T*& _pMember);

template<typename T>
void ReadDatasaveInstanceArray(DataParseContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<T*>& _vecMember);

//----------------------------------------------
// Write datasave instances

void WriteDatasaveInstance(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const DatasaveObject* _pMember);

template<typename T>
void WriteDatasaveInstanceArray(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<T*>& _pMember);

//----------------------------------------------
// impl methods

namespace impl {

pugi::xml_node FindNodeData(DataParseContext& _kContext, const std::string& _strName);
pugi::xml_node AddNodeData(DataSaveContext& _kContext, const std::string& _strName);

bool ReadEnumValue(DataParseContext& _kContext, const std::string& _strName, const std::string& _strType, int& _iValue);
bool ReadEnumValues(DataParseContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<int>& _vecValues);

void WriteEnumValue(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, int _iValue);
void WriteEnumValues(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<int>& _vecValues);

const DatasheetObject* ResolveDatasheetReference(const std::string& _strName);
bool ResolveDatasheetReference(DataParseContext& _kContext, const std::string& _strName, const DatasheetObject*& _pDatasheet);
bool ResolveDatasheetReferences(DataParseContext& _kContext, const std::string& _strName, std::vector<const DatasheetObject*>& _vecDatasheets);

void WriteDatasheetReferences(DataSaveContext& _kContext, const std::string& _strName, const std::vector<const DatasheetObject*>& _pMember);

bool ResolveDatasheetObjectInstance(DataParseContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, DataObject*& _pInstance);
bool ResolveDatasheetObjectInstances(DataParseContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, std::vector<DataObject*>& _vecInstances);

DataObject* InstanciateDatasaveObject(DataParseContext& _kContext, const std::string& _strType);
bool InstanciateDatasaveObject(DataParseContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, DataObject*& _pInstance);
bool InstanciateDatasaveObjects(DataParseContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, std::vector<DataObject*>& _vecInstances);

void WriteDatasaveInstances(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<DatasaveObject*>& _pMember);

}   // namespace impl

}   // namespace binding

}   // namespace gugu

////////////////////////////////////////////////////////////////
// Template Implementation

#include "Gugu/Data/DataBindingUtility.tpp"
