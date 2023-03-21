////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/ElementWidget.h"

////////////////////////////////////////////////////////////////
// Includes

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementWidget::ElementWidget()
{
}

ElementWidget::~ElementWidget()
{
}

EResourceType::Type ElementWidget::GetResourceType() const
{
    return EResourceType::ElementWidget;
}

void ElementWidget::GetDependencies(std::set<Resource*>& dependencies) const
{
}

void ElementWidget::OnDependencyRemoved(const Resource* removedDependency)
{
}

}   // namespace gugu
