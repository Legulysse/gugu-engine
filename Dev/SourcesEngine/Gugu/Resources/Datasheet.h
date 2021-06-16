#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

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

struct DatasheetEnum
{
    std::vector<std::string> values;    //TODO: Store both string and enum values
};

class Datasheet : public Resource
{
public:

    Datasheet();
    virtual ~Datasheet();

    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;

    //TODO: Store reference to parent, at least a Getter

protected:

    virtual void ParseMembers(DatasheetParserContext& _kContext);

    //Read basic type single values
    void ReadString         (DatasheetParserContext& _kContext, const std::string& _strName, std::string& _strMember);
    void ReadInt            (DatasheetParserContext& _kContext, const std::string& _strName, int& _iMember);
    void ReadFloat          (DatasheetParserContext& _kContext, const std::string& _strName, float& _fMember);
    void ReadBool           (DatasheetParserContext& _kContext, const std::string& _strName, bool& _bMember);

    //Read basic type array values
    void ReadArrayString    (DatasheetParserContext& _kContext, const std::string& _strName, std::vector<std::string>& _vecMember);
    void ReadArrayInt       (DatasheetParserContext& _kContext, const std::string& _strName, std::vector<int>& _vecMember);
    void ReadArrayFloat     (DatasheetParserContext& _kContext, const std::string& _strName, std::vector<float>& _vecMember);
    void ReadArrayBool      (DatasheetParserContext& _kContext, const std::string& _strName, std::vector<bool>& _vecMember);

    //Read instance (instanced datasheet structure)
    template<typename T>
    void ReadInstance (DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, T*& _pMember)
    {
        Datasheet* _pInstance = nullptr;
        if (InstanciateDatasheet(_kContext, _strName, _strType, _pInstance))
        {
            SafeDelete(_pMember);
            _pMember = dynamic_cast<T*>(_pInstance);
        }
    }

    //Read reference (reference to an other datasheet)
    template<typename T>
    void ReadReference (DatasheetParserContext& _kContext, const std::string& _strName, T*& _pMember)
    {
        Datasheet* _pDatasheet = nullptr;
        if (ResolveDatasheetLink(_kContext, _strName, _pDatasheet))
        {
            _pMember = dynamic_cast<T*>(_pDatasheet);
        }
    }

    //Read Enum value
    template<typename T>
    void ReadEnum (DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, T& _eMember)
    {
        int iValue = 0;
        if (ReadEnumValue(_kContext, _strName, _strType, iValue))
            _eMember = (T)iValue;
    }
    
    //Read instance array (instanced datasheet structures)
    template<typename T>
    void ReadArrayInstance (DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<T*>& _vecMember)
    {
        std::vector<Datasheet*> vecInstances;
        if (InstanciateDatasheets(_kContext, _strName, _strType, vecInstances))
        {
            ClearStdVector(_vecMember);

            for (size_t i = 0; i < vecInstances.size(); ++i)
            {
                T* pInstance = dynamic_cast<T*>(vecInstances[i]);
                if (pInstance)
                    _vecMember.push_back(pInstance);
            }
        }
    }

    //Read reference array (references to other datasheets)
    template<typename T>
    void ReadArrayReference (DatasheetParserContext& _kContext, const std::string& _strName, std::vector<T*>& _vecMember)
    {
        std::vector<Datasheet*> vecDatasheets;
        if (ResolveDatasheetLinks(_kContext, _strName, vecDatasheets))
        {
            _vecMember.clear();

            for (size_t i = 0; i < vecDatasheets.size(); ++i)
            {
                T* pDatasheet = dynamic_cast<T*>(vecDatasheets[i]);
                if (pDatasheet)
                    _vecMember.push_back(pDatasheet);
            }
        }
    }
    
    //Read Enum value array
    template<typename T>
    void ReadArrayEnum (DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<T>& _vecMember)
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
    
    bool        LoadFromDatasheet       (const std::string& _strPathName);

    Datasheet*  InstanciateDatasheet    (DatasheetParserContext& _kContext, const std::string& _strType);
    bool        InstanciateDatasheet    (DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, Datasheet*& _pInstance);
    bool        InstanciateDatasheets   (DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, std::vector<Datasheet*>& _vecInstances);

    Datasheet*  ResolveDatasheetLink    (const std::string& _strName);
    bool        ResolveDatasheetLink    (DatasheetParserContext& _kContext, const std::string& _strName, Datasheet*& _pDatasheet);
    bool        ResolveDatasheetLinks   (DatasheetParserContext& _kContext, const std::string& _strName, std::vector<Datasheet*>& _vecDatasheets);

    bool        ReadEnumValue           (DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, int& _iValue);
    bool        ReadEnumValues          (DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<int>& _vecValues);

    pugi::xml_node  FindNodeData        (DatasheetParserContext& _kContext, const std::string& _strName);
};

}   // namespace gugu
