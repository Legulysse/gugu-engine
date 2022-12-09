#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"

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
        FixedSize,      //No auto-resize, no auto-scale
        FitSize,        //Recompute size to fit the text, no auto-scale (Default) (use before SetValue)
        FitHeight,      //Recompute height to fit the text, no auto-scale (Use before SetSize)
        FitScale,       //No auto-resize, rescale text to fill (Use before SetSize)

        //TODO: Find a more robust way yo handle initialization :
        //if setsize is called while FitSize is active and text is empty, it will be reset to size=0,
        //and following instructions with FitHeight and SetValue wont be able to resize the Element.
    };
}

class ElementText : public Element
{
public:

    ElementText();
    virtual ~ElementText();

    void SetText(const sf::String& value /* , bool _bResize = true */);
    sf::String GetText() const;

    void SetFont        (const std::string& _strFontPath);
    void SetFont        (Font* _pFont);
    void SetFontSize    (unsigned int _uiSize);
    void SetFontColor   (const sf::Color& _oColor);
    //TODO: Add accessors for all sf::Text methods (LineSpacing, LetterSpacing, Style, etc)

    void SetResizeRule  (ETextResizeRule::Type _eResizeRule);
    void SetMultiline   (bool _bIsMultiline);

protected:

    void Recompute();

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void OnSizeChanged() override;
    
    virtual void OnRecompute() {}
    virtual void OnTextRendered(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) {}

protected:

    Font* m_font;

    sf::Text* m_sfText;
    sf::String m_textValue;

    ETextResizeRule::Type m_resizeRule;
    bool m_isMultiline;
    bool m_skipRecomputeOnResize;

    //std::vector<sf::Text*> m_components;
};

}   // namespace gugu
