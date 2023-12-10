#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/Document/DocumentPanel.h"

#include <SFML/Graphics/Rect.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct ElementDataBindings;
    class DeltaTime;
    class RenderViewport;
    class ElementWidget;
    class BaseElementData;
    class ElementData;
    class ElementCompositeData;
    class ElementSpriteGroupData;
    class ElementSpriteGroupItemData;
    class Element;
    class ElementSpriteGroup;
    class ElementSpriteGroupItem;
    class ElementSFDrawable;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementWidgetPanel : public DocumentPanel
{
public:

    ElementWidgetPanel(ElementWidget* ressource);
    virtual ~ElementWidgetPanel();

private:

    virtual void OnUndoRedo() override;

    virtual void UpdatePanelImpl(const DeltaTime& dt) override;
    virtual void UpdateHierarchyImpl(const DeltaTime& dt) override;
    virtual void UpdatePropertiesImpl(const DeltaTime& dt) override;

    void DisplayTreeNode(BaseElementData* node, int itemFlags, BaseElementData*& deleted);
    void HandleContextMenu(BaseElementData* node, BaseElementData*& deleted);
    BaseElementData* DisplayElementInstanciationContextMenu();

    void ClearHierarchy();
    void RebuildHierarchy();
    void SelectRootNode();

    void DisplayGenerators(ElementSpriteGroupData* elementSpriteGroupData, ElementSpriteGroup* elementSpriteGroup);

    void AddChildElement(BaseElementData* parentData, BaseElementData* newData);
    void AddChildElement(BaseElementData* parentData, BaseElementData* newData, size_t index);
    void InsertElement(BaseElementData* referenceData, BaseElementData* newData);
    Element* AddComponent(ElementCompositeData* compositeData, ElementData* componentData);
    void DeleteElement(BaseElementData* elementData);

    void CopyElementToClipboard(BaseElementData* elementData);
    void PasteElementFromClipboard(BaseElementData* parentData, bool asComponent);
    void PasteElementFromClipboard(BaseElementData* parentData, bool asComponent, size_t index);

    void CreateGizmo();
    void UpdateGizmo();

private:

    RenderViewport* m_renderViewport;
    Element* m_viewportRoot;
    float m_zoomFactor;
    bool m_showOrigin;
    bool m_showBounds;
    bool m_showAllBounds;

    ElementWidget* m_elementWidget;
    ElementDataBindings* m_dataBindings;
    BaseElementData* m_widgetRootData;
    Element* m_widgetRootElement;
    BaseElementData* m_selectedElementData;
    Element* m_selectedElement;

    ElementSFDrawable* m_gizmoOrigin;
    ElementSFDrawable* m_gizmoBounds;

    size_t m_generatorIndex;
    std::string m_topLeftName, m_topName, m_topRightName, m_leftName, m_centerName, m_rightName, m_bottomLeftName, m_bottomName, m_bottomRightName;
    sf::IntRect m_topLeftRect, m_topRect, m_topRightRect, m_leftRect, m_centerRect, m_rightRect, m_bottomLeftRect, m_bottomRect, m_bottomRightRect;
    bool m_tileTopSegment, m_tileLeftSegment, m_tileRightSegment, m_tileBottomSegment;
};

}   //namespace gugu
