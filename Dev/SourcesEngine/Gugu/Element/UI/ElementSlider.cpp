////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/UI/ElementSlider.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Events/ElementEventHandler.h"
#include "Gugu/Element/ElementData.h"
#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementSlider::ElementSlider()
    : m_idleStateComponent(nullptr)
    , m_focusedStateComponent(nullptr)
    , m_disabledStateComponent(nullptr)
    , m_currentStateComponent(nullptr)
    , m_cursorComponent(nullptr)
    , m_actionOnValueChanged(nullptr)
    , m_isDisabled(false)
    , m_minValue(0)
    , m_maxValue(100)
    , m_value(0)
{
    GetEvents()->AddCallback(EInteractionEvent::MouseEntered, STD_BIND_1(&ElementSlider::OnMouseEntered, this));
    GetEvents()->AddCallback(EInteractionEvent::MouseLeft, STD_BIND_1(&ElementSlider::OnMouseLeft, this));
    GetEvents()->AddCallback(EInteractionEvent::MousePressed, STD_BIND_1(&ElementSlider::OnMousePressed, this));
}

ElementSlider::~ElementSlider()
{
    m_currentStateComponent = nullptr;

    SafeDelete(m_cursorComponent);
    SafeDelete(m_idleStateComponent);
    SafeDelete(m_focusedStateComponent);
    SafeDelete(m_disabledStateComponent);
}

bool ElementSlider::LoadFromWidget(const std::string& elementWidgetID)
{
    return LoadFromWidget(GetResources()->GetElementWidget(elementWidgetID));
}

bool ElementSlider::LoadFromWidget(ElementWidget* elementWidget)
{
    if (elementWidget)
    {
        elementWidget->LoadElementFromWidget(this);
    }

    return false;
}

void ElementSlider::SetValueLimits(int minValue, int maxValue)
{
    m_minValue = minValue;
    m_maxValue = maxValue;
    m_value = Clamp(m_value, m_minValue, m_maxValue);

    RaiseNeedRecompute();
}

void ElementSlider::SetValue(int value)
{
    value = Clamp(value, m_minValue, m_maxValue);

    if (m_value != value)
    {
        m_value = value;

        RaiseNeedRecompute();

        if (m_actionOnValueChanged)
        {
            m_actionOnValueChanged();
        }
    }
}

int ElementSlider::GetValue() const
{
    return m_value;
}

void ElementSlider::SetDisabled(bool _bDisabled)
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

void ElementSlider::SetOnValueChanged(const Callback& _pActionOnValueChanged)
{
    m_actionOnValueChanged = _pActionOnValueChanged;
}

void ElementSlider::OnMousePressed(const InteractionInfos& interactionInfos)
{
    if (!m_isDisabled)
    {
        Vector2f pickedPosition = interactionInfos.localPickingPosition;

        float offset = m_cursorComponent->GetSize().x / 2.f;
        int value = RemapLerp(offset, GetSize().x - offset, m_minValue, m_maxValue, pickedPosition.x);
        SetValue(value);

        // Hack to allow drag event to trigger.
        // TODO: I need to be able to directly call dragstart to avoid this hack.
        interactionInfos.absorbEvent = false;
    }
}

void ElementSlider::OnMouseDragMoved(const InteractionInfos& interactionInfos)
{
    if (!m_isDisabled)
    {
        Vector2f pickedPosition = interactionInfos.localPickingPosition;
        pickedPosition = m_cursorComponent->TransformToGlobal(pickedPosition, this);

        float offset = m_cursorComponent->GetSize().x / 2.f;
        int value = RemapLerp(offset, GetSize().x - offset, m_minValue, m_maxValue, pickedPosition.x);
        SetValue(value);

        RaiseNeedRecompute();
    }
}

void ElementSlider::OnMouseEntered(const InteractionInfos& interactionInfos)
{
    if (!m_isDisabled)
    {
        m_currentStateComponent = m_focusedStateComponent ? m_focusedStateComponent : m_idleStateComponent;
    }
}

void ElementSlider::OnMouseLeft(const InteractionInfos& interactionInfos)
{
    if (!m_isDisabled)
    {
        m_currentStateComponent = m_idleStateComponent;
    }
}

void ElementSlider::RecomputeImpl()
{
    float offset = m_cursorComponent->GetSize().x / 2.f;
    float position = RemapLerp(m_minValue, m_maxValue, offset, GetSize().x - offset, m_value);
    m_cursorComponent->SetPosition(position, 0.f);
}

void ElementSlider::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    sf::FloatRect kGlobalTransformed = _kTransformSelf.transformRect(sf::FloatRect(Vector2::Zero_f, m_size));
    if (_kRenderPass.rectViewport.intersects(kGlobalTransformed))
    {
        if (m_currentStateComponent)
        {
            m_currentStateComponent->Render(_kRenderPass, _kTransformSelf);
        }

        if (m_cursorComponent)
        {
            m_cursorComponent->Render(_kRenderPass, _kTransformSelf);
        }
    }
}

void ElementSlider::OnSizeChanged()
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

    if (m_cursorComponent)
    {
        m_cursorComponent->ComputeUnifiedDimensionsFromParent();
    }
}

bool ElementSlider::LoadFromDataImpl(ElementDataContext& context)
{
    if (!Element::LoadFromDataImpl(context))
        return false;

    ElementSliderData* elementData = dynamic_cast<ElementSliderData*>(context.data);
    BaseElementData* backupData = context.data;

    bool result = true;

    auto loadComponentFromData = [&](ElementDataContext& context, BaseElementData* componentData, Element*& component)
    {
        context.data = componentData;
        component = InstanciateAndLoadElement(context, this);
        // TODO: bool result
    };

    loadComponentFromData(context, elementData->idleStateComponent, m_idleStateComponent);
    loadComponentFromData(context, elementData->focusedStateComponent, m_focusedStateComponent);
    loadComponentFromData(context, elementData->disabledStateComponent, m_disabledStateComponent);
    loadComponentFromData(context, elementData->cursorComponent, m_cursorComponent);

    // TODO: check null.
    m_currentStateComponent = m_idleStateComponent;

    m_cursorComponent->GetEvents()->AddCallback(EInteractionEvent::MouseDragMoved, STD_BIND_1(&ElementSlider::OnMouseDragMoved, this));

    context.data = backupData;

    return result;
}

}   // namespace gugu
