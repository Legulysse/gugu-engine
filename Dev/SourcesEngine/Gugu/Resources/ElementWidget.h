#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementWidget : public Resource
{
public:

    ElementWidget();
    virtual ~ElementWidget();
    
    virtual EResourceType::Type GetResourceType() const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const override;
    virtual void OnDependencyRemoved(const Resource* removedDependency) override;
};

}   // namespace gugu
