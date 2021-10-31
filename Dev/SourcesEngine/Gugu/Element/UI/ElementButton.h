#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class ElementText;
    class Texture;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
namespace ETextAlignment {
    enum Type
    {
        Center,
        Left,
        Right,
        Top,
        Bottom,
    };
}

class ElementButton : public Element
{
public:

    ElementButton();
    virtual ~ElementButton();

    [[deprecated("Deprecated, use LoadFromFile() instead.")]]
    void SetTexture(const std::string& textureIdleID, const std::string& textureFocusedID = "", const std::string& textureDisabledID = "");
    [[deprecated("Deprecated, use LoadFromFile() instead.")]]
    void SetTexture(Texture* textureIdle, Texture* textureFocused, Texture* textureDisabled);

    void SetText(const std::string& _strText);

    // Use alignment for origin, and alignment + offset for position.
    void SetTextAlignment(const UDim2& alignment, const Vector2f& offset = Vector2f(0.f, 0.f));

    void SetDisabled(bool _bDisabled);

    void SetOnMousePressed(Callback _pActionOnPressed);
    void SetOnMouseReleased(Callback _pActionOnReleased);

    virtual bool OnMousePressed() override;
    virtual bool OnMouseReleased() override;
    virtual void OnMouseEnter() override;
    virtual void OnMouseLeave() override;

    ElementText* GetElementText() const;

    bool LoadFromFile(const std::string& path);
    virtual bool LoadFromXml(const pugi::xml_node& nodeSelf) override;

protected:

    void SetTextureImpl(Texture* textureIdle, Texture* textureFocused, Texture* textureDisabled);

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void OnSizeChanged(Vector2f _kOldSize) override;
    
protected:

    Element* m_spriteIdle;
    Element* m_spriteFocused;
    Element* m_spriteDisabled;
    Element* m_currentSprite;

    ElementText* m_text;

    Callback m_actionOnPressed;
    Callback m_actionOnReleased;

    bool m_isDisabled;
};

}   // namespace gugu
