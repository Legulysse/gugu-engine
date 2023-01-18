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
void WriteArrayEnum(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<T>& _vecMember)
{
    std::vector<int> vecValues;
    vecValues.reserve(_vecMember.size());

    for (size_t i = 0; i < _vecMember.size(); ++i)
    {
        vecValues.push_back((int)_vecMember[i]);
    }

    Impl::WriteEnumValues(_kContext, _strName, _strType, vecValues);
}

namespace Impl {

pugi::xml_node FindNodeData(DatasheetParserContext& _kContext, const std::string& _strName);
pugi::xml_node AddNodeData(DataSaveContext& _kContext, const std::string& _strName);

bool ReadEnumValue(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, int& _iValue);
bool ReadEnumValues(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<int>& _vecValues);

void WriteEnumValue(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, int _iValue);
void WriteEnumValues(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<int>& _vecValues);

}   // namespace Impl

}   // namespace DataBinding

}   // namespace gugu
