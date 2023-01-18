#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct DatasheetParserContext;
    class Datasheet;
}

namespace pugi
{
    class xml_node;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
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

private:

    const DatasheetObject* InstanciateDatasheetObject(DatasheetParserContext& _kContext, const std::string& _strType);
    bool InstanciateDatasheetObject(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, const DatasheetObject*& _pInstance);
    bool InstanciateDatasheetObjects(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, std::vector<const DatasheetObject*>& _vecInstances);

    const DatasheetObject* ResolveDatasheetLink(const std::string& _strName);
    bool ResolveDatasheetLink(DatasheetParserContext& _kContext, const std::string& _strName, const DatasheetObject*& _pDatasheet);
    bool ResolveDatasheetLinks(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<const DatasheetObject*>& _vecDatasheets);

    pugi::xml_node FindNodeData(DatasheetParserContext& _kContext, const std::string& _strName);

private:

    Datasheet* m_datasheet;
};

}   // namespace gugu
