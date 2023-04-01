#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
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
    bool UpdateFromInstance(const Element* instance);
    void ResetWidget();

    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;
    virtual bool LoadFromString(const std::string& source) override;

    virtual bool SaveToFile() const override;
    virtual bool SaveToString(std::string& result) const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const override;
    virtual void OnDependencyRemoved(const Resource* removedDependency) override;

protected:

    virtual void Unload() override;

protected:

    // TODO: I could provide an option to avoid caching xml trees (its mostly useful for editor).
    pugi::xml_document* m_cache;
};

}   // namespace gugu
