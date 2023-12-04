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
    class ElementText;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementCheckbox : public Element
{
public:

    ElementCheckbox();
    virtual ~ElementCheckbox();

    bool LoadFromWidget(const std::string& elementWidgetID);
    bool LoadFromWidget(ElementWidget* elementWidget);

    void SetChecked(bool checked);
    bool IsChecked() const;

    void SetText(const std::string& text, const std::string& fontID = "");

    void SetDisabled(bool _bDisabled);

    void SetOnCheckedChanged(const Callback& _pActionOnCheckedChanged);

protected:

    void OnMousePressed(const InteractionInfos& interactionInfos);
    void OnMouseEntered(const InteractionInfos& interactionInfos);
    void OnMouseLeft(const InteractionInfos& interactionInfos);

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void OnSizeChanged() override;
    
    virtual bool LoadFromDataImpl(ElementDataContext& context) override;

protected:

    Element* m_commonComponent;
    Element* m_idleStateComponent;
    Element* m_focusedStateComponent;
    Element* m_disabledStateComponent;
    Element* m_currentStateComponent;
    Element* m_checkedComponent;
    ElementText* m_text;

    Callback m_actionOnCheckedChanged;

    bool m_isDisabled;
    bool m_isChecked;
};

}   // namespace gugu
