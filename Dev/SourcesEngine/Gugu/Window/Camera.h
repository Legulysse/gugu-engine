#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

#include <SFML/Graphics/View.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Scene;
    class Window;
    class Element;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class Camera
{
public:

    Camera();
    virtual ~Camera();

    void            SetWindow   (Window* _pWindow);
    Window*         GetWindow   () const;
    
    void            SetScene    (Scene* scene);
    Scene*          GetScene    () const;
    
    void            SetSFView   (const sf::View& _kView);
    const sf::View& GetSFView   () const;

    void            RecomputeSizeFromWindow();
    void            SetSize     (float _fSizeX, float _fSizeY);
    void            SetSize     (Vector2f _kSize);
    Vector2f        GetSize     () const;

    //TOOD: handle zoom elsewhere, like in renderer ? applied on a rendertarget ?
    void            SetZoom(float zoom);
    float           GetZoom() const;

    void            SetTarget   (float _fTargetX, float _fTargetY);
    void            SetTarget   (Vector2f _kTarget);
    Vector2f        GetTarget   () const;
    
    //TODO: Use UDim, add shortcuts for Center and UpperLeft
    void            SetCenterOnTarget   (bool _bCenterOnTarget);  //true = target is the center of the view (useful for Scenes), false = target is the top-left corner of the view (default, useful for Window UI)
    bool            IsCenterOnTarget    () const;

    void            SetViewport (const sf::FloatRect& _kViewport);

    Vector2f        GetPickedPosition   (const Vector2i& _kMouseCoords) const;
    bool            IsMouseOverCamera   (const Vector2i& _kMouseCoords) const;
    bool            IsMouseOverElement  (const Vector2i& _kMouseCoords, Element* _pElement) const;
    bool            IsMouseOverElement  (const Vector2i& _kMouseCoords, Element* _pElement, Vector2f& localPickedCoords) const;

protected:

    void            ComputeViewSize();
    void            ComputeViewCenter();

protected:

    sf::View m_sfView;

    //TODO: store only integer size/position ?
    Vector2f m_targetPosition;
    Vector2f m_size;
	float m_zoomMultiplier;
    bool m_centerOnTarget;
    
    Window* m_window;
    Scene* m_scene;
};

}   // namespace gugu
