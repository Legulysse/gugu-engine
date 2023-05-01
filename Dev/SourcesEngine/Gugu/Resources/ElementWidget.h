#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct ElementDataBindings;
    class Element;
    class ElementData;
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
    Element* InstanciateWidget(ElementDataBindings* bindings) const;
    ElementData* GetData() const;

    virtual EResourceType::Type GetResourceType() const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const override;
    virtual void OnDependencyRemoved(const Resource* removedDependency) override;

protected:

    virtual void Unload() override;
    virtual bool LoadFromXml(const pugi::xml_document& document) override;
    virtual bool SaveToXml(pugi::xml_document& document) const override;

protected:

    ElementData* m_data;
};

}   // namespace gugu
