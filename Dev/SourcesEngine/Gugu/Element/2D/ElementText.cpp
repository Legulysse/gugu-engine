////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/2D/ElementText.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Font.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/System/SystemUtility.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementText::ElementText()
: m_font(nullptr)
{
    m_textValue = "";

    m_resizeRule   = ETextResizeRule::FitSize;
    m_isMultiline   = false;
    m_isEditable    = false;

    m_skipRecomputeOnResize = false;
    m_isEditing     = false;
    m_isTickDisplayed   = false;
    m_timeSinceTick = 0.f;

    m_callbackOnValidate = nullptr;

    m_sfText = new sf::Text;
    m_sfText->setFillColor(sf::Color(0, 0, 0));

    m_sfTextCursor = nullptr;

    SetFont(GetResources()->GetDefaultFont());
    SetFontSize(20);

    GetInteractions()->AddCallback(EElementInteractionEvent::MouseSelected, std::bind(&ElementText::OnMouseSelected, this));
    GetInteractions()->AddCallback(EElementInteractionEvent::MouseDeselected, std::bind(&ElementText::OnMouseDeselected, this));
    GetInteractions()->AddCallback(EElementInteractionEvent::RawSFEvent, std::bind(&ElementText::OnSFEvent, this, std::placeholders::_1));

    GetInteractions()->SetInteractionEnabled(EElementInteraction::Selection, false);
    GetInteractions()->SetInteractionEnabled(EElementInteraction::RawSFEvent, false);
}

ElementText::~ElementText()
{
    SafeDelete(m_sfTextCursor);
    SafeDelete(m_sfText);

    //ClearStdVector(m_components);
}

void ElementText::SetFont(const std::string& _strFontPath)
{
    SetFont(GetResources()->GetFont(_strFontPath));
}

void ElementText::SetFont(Font* _pFont)
{
    if (_pFont)
    {
        m_font = _pFont;
        m_sfText->setFont(*m_font->GetSFFont());
    
        Recompute();
    }
}

void ElementText::SetFontSize(uint32 _uiSize)
{
    m_sfText->setCharacterSize(_uiSize);
    
    Recompute();
}

void ElementText::SetFontColor(const sf::Color& _oColor)
{
    m_sfText->setFillColor(_oColor);
}

void ElementText::SetResizeRule(ETextResizeRule::Type _eResizeRule)
{
    m_resizeRule = _eResizeRule;
    
    Recompute();
}

void ElementText::SetMultiline(bool _bIsMultiline)
{
    m_isMultiline = _bIsMultiline;
    
    Recompute();
}

void ElementText::SetEditable(bool _bIsEditable)
{
    if (m_isEditable != _bIsEditable)
    {
        m_isEditable = _bIsEditable;

        if (m_isEditable)
        {
            GetInteractions()->SetInteractionEnabled(EElementInteraction::Selection, true);
        }
        else
        {
            GetInteractions()->SetInteractionEnabled(EElementInteraction::Selection, false);

            StopEditionImpl();
        }
    }
}

void ElementText::SetText(const sf::String& value /*, bool _bResize */)
{
    if (m_textValue != value)
    {
        m_textValue = value;

        Recompute();
    }

    //if (/*_bResize && */ !m_bIsMultiline)
    /*if (m_eResizeRule == TextResizeRule::FitSize)
    {
        SetSize(Vector2f(m_pSFText->GetRect().Width, m_pSFText->GetRect().Height));
    }*/
}

sf::String ElementText::GetText() const
{
    return m_textValue;
}

void ElementText::Recompute()
{
    size_t textSize = 0;

    if (!m_isMultiline)
    {
        m_sfText->setString(m_textValue);
        textSize = m_textValue.getSize();
    }
    else
    {
        //ClearStdVector(m_components);

        std::string strTextSource = m_textValue;
        std::string strTextImpl = "";

        //std::string strNewText = "";  //Used to build the new StringImpl
        sf::Text oSFText;               //Temp element to compute string size on screen
        oSFText.setFont(*m_sfText->getFont());
        oSFText.setCharacterSize(m_sfText->getCharacterSize());
        oSFText.setStyle(m_sfText->getStyle());

        size_t iLineNext = 0;
        size_t iLineBegin = 0;
        size_t iChunkNext = 0;
        size_t iChunkBegin = 0;
        size_t iChunkEnd = 0;
        size_t iNbChars = 0;

        //Each loop turn will generate a text line
        while (iLineNext < strTextSource.size())
        {
            //TODO: Refacto (the cast from sf::string to std::string is probably breaking things)
            //TODO: maybe check how imgui wraps text ?

            //Step 1 : remove whitespaces on beginning (not any more !)
            //iLineBegin = iLineNext; //m_strValue.find_first_not_of(" ", iLineNext);
            iLineBegin = strTextSource.find_first_not_of(" ", iLineNext);
            if (iLineBegin == std::string::npos)
                iLineBegin = iLineNext;
            iNbChars = 0;

            //Step 2 : parse chunks of text
            iChunkNext = iLineBegin;

            std::string strLineTest = "";
            std::string strNewLine  = "";
            bool bLineFinished = false;

            //Each loop will try to fill the current line with one more word, and eventual ponctuation behind
            while (!bLineFinished)
            {
                iChunkBegin = iChunkNext;
                iChunkEnd = std::string::npos;

                //we have the beginning of the chunk, let's find its end
                size_t iSearchBegin = iChunkBegin;
                bool bChunkFinished = false;

                while (!bChunkFinished)
                {
                    iChunkEnd = strTextSource.find_first_of(" ", iSearchBegin + 1);   //end of normal string (+1 is important to manage whitespaces between chunks)

                    size_t iNext = (iChunkEnd == std::string::npos) ? std::string::npos : strTextSource.find_first_not_of(" ", iChunkEnd);        //next chunk (can be one of the cases below)
                    size_t iForceEndLine = strTextSource.find_first_of("\n", iSearchBegin);   //possible cut
                    size_t iSpecialChar = strTextSource.find_first_of(".,;:?!", iSearchBegin);    //possible extension

                    if (iForceEndLine != std::string::npos && (iChunkEnd == std::string::npos || iForceEndLine < iChunkEnd))
                    {
                        iChunkEnd = iForceEndLine;
                        iChunkNext = iForceEndLine + 1;
                        iNbChars = iChunkEnd - iChunkBegin;

                        bLineFinished = true;       //stop parsing chunks for this line
                        iLineNext = iChunkNext;

                        bChunkFinished = true;
                    }
                    else if (iNext != std::string::npos && iNext == iSpecialChar)
                    {
                        iSearchBegin = iSpecialChar;
                    }
                    else
                    {
                        iChunkNext = iChunkEnd;
                        iNbChars = (iChunkEnd == std::string::npos) ? strTextSource.size() - iChunkBegin : iChunkEnd - iChunkBegin;

                        bChunkFinished = true;
                    }
                }

                //Step 2:1 : compute a text chunk
                strLineTest += strTextSource.substr(iChunkBegin, iNbChars);

                //Step 2:2 : compute new size. if size is too big, skip this chunk and go to step 3
                oSFText.setString(strLineTest);
                if (oSFText.getLocalBounds().width > GetSize().x)
                {
                    if (!strNewLine.empty())
                    {
                        iLineNext = iChunkBegin;        //Not enough place on this line, this chunk will be read on next line
                    }
                    else
                    {
                        //Special case : chunk is too big, but line is empty, so force the display of the chunk as a new line
                        strNewLine = strLineTest;
                        iLineNext = iChunkEnd;
                    }

                    bLineFinished = true;       //stop parsing chunks for this line
                }
                else
                {
                    strNewLine = strLineTest;

                    if (iChunkNext == std::string::npos)
                    {
                        iLineNext = std::string::npos;
                        bLineFinished = true;   //end of text
                    }
                }
            }

            //Step 3 : add the new line and loop back
            strTextImpl += strNewLine + "\n";
        }

        // Remove last endline (we always add some during the line cut) unless it already exists in the source text.
        if (!strTextImpl.empty() && !strTextSource.empty() && strTextSource[strTextSource.size() - 1] != '\n')
        {
            strTextImpl = strTextImpl.substr(0, strTextImpl.size() - 1);
        }

        m_sfText->setString(strTextImpl);
        textSize = strTextImpl.size();
    }

    //TODO: Here I can split my sf::Text into an array (1 line = 1 sf::Text) to allow center-alignment & line visibility.
    
    //TODO: Is this better for Vertical size ?
    //float fLineHeight = 0.f;
    //if (m_pSFText->getFont())
    //    fLineHeight = m_pSFText->getFont()->getLineSpacing(m_pSFText->getCharacterSize());

    //TODO: if the string is empty, we cant select it for edition since the size is (0,0).
    if (m_resizeRule == ETextResizeRule::FitScale && m_size.x > 0 && m_size.y > 0 && m_sfText->getLocalBounds().width > 0 && m_sfText->getLocalBounds().height > 0)
    {
        m_sfText->setScale(m_size.x / (m_sfText->getLocalBounds().left + m_sfText->getLocalBounds().width), m_size.y / (m_sfText->getLocalBounds().top + m_sfText->getLocalBounds().height));
    }
    else if (m_resizeRule == ETextResizeRule::FitSize)
    {
        m_skipRecomputeOnResize = true;
        SetSize(m_sfText->getLocalBounds().left + m_sfText->getLocalBounds().width, m_sfText->getLocalBounds().top + m_sfText->getLocalBounds().height);
    }
    else if (m_resizeRule == ETextResizeRule::FitHeight)
    {
        m_skipRecomputeOnResize = true;
        SetSizeY(m_sfText->getLocalBounds().top + m_sfText->getLocalBounds().height);
    }

    if (m_isEditing)
    {
        //TODO: Some fonts may not have the '|' character
        m_sfTextCursor->setFont(*m_sfText->getFont());
        m_sfTextCursor->setCharacterSize(m_sfText->getCharacterSize());
        m_sfTextCursor->setStyle(m_sfText->getStyle());
        m_sfTextCursor->setFillColor(m_sfText->getFillColor());
        m_sfTextCursor->setScale(m_sfText->getScale());

        m_sfTextCursor->setPosition(m_sfText->getTransform().transformPoint(m_sfText->findCharacterPos(textSize)));
    }
}

void ElementText::StartEdition()
{
    StartEditionImpl();
}

void ElementText::StopEdition()
{
    StopEditionImpl();
}

void ElementText::StartEditionImpl()
{
    m_timeSinceTick = 0.f;
    m_isTickDisplayed   = true;
    m_isEditing     = true;
    
    if (!m_sfTextCursor)
    {
        m_sfTextCursor = new sf::Text;
        m_sfTextCursor->setString("|");
    }

    GetInteractions()->SetInteractionEnabled(EElementInteraction::RawSFEvent, true);
    
    Recompute();
}

void ElementText::StopEditionImpl()
{
    m_timeSinceTick = 0.f;
    m_isTickDisplayed   = false;
    m_isEditing     = false;

    GetInteractions()->SetInteractionEnabled(EElementInteraction::RawSFEvent, false);
    
    Recompute();
}

void ElementText::OnMouseSelected()
{
    StartEditionImpl();
}

void ElementText::OnMouseDeselected()
{
    StopEditionImpl();
}

void ElementText::OnSFEvent(const ElementInteractionInfos& interactionInfos)
{
    if (!m_isEditing)
        return;     // return true;

    return ProcessSFEvent(*interactionInfos.rawSFEvent);
}

void ElementText::ProcessSFEvent(const sf::Event& _oSFEvent)
{
    if (!m_isEditing)
        return;     // return true;

    if (_oSFEvent.type == sf::Event::KeyPressed)
    {
        if (_oSFEvent.key.code == sf::Keyboard::Enter)
        {
            if (!m_isMultiline)
            {
                StopEditionImpl();

                if (m_callbackOnValidate)
                    m_callbackOnValidate();

                return;     // return false;
            }
            else
            {
                m_textValue += '\n';
                Recompute();
                return;     // return false;
            }
        }
        else if (_oSFEvent.key.code == sf::Keyboard::Backspace)
        {
            if (!m_textValue.isEmpty())
            {
                m_textValue.erase(m_textValue.getSize() - 1, 1);
                Recompute();
                return;     // return false;
            }
        }
    }
    else if (_oSFEvent.type == sf::Event::TextEntered)
    {
        if (std::isprint(_oSFEvent.text.unicode))
        {
            m_textValue += _oSFEvent.text.unicode;
            Recompute();
            return;     // return false;
        }
#if 0
        char cNewChar = '\0';

        if (_oSFEvent.key.code >= sf::Keyboard::A && _oSFEvent.key.code <= sf::Keyboard::Z)
        {
            cNewChar = (char)('a' + (_oSFEvent.key.code - sf::Keyboard::A));

            if (_oSFEvent.key.shift)
                cNewChar += 'A' - 'a';
        }
        else if (_oSFEvent.key.code >= sf::Keyboard::Num0 && _oSFEvent.key.code <= sf::Keyboard::Num9)
        {
            cNewChar = (char)('0' + (_oSFEvent.key.code - sf::Keyboard::Num0));
        }
        else if (_oSFEvent.key.code >= sf::Keyboard::Numpad0 && _oSFEvent.key.code <= sf::Keyboard::Numpad9)
        {
            cNewChar = (char)('0' + (_oSFEvent.key.code - sf::Keyboard::Numpad0));
        }
        else if (_oSFEvent.key.code == sf::Keyboard::Add)
        {
            cNewChar = '+';
        }
        else if (_oSFEvent.key.code == sf::Keyboard::Subtract)
        {
            cNewChar = '-';
        }
        else if (_oSFEvent.key.code == sf::Keyboard::Multiply)
        {
            cNewChar = '*';
        }
        else if (_oSFEvent.key.code == sf::Keyboard::Divide)
        {
            cNewChar = '/';
        }
        else if (_oSFEvent.key.code == sf::Keyboard::Period)
        {
            cNewChar = '.';
        }
        else if (_oSFEvent.key.code == sf::Keyboard::Space)
        {
            cNewChar = ' ';
        }
        else if (_oSFEvent.key.code == sf::Keyboard::Return && m_isMultiline)
        {
            cNewChar = '\n';
        }

        if (cNewChar != '\0')
        {
            std::string strNewChar;
            strNewChar = cNewChar;

            m_textValue.insert(m_textValue.getSize(), strNewChar);
            SetText(m_textValue /*, false */);

            return false;
        }
#endif
    }

    return;     // return true;
}

void ElementText::SetOnValidate(const Callback& callbackOnValidate)
{
    m_callbackOnValidate = callbackOnValidate;
}

void ElementText::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    //TODO: register the element to an Update call.
    //if (m_isEditing)
    //{
    //    m_timeSinceTick += dt.s();

    //    if (m_timeSinceTick >= 0.75f)
    //    {
    //        m_timeSinceTick = 0.f;
    //        m_isTickDisplayed = !m_isTickDisplayed;
    //    }
    //}

    sf::FloatRect kGlobalTransformed = _kTransformSelf.transformRect(sf::FloatRect(Vector2::Zero_f, m_size));
    if (_kRenderPass.rectViewport.intersects(kGlobalTransformed))
    {
        {
            _kRenderPass.target->draw(*m_sfText, _kTransformSelf);

            //Stats
            if (_kRenderPass.frameInfos)
            {
                _kRenderPass.frameInfos->statDrawCalls += 1;
                // TODO: find  away to retrieve triangles count.
            }

            _kRenderPass.statRenderedTexts += 1;
        }

        //for (size_t i = 0; i < m_components.size(); ++i)
        //{
        //    _kRenderPass.target->draw(*m_components[i], _kTransformSelf);

        //    //Stats
        //    if (_kRenderPass.frameInfos)
        //        _kRenderPass.frameInfos->statDrawCalls += 1;
        //}

        if (m_isTickDisplayed)
        {
            _kRenderPass.target->draw(*m_sfTextCursor, _kTransformSelf);

            //Stats
            if (_kRenderPass.frameInfos)
            {
                _kRenderPass.frameInfos->statDrawCalls += 1;
                // TODO: find  away to retrieve triangles count.
            }
        }
    }
}

void ElementText::OnSizeChanged()
{
    if (!m_skipRecomputeOnResize)
        Recompute();
    m_skipRecomputeOnResize = false;
}

}   // namespace gugu
