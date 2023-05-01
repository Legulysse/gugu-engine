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
    class ElementData;
    class ElementSpriteGroupData;
    class ElementSpriteGroupItemData;
    class Element;
    class ElementSpriteGroup;
    class ElementSpriteGroupItem;
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

    void DisplayTreeNode(ElementData* node, int itemFlags, ElementData*& deleted);
    void HandleContextMenu(ElementData* node, ElementData*& deleted);
    ElementData* DisplayElementInstanciationContextMenu();

    void ClearHierarchy();
    void RebuildHierarchy();
    void SelectRootNode();

    void DisplayGenerators(ElementSpriteGroupData* elementSpriteGroupData, ElementSpriteGroup* elementSpriteGroup);

    void AddChildElement(ElementData* parentData, ElementData* elementData);
    void AddChildElement(ElementData* parentData, ElementData* elementData, size_t index);
    void InsertElement(ElementData* referenceData, ElementData* elementData);
    ElementSpriteGroupItem* AppendNewComponent(ElementSpriteGroupData* groupData, ElementSpriteGroupItemData* componentData);
    void DeleteElement(ElementData* elementData);

private:

    RenderViewport* m_renderViewport;
    float m_zoomFactor;

    ElementWidget* m_elementWidget;
    ElementDataBindings* m_dataBindings;
    ElementData* m_widgetRootData;
    Element* m_widgetRootElement;
    ElementData* m_selectedElementData;
    Element* m_selectedElement;

    size_t m_generatorIndex;
    std::string m_topLeftName, m_topName, m_topRightName, m_leftName, m_centerName, m_rightName, m_bottomLeftName, m_bottomName, m_bottomRightName;
    sf::IntRect m_topLeftRect, m_topRect, m_topRightRect, m_leftRect, m_centerRect, m_rightRect, m_bottomLeftRect, m_bottomRect, m_bottomRightRect;
    bool m_tileTopSegment, m_tileLeftSegment, m_tileRightSegment, m_tileBottomSegment;
};

}   //namespace gugu
