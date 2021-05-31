////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Window/Camera.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Window/Window.h"
#include "Gugu/World/Level.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/Element/Element.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"

#include <SFML/Graphics/RenderWindow.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
Camera::Camera()
{
    m_zoomMultiplier = 1.f;

    m_targetPosition = sf::Vector2f(0.f, 0.f);
    m_centerOnTarget = false;

    m_window = nullptr;
    m_level = nullptr;

    //m_extendOnResize = false;
}

Camera::~Camera()
{
}

void Camera::SetWindow(Window* _pWindow)
{
    m_window = _pWindow;
}

Window* Camera::GetWindow() const
{
    return m_window;
}

void Camera::SetLevel(Level* _pLevel)
{
    m_level = _pLevel;
}

Level* Camera::GetLevel() const
{
    return m_level;
}

void Camera::SetSFView(const sf::View& _kView)
{
    m_sfView = _kView;
}

const sf::View& Camera::GetSFView() const
{
    return m_sfView;
}

/*void Camera::SetExtendViewOnResize(bool _bExtendOnResize)
{
    m_extendOnResize = _bExtendOnResize;
}*/

/*void Camera::OnSizeChanged(sf::Vector2f _kOldSize)
{
    if (m_extendOnResize)
    {
        sf::Vector2f kComputedSize;

        if (_kOldSize.x != 0.f)
        {
            kComputedSize.x = m_oSFView.getSize().x * GetSize().x / _kOldSize.x;
        }
        else
        {
            kComputedSize.x = GetSize().x;
        }

        if (_kOldSize.y != 0.f)
        {
            kComputedSize.y = m_oSFView.getSize().y * GetSize().y / _kOldSize.y;
        }
        else
        {
            kComputedSize.y = GetSize().y;
        }

        SetSize(kComputedSize);
    }
}*/

void Camera::SetSize(float _fSizeX, float _fSizeY)
{
    SetSize(sf::Vector2f(_fSizeX, _fSizeY));
}

void Camera::SetSize(sf::Vector2f _kSize)
{
    m_sfView.setSize(_kSize);
    ComputeViewCenter();
}

sf::Vector2f Camera::GetSize() const
{
    return m_sfView.getSize();
}

void Camera::SetZoom(float zoom)
{
    m_zoomMultiplier = 1.f / zoom;
    ComputeViewSize();
}

float Camera::GetZoom() const
{
    return 1.f / m_zoomMultiplier;
}

void Camera::SetTarget(float _fTargetX, float _fTargetY)
{
    SetTarget(sf::Vector2f(_fTargetX, _fTargetY));
}

void Camera::SetTarget(sf::Vector2f _kTarget)
{
    m_targetPosition = _kTarget;
    ComputeViewCenter();
}

sf::Vector2f Camera::GetTarget() const
{
    return m_targetPosition;
}

void Camera::SetCenterOnTarget(bool _bCenterOnTarget)
{
    m_centerOnTarget = _bCenterOnTarget;
    ComputeViewCenter();
}

bool Camera::IsCenterOnTarget() const
{
    return m_centerOnTarget;
}

void Camera::SetViewport(const sf::FloatRect& _kViewport)
{
    m_sfView.setViewport(_kViewport);
}

void Camera::ComputeViewSize()
{
    sf::Vector2f kWindowSize = sf::Vector2f(m_window->GetSize());
    const sf::FloatRect& kViewport = m_sfView.getViewport();
    
    SetSize((kWindowSize.x * kViewport.width) * m_zoomMultiplier, (kWindowSize.y * kViewport.height) * m_zoomMultiplier); //Will call ComputeViewCenter
}

void Camera::ComputeViewCenter()
{
    if (m_centerOnTarget)
    {
        m_sfView.setCenter(m_targetPosition);
    }
    else
    {
        m_sfView.setCenter(m_targetPosition + (m_sfView.getSize() / 2.f));
    }
}

sf::Vector2f Camera::GetPickedPosition(const sf::Vector2i& _kMouseCoords) const
{
    return m_window->GetSFRenderWindow()->mapPixelToCoords(_kMouseCoords, m_sfView);
}

bool Camera::IsMouseOverCamera(const sf::Vector2i& _kMouseCoords) const
{
    sf::Vector2f kWindowSize = sf::Vector2f(m_window->GetSize());
    const sf::FloatRect& kViewport = m_sfView.getViewport();
    
    sf::Vector2f kViewportPosition = sf::Vector2f(kWindowSize.x * kViewport.left, kWindowSize.y * kViewport.top);
    sf::Vector2f kViewportSize = sf::Vector2f(kWindowSize.x * kViewport.width, kWindowSize.y * kViewport.height);

    if (IsInRange((float)_kMouseCoords.x, kViewportPosition.x, kViewportPosition.x + kViewportSize.x - 1.f)
    &&  IsInRange((float)_kMouseCoords.y, kViewportPosition.y, kViewportPosition.y + kViewportSize.y - 1.f) )
    {
        return true;
    }

    return false;
}

bool Camera::IsMouseOverElement(const sf::Vector2i& _kMouseCoords, Element* _pElement) const
{
    if (!IsMouseOverCamera(_kMouseCoords))
        return false;

    const Element* pTopParent = _pElement->GetTopParent();
    bool bLevelNode = true;
    bool bWindowNode = true;

    if (m_level)
        bLevelNode = (m_level->GetRootNode() == pTopParent);
    if (m_window)
        bWindowNode = (m_window->GetRootNode() == pTopParent);

    if (!bLevelNode && !bWindowNode)
        return false;

    return _pElement->IsPicked(GetPickedPosition(_kMouseCoords));
}

}   // namespace gugu
