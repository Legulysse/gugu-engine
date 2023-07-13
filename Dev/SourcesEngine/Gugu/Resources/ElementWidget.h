#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct ElementDataContext;
    struct ElementPathBindings;
    class Element;
    class BaseElementData;
}

namespace pugi
{
    class xml_document;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementWidget : public Resource
{
public:

    ElementWidget();
    virtual ~ElementWidget();
    
    Element* InstanciateWidget() const;
    Element* InstanciateWidget(ElementPathBindings& pathBindings) const;
    Element* InstanciateWidget(ElementDataContext& context) const;
    bool LoadElementFromWidget(Element* element) const;

    BaseElementData* GetRootData() const;
    void SetRootData(BaseElementData* data, bool deleteData);

    virtual EResourceType::Type GetResourceType() const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const override;
    virtual void OnDependencyRemoved(const Resource* removedDependency) override;

protected:

    virtual void Unload() override;
    virtual bool LoadFromXml(const pugi::xml_document& document) override;
    virtual bool SaveToXml(pugi::xml_document& document) const override;

protected:

    BaseElementData* m_data;
};

}   // namespace gugu
