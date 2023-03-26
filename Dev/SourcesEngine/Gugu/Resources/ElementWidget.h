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

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementWidget : public Resource
{
public:

    ElementWidget();
    virtual ~ElementWidget();
    
    Element* InstanciateWidget() const;

    bool SaveInstanceToFile(const Element* instance) const;
    bool LoadInstanceFromString(const std::string& source, Element*& instance);
    bool SaveInstanceToString(const Element* instance, std::string& result) const;

    virtual EResourceType::Type GetResourceType() const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const override;
    virtual void OnDependencyRemoved(const Resource* removedDependency) override;
};

}   // namespace gugu
