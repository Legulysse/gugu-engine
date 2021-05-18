////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/Element.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Misc/Action.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/External/PugiXmlWrap.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
        
ElementInteractions::ElementInteractions()
{
    m_interactionFlags = EInteraction::None;
}

ElementInteractions::~ElementInteractions()
{
    RemoveCallbacks();
}

void ElementInteractions::SetInteractionFlags(int _iFlags)
{
    m_interactionFlags = _iFlags;
}

void ElementInteractions::AddInteractionFlag(EInteraction::Type _eFlag)
{
    m_interactionFlags = m_interactionFlags | _eFlag;
}

void ElementInteractions::RemoveInteractionFlag(EInteraction::Type _eFlag)
{
    m_interactionFlags = m_interactionFlags & ~_eFlag;
}

int ElementInteractions::GetInteractionFlags() const
{
    return m_interactionFlags;
}

bool ElementInteractions::HasInteractionFlags() const
{
    return ((m_interactionFlags & ~EInteraction::Absorb) & ~EInteraction::Disabled) != EInteraction::None;
}

bool ElementInteractions::HasInteractionFlag(EInteraction::Type _eFlag) const
{
    return ((m_interactionFlags & _eFlag) != EInteraction::None);
}

void ElementInteractions::AddCallback(EInteraction::Type _eFlag, Action* _pCallback)
{
    if (!_pCallback || _eFlag == EInteraction::None)
        return;

    CallbackInfos kInfos;
    kInfos.interactionFlag = _eFlag;
    kInfos.action = _pCallback;
    m_callbacks.push_back(kInfos);
}

void ElementInteractions::RemoveCallbacks(EInteraction::Type _eFlag)
{
    for (auto iteCallback = m_callbacks.begin(); iteCallback != m_callbacks.end();)
    {
        if (iteCallback->interactionFlag == _eFlag)
        {
            SafeDelete(iteCallback->action);
            iteCallback = m_callbacks.erase(iteCallback);
        }
        else
        {
             ++iteCallback;
        }
    }
}

void ElementInteractions::RemoveCallbacks()
{
    for (size_t i = 0; i < m_callbacks.size(); ++i)
    {
        SafeDelete(m_callbacks[i].action);
    }
    m_callbacks.clear();
}

void ElementInteractions::FireCallbacks(EInteraction::Type _eFlag)
{
    for (size_t i = 0; i < m_callbacks.size(); ++i)
    {
        if (m_callbacks[i].interactionFlag == _eFlag)
            m_callbacks[i].action->Call();
    }
}


Element::Element()
{
    m_parent        = nullptr;

    m_flipX     = false;
    m_flipY     = false;

    m_renderPass    = GUGU_RENDERPASS_DEFAULT;
    m_isVisible = true;
    m_zIndex        = 0;

    m_showDebugBounds = false;

    m_useDimOrigin    = false;
    m_useDimPosition        = false;
    m_useDimSize        = false;

    //m_pShader     = nullptr;

    m_interactions     = nullptr;
}

Element::~Element()
{
    SafeDelete(m_interactions);
    //SafeDelete(m_pShader);

    if (m_parent)
    {
        StdVectorRemove(m_parent->m_children, this);
    }

    DeleteAllChildren();
}

void Element::AddChild(Element* _pNewChild)
{
    m_children.push_back(_pNewChild);
    _pNewChild->m_parent = this;
}

void Element::RemoveChild(Element* _pChild)
{
    StdVectorRemove(m_children, _pChild);
    _pChild->m_parent = nullptr;
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

void Element::SetParent(Element* _pNewParent, bool _bUpdateParents)
{
    if (_bUpdateParents)
    {
        if (m_parent)
            StdVectorRemove(m_parent->m_children, this);

        _pNewParent->AddChild(this);
    }
    else
    {
        m_parent = _pNewParent;
    }
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

int Element::GetChildCount() const
{
    return m_children.size();
}

const std::vector<Element*>& Element::GetChildren() const
{
    return m_children;
}

sf::Vector2f Element::TransformToLocalFull(const sf::Vector2f& _oPoint) const
{
    if (m_parent)
    {
        return GetInverseTransform().transformPoint( m_parent->TransformToLocalFull(_oPoint) );
    }

    return GetInverseTransform().transformPoint(_oPoint);
}

sf::Vector2f Element::TransformToGlobalFull(const sf::Vector2f& _oPoint) const
{
    if (m_parent)
    {
        return m_parent->TransformToGlobalFull( GetTransform().transformPoint(_oPoint) );
    }

    return GetTransform().transformPoint(_oPoint);
}

void Element::SetVisible(bool _bIsVisible)
{
    m_isVisible = _bIsVisible;
}

bool Element::IsVisible(bool _bCheckParent) const
{
    return m_isVisible && (!_bCheckParent || !m_parent || m_parent->IsVisible());
}

void Element::Step(const DeltaTime& dt)
{
    for (size_t i = 0; i < m_children.size(); ++i)
    {
        m_children[i]->Step(dt);
    }
}

void Element::Update(const DeltaTime& dt)
{
    for (size_t i = 0; i < m_children.size(); ++i)
    {
        m_children[i]->Update(dt);
    }
}

void Element::SetUnifiedOrigin(UDim2 _oNewDimOrigin)
{
    m_useDimOrigin = true;
    m_dimOrigin = _oNewDimOrigin;

    ComputeUnifiedOrigin();
}

void Element::SetUnifiedPosition(UDim2 _oNewDimPos)
{
    m_useDimPosition = true;
    m_dimPosition = _oNewDimPos;

    ComputeUnifiedDimensions();
}

void Element::SetUnifiedSize(UDim2 _oNewDimSize)
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
}

void Element::SetPositionY(float _fPosY)
{
    m_transform.setPosition(m_transform.getPosition().x, _fPosY);
}

void Element::SetPosition(float _fPosX, float _fPosY)
{
    m_transform.setPosition(_fPosX, _fPosY);
}

void Element::SetPosition(const sf::Vector2f& _kPosition)
{
    m_transform.setPosition(_kPosition);
}

void Element::Move(float _fOffsetX, float _fOffsetY)
{
    m_transform.move(_fOffsetX, _fOffsetY);
}

void Element::Move(const sf::Vector2f& _kOffset)
{
    m_transform.move(_kOffset);
}

const sf::Vector2f& Element::GetPosition() const
{
    return m_transform.getPosition();
}

void Element::SetRotation(float _fAngle)
{
    m_transform.setRotation(_fAngle);
}

void Element::Rotate(float _fAngle)
{
    m_transform.rotate(_fAngle);
}

float Element::GetRotation() const
{
    return m_transform.getRotation();
}

void Element::SetScaleX(float _fScaleX)
{
    if (m_flipY)
        _fScaleX = -_fScaleX;
    m_transform.setScale(_fScaleX, m_transform.getScale().y);
}

void Element::SetScaleY(float _fScaleY)
{
    if (m_flipX)
        _fScaleY = -_fScaleY;
    m_transform.setScale(m_transform.getScale().x, _fScaleY);
}

void Element::SetScale(float _fScale)
{
    SetScaleX(_fScale);
    SetScaleY(_fScale);
}

void Element::SetScale(float _fScaleX, float _fScaleY)
{
    SetScaleX(_fScaleX);
    SetScaleY(_fScaleY);
}

void Element::SetScale(const sf::Vector2f& _kScale)
{
    SetScaleX(_kScale.x);
    SetScaleY(_kScale.y);
}

void Element::Scale(float _fScaleX, float _fScaleY)
{
    m_transform.scale(_fScaleX, _fScaleY);
}

void Element::Scale(const sf::Vector2f& _kScale)
{
    m_transform.scale(_kScale);
}

sf::Vector2f Element::GetScale() const
{
    sf::Vector2f kScale = m_transform.getScale();
    if (m_flipY)
        kScale.x = -kScale.x;
    if (m_flipX)
        kScale.y = -kScale.y;
    return kScale;
}

void Element::SetOriginX(float _fOriginX)
{
    m_transform.setOrigin(_fOriginX, m_transform.getOrigin().y);
}

void Element::SetOriginY(float _fOriginY)
{
    m_transform.setOrigin(m_transform.getOrigin().x, _fOriginY);
}

void Element::SetOrigin(float _fOriginX, float _fOriginY)
{
    m_transform.setOrigin(_fOriginX, _fOriginY);
}

void Element::SetOrigin(const sf::Vector2f& _kOrigin)
{
    m_transform.setOrigin(_kOrigin);
}

const sf::Vector2f& Element::GetOrigin() const
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
    SetSize(sf::Vector2f(_fNewSizeX, m_size.y));
}

void Element::SetSizeY(float _fNewSizeY)
{
    SetSize(sf::Vector2f(m_size.x, _fNewSizeY));
}

void Element::SetSize(float _fNewSizeX, float _fNewSizeY)
{
    SetSize(sf::Vector2f(_fNewSizeX, _fNewSizeY));
}

void Element::SetSize(sf::Vector2f _kNewSize)
{
    sf::Vector2f kOldSize = m_size;
    m_size = _kNewSize;

    SetSizeImpl(kOldSize);

    ComputeUnifiedOrigin();
    
    for (size_t i = 0; i < m_children.size(); ++i)
    {
        m_children[i]->OnParentResized(kOldSize, m_size);
    }
}

void Element::SetSizeImpl(sf::Vector2f _kOldSize)
{
}

const sf::Vector2f& Element::GetSize() const
{
    return m_size;
}

sf::Vector2f Element::GetSizeScaled() const
{
    sf::Vector2f fSize = GetSize();
    fSize.x *= GetScale().x;
    fSize.y *= GetScale().y;
    return fSize;
}

sf::Vector2f Element::GetSizeOnScreen() const
{
    sf::Vector2f fSize = GetSizeScaled();

    Element* pParent = m_parent;
    while (pParent)
    {
        fSize.x *= pParent->GetScale().x;
        fSize.y *= pParent->GetScale().y;

        pParent = pParent->m_parent;
    }

    return fSize;
}

void Element::SetFlipX(bool _bFlip)
{
    //TODO: Rename FlipH and FlipV ?
    SetFlip(_bFlip, m_flipY);
}

void Element::SetFlipY(bool _bFlip)
{
    SetFlip(m_flipX, _bFlip);
}

void Element::SetFlip(bool _bFlipX, bool _bFlipY)
{
    sf::Vector2f kScale = m_transform.getScale();
    if (m_flipY != _bFlipY)
        kScale.x = -kScale.x;
    if (m_flipX != _bFlipX)
        kScale.y = -kScale.y;

    m_transform.setScale(kScale);

    m_flipX = _bFlipX;
    m_flipY = _bFlipY;
}

bool Element::GetFlipX() const
{
    return m_flipX;
}

bool Element::GetFlipY() const
{
    return m_flipY;
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
        sf::Vector2f oParentSize = m_parent->GetSize();

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

void Element::OnParentResized(sf::Vector2f _kOldSize, sf::Vector2f _kNewSize)
{
    ComputeUnifiedDimensions();

    for (size_t i = 0; i < m_children.size(); ++i)
    {
        m_children[i]->OnParentResized(_kOldSize, _kNewSize);
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

bool Element::IsPicked(const sf::Vector2f& _kGlobalCoords) const
{
    sf::Vector2f oPosMouse          = TransformToLocalFull(_kGlobalCoords);
    sf::Vector2f oPosBottomRight    = GetSize();

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
        
        OnPreDraw(_kRenderPass);

        if ((_kRenderPass.pass & m_renderPass) != GUGU_RENDERPASS_INVALID)
            DrawSelf(_kRenderPass, combinedTransform);
        
        for (size_t i = 0; i < m_children.size(); ++i)
        {
            m_children[i]->Render(_kRenderPass, combinedTransform);
        }

        OnPostDraw(_kRenderPass);

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
    pugi::xml_node oNodeUDimPosition = _oNodeElement.child("UDimPosition");
    if (!oNodeUDimPosition.empty())
    {
        UDim2 oDim;
        oDim.x.relative = oNodeUDimPosition.attribute("XRel").as_float(0.f);
        oDim.y.relative = oNodeUDimPosition.attribute("YRel").as_float(0.f);
        oDim.x.absolute = oNodeUDimPosition.attribute("XAbs").as_float(0.f);
        oDim.y.absolute = oNodeUDimPosition.attribute("YAbs").as_float(0.f);
        SetUnifiedPosition(oDim);
    }

    pugi::xml_node oNodeUDimSize = _oNodeElement.child("UDimSize");
    if (!oNodeUDimSize.empty())
    {
        UDim2 oDim;
        oDim.x.relative = oNodeUDimSize.attribute("XRel").as_float(0.f);
        oDim.y.relative = oNodeUDimSize.attribute("YRel").as_float(0.f);
        oDim.x.absolute = oNodeUDimSize.attribute("XAbs").as_float(0.f);
        oDim.y.absolute = oNodeUDimSize.attribute("YAbs").as_float(0.f);
        SetUnifiedSize(oDim);
    }

    return true;
}

}   // namespace gugu
