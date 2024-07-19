////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/2D/ElementText.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/ElementData.h"
#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Font.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/System/Memory.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementText::ElementText()
    : m_font(nullptr)
    , m_sfText(nullptr)
    , m_resizeRule(ETextResizeRule::FitSize)
    , m_isMultiline(false)
    , m_skipRecomputeOnResize(false)
{
    m_sfText = new sf::Text;
    m_sfText->setFillColor(sf::Color(0, 0, 0));

    SetFont(GetResources()->GetDefaultFont());
    SetFontSize(20);    // Duplicate in ElementTextData declaration.
}

ElementText::~ElementText()
{
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
    
        RaiseNeedRecompute();
    }
}

void ElementText::SetFontSize(uint32 fontSize)
{
    m_sfText->setCharacterSize(fontSize);
    
    RaiseNeedRecompute();
}

void ElementText::SetFontColor(const sf::Color& _oColor)
{
    m_sfText->setFillColor(_oColor);
}

void ElementText::SetResizeRule(ETextResizeRule::Type _eResizeRule)
{
    m_resizeRule = _eResizeRule;
    
    RaiseNeedRecompute();
}

void ElementText::SetMultiline(bool _bIsMultiline)
{
    m_isMultiline = _bIsMultiline;
    
    RaiseNeedRecompute();
}

void ElementText::SetText(const std::string& value /*, bool _bResize */)
{
    sf::String textValue = sf::String::fromUtf8(value.begin(), value.end());
    if (m_textValue != textValue)
    {
        m_textValue = textValue;

        RaiseNeedRecompute();
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

void ElementText::RecomputeImpl()
{
    if (!m_isMultiline)
    {
        m_sfText->setString(m_textValue);
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
    }

    //TODO: Here I can split my sf::Text into an array (1 line = 1 sf::Text) to allow center-alignment & line visibility.
    
    //TODO: Is this better for Vertical size ?
    //float fLineHeight = 0.f;
    //if (m_pSFText->getFont())
    //    fLineHeight = m_pSFText->getFont()->getLineSpacing(m_pSFText->getCharacterSize());

    //TODO: if the string is empty, we cant select it for edition since the size is (0,0).
    if (m_resizeRule == ETextResizeRule::FitScale)
    {
        if (m_size.x > 0 && m_size.y > 0 && m_sfText->getLocalBounds().width > 0 && m_sfText->getLocalBounds().height > 0)
        {
            m_sfText->setScale(m_size.x / (m_sfText->getLocalBounds().left + m_sfText->getLocalBounds().width), m_size.y / (m_sfText->getLocalBounds().top + m_sfText->getLocalBounds().height));
        }
    }
    else if (m_resizeRule == ETextResizeRule::FitSize)
    {
        m_skipRecomputeOnResize = true;
        SetSize(m_sfText->getLocalBounds().left + m_sfText->getLocalBounds().width, m_sfText->getLocalBounds().top + m_sfText->getLocalBounds().height);
    }
    else if (m_resizeRule == ETextResizeRule::FitWidth)
    {
        m_skipRecomputeOnResize = true;
        SetSizeX(m_sfText->getLocalBounds().left + m_sfText->getLocalBounds().width);
    }
    else if (m_resizeRule == ETextResizeRule::FitHeight)
    {
        m_skipRecomputeOnResize = true;
        SetSizeY(m_sfText->getLocalBounds().top + m_sfText->getLocalBounds().height);
    }
}

void ElementText::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    sf::FloatRect kGlobalTransformed = _kTransformSelf.transformRect(sf::FloatRect(Vector2::Zero_f, m_size));
    if (_kRenderPass.rectViewport.intersects(kGlobalTransformed))
    {
        {
            _kRenderPass.target->draw(*m_sfText, _kTransformSelf);

            //Stats
            if (_kRenderPass.frameInfos)
            {
                _kRenderPass.frameInfos->statDrawCalls += 1;
                // TODO: find a way to retrieve triangles count.
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

        OnTextRendered(_kRenderPass, _kTransformSelf);
    }
}

void ElementText::OnSizeChanged()
{
    if (!m_skipRecomputeOnResize)
    {
        RaiseNeedRecompute();
    }
    else
    {
        m_skipRecomputeOnResize = false;
    }
}

bool ElementText::LoadFromDataImpl(ElementDataContext& context)
{
    if (!Element::LoadFromDataImpl(context))
        return false;

    ElementTextData* textData = dynamic_cast<ElementTextData*>(context.data);

    SetFont(textData->font);

    if (textData->fontSize != 0)
    {
        SetFontSize(textData->fontSize);
    }

    SetMultiline(textData->multiline);
    SetText(textData->text);

    return true;
}

}   // namespace gugu
