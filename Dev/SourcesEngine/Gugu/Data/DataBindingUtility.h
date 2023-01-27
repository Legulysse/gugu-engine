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

void ReadString(DatasheetParserContext& _kContext, const std::string& _strName, std::string& _strMember);
void ReadInt(DatasheetParserContext& _kContext, const std::string& _strName, int& _iMember);
void ReadFloat(DatasheetParserContext& _kContext, const std::string& _strName, float& _fMember);
void ReadBool(DatasheetParserContext& _kContext, const std::string& _strName, bool& _bMember);

void ReadArrayString(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<std::string>& _vecMember);
void ReadArrayInt(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<int>& _vecMember);
void ReadArrayFloat(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<float>& _vecMember);
void ReadArrayBool(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<bool>& _vecMember);

void WriteString(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strMember);
void WriteInt(DataSaveContext& _kContext, const std::string& _strName, int _iMember);
void WriteFloat(DataSaveContext& _kContext, const std::string& _strName, float _fMember);
void WriteBool(DataSaveContext& _kContext, const std::string& _strName, bool _bMember);

void WriteArrayString(DataSaveContext& _kContext, const std::string& _strName, const std::vector<std::string>& _vecMember);
void WriteArrayInt(DataSaveContext& _kContext, const std::string& _strName, const std::vector<int>& _vecMember);
void WriteArrayFloat(DataSaveContext& _kContext, const std::string& _strName, const std::vector<float>& _vecMember);
void WriteArrayBool(DataSaveContext& _kContext, const std::string& _strName, const std::vector<bool>& _vecMember);

template<typename T>
void ReadEnum(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, T& _eMember)
{
    int iValue = 0;
    if (Impl::ReadEnumValue(_kContext, _strName, _strType, iValue))
        _eMember = (T)iValue;
}

template<typename T>
void ReadArrayEnum(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<T>& _vecMember)
{
    std::vector<int> vecValues;
    if (Impl::ReadEnumValues(_kContext, _strName, _strType, vecValues))
    {
        _vecMember.clear();

        for (size_t i = 0; i < vecValues.size(); ++i)
        {
            _vecMember.push_back((T)(vecValues[i]));
        }
    }
}

template<typename T>
void WriteEnum(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, T _eMember)
{
    Impl::WriteEnumValue(_kContext, _strName, _strType, (int)_eMember);
}

template<typename T>
void WriteArrayEnum(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<T>& _vecMember)
{
    std::vector<int> vecValues;
    vecValues.reserve(_vecMember.size());

    for (size_t i = 0; i < _vecMember.size(); ++i)
    {
        vecValues.push_back((int)_vecMember[i]);
    }

    Impl::WriteEnumValues(_kContext, _strName, _strType, vecValues);
}

//Read reference (reference to an other datasheet)
template<typename T>
void ReadReference(DatasheetParserContext& _kContext, const std::string& _strName, const T*& _pMember)
{
    static_assert(std::is_base_of<DatasheetObject, T>::value, "Data type is not based on DatasheetObject type");

    const DatasheetObject* pReference = nullptr;
    if (Impl::ResolveDatasheetLink(_kContext, _strName, pReference))
    {
        _pMember = dynamic_cast<const T*>(pReference);
    }
}

//Read reference array (references to other datasheets)
template<typename T>
void ReadArrayReference(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<const T*>& _vecMember)
{
    static_assert(std::is_base_of<DatasheetObject, T>::value, "Data type is not based on DatasheetObject type");

    std::vector<const DatasheetObject*> vecReferences;
    if (Impl::ResolveDatasheetLinks(_kContext, _strName, vecReferences))
    {
        _vecMember.clear();

        for (size_t i = 0; i < vecReferences.size(); ++i)
        {
            // Fill the actual member values (may contain null values).
            const T* pReference = dynamic_cast<const T*>(vecReferences[i]);
            _vecMember.push_back(pReference);
        }
    }
}

void WriteReference(DataSaveContext& _kContext, const std::string& _strName, const DatasheetObject* _pMember);

template<typename T>
void WriteArrayReference(DataSaveContext& _kContext, const std::string& _strName, const std::vector<const T*>& _pMember)
{
    static_assert(std::is_base_of<DatasheetObject, T>::value, "Data type is not based on DatasheetObject type");

    std::vector<const DatasheetObject*> vecReferences;
    vecReferences.reserve(_pMember.size());

    for (size_t i = 0; i < _pMember.size(); ++i)
    {
        vecReferences.push_back(_pMember[i]);
    }

    WriteArrayReference(_kContext, _strName, vecReferences);
}

void WriteArrayReference(DataSaveContext& _kContext, const std::string& _strName, const std::vector<const DatasheetObject*>& _pMember);

template<typename T>
void ReadDatasheetInstance(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, const T*& _pMember)
{
    static_assert(std::is_base_of<DatasheetObject, T>::value, "Data type is not based on DatasheetObject type");

    DataObject* pInstance = nullptr;
    if (Impl::InstanciateDataObject(_kContext, _strName, _strType, pInstance))
    {
        SafeDelete(_pMember);
        _pMember = dynamic_cast<const T*>(pInstance);
    }
}

template<typename T>
void ReadDatasheetInstanceArray(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<const T*>& _vecMember)
{
    static_assert(std::is_base_of<DatasheetObject, T>::value, "Data type is not based on DatasheetObject type");

    std::vector<DataObject*> vecInstances;
    if (Impl::InstanciateDataObjects(_kContext, _strName, _strType, vecInstances))
    {
        ClearStdVector(_vecMember);

        for (size_t i = 0; i < vecInstances.size(); ++i)
        {
            // Fill the actual member values (may contain null values).
            const T* pInstance = dynamic_cast<const T*>(vecInstances[i]);
            _vecMember.push_back(pInstance);
        }
    }
}

template<typename T>
void ReadDatasaveInstance(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, T*& _pMember)
{
    static_assert(std::is_base_of<DatasaveObject, T>::value, "Data type is not based on DatasheetObject type");

    DataObject* pInstance = nullptr;
    if (Impl::InstanciateDataObject(_kContext, _strName, _strType, pInstance))
    {
        SafeDelete(_pMember);
        _pMember = dynamic_cast<T*>(pInstance);
    }
}

template<typename T>
void ReadDatasaveInstanceArray(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<T*>& _vecMember)
{
    static_assert(std::is_base_of<DatasaveObject, T>::value, "Data type is not based on DatasheetObject type");

    std::vector<DataObject*> vecInstances;
    if (Impl::InstanciateDataObjects(_kContext, _strName, _strType, vecInstances))
    {
        ClearStdVector(_vecMember);

        for (size_t i = 0; i < vecInstances.size(); ++i)
        {
            // Fill the actual member values (may contain null values).
            T* pInstance = dynamic_cast<T*>(vecInstances[i]);
            _vecMember.push_back(pInstance);
        }
    }
}

void WriteInstance(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const DatasaveObject* _pMember);

template<typename T>
void WriteArrayInstance(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<T*>& _pMember)
{
    static_assert(std::is_base_of<DatasaveObject, T>::value, "Data type is not based on DatasaveObject type");

    std::vector<DatasaveObject*> vecInstances;
    vecInstances.reserve(_pMember.size());

    for (size_t i = 0; i < _pMember.size(); ++i)
    {
        vecInstances.push_back((DatasaveObject*)_pMember[i]);
    }

    WriteInstances(_kContext, _strName, _strType, vecInstances);
}

void WriteInstances(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<DatasaveObject*>& _pMember);

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

}   // namespace Impl

}   // namespace DataBinding

}   // namespace gugu
