#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"
#include "Gugu/Core/Callback.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class ElementWidget;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementSlider : public Element
{
public:

    ElementSlider();
    virtual ~ElementSlider();

    bool LoadFromWidget(const std::string& elementWidgetID);
    bool LoadFromWidget(ElementWidget* elementWidget);

    void SetValueLimits(int minValue, int maxValue);
    void SetValue(int value);
    int GetValue() const;

    void SetDisabled(bool _bDisabled);

    void SetOnValueChanged(const Callback& _pActionOnValueChanged);

protected:

    void OnMousePressed(const InteractionInfos& interactionInfos);
    void OnMouseEntered(const InteractionInfos& interactionInfos);
    void OnMouseLeft(const InteractionInfos& interactionInfos);
    void OnMouseDragMoved(const InteractionInfos& interactionInfos);

    virtual void RecomputeImpl() override;
    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void OnSizeChanged() override;
    
    virtual bool LoadFromDataImpl(ElementDataContext& context) override;

protected:

    Element* m_idleStateComponent;
    Element* m_focusedStateComponent;
    Element* m_disabledStateComponent;
    Element* m_currentStateComponent;
    Element* m_cursorComponent;

    Callback m_actionOnValueChanged;

    bool m_isDisabled;
    int m_minValue;
    int m_maxValue;
    int m_value;
};

}   // namespace gugu
