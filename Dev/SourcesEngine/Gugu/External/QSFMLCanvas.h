#pragma once

#ifdef GUGU_QT

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Window.h"
#include "Gugu/Core/Callback.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <QtWidgets/QWidget>
#include <qtimer>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
public :

    ////////////////////////////////////////////////////////////
    /// Construct the QSFMLCanvas
    ///
    /// \param Parent :    Parent of the widget
    /// \param Position :  Position of the widget
    /// \param Size :      Size of the widget
    /// \param FrameTime : Frame duration, in milliseconds (0 by default)
    ///
    ////////////////////////////////////////////////////////////
    QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 16);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~QSFMLCanvas();

    void SetWindow          (gugu::Window* _pWindow);
    void SetCallbackResize  (gugu::Callback callback);

private :

    ////////////////////////////////////////////////////////////
    /// Notification for the derived class that moment is good
    /// for doing initializations
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnInit();

    ////////////////////////////////////////////////////////////
    /// Notification for the derived class that moment is good
    /// for doing its update and drawing stuff
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnUpdate();

    ////////////////////////////////////////////////////////////
    /// Return the paint engine used by the widget to draw itself
    ///
    ////////////////////////////////////////////////////////////
    virtual QPaintEngine* paintEngine() const;

    ////////////////////////////////////////////////////////////
    /// Called when the widget is shown ;
    /// we use it to initialize our SFML window
    ///
    ////////////////////////////////////////////////////////////
    virtual void showEvent(QShowEvent*);

    ////////////////////////////////////////////////////////////
    /// Called when the widget needs to be painted ;
    /// we use it to display a new frame
    ///
    ////////////////////////////////////////////////////////////
    virtual void paintEvent(QPaintEvent*);

    virtual void resizeEvent(QResizeEvent*);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    QTimer myTimer;       ///< Timer used to update the view
    bool   myInitialized; ///< Tell whether the SFML window has been initialized or not

protected:

    gugu::Window* m_window;
    gugu::Callback m_callbackResize;
};

}   // namespace gugu

#endif
