#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/BasePanel.h"

#include "Gugu/Resources/ManagerResources.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DeltaTime;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class DependenciesPanel : public BasePanel
{
public:

    DependenciesPanel();
    virtual ~DependenciesPanel();

    virtual void UpdatePanel(const DeltaTime& dt) override;

private:

    void DisplayResourceDependencies(const ManagerResources::ResourceDependencies& dependencies) const;
};

}   //namespace gugu
