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
    class Texture;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementButton : public Element
{
public:

    ElementButton();
    virtual ~ElementButton();

    bool LoadFromWidget(const std::string& elementWidgetID);
    bool LoadFromWidget(ElementWidget* elementWidget);

    [[deprecated("Deprecated, use LoadFromWidget() instead.")]]
    void SetTexture(const std::string& textureIdleID, const std::string& textureFocusedID = "", const std::string& textureDisabledID = "");
    [[deprecated("Deprecated, use LoadFromWidget() instead.")]]
    void SetTexture(Texture* textureIdle, Texture* textureFocused, Texture* textureDisabled);

    void SetText(const std::string& text, const std::string& fontID = "");

    void SetDisabled(bool _bDisabled);

    void SetOnMousePressed(const Callback& _pActionOnPressed);
    void SetOnMouseReleased(const Callback& _pActionOnReleased);

protected:

    void OnMousePressed(const InteractionInfos& interactionInfos);
    void OnMouseReleased(const InteractionInfos& interactionInfos);
    void OnMouseEntered(const InteractionInfos& interactionInfos);
    void OnMouseLeft(const InteractionInfos& interactionInfos);

    void SetTextureImpl(Texture* textureIdle, Texture* textureFocused, Texture* textureDisabled);

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void OnSizeChanged() override;
    
    void DeleteComponents();
    virtual bool LoadFromDataImpl(ElementDataContext& context) override;

protected:

    Element* m_commonComponent;
    Element* m_idleStateComponent;
    Element* m_focusedStateComponent;
    Element* m_disabledStateComponent;
    Element* m_currentStateComponent;
    ElementText* m_text;

    Callback m_actionOnPressed;
    Callback m_actionOnReleased;

    bool m_isDisabled;
};

}   // namespace gugu
