////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/2D/ElementText.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Manager/ManagerConfig.h"
#include "Gugu/Resources/Font.h"
#include "Gugu/Render/Renderer.h"
#include "Gugu/Utility/System.h"
#include "Gugu/Utility/Action.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementText::ElementText()
: m_font(nullptr)
{
	m_textValue	= "";

    m_resizeRule   = ETextResizeRule::FitSize;
	m_isMultiline	= false;
	m_isEditable	= false;

    m_skipRecomputeOnResize = false;
    m_isEditing		= false;
    m_isTickDisplayed	= false;
    m_timeSinceTick	= 0.f;

    m_actionOnValidate = nullptr;

    m_showDebugBounds       = false;

	m_sfText = new sf::Text;
	m_sfText->setFillColor(sf::Color(0, 0, 0));

	m_sfTextCursor = nullptr;

	SetFont(GetResources()->GetDefaultFont());
	SetFontSize(20);
}

ElementText::~ElementText()
{
	SafeDelete(m_actionOnValidate);

	SafeDelete(m_sfTextCursor);
	SafeDelete(m_sfText);

	ClearStdVector(m_components);
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
            AddInteractionFlag(EInteraction::Selection);
        }
        else
        {
            RemoveInteractionFlag(EInteraction::Selection);

            StopEditionImpl();
        }
    }
}

void ElementText::SetDebugBoundsVisible(bool _bShowDebugBounds)
{
    m_showDebugBounds = _bShowDebugBounds;
}

void ElementText::SetText(const std::string& _strValue /*, bool _bResize */)
{
	m_textValue = _strValue;
    
	Recompute();

	//if (/*_bResize && */ !m_bIsMultiline)
    /*if (m_eResizeRule == TextResizeRule::FitSize)
    {
		SetSize(sf::Vector2f(m_pSFText->GetRect().Width, m_pSFText->GetRect().Height));
    }*/
}

std::string ElementText::GetValue() const
{
    return m_textValue;
}

void ElementText::Recompute()
{
    std::string	strTextImpl = "";

	if (!m_isMultiline)
	{
		strTextImpl = m_textValue;
	}
	else
	{
		ClearStdVector(m_components);

		//std::string strNewText = "";	//Used to build the new StringImpl
		sf::Text oSFText;				//Temp element to compute string size on screen
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
		while (iLineNext < m_textValue.size())
		{
			//Step 1 : remove whitespaces on beginning (not any more !)
			//iLineBegin = iLineNext; //m_strValue.find_first_not_of(" ", iLineNext);
			iLineBegin = m_textValue.find_first_not_of(" ", iLineNext);
			if (iLineBegin == std::string::npos)
			    iLineBegin = iLineNext;
			iNbChars = 0;

			//Step 2 : parse chunks of text
			iChunkNext = iLineBegin;

			std::string strLineTest = "";
			std::string strNewLine	= "";
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
					iChunkEnd = m_textValue.find_first_of(" ", iSearchBegin + 1);	//end of normal string (+1 is important to manage whitespaces between chunks)

                    size_t iNext = (iChunkEnd == std::string::npos) ? std::string::npos : m_textValue.find_first_not_of(" ", iChunkEnd);		//next chunk (can be one of the cases below)
					size_t iForceEndLine = m_textValue.find_first_of("\n", iSearchBegin);	//possible cut
					size_t iSpecialChar	= m_textValue.find_first_of(".,;:?!", iSearchBegin);	//possible extension

                    if (iForceEndLine != std::string::npos && (iChunkEnd == std::string::npos || iForceEndLine < iChunkEnd))
					{
						iChunkEnd = iForceEndLine;
						iChunkNext = iForceEndLine + 1;
						iNbChars = iChunkEnd - iChunkBegin;

						bLineFinished = true;		//stop parsing chunks for this line
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
                        iNbChars = (iChunkEnd == std::string::npos) ? m_textValue.size() - iChunkBegin : iChunkEnd - iChunkBegin;

                        bChunkFinished = true;
					}
                }

				//Step 2:1 : compute a text chunk
				strLineTest += m_textValue.substr(iChunkBegin, iNbChars);

				//Step 2:2 : compute new size. if size is too big, skip this chunk and go to step 3
				oSFText.setString(strLineTest);
				if (oSFText.getLocalBounds().width > GetSize().x)
				{
					if (!strNewLine.empty())
					{
						iLineNext = iChunkBegin;		//Not enough place on this line, this chunk will be read on next line
					}
					else
					{
						//Special case : chunk is too big, but line is empty, so force the display of the chunk as a new line
						strNewLine = strLineTest;
						iLineNext = iChunkEnd;
					}

					bLineFinished = true;		//stop parsing chunks for this line
				}
				else
				{
					strNewLine = strLineTest;

					if (iChunkNext == std::string::npos)
					{
						iLineNext = std::string::npos;
						bLineFinished = true;	//end of text
					}
				}
			}

			//Step 3 : add the new line and loop back
			strTextImpl += strNewLine + "\n";
		}

		strTextImpl = strTextImpl.substr(0, strTextImpl.size()-1);	//remove last EndLine
	}

	m_sfText->setString(strTextImpl);

    //TODO: Here I can split my sf::Text into an array (1 line = 1 sf::Text) to allow center-alignment & line visibility.
    
    //TODO: Is this better for Vertical size ?
    //float fLineHeight = 0.f;
    //if (m_pSFText->getFont())
    //    fLineHeight = m_pSFText->getFont()->getLineSpacing(m_pSFText->getCharacterSize());

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

        m_sfTextCursor->setPosition(m_sfText->getTransform().transformPoint(m_sfText->findCharacterPos(strTextImpl.size())));
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
    m_timeSinceTick	= 0.f;
    m_isTickDisplayed	= true;
    m_isEditing		= true;
    
    if (!m_sfTextCursor)
    {
        m_sfTextCursor = new sf::Text;
        m_sfTextCursor->setString("|");
    }
            
    AddInteractionFlag(EInteraction::RawSFEvent);
    
	Recompute();
}

void ElementText::StopEditionImpl()
{
    m_timeSinceTick	= 0.f;
    m_isTickDisplayed	= false;
    m_isEditing		= false;

    RemoveInteractionFlag(EInteraction::RawSFEvent);
    
	Recompute();
}

void ElementText::Update(const DeltaTime& dt)
{
    if (m_isEditing)
    {
        m_timeSinceTick += dt.s();

        if(m_timeSinceTick >= 0.75f)
        {
            m_timeSinceTick = 0.f;
            m_isTickDisplayed = !m_isTickDisplayed;
        }
    }
}

bool ElementText::OnMouseSelected()
{
    StartEditionImpl();
    return true;
}

bool ElementText::OnMouseDeselected()
{
    StopEditionImpl();
    return true;
}

bool ElementText::OnSFEvent(const sf::Event& _oSFEvent)
{
	if(!m_isEditing || _oSFEvent.type != sf::Event::KeyPressed)
        return true;

    if (_oSFEvent.key.code == sf::Keyboard::Return && !m_isMultiline && m_actionOnValidate)
    {
        m_actionOnValidate->Call();
        return false;
    }

    if(_oSFEvent.key.code == sf::Keyboard::BackSpace)
	{
		if(!m_textValue.empty())
	    {
            m_textValue.erase(m_textValue.length()-1, 1);
            SetText(m_textValue /*, false*/);

            return false;
	    }
	}
	else
	{
        char cNewChar = '\0';

        if (_oSFEvent.key.code >= sf::Keyboard::A && _oSFEvent.key.code <= sf::Keyboard::Z)
        {
            cNewChar = (char)('a' + (_oSFEvent.key.code - sf::Keyboard::A));

            if(GetConfig()->IsShiftDown())
            {
                cNewChar += 'A' - 'a';
            }
        }
        else if (_oSFEvent.key.code >= sf::Keyboard::Num0 && _oSFEvent.key.code <= sf::Keyboard::Num9)
        {
            cNewChar = (char)('0' + (_oSFEvent.key.code - sf::Keyboard::Num0));
        }
        else if (_oSFEvent.key.code >= sf::Keyboard::Numpad0 && _oSFEvent.key.code <= sf::Keyboard::Numpad9)
        {
            cNewChar = (char)('0' + (_oSFEvent.key.code - sf::Keyboard::Numpad0));
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

            m_textValue.insert(m_textValue.length(), strNewChar);
            SetText(m_textValue /*, false */);

            return false;
        }
	}

    return true;
}

void ElementText::SetOnValidate(Action* _pAction)
{
    SafeDelete(m_actionOnValidate);

    m_actionOnValidate = _pAction;
}

void ElementText::DrawSelf(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    sf::FloatRect kGlobalTransformed = _kTransformSelf.transformRect(sf::FloatRect(sf::Vector2f(), m_size));
    if (_kRenderPass.rectViewport.intersects(kGlobalTransformed))
    {
        {
            _kRenderPass.target->draw(*m_sfText, _kTransformSelf);

            //Stats
            if (_kRenderPass.frameInfos)
                _kRenderPass.frameInfos->statDrawCalls += 1;

            _kRenderPass.statRenderedTexts += 1;
        }

        //Debug Bounds
        if (m_showDebugBounds)
        {
            sf::RectangleShape oBorder;
            oBorder.setSize(m_size);
            oBorder.setOutlineColor(sf::Color::Magenta);
            oBorder.setOutlineThickness(1.f);
            oBorder.setFillColor(sf::Color::Transparent);
            _kRenderPass.target->draw(oBorder, _kTransformSelf);

            //Stats
            if (_kRenderPass.frameInfos)
                _kRenderPass.frameInfos->statDrawCalls += 1;
        }

        //TODO: This is currently unused !
        for (size_t i = 0; i < m_components.size(); ++i)
        {
            _kRenderPass.target->draw(*m_components[i], _kTransformSelf);

            //Stats
            if (_kRenderPass.frameInfos)
                _kRenderPass.frameInfos->statDrawCalls += 1;
        }

        if (m_isTickDisplayed)
        {
            _kRenderPass.target->draw(*m_sfTextCursor, _kTransformSelf);

            //Stats
            if (_kRenderPass.frameInfos)
                _kRenderPass.frameInfos->statDrawCalls += 1;
        }
    }
}

void ElementText::SetSizeImpl(sf::Vector2f _kOldSize)
{
    if (!m_skipRecomputeOnResize)
	    Recompute();
    m_skipRecomputeOnResize = false;
}

}	// namespace gugu
