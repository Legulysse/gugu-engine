#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

namespace DataBinding {

template<typename T>
void ReadEnum(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, T& _eMember)
{
    int iValue = 0;
    if (Impl::ReadEnumValue(_kContext, _strName, _strType, iValue))
        _eMember = (T)iValue;
}

template<typename T>
void ReadEnumArray(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<T>& _vecMember)
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
void WriteEnumArray(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<T>& _vecMember)
{
    std::vector<int> vecValues;
    vecValues.reserve(_vecMember.size());

    for (size_t i = 0; i < _vecMember.size(); ++i)
    {
        vecValues.push_back((int)_vecMember[i]);
    }

    Impl::WriteEnumValues(_kContext, _strName, _strType, vecValues);
}

template<typename T>
void ReadDatasheetReference(DatasheetParserContext& _kContext, const std::string& _strName, const T*& _pMember)
{
    static_assert(std::is_base_of<DatasheetObject, T>::value, "Data type is not based on DatasheetObject type");

    const DatasheetObject* pReference = nullptr;
    if (Impl::ResolveDatasheetLink(_kContext, _strName, pReference))
    {
        _pMember = dynamic_cast<const T*>(pReference);
    }
}

template<typename T>
void ReadDatasheetReferenceArray(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<const T*>& _vecMember)
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

template<typename T>
void WriteDatasheetReferenceArray(DataSaveContext& _kContext, const std::string& _strName, const std::vector<const T*>& _pMember)
{
    static_assert(std::is_base_of<DatasheetObject, T>::value, "Data type is not based on DatasheetObject type");

    std::vector<const DatasheetObject*> vecReferences;
    vecReferences.reserve(_pMember.size());

    for (size_t i = 0; i < _pMember.size(); ++i)
    {
        vecReferences.push_back(_pMember[i]);
    }

    Impl::WriteDatasheetReferences(_kContext, _strName, vecReferences);
}

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

template<typename T>
void WriteDatasaveInstanceArray(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<T*>& _pMember)
{
    static_assert(std::is_base_of<DatasaveObject, T>::value, "Data type is not based on DatasaveObject type");

    std::vector<DatasaveObject*> vecInstances;
    vecInstances.reserve(_pMember.size());

    for (size_t i = 0; i < _pMember.size(); ++i)
    {
        vecInstances.push_back((DatasaveObject*)_pMember[i]);
    }

    Impl::WriteDatasaveInstances(_kContext, _strName, _strType, vecInstances);
}

}   // namespace DataBinding

}   // namespace gugu
