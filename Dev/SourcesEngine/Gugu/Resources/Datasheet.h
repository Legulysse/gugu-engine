#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Datasheet;
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

struct DatasheetEnum
{
    std::vector<std::string> values;    //TODO: Store both string and enum values
};

class DatasheetObject
{
public:

    DatasheetObject();
    virtual ~DatasheetObject();

    bool LoadFromXml(const std::string& _strPathName, Datasheet* ownerDatasheet, std::vector<class Datasheet*>& ancestors);

    // Return the owning datasheet (only if this object is a root object, return nullptr otherwise).
    Datasheet* GetDatasheet() const;

protected:

    virtual void ParseMembers(DatasheetParserContext& _kContext) = 0;

    //Read basic type single values
    void ReadString(DatasheetParserContext& _kContext, const std::string& _strName, std::string& _strMember);
    void ReadInt(DatasheetParserContext& _kContext, const std::string& _strName, int& _iMember);
    void ReadFloat(DatasheetParserContext& _kContext, const std::string& _strName, float& _fMember);
    void ReadBool(DatasheetParserContext& _kContext, const std::string& _strName, bool& _bMember);

    //Read basic type array values
    void ReadArrayString(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<std::string>& _vecMember);
    void ReadArrayInt(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<int>& _vecMember);
    void ReadArrayFloat(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<float>& _vecMember);
    void ReadArrayBool(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<bool>& _vecMember);

    //Read instance (instanced datasheet structure)
    template<typename T>
    void ReadInstance(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, const T*& _pMember)
    {
        const DatasheetObject* pInstance = nullptr;
        if (InstanciateDatasheetObject(_kContext, _strName, _strType, pInstance))
        {
            SafeDelete(_pMember);
            _pMember = dynamic_cast<const T*>(pInstance);
        }
    }

    //Read reference (reference to an other datasheet)
    template<typename T>
    void ReadReference(DatasheetParserContext& _kContext, const std::string& _strName, const T*& _pMember)
    {
        const DatasheetObject* pReference = nullptr;
        if (ResolveDatasheetLink(_kContext, _strName, pReference))
        {
            _pMember = dynamic_cast<const T*>(pReference);
        }
    }

    //Read Enum value
    template<typename T>
    void ReadEnum(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, T& _eMember)
    {
        int iValue = 0;
        if (ReadEnumValue(_kContext, _strName, _strType, iValue))
            _eMember = (T)iValue;
    }

    //Read instance array (instanced datasheet structures)
    template<typename T>
    void ReadArrayInstance(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<const T*>& _vecMember)
    {
        std::vector<const DatasheetObject*> vecInstances;
        if (InstanciateDatasheetObjects(_kContext, _strName, _strType, vecInstances))
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

    //Read reference array (references to other datasheets)
    template<typename T>
    void ReadArrayReference(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<const T*>& _vecMember)
    {
        std::vector<const DatasheetObject*> vecReferences;
        if (ResolveDatasheetLinks(_kContext, _strName, vecReferences))
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

    //Read Enum value array
    template<typename T>
    void ReadArrayEnum(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<T>& _vecMember)
    {
        std::vector<int> vecValues;
        if (ReadEnumValues(_kContext, _strName, _strType, vecValues))
        {
            _vecMember.clear();

            for (size_t i = 0; i < vecValues.size(); ++i)
            {
                _vecMember.push_back((T)(vecValues[i]));
            }
        }
    }

private:

    const DatasheetObject* InstanciateDatasheetObject(DatasheetParserContext& _kContext, const std::string& _strType);
    bool InstanciateDatasheetObject(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, const DatasheetObject*& _pInstance);
    bool InstanciateDatasheetObjects(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, std::vector<const DatasheetObject*>& _vecInstances);

    const DatasheetObject* ResolveDatasheetLink(const std::string& _strName);
    bool ResolveDatasheetLink(DatasheetParserContext& _kContext, const std::string& _strName, const DatasheetObject*& _pDatasheet);
    bool ResolveDatasheetLinks(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<const DatasheetObject*>& _vecDatasheets);

    bool ReadEnumValue(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, int& _iValue);
    bool ReadEnumValues(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<int>& _vecValues);

    pugi::xml_node FindNodeData(DatasheetParserContext& _kContext, const std::string& _strName);

private:

    Datasheet* m_datasheet;
};


class Datasheet : public Resource
{
public:

    Datasheet();
    virtual ~Datasheet();

    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;

    //TODO: Store reference to parent, at least a Getter
    const DatasheetObject* GetRootObject() const;

protected:

    virtual void Unload() override;

private:

    DatasheetObject* m_rootObject;
};

}   // namespace gugu
