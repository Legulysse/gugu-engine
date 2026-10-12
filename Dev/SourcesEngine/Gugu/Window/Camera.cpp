////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Window/Camera.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Window.h"
#include "Gugu/Scene/Scene.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Math/MathUtility.h"

#include <SFML/Graphics/RenderWindow.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
Camera::Camera()
{
    m_targetPosition = Vector2f(0.f, 0.f);
    m_size = Vector2f(0.f, 0.f);
    m_viewport = sf::FloatRect(Vector2f(0.f, 0.f), Vector2f(1.f, 1.f));
    m_zoomMultiplier = 1.f;
    m_centerOnTarget = false;

    m_window = nullptr;
    m_scene = nullptr;
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

void Camera::SetScene(Scene* scene)
{
    m_scene = scene;
}

Scene* Camera::GetScene() const
{
    return m_scene;
}

const sf::View& Camera::GetSFView() const
{
    return m_sfView;
}

Vector2f Camera::GetViewSize() const
{
    return m_sfView.getSize();
}

void Camera::SetSize(float _fSizeX, float _fSizeY)
{
    SetSize(Vector2f(_fSizeX, _fSizeY));
}

void Camera::SetSize(Vector2f _kSize)
{
    // Reference size of the space rendered by the camera.
    // - This implies that if the viewport does not match the size ratio, the render will be deformed.
    // - Viewport, camera size and rendertarget size need to be in sync for the render to not be deformed.
    // - This allows specific computations like a fixed rendering size.
    m_size = _kSize;
    ComputeViewSize();
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
    SetTarget(Vector2f(_fTargetX, _fTargetY));
}

void Camera::SetTarget(Vector2f _kTarget)
{
    m_targetPosition = _kTarget;
    ComputeViewCenter();
}

Vector2f Camera::GetTarget() const
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

void Camera::SetViewport(const sf::FloatRect& viewport)
{
    m_viewport = viewport;
    ComputeViewSize();
}

const sf::FloatRect& Camera::GetViewport() const
{
    return m_viewport;
}

void Camera::RecomputeSizeFromWindow()
{
    if (m_window)
    {
        SetSize(Vector2f(m_window->GetSize()));
    }
}

void Camera::ComputeViewSize()
{
    // Adjust view size to match the intended zoom multiplier and the target viewport.
    Vector2f adjustedSize(RoundFloor(m_size.x * m_viewport.size.x * m_zoomMultiplier), RoundFloor(m_size.y * m_viewport.size.y * m_zoomMultiplier));
    m_sfView.setSize(adjustedSize);
    m_sfView.setViewport(m_viewport);

    ComputeViewCenter();
}

void Camera::ComputeViewCenter()
{
    if (m_centerOnTarget)
    {
        Vector2f viewSize = m_sfView.getSize();
        Vector2f adjustedTargetPosition(RoundFloor(m_targetPosition.x), RoundFloor(m_targetPosition.y));

        Vector2f offset(
            (int)viewSize.x % 2 == 1 ? 0.5f : 0.f,
            (int)viewSize.y % 2 == 1 ? 0.5f : 0.f
        );

        m_sfView.setCenter(adjustedTargetPosition + offset);
    }
    else
    {
        Vector2f viewSize = m_sfView.getSize();
        Vector2f adjustedTargetPosition(RoundFloor(m_targetPosition.x), RoundFloor(m_targetPosition.y));
        Vector2f adjustedHalfSize(RoundFloor(viewSize.x / 2.f), RoundFloor(viewSize.y / 2.f));

        Vector2f offset(
            (int)viewSize.x % 2 == 1 ? 0.5f : 0.f,
            (int)viewSize.y % 2 == 1 ? 0.5f : 0.f
        );

        m_sfView.setCenter(adjustedTargetPosition + offset + adjustedHalfSize);
    }
}

Vector2i Camera::WorldToScreenPosition(const Vector2f& worldPosition) const
{
    return m_window->GetSFRenderWindow()->mapCoordsToPixel(worldPosition, m_sfView);
}

Vector2f Camera::ScreenToWorldPosition(const Vector2i& screenPosition) const
{
    return m_window->GetSFRenderWindow()->mapPixelToCoords(screenPosition, m_sfView);
}

bool Camera::IsMouseOverCamera(const Vector2i& _kMouseCoords) const
{
    sf::IntRect viewport = m_window->GetSFRenderWindow()->getViewport(m_sfView);
    return viewport.contains(_kMouseCoords);
}

bool Camera::IsMouseOverElement(const Vector2i& _kMouseCoords, Element* _pElement) const
{
    Vector2f localPickedCoords;
    return IsMouseOverElement(_kMouseCoords, _pElement, localPickedCoords);
}

bool Camera::IsMouseOverElement(const Vector2i& _kMouseCoords, Element* _pElement, Vector2f& localPickedCoords) const
{
    if (!IsMouseOverCamera(_kMouseCoords))
        return false;

    const Element* pTopParent = _pElement->GetTopAncestor();
    bool bSceneNode = false;
    bool bWindowNode = false;

    // If this camera is associated to a scene, it should not consider nodes from its associated window.
    if (m_scene)
        bSceneNode = (m_scene->GetRootNode() == pTopParent);
    else if (m_window)
        bWindowNode = (m_window->GetRootNode() == pTopParent);

    if (!bSceneNode && !bWindowNode)
        return false;

    return _pElement->IsPicked(ScreenToWorldPosition(_kMouseCoords), localPickedCoords);
}

}   // namespace gugu
