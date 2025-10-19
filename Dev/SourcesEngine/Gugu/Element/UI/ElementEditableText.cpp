////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/UI/ElementEditableText.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Events/ElementEventHandler.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/System/Memory.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementEditableText::ElementEditableText()
    : m_sfTextCursor(nullptr)
    , m_callbackOnValidate(nullptr)
    , m_isEditable(false)
    , m_isEditing(false)
    , m_isTickDisplayed(false)
    , m_timeSinceTick(0.f)
{
    GetEvents()->AddCallback(EInteractionEvent::MouseSelected, STD_BIND(&ElementEditableText::OnMouseSelected, this));
    GetEvents()->AddCallback(EInteractionEvent::MouseDeselected, STD_BIND(&ElementEditableText::OnMouseDeselected, this));
    GetEvents()->AddCallback(EInteractionEvent::RawSFEvent, STD_BIND_1(&ElementEditableText::OnSFEvent, this));

    GetEvents()->SetInteractionDisabled(EInteractionType::Selection, true);
    GetEvents()->SetInteractionDisabled(EInteractionType::RawSFEvent, true);
}

ElementEditableText::~ElementEditableText()
{
    SafeDelete(m_sfTextCursor);
}

void ElementEditableText::SetEditable(bool _bIsEditable)
{
    if (m_isEditable != _bIsEditable)
    {
        m_isEditable = _bIsEditable;

        if (m_isEditable)
        {
            GetEvents()->SetInteractionDisabled(EInteractionType::Selection, false);
        }
        else
        {
            GetEvents()->SetInteractionDisabled(EInteractionType::Selection, true);

            StopEditionImpl();
        }
    }
}

void ElementEditableText::StartEdition()
{
    StartEditionImpl();
}

void ElementEditableText::StopEdition()
{
    StopEditionImpl();
}

void ElementEditableText::StartEditionImpl()
{
    m_timeSinceTick = 0.f;
    m_isTickDisplayed   = true;
    m_isEditing     = true;
    
    if (!m_sfTextCursor)
    {
        //TODO: Some fonts may not have the '|' character.
        m_sfTextCursor = new sf::Text(m_sfText->getFont());
        m_sfTextCursor->setString("|");
    }

    GetEvents()->SetInteractionDisabled(EInteractionType::RawSFEvent, false);
    
    RaiseNeedRecompute();
}

void ElementEditableText::StopEditionImpl()
{
    m_timeSinceTick = 0.f;
    m_isTickDisplayed   = false;
    m_isEditing     = false;

    GetEvents()->SetInteractionDisabled(EInteractionType::RawSFEvent, true);
    
    RaiseNeedRecompute();
}

void ElementEditableText::OnMouseSelected()
{
    StartEditionImpl();
}

void ElementEditableText::OnMouseDeselected()
{
    StopEditionImpl();
}

void ElementEditableText::OnSFEvent(const InteractionInfos& interactionInfos)
{
    if (!m_isEditing)
        return;     // return true;

    return ProcessSFEvent(*interactionInfos.rawSFEvent);
}

void ElementEditableText::ProcessSFEvent(const sf::Event& event)
{
    if (!m_isEditing)
        return;     // return true;

    if (const auto keyPressedEvent = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressedEvent->scancode == sf::Keyboard::Scan::Enter)
        {
            if (!m_isMultiline)
            {
                StopEditionImpl();

                // TODO: notify event handler that we should no more be kept as the active selection.

                if (m_callbackOnValidate)
                    m_callbackOnValidate();

                return;     // return false;
            }
            else
            {
                m_textValue += '\n';
                RaiseNeedRecompute();
                return;     // return false;
            }
        }
        else if (keyPressedEvent->scancode == sf::Keyboard::Scan::Backspace)
        {
            if (!m_textValue.isEmpty())
            {
                m_textValue.erase(m_textValue.getSize() - 1, 1);
                RaiseNeedRecompute();
                return;     // return false;
            }
        }
    }
    else if (const auto textEnteredEvent = event.getIf<sf::Event::TextEntered>())
    {
        if (std::isprint(textEnteredEvent->unicode))
        {
            m_textValue += textEnteredEvent->unicode;
            RaiseNeedRecompute();
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

void ElementEditableText::SetOnValidate(const Callback& callbackOnValidate)
{
    m_callbackOnValidate = callbackOnValidate;
}

void ElementEditableText::RecomputeImpl()
{
    ElementText::RecomputeImpl();

    if (m_isEditing)
    {
        size_t textSize = m_textValue.getSize();

        //TODO: Some fonts may not have the '|' character.
        m_sfTextCursor->setFont(m_sfText->getFont());
        m_sfTextCursor->setCharacterSize(m_sfText->getCharacterSize());
        m_sfTextCursor->setStyle(m_sfText->getStyle());
        m_sfTextCursor->setFillColor(m_sfText->getFillColor());
        m_sfTextCursor->setScale(m_sfText->getScale());

        m_sfTextCursor->setPosition(m_sfText->getTransform().transformPoint(m_sfText->findCharacterPos(textSize)));
    }
}

void ElementEditableText::OnTextRendered(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    //TODO: register the element to an Update call ? add a delta time into RenderPass ?
    //if (m_isEditing)
    //{
    //    m_timeSinceTick += dt.s();

    //    if (m_timeSinceTick >= 0.75f)
    //    {
    //        m_timeSinceTick = 0.f;
    //        m_isTickDisplayed = !m_isTickDisplayed;
    //    }
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

}   // namespace gugu
