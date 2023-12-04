////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/UI/ElementCheckbox.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Events/ElementEventHandler.h"
#include "Gugu/Element/ElementData.h"
#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementCheckbox::ElementCheckbox()
    : m_commonComponent(nullptr)
    , m_idleStateComponent(nullptr)
    , m_focusedStateComponent(nullptr)
    , m_disabledStateComponent(nullptr)
    , m_currentStateComponent(nullptr)
    , m_checkedComponent(nullptr)
    , m_text(nullptr)
    , m_actionOnCheckedChanged(nullptr)
    , m_isDisabled(false)
    , m_isChecked(false)
{
    GetEvents()->AddCallback(EInteractionEvent::MouseEntered, STD_BIND_1(&ElementCheckbox::OnMouseEntered, this));
    GetEvents()->AddCallback(EInteractionEvent::MouseLeft, STD_BIND_1(&ElementCheckbox::OnMouseLeft, this));
    GetEvents()->AddCallback(EInteractionEvent::MousePressed, STD_BIND_1(&ElementCheckbox::OnMousePressed, this));
}

ElementCheckbox::~ElementCheckbox()
{
    m_currentStateComponent = nullptr;
    m_text = nullptr;

    SafeDelete(m_checkedComponent);
    SafeDelete(m_idleStateComponent);
    SafeDelete(m_focusedStateComponent);
    SafeDelete(m_disabledStateComponent);
    SafeDelete(m_commonComponent);
}

bool ElementCheckbox::LoadFromWidget(const std::string& elementWidgetID)
{
    return LoadFromWidget(GetResources()->GetElementWidget(elementWidgetID));
}

bool ElementCheckbox::LoadFromWidget(ElementWidget* elementWidget)
{
    if (elementWidget)
    {
        elementWidget->LoadElementFromWidget(this);
    }

    return false;
}

void ElementCheckbox::SetChecked(bool checked)
{
    if (m_isChecked != checked)
    {
        m_isChecked = checked;

        if (m_actionOnCheckedChanged)
        {
            m_actionOnCheckedChanged();
        }
    }
}

bool ElementCheckbox::IsChecked() const
{
    return m_isChecked;
}

void ElementCheckbox::SetText(const std::string& value, const std::string& fontID)
{
    // TODO: This is a safety for incomplete button widgets, it could be rethink along with deprecated SetTexture methods.
    if (!m_commonComponent)
    {
        m_commonComponent = new Element;
        m_commonComponent->SetUnifiedSize(UDim2::SIZE_FULL);
        m_commonComponent->SetParent(this);
    }

    // If the common component is not a text, add a text child.
    if (!m_text)
    {
        m_text = dynamic_cast<ElementText*>(m_commonComponent);
        if (!m_text)
        {
            m_text = m_commonComponent->AddChild<ElementText>();
            m_text->SetFont(fontID);
            m_text->SetUnifiedOrigin(UDim2::POSITION_TOP_CENTER);
            m_text->SetUnifiedPosition(UDim2::POSITION_TOP_CENTER);
        }
    }

    m_text->SetText(value);
}

void ElementCheckbox::SetDisabled(bool _bDisabled)
{
    if (m_isDisabled == _bDisabled)
        return;

    m_isDisabled = _bDisabled;

    if (m_isDisabled)
    {
        m_currentStateComponent = m_disabledStateComponent ? m_disabledStateComponent : m_idleStateComponent;
    }
    else
    {
        m_currentStateComponent = m_idleStateComponent;
    }
}

void ElementCheckbox::SetOnCheckedChanged(const Callback& _pActionOnCheckedChanged)
{
    m_actionOnCheckedChanged = _pActionOnCheckedChanged;
}

void ElementCheckbox::OnMousePressed(const InteractionInfos& interactionInfos)
{
    if (!m_isDisabled)
    {
        SetChecked(!m_isChecked);
    }
}

void ElementCheckbox::OnMouseEntered(const InteractionInfos& interactionInfos)
{
    if (!m_isDisabled)
    {
        m_currentStateComponent = m_focusedStateComponent ? m_focusedStateComponent : m_idleStateComponent;
    }
}

void ElementCheckbox::OnMouseLeft(const InteractionInfos& interactionInfos)
{
    if (!m_isDisabled)
    {
        m_currentStateComponent = m_idleStateComponent;
    }
}

void ElementCheckbox::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    sf::FloatRect kGlobalTransformed = _kTransformSelf.transformRect(sf::FloatRect(Vector2::Zero_f, m_size));
    if (_kRenderPass.rectViewport.intersects(kGlobalTransformed))
    {
        if (m_currentStateComponent)
        {
            m_currentStateComponent->Render(_kRenderPass, _kTransformSelf);
        }

        if (m_checkedComponent && m_isChecked)
        {
            m_checkedComponent->Render(_kRenderPass, _kTransformSelf);
        }

        if (m_commonComponent)
        {
            m_commonComponent->Render(_kRenderPass, _kTransformSelf);
        }
    }
}

void ElementCheckbox::OnSizeChanged()
{
    if (m_idleStateComponent)
    {
        m_idleStateComponent->ComputeUnifiedDimensionsFromParent();
    }

    if (m_focusedStateComponent)
    {
        m_focusedStateComponent->ComputeUnifiedDimensionsFromParent();
    }

    if (m_disabledStateComponent)
    {
        m_disabledStateComponent->ComputeUnifiedDimensionsFromParent();
    }

    if (m_commonComponent)
    {
        m_commonComponent->ComputeUnifiedDimensionsFromParent();
    }

    if (m_checkedComponent)
    {
        m_checkedComponent->ComputeUnifiedDimensionsFromParent();
    }
}

bool ElementCheckbox::LoadFromDataImpl(ElementDataContext& context)
{
    if (!Element::LoadFromDataImpl(context))
        return false;

    ElementCheckboxData* elementData = dynamic_cast<ElementCheckboxData*>(context.data);
    BaseElementData* backupData = context.data;

    bool result = true;

    auto loadComponentFromData = [&](ElementDataContext& context, BaseElementData* componentData, Element*& component)
    {
        context.data = componentData;
        component = InstanciateAndLoadElement(context, this);
        // TODO: bool result
    };

    loadComponentFromData(context, elementData->commonComponent, m_commonComponent);
    loadComponentFromData(context, elementData->idleStateComponent, m_idleStateComponent);
    loadComponentFromData(context, elementData->focusedStateComponent, m_focusedStateComponent);
    loadComponentFromData(context, elementData->disabledStateComponent, m_disabledStateComponent);
    loadComponentFromData(context, elementData->checkedComponent, m_checkedComponent);

    // TODO: check null.
    m_currentStateComponent = m_idleStateComponent;

    context.data = backupData;

    return result;
}

}   // namespace gugu
