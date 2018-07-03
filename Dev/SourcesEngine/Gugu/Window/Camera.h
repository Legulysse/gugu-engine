#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/Graphics/View.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Level;
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
    
    void            SetLevel    (Level* _pLevel);
    Level*          GetLevel    () const;
    
    void            SetSFView   (const sf::View& _kView);
    const sf::View& GetSFView   () const;

    void            SetSize     (float _fSizeX, float _fSizeY);
    void            SetSize     (sf::Vector2f _kSize);
    sf::Vector2f    GetSize     () const;

    void            SetTarget   (float _fTargetX, float _fTargetY);
    void            SetTarget   (sf::Vector2f _kTarget);
    sf::Vector2f    GetTarget   () const;
    
    //TODO: Use UDim, add shortcuts for Center and UpperLeft
    void            SetCenterOnTarget   (bool _bCenterOnTarget);  //true = target is the center of the view (useful for Levels), false = target is the top-left corner of the view (default, useful for Window UI)
    bool            IsCenterOnTarget    () const;

    void            SetViewport (const sf::FloatRect& _kViewport);

    void            ComputeViewSize     ();     //Recompute View size, from Viewport and Window size
    void            ComputeViewCenter   ();     //Recompute View center, from Target and View size
    
    sf::Vector2f    GetPickedPosition   (const sf::Vector2i& _kMouseCoords) const;
    bool            IsMouseOverCamera   (const sf::Vector2i& _kMouseCoords) const;
    bool            IsMouseOverElement  (const sf::Vector2i& _kMouseCoords, Element* _pElement) const;

private:

    //void OnPreDraw            (RenderPass& _kRenderPass);
    //void OnPostDraw           (RenderPass& _kRenderPass);

    //void SetSizeImpl        (sf::Vector2f _kOldSize);
    
protected:

    sf::View        m_sfView;
    sf::Vector2f    m_targetPosition;
    bool            m_centerOnTarget;
    
    Window*         m_window;
    Level*          m_level;

    //bool        m_extendOnResize;
};

}   // namespace gugu
