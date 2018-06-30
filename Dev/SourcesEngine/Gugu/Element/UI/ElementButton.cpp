////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/UI/ElementButton.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Utility/System.h"
#include "Gugu/Utility/Action.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Render/Renderer.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementButton::ElementButton()
{
	m_sprite			= new ElementSprite;
	m_text				= new ElementText;

	m_sprite->SetParent(this, false);
	m_text->SetParent(this, false);

	m_sprite->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

	SetTextAlignment();

    m_textureIdle = nullptr;
    m_textureFocused = nullptr;
    m_textureDisabled = nullptr;

	m_actionOnPressed	= nullptr;
	m_actionOnReleased	= nullptr;
    
    AddInteractionFlag(EInteraction::Focus);
    AddInteractionFlag(EInteraction::Click);

    m_isButtonDisabled = false;
}

ElementButton::~ElementButton()
{
	SafeDelete(m_sprite);
	SafeDelete(m_text);
	SafeDelete(m_actionOnPressed);
	SafeDelete(m_actionOnReleased);
}

void ElementButton::SetTexture(const std::string& _strTexturePathIdle, const std::string& _strTexturePathFocus)
{
    SetTexture(GetResources()->GetTexture(_strTexturePathIdle), GetResources()->GetTexture(_strTexturePathFocus));
}

void ElementButton::SetTexture(Texture* _pTextureIdle, Texture* _pTextureFocused)
{
    if (_pTextureIdle && _pTextureFocused)
    {
        m_textureIdle = _pTextureIdle;
        m_textureFocused = _pTextureFocused;

        m_sprite->SetTexture(m_textureIdle);

        SetSize(m_sprite->GetSize());
    }
}

void ElementButton::SetTextureDisabled(const std::string& _strTexture)
{
    m_textureDisabled = GetResources()->GetTexture(_strTexture);
}

void ElementButton::SetButtonDisabled(bool _bDisabled)
{
    if (m_isButtonDisabled == _bDisabled)
        return;

    m_isButtonDisabled = _bDisabled;

    if (m_isButtonDisabled)
    {
        AddInteractionFlag(EInteraction::Disabled);

        if (m_textureDisabled)
        {
            sf::Vector2f kSize = m_size;
            m_sprite->SetTexture(m_textureDisabled);
            SetSize(kSize);
        }
    }
    else
    {
        RemoveInteractionFlag(EInteraction::Disabled);

        if (m_textureIdle)
        {
            sf::Vector2f kSize = m_size;
            m_sprite->SetTexture(m_textureIdle);
            SetSize(kSize);
        }
    }
}

void ElementButton::SetOnMousePressed(Action* _pActionOnPressed)
{
	SafeDelete(m_actionOnPressed);
	m_actionOnPressed = _pActionOnPressed;
}

void ElementButton::SetOnMouseReleased(Action* _pActionOnReleased)
{
	SafeDelete(m_actionOnReleased);
	m_actionOnReleased = _pActionOnReleased;
}

void ElementButton::SetText(const std::string& _strText)
{
    m_text->SetText(_strText);
}

bool ElementButton::OnMousePressed()
{
	if (m_actionOnPressed)
	{
		m_actionOnPressed->Call();
        return false;
    }

    return true;
}

bool ElementButton::OnMouseReleased()
{
	if (m_actionOnReleased)
	{
        m_actionOnReleased->Call();
        return false;
    }

    return true;
}

void ElementButton::OnMouseEnter()
{
    if (m_textureFocused)
	{
        sf::Vector2f kSize = m_size;
        m_sprite->SetTexture(m_textureFocused);   //TODO: a boolean param to allow the size reset ?
        SetSize(kSize);
	}
}

void ElementButton::OnMouseLeave()
{
    if (m_textureIdle)
    {
        sf::Vector2f kSize = m_size;
        m_sprite->SetTexture(m_textureIdle);
        SetSize(kSize);
	}
}

void ElementButton::DrawSelf(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    sf::FloatRect kGlobalTransformed = _kTransformSelf.transformRect(sf::FloatRect(sf::Vector2f(), m_size));
    if (_kRenderPass.rectViewport.intersects(kGlobalTransformed))
    {
        m_sprite->Render(_kRenderPass, _kTransformSelf);
        m_text->Render(_kRenderPass, _kTransformSelf);
    }
}

void ElementButton::SetSizeImpl(sf::Vector2f _kOldSize)
{
	m_text->ComputeUnifiedDimensions();
	m_sprite->ComputeUnifiedDimensions();
}

void ElementButton::SetTextAlignment(ETextAlignment::Type _eAlignX, float _fOffsetX, ETextAlignment::Type _eAlignY, float _fOffsetY)
{
    UDim2 kDim;

    switch (_eAlignX)
	{
        case ETextAlignment::Left:		kDim.x = UDim::ZERO;	break;
        case ETextAlignment::Center:	kDim.x = UDim::HALF;	break;
        case ETextAlignment::Right:		kDim.x = UDim::FULL;	break;
		default: break;
	}

    switch (_eAlignY)
	{
		case ETextAlignment::Top:		kDim.y = UDim::ZERO;		break;
		case ETextAlignment::Center:	kDim.y = UDim::HALF;		break;
		case ETextAlignment::Bottom:	kDim.y = UDim::FULL;		break;
		default: break;
	}

    m_text->SetUnifiedOrigin(kDim);

    kDim.x += _fOffsetX;
    kDim.y += _fOffsetY;

    m_text->SetUnifiedPosition(kDim);
}

ElementText* ElementButton::GetElementText() const
{
	return m_text;
}

ElementSprite* ElementButton::GetElementSprite() const
{
	return m_sprite;
}

}	// namespace gugu
