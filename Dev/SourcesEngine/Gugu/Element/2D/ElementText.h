#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"

#include <SFML/System/String.hpp>
#include <SFML/Graphics/Color.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Font;
}

namespace sf
{
    class Text;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
namespace ETextResizeRule
{
    enum Type
    {
        FixedSize,      // No auto-resize, no auto-scale.
        FitSize,        // Recompute size to fit the text (Default).
        FitWidth,       // Recompute width to fit the text.
        FitHeight,      // Recompute height to fit the text.
        FitScale,       // Rescale text to fill the element size.
    };
}

class ElementText : public Element
{
public:

    ElementText();
    virtual ~ElementText();

    void SetText(const std::string& value /* , bool _bResize = true */);
    sf::String GetText() const;

    void SetFont(const std::string& _strFontPath);
    void SetFont(Font* _pFont);
    void SetFontSize(uint32 fontSize);
    void SetColor(const sf::Color& _oColor);
    void SetOutlineColor(const sf::Color& color);
    void SetOutlineThickness(float thickness);
    //TODO: Add accessors for all sf::Text methods (LineSpacing, LetterSpacing, Style, etc)

    void SetResizeRule  (ETextResizeRule::Type _eResizeRule);
    void SetMultiline   (bool _bIsMultiline);

protected:

    virtual void RecomputeImpl() override;
    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void OnSizeChanged() override;

    virtual bool LoadFromDataImpl(ElementDataContext& context) override;

    virtual void OnTextRendered(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) {}

protected:

    Font* m_font;
    sf::String m_textValue;
    ETextResizeRule::Type m_resizeRule;
    bool m_isMultiline;

    sf::Text* m_sfText;
    bool m_skipRecomputeOnResize;

    //std::vector<sf::Text*> m_components;
};

}   // namespace gugu
