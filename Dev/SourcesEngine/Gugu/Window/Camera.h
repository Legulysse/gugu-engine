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

    //void SetExtendViewOnResize(bool _bExtendOnResize);
    
    void            SetWindow   (Window* _pWindow);
    Window*         GetWindow   () const;
    
    void            SetScene    (Scene* scene);
    Scene*          GetScene    () const;
    
    void            SetSFView   (const sf::View& _kView);
    const sf::View& GetSFView   () const;

    void            SetSize     (float _fSizeX, float _fSizeY);
    void            SetSize     (Vector2f _kSize);
    Vector2f    GetSize     () const;

    void            SetZoom(float zoom);
    float           GetZoom() const;

    void            SetTarget   (float _fTargetX, float _fTargetY);
    void            SetTarget   (Vector2f _kTarget);
    Vector2f    GetTarget   () const;
    
    //TODO: Use UDim, add shortcuts for Center and UpperLeft
    void            SetCenterOnTarget   (bool _bCenterOnTarget);  //true = target is the center of the view (useful for Scenes), false = target is the top-left corner of the view (default, useful for Window UI)
    bool            IsCenterOnTarget    () const;

    void            SetViewport (const sf::FloatRect& _kViewport);

    void            ComputeViewSize     ();     //Recompute View size, from Viewport and Window size
    void            ComputeViewCenter   ();     //Recompute View center, from Target and View size
    
    Vector2f    GetPickedPosition   (const Vector2i& _kMouseCoords) const;
    bool            IsMouseOverCamera   (const Vector2i& _kMouseCoords) const;
    bool            IsMouseOverElement  (const Vector2i& _kMouseCoords, Element* _pElement) const;

private:

    //void OnPreDraw            (RenderPass& _kRenderPass);
    //void OnPostDraw           (RenderPass& _kRenderPass);

    //void OnSizeChanged        (Vector2f _kOldSize);
    
protected:

    sf::View m_sfView;
	float m_zoomMultiplier;

    Vector2f m_targetPosition;
    bool m_centerOnTarget;
    
    Window* m_window;
    Scene* m_scene;

    //bool m_extendOnResize;
};

}   // namespace gugu
