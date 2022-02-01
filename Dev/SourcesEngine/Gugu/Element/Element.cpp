////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/Element.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Renderer.h"
#include "Gugu/External/PugiXmlWrap.h"
#include "Gugu/Math/MathUtility.h"

#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
        
Element::Element()
{
    m_parent = nullptr;

    m_flipV = false;
    m_flipH = false;

    m_renderPass = GUGU_RENDERPASS_DEFAULT;
    m_isVisible = true;
    m_zIndex = 0;

    m_showDebugBounds = false;

    m_useDimOrigin = false;
    m_useDimPosition = false;
    m_useDimSize = false;

    //m_pShader = nullptr;

    m_interactions = nullptr;
}

Element::~Element()
{
    if (m_interactions)
    {
        m_interactions->FireCallbacks(EInteraction::Destroyed);
        SafeDelete(m_interactions);
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

void Element::SetParent(Element* parent, bool recomputeDimensions)
{
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

Vector2f Element::TransformToLocal(const Vector2f& point) const
{
    if (m_parent)
    {
        return GetInverseTransform().transformPoint( m_parent->TransformToLocal(point) );
    }

    return GetInverseTransform().transformPoint(point);
}

Vector2f Element::TransformToGlobal(const Vector2f& point) const
{
    if (m_parent)
    {
        return m_parent->TransformToGlobal( GetTransform().transformPoint(point) );
    }

    return GetTransform().transformPoint(point);
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

UDim2 Element::GetUnifiedOrigin() const
{
    return m_dimOrigin;
}

UDim2 Element::GetUnifiedPosition() const
{
    return m_dimPosition;
}

UDim2 Element::GetUnifiedSize() const
{
    return m_dimSize;
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
        SetOrigin(m_dimOrigin.GetComputedDimension(m_size));
    }
}

void Element::ComputeUnifiedDimensions()
{
    if (m_parent)
    {
        Vector2f oParentSize = m_parent->GetSize();

        if (m_useDimPosition)
        {
            SetPosition(m_dimPosition.GetComputedDimension(oParentSize));
        }

        if (m_useDimSize)
        {
            SetSize(m_dimSize.GetComputedDimension(oParentSize));
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

/*void Element::SetShader(sf::Shader* _pShader)
{
    m_pShader = _pShader;
}*/

void Element::SetInteractionFlags(int _iFlags)
{
    InitInteractions();
    m_interactions->SetInteractionFlags(_iFlags);
}

void Element::AddInteractionFlag(EInteraction::Type _eFlag)
{
    InitInteractions();
    m_interactions->AddInteractionFlag(_eFlag);
}

void Element::RemoveInteractionFlag(EInteraction::Type _eFlag)
{
    if (m_interactions)
        m_interactions->RemoveInteractionFlag(_eFlag);
}

int Element::GetInteractionFlags() const
{
    if (m_interactions)
        return m_interactions->GetInteractionFlags();
    return EInteraction::None;
}

bool Element::HasInteractionFlags() const
{
    if (m_interactions)
        return m_interactions->HasInteractionFlags();
    return false;
}

bool Element::HasInteractionFlag(EInteraction::Type _eFlag) const
{
    if (m_interactions)
        return m_interactions->HasInteractionFlag(_eFlag);
    return false;
}

void Element::InitInteractions()
{
    if (!m_interactions)
    {
        m_interactions = new ElementInteractions;
    }
}

ElementInteractions* Element::GetInteractions() const
{
    return m_interactions;
}

bool Element::IsPicked(const Vector2f& _kGlobalCoords) const
{
    Vector2f oPosMouse = TransformToLocal(_kGlobalCoords);
    Vector2f oPosBottomRight = GetSize();

    if (    oPosMouse.x >= 0 && oPosMouse.x < oPosBottomRight.x
        &&  oPosMouse.y >= 0 && oPosMouse.y < oPosBottomRight.y )
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
        if (m_showDebugBounds || (_kRenderPass.frameInfos && _kRenderPass.frameInfos->showBounds))
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

bool Element::LoadFromXml(const pugi::xml_node& _oNodeElement)
{
    pugi::xml_node nodeSize = _oNodeElement.child("Size");
    if (nodeSize)
    {
        Vector2f size;
        size.x = nodeSize.attribute("x").as_float(0.f);
        size.y = nodeSize.attribute("y").as_float(0.f);
        SetSize(size);
    }

    pugi::xml_node oNodeUDimPosition = _oNodeElement.child("UPosition");
    if (!oNodeUDimPosition.empty())
    {
        UDim2 oDim;
        oDim.x.relative = oNodeUDimPosition.attribute("xRel").as_float(0.f);
        oDim.y.relative = oNodeUDimPosition.attribute("yRel").as_float(0.f);
        oDim.x.absolute = oNodeUDimPosition.attribute("xAbs").as_float(0.f);
        oDim.y.absolute = oNodeUDimPosition.attribute("yAbs").as_float(0.f);
        SetUnifiedPosition(oDim);
    }

    pugi::xml_node oNodeUDimSize = _oNodeElement.child("USize");
    if (!oNodeUDimSize.empty())
    {
        UDim2 oDim;
        oDim.x.relative = oNodeUDimSize.attribute("xRel").as_float(0.f);
        oDim.y.relative = oNodeUDimSize.attribute("yRel").as_float(0.f);
        oDim.x.absolute = oNodeUDimSize.attribute("xAbs").as_float(0.f);
        oDim.y.absolute = oNodeUDimSize.attribute("yAbs").as_float(0.f);
        SetUnifiedSize(oDim);
    }

    return true;
}

}   // namespace gugu
