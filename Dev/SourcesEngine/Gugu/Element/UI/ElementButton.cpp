////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/UI/ElementButton.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Events/ElementEventHandler.h"
#include "Gugu/Element/ElementData.h"
#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSpriteGroup.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementButton::ElementButton()
    : m_commonComponent(nullptr)
    , m_idleStateComponent(nullptr)
    , m_focusedStateComponent(nullptr)
    , m_disabledStateComponent(nullptr)
    , m_currentStateComponent(nullptr)
    , m_actionOnPressed(nullptr)
    , m_actionOnReleased(nullptr)
    , m_isDisabled(false)
{
    GetEvents()->AddCallback(EInteractionEvent::MouseEntered, STD_BIND_1(&ElementButton::OnMouseEntered, this));
    GetEvents()->AddCallback(EInteractionEvent::MouseLeft, STD_BIND_1(&ElementButton::OnMouseLeft, this));
    GetEvents()->AddCallback(EInteractionEvent::MousePressed, STD_BIND_1(&ElementButton::OnMousePressed, this));
    GetEvents()->AddCallback(EInteractionEvent::MouseReleased, STD_BIND_1(&ElementButton::OnMouseReleased, this));
}

ElementButton::~ElementButton()
{
    m_currentStateComponent = nullptr;

    SafeDelete(m_idleStateComponent);
    SafeDelete(m_focusedStateComponent);
    SafeDelete(m_disabledStateComponent);
    SafeDelete(m_commonComponent);
}

bool ElementButton::LoadFromWidget(const std::string& elementWidgetID)
{
    return LoadFromWidget(GetResources()->GetElementWidget(elementWidgetID));
}

bool ElementButton::LoadFromWidget(ElementWidget* elementWidget)
{
    if (elementWidget)
    {
        ElementDataContext context;
        context.data = elementWidget->GetData();
        return LoadFromData(context);
    }

    return false;
}

void ElementButton::SetTexture(const std::string& textureIdleID, const std::string& textureFocusedID, const std::string& textureDisabledID)
{
    SetTextureImpl(GetResources()->GetTexture(textureIdleID),
        textureFocusedID.empty() ? nullptr : GetResources()->GetTexture(textureFocusedID),
        textureDisabledID.empty() ? nullptr : GetResources()->GetTexture(textureDisabledID));
}

void ElementButton::SetTexture(Texture* textureIdle, Texture* textureFocused, Texture* textureDisabled)
{
    SetTextureImpl(textureIdle, textureFocused, textureDisabled);
}

void ElementButton::SetTextureImpl(Texture* textureIdle, Texture* textureFocused, Texture* textureDisabled)
{
    if (!textureIdle)
        return;

    ElementSprite* spriteIdle = new ElementSprite;
    spriteIdle->SetTexture(textureIdle);
    spriteIdle->SetUnifiedSize(UDim2::SIZE_FULL);
    spriteIdle->SetParent(this);

    m_idleStateComponent = spriteIdle;

    if (textureFocused)
    {
        ElementSprite* spriteFocused = new ElementSprite;
        spriteFocused->SetTexture(textureFocused);
        spriteFocused->SetUnifiedSize(UDim2::SIZE_FULL);
        spriteFocused->SetParent(this);

        m_focusedStateComponent = spriteFocused;
    }

    if (textureDisabled)
    {
        ElementSprite* spriteDisabled = new ElementSprite;
        spriteDisabled->SetTexture(textureDisabled);
        spriteDisabled->SetUnifiedSize(UDim2::SIZE_FULL);
        spriteDisabled->SetParent(this);

        m_disabledStateComponent = spriteDisabled;
    }

    SetSize(Vector2f(textureIdle->GetSize()));

    m_currentStateComponent = m_idleStateComponent;
}

void ElementButton::SetText(const std::string& value, const std::string& fontID)
{
    // TODO: This is a safety for incomplete button widgets, it could be rethink along with deprecated SetTexture methods.
    if (!m_commonComponent)
    {
        m_commonComponent = new Element;
        m_commonComponent->SetUnifiedSize(UDim2::SIZE_FULL);
        m_commonComponent->SetParent(this);
    }

    // If the common component is not a text, add a text child.
    // TODO: successive calls will create several text children, this should either look for the first available text, or use a cache.
    // - The default text target could be defined in xml components (I could have both a text and a common component ?).
    // - The intended behaviour for this SetText method is to provide a helper for buttons customization, but buttons may not use a text when defined in xml.
    // - Another possible decision would be to ignore this call if the component is not a text.
    ElementText* text = dynamic_cast<ElementText*>(m_commonComponent);
    if (!text)
    {
        text = new ElementText;
        m_commonComponent->AddChild(text);

        text->SetFont(fontID);
        text->SetUnifiedOrigin(UDim2::POSITION_TOP_CENTER);
        text->SetUnifiedPosition(UDim2::POSITION_TOP_CENTER);
    }

    text->SetText(value);
}

void ElementButton::SetDisabled(bool _bDisabled)
{
    if (m_isDisabled == _bDisabled)
        return;

    m_isDisabled = _bDisabled;

    if (m_isDisabled)
    {
        if (m_currentStateComponent)
        {
            m_currentStateComponent = m_disabledStateComponent ? m_disabledStateComponent : m_idleStateComponent;
        }
    }
    else
    {
        if (m_currentStateComponent)
        {
            m_currentStateComponent = m_idleStateComponent;
        }
    }
}

void ElementButton::SetOnMousePressed(const Callback& _pActionOnPressed)
{
    m_actionOnPressed = _pActionOnPressed;
}

void ElementButton::SetOnMouseReleased(const Callback& _pActionOnReleased)
{
    m_actionOnReleased = _pActionOnReleased;
}

void ElementButton::OnMousePressed(const InteractionInfos& interactionInfos)
{
    if (m_actionOnPressed)
    {
        m_actionOnPressed();
    }
}

void ElementButton::OnMouseReleased(const InteractionInfos& interactionInfos)
{
    if (m_actionOnReleased)
    {
        m_actionOnReleased();
    }
}

void ElementButton::OnMouseEntered(const InteractionInfos& interactionInfos)
{
    if (m_currentStateComponent && !m_isDisabled)
    {
        m_currentStateComponent = m_focusedStateComponent ? m_focusedStateComponent : m_idleStateComponent;
    }
}

void ElementButton::OnMouseLeft(const InteractionInfos& interactionInfos)
{
    if (m_currentStateComponent && !m_isDisabled)
    {
        m_currentStateComponent = m_idleStateComponent;
    }
}

void ElementButton::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    sf::FloatRect kGlobalTransformed = _kTransformSelf.transformRect(sf::FloatRect(Vector2::Zero_f, m_size));
    if (_kRenderPass.rectViewport.intersects(kGlobalTransformed))
    {
        if (m_currentStateComponent)
        {
            m_currentStateComponent->Render(_kRenderPass, _kTransformSelf);
        }

        if (m_commonComponent)
        {
            m_commonComponent->Render(_kRenderPass, _kTransformSelf);
        }
    }
}

void ElementButton::OnSizeChanged()
{
    if (m_idleStateComponent)
    {
        m_idleStateComponent->ComputeUnifiedDimensions();
    }

    if (m_focusedStateComponent)
    {
        m_focusedStateComponent->ComputeUnifiedDimensions();
    }

    if (m_disabledStateComponent)
    {
        m_disabledStateComponent->ComputeUnifiedDimensions();
    }

    if (m_commonComponent)
    {
        m_commonComponent->ComputeUnifiedDimensions();
    }
}

bool ElementButton::LoadFromDataImpl(ElementDataContext& context)
{
    if (!Element::LoadFromDataImpl(context))
        return false;

    ElementButtonData* buttonData = dynamic_cast<ElementButtonData*>(context.data);
    ElementData* backupData = context.data;

    bool result = true;

    auto loadComponentFromData = [&](ElementData* componentData, Element*& component)
    {
        component = InstanciateElement(componentData);
        if (component)
        {
            component->SetParent(this);

            context.data = componentData;
            result &= component->LoadFromData(context);
        }
    };

    loadComponentFromData(buttonData->commonComponent, m_commonComponent);
    loadComponentFromData(buttonData->idleStateComponent, m_idleStateComponent);
    loadComponentFromData(buttonData->focusedStateComponent, m_focusedStateComponent);
    loadComponentFromData(buttonData->disabledStateComponent, m_disabledStateComponent);

    // TODO: check null.
    m_currentStateComponent = m_idleStateComponent;

    context.data = backupData;

    return result;
}

}   // namespace gugu
