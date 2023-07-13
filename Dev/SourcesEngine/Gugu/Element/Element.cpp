////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/Element.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/ElementData.h"
#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Events/ElementEventHandler.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Debug/Logger.h"

#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
        
Element::Element()
    : m_parent(nullptr)
    , m_flipV(false)
    , m_flipH(false)
    , m_renderPass(GUGU_RENDERPASS_DEFAULT)
    , m_isVisible(true)
    , m_zIndex(0)
    , m_showDebugBounds(false)
    , m_useDimOrigin(false)
    , m_useDimPosition(false)
    , m_useDimSize(false)
    //, m_pShader(nullptr)
    , m_eventHandler(nullptr)
{
}

Element::~Element()
{
    if (m_eventHandler)
    {
        m_eventHandler->FireCallbacks(EElementEvent::Destroyed);
        SafeDelete(m_eventHandler);
    }

    //SafeDelete(m_pShader);

    if (m_parent)
    {
        StdVectorRemove(m_parent->m_children, this);
    }

    DeleteAllChildren();
}

void Element::AddChild(Element* child)
{
    child->SetParent(this);
    m_children.push_back(child);
}

bool Element::InsertChild(Element* child, size_t index)
{
    if (index < 0 || index > m_children.size())
        return false;

    child->SetParent(this);
    StdVectorInsertAt(m_children, index, child);
    return true;
}

Element* Element::AddChildWidget(const std::string& elementWidgetID)
{
    return AddChildWidget(GetResources()->GetElementWidget(elementWidgetID));
}

Element* Element::AddChildWidget(ElementWidget* elementWidget)
{
    if (elementWidget)
    {
        if (Element* child = elementWidget->InstanciateWidget())
        {
            AddChild(child);
            return child;
        }
    }

    return nullptr;
}

void Element::SetParent(Element* parent, bool recomputeDimensions)
{
    // Some processes can set the parent before calling AddChild, this test ensures we reduce unnecessary dimensions computation.
    if (m_parent == parent)
        return;

    if (m_parent)
    {
        // Ensure this Element is not attached anymore to another Element.
        m_parent->RemoveChild(this);
    }

    m_parent = parent;

    if (recomputeDimensions)
    {
        ComputeUnifiedDimensions();
    }
}

void Element::RemoveChild(Element* child)
{
    if (child->m_parent == this)
    {
        StdVectorRemove(m_children, child);
        child->m_parent = nullptr;
    }
}

void Element::DeleteAllChildren()
{
    for (size_t i = 0; i < m_children.size(); ++i)
    {
        Element* pElem = m_children[i];
        pElem->m_parent = nullptr;  //Set to null before calling the delete, to avoid the child to call a remove

        SafeDelete(pElem);
    }

    m_children.clear();
}

Element* Element::GetParent() const
{
    return m_parent;
}

Element* Element::GetTopParent() const
{
    if (m_parent)
        return m_parent->GetTopParent();
    return const_cast<Element*>(this);
}

size_t Element::GetChildCount() const
{
    return m_children.size();
}

const std::vector<Element*>& Element::GetChildren() const
{
    return m_children;
}

Vector2f Element::TransformToLocal(const Vector2f& globalCoords) const
{
    if (m_parent)
    {
        return GetInverseTransform().transformPoint(m_parent->TransformToLocal(globalCoords));
    }

    return GetInverseTransform().transformPoint(globalCoords);
}

Vector2f Element::TransformToLocal(const Vector2f& ancestorCoords, Element* ancestorReference) const
{
    if (ancestorReference == this)
    {
        return ancestorCoords;
    }

    if (m_parent && ancestorReference != m_parent)
    {
        return GetInverseTransform().transformPoint(m_parent->TransformToLocal(ancestorCoords, ancestorReference));
    }

    return GetInverseTransform().transformPoint(ancestorCoords);
}

Vector2f Element::TransformToGlobal(const Vector2f& localCoords) const
{
    if (m_parent)
    {
        return m_parent->TransformToGlobal(GetTransform().transformPoint(localCoords));
    }

    return GetTransform().transformPoint(localCoords);
}

void Element::SetVisible(bool visible)
{
    m_isVisible = visible;
    OnVisibleChanged();
}

bool Element::IsVisible(bool checkParents) const
{
    return m_isVisible && (!checkParents || !m_parent || m_parent->IsVisible(true));
}

void Element::SetUnifiedOrigin(const UDim2& _oNewDimOrigin)
{
    m_useDimOrigin = true;
    m_dimOrigin = _oNewDimOrigin;

    ComputeUnifiedOrigin();
}

void Element::SetUnifiedPosition(const UDim2& _oNewDimPos)
{
    m_useDimPosition = true;
    m_dimPosition = _oNewDimPos;

    ComputeUnifiedDimensions();
}

void Element::SetUnifiedSize(const UDim2& _oNewDimSize)
{
    m_useDimSize = true;
    m_dimSize = _oNewDimSize;

    ComputeUnifiedDimensions();
}

bool Element::GetUseUnifiedOrigin() const
{
    return m_useDimOrigin;
}

bool Element::GetUseUnifiedPosition() const
{
    return m_useDimPosition;
}

bool Element::GetUseUnifiedSize() const
{
    return m_useDimSize;
}

const UDim2& Element::GetUnifiedOrigin() const
{
    return m_dimOrigin;
}

const UDim2& Element::GetUnifiedPosition() const
{
    return m_dimPosition;
}

const UDim2& Element::GetUnifiedSize() const
{
    return m_dimSize;
}

void Element::ResetUnifiedOrigin()
{
    m_useDimOrigin = false;
    m_dimOrigin = UDim2::ZERO;
}

void Element::ResetUnifiedPosition()
{
    m_useDimPosition = false;
    m_dimPosition = UDim2::ZERO;
}

void Element::ResetUnifiedSize()
{
    m_useDimSize = false;
    m_dimSize = UDim2::ZERO;
}

void Element::SetPositionX(float _fPosX)
{
    m_transform.setPosition(_fPosX, m_transform.getPosition().y);
    OnTransformChanged();
}

void Element::SetPositionY(float _fPosY)
{
    m_transform.setPosition(m_transform.getPosition().x, _fPosY);
    OnTransformChanged();
}

void Element::SetPosition(float _fPosX, float _fPosY)
{
    m_transform.setPosition(_fPosX, _fPosY);
    OnTransformChanged();
}

void Element::SetPosition(const Vector2f& _kPosition)
{
    m_transform.setPosition(_kPosition);
    OnTransformChanged();
}

void Element::Move(float _fOffsetX, float _fOffsetY)
{
    m_transform.move(_fOffsetX, _fOffsetY);
    OnTransformChanged();
}

void Element::Move(const Vector2f& _kOffset)
{
    m_transform.move(_kOffset);
    OnTransformChanged();
}

const Vector2f& Element::GetPosition() const
{
    return m_transform.getPosition();
}

void Element::SetRotation(float _fAngle)
{
    m_transform.setRotation(_fAngle);
    OnTransformChanged();
}

void Element::Rotate(float _fAngle)
{
    m_transform.rotate(_fAngle);
    OnTransformChanged();
}

float Element::GetRotation() const
{
    return m_transform.getRotation();
}

void Element::SetScaleX(float _fScaleX)
{
    _fScaleX = m_flipH ? -_fScaleX : _fScaleX;
    m_transform.setScale(_fScaleX, m_transform.getScale().y);
    OnTransformChanged();
}

void Element::SetScaleY(float _fScaleY)
{
    _fScaleY = m_flipV ? -_fScaleY : _fScaleY;
    m_transform.setScale(m_transform.getScale().x, _fScaleY);
    OnTransformChanged();
}

void Element::SetScale(float _fScale)
{
    SetScale(_fScale, _fScale);
}

void Element::SetScale(float _fScaleX, float _fScaleY)
{
    _fScaleX = m_flipH ? -_fScaleX : _fScaleX;
    _fScaleY = m_flipV ? -_fScaleY : _fScaleY;
    m_transform.setScale(_fScaleX, _fScaleY);
    OnTransformChanged();
}

void Element::SetScale(const Vector2f& _kScale)
{
    SetScale(_kScale.x, _kScale.y);
}

void Element::Scale(float factorX, float factorY)
{
    m_transform.scale(factorX, factorY);
    OnTransformChanged();
}

void Element::Scale(const Vector2f& factor)
{
    m_transform.scale(factor);
    OnTransformChanged();
}

Vector2f Element::GetScale() const
{
    Vector2f scale = m_transform.getScale();
    scale.x = m_flipH ? -scale.x : scale.x;
    scale.y = m_flipV ? -scale.y : scale.y;
    return scale;
}

void Element::SetOriginX(float _fOriginX)
{
    m_transform.setOrigin(_fOriginX, m_transform.getOrigin().y);
    OnTransformChanged();
}

void Element::SetOriginY(float _fOriginY)
{
    m_transform.setOrigin(m_transform.getOrigin().x, _fOriginY);
    OnTransformChanged();
}

void Element::SetOrigin(float _fOriginX, float _fOriginY)
{
    m_transform.setOrigin(_fOriginX, _fOriginY);
    OnTransformChanged();
}

void Element::SetOrigin(const Vector2f& _kOrigin)
{
    m_transform.setOrigin(_kOrigin);
    OnTransformChanged();
}

const Vector2f& Element::GetOrigin() const
{
    return m_transform.getOrigin();
}

const sf::Transform& Element::GetTransform() const
{
    return m_transform.getTransform();
}

const sf::Transform& Element::GetInverseTransform() const
{
    return m_transform.getInverseTransform();
}

void Element::GetGlobalCorners(Vector2f& topLeft, Vector2f& topRight, Vector2f& bottomLeft, Vector2f& bottomRight) const
{
    topLeft = TransformToGlobal(Vector2::Zero_f);
    topRight = TransformToGlobal(Vector2::Zero_f + Vector2f(m_size.x, 0));
    bottomLeft = TransformToGlobal(Vector2::Zero_f + Vector2f(0, m_size.y));
    bottomRight = TransformToGlobal(Vector2::Zero_f + m_size);
}

sf::FloatRect Element::GetGlobalBounds() const
{
    Vector2f topLeft, topRight, bottomLeft, bottomRight;
    GetGlobalCorners(topLeft, topRight, bottomLeft, bottomRight);

    float minX = Min(Min(topLeft.x, topRight.x), Min(bottomLeft.x, bottomRight.x));
    float miny = Min(Min(topLeft.y, topRight.y), Min(bottomLeft.y, bottomRight.y));
    float maxX = Max(Max(topLeft.x, topRight.x), Max(bottomLeft.x, bottomRight.x));
    float maxy = Max(Max(topLeft.y, topRight.y), Max(bottomLeft.y, bottomRight.y));

    Vector2f topLeftBounds = Vector2f(minX, miny);
    Vector2f bottomRightBounds = Vector2f(maxX, maxy);
    return sf::FloatRect(topLeftBounds, bottomRightBounds - topLeftBounds);
}

void Element::SetSizeX(float _fNewSizeX)
{
    SetSize(Vector2f(_fNewSizeX, m_size.y));
}

void Element::SetSizeY(float _fNewSizeY)
{
    SetSize(Vector2f(m_size.x, _fNewSizeY));
}

void Element::SetSize(float _fNewSizeX, float _fNewSizeY)
{
    SetSize(Vector2f(_fNewSizeX, _fNewSizeY));
}

void Element::SetSize(const Vector2f& _kNewSize)
{
    m_size = _kNewSize;

    // Ensure we dont get a negative size.
    m_size.x = Max(0.f, m_size.x);
    m_size.y = Max(0.f, m_size.y);

    OnSizeChanged();

    ComputeUnifiedOrigin();
    
    for (size_t i = 0; i < m_children.size(); ++i)
    {
        m_children[i]->OnParentResized();
    }
}

const Vector2f& Element::GetSize() const
{
    return m_size;
}

void Element::SetFlipV(bool _bFlip)
{
    SetFlip(_bFlip, m_flipH);
}

void Element::SetFlipH(bool _bFlip)
{
    SetFlip(m_flipV, _bFlip);
}

void Element::SetFlip(bool _bFlipV, bool _bFlipH)
{
    Vector2f scale = GetScale();

    m_flipV = _bFlipV;
    m_flipH = _bFlipH;

    SetScale(scale);
}

bool Element::GetFlipV() const
{
    return m_flipV;
}

bool Element::GetFlipH() const
{
    return m_flipH;
}

void Element::ComputeUnifiedOrigin()
{
    if (m_useDimOrigin)
    {
        SetOrigin(m_dimOrigin.GetPixelAlignedComputedDimension(m_size));
    }
}

void Element::ComputeUnifiedDimensions()
{
    if (m_parent)
    {
        Vector2f oParentSize = m_parent->GetSize();

        if (m_useDimPosition)
        {
            SetPosition(m_dimPosition.GetPixelAlignedComputedDimension(oParentSize));
        }

        if (m_useDimSize)
        {
            SetSize(m_dimSize.GetPixelAlignedComputedDimension(oParentSize));
        }
    }
}

void Element::OnParentResized()
{
    ComputeUnifiedDimensions();

    for (size_t i = 0; i < m_children.size(); ++i)
    {
        m_children[i]->OnParentResized();
    }
}

void Element::SetZIndex(int32 _iZIndex)
{
    m_zIndex = _iZIndex;
}

int32 Element::GetZIndex() const
{
    return m_zIndex;
}

void Element::SortOnZIndex()
{
    std::sort(m_children.begin(), m_children.end(), CompareZIndex);

    for (size_t i = 0; i < m_children.size(); ++i)
    {
        m_children[i]->SortOnZIndex();
    }
}

bool Element::CompareZIndex(Element* _pLeft, Element* _pRight)
{
    return _pLeft->m_zIndex < _pRight->m_zIndex;
}

void Element::SetRenderPass(int _iPass)
{
    m_renderPass = _iPass;
}

void Element::AddRenderPass(int _iPass)
{
    m_renderPass = m_renderPass | _iPass;
}

void Element::RemoveRenderPass(int _iPass)
{
    m_renderPass = m_renderPass & ~_iPass;
}

void Element::SetDebugBoundsVisible(bool showDebugBounds)
{
    m_showDebugBounds = showDebugBounds;
}

//void Element::SetShader(sf::Shader* _pShader)
//{
//    m_pShader = _pShader;
//}

ElementEventHandler* Element::GetEvents()
{
    if (!m_eventHandler)
    {
        m_eventHandler = new ElementEventHandler(this);
    }

    return m_eventHandler;
}

bool Element::IsPicked(const Vector2f& globalCoords) const
{
    Vector2f localPickedCoords = TransformToLocal(globalCoords);
    return IsPickedLocal(localPickedCoords);
}

bool Element::IsPicked(const Vector2f& globalCoords, Vector2f& localPickedCoords) const
{
    localPickedCoords = TransformToLocal(globalCoords);
    return IsPickedLocal(localPickedCoords);
}

bool Element::IsPickedLocal(Vector2f& localCoords) const
{
    Vector2f bottomRight = GetSize();

    if (localCoords.x >= 0 && localCoords.x < bottomRight.x
        && localCoords.y >= 0 && localCoords.y < bottomRight.y)
    {
        return true;
    }

    return false;
}

void Element::Render(RenderPass& _kRenderPass, const sf::Transform& _kTransformParent)
{
    if (m_isVisible)
    {
        sf::Transform combinedTransform = _kTransformParent * GetTransform();
        
        if ((_kRenderPass.pass & m_renderPass) != GUGU_RENDERPASS_INVALID)
        {
            RenderImpl(_kRenderPass, combinedTransform);
        }
        
        for (size_t i = 0; i < m_children.size(); ++i)
        {
            m_children[i]->Render(_kRenderPass, combinedTransform);
        }

        //Debug Bounds
        if (_kRenderPass.frameInfos && (m_showDebugBounds || _kRenderPass.frameInfos->showBounds))
        {
            // TODO: Maybe try to render all bounds in a single drawcall ?
            _kRenderPass.frameInfos->defaultBoundsShape.setSize(m_size);
            _kRenderPass.target->draw(_kRenderPass.frameInfos->defaultBoundsShape, combinedTransform);

            // TODO: I believe it's not useful to count the bounds drawcalls, but it could be done through separate stats.
            ////Stats
            //if (_kRenderPass.frameInfos)
            //    _kRenderPass.frameInfos->statDrawCalls += 1;
        }
    }
}

bool Element::LoadFromData(ElementDataContext& context)
{
    // Load this Element data.
    // - At this point, the element has already been deserialized through ElementWidget::InstanciateWidget.
    // - This method will load the current Element data, then proceed to instanciate and load its children.

    bool result = true;
    BaseElementData* elementData = context.data;

    FillElementPath(context);
    context.path.push_back(elementData->name);
    result = LoadFromDataImpl(context);

    if (context.dataBindings)
    {
        context.dataBindings->elementFromData.insert(std::make_pair(elementData, this));
        context.dataBindings->dataFromElement.insert(std::make_pair(this, elementData));
    }

    result &= LoadChildrenFromData(context);
    context.path.pop_back();

    return result;
}

bool Element::LoadFromWidgetData(ElementDataContext& context)
{
    // Load this Element override data from a given ElementWidgetData.
    // - At this point, the element has already been deserialized through ElementWidget::InstanciateWidget.
    // - We only need to load the overrides from the widget root (transform etc).
    // - Some children may already exist from the initial deserialization.

    bool result = true;
    BaseElementData* elementData = context.data;

    FillElementPath(context);
    context.path.push_back(elementData->name);
    result = LoadFromWidgetDataImpl(context);

    if (context.dataBindings)
    {
        // Fill bindings informations.
        // - The instantiated Element will be referenced by both the ElementWidgetData and the widget root ElementData, but it will only reference the root ElementData.
        // - As a result, the Element will have no knowledge of the ElementWidget it originates from, and get attached directly to the provided parent.
        context.dataBindings->elementFromData.insert(std::make_pair(elementData, this));
        //context.dataBindings->dataFromElement.insert(std::make_pair(this, elementData));
    }

    result &= LoadChildrenFromData(context);
    context.path.pop_back();

    return result;
}

void Element::FillElementPath(ElementDataContext& context)
{
    const std::string& name = context.data->name;

    if (context.pathBindings && !name.empty())
    {
        std::string path = "";
        for (const std::string& part : context.path)
        {
            if (!part.empty())
            {
                path += part + '/';
            }
        }

        path += name;

        // TODO: handle multiple elements with same name ?
        auto it = context.pathBindings->elementFromPath.find(path);
        if (it == context.pathBindings->elementFromPath.end())
        {
            context.pathBindings->elementFromPath.insert(std::make_pair(path, this));
        }
        else
        {
            GetLogEngine()->Print(ELog::Error, ELogEngine::Element, StringFormat("An ElementWidget contains several elements with the same path : {0}", path));
        }
    }
}

bool Element::LoadChildrenFromData(ElementDataContext& context)
{
    // This method will instanciate and load this Element children.
    // - Some children may already exist from the initial deserialization.

    bool result = true;
    BaseElementData* elementData = context.data;

    size_t childCount = elementData->children.size();
    if (childCount > 0)
    {
        BaseElementData* backupData = elementData;

        for (size_t i = 0; i < childCount; ++i)
        {
            BaseElementData* childData = elementData->children[i];

            context.data = childData;
            if (Element* child = InstanciateAndLoadElement(context, this))
            {
                AddChild(child);
            }
        }

        context.data = backupData;
    }

    return result;
}

bool Element::LoadFromDataImpl(ElementDataContext& context)
{
    ElementData* elementData = dynamic_cast<ElementData*>(context.data);

    if (elementData->useDimOrigin)
    {
        SetUnifiedOrigin(elementData->dimOrigin);
    }
    else if (elementData->origin != Vector2::Zero_f)
    {
        SetOrigin(elementData->origin);
    }

    if (elementData->useDimPosition)
    {
        SetUnifiedPosition(elementData->dimPosition);
    }
    else if (elementData->position != Vector2::Zero_f)
    {
        SetPosition(elementData->position);
    }

    if (elementData->useDimSize)
    {
        SetUnifiedSize(elementData->dimSize);
    }
    else if (elementData->size != Vector2::Zero_f)
    {
        SetSize(elementData->size);
    }

    if (elementData->rotation != 0.f)
    {
        SetRotation(elementData->rotation);
    }

    if (elementData->flipV)
    {
        SetFlipV(elementData->flipV);
    }

    if (elementData->flipH)
    {
        SetFlipH(elementData->flipH);
    }

    return true;
}

bool Element::LoadFromWidgetDataImpl(ElementDataContext& context)
{
    ElementWidgetData* widgetInstanceData = dynamic_cast<ElementWidgetData*>(context.data);

    if (widgetInstanceData->overrideOrigin)
    {
        SetUnifiedOrigin(widgetInstanceData->dimOrigin);
    }

    if (widgetInstanceData->overridePosition)
    {
        SetUnifiedPosition(widgetInstanceData->dimPosition);
    }

    if (widgetInstanceData->overrideSize)
    {
        SetUnifiedSize(widgetInstanceData->dimSize);
    }

    if (widgetInstanceData->overrideRotation)
    {
        SetRotation(widgetInstanceData->rotation);
    }

    if (widgetInstanceData->overrideFlipV)
    {
        SetFlipV(widgetInstanceData->flipV);
    }

    if (widgetInstanceData->overrideFlipH)
    {
        SetFlipH(widgetInstanceData->flipH);
    }

    return true;
}

}   // namespace gugu
